//==========================================================================
// object.c: crea thread secondario con object window
//==========================================================================

#pragma strings(readonly)

// includes -----------------------------------------------------------------

#include "pref.h"
#include "api.h"


// defines ------------------------------------------------------------------

#define THREADSTACKSZ     8192L
#define SEM_TIMEOUT       1000L
#define OBJWINCLASS       "SWPrefObjWin"


// prototypes ---------------------------------------------------------------
VOID _System ObjThreadProc(ULONG ulArg);
MRESULT EXPENTRY ObjWinProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID TerminateObjProc(VOID);
VOID ProcessBubbleHelp(HWND hwnd, HWND hFrom);
VOID ProcessBhlpTimer(HWND hwnd, ULONG ulid);
VOID OWBSDFillBmpList(HWND hwnd, PBMPSELDLG pbsd);
VOID OWBSDImport(HWND hwnd, PBMPSELDLG pbsd);
VOID OWShowError(HWND hwnd, PSZ pszErr);
VOID OWProgress(HWND hwnd, HWND hProgress);


//==========================================================================
// Inizia il thread secondario in cui verr… creata la object window
// parametri:
// PAPPDATA p : struttura dati applicazione
// valore restituito:
// BOOL:    TRUE  = errore
//          FALSE = successo
//==========================================================================

BOOL StartObjectThread(VOID) {
   APIRET rc;
   DosCreateEventSem((PSZ)NULL, &p->hevObj, 0L, FALSE);
   rc = DosCreateThread(&p->tidObj, (PFNTHREAD)ObjThreadProc, (ULONG)p,
                        CREATE_READY | STACK_SPARSE, THREADSTACKSZ);
   if (WinWaitEventSem(p->hevObj, SEM_TIMEOUT)) rc = TRUE;
   DosCloseEventSem(p->hevObj);
   return(BOOL)rc;
}


//==========================================================================
//  |
// parametri:
//  |
// valore restituito:
//==========================================================================

VOID _System ObjThreadProc(ULONG ulArg) {
   PAPPDATA p = (PAPPDATA)ulArg;
   QMSG qmsg;
   p->hmqObj = WinCreateMsgQueue((p->habObj = WinInitialize(0)), 0);
   // solo il thread principale riceve WM_QUIT dovuti allo shutdown
   WinCancelShutdown(p->hmqObj, TRUE);
   // registra classe "TabWinTimeManagement"
   if (!WinRegisterClass(p->habObj, OBJWINCLASS, (PFNWP)ObjWinProc, 0L, 4L) ||
      (NULLHANDLE ==
       (p->hObj = WinCreateWindow(HWND_OBJECT, OBJWINCLASS, NULL,
                                    0, 0, 0, 0, 0, NULLHANDLE, HWND_BOTTOM,
                                    ID_OBJWIN, NULL, NULL))))
      TerminateObjProc();
   p->is.objOK = TRUE;
   DosPostEventSem(p->hevObj);
   // message loop
   while (WinGetMsg(p->habObj, &qmsg, NULLHANDLE, 0, 0) && !p->is.closing)
      WinDispatchMsg(p->habObj, &qmsg);
   p->is.objOK = 0;
   TerminateObjProc();
}


//==========================================================================
// procedura object window
// messaggi:
// WM_CREATE
//
//==========================================================================

MRESULT EXPENTRY ObjWinProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_BUBBLEHELP:
         ProcessBubbleHelp(hwnd, HWNDFROMMP(mp1));
         break;
      case OWM_BSDLBOXFILL:
         OWBSDFillBmpList((HWND)mp1, (PVOID)mp2);
         break;
      case OWM_BSDIMPORT:
         OWBSDImport((HWND)mp1, (PBMPSELDLG)mp2);
         break;
      case OWM_SHOWERROR:
         OWShowError(hwnd, (PSZ)mp1);
         break;
      case WM_PRGRSREADY:   // incremento barra progresso
         OWProgress(hwnd, (HWND)mp1);
         break;
      case WM_TIMER:
         ProcessBhlpTimer(hwnd, LONGFROMMP(mp1));
         break;
      default:
         return WinDefWindowProc(hwnd, msg, mp1, mp2);
   } // end switch
   return (MRESULT)FALSE;
}


