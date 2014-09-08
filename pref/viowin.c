//==========================================================================
// viowin.c: pagina impostazioni finestre vio
//==========================================================================

#pragma strings(readonly)

#include "pref.h"
#include "api.h"

// prototypes ---------------------------------------------------------------
VOID VIOwinInit(HWND hwnd, POPTIONS pgo);
VOID VIOwinControl(HWND hwnd, ULONG idCtl, ULONG ulEvent);
VOID VIOwinCommand(HWND hwnd, ULONG idCtl);
VOID VIOmouCpyCtls(HWND hwnd, BOOL fl);
VOID VIOkbdCpyCtls(HWND hwnd, BOOL fl);
VOID VIOwinUndo(HWND hwnd);

//==========================================================================
// procedura dialogo opzioni VIO windows
//==========================================================================

MRESULT EXPENTRY VIOwinOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         p->pd[p->idxPage].hwnd = hwnd;
         VIOwinInit(hwnd, &p->ocur);
         // riattiva finestra principale
         setparown(hwnd);
         break;
      case WM_CONTROL:
         VIOwinControl(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1));
         break;
      case WM_COMMAND:
         VIOwinCommand(hwnd, SHORT1FROMMP(mp1));
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
VOID VIOwinInit(HWND hwnd, POPTIONS po) {
   WinCheckButton(hwnd, CHK_VIONOHSCRL, po->vio.noscrl);
   WinCheckButton(hwnd, CHK_VIOINS, po->vio.ins);
   WinCheckButton(hwnd, CHK_VIOMOUCPY, po->vio.mou);
   VIOmouCpyCtls(hwnd, po->vio.mou);
   WinCheckButton(hwnd, CHK_VIOKBDCPY, po->vio.kbd);
   VIOkbdCpyCtls(hwnd, po->vio.kbd);
   // inizializza caption bottoni
   SetMouEventTxt(ID_VIOCPYBY, p->ocur.vio.cpyMoumsg,
                  p->ocur.vio.cpyMoukbd, hwnd, BTN_VIOCPYMOU);
   SetMouEventTxt(ID_VIOCPYALLBY, p->ocur.vio.cpyaMoumsg,
                  p->ocur.vio.cpyaMoukbd, hwnd, BTN_VIOAPNDMOU);
   SetMouEventTxt(ID_VIOPASTEBY, p->ocur.vio.insMoumsg,
                  p->ocur.vio.insMoukbd, hwnd, BTN_VIOPASTEMOU);
   SetKbdEventTxt(ID_VIOCPYBY, p->ocur.vio.cpyKbdVK,
                  p->ocur.vio.cpyKbdKC, hwnd, BTN_VIOCPYKBD);
   SetKbdEventTxt(ID_VIOCPYALLBY, p->ocur.vio.cpyaKbdVK,
                  p->ocur.vio.cpyaKbdKC, hwnd, BTN_VIOAPNDKBD);
   SetKbdEventTxt(ID_VIOPASTEBY, p->ocur.vio.insKbdVK,
                  p->ocur.vio.insKbdKC, hwnd, BTN_VIOPASTEKBD);
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
VOID VIOwinControl(HWND hwnd, ULONG idCtl, ULONG ulEvent) {
   switch (idCtl) {
      case CHK_VIONOHSCRL:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->vio.noscrl = p->ocur.vio.noscrl =
                           WinQueryButtonCheckstate(hwnd, CHK_VIONOHSCRL);
            EnableToolbarUndoBtn(1);
         } // end if
      case CHK_VIOINS:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->vio.ins = p->ocur.vio.ins =
                           WinQueryButtonCheckstate(hwnd, CHK_VIOINS);
            EnableToolbarUndoBtn(1);
         } // end if
      case CHK_VIOMOUCPY:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->vio.mou = p->ocur.vio.mou =
                           WinQueryButtonCheckstate(hwnd, CHK_VIOMOUCPY);
            VIOmouCpyCtls(hwnd, p->ocur.vio.mou);
            EnableToolbarUndoBtn(1);
         } // end if
      case CHK_VIOKBDCPY:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->vio.kbd = p->ocur.vio.kbd =
                           WinQueryButtonCheckstate(hwnd, CHK_VIOKBDCPY);
            VIOkbdCpyCtls(hwnd, p->ocur.vio.kbd);
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
VOID VIOwinCommand(HWND hwnd, ULONG idCtl) {
   switch (idCtl) {
      // bottoni toolbar ---------------------------------------------------
      case BTN_UNDO:
         VIOwinUndo(hwnd);
         break;
      case BTN_DEFAULT:
         VIOwinDefault(hwnd);
         break;
      case BTN_VIOCPYMOU:
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | FEIDX_VIOWINCOPY;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_MOUSE_FUNCS].undo = 1;
            p->pd[IDPG_MOUSE_FUNCS].apply = 0;
            SetMouEventTxt(ID_VIOCPYBY, p->ocur.vio.cpyMoumsg,
                           p->ocur.vio.cpyMoukbd, hwnd, BTN_VIOCPYMOU);
         } // end if
         p->flFE = 0;
         break;
      case BTN_VIOAPNDMOU:
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | FEIDX_VIOWINAPND;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_MOUSE_FUNCS].undo = 1;
            p->pd[IDPG_MOUSE_FUNCS].apply = 0;
            SetMouEventTxt(ID_VIOCPYALLBY, p->ocur.vio.cpyaMoumsg,
                           p->ocur.vio.cpyaMoukbd, hwnd, BTN_VIOAPNDMOU);
         } // end if
         p->flFE = 0;
         break;
      case BTN_VIOPASTEMOU:
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | FEIDX_VIOWINPASTE;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_MOUSE_FUNCS].undo = 1;
            p->pd[IDPG_MOUSE_FUNCS].apply = 0;
            SetMouEventTxt(ID_VIOPASTEBY, p->ocur.vio.insMoumsg,
                           p->ocur.vio.insMoukbd, hwnd, BTN_VIOPASTEMOU);
         } // end if
         p->flFE = 0;
         break;
      case BTN_VIOCPYKBD:
         p->flFE = FE_FLOATDLG | FEIDX_VIOWINCOPY;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_KEYB_FUNCS].undo = 1;
            p->pd[IDPG_KEYB_FUNCS].apply = 0;
            SetKbdEventTxt(ID_VIOCPYBY, p->ocur.vio.cpyKbdVK,
                           p->ocur.vio.cpyKbdKC, hwnd, BTN_VIOCPYKBD);
         } // end if
         p->flFE = 0;
         break;
      case BTN_VIOAPNDKBD:
         p->flFE = FE_FLOATDLG | FEIDX_VIOWINAPND;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_KEYB_FUNCS].undo = 1;
            p->pd[IDPG_KEYB_FUNCS].apply = 0;
            SetKbdEventTxt(ID_VIOCPYALLBY, p->ocur.vio.cpyaKbdVK,
                           p->ocur.vio.cpyaKbdKC, hwnd, BTN_VIOAPNDKBD);
         } // end if
         p->flFE = 0;
         break;
      case BTN_VIOPASTEKBD:
         p->flFE = FE_FLOATDLG | FEIDX_VIOWINPASTE;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_KEYB_FUNCS].undo = 1;
            p->pd[IDPG_KEYB_FUNCS].apply = 0;
            SetKbdEventTxt(ID_VIOPASTEBY, p->ocur.vio.insKbdVK,
                           p->ocur.vio.insKbdKC, hwnd, BTN_VIOPASTEKBD);
         } // end if
         p->flFE = 0;
         break;
   } /* endswitch */
}


