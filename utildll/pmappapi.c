//==========================================================================\
// pmappapi.c  (c) 1998-1999 * Alessandro Cantatore * Team OS/2 Italy       |
// API creazione applicazioni e finestre parti di applicazioni:
//  |
// funzioni: -------------------------------------------------------------- |
// VOID APIENTRY Wapploop(PAPPDATA pad, PQMSG pqmsg, HWND hsbar, ULONG fl);
// ULONG APIENTRY WcreateApp(PCREATEAPP pca, int argc, char ** argv);
// VOID APIENTRY WdefDlgInit(PWDATA pw);
// MRESULT EXPENTRY WdefDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
// MRESULT EXPENTRY WdefModProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
// MRESULT EXPENTRY WdefObjWinProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
// ULONG WdlgBox(HWND hpar, HWND hown, PMODDLG pm);
// INT APIENTRY WrunApp(PCREATEAPP pca);
//  |
// API: --------------------------------------------------------------------|
//  |
//==========================================================================/

#pragma strings(readonly)

#include "common.h"
#include "pmappapi.h"
#include "api.h"


//==========================================================================\
// Crea un applicazione:
// - alloca la memoria necessaria a memorizzare dati usati dall'applicazione|
// - esegue inizializzazione default:
//   - se flag objwin crea secondo thread con object window
//   - se specificato nome dll la carica e ne memorizza l'handle
//   - ricava anchor block
//   - se specificata flag registra controlli PM
// parametri usati:
// - flag creazione thread secondario e object window
// - nome DLL risorse
// - flag controlli da registrare
// codici messaggi errore:
// 1 : errore allocazione
// 2 : errore creazione thread secondario con object window
// 3 : errore caricamento modulo risorse
// 4 : errore inizializzazione controlli PM estesi
// 5 : errore creazione finestra principale applicazione
// valore restituito:
// in caso di errore flagga il membro rc di APPDATA al valore dell'errore
// e restituisce true per permettere a WrunApp di liberare le risorse
// BOOL: FALSE = errore allocazione APPDATA
//       TRUE l'esito della procedura viene riportato da pca->pad->rc
//==========================================================================/

ULONG APIENTRY WcreateApp(PCREATEAPP pca, int argc, char ** argv) {
   // alloca memoria per struttura dati applicazione
   if (!(pca->pad = (PAPPDATA)malloc(pca->cbad)))
      return WinitErr(APPLERR_ALLOC);

   memset(pca->pad, 0, pca->cbad);
   pca->pw->pad = pca->pad;

   // avvia thread secondario
   if ((pca->fl & APPLS_OBJWIN) && !startObjThrd(pca))
      return WinitErr(APPLERR_THREAD);

   // carica modulo da cui ricava risorse dialoghi, bitmap e stringhe
   if (pca->pszres && DosLoadModule(NULL, 0, pca->pszres, &pca->pad->hres))
      return WinitErr(APPLERR_LOADMOD);

   // ricava anchorblock
   pca->pad->hab = WinInitialize(0);

   // registra classi controlli PM estesi
   if ((pca->fl & APPLS_EXTCTLS) && !InitControls(pca->pad->hab))
      return WinitErr(APPLERR_REGCTLS);

   // se registrata funzione di inizializzazione applicazione la chiama
   // se si verifica un errore Š compito della funzione di inizializzazione
   // registrata mostrare un messaggio opportuno tramite Wprint() e settare
   // al valore opportuno pca->pad->rc
   if (pca->pfinit && !pca->pfinit(argc, argv, pca->pad, pca->pw))
      return pca->pad->rc;

   // crea coda messaggi
   pca->pad->hmq = WinCreateMsgQueue(pca->pad->hab, 0);

   // memorizza indirizzo struttura creazione applicazione in pa che Š
   // dichiarata come variabile globale nella DLL
   pa = pca;

   // crea finestra principale applicazione
   if (pca->fl & APPLS_STDWIN) {
      ;  // sviluppare codice per creare standard window
   } else {
      // messaggio errore all'interno della funzione chiamata
      if (!WloadAppDlg(pca)) return pca->pad->rc;
   } /* endif */
   return TRUE;
}