//==========================================================================
// termina il thread secondario liberando le risorse e notificando il
// thread principale tramite DosPostEventSem() se la terminazione Š dovuta
// al verificarsi di un errore
// parametri:
// PAPPDATA p: indirizzo struttura dati applicazione
// valore restituito: VOID
//==========================================================================

VOID TerminateObjProc(VOID) {
   if (p->hObj) WinDestroyWindow(p->hObj);
   if (p->hmqObj) WinDestroyMsgQueue(p->hmqObj);
   if (p->habObj) WinTerminate(p->habObj);
   if (!p->is.objOK) DosPostEventSem(p->hevObj);
   p->is.objOK = 0;
   DosExit(EXIT_THREAD, 0L);
}


//==========================================================================
// procedura in risposta all'evento di BUBBLEHELP generato dal movimento
// del mouse sui bottoni superclassati
// parametri:
// HWND hwnd: handle finestra principale dialogo
// HWND hFrom: handle del controllo che ha generato l'evento di BUBBLEHELP
// valore restituito:
// VOID
//==========================================================================

VOID ProcessBubbleHelp(HWND hwnd, HWND hFrom) {
   LONG i;
   if (p->is.closing) return;
   i = WinQueryWindowUShort(hFrom, QWS_ID);
   if (i == 0x29b)          // entryfield di combobox
      i = WinQueryWindowUShort(_winParent(hFrom), QWS_ID);
   if (i < 0 || i >I_LASTCONTEXTHELP) i = 0;
   p->hCtlHelp = hFrom;
   SetWinTxt(p->hSbar, i);
   WinStartTimer(hwnd, p->hObj, TID_CONTEXTHELP, 500);
}


//==========================================================================
// controlla se il mouse Š ancora sul controllo che ha originato il msg
// se Š al di fuori ferma il timer, cancella il testo nella statusbar
// e setta a NULLHANDLE l'handle della finestra originatrice dell'HELP
// parametri:
// HWND hFrom: handle del controllo che ha generato l'evento di BUBBLEHELP
// ULONG ulid: ID timer
// valore restituito:
// VOID
//==========================================================================

VOID ProcessBhlpTimer(HWND hwnd, ULONG ulid) {
   POINTL ptl;
   if (ulid == TID_CONTEXTHELP && p) {
      if (p->is.closing) return;
      WinQueryPointerPos(HWND_DESKTOP, &ptl);
      if (p->hCtlHelp != WinWindowFromPoint(HWND_DESKTOP, &ptl, TRUE)) {
         if (p->hCtlHelp == HWND_DESKTOP)
            WinRemovePresParam(p->hSbar, PP_FOREGROUNDCOLORINDEX);
         p->hCtlHelp = NULLHANDLE;
         WinStopTimer(p->habObj, hwnd, TID_CONTEXTHELP);
         WinSetWindowText(p->hSbar, SZ_NULL);
      } // end if
   } // end if
}


//==========================================================================
// Riempie la listbox con i file di bitmap validi
// parametri:
// HWND hwnd: handle del dialogo
// valore restituito:
// VOID
//==========================================================================