// abilita pulsanti dipendenti da stato checkbox mouse copy/paste
static
VOID VIOmouCpyCtls(HWND hwnd, BOOL fl) {
   WinEnableControl(hwnd, BTN_VIOCPYMOU, fl);
   WinEnableControl(hwnd, BTN_VIOAPNDMOU, fl);
   WinEnableControl(hwnd, BTN_VIOPASTEMOU, fl);
}

// abilita pulsanti dipendenti da stato checkbox mouse copy/paste
static
VOID VIOkbdCpyCtls(HWND hwnd, BOOL fl) {
   WinEnableControl(hwnd, BTN_VIOCPYKBD, fl);
   WinEnableControl(hwnd, BTN_VIOAPNDKBD, fl);
   WinEnableControl(hwnd, BTN_VIOPASTEKBD, fl);
}


// undo
static
VOID VIOwinUndo(HWND hwnd) {
   p->pd[p->idxPage].undo = 0;     // disabilita undo
   WinEnableWindow(p->swpCtls[4].hwnd, FALSE);
   // opzioni generiche
   p->po->vio.noscrl = p->ocur.vio.noscrl = p->ou.vio.noscrl;
   p->po->vio.ins = p->ocur.vio.ins = p->ou.vio.ins;
   // mouse copy/append/paste
   p->po->vio.mou = p->ocur.vio.mou = p->ou.vio.mou;
   p->po->vio.cpyMoumsg = p->ocur.vio.cpyMoumsg = p->ou.vio.cpyMoumsg;
   p->po->vio.cpyMoukbd = p->ocur.vio.cpyMoukbd = p->ou.vio.cpyMoukbd;
   p->po->vio.cpyaMoumsg = p->ocur.vio.cpyaMoumsg = p->ou.vio.cpyaMoumsg;
   p->po->vio.cpyaMoukbd = p->ocur.vio.cpyaMoukbd = p->ou.vio.cpyaMoukbd;
   p->po->vio.insMoumsg = p->ocur.vio.insMoumsg = p->ou.vio.insMoumsg;
   p->po->vio.insMoukbd = p->ocur.vio.insMoukbd = p->ou.vio.insMoukbd;
   // keyboard copy/append/paste
   p->po->vio.kbd = p->ocur.vio.kbd = p->ou.vio.kbd;
   p->po->vio.cpyKbdVK = p->ocur.vio.cpyKbdVK = p->ou.vio.cpyKbdVK;
   p->po->vio.cpyKbdKC = p->ocur.vio.cpyKbdKC = p->ou.vio.cpyKbdKC;
   p->po->vio.cpyaKbdVK = p->ocur.vio.cpyaKbdVK = p->ou.vio.cpyaKbdVK;
   p->po->vio.cpyaKbdKC = p->ocur.vio.cpyaKbdKC = p->ou.vio.cpyaKbdKC;
   p->po->vio.insKbdVK = p->ocur.vio.insKbdVK = p->ou.vio.insKbdVK;
   p->po->vio.insKbdKC = p->ocur.vio.insKbdKC = p->ou.vio.insKbdKC;
   // aggiorna settaggi azioni mouse/tastiera nelle pagine notebook
   MouKbdUpdateFElist(p->pd[IDPG_MOUSE_FUNCS].hwnd);
   MouKbdUpdateFElist(p->pd[IDPG_KEYB_FUNCS].hwnd);
   // aggiorna controlli dialogo
   VIOwinInit(hwnd, &p->ocur);
}