//==========================================================================\
// esegue inizializzazione default dialogo:                                 |
// - memorizza eventuale handle statusbar
// - setta icona dialogo
// - setta barra menu
// - setta accelleratori
// - aggiorna frame
// - resetta font dialogo
// - calcola dimensioni minime
// - ridimensiona e riposiziona la finestra mostrandola sullo schermo
// parametri:
// PWDATA pw: dati finestra dialogo
// ULONG fl: flag inizializzazion dialogo (stesse usate in CREATEAPP)
// ULONG idres: id risorse (menu, icona, acceleratori) usate nel dialogo
// PSZ pszfont: font da usare nel dialogo
// valore restituito:
// VOID
//==========================================================================/

VOID APIENTRY WdefDlgInit(PWDATA pw) {
   // ottiene handle eventuale statusbar
   if (pw->fcf & FCF_STSBAR)
       pw->hsbar = WinWindowFromID(pw->hwnd, FID_STSBAR);
   // setta icona dialogo: la flag FCF_AUTOICON indica che l'icona Š
   // presente nell'eseguibile stesso con ID 1
   if (pw->fcf & FCF_ICON)
      pw->hico = (pw->fcf & FCF_AUTOICON)?
                 WsetDlgIcon(pw->hwnd, 0, 1):
                 WsetDlgIcon(pw->hwnd, pw->pad->hres, pw->id);
   // setta barra menu
   if (pw->fcf & FCF_MENU)
     pw->hmenu = WinLoadMenu(pw->hwnd, pw->pad->hres, pw->id);
   // setta tabella accelleratori
   if (pw->fcf & FCF_ACCELTABLE)
      pw->hacc = WinLoadAccelTable(pw->pad->hab, pw->pad->hres, pw->id);
   // aggiorna la frame
   if (pw->fcf & FCF_STDDFLAGS)
      WinDefDlgProc(pw->hwnd, WM_UPDATEFRAME,
                    (MPARAM)(pw->fcf & FCF_STDDFLAGS), MPVOID);
   // se necessario setta font dialogo
   if (pw->fcf & FCF_FONT) WsetDlgFonts(pw->hwnd, NULLHANDLE, pw->pszfont);
   // se finestra ridimensionabile calcola dimensioni minime (se non gi… iniz.)
   if ((WinQueryWindowULong(pw->hwnd, QWL_STYLE) & FS_SIZEBORDER) &&
       !pw->smin.cx)
      WsetMinSize(pw);
   // se non ha letto dimensioni-posizione da file inizializzazione
   if (!pw->flswp) {
      if (pw->smin.cx) // se la finestra Š ridimensionabile
         // setta la dimensione inferiore al minimo cos da forzare
         // riposizionamento controlli interni
         WinSetWindowPos(pw->hwnd, 0, 0, 0, 10, 10, SWP_SIZE);
      WcenterWindow(pw->hwnd);   // centra la finestra
   } else {
      WinSetWindowPos(pw->hwnd, HWND_TOP, pw->wr.x, pw->wr.y,
                      pw->wr.cx, pw->wr.cy, pw->flswp);
   } /* endif */
}


//==========================================================================\
// Procedura di default dei dialoghi creati con WloadAppDlg() o con WdlgBox()
//==========================================================================/

MRESULT EXPENTRY WdefDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      // inizializzazione dialogo
      case WM_INITDLG:
         // controlla che non venga chiamata nel contesto sbagliato
         if (pa && pa->pad) {
            INT rc = 0;
            PWDATA pw = (PWDATA)mp2;
            // memorizza nelle window words i dati della finestra stessa
            WinSetWindowPtr(hwnd, 0, mp2);
            pw->hwnd = hwnd;
            // se specificata funzione opzionale la chiama
            if (pw->pfinit && (rc = pw->pfinit(pw)) < 0) {
               WinPostMsg(hwnd, WM_QUIT, MPVOID, MPVOID); // se errore termina
               return FALSE;
            } /* endif */
            WdefDlgInit(pw);
            return (MRESULT)rc;
         } /* endif */
         break;
      // flagga per help contestuale
      case WM_ACTIVATE:
         if (pa && pa->pad) {
            PWDATA pw = (PWDATA)WinQueryWindowPtr(hwnd, 0);
            if (pw) pw->is.active = (BOOL)mp1;
         } /* endif */
         break;
      // solo se finestra ridimensionabile: ----------------------------
      case WM_QUERYTRACKINFO:
         if (pa && pa->pad) {
            PWDATA pw = (PWDATA)WinQueryWindowPtr(hwnd, 0);
            // se la finestra Š ridimensionabile controlla la dimensione minima
            if (pw && pw->smin.cx)
               return WcheckMinSize(hwnd, mp1, mp2, &pw->smin);
         } /* endif */
         break;
      case WM_SETBORDERSIZE:
         if (pa && pa->pad) {
            PWDATA pw = (PWDATA)WinQueryWindowPtr(hwnd, 0);
            // se la finestra Š ridimensionabile aggiorna la dimensione minima
            if (pw && pw->smin.cx)
               return WupdBrdSize(hwnd, mp1, mp2, &pw->smin, &pw->sbrd);
         } /* endif */
         break;
      case WM_CLOSE:
         if (pa && pa->pad) {
            PWDATA pw = (PWDATA)WinQueryWindowPtr(hwnd, 0);
            if (pw && pw->pfend) pw->pfend(pw);
            WinPostMsg(hwnd, WM_QUIT, 0, 0);
            return (MRESULT)FALSE;
         } /* endif */
   } /* endswitch */
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
}


