//==========================================================================
// wroll.c: pagina impostazioni window rolling
//==========================================================================

#pragma strings(readonly)

#include "pref.h"
#include "api.h"

// prototypes ---------------------------------------------------------------
VOID WinRollInit(HWND hwnd, POPTIONS po);
VOID WinRollControl(HWND hwnd, ULONG idCtl, ULONG ulEvent);
VOID WinRollCommand(HWND hwnd, ULONG idCtl);
VOID WinRollSetMainDep(HWND hwnd, BOOL flag);
VOID WinRollUndo(HWND hwnd);


//==========================================================================
// procedura dialogo opzioni titlebar hiding
// messaggi:
// WM_INITDLG
// WM_CONTROL
// WM_COMMAND
//==========================================================================

MRESULT EXPENTRY WinRollOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         p->pd[p->idxPage].hwnd = hwnd;
         WinRollInit(hwnd, &p->ocur);
         // riattiva finestra principale
         setparown(hwnd);
         break;
      case WM_CONTROL:
         WinRollControl(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1));
         break;
      case WM_COMMAND:
         WinRollCommand(hwnd, SHORT1FROMMP(mp1));
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
VOID WinRollInit(HWND hwnd, POPTIONS po) {
   HPS hps;
   WinCheckButton(hwnd, CHK_RLWINSYSMENU, po->roll.sysMn);
   hps = WinGetPS(hwnd);
   // inizializza bitmap bottone per nascondere/mostrare barra titolo
   featureBtnSetBmp(hwnd, hps, BMPROLL);
   featureBtnSetBmp(hwnd, hps, BMPUNROLL);
   WinReleasePS(hps);
   WinCheckButton(hwnd, CHK_AUNRLGETFOCUS, po->roll.unrollOnFocus);
   WinCheckButton(hwnd, CHK_ARLLOSEFOCUS, po->roll.rollOnUnfocus);
   WinCheckButton(hwnd, CHK_AUNRLCLOSEWIN, po->roll.unrollOnClose);
   WinCheckButton(hwnd, CHK_ALLOWVRSZ, po->roll.letVsize);
   WinCheckButton(hwnd, CHK_AUNRLVRSZWIN, po->roll.unrollOnVsize);
   WinCheckButton(hwnd, CHK_RLTOBTOTTOM, po->roll.bottom);
   WinCheckButton(hwnd, CHK_AUNRLCLOSEWIN, po->roll.unrollOnClose);
   WinRollSetMainDep(hwnd, po->roll.on);
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
VOID WinRollControl(HWND hwnd, ULONG idCtl, ULONG ulEvent) {
   switch (idCtl) {
      case CHK_RLWINENABLE:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->roll.on = p->ocur.roll.on =
                           WinQueryButtonCheckstate(hwnd, CHK_RLWINENABLE);
            WinRollSetMainDep(hwnd, p->ocur.roll.on);
            EnableToolbarUndoBtn(1);
            _stlrMsgPostAll(STLR_REDRAWTITLE, MODIF_WROLL | MODIF_WUNROLL);
         } // end if
         break;
      case CHK_RLWINSYSMENU:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->roll.sysMn = p->ocur.roll.sysMn =
                           WinQueryButtonCheckstate(hwnd, CHK_RLWINSYSMENU);
            EnableToolbarUndoBtn(1);
            _stlrMsgPostAll(STLR_EDITSYSMENU, 0);
         } // end if
         break;
      case CHK_AUNRLGETFOCUS:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->roll.unrollOnFocus = p->ocur.roll.unrollOnFocus =
                           WinQueryButtonCheckstate(hwnd, CHK_AUNRLGETFOCUS);
            WinEnableControl(hwnd, CHK_ARLLOSEFOCUS, p->ocur.roll.unrollOnFocus);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_ARLLOSEFOCUS:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->roll.rollOnUnfocus = p->ocur.roll.rollOnUnfocus =
                           WinQueryButtonCheckstate(hwnd, CHK_ARLLOSEFOCUS);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_ALLOWVRSZ:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            ulEvent = WinQueryButtonCheckstate(hwnd, CHK_ALLOWVRSZ);
            p->po->roll.letVsize = p->ocur.roll.letVsize = (ulEvent > 0);
//            p->po->roll.letVsize = p->ocur.roll.letVsize =
//                           WinQueryButtonCheckstate(hwnd, CHK_ALLOWVRSZ);
            WinEnableControl(hwnd, CHK_AUNRLVRSZWIN, p->ocur.roll.letVsize);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_AUNRLVRSZWIN:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->roll.unrollOnVsize = p->ocur.roll.unrollOnVsize =
                           WinQueryButtonCheckstate(hwnd, CHK_AUNRLVRSZWIN);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_RLTOBTOTTOM:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->roll.bottom = p->ocur.roll.bottom =
                           WinQueryButtonCheckstate(hwnd, CHK_RLTOBTOTTOM);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_AUNRLCLOSEWIN:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->roll.unrollOnClose = p->ocur.roll.unrollOnClose =
                           WinQueryButtonCheckstate(hwnd, CHK_AUNRLCLOSEWIN);
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
VOID WinRollCommand(HWND hwnd, ULONG idCtl) {
   switch (idCtl) {
      // bottoni toolbar ---------------------------------------------------
      case BTN_UNDO:
         WinRollUndo(hwnd);
         break;
      case BTN_DEFAULT:
         WinRollDefault(hwnd);
         break;
      // bottoni selezione azioni mouse tastiera
      case BTN_RLWINMOU:
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | FEIDX_ROLL;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_MOUSE_FUNCS].undo = 1;
            p->pd[IDPG_MOUSE_FUNCS].apply = 0;
         } // end if
         p->flFE = 0;
         break;
      case BTN_RLWINKEYB:
         p->flFE =  FE_FLOATDLG | FEIDX_ROLL;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_KEYB_FUNCS].undo = 1;
            p->pd[IDPG_KEYB_FUNCS].apply = 0;
         } // end if
         p->flFE = 0;
         break;
      // bottoni barra titolo
      case BTN_RLWINROLLBTN:
         featureBtnSetCmd(hwnd, BMPROLL);
         break;
      case BTN_RLWINUNROLLBTN:
         featureBtnSetCmd(hwnd, BMPUNROLL);
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
VOID WinRollSetMainDep(HWND hwnd, BOOL flag) {
   WinCheckButton(hwnd, CHK_RLWINENABLE, flag);
   WinEnableControl(hwnd, CHK_RLWINSYSMENU, flag);
   WinEnableControl(hwnd, BTN_RLWINMOU, flag);
   WinEnableControl(hwnd, BTN_RLWINKEYB, flag);
   WinEnableControl(hwnd, BTN_RLWINROLLBTN, flag);
   WinEnableControl(hwnd, WSBMP_ROLL, flag);
   WinEnableControl(hwnd, BTN_RLWINUNROLLBTN, flag);
   WinEnableControl(hwnd, WSBMP_UNROLL, flag);
   WinEnableControl(hwnd, CHK_AUNRLGETFOCUS, flag);
   WinEnableControl(hwnd, CHK_ARLLOSEFOCUS, flag && p->ocur.roll.unrollOnFocus);
   WinEnableControl(hwnd, CHK_ALLOWVRSZ, flag);
   WinEnableControl(hwnd, CHK_AUNRLVRSZWIN, flag && p->ocur.roll.letVsize);
   WinEnableControl(hwnd, CHK_RLTOBTOTTOM, flag);
   WinEnableControl(hwnd, CHK_AUNRLCLOSEWIN, flag);
}


