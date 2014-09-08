//==========================================================================
// sizemove.c: pagina impostazioni movimeno ridimensionamento finestre
//==========================================================================

#pragma strings(readonly)

#include "pref.h"
#include "api.h"

// prototypes ---------------------------------------------------------------
VOID SizeMoveInit(HWND hwnd, POPTIONS po);
VOID SizeMoveControl(HWND hwnd, ULONG idCtl, ULONG ulEvent);
VOID SizeMoveCommand(HWND hwnd, ULONG idCtl);
VOID SizeMoveUndo(HWND hwnd);
VOID moveSetW4drag(ULONG ul);


//==========================================================================
// procedura dialogo opzioni titlebar hiding
// messaggi:
// WM_INITDLG
// WM_CONTROL
// WM_COMMAND
//==========================================================================

MRESULT EXPENTRY SizeMoveOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         p->pd[p->idxPage].hwnd = hwnd;
      #ifndef STLR_WARP3
         // inizializza settaggio full window drag (dipendente da versione s.o.)
         p->po->sizpos.fulldrag = p->ocur.sizpos.fulldrag
                                  = uGetSval(SV_FULLWINDOWDRAG);
      #endif
         SizeMoveInit(hwnd, &p->ocur);
         // riattiva finestra principale
         setparown(hwnd);
         break;
      case WM_CONTROL:
         SizeMoveControl(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1));
         break;
      case WM_COMMAND:
         SizeMoveCommand(hwnd, SHORT1FROMMP(mp1));
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
VOID SizeMoveInit(HWND hwnd, POPTIONS po) {
   WinCheckButton(hwnd, CHK_FULLWINDRAG, po->sizpos.fulldrag);
   WinCheckButton(hwnd, CHK_SMOOTHHMOVE, po->sizpos.noalign);
   WinCheckButton(hwnd, CHK_SZMVZONE, po->sizpos.Zone);
   WinCheckButton(hwnd, CHK_BKGNDSZMOVE, po->sizpos.snap);
   WinEnableControl(hwnd, BTN_BKGNDSZMOVE, po->sizpos.snap);
   SetMouEventTxt(ID_SZMVSZMVBY, po->sizpos.snapMoumsg, po->sizpos.snapMoukbd,
                   hwnd, BTN_BKGNDSZMOVE);
   WinCheckButton(hwnd, CHK_FRAMEDRAG, po->sizpos.framedrg);
   WinEnableControl(hwnd, BTN_FRAMEDRAG, po->sizpos.framedrg);
   SetMouEventTxt(ID_SZMVDRAGBY, po->sizpos.frameMoumsg, po->sizpos.frameMoukbd,
                   hwnd, BTN_FRAMEDRAG);
   WinCheckButton(hwnd, CHK_PUSHBOTTOM, po->sizpos.Zmove);
   WinEnableControl(hwnd, BTN_PUSHBOTTOM, po->sizpos.Zmove);
   WinEnableControl(hwnd, CHK_SZMVZONE, p->ocur.sizpos.Zmove);
   SetMouEventTxt(ID_SZMVPUSHBY, po->sizpos.ZordMoumsg, po->sizpos.ZordMoukbd,
                  hwnd, BTN_PUSHBOTTOM);
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
VOID SizeMoveControl(HWND hwnd, ULONG idCtl, ULONG ulEvent) {
   switch (idCtl) {
      case CHK_FULLWINDRAG:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->sizpos.fulldrag = p->ocur.sizpos.fulldrag =
                           WinQueryButtonCheckstate(hwnd, CHK_FULLWINDRAG);
         #ifndef STLR_WARP3
            moveSetW4drag(p->po->sizpos.fulldrag);
         #endif
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_SMOOTHHMOVE:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->sizpos.noalign = p->ocur.sizpos.noalign =
                           WinQueryButtonCheckstate(hwnd, CHK_SMOOTHHMOVE);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      // ora ha la funzione di snap to the desktop edge
      case CHK_BKGNDSZMOVE:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->sizpos.snap = p->ocur.sizpos.snap =
                           WinQueryButtonCheckstate(hwnd, CHK_BKGNDSZMOVE);
            WinEnableControl(hwnd, BTN_BKGNDSZMOVE, p->ocur.sizpos.snap);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_FRAMEDRAG:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->sizpos.framedrg = p->ocur.sizpos.framedrg =
                           WinQueryButtonCheckstate(hwnd, CHK_FRAMEDRAG);
            WinEnableControl(hwnd, BTN_FRAMEDRAG, p->ocur.sizpos.framedrg);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_PUSHBOTTOM:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->sizpos.Zmove = p->ocur.sizpos.Zmove =
                           WinQueryButtonCheckstate(hwnd, CHK_PUSHBOTTOM);
            WinEnableControl(hwnd, CHK_SZMVZONE, p->ocur.sizpos.Zmove);
            WinEnableControl(hwnd, BTN_PUSHBOTTOM, p->ocur.sizpos.Zmove);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_SZMVZONE:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->sizpos.Zone = p->ocur.sizpos.Zone =
                           WinQueryButtonCheckstate(hwnd, CHK_SZMVZONE);
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
VOID SizeMoveCommand(HWND hwnd, ULONG idCtl) {
   switch (idCtl) {
      // bottoni toolbar ---------------------------------------------------
      case BTN_UNDO:
         SizeMoveUndo(hwnd);
         break;
      case BTN_DEFAULT:
         SizeMoveDefault(hwnd);
         break;
      // bottoni selezione azioni mouse tastiera
      // ora per funzione snap to screen edge
      case BTN_BKGNDSZMOVE:
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | FEIDX_BKSZMOVE;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_MOUSE_FUNCS].undo = 1;
            p->pd[IDPG_MOUSE_FUNCS].apply = 0;
            SetMouEventTxt(ID_SZMVSZMVBY, p->ocur.sizpos.snapMoumsg,
                           p->ocur.sizpos.snapMoukbd, hwnd, BTN_BKGNDSZMOVE);
         } // end if
         p->flFE = 0;
         break;
      case BTN_FRAMEDRAG:
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | FEIDX_FRAMEDRAG;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_MOUSE_FUNCS].undo = 1;
            p->pd[IDPG_MOUSE_FUNCS].apply = 0;
            SetMouEventTxt(ID_SZMVDRAGBY,p->ocur.sizpos.frameMoumsg,
                           p->ocur.sizpos.frameMoukbd, hwnd, BTN_FRAMEDRAG);
         } // end if
         p->flFE = 0;
         break;
      case BTN_PUSHBOTTOM:
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | FEIDX_PUTBEHIND;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_MOUSE_FUNCS].undo = 1;
            p->pd[IDPG_MOUSE_FUNCS].apply = 0;
            SetMouEventTxt(ID_SZMVPUSHBY, p->ocur.sizpos.ZordMoumsg,
                           p->ocur.sizpos.ZordMoukbd, hwnd, BTN_PUSHBOTTOM);
         } // end if
         p->flFE = 0;
         break;
   } /* endswitch */
}