VOID OWBSDFillBmpList(HWND hwnd, PBMPSELDLG pbsd) {
   HDIR hdir = HDIR_CREATE;
   FILEFINDBUF3 FBuf = {0};
   ULONG ul = 1;
   APIRET rc = 0;
   PSZ pszFile;
   HPS hps;
   HBITMAP hbmp;
   SIZEL szl;
   // inizializza indice bmp corrente
   pbsd->idxCurr = pbsd->idxSel = -1;
   // ricava puntatore nomefile
   pszFile = ioFNameFromPath(pbsd->achPath);
   ul = 1;
   // scorre tutti i file *.bmp
   rc = DosFindFirst(pbsd->achPath, &hdir, FILE_NORMAL, &FBuf,
                     sizeof(FILEFINDBUF3), &ul, FIL_STANDARD);
   hps = WinGetPS(hwnd);
   while (!rc) {
      strcpy(pszFile, FBuf.achName);
      if (p->is.bsdend) {
         WinReleasePS(hps);
         DosFindClose(hdir);
         p->is.bsdbusy = 0;
         return;
      }
      // se dati bmp validi aggiunge bmp
      if (ReadBmpFile(pbsd->achPath, pbsd->bmpdata) > 0) {
         // ricava handle bitmap
         hbmp = stlrHbmpFromData(hps, pbsd->bmpdata, NULL);
         // aggiunge item alla listbox
         ul = DlgLboxInsertItem(hwnd, LBOX_BMPSEL,
                                LIT_SORTASCENDING, FBuf.achName);
         // associa l'handle della bitmap all'item della listbox
         DlgLboxSetItemHandle(hwnd, LBOX_BMPSEL, ul, hbmp);
         // se Š la bmp corrente memorizza indice e la seleziona
         if (!strcmp(FBuf.achName, pbsd->achBmp)) {
            pbsd->idxCurr = ul;
            DlgLboxSelectItem(hwnd, LBOX_BMPSEL, ul);
         } // end if
      } // end if
      ul = 1;
      rc = DosFindNext(hdir, &FBuf,  sizeof(FILEFINDBUF3), &ul);
   } /* endwhile */
   WinReleasePS(hps);
   rc = DosFindClose(hdir);
   WinSendDlgItemMsg(hwnd, LBOX_BMPSEL, LM_SETTOPINDEX,
                     (MPARAM)pbsd->idxCurr, NULL);
   repaintWindow(pbsd->ahwnd[0]);
   p->is.bsdbusy = 0;
}


//==========================================================================
// copia un file bmp nel path corrente, richiede conferma se il file gi…
// esiste, mostra msg di errore e non pu• copiarlo
// parametri:
// HWND hwnd: handle dialogo selezione bitmap
// PVOID pb: indirizzo struttura dati dialogo selezione bitmap
// valore restituito:
// VOID
//==========================================================================

VOID OWBSDImport(HWND hwnd, PBMPSELDLG pbsd) {
   PSZ pszEnd = ioFNameFromPath(pbsd->achPath);
   PSZ pszNew;
   APIRET rc;
   BOOL overwritten = 0;
   if (p->is.bsdend) {
      p->is.bsdbusy = 0;
      return;
   }
   // inizializza strutura FILEDLG
   memset(&pbsd->fdlg, 0, sizeof(FILEDLG));
   pbsd->fdlg.cbSize = sizeof(FILEDLG);
   pbsd->fdlg.fl = FDS_CENTER | FDS_OPEN_DIALOG;
   pbsd->fdlg.pszTitle = (PSZ)malloc(256);
   pbsd->fdlg.pszOKButton = (PSZ)malloc(256);
   GetStr(ID_IMPORTBMP, pbsd->fdlg.pszTitle , 256);
   GetStr(ID_IMPORTOK, pbsd->fdlg.pszOKButton , 256);
   if (*p->impBmp)     // inizializza path secondo ultimo path usato
      strcpy(pbsd->fdlg.szFullFile, p->impBmp);
   strcat(pbsd->fdlg.szFullFile, "*.bmp");   // filtra *.bmp
   // se il dialogo file fallisce o si preme cancel termina
   if (!WinFileDlg(HWND_DESKTOP, hwnd, &pbsd->fdlg) ||
       pbsd->fdlg.lReturn != DID_OK) {
      free((PVOID)pbsd->fdlg.pszTitle);
      free((PVOID)pbsd->fdlg.pszOKButton);
      p->is.bsdbusy = 0;
      return;
   } // end if
   *--pszEnd = 0; // ricava path bitmap corrente escludendo ultimo slash
   rc = DosCopy(pbsd->fdlg.szFullFile, pbsd->achPath, 0);
   if (rc == 5) {   // file gi… presente nel path desiderato
      // riusa buffer precedentemente allocati
      GetStr(ID_ASKOVERWRITE, pbsd->fdlg.pszTitle , 256);
      GetStr(ID_ASKCONFIRM, pbsd->fdlg.pszOKButton , 256);
      if (MBID_YES == WinMessageBox(HWND_DESKTOP, hwnd, pbsd->fdlg.pszTitle,
                                    pbsd->fdlg.pszOKButton, 0,
                                    MB_YESNO | MB_WARNING | MB_MOVEABLE)) {
         overwritten = TRUE;
         rc = DosCopy(pbsd->fdlg.szFullFile, pbsd->achPath, DCPY_EXISTING);
      } else {
        *pszEnd++ = '\\';
        return;
      } /* endif */
   } /* endif */
   free((PVOID)pbsd->fdlg.pszTitle);
   free((PVOID)pbsd->fdlg.pszOKButton);
   *pszEnd++ = '\\';
   if (!rc) { // se DosCopy() OK costruisce path nuovo file
      HPS hps = WinGetPS(hwnd);
      pszNew = ioFNameFromPath(pbsd->fdlg.szFullFile);
      strcpy(pszEnd, pszNew);
      // se file Š bmp e < maxsize lo inserisce nella lista
      if ((ReadBmpFile(pbsd->achPath, pbsd->bmpdata) > 0) &&
          ((*((PUSHORT)pbsd->bmpdata) == BFT_BMAP) ||
           (*((PUSHORT)pbsd->bmpdata) == BFT_BITMAPARRAY))) {
         if (overwritten &&
             (pbsd->idxSel = (SHORT)WinSendDlgItemMsg(hwnd, LBOX_BMPSEL,
                                     LM_SEARCHSTRING,
                                     MPFROM2SHORT(0, LIT_FIRST),
                                     pszNew)) >= 0) {
            GpiDeleteBitmap(DlgLboxQueryItemHandle(hwnd, LBOX_BMPSEL,
                            pbsd->idxSel));
         } else {
            pbsd->idxSel = DlgLboxInsertItem(hwnd, LBOX_BMPSEL,
                                             LIT_SORTASCENDING, pszNew);
         } /* endif */
         DlgLboxSetItemHandle(hwnd, LBOX_BMPSEL, pbsd->idxSel,
                              stlrHbmpFromData(hps, pbsd->bmpdata, NULL));
         DlgLboxSelectItem(hwnd, LBOX_BMPSEL, pbsd->idxSel);
         pbsd->idxCurr = -1;
      } else {  // se non Š file bmp valido lo cancella
         DosDelete(pbsd->achPath);
      } // end if
      WinReleasePS(hps);
      *pszNew = 0;
      strcpy(p->impBmp, pbsd->fdlg.szFullFile);
   } else {
      MsgBox(HWND_DESKTOP, ID_ERR_COPYBMP, PMPRNT_ERROR);
   } // end if
   p->is.bsdbusy = 0;
}