// default

VOID VIOwinDefault(HWND hwnd) {
   EnableToolbarUndoBtn(1);      // abilita undo
   // opzioni generiche
   p->po->vio.noscrl = p->ocur.vio.noscrl = 0;
   p->po->vio.ins = p->ocur.vio.ins = 0;
   // mouse copy/append/paste
   p->po->vio.mou = p->ocur.vio.mou = 0;
   p->po->vio.cpyMoumsg = p->ocur.vio.cpyMoumsg = WM_BUTTON1DOWN;
   p->po->vio.cpyMoukbd = p->ocur.vio.cpyMoukbd = KC_CTRL;
   p->po->vio.cpyaMoumsg = p->ocur.vio.cpyaMoumsg = WM_BUTTON1CLICK;
   p->po->vio.cpyaMoukbd = p->ocur.vio.cpyaMoukbd = KC_CTRL | KC_SHIFT;
   p->po->vio.insMoumsg = p->ocur.vio.insMoumsg = WM_BUTTON1CLICK;
   p->po->vio.insMoukbd = p->ocur.vio.insMoukbd = KC_SHIFT;
   // keyboard copy/copyall/paste
   p->po->vio.kbd = p->ocur.vio.kbd = 0;
   p->po->vio.cpyKbdVK = p->ocur.vio.cpyKbdVK = VK_INSERT | 0x6800;
   p->po->vio.cpyKbdKC = p->ocur.vio.cpyKbdKC = KC_CTRL;
   p->po->vio.cpyaKbdVK = p->ocur.vio.cpyaKbdVK = VK_INSERT | 0x6800;
   p->po->vio.cpyaKbdKC = p->ocur.vio.cpyaKbdKC = KC_CTRL | KC_SHIFT;
   p->po->vio.insKbdVK = p->ocur.vio.insKbdVK = VK_INSERT | 0x6800;
   p->po->vio.insKbdKC = p->ocur.vio.insKbdKC = KC_SHIFT;
   // aggiorna settaggi azioni mouse/tastiera nelle pagine notebook
   if (p->pd[IDPG_MOUSE_FUNCS].hwnd)
      MouKbdUpdateFElist(p->pd[IDPG_MOUSE_FUNCS].hwnd);
   if (p->pd[IDPG_KEYB_FUNCS].hwnd)
      MouKbdUpdateFElist(p->pd[IDPG_KEYB_FUNCS].hwnd);
   // aggiorna controlli dialogo
   if (hwnd) VIOwinInit(hwnd, &p->ocur);
}
