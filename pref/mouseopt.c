//=========================================================================
// mouseopt.c: pagina impostazioni opzioni mouse
//==========================================================================

#pragma strings(readonly)

#include "pref.h"
#include "api.h"

// prototypes ---------------------------------------------------------------
VOID MouseOptInit(HWND hwnd, POPTIONS pgo);
VOID MouseOptControl(HWND hwnd, ULONG idCtl, ULONG ulEvent);
VOID MouseOptSetAutofocusDep(HWND hwnd, BOOL flag);
VOID MouseOptSetMptrHideDep(HWND hwnd, BOOL flag);
VOID MouseOptSetMptrMove2FocusDep(HWND hwnd, BOOL flag);
VOID MouseOptCommand(HWND hwnd, ULONG idCtl);
VOID MouseOptUndo(HWND hwnd);

//==========================================================================
// procedura dialogo opzioni titlebar hiding
// messaggi:
// WM_INITDLG
// WM_CONTROL
//==========================================================================

MRESULT EXPENTRY MouseOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         p->pd[p->idxPage].hwnd = hwnd;
         MouseOptInit(hwnd, &p->ocur);
         // riattiva finestra principale
         setparown(hwnd);
         break;
      case WM_CONTROL:
         MouseOptControl(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1));
         break;
      case WM_COMMAND:
         MouseOptCommand(hwnd, SHORT1FROMMP(mp1));
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
// PAPPDATA p: indirizzo struttura dati applicazione
// POPTIONS po: indirizzo struttura opzioni globali (ocur=init,
//                                                       ou=undo)
// valore restituito:
// VOID
//==========================================================================
static
VOID MouseOptInit(HWND hwnd, POPTIONS po) {
   // autofocus
   WinCheckButton(hwnd, CHK_MAUTFOCUS, po->mou.focusAuto);
   DlgSpinSetValue(hwnd, SPIN_MAUTFOCUS, po->mou.focusDelay / 10);
   WinCheckButton(hwnd, CHK_MAFIGNDESK, po->mou.focusNoDesk);
   WinCheckButton(hwnd, CHK_MAFNOTOP, po->mou.focusNoTop);
   WinCheckButton(hwnd, CHK_MAFIGNNEW, po->mou.focusNoNew);
   MouseOptSetAutofocusDep(hwnd, po->mou.focusAuto);
   // pointer hiding
   WinCheckButton(hwnd, CHK_MPTRHIDE, po->mou.hide);
   DlgSpinSetValue(hwnd, SPIN_MPTRHIDE, po->mou.hideDelay / 100);
   WinCheckButton(hwnd, CHK_MPHXCLBTN, po->mou.hideNotOnBtn);
   WinCheckButton(hwnd, CHK_MPHXCLMENU, po->mou.hideNotOnMenu);
   MouseOptSetMptrHideDep(hwnd, po->mou.hide);
   // button 3 remap
   WinCheckButton(hwnd, CHK_MBTN3MAP, po->mou.btn3dblClk);
   WinEnableControl(hwnd, CHK_MBTN3MAP, p->is.mou3btns);
   WinCheckButton(hwnd, CHK_MBTN3TBAR, po->mou.btn3title);
   WinEnableControl(hwnd, CHK_MBTN3TBAR, po->mou.btn3dblClk && p->is.mou3btns);
   // move pointer to the focus window
   WinCheckButton(hwnd, CHK_MMOVETOFOCUS, po->mou.moveToFocus);
   WinCheckButton(hwnd, CHK_MMTFWINCREATE, po->mou.moveToNew);
   WinCheckButton(hwnd, CHK_MMTFWINDLG, po->mou.moveToDlg);
   WinCheckButton(hwnd, CHK_MMTFXCLDLGNOBTN, po->mou.moveNotBtnles);
   DlgSpinSetValue(hwnd, SPIN_MMTFSTEPS, po->mou.moveSteps);
   MouseOptSetMptrMove2FocusDep(hwnd, po->mou.moveToFocus);
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
static
VOID MouseOptControl(HWND hwnd, ULONG idCtl, ULONG ulEvent) {
   switch (idCtl) {
      // opzioni autofocus
      case CHK_MAUTFOCUS:           // enable autofocus
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->mou.focusAuto = p->ocur.mou.focusAuto =
                              WinQueryButtonCheckstate(hwnd, CHK_MAUTFOCUS);
            MouseOptSetAutofocusDep(hwnd, p->ocur.mou.focusAuto);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case SPIN_MAUTFOCUS:          // delay autofocus
         if (ulEvent == SPBN_CHANGE) {
            LONG l;
            WinSendDlgItemMsg(hwnd, idCtl, SPBM_QUERYVALUE, (MPARAM)&l,
                              MPFROM2SHORT(0, SPBQ_ALWAYSUPDATE));

            if ((l *= 10) != p->ocur.mou.focusDelay) {
               p->ocur.mou.focusDelay = l;
               EnableToolbarUndoBtn(1);
            } /* endif */
         } else if (ulEvent == SPBN_KILLFOCUS) {
            p->po->mou.focusDelay = p->ocur.mou.focusDelay;
         } /* endif */
         break;
      case CHK_MAFIGNDESK:          // ignore desktop
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->mou.focusNoDesk = p->ocur.mou.focusNoDesk =
                           WinQueryButtonCheckstate(hwnd, CHK_MAFIGNDESK);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_MAFNOTOP:            // don't bring to top
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->mou.focusNoTop = p->ocur.mou.focusNoTop =
                           WinQueryButtonCheckstate(hwnd, CHK_MAFNOTOP);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_MAFIGNNEW:           // ignore new windows
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->mou.focusNoNew = p->ocur.mou.focusNoNew =
                           WinQueryButtonCheckstate(hwnd, CHK_MAFIGNNEW);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      // opzioni pointer hiding
      case CHK_MPTRHIDE:            // abilita pointer hiding
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->mou.hide = p->ocur.mou.hide = WinQueryButtonCheckstate(hwnd, CHK_MPTRHIDE);
            MouseOptSetMptrHideDep(hwnd, p->ocur.mou.hide);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case SPIN_MPTRHIDE:           // ritardo pointer hiding
         if (ulEvent == SPBN_CHANGE) {
            LONG l;
            WinSendDlgItemMsg(hwnd, idCtl, SPBM_QUERYVALUE, (MPARAM)&l,
                              MPFROM2SHORT(0, SPBQ_ALWAYSUPDATE));
            if ((l *= 100) != p->ocur.mou.hideDelay) {
               p->ocur.mou.hideDelay = l;
               EnableToolbarUndoBtn(1);
            } /* endif */
         } else if (ulEvent == SPBN_KILLFOCUS) {
            p->po->mou.hideDelay = p->ocur.mou.hideDelay;
         } /* endif */
         break;
      case CHK_MPHXCLBTN:           // exclude buttons
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->mou.hideNotOnBtn = p->ocur.mou.hideNotOnBtn =
                           WinQueryButtonCheckstate(hwnd, CHK_MPHXCLBTN);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_MPHXCLMENU:          // exclude menu
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->mou.hideNotOnMenu = p->ocur.mou.hideNotOnMenu =
                           WinQueryButtonCheckstate(hwnd, CHK_MPHXCLMENU);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      // opzione remap tasto 3
      case CHK_MBTN3MAP:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->mou.btn3dblClk = p->ocur.mou.btn3dblClk =
                           WinQueryButtonCheckstate(hwnd, CHK_MBTN3MAP);
            WinEnableControl(hwnd, CHK_MBTN3TBAR,
                             p->ocur.mou.btn3dblClk && p->is.mou3btns);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      // opzione include titlebar
      case CHK_MBTN3TBAR:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->mou.btn3title = p->ocur.mou.btn3title =
                           WinQueryButtonCheckstate(hwnd, CHK_MBTN3TBAR);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      // opzione move pointer to focus window
      case CHK_MMOVETOFOCUS:        // move the pointer to the focus window
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->mou.moveToFocus = p->ocur.mou.moveToFocus =
                            WinQueryButtonCheckstate(hwnd, CHK_MMOVETOFOCUS);
            MouseOptSetMptrMove2FocusDep(hwnd, p->ocur.mou.moveToFocus);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case SPIN_MMTFSTEPS:           // ritardo pointer hiding
         if (ulEvent == SPBN_CHANGE) {
            LONG l;
            WinSendDlgItemMsg(hwnd, idCtl, SPBM_QUERYVALUE, (MPARAM)&l,
                              MPFROM2SHORT(0, SPBQ_ALWAYSUPDATE));
            if ((l) != p->ocur.mou.moveSteps) {
               p->ocur.mou.moveSteps = l;
               EnableToolbarUndoBtn(1);
            } /* endif */
         } else if (ulEvent == SPBN_KILLFOCUS) {
            p->po->mou.moveSteps = p->ocur.mou.moveSteps;
         } /* endif */
         break;
      case CHK_MMTFWINCREATE:   // check box
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->mou.moveToNew = p->ocur.mou.moveToNew =
                           WinQueryButtonCheckstate(hwnd, CHK_MMTFWINCREATE);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_MMTFWINDLG:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->mou.moveToDlg = p->ocur.mou.moveToDlg =
                               WinQueryButtonCheckstate(hwnd, CHK_MMTFWINDLG);
            WinEnableControl(hwnd, CHK_MMTFXCLDLGNOBTN, p->ocur.mou.moveToDlg);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_MMTFXCLDLGNOBTN:   // check box
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->mou.moveNotBtnles = p->ocur.mou.moveNotBtnles =
                           WinQueryButtonCheckstate(hwnd, CHK_MMTFXCLDLGNOBTN);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
   } /* endswitch */
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
static
VOID MouseOptCommand(HWND hwnd, ULONG idCtl) {
   switch (idCtl) {
      // bottoni toolbar ---------------------------------------------------
      case BTN_UNDO:
         MouseOptUndo(hwnd);
         break;
      case BTN_DEFAULT:
         MouseOptDefault(hwnd);
         break;
   } /* endswitch */
}


//==========================================================================
// abilita/disabilita controlli secondo settaggio checkbox Enable
// parametri:
// HWND hwnd: handle dialogo
// BOOL flag: TRUE = abilita, FALSE = disabilita
// valore restituito:
// VOID
//==========================================================================
static
VOID MouseOptSetAutofocusDep(HWND hwnd, BOOL flag) {
   WinEnableControl(hwnd, TXT_MAUTFOCUS, flag);
   WinEnableControl(hwnd, SPIN_MAUTFOCUS, flag);
   WinEnableControl(hwnd, CHK_MAFIGNDESK, flag);
   WinEnableControl(hwnd, CHK_MAFNOTOP, flag);
   WinEnableControl(hwnd, CHK_MAFIGNNEW, flag);
}

static
VOID MouseOptSetMptrHideDep(HWND hwnd, BOOL flag) {
   WinEnableControl(hwnd, TXT_MPTRHIDE, flag);
   WinEnableControl(hwnd, SPIN_MPTRHIDE, flag);
   WinEnableControl(hwnd, CHK_MPHXCLBTN, flag);
   WinEnableControl(hwnd, CHK_MPHXCLMENU, flag);
}

static
VOID MouseOptSetMptrMove2FocusDep(HWND hwnd, BOOL flag) {
   WinEnableControl(hwnd, TXT_MMTFSTEPS, flag);
   WinEnableControl(hwnd, SPIN_MMTFSTEPS, flag);
   WinEnableControl(hwnd, CHK_MMTFWINCREATE, flag);
   WinEnableControl(hwnd, CHK_MMTFWINDLG, flag);
   WinEnableControl(hwnd, CHK_MMTFXCLDLGNOBTN, flag &&
                    WinQueryButtonCheckstate(hwnd, CHK_MMTFWINDLG));
}


// undo
static
VOID MouseOptUndo(HWND hwnd) {
   p->pd[p->idxPage].undo = 0;     // disabilita undo
   WinEnableWindow(p->swpCtls[4].hwnd, FALSE);
   // opzioni autofocus
   p->po->mou.focusAuto = p->ocur.mou.focusAuto = p->ou.mou.focusAuto;
   p->po->mou.focusDelay = p->ocur.mou.focusDelay = p->ou.mou.focusDelay;
   p->po->mou.focusNoDesk = p->ocur.mou.focusNoDesk = p->ou.mou.focusNoDesk;
   p->po->mou.focusNoTop = p->ocur.mou.focusNoTop = p->ou.mou.focusNoTop;
   p->po->mou.focusNoNew = p->ocur.mou.focusNoNew = p->ou.mou.focusNoNew;
   // opzioni pointer hiding
   p->po->mou.hide = p->ocur.mou.hide = p->ou.mou.hide;
   p->po->mou.hideDelay = p->ocur.mou.hideDelay = p->ou.mou.hideDelay;
   p->po->mou.hideNotOnMenu = p->ocur.mou.hideNotOnMenu = p->ou.mou.hideNotOnMenu;
   p->po->mou.hideNotOnBtn = p->ocur.mou.hideNotOnBtn = p->ou.mou.hideNotOnBtn;
   // opzione remap tasto 3 mouse
   p->po->mou.btn3dblClk = p->ocur.mou.btn3dblClk = p->ou.mou.btn3dblClk;
   p->po->mou.btn3title = p->ocur.mou.btn3title = p->ou.mou.btn3title;
   // opzione mode to the focus window
   p->po->mou.moveToFocus = p->ocur.mou.moveToFocus = p->ou.mou.moveToFocus;
   p->po->mou.moveToNew = p->ocur.mou.moveToNew = p->ou.mou.moveToNew;
   p->po->mou.moveToDlg = p->ocur.mou.moveToDlg = p->ou.mou.moveToDlg;
   p->po->mou.moveNotBtnles = p->ocur.mou.moveNotBtnles = p->ou.mou.moveNotBtnles;
   p->po->mou.moveSteps = p->ocur.mou.moveSteps = p->ou.mou.moveSteps;
   // aggiorna controlli dialogo
   MouseOptInit(hwnd, &p->ocur);
}


// default

VOID MouseOptDefault(HWND hwnd) {
   EnableToolbarUndoBtn(1);      // abilita undo
   // opzioni autofocus
   p->po->mou.focusAuto = p->ocur.mou.focusAuto = 1;
   p->po->mou.focusDelay = p->ocur.mou.focusDelay = 600;
   p->po->mou.focusNoDesk = p->ocur.mou.focusNoDesk = 1;
   p->po->mou.focusNoTop = p->ocur.mou.focusNoTop = 0;
   p->po->mou.focusNoNew = p->ocur.mou.focusNoNew = 1;
   // pointer hiding
   p->po->mou.hide = p->ocur.mou.hide = 1;
   p->po->mou.hideDelay = p->ocur.mou.hideDelay = 1000;
   p->po->mou.hideNotOnMenu = p->ocur.mou.hideNotOnMenu = 1;
   p->po->mou.hideNotOnBtn = p->ocur.mou.hideNotOnBtn = 1;
   // opzione remap tasto 3 mouse
   p->po->mou.btn3dblClk = p->ocur.mou.btn3dblClk = p->is.mou3btns? 1: 0;
   p->po->mou.btn3title = p->ocur.mou.btn3title = 0;
   // opzione move to the focus window
   p->po->mou.moveToFocus = p->ocur.mou.moveToFocus = 0;
   p->po->mou.moveToNew = p->ocur.mou.moveToNew = 0;
   p->po->mou.moveToDlg = p->ocur.mou.moveToDlg = 0;
   p->po->mou.moveNotBtnles = p->ocur.mou.moveNotBtnles = 0;
   p->po->mou.moveSteps = p->ocur.mou.moveSteps = 30;
   // aggiorna controlli dialogo
   if (hwnd) MouseOptInit(hwnd, &p->ocur);
}
