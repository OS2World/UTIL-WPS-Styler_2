//==========================================================================\
// pmappapi.c  (c) 1998-1999 * Alessandro Cantatore * Team OS/2 Italy       |
// funzioni utilit… relative alla creazione di applicazioni standard
//  |
// funzioni: -------------------------------------------------------------- |
//  |
// API: --------------------------------------------------------------------|
//  |
//==========================================================================/

#pragma strings(readonly)

#include "common.h"
#include "pmappapi.h"
#include "api.h"
#include <stdarg.h>

PCREATEAPP pa;

//==========================================================================\
// richiede dimensioni dati memorizzati nel file ini, allocando la memoria  |
// necessaria per scriverli e restituisce l'indirizzo della memoria,        |
// 0xffffffff in caso di errore o NULL in caso di mancanza dati             |
// Vengono sempre allocati 0x10000 byte in modo che i dati possano essere   |
// editati                                                                  |
// parametri:                                                               |
// HINI hini, PSZ pszApp, PSZ pszKey                                        |
// PULONG pul: dimensione dati trovati                                      |
// valore restituito:                                                       |
// PVOID: 0xffffffff = errore, NULL = no dati, altro = indirizzo memoria    |
//==========================================================================/

PVOID APIENTRY getPrfData(HINI hini, PSZ pszApp, PSZ pszKey, PULONG pul) {
   PVOID pv;
   // SE si Š verificato errore, non dipendente dal fatto che non c'Š alcuna
   // voce in lista rstituisce 0xffffffff
   if (!PrfQueryProfileSize(hini, pszApp, pszKey, pul) &&
       (LOUSHORT(WinGetLastError(pa->pad->hab)) != PMERR_NOT_IN_IDX))
      return GPD_ERROR;
   if (!*pul) return NULL;
   *pul = 0x10000;
   if (!(pv = tsmalloc(*pul))) return GPD_ERROR;
   if(!PrfQueryProfileData(hini, pszApp, pszKey, pv, pul)) {
      tsfree(pv);
      return GPD_ERROR;
   }
   return pv;
}


//==========================================================================\
// ricava il titolo della finestra dall'ID della stringa, setta il testo    |
// della titlebar e aggiunge la finestra alla window list                   |
// parametri:                                                               |
// HWND hwnd: handle finestra da aggiungere alla window list                |
// PSZ psz: se < 0xffff viene interpretato come id stringa altrimenti come  |
//          stringa                                                         |
// valore restituito:                                                       |
// BOOL rc: FALSE/TRUE = errore/successo                                    |
//==========================================================================/

BOOL APIENTRY WaddToWinList(HWND hwnd, PSZ psz) {
   PSWCNTRL pswctl;
   PID pid;
   TID tid;
   BOOL rc = FALSE;
   ULONG id = 0;
   PSZ psztitle;
   if (!psz || !(pswctl = malloc(sizeof(SWCNTRL)))) return FALSE;
   memset(pswctl, 0, sizeof(SWCNTRL));
   // se il parametro rappresenta l'identit… di una stringa la ricava dalla dll
   if ((id = (ULONG)psz) < 0xffff) {
      if (!(psz = malloc(256))) goto end;
      memset(psz, 0, 256);
      WgetStr(id, psz, 256);
   } /* endif */
   WinQueryWindowProcess(hwnd, &pid, &tid);
   WinSetDlgItemText(hwnd, FID_TITLEBAR, psz);
   // si aggiunge alla window list
   pswctl->hwnd = hwnd;
   pswctl->idProcess = pid;
   pswctl->uchVisibility = SWL_VISIBLE;
   pswctl->fbJump = SWL_JUMPABLE;
   strncpy(pswctl->szSwtitle, psz, MAXNAMEL);
   pswctl->bProgType = PROG_DEFAULT;
   rc = (BOOL)WinAddSwitchEntry(pswctl);
   if (id) free(psz);
end:
   free(pswctl);
   return rc;
}