//==========================================================================
// - setta handle finestra help contestuale = HWND_DESKTOP
// - cambia colore testo statusbar |
// - imposta testo statusbar a pszErr
// - avvia timer con scadenza 10 secondi
// - emette segnalazione acustica tramite WinALarm()
// parametri:
//
// valore restituito:
//==========================================================================

VOID OWShowError(HWND hwnd, PSZ pszErr) {
   LONG lclr = CLR_RED;
   if (p->is.closing) return;
   p->hCtlHelp = HWND_DESKTOP;
   WinSetPresParam(p->hSbar, PP_FOREGROUNDCOLORINDEX,
                   sizeof(ULONG), (PVOID)&lclr);
   WinSetWindowText(p->hSbar, pszErr);
   WinStartTimer(hwnd, p->hObj, TID_CONTEXTHELP, 10000);
   WinAlarm(HWND_DESKTOP, WA_ERROR);
}


//==========================================================================
//
// parametri:
// HWND hwnd : handle object window
// HWND hProgress : handle progress dialog
// valore restituito:
//==========================================================================

VOID OWProgress(HWND hwnd, HWND hProgress) {
   INT i = 0;
   p->flFE = 0;
   while (i < CPAGES) {
      p->idxPage = i;
      if (p->pd[p->idxPage].defFunc) {
         if (p->idxPage == IDPG_MOUSE_FUNCS) p->flFE = FE_MOUSECLK;
         (*p->pd[p->idxPage].defFunc)(p->pd[p->idxPage].hwnd);
      } // end if
      if (WinSendMsg(hProgress, PRGSM_INCREMENT,
                     (MPARAM)((++i * 1000) / CPAGES), MPVOID))
         break;
   } // end while
   WinPostMsg(hProgress, PRGSM_END, MPVOID, MPVOID);
}