//==========================================================================\
// procedura default dialoghi modali con propria statusbar
//==========================================================================/

MRESULT EXPENTRY WdefModProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      // inizializzazione: prima di chiamare la procedura di default per i
      // dialoghi ottiene l'handle della frame owner figlia del desktop,
      // la disabilita, resetta il testo della statusbar della owner
      // e si aggiunge alla window list
      case WM_INITDLG:
         if (pa && pa->pad && mp2) {
            PMODDLG pm = (PMODDLG)mp2;
            pm->hown = WqueryHwndFrame(pm->hown); // ricava hwnd frame owner
            WinEnableWindow(pm->hown, FALSE);     // disabilita frame owner
            // resetta testo statusbar finestra disattivata
            WinStopTimer(pa->pad->habObj, pa->pad->hObj, TID_CONTEXTHLP);
            WinSetWindowText(pa->pad->hsbar, SZ_NULL);
            // si aggiunge alla windowlist
            if (pm->fcf & FCF_TASKLIST) WaddToWinList(hwnd, pm->psztxt);
            // esegue inizializzazione di default
            return WdefDlgProc(hwnd, msg, mp1, mp2);
         } /* endif */
         return (MRESULT)FALSE;
      // quando si clicca su un bottone termina il loop dei messaggi tramite
      // WM_DLGDISMISS restituendo in mp1 TRUE se il bottone Š pm->idok
      case WM_COMMAND:
         if (pa && pa->pad) {
            PMODDLG pm = (PMODDLG)WinQueryWindowPtr(hwnd, 0L);
            if (pm) {
               WinPostMsg(hwnd, WM_DLGDISMISS,
                          (MPARAM)((USHORT)mp1 == pm->idok), MPVOID);
               return (MRESULT)FALSE;
            } /* endif */
         } /* endif */
         break;
      // alla chiusura richiama la procedura del dialogo con WM_COMMAND e
      // con id del controllo OK o CANCEL secondo il valore restituito da
      // pm->pfend()
      case WM_CLOSE:
         if (pa && pa->pad) {
            PMODDLG pm = (PMODDLG)WinQueryWindowPtr(hwnd, 0L);
            if (pm) {
               pm->pf(hwnd, WM_COMMAND,
                      (MPARAM)(pm->pfend(pm)? pm->idok: pm->idcanc), MPVOID);
               return (MRESULT)FALSE;
            } /* endif */
         } /* endif */
         break;
      default:
         return WdefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return (MRESULT)FALSE;
}


//==========================================================================\
// procedura di default della object window                                 |
// aggiungere variabile per controllare disabilitazione help contestuale
// nel caso in cui l'help venga implementato sotto forma di finestre popup
//==========================================================================/

