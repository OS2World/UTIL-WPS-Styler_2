//==========================================================================
// txtctlgen.c: pagina impostazioni generiche controlli testo
//==========================================================================

#pragma strings(readonly)

#include "pref.h"
#include "api.h"

// prototypes ---------------------------------------------------------------
VOID TxtCtlGenInit(HWND hwnd, POPTIONS pgo);
VOID TxtCtlGenControl(HWND hwnd, ULONG idCtl, ULONG ulEvent);
VOID TxtCtlGenCommand(HWND hwnd, ULONG idCtl);
VOID TxtCtlGenSetDep(HWND hwnd, BOOL flag);
VOID TxtCtlGenUndo(HWND hwnd);
ULONG skipmoumsg(ULONG msg);


//==========================================================================
// procedura dialogo opzioni titlebar hiding
//==========================================================================

MRESULT EXPENTRY TxtCtlGenOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         p->pd[p->idxPage].hwnd = hwnd;
         TxtCtlGenInit(hwnd, &p->ocur);
         // riattiva finestra principale
         setparown(hwnd);
         break;
      case WM_CONTROL:
         TxtCtlGenControl(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1));
         break;
      case WM_COMMAND:
         TxtCtlGenCommand(hwnd, SHORT1FROMMP(mp1));
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
VOID TxtCtlGenInit(HWND hwnd, POPTIONS po) {
   WinCheckButton(hwnd, CHK_TCGEFAUTOSEL, po->txt.efsel);
   WinEnableControl(hwnd, BTN_TCGEFAUTOSEL, po->txt.efsel);
   SetMouEventTxt(ID_TCGEFAUTOSELBY, po->txt.selMoumsg,po->txt.selMoukbd,
                  hwnd, BTN_TCGEFAUTOSEL);
   WinCheckButton(hwnd, CHK_TCGMOUCPYPST, po->txt.on);
   TxtCtlGenSetDep(hwnd, po->txt.on);
   WinCheckButton(hwnd, CHK_TCGMCPYEF, po->txt.ef);
   WinCheckButton(hwnd, CHK_TCGMCPYMLE, po->txt.mle);
   WinCheckButton(hwnd, CHK_TCGMCPYSTXT, po->txt.stat);
   WinCheckButton(hwnd, CHK_TCGMCPYLBOX, po->txt.lbox);
   WinCheckButton(hwnd, CHK_TCGMCPYBTN, po->txt.btn);
   SetMouEventTxt(ID_TCGCPYBY, po->txt.cpyMoumsg, po->txt.cpyMoukbd,
                  hwnd, BTN_TCGCPYBY);
   SetMouEventTxt(ID_TCGCUTBY, po->txt.cutMoumsg, po->txt.cutMoukbd,
                  hwnd, BTN_TCGCUTBY);
   SetMouEventTxt(ID_TCGPSTINSBY, po->txt.insMoumsg, po->txt.insMoukbd,
                  hwnd, BTN_TCGPSTINSBY);
   SetMouEventTxt(ID_TCGOVRWRTBY, po->txt.ovwMoumsg, po->txt.ovwMoukbd,
                  hwnd, BTN_TCGOVRWRTBY);
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
VOID TxtCtlGenControl(HWND hwnd, ULONG idCtl, ULONG ulEvent) {
   switch (idCtl) {
      case CHK_TCGEFAUTOSEL:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->txt.efsel = p->ocur.txt.efsel =
                           WinQueryButtonCheckstate(hwnd, CHK_TCGEFAUTOSEL);
            WinEnableControl(hwnd, BTN_TCGEFAUTOSEL, p->ocur.txt.efsel);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_TCGMOUCPYPST:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->txt.on = p->ocur.txt.on =
                           WinQueryButtonCheckstate(hwnd, CHK_TCGMOUCPYPST);
            TxtCtlGenSetDep(hwnd, p->ocur.txt.on);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_TCGMCPYEF:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->txt.ef = p->ocur.txt.ef =
                           WinQueryButtonCheckstate(hwnd, CHK_TCGMCPYEF);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_TCGMCPYMLE:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->txt.mle = p->ocur.txt.mle =
                           WinQueryButtonCheckstate(hwnd, CHK_TCGMCPYMLE);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_TCGMCPYSTXT:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->txt.stat = p->ocur.txt.stat =
                           WinQueryButtonCheckstate(hwnd, CHK_TCGMCPYSTXT);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_TCGMCPYLBOX:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->txt.lbox = p->ocur.txt.lbox =
                           WinQueryButtonCheckstate(hwnd, CHK_TCGMCPYLBOX);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_TCGMCPYBTN:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->txt.btn = p->ocur.txt.btn =
                           WinQueryButtonCheckstate(hwnd, CHK_TCGMCPYBTN);
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
VOID TxtCtlGenCommand(HWND hwnd, ULONG idCtl) {
   switch (idCtl) {
      // bottoni toolbar ---------------------------------------------------
      case BTN_UNDO:
         TxtCtlGenUndo(hwnd);
         break;
      case BTN_DEFAULT:
         TxtCtlGenDefault(hwnd);
         break;
      // selezione entryfield
      case BTN_TCGEFAUTOSEL:
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | FEIDX_EFAUTOSEL;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_MOUSE_FUNCS].undo = 1;
            p->pd[IDPG_MOUSE_FUNCS].apply = 0;
            SetMouEventTxt(ID_TCGEFAUTOSELBY, p->ocur.txt.selMoumsg,
                           p->ocur.txt.selMoukbd, hwnd, BTN_TCGEFAUTOSEL);
         } // end if
         p->flFE = 0;
         break;
      // mouse copy/append/cut/paste/overwrite
      case BTN_TCGCPYBY:
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | FEIDX_TXTCTLCOPY;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_MOUSE_FUNCS].undo = 1;
            p->pd[IDPG_MOUSE_FUNCS].apply = 0;
            SetMouEventTxt(ID_TCGCPYBY, p->ocur.txt.cpyMoumsg,
                           p->ocur.txt.cpyMoukbd, hwnd, BTN_TCGCPYBY);
            p->po->txt.skipcpy = skipmoumsg(p->po->txt.cpyMoumsg);
         } // end if
         p->flFE = 0;
         break;
      case BTN_TCGCUTBY:
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | FEIDX_TXTCTLCUT;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_MOUSE_FUNCS].undo = 1;
            p->pd[IDPG_MOUSE_FUNCS].apply = 0;
            SetMouEventTxt(ID_TCGCUTBY, p->ocur.txt.cutMoumsg,
                           p->ocur.txt.cutMoukbd, hwnd, BTN_TCGCUTBY);
            p->po->txt.skipcut = skipmoumsg(p->po->txt.cutMoumsg);
         } // end if
         p->flFE = 0;
         break;
      case BTN_TCGPSTINSBY:
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | FEIDX_TXTCTLINS;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_MOUSE_FUNCS].undo = 1;
            p->pd[IDPG_MOUSE_FUNCS].apply = 0;
            SetMouEventTxt(ID_TCGPSTINSBY, p->ocur.txt.insMoumsg,
                           p->ocur.txt.insMoukbd, hwnd, BTN_TCGPSTINSBY);
            p->po->txt.skippaste = skipmoumsg(p->po->txt.insMoumsg);
         } // end if
         p->flFE = 0;
         break;
      case BTN_TCGOVRWRTBY:
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | FEIDX_TXTCTLOVRWRITE;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            SetMouEventTxt(ID_TCGOVRWRTBY, p->ocur.txt.ovwMoumsg,
                           p->ocur.txt.ovwMoukbd, hwnd, BTN_TCGOVRWRTBY);
            p->pd[IDPG_MOUSE_FUNCS].undo = 1;
            p->pd[IDPG_MOUSE_FUNCS].apply = 0;
            p->po->txt.skipovwrt = skipmoumsg(p->po->txt.ovwMoumsg);
         } // end if
         p->flFE = 0;
         break;
   } /* endswitch */
}


