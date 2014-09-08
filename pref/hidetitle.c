//===========================================================================
// hidetitle.c: pagina impostazioni wrapping
//===========================================================================

#pragma strings(readonly)

#include "pref.h"
#include "api.h"

// prototypes ---------------------------------------------------------------
VOID TitleBarHideInit(HWND hwnd, POPTIONS po);
VOID TitleBarHideControl(HWND hwnd, ULONG idCtl, ULONG ulEvent);
VOID TitleBarHideCommand(HWND hwnd, ULONG idCtl);
VOID TitleBarHideSetMainDep(HWND hwnd, BOOL flag);
VOID TitleBarHideUndo(HWND hwnd);


//==========================================================================
// procedura dialogo opzioni titlebar hiding
// messaggi:
// WM_INITDLG
// WM_COMMAND
// WM_CONTROL
//==========================================================================

MRESULT EXPENTRY TitleBarHideOptProc(HWND hwnd, ULONG msg,
                                     MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         p->pd[p->idxPage].hwnd = hwnd;
         TitleBarHideInit(hwnd, &p->ocur);
         // riattiva finestra principale
         setparown(hwnd);
         break;
      case WM_CONTROL:
         TitleBarHideControl(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1));
         break;
      case WM_COMMAND:
         TitleBarHideCommand(hwnd, SHORT1FROMMP(mp1));
         break;
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return (MRESULT)FALSE;
}