// undo
static
VOID WinRollUndo(HWND hwnd) {
   ULONG flModif = 0;
   HPS hps;
   p->pd[p->idxPage].undo = 0;     // disabilita undo
   WinEnableWindow(p->swpCtls[4].hwnd, FALSE);
   // bottone titlebar per arrotolare/srotolare finestre
   if (p->ou.tb.roll != p->ocur.tb.roll) {
      p->po->tb.roll = p->ocur.tb.roll = p->ou.tb.roll;
      flModif |= (MODIF_WROLL | MODIF_WUNROLL);
   } /* endif */
   // altre opzioni
   if (p->ocur.roll.on != p->ou.roll.on) {
      p->po->roll.on = p->ocur.roll.on = p->ou.roll.on;
      flModif |= (MODIF_WROLL | MODIF_WUNROLL);
   } /* endif */
   p->po->roll.sysMn = p->ocur.roll.sysMn = p->ou.roll.sysMn;
   p->po->roll.unrollOnFocus = p->ocur.roll.unrollOnFocus = p->ou.roll.unrollOnFocus;
   p->po->roll.rollOnUnfocus = p->ocur.roll.rollOnUnfocus = p->ou.roll.rollOnUnfocus;
   p->po->roll.letVsize = p->ocur.roll.letVsize = p->ou.roll.letVsize;
   p->po->roll.unrollOnVsize = p->ocur.roll.unrollOnVsize = p->ou.roll.unrollOnVsize;
   p->po->roll.bottom = p->ocur.roll.bottom = p->ou.roll.bottom;
   p->po->roll.unrollOnClose = p->ocur.roll.unrollOnClose = p->ou.roll.unrollOnClose;
   // opzioni mouse/tastiera
   p->po->roll.mou = p->ocur.roll.mou = p->ou.roll.mou;
   p->po->roll.kbd = p->ocur.roll.kbd = p->ou.roll.kbd;
   p->po->roll.moumsg = p->ocur.roll.moumsg = p->ou.roll.moumsg;
   p->po->roll.moukbd = p->ocur.roll.moukbd = p->ou.roll.moukbd;
   p->po->roll.kbdKC = p->ocur.roll.kbdKC = p->ou.roll.kbdKC;
   p->po->roll.kbdVK = p->ocur.roll.kbdVK = p->ou.roll.kbdVK;
   // aggiorna settaggi azioni mouse/tastiera nelle pagine notebook
   MouKbdUpdateFElist(p->pd[IDPG_MOUSE_FUNCS].hwnd);
   MouKbdUpdateFElist(p->pd[IDPG_KEYB_FUNCS].hwnd);
   p->hini = openProfile(TRUE);          // apre file inizializzazione
   hps = WinGetPS(hwnd);
   flModif |= undoFrameCtrlBmp(hwnd, hps, BMPROLL, MODIF_WROLL);
   flModif |= undoFrameCtrlBmp(hwnd, hps, BMPUNROLL, MODIF_WUNROLL);
   WinReleasePS(hps);
   stlrCloseProfile();
   if (flModif) _stlrMsgPostAll(STLR_REDRAWTITLE, flModif);
   WinRollInit(hwnd, &p->ocur);  // aggiorna controlli dialogo
}