//==========================================================================\
// modifica la struttura TRACKINFO per controllare la dimensione minima     |
// finestra. I membri di ps devono essere aggiornati subito dopo            |
// la creazione del dialogo con la dimensione iniziale del dialogo che deve |
// essere pari alla dimensione minima del dialogo                           |
// parametri:                                                               |
// HWND, MPARAM, MPARAM                                                     |
// PSSIZE ps: struttura dimensioni minime finestra                          |
// valore restituito:                                                       |
// MRESULT mr restituito dalla WinDefDlgProc()                              |
//==========================================================================/

MRESULT APIENTRY WcheckMinSize(HWND hwnd, MPARAM mp1, MPARAM mp2, PSSIZE ps) {
   MRESULT mr = WinDefDlgProc(hwnd, WM_QUERYTRACKINFO, mp1, mp2);
   if (WS_MINIMIZED & WinQueryWindowULong(hwnd, QWL_STYLE)) return mr;
   ((PTRACKINFO)mp2)->ptlMinTrackSize.x = ps->cx;
   ((PTRACKINFO)mp2)->ptlMinTrackSize.y = ps->cy;
   return mr;
}
 

//==========================================================================\
// Copia cstr stringhe in trg concatenandole                                |
// Restituisce un puntatore alla fine della stringa.                        |
// Se l'indirizzo di una stringa Š minore di 0xffff lo considera l'id di    |
// una stringa che deve essere ricavata dalla dll delle risorse             |
// parametri:                                                               |
// PSZ trg: stringa target                                                  |
// INT cstr: numero argomenti variabili                                     |
// valore restituito:                                                       |
// PSZ p: indirizzo fine stringa                                            |
//==========================================================================/

PSZ APIENTRY Wcpycat(PSZ trg, INT cstr, ...) {
   va_list ap;
   PSZ p, parg;
   CHAR buf[256];
   va_start(ap, cstr);
   for (p = trg; cstr--; ) {
      parg = va_arg(ap, PSZ);
      if ((ULONG)parg < 0xffff) {
         WgetStr((ULONG)parg, buf, 256);
         p = pszcpy(p, buf);
      } else {
         p = pszcpy(p, parg);
      } /* endif */
   } /* endfor */
   va_end(ap);
   return p;
}


//==========================================================================\
// Alloca la struttura FILEDLG settandone opportunamente i membri. Se non   |
// ci sono errori e il file viene chiuso tramite il bottone OK, restituisce |
// l'indirizzo della struttura stessa, che dovr… essere liberata dopo l'uso,|
// oppure NULL in caso di errore.                                           |
// parametri:                                                               |
// HWND hwnd: handle owner window                                           |
// PSZ pszFile: in/out specifiche file da ricercare/nome file trovato       |
// PSZ pszext: eventuale maschera (es: *.txt) che viene sostituita al nome  |
//             file in pszFile all'apertura del dialogo se non NULL         |
// ULONG idTitle: titolo dialogo (se 0 usa default)                         |
// ULONG idBtn: titolo bottone (se 0 usa default)                           |
// ULONG fl: FDS_* flag file dialog                                         |
//           3 configurazioni tipiche:                                      |
//           FDLGS_OPEN                                                     |
//           FDLSG_OPENMULTI                                                |
//           FDLGS_SAVE                                                     |
// valore restituito:                                                       |
// PFILEDLG struttura file dialog allocata o NULL se errore o si preme cancel
//==========================================================================/