MRESULT EXPENTRY WdefObjWinProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_CREATE:
         return (MRESULT)!WinSetWindowPtr(hwnd, 0, mp1);
      // l'id della stringa di help contestuale viene memorizzata nelle
      // window words del controllo all'offset 0
      case WM_BUBBLEHELP:
         if (pa && pa->pad) {
            ULONG idstr;
            // se non Š 0 o un indirizzo di memoria in una finestra subclassata
            if ((idstr = WinQueryWindowULong((HWND)mp1, 0L)) < 0xffff && idstr) {
               if (!WgetChlpStr(idstr) &&
                   !WgetTxt(idstr, pa->pad->szhlp, CBSZHLP))
                  *pa->pad->szhlp = 0;
               WinSetWindowText((pa->pad->hsbar = (HWND)mp2), pa->pad->szhlp);
               WinStartTimer(hwnd, pa->pad->hObj, TID_CONTEXTHLP, 500);
            } /* endif */
            if (!idstr)
               WinSetWindowText((pa->pad->hsbar = (HWND)mp2), SZ_NULL);
         } /* endif */
         break;
      // quando la statusbar viene usata per mostrare un messaggio on un
      // errore viene emesso anche un segnale acustico
      case WM_SHOWSBARMSG:
         if (pa && pa->pad) {
            if (!WgetChlpStr((ULONG)mp1 & 0xffff)) *pa->pad->szhlp = 0;
            pa->pad->hsbar = (HWND)mp2;
            pa->pad->hctlhlp = HWND_DESKTOP;
            WinAlarm(HWND_DESKTOP, ((ULONG)mp1) >> 16);
            WinSetWindowText((HWND)mp2, pa->pad->szhlp);
            WinStartTimer(hwnd, pa->pad->hObj, TID_CONTEXTHLP, 10000);
         } /* endif */
         break;
      case WM_TIMER:
         if (pa && pa->pad && (LONG)mp1 == TID_CONTEXTHLP) {
            POINTL ptl;
            WinQueryPointerPos(HWND_DESKTOP, &ptl);
            if (pa->pad->hctlhlp !=
                               WinWindowFromPoint(HWND_DESKTOP, &ptl, TRUE)) {
               pa->pad->hctlhlp = NULLHANDLE;
               WinStopTimer(pa->pad->habObj, hwnd, TID_CONTEXTHLP);
               WinSetWindowText(pa->pad->hsbar, SZ_NULL);
            } // end if
         } // end if
         break;
      default:
         return WinDefWindowProc(hwnd, msg, mp1, mp2);
   } // end switch
   return (MRESULT)FALSE;
}


//==========================================================================\
// crea una finestra di dialogo modale con la sua status bar memorizzando
// l'handle di quest'ultima come statusbar attiva dell'applicazione
// La struttura WDATA deve essere allocata e inizializzata prima della
// chiamata
// parametri:
// HWND hownr: handle owner window
// PFNWP pfnwp: procedura dialogo
// ULONG id: id dialogo
// PVOID pb: indirizzo dati da memorizzare nelle window words del dialogo
// ULONG idbar: id statusbar che verr… resa attiva
//==========================================================================/

ULONG WdlgBox(HWND hpar, HWND hown, PMODDLG pm) {
   QMSG qmsg;
   ULONG rc;
   HWND hsbar;        // handle statusbar precedentemente attiva

   if (!WinLoadDlg(hpar, pm->hown, pm->pf, pa->pad->hres, pm->id, pm))
      return DID_ERROR;

   while (WinGetMsg(pa->pad->hab, &qmsg, NULLHANDLE, 0, 0)) {
      if (qmsg.msg == WM_DLGDISMISS) {
         rc = (ULONG)qmsg.mp1;
         break;
      } else if ((pm->fcf & FCF_STSBAR) &&
                 qmsg.msg == WM_MOUSEMOVE &&         // se MOUSEMOVE
                 pm->is.active &&                    // finestra attiva
                 pa->pad->hctlhlp != HWND_DESKTOP && // messaggio in corso
                 qmsg.hwnd != pa->pad->hctlhlp) {    // nuovo controllo
         pa->pad->hctlhlp = qmsg.hwnd;
         WinPostMsg(pa->pad->hObj, WM_BUBBLEHELP,
                    (MPARAM)qmsg.hwnd, (MPARAM)pm->hsbar);
      } // end if
      WinDispatchMsg(pa->pad->hab, &qmsg);
   } /* endwhile */
   WinSetFocus(HWND_DESKTOP, hown);
   WfreeWres((PWDATA)pm);
   WinEnableWindow(pm->hown, TRUE);
   return rc;
}


//==========================================================================\
// esegue il loop messaggi dell'applicazione e al termine, dopo aver fatto  |
// terminare l'eventuale thread secondario, libera le risorse               |
// parametri:                                                               |
// VOID                                                                     |
// valore restituito:                                                       |
// int: codice restituito da applicazione, tipicamente 0 = successo         |
//==========================================================================/