// default

VOID WinRollDefault(HWND hwnd) {
   ULONG flModif = 0;
   HPS hps;
   EnableToolbarUndoBtn(1);      // abilita undo
   // bottone titlebar per arrotolare/srotolare finestre
   if (p->ocur.tb.roll != 1) {
      p->po->tb.roll = p->ocur.tb.roll = 1;
      flModif |= (MODIF_WROLL | MODIF_WUNROLL);
   } /* endif */
   // altre opzioni
   if (!p->ocur.roll.on) {
      p->po->roll.on = p->ocur.roll.on = 1;
      flModif |= (MODIF_WROLL | MODIF_WUNROLL);
   } /* endif */
   p->po->roll.sysMn = p->ocur.roll.sysMn = 0;
   p->po->roll.unrollOnFocus = p->ocur.roll.unrollOnFocus = 1;
   p->po->roll.rollOnUnfocus = p->ocur.roll.rollOnUnfocus = 1;
   p->po->roll.letVsize = p->ocur.roll.letVsize = 1;
   p->po->roll.unrollOnVsize = p->ocur.roll.unrollOnVsize = 1;
   p->po->roll.bottom = p->ocur.roll.bottom = 0;
   p->po->roll.unrollOnClose = p->ocur.roll.unrollOnClose = 1;
   // opzioni mouse/tastiera
   p->po->roll.mou = p->ocur.roll.mou = 1;
   p->po->roll.kbd = p->ocur.roll.kbd = 0;
   p->po->roll.moumsg = p->ocur.roll.moumsg = p->is.mou3btns?
                                          WM_BUTTON3CLICK: WM_BUTTON2DBLCLK;
   p->po->roll.moukbd = p->ocur.roll.moukbd = 0;
   p->po->roll.kbdKC = p->ocur.roll.kbdKC = KC_ALT;
   p->po->roll.kbdVK = p->ocur.roll.kbdVK = VK_F1 | 0x3b00;
   // aggiorna settaggi azioni mouse/tastiera nelle pagine notebook
   if (p->pd[IDPG_MOUSE_FUNCS].hwnd)
      MouKbdUpdateFElist(p->pd[IDPG_MOUSE_FUNCS].hwnd);
   if (p->pd[IDPG_KEYB_FUNCS].hwnd)
      MouKbdUpdateFElist(p->pd[IDPG_KEYB_FUNCS].hwnd);
   p->hini = openProfile(TRUE);          // apre file inizializzazione
   hps = WinGetPS(p->hMain);
   flModif |= defaultFrameCtrlBmp(hwnd, hps, BMPROLL, MODIF_WROLL);
   flModif |= defaultFrameCtrlBmp(hwnd, hps, BMPUNROLL, MODIF_WUNROLL);
   WinReleasePS(hps);
   stlrCloseProfile();
   if (flModif) _stlrMsgPostAll(STLR_REDRAWTITLE, flModif);
   // aggiorna controlli dialogo
   if (hwnd) WinRollInit(hwnd, &p->ocur);
}