PFILEDLG APIENTRY Wfdlg(HWND hwnd, PSZ pszFile, PSZ pszext,
                        ULONG idTitle, ULONG idBtn, ULONG fl) {
   PFILEDLG pfdlg = NULL;
   PSZ pszret = NULL;
   if (!(pfdlg = (PFILEDLG)malloc(sizeof(FILEDLG)))) goto end;
   memset((PVOID)pfdlg, 0, sizeof(FILEDLG));
   if ((idTitle && !(pfdlg->pszTitle = (PSZ)malloc(256))) ||
       (idBtn && !(pfdlg->pszOKButton = (PSZ)malloc(256)))) goto end;
   pfdlg->cbSize = sizeof(FILEDLG);
   pfdlg->fl = FDS_CENTER | fl;
   if (idTitle && !WgetStr(idTitle, pfdlg->pszTitle, 256))
      free(pfdlg->pszTitle), pfdlg->pszTitle = NULL;
   if (idBtn && !WgetStr(idBtn, pfdlg->pszOKButton, 256))
      free(pfdlg->pszOKButton), pfdlg->pszOKButton = NULL;
   strcpy(pfdlg->szFullFile, pszFile);
   if (pszext) strcpy(ioFNameFromPath(pfdlg->szFullFile), pszext);
   if (WinFileDlg(HWND_DESKTOP, hwnd, pfdlg) &&
       pfdlg->lReturn == DID_OK) {
      free(pfdlg->pszTitle);
      free(pfdlg->pszOKButton);
      return pfdlg;
   } // end if
end:
   if (pfdlg) {
      free(pfdlg->pszTitle);
      free(pfdlg->pszOKButton);
      free(pfdlg);
   } /* endif */
   return NULL;
}


//==========================================================================\
// restituisce la stringa idstr ricavandola dalla DLL delle risorse della   |
// corrente applicazione                                                    |
// parametri:                                                               |
// ULONG idstr: id stringa da restituire                                    |
// valore restituito:                                                       |
// ULONG cb: il numero dei caratteri copiati nel buffer                     |
//==========================================================================/

ULONG APIENTRY WgetChlpStr(ULONG idstr) {
   if (!(pa && pa->pad)) return 0;
   return WinLoadString(pa->pad->hab, pa->pad->hres, idstr,
                        256, pa->pad->szhlp);
}


//==========================================================================\
// restituisce l'indirizzo della struttura dati creazione applicazione      |
// parametri:                                                               |
// VOID                                                                     |
// valore restituito:                                                       |
// PCREATEAPP pa;                                                           |
//==========================================================================/

PCREATEAPP APIENTRY WgetCreationData(VOID) {
   return pa;
}


//==========================================================================\
// restituisce la stringa idstr ricavandola dalla DLL delle risorse della   |
// corrente applicazione e copiandola nel buffer puntato da psz             |
// parametri:                                                               |
// ULONG idstr: id stringa da restituire                                    |
// PSZ psz: indirizzo buffer                                                |
// ULONG cbpsz: dimensione buffer                                           |
// valore restituito:                                                       |
// ULONG cb: il numero dei caratteri copiati nel buffer                     |
//==========================================================================/

ULONG APIENTRY WgetStr(ULONG idstr, PSZ psz, ULONG cbpsz) {
   if (!(pa && pa->pad)) return 0;
   return WinLoadString(pa->pad->hab, pa->pad->hres, idstr, cbpsz, psz);
}


//==========================================================================\
// Ricava un testo che pu• essere pi— lungo di 256 byte dalla risorsa       |
// RCLONGSTRING definita come 301                                           |
// parametri:                                                               |
// ULONG idstr: id stringa da restituire                                    |
// PSZ psz: indirizzo buffer                                                |
// ULONG cbpsz: dimensione buffer                                           |
// valore restituito:                                                       |
// ULONG cb: il numero dei caratteri copiati nel buffer                     |
//==========================================================================/

ULONG APIENTRY WgetTxt(ULONG idstr, PSZ psz, ULONG cbpsz) {
   PSZ pszres;
   ULONG cb, cbret;
   if (DosGetResource(pa->pad->hres, RCLONGSTRING, idstr, (PPVOID)&pszres))
      return 0;
   cb = strlen(pszres) + 1;    // lunghezza stringa incluso 0 terminazione
   if (cbpsz < cb) {
      cbret = cb = cbpsz - 1;
      psz[cb] = 0;
   } else {
      cbret = cb - 1;
   } /* endif */
   memcpy(psz, pszres, cb);
   DosFreeResource(pszres);
   return cbret;
}


//==========================================================================\
// Se si verifica un errore durante l'inizializzazione del programma mostra |
// una message box ricavando il messaggio di errore dal relativo membro     |
// della struttura CREATEAPP.                                               |
//==========================================================================/