// undo
static
VOID SizeMoveUndo(HWND hwnd) {
   p->pd[p->idxPage].undo = 0;     // disabilita undo
   WinEnableWindow(p->swpCtls[4].hwnd, FALSE);
   // full window drag
   p->po->sizpos.fulldrag = p->ocur.sizpos.fulldrag = p->ou.sizpos.fulldrag;
#ifndef STLR_WARP3
   moveSetW4drag(p->po->sizpos.fulldrag);
#endif
   // no byte align
   p->po->sizpos.noalign = p->ocur.sizpos.noalign = p->ou.sizpos.noalign;
   // snap to window edge
   p->po->sizpos.snap = p->ocur.sizpos.snap = p->ou.sizpos.snap;
   p->po->sizpos.snapMoumsg = p->ocur.sizpos.snapMoumsg = p->ou.sizpos.snapMoumsg;
   p->po->sizpos.snapMoukbd = p->ocur.sizpos.snapMoukbd = p->ou.sizpos.snapMoukbd;
   // frame drag
   p->po->sizpos.framedrg = p->ocur.sizpos.framedrg = p->ou.sizpos.framedrg;
   p->po->sizpos.frameMoumsg = p->ocur.sizpos.frameMoumsg = p->ou.sizpos.frameMoumsg;
   p->po->sizpos.frameMoukbd = p->ocur.sizpos.frameMoukbd = p->ou.sizpos.frameMoukbd;
   // push behind
   p->po->sizpos.Zmove = p->ocur.sizpos.Zmove = p->ou.sizpos.Zmove;
   // riservata per spostare nello Z order di una posizione
   p->po->sizpos.Zone = p->ocur.sizpos.Zone = p->ou.sizpos.Zone;
   p->po->sizpos.ZordMoumsg = p->ocur.sizpos.ZordMoumsg = p->ou.sizpos.ZordMoumsg;
   p->po->sizpos.ZordMoukbd = p->ocur.sizpos.ZordMoukbd = p->ou.sizpos.ZordMoukbd;
   // aggiorna settaggi azioni mouse/tastiera nelle pagine notebook
   MouKbdUpdateFElist(p->pd[IDPG_MOUSE_FUNCS].hwnd);
   // aggiorna controlli dialogo
   SizeMoveInit(hwnd, &p->ocur);
}