//==========================================================================
// abilita/disabilita controlli secondo lo stato di flag
// parametri:
// HWND hwnd: handle dialogo
// BOOL flag: abilitazione/disabilitazione
// valore restituito:
// VOID
//==========================================================================
static
VOID TxtCtlGenSetDep(HWND hwnd, BOOL flag) {
   WinEnableControl(hwnd, CHK_TCGMCPYEF, flag);
   WinEnableControl(hwnd, CHK_TCGMCPYMLE, flag);
   WinEnableControl(hwnd, CHK_TCGMCPYSTXT, flag);
   WinEnableControl(hwnd, CHK_TCGMCPYLBOX, flag);
   WinEnableControl(hwnd, CHK_TCGMCPYBTN, flag);
   WinEnableControl(hwnd, BTN_TCGCPYBY, flag);
   WinEnableControl(hwnd, BTN_TCGCUTBY, flag);
   WinEnableControl(hwnd, BTN_TCGPSTINSBY, flag);
   WinEnableControl(hwnd, BTN_TCGOVRWRTBY, flag);
}


// undo
static
VOID TxtCtlGenUndo(HWND hwnd) {
   p->pd[p->idxPage].undo = 0;     // disabilita undo
   WinEnableWindow(p->swpCtls[4].hwnd, FALSE);
   // entry field text selection
   p->po->txt.efsel = p->ocur.txt.efsel = p->ou.txt.efsel;
   p->po->txt.selMoumsg = p->ocur.txt.selMoumsg = p->ou.txt.selMoumsg;
   p->po->txt.selMoukbd = p->ocur.txt.selMoukbd = p->ou.txt.selMoukbd;
   // mouse copy/append/cut/insert/overwrite
   p->po->txt.on = p->ocur.txt.on = p->ou.txt.on;
   p->po->txt.ef = p->ocur.txt.ef = p->ou.txt.ef;
   p->po->txt.mle = p->ocur.txt.mle = p->ou.txt.mle;
   p->po->txt.stat = p->ocur.txt.stat = p->ou.txt.stat;
   p->po->txt.lbox = p->ocur.txt.lbox = p->ou.txt.lbox;
   p->po->txt.btn = p->ocur.txt.btn = p->ou.txt.btn;
   p->po->txt.cpyMoumsg = p->ocur.txt.cpyMoumsg = p->ou.txt.cpyMoumsg;
   p->po->txt.cpyMoukbd = p->ocur.txt.cpyMoukbd = p->ou.txt.cpyMoukbd;
   p->po->txt.cutMoumsg = p->ocur.txt.cutMoumsg = p->ou.txt.cutMoumsg;
   p->po->txt.cutMoukbd = p->ocur.txt.cutMoukbd = p->ou.txt.cutMoukbd;
   p->po->txt.insMoumsg = p->ocur.txt.insMoumsg = p->ou.txt.insMoumsg;
   p->po->txt.insMoukbd = p->ocur.txt.insMoukbd = p->ou.txt.insMoukbd;
   p->po->txt.ovwMoumsg = p->ocur.txt.ovwMoumsg = p->ou.txt.ovwMoumsg;
   p->po->txt.ovwMoukbd = p->ocur.txt.ovwMoukbd = p->ou.txt.ovwMoukbd;
   // aggiorna settaggi azioni mouse/tastiera nelle pagine notebook
   MouKbdUpdateFElist(p->pd[IDPG_MOUSE_FUNCS].hwnd);
   p->po->txt.skipcpy = skipmoumsg(p->po->txt.cpyMoumsg);
   p->po->txt.skipcut = skipmoumsg(p->po->txt.cutMoumsg);
   p->po->txt.skippaste = skipmoumsg(p->po->txt.insMoumsg);
   p->po->txt.skipovwrt = skipmoumsg(p->po->txt.ovwMoumsg);
   // aggiorna controlli dialogo
   TxtCtlGenInit(hwnd, &p->ocur);
}