ULONG APIENTRY WinitErr(ULONG ulerr) {
   if (ulerr) Wprint(HWND_DESKTOP, pa->ppszerr[ulerr], PMPRNT_WININIT);
   if (pa->pad && ulerr) pa->pad->rc = ulerr;
   return ulerr;
}


//==========================================================================\
// mostra un massimo di 256 byte di testo ricavandolo da una risorsa stringa|
// della dll delle risorse.                                                 |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idstr: id stringa da mostrare                                      |
// ULONG fl: stile message box                                              |
// valore restituito:                                                       |
// BOOL rc: il valore restituito da Wprint                                  |
//==========================================================================/

BOOL APIENTRY WmsgBox(HWND hwnd, ULONG idstr, ULONG fl) {
   CHAR buf[256];
   ULONG cch = WgetStr(idstr, buf, 256);
   if (cch)
      cch = Wprint(hwnd, buf, fl);
   else
      WinAlarm(HWND_DESKTOP, WA_ERROR);
   return (BOOL)cch;
}


//==========================================================================\
// mostra un massimo di 64 KB di testo ricavandolo dalla risorsa            |
// RCLONGSTRING della dll delle risorse.                                    |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idstr: id stringa da mostrare                                      |
// ULONG fl: stile message box                                              |
// valore restituito:                                                       |
// BOOL rc: il valore restituito da Wprint                                  |
//==========================================================================/

BOOL APIENTRY WmsglBox(HWND hwnd, ULONG idstr, ULONG fl) {
   PSZ pszres;
   ULONG cb;
   if (DosGetResource(pa->pad->hres, RCLONGSTRING, idstr, (PPVOID)&pszres)) {
      WinAlarm(HWND_DESKTOP, WA_ERROR);
      return 0;
   } /* endif */
   cb = strlen(pszres);
   if (cb) cb = Wprint(hwnd, pszres, fl);
   DosFreeResource(pszres);
   return (BOOL)cb;
}


//==========================================================================\
// inizializza dimensioni minime dialogo in base alla dimensione di default |
// e memorizza spessore bordi dialogo                                       |
// parametri:                                                               |
// PWDATA pw: dati finestra                                                 |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID APIENTRY WsetMinSize(PWDATA pw) {
   // inizializza dimensioni minime dialogo
   RECTL rcl;
   pw->sbrd.cx = uGetSval(SV_CXSIZEBORDER);
   pw->sbrd.cy = uGetSval(SV_CYSIZEBORDER);
   WinQueryWindowRect(pw->hwnd, &rcl);
   pw->smin.cx = rcl.xRight;
   pw->smin.cy = rcl.yTop;
}


//==========================================================================\
// Imposta il testo di una finestra come:                                   |
// "pszTerm" "pszData"                                                      |
// se pszData Š NULL lo sostituisce con "-"                                 |
// parametri:                                                               |
// PAPPDATA pad: struttura dati applicazione                                |
// HWND hwnd: handle finestra                                               |
// ULONG ids: id stringa (font/bitmap)                                      |
// PSZ pszData: dati (es. "5.System VIO", "tile.bmp")                       |
// Nota: la lunghezza totale delle 2 stringhe non deve superare i 255 byte  |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID APIENTRY WsetWcompTxt(HWND hwnd, ULONG ids, PSZ pszData) {
   CHAR buf[256];
   WgetStr(ids, buf, 256);
   strcat(buf, (*pszData? pszData: "-"));
   WinSetWindowText(hwnd, buf); 
}


//==========================================================================\
// setta il testo di una finestra ricavandolo da una risorsa stringa        |
// della DLL delle risorse.                                                 |
// restituisce il valore restituito da WgetStr                              |
// parametri:                                                               |
// HWND hwnd: handle finestra                                               |
// ULONG idstr: id stringa da usare come testo per la finestra              |
// valore restituito:                                                       |
// ULONG cb: numero caratteri ottenuti da WgetStr()                         |
//==========================================================================/