//==========================================================================\
// procedura utilizzata sia per l'inizializzazione del dialogo che per      |
// l'UNDO                                                                   |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// PAPPDATA p: indirizzo struttura dati applicazione                      |
// POPTIONS po: indirizzo struttura opzioni globali (ocur=init,        |
//                                                       ou=undo)           |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/
static
VOID TitleBarHideInit(HWND hwnd, POPTIONS po) {
   HPS hps;
   TitleBarHideSetMainDep(hwnd, po->tbh.on);
   WinCheckButton(hwnd, CHK_HTBARSYSMENU, po->tbh.sysMn);
   hps = WinGetPS(hwnd);
   // inizializza bitmap bottone per nascondere/mostrare barra titolo
   featureBtnSetBmp(hwnd, hps, BMPHTBAR);
   featureBtnSetBmp(hwnd, hps, BMPUNHTBAR);
   WinReleasePS(hps);
   WinCheckButton(hwnd, CHK_HTREDFLDH, po->tbh.redfld);
   WinCheckButton(hwnd, CHK_HTXCLDLGWIN, po->tbh.noDlg);
   WinCheckButton(hwnd, CHK_HTBARINCLMENU, po->tbh.inclMn);
   DlgSpinSetValue(hwnd, SPIN_HTBARPUPDELAY, po->tbh.unhideDelay / 10);
   DlgSpinSetValue(hwnd, SPIN_HTBARHIDEDELAY, po->tbh.hideDelay / 10);
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
VOID TitleBarHideControl(HWND hwnd, ULONG idCtl, ULONG ulEvent) {
   switch (idCtl) {
      case CHK_HTBARENABLE:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->tbh.on = p->ocur.tbh.on =
                           WinQueryButtonCheckstate(hwnd, CHK_HTBARENABLE);
            TitleBarHideSetMainDep(hwnd, p->ocur.tbh.on);
            EnableToolbarUndoBtn(1);
            _stlrMsgPostAll(STLR_REDRAWTITLE, MODIF_TBHIDE | MODIF_TBUNHIDE);
         } /* endif */
         break;
      case CHK_HTBARSYSMENU:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->tbh.sysMn = p->ocur.tbh.sysMn =
                           WinQueryButtonCheckstate(hwnd, CHK_HTBARSYSMENU);
            EnableToolbarUndoBtn(1);
            _stlrMsgPostAll(STLR_EDITSYSMENU, 0);
         } /* endif */
         break;
      case CHK_HTREDFLDH:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->tbh.redfld = p->ocur.tbh.redfld =
                           WinQueryButtonCheckstate(hwnd, CHK_HTREDFLDH);
            EnableToolbarUndoBtn(1);
         } /* endif */
         break;
      case CHK_HTXCLDLGWIN:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->tbh.noDlg = p->ocur.tbh.noDlg =
                           WinQueryButtonCheckstate(hwnd, CHK_HTXCLDLGWIN);
            EnableToolbarUndoBtn(1);
         } /* endif */
         break;
      case CHK_HTBARINCLMENU:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->tbh.inclMn = p->ocur.tbh.inclMn =
                           WinQueryButtonCheckstate(hwnd, CHK_HTBARINCLMENU);
            EnableToolbarUndoBtn(1);
         } /* endif */
         break;
      case SPIN_HTBARPUPDELAY:
         if (ulEvent == SPBN_CHANGE) {
            LONG l;
            WinSendDlgItemMsg(hwnd, idCtl, SPBM_QUERYVALUE, (MPARAM)&l,
                              MPFROM2SHORT(0, SPBQ_ALWAYSUPDATE));
            if ((l *= 10) != p->ocur.tbh.unhideDelay) {
               p->ocur.tbh.unhideDelay = l;
               EnableToolbarUndoBtn(1);
            } /* endif */
         } else if (ulEvent == SPBN_KILLFOCUS) {
            p->po->tbh.unhideDelay = p->ocur.tbh.unhideDelay;
         } /* endif */
         break;
      case SPIN_HTBARHIDEDELAY:
         if (ulEvent == SPBN_CHANGE) {
            LONG l;
            WinSendDlgItemMsg(hwnd, idCtl, SPBM_QUERYVALUE, (MPARAM)&l,
                              MPFROM2SHORT(0, SPBQ_ALWAYSUPDATE));
            if ((l *= 10) != p->ocur.tbh.hideDelay) {
               p->ocur.tbh.hideDelay = l;
               EnableToolbarUndoBtn(1);
            } /* endif */
         } else if (ulEvent == SPBN_KILLFOCUS) {
            p->po->tbh.hideDelay = p->ocur.tbh.hideDelay;
         } /* endif */
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
VOID TitleBarHideCommand(HWND hwnd, ULONG idCtl) {
   switch (idCtl) {
      // bottoni toolbar ---------------------------------------------------
      case BTN_UNDO:
         TitleBarHideUndo(hwnd);
         break;
      case BTN_DEFAULT:
         TitleBarHideDefault(hwnd);
         break;
      // bottoni selezione azioni mouse tastiera
      case BTN_HTBARMOU:
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | FEIDX_HTBAR;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_MOUSE_FUNCS].undo = 1;
            p->pd[IDPG_MOUSE_FUNCS].apply = 0;
         } // end if
         p->flFE = 0;
         break;
      case BTN_HTBARKEYB:
         p->flFE =  FE_FLOATDLG | FEIDX_HTBAR;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_KEYB_FUNCS].undo = 1;
            p->pd[IDPG_KEYB_FUNCS].apply = 0;
         } // end if
         p->flFE = 0;
         break;
      // bottoni barra titolo
      case BTN_HTBARHIDEBTN:           // bottone per nascondere titlebar
         featureBtnSetCmd(hwnd, BMPHTBAR);
         break;
      case BTN_HTBARUNHIDEBTN:         // bottone per ripristinare titlebar
         featureBtnSetCmd(hwnd, BMPUNHTBAR);
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
VOID TitleBarHideSetMainDep(HWND hwnd, BOOL flag) {
   WinCheckButton(hwnd, CHK_HTBARENABLE, flag);
   WinEnableControl(hwnd, CHK_HTBARSYSMENU, flag);
   WinEnableControl(hwnd, BTN_HTBARMOU, flag);
   WinEnableControl(hwnd, BTN_HTBARKEYB, flag);
   WinEnableControl(hwnd, BTN_HTBARHIDEBTN, flag);
   WinEnableControl(hwnd, WSBMP_HIDE, flag);
   WinEnableControl(hwnd, BTN_HTBARUNHIDEBTN, flag);
   WinEnableControl(hwnd, WSBMP_UNHIDE, flag);
   WinEnableControl(hwnd, CHK_HTREDFLDH, flag);
   WinEnableControl(hwnd, CHK_HTXCLDLGWIN, flag);
   WinEnableControl(hwnd, CHK_HTBARINCLMENU, flag);
   WinEnableControl(hwnd, TXT_HTBARPUPDELAY, flag);
   WinEnableControl(hwnd, SPIN_HTBARPUPDELAY, flag);
   WinEnableControl(hwnd, TXT_HTBARHIDEDELAY, flag);
   WinEnableControl(hwnd, SPIN_HTBARHIDEDELAY, flag);
}


