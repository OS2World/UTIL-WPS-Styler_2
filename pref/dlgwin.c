//==========================================================================
// mainwins.c: pagina impostazioni finestre dialogo
//==========================================================================

#pragma strings(readonly)

#include "pref.h"
#include "api.h"

// prototypes ---------------------------------------------------------------
VOID MainWinInit(HWND hwnd, POPTIONS po);
VOID MainWinControl(HWND hwnd, ULONG idCtl, ULONG ulEvent);
VOID MainWinCommand(HWND hwnd, ULONG idCtl);
VOID MainWinSetMainDep(HWND hwnd, POPTIONS po);
VOID MainWinUndo(HWND hwnd);

//==========================================================================
// procedura dialogo opzioni titlebar hiding
//==========================================================================

MRESULT EXPENTRY MainWinOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         p->pd[p->idxPage].hwnd = hwnd;
         MainWinInit(hwnd, &p->ocur);
         // riattiva finestra principale
         setparown(hwnd);
         break;
      case WM_CONTROL:
         MainWinControl(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1));
         break;
      case WM_COMMAND:
         MainWinCommand(hwnd, SHORT1FROMMP(mp1));
         break;
      case WM_STYLER2:
         if ((ULONG)mp1 == STLR_SETFRAMEFONT)
            return (MRESULT)TRUE;
         break;
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return (MRESULT)FALSE;
}


//==========================================================================
// procedura utilizzata sia per l'inizializzazione del dialogo che per
// l'UNDO
// parametri:
// HWND hwnd: handle dialogo
// POPTIONS po: indirizzo struttura opzioni globali (ocur=init,
//                                                       ou=undo)
// valore restituito:
// VOID
//==========================================================================

VOID MainWinInit(HWND hwnd, POPTIONS po) {
   WinCheckButton(hwnd, CHK_MWUSESMWFONT, po->dlg.font);
   WinCheckButton(hwnd, CHK_MWOVERRDPPS, po->dlg.ovrPP);
   _setPPFont(WinWindowFromID(hwnd, BTN_MWFONT), po->dlg.achFont);
   GetStr(ID_FONT, p->buf, 256);
   SetCtlCompTxt(hwnd, BTN_MWFONT, p->buf, po->dlg.achFont);
   MainWinSetMainDep(hwnd, po);
}


//==========================================================================
// processa il messaggio WM_CONTROL
// - se cambiato stato checkbox Enable abilita/disabilita altri controlli
// parametri:
// HWND hwnd: handle dialogo
// ULONG idCtl: identit… controllo
// ULONG ulEvent: identit… evento
// valore restituito:
// VOID
//==========================================================================

VOID MainWinControl(HWND hwnd, ULONG idCtl, ULONG ulEvent) {
   if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
      switch (idCtl) {
         case CHK_MWUSESMWFONT:   // check box "Use Smart Windows Font"
            p->po->dlg.font = p->ocur.dlg.font =
                           WinQueryButtonCheckstate(hwnd, CHK_MWUSESMWFONT);
            EnableToolbarUndoBtn(1);
            MainWinSetMainDep(hwnd, p->po);
            break;
         case CHK_MWOVERRDPPS:    // check box "Override PresParms"
            p->po->dlg.ovrPP = p->ocur.dlg.ovrPP =
                           WinQueryButtonCheckstate(hwnd, CHK_MWOVERRDPPS);
            WinEnableControl(hwnd, CHK_MWIGNOREPP,
                             p->po->dlg.font && p->po->dlg.ovrPP);
            EnableToolbarUndoBtn(1);
            break;
      } // end switch
      _stlrMsgPostAll(STLR_SETFRAMEFONT, 0);
   } // end if
}


//==========================================================================
// processa il messaggio WM_COMMAND visualizzando dialoghi settaggi azioni
// mouse, tastiera o di selezione bitmap bottoni bsrrs titolo
// parametri:
// HWND hwnd: handle dialogo
// ULONG idCtl: identit… controllo
// valore restituito:
// VOID
//==========================================================================

VOID MainWinCommand(HWND hwnd, ULONG idCtl) {
   switch (idCtl) {
      // bottoni toolbar ---------------------------------------------------
      case BTN_UNDO:
         MainWinUndo(hwnd);
         break;
      case BTN_DEFAULT:
         MainWinDefault(hwnd);
         break;
      // bottoni selezione Font
      case BTN_MWFONT: {
         ULONG ul;
         HWND hsmpl = WinWindowFromID(hwnd, BTN_MWFONT);
         if (0 != (ul = setFont(hsmpl,
                                ID_DLGFONT + (ID_FONTSAMPLE << 16),
                                p->ocur.dlg.achFont))) {
            p->po->dlg.cbFont = ul;
            strcpy(p->po->dlg.achFont, p->ocur.dlg.achFont);
            _setPPFont(hsmpl, p->po->dlg.achFont);
            SetCtlCompTxt(hwnd, BTN_MWFONT, (PSZ)ID_FONT, p->po->dlg.achFont);
            _stlrMsgPostAll(STLR_SETFRAMEFONT, 0);
         } /* endif */
      }  break;
   } /* endswitch */
}


//==========================================================================
// abilita/disabilita controlli secondo settaggio checkbox Enable
// parametri:
// HWND hwnd: handle dialogo
// POPTIONS po: indirizzo struttura opzioni globali
// valore restituito:
// VOID
//==========================================================================

VOID MainWinSetMainDep(HWND hwnd, POPTIONS po) {
   WinEnableControl(hwnd, CHK_MWOVERRDPPS, po->dlg.font);
   WinEnableControl(hwnd, BTN_MWFONT, po->dlg.font);
}


// undo

VOID MainWinUndo(HWND hwnd) {
   p->pd[p->idxPage].undo = 0;     // disabilita undo
   WinEnableWindow(p->swpCtls[4].hwnd, FALSE);
   // Use Smart Windows Font
   p->po->dlg.font = p->ocur.dlg.font = p->ou.dlg.font;
   // Override PresParms
   p->po->dlg.ovrPP = p->ocur.dlg.ovrPP = p->ou.dlg.ovrPP;
   // Selected Font
   strcpy(p->po->dlg.achFont, p->ou.dlg.achFont);
   strcpy(p->ocur.dlg.achFont, p->ou.dlg.achFont);
   p->po->dlg.cbFont = strlen(p->po->dlg.achFont) + 1;
   // aggiorna controlli dialogo
   MainWinInit(hwnd, &p->ocur);
   _stlrMsgPostAll(STLR_SETFRAMEFONT, 0);
}


// default

VOID MainWinDefault(HWND hwnd) {
   EnableToolbarUndoBtn(1);      // abilita undo
   // Use Smart Windows Font
   p->po->dlg.font = p->ocur.dlg.font = 0;
   // Override PresParms
   p->po->dlg.ovrPP = p->ocur.dlg.ovrPP = 0;
   // Selected Font
#ifdef STLR_WARP3
   strcpy(p->po->dlg.achFont, "8.Helv");
   strcpy(p->ocur.dlg.achFont, "8.Helv");
#else
   strcpy(p->po->dlg.achFont, "9.WarpSans");
   strcpy(p->ocur.dlg.achFont, "9.WarpSans");
#endif
   p->po->dlg.cbFont = strlen(p->po->dlg.achFont) + 1;
   // aggiorna controlli dialogo
   if (hwnd) MainWinInit(hwnd, &p->ocur);
   _stlrMsgPostAll(STLR_SETFRAMEFONT, 0);
}