// default

VOID TxtCtlGenDefault(HWND hwnd) {
   EnableToolbarUndoBtn(1);      // abilita undo
   // entry field text selection
   p->po->txt.efsel = p->ocur.txt.efsel = 1;
   p->po->txt.selMoumsg = p->ocur.txt.selMoumsg = p->is.mou3btns?
                                           WM_BUTTON3CLICK: WM_BUTTON1DBLCLK;
   p->po->txt.selMoukbd = p->ocur.txt.selMoukbd = 0;
   // mouse copy/append/cut/insert/overwrite
   p->po->txt.on = p->ocur.txt.on = 0;
   p->po->txt.ef = p->ocur.txt.ef = 0;
   p->po->txt.mle = p->ocur.txt.mle = 0;
   p->po->txt.stat = p->ocur.txt.stat = 0;
   p->po->txt.lbox = p->ocur.txt.lbox = 0;
   p->po->txt.btn = p->ocur.txt.btn = 0;
   p->po->txt.cpyMoumsg = p->ocur.txt.cpyMoumsg = WM_BUTTON1CLICK;
   p->po->txt.cpyMoukbd = p->ocur.txt.cpyMoukbd = KC_CTRL;
   p->po->txt.cutMoumsg = p->ocur.txt.cutMoumsg = WM_BUTTON1CLICK;
   p->po->txt.cutMoukbd = p->ocur.txt.cutMoukbd = KC_ALT;
   p->po->txt.insMoumsg = p->ocur.txt.insMoumsg = WM_BUTTON1CLICK;
   p->po->txt.insMoukbd = p->ocur.txt.insMoukbd = KC_ALT | KC_SHIFT;
   p->po->txt.ovwMoumsg = p->ocur.txt.ovwMoumsg = WM_BUTTON2CLICK;
   p->po->txt.ovwMoukbd = p->ocur.txt.ovwMoukbd = KC_SHIFT;
   // aggiorna settaggi azioni mouse/tastiera nelle pagine notebook
   if (p->pd[IDPG_MOUSE_FUNCS].hwnd)
      MouKbdUpdateFElist(p->pd[IDPG_MOUSE_FUNCS].hwnd);
   p->po->txt.skipcpy = skipmoumsg(p->po->txt.cpyMoumsg);
   p->po->txt.skipcut = skipmoumsg(p->po->txt.cutMoumsg);
   p->po->txt.skippaste = skipmoumsg(p->po->txt.insMoumsg);
   p->po->txt.skipovwrt = skipmoumsg(p->po->txt.ovwMoumsg);
   // aggiorna controlli dialogo
   if (hwnd) TxtCtlGenInit(hwnd, &p->ocur);
}


//==========================================================================\
// in base all'evento di click o doppio click selezionato per attivare una  |
// funzione restituisce il corrispondente evento WM_BUTTONxDOWN da ignorare |
// parametri:                                                               |
// ULONG msg: evento mouse selezionato                                      |
// valore restituito:                                                       |
// ULONG msg: messaggio WM_BUTTONxDOWN da ignorare                          |
//==========================================================================/
static
ULONG skipmoumsg(ULONG msg) {
   switch (msg) {
   case WM_BUTTON1CLICK:
   case WM_BUTTON1DBLCLK:
      return WM_BUTTON1DOWN;
   case WM_BUTTON2CLICK:
   case WM_BUTTON2DBLCLK:
      return WM_BUTTON2DOWN;
   case WM_BUTTON3CLICK:
   case WM_BUTTON3DBLCLK:
      return WM_BUTTON3DOWN;
   } /* endswitch */
   return 0;
}