// undo
static
VOID TitleBarHideUndo(HWND hwnd) {
   ULONG flModif = 0;
   HPS hps;
   p->pd[p->idxPage].undo = 0;     // disabilita undo
   WinEnableWindow(p->swpCtls[4].hwnd, FALSE);
   // bottone per nascondere e far riapparire barratitolo
   if (p->ocur.tb.htbar != p->ou.tb.htbar) {
      p->po->tb.htbar = p->ocur.tb.htbar = p->ou.tb.htbar;
      flModif |= (MODIF_TBHIDE | MODIF_TBUNHIDE);
   } /* endif */
   // altre opzioni
   if (p->ocur.tbh.on != p->ou.tbh.on) {
      p->po->tbh.on = p->ocur.tbh.on = p->ou.tbh.on;
      flModif |= (MODIF_TBHIDE | MODIF_TBUNHIDE);
   } /* endif */
   p->po->tbh.sysMn = p->ocur.tbh.sysMn = p->ou.tbh.sysMn;
   p->po->tbh.redfld = p->ocur.tbh.redfld = p->ou.tbh.redfld;
   p->po->tbh.noDlg = p->ocur.tbh.noDlg = p->ou.tbh.noDlg;
   p->po->tbh.inclMn = p->ocur.tbh.inclMn = p->ou.tbh.inclMn;
   p->po->tbh.unhideDelay = p->ocur.tbh.unhideDelay = p->ou.tbh.unhideDelay;
   p->po->tbh.hideDelay = p->ocur.tbh.hideDelay = p->ou.tbh.hideDelay;
   // opzioni mouse/tastiera
   p->po->tbh.mou = p->ocur.tbh.mou = p->ou.tbh.mou;
   p->po->tbh.kbd = p->ocur.tbh.kbd = p->ou.tbh.kbd;
   p->po->tbh.moumsg = p->ocur.tbh.moumsg = p->ou.tbh.moumsg;
   p->po->tbh.moukbd = p->ocur.tbh.moukbd = p->ou.tbh.moukbd;
   p->po->tbh.kbdKC = p->ocur.tbh.kbdKC = p->ou.tbh.kbdKC;
   p->po->tbh.kbdVK = p->ocur.tbh.kbdVK = p->ou.tbh.kbdVK;
   // aggiorna settaggi azioni mouse/tastiera nelle pagine notebook
   MouKbdUpdateFElist(p->pd[IDPG_MOUSE_FUNCS].hwnd);
   MouKbdUpdateFElist(p->pd[IDPG_KEYB_FUNCS].hwnd);
   p->hini = openProfile(TRUE);          // apre file inizializzazione
   hps = WinGetPS(hwnd);
   flModif |= undoFrameCtrlBmp(hwnd, hps, BMPHTBAR, MODIF_TBHIDE);
   flModif |= undoFrameCtrlBmp(hwnd, hps, BMPUNHTBAR, MODIF_TBUNHIDE);
   WinReleasePS(hps);
   stlrCloseProfile();
   if (flModif) _stlrMsgPostAll(STLR_REDRAWTITLE, flModif);
   TitleBarHideInit(hwnd, &p->ocur); // aggiorna controlli dialogo
}


// default

VOID TitleBarHideDefault(HWND hwnd) {
   ULONG flModif = 0;
   HPS hps;
   EnableToolbarUndoBtn(1);      // abilita undo
   // bottone per nascondere e far riapparire barratitolo
   if (p->ocur.tb.htbar != 0) {
      p->po->tb.htbar = p->ocur.tb.htbar = 0;
      flModif |= (MODIF_TBHIDE | MODIF_TBUNHIDE);
   } /* endif */
   // altre opzioni
   if (!p->ocur.tbh.on) {
      p->po->tbh.on = p->ocur.tbh.on = 1;
      flModif |= (MODIF_TBHIDE | MODIF_TBUNHIDE);
   } /* endif */
   p->po->tbh.sysMn = p->ocur.tbh.sysMn = 0;
   p->po->tbh.redfld = p->ocur.tbh.redfld = 0;
   p->po->tbh.noDlg = p->ocur.tbh.noDlg = 0;
   p->po->tbh.inclMn = p->ocur.tbh.inclMn = 1;
   p->po->tbh.unhideDelay = p->ocur.tbh.unhideDelay = 600;
   p->po->tbh.hideDelay = p->ocur.tbh.hideDelay = 900;
   // opzioni mouse/tastiera
   p->po->tbh.mou = p->ocur.tbh.mou = 1;
   p->po->tbh.kbd = p->ocur.tbh.kbd = 0;
   p->po->tbh.moumsg = p->ocur.tbh.moumsg = WM_BUTTON1DBLCLK;
   p->po->tbh.moukbd = p->ocur.tbh.moukbd = 0;
   p->po->tbh.kbdKC = p->ocur.tbh.kbdKC = KC_ALT;
   p->po->tbh.kbdVK = p->ocur.tbh.kbdVK = VK_F2 | 0x3c00;
   // aggiorna settaggi azioni mouse/tastiera nelle pagine notebook
   if (p->pd[IDPG_MOUSE_FUNCS].hwnd)
      MouKbdUpdateFElist(p->pd[IDPG_MOUSE_FUNCS].hwnd);
   if (p->pd[IDPG_KEYB_FUNCS].hwnd)
      MouKbdUpdateFElist(p->pd[IDPG_KEYB_FUNCS].hwnd);
   // aggiorna controlli dialogo
   if (hwnd) TitleBarHideInit(hwnd, &p->ocur);
   p->hini = openProfile(TRUE);          // apre file inizializzazione
   hps = WinGetPS(p->hMain);
   flModif |= defaultFrameCtrlBmp(hwnd, hps, BMPHTBAR, MODIF_TBHIDE);
   flModif |= defaultFrameCtrlBmp(hwnd, hps, BMPUNHTBAR, MODIF_TBUNHIDE);
   WinReleasePS(hps);
   stlrCloseProfile();
   if (flModif) _stlrMsgPostAll(STLR_REDRAWTITLE, flModif);
   // aggiorna controlli dialogo
   if (hwnd) TitleBarHideInit(hwnd, &p->ocur);
}