INT APIENTRY WrunApp(PCREATEAPP pca) {
   QMSG qmsg;
   ULONG rc;
   // se errore inizializzazione esegue direttamente liberazione risorse default
   if (!pca->pad || pca->pad->rc) goto freedefres;
   // esegue loop messaggi
   Wapploop(&qmsg, pca->fl);
   // usa lockxchng() come semaforo mutex per terminare thread object window
   // quando, nel loop dei messaggi della object window lockxchng() restituisce
   // il loop termina e dopo aver liberato le risorse allocate il thread
   // posta un semaforo e termina
   if (pca->fl & APPLS_OBJWIN) {
      while (lockxchng(&pca->pad->ulsem, 1)) DosSleep(1);
      WinWaitEventSem(pca->pad->hevObj, SEM_INDEFINITE_WAIT);
      DosCloseEventSem(pca->pad->hevObj);
   } /* endif */
   // se dichiarata funzione user per la liberazione delle risorse, la chiama
   if (pca->pfterm) pca->pfterm(pca->pad);
   // liberazione risorse standard
freedefres:
   if (pca->pw) WfreeWres(pca->pw);
   if (pca->pad) {
      rc = pa->pad->rc; // eventuale valore di ritorno settato dall'applicazione
      DosFreeModule(pca->pad->hres);
      if (pca->pad->hmq) WinDestroyMsgQueue(pca->pad->hmq);
      if (pca->pad->hab) WinTerminate(pca->pad->hab);
      free(pca->pad);
   } else {
      rc = APPLERR_ALLOC;
   } /* endif */
   return rc;
}


//==========================================================================\
//==========================================================================/

//==========================================================================\
//==========================================================================/

//==========================================================================\
//==========================================================================/

//==========================================================================\
// crea una finestra standard come finestra principale dell'applicazione:
// - crea la frame window usando gli attributi FCF_
// - crea la client window
// la inizializzazione viene eseguita all'intercettazione del WM_CREATE
// della client window
// se la finestra deve essere ridimensionabile, la dimensione specificata
// nel template del dialogo deve essere pari a quella minima prevista per
// il dialogo
// - se specificata funzione inizializzazione UI la chiama
// - se flag statusbar o bubblehelp ottiene handle finestra
// - se flag carica icona
// - se carica menu (deve avere id 1)
// - se carica tabella acceleratori
// - se flag accelleratori o menu aggiorna la frame
// - se specificato font aggiorna parametri presentazione finestra
// - inizializza la dimensione minima della finestra
// - centra finestra o ripristina dimensioni memorizzate
// parametri:
// PCREATEAPP pca: struttura dati creazione applicazione
// BOOL rc: TRUE = successo, FALSE = errore;
//==========================================================================/
//
//BOOL WcreateStdAppWin(PCREATEAPP pca) {
//   FRAMECDATA fcd;
//   HWND hclient;
//   fcd.cb = sizeof(FRAMECDATA);
//   fcd.flCreateFlags = pca->pw->fl;
//   fcd.hmodResources = pca->pad->hres;
//   fcd.idResources = pca->pw->id;
//   // crea frame window
//   pca->pw->hwnd = WinCreateWindow(HWND_DESKTOP, NULLHANDLE,
//                                   pca->pw->pf,
//                   pca->pad->hres, pca->pw->id, pca->pw))
//      return !WinitErr(pca->ppszerr, APPLERR_CREATEWIN);
//   return TRUE;
//}


//===========================================================================
//
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// VOID
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
// VOID
//===========================================================================

HWND APIENTRY WAppInitHelp(PCREATEAPP pca,
                  ULONG idHelpTable, PSZ pszTitle, PSZ pszHelpFile) {
   HELPINIT hi;
   HWND hHelp;
   memset(&hi, 0, sizeof(HELPINIT));
   hi.cb = sizeof(HELPINIT);
   hi.phtHelpTable = PHLPTBLFROMID(idHelpTable);
   hi.hmodHelpTableModule = pca->pad->hres;
   hi.pszHelpWindowTitle = pszTitle;
   hi.fShowPanelId = CMIC_HIDE_PANEL_ID;
   hi.pszHelpLibraryName = pszHelpFile;
   if (NULLHANDLE != (hHelp = WinCreateHelpInstance(pca->pad->hab, &hi))) {
      if (hi.ulReturnCode) {
         WinDestroyHelpInstance(hHelp);
         hHelp = NULLHANDLE;
      } else {
         WinAssociateHelpInstance(hHelp, pca->pw->hwnd);
      } /* endif */
   } /* endif */
   return hHelp;
}