ULONG APIENTRY WsetWinTxt(HWND hwnd, ULONG idstr) {
   CHAR buf[256];
   ULONG cch = WgetStr(idstr, buf, 256);
   WinSetWindowText(hwnd, (cch? buf: SZ_NULL));
   return cch;
}


//==========================================================================\
// Setta il testo di una finestra ricavandolo da una risorsa                |
// RCLONGSTRING della DLL delle risorse                                     |
// Restituisce la lunghezza del testo.                                      |
// parametri:                                                               |
// HWND hwnd: handle finestra                                               |
// ULONG idstr: id stringa da usare come testo per la finestra              |
// valore restituito:                                                       |
// ULONG cb: numero caratteri ottenuti da WgetStr()                         |
//==========================================================================/

ULONG APIENTRY WsetWinTxtl(HWND hwnd, ULONG idstr) {
   PSZ pszres;
   ULONG cb;
   if (DosGetResource(pa->pad->hres, RCLONGSTRING, idstr, (PPVOID)&pszres)) {
      pszres = SZ_NULL;
      cb = 0;
   } else {
      cb = strlen(pszres);
   } /* endif */
   WinSetWindowText(hwnd, pszres);
   DosFreeResource(pszres);
   return cb;
}


//==========================================================================\
// apre o porta in primo piano una finestra di un file inf passandogli come |
// parametro la stringa opportuna                                           |
// parametri:                                                               |
// HWND hwnd: finestra che riceve notificazione alla chiusura del file INF  |
// ULONG idinf: id stringa nome file inf                                    |
// ULONG idarg: id stringa che contiene argomento da visualizzare           |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

BOOL APIENTRY WshowInf(HWND hwnd, ULONG idinf, ULONG idarg) {
   CHAR buf[256];
   ULONG cb = 256;
   return (0 != (cb = WgetStr(idinf, buf, cb)) &&
          WgetStr(idarg, buf + cb, 256 - cb) &&
          (BOOL)WstartApp(hwnd, "VIEW.EXE", buf, NULL, SAF_INSTALLEDCMDLINE));
}


//==========================================================================\
// mostra messaggio per 10 secondi nella statusbar generando un WA_NOTE,    |
// WA_WARNING o WA_ERROR                                                    |
// parametri:                                                               |
// ULONG iderr: id stringa messaggio                                        |
// ULONG fl: id messaggio acustico (WA_WARNING, WA_NOTE, WA_ERROR)          |
// HWND hsbar: handle statusbar                                             |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID APIENTRY WshowMsg(ULONG idmsg, BOOL fl, HWND hsbar) {
   if (!(pa && pa->pad)) return;
   idmsg |= fl << 16;
   WinPostMsg(pa->pad->hObj, WM_SHOWSBARMSG, (MPARAM)idmsg, (MPARAM)hsbar);
}


//==========================================================================\
// aggiorna dimensioni minime dialogo a seguito cambiamento spessore bordo  |
// finestra                                                                 |
// parametri:                                                               |
// HWND, MPARAM                                                             |
// PSSIZE ps = dimensioni minime finestra                                   |
// PSSIZE pb = dimensioni bordo finestra                                    |
// valore restituito:                                                       |
// MRESULT restituito dalla WinDefDlgProc()                                 |
//==========================================================================/

MRESULT APIENTRY WupdBrdSize(HWND hwnd, MPARAM mp1, MPARAM mp2,
                             PSSIZE ps, PSSIZE pb) {
   // reinizializza dimensioni minime dialoghi
   ps->cx += (LONG)mp1 - pb->cx;
   ps->cy += (LONG)mp2 - pb->cy;
   // reinizializza dimensioni bordi
   pb->cx = (LONG)mp1;
   pb->cy = (LONG)mp2;
   return WinDefDlgProc(hwnd, WM_SETBORDERSIZE, mp1, mp2);
}


//==========================================================================\
//==========================================================================/
//==========================================================================\
//==========================================================================/
//==========================================================================\
//==========================================================================/
//==========================================================================\
//==========================================================================/