// default

VOID SizeMoveDefault(HWND hwnd) {
   EnableToolbarUndoBtn(1);      // abilita undo
   // full window drag
   p->po->sizpos.fulldrag = p->ocur.sizpos.fulldrag = 0;
#ifndef STLR_WARP3
   moveSetW4drag(0);
#endif
   // no byte align
   p->po->sizpos.noalign = p->ocur.sizpos.noalign = 0;
   // riservata per spostare nello Zorder di una posizione
   p->po->sizpos.Zone = p->ocur.sizpos.Zone = 0;
   // snap to screen edge
   p->po->sizpos.snap = p->ocur.sizpos.snap = 0;
   p->po->sizpos.snapMoumsg = p->ocur.sizpos.snapMoumsg = WM_BUTTON1CLICK;
   p->po->sizpos.snapMoukbd = p->ocur.sizpos.snapMoukbd = KC_SHIFT;
   // frame drag
   p->po->sizpos.framedrg = p->ocur.sizpos.framedrg = 1;
   p->po->sizpos.frameMoumsg = p->ocur.sizpos.frameMoumsg = WM_BUTTON2DOWN;
   p->po->sizpos.frameMoukbd = p->ocur.sizpos.frameMoukbd = 0;
   // push behind
   p->po->sizpos.Zmove = p->ocur.sizpos.Zmove = 1;
   p->po->sizpos.ZordMoumsg = p->ocur.sizpos.ZordMoumsg = WM_BUTTON2CLICK;
   p->po->sizpos.ZordMoukbd = p->ocur.sizpos.ZordMoukbd = KC_SHIFT;
   // aggiorna settaggi azioni mouse/tastiera nelle pagine notebook
   if (p->pd[IDPG_MOUSE_FUNCS].hwnd)
      MouKbdUpdateFElist(p->pd[IDPG_MOUSE_FUNCS].hwnd);
   // aggiorna controlli dialogo
   if (hwnd) SizeMoveInit(hwnd, &p->ocur);
}


// setta/resetta modalit… full window drag in warp 4
static VOID moveSetW4drag(ULONG ul) {
   WinSetSysValue(HWND_DESKTOP, SV_FULLWINDOWDRAG, ul);
   PrfWriteProfileData(HINI_USER, "PM_ControlPanel",
                       "FullWindowDrag", &ul, sizeof(ULONG));
}
