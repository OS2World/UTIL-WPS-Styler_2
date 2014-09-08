//===========================================================================
// .c: pagina impostazioni
//===========================================================================

#pragma strings(readonly)

// includes -----------------------------------------------------------------

#include "pref.h"
#include "api.h"

// defines ------------------------------------------------------------------
// globals ------------------------------------------------------------------
// prototypes ---------------------------------------------------------------
VOID pgInit(HWND hwnd, POPTIONS po);
VOID pgControl(HWND hwnd, ULONG idCtl, ULONG ulEvent);
VOID pgCommand(HWND hwnd, ULONG idCtl);
VOID pgSetMainDep(HWND hwnd, BOOL flag);
VOID pgUndo(HWND hwnd);
VOID pgDefault(HWND hwnd);


//===========================================================================
//  page window procedure.
// Parameters --------------------------------------------------------------
// Ordinary window procedure parameters.
// Return value ------------------------------------------------------------
// MRESULT.
//===========================================================================

MRESULT EXPENTRY PrefixPgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         p->pd[p->idxPage].hwnd = hwnd;
         pgInit(hwnd, &p->ocur);
         // riattiva finestra principale
         setparown(hwnd);
         break;
      case WM_CONTROL:
         pgControl(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1));
         break;
      case WM_COMMAND:
         pgCommand(hwnd, SHORT1FROMMP(mp1));
         break;
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return (MRESULT)FALSE;
}


//===========================================================================
// Initialization/reinitialization procedure.
// Parameters --------------------------------------------------------------
// HWND hwnd   : window handle
// POPTIONS po : current options used to init the page.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID pgInit(HWND hwnd, POPTIONS po) {
   WinCheckButton(hwnd, , po->);
   WinCheckButton(hwnd, , po->);
   WinCheckButton(hwnd, , po->);
   WinCheckButton(hwnd, , po->);
   WinCheckButton(hwnd, , po->);
   WinCheckButton(hwnd, , po->);
   WinEnableControl(hwnd, , po->);
   WinEnableControl(hwnd, , po->);
   WinEnableControl(hwnd, , po->);
   WinEnableControl(hwnd, , po->);
   WinEnableControl(hwnd, , po->);
   WinEnableControl(hwnd, , po->);
   WinEnableControl(hwnd, , po->);
   DlgSpinSetValue(hwnd, , po->);
   DlgSpinSetValue(hwnd, , po->);
   DlgSpinSetValue(hwnd, , po->);
   DlgSpinSetValue(hwnd, , po->);
   DlgSpinSetValue(hwnd, , po->);
   DlgSpinSetValue(hwnd, , po->);
   DlgSpinSetValue(hwnd, , po->);
   DlgSpinSetValue(hwnd, , po->);
   PrefixSetMainDep(hwnd, po->);
}


//===========================================================================
// Handles notification messages.
// Parameters --------------------------------------------------------------
// HWND hwnd     : window handle.
// ULONG idCtl   : ID of the control sending the notification message.
// ULONG ulEvent : type of notification message.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID pgControl(HWND hwnd, ULONG idCtl, ULONG ulEvent) {
   switch (idCtl) {
      case :   // check box
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po-> = p->ocur. =
                           WinQueryButtonCheckstate(hwnd, );
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case :   // check box
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po-> = p->ocur. =
                           WinQueryButtonCheckstate(hwnd, );
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case :   // check box
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po-> = p->ocur. =
                           WinQueryButtonCheckstate(hwnd, );
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case :   // check box
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po-> = p->ocur. =
                           WinQueryButtonCheckstate(hwnd, );
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case :     // spinbutton x
         if (ulEvent == SPBN_CHANGE) {
            LONG l;
            WinSendDlgItemMsg(hwnd, idCtl, SPBM_QUERYVALUE, (MPARAM)&l,
                              MPFROM2SHORT(0, SPBQ_ALWAYSUPDATE));
            if (l != p->ocur.) {
               p->ocur. = l;
               EnableToolbarUndoBtn(1);
            } /* endif */
         } else if (ulEvent == SPBN_KILLFOCUS) {
            p->po-> = p->ocur.;
         } /* endif */
         break;
      case :     // spinbutton x
         if (ulEvent == SPBN_CHANGE) {
            LONG l;
            WinSendDlgItemMsg(hwnd, idCtl, SPBM_QUERYVALUE, (MPARAM)&l,
                              MPFROM2SHORT(0, SPBQ_ALWAYSUPDATE));
            if (l != p->ocur.) {
               p->ocur. = l;
               EnableToolbarUndoBtn(1);
            } /* endif */
         } else if (ulEvent == SPBN_KILLFOCUS) {
            p->po-> = p->ocur.;
         } /* endif */
         break;
      case :     // spinbutton x
         if (ulEvent == SPBN_CHANGE) {
            LONG l;
            WinSendDlgItemMsg(hwnd, idCtl, SPBM_QUERYVALUE, (MPARAM)&l,
                              MPFROM2SHORT(0, SPBQ_ALWAYSUPDATE));
            if (l != p->ocur.) {
               p->ocur. = l;
               EnableToolbarUndoBtn(1);
            } /* endif */
         } else if (ulEvent == SPBN_KILLFOCUS) {
            p->po-> = p->ocur.;
         } /* endif */
         break;
      case :     // spinbutton x
         if (ulEvent == SPBN_CHANGE) {
            LONG l;
            WinSendDlgItemMsg(hwnd, idCtl, SPBM_QUERYVALUE, (MPARAM)&l,
                              MPFROM2SHORT(0, SPBQ_ALWAYSUPDATE));
            if (l != p->ocur.) {
               p->ocur. = l;
               EnableToolbarUndoBtn(1);
            } /* endif */
         } else if (ulEvent == SPBN_KILLFOCUS) {
            p->po-> = p->ocur.;
         } /* endif */
         break;
   } /* endswitch */
}


//===========================================================================
// Handle the WM_COMMAND message.
// Parameters --------------------------------------------------------------
// HWND hwnd   : window handle.
// ULONG idCtl : ID of the control which generated the command.
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
// VOID
//===========================================================================
static
VOID pgCommand(HWND hwnd, ULONG idCtl) {
   switch (idCtl) {
      // bottoni toolbar ---------------------------------------------------
      case BTN_UNDO:
         pgUndo(hwnd);
         break;
      case BTN_DEFAULT:
         pgDefault(hwnd);
         break;
      // bottoni selezione azioni mouse tastiera
      case :
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | ;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_MOUSE_FUNCS].undo = 1;
            p->pd[IDPG_MOUSE_FUNCS].apply = 0;
            SetMouEventTxt(ID_, p->ocur.,          //mouVK
                           p->ocur., hwnd, BTN_)); // KC
         } // end if
         p->flFE = 0;
         break;
      case :
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | ;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_MOUSE_FUNCS].undo = 1;
            p->pd[IDPG_MOUSE_FUNCS].apply = 0;
            SetMouEventTxt(ID_, p->ocur.,          //mouVK
                           p->ocur., hwnd, BTN_)); // KC
         } // end if
         p->flFE = 0;
         break;
      case :
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | ;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_MOUSE_FUNCS].undo = 1;
            p->pd[IDPG_MOUSE_FUNCS].apply = 0;
            SetMouEventTxt(ID_, p->ocur.,          //mouVK
                           p->ocur., hwnd, BTN_)); // KC
         } // end if
         p->flFE = 0;
         break;
      case :
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | ;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_KEYB_FUNCS].undo = 1;
            p->pd[IDPG_KEYB_FUNCS].apply = 0;
            SetMouEventTxt(ID_, p->ocur.,          //mouVK
                           p->ocur., hwnd, BTN_)); // KC
         } // end if
         p->flFE = 0;
         break;
      case :
         p->flFE = FE_FLOATDLG | FE_MOUSECLK | ;
         if (WinDlgBox(HWND_DESKTOP, hwnd, (PFNWP)MouKeybDlgProc, p->hres,
                       DLG_MOUKEYBACTSFL, NULL)) {
            EnableToolbarUndoBtn(1);
            p->pd[IDPG_KEYB_FUNCS].undo = 1;
            p->pd[IDPG_KEYB_FUNCS].apply = 0;
            SetMouEventTxt(ID_, p->ocur.,          //mouVK
                           p->ocur., hwnd, BTN_)); // KC
         } // end if
         p->flFE = 0;
         break;
   } /* endswitch */
}


//===========================================================================
// Enable/disable controls depending on the state of another control.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle.
// BOOL flag : TRUE/FALSE (enable/disable).
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID pgSetMainDep(HWND hwnd, BOOL flag) {
   WinCheckButton(hwnd, , flag);
   WinEnableControl(hwnd, , flag);
   WinEnableControl(hwnd, , flag);
   WinEnableControl(hwnd, , flag);
   WinEnableControl(hwnd, , flag);
   WinEnableControl(hwnd, , flag);
   WinEnableControl(hwnd, , flag);
   WinEnableControl(hwnd, , flag);
   WinEnableControl(hwnd, , flag);
   WinEnableControl(hwnd, , flag);
   WinEnableControl(hwnd, , flag);
   WinEnableControl(hwnd, , flag);
   WinEnableControl(hwnd, , flag);
}


//===========================================================================
// Undo the current settings restoring the settings used when the program
// was started.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID pgUndo(HWND hwnd) {
   p->pd[p->idxPage].undo = 0;     // disabilita undo
   WinEnableWindow(p->swpCtls[4].hwnd, FALSE);
   //
   p->po-> = p->ocur. = p->ou.;
   //
   p->po-> = p->ocur. = p->ou.;
   //
   p->po-> = p->ocur. = p->ou.;
   //
   p->po-> = p->ocur. = p->ou.;
   p->po-> = p->ocur. = p->ou.;
   p->po-> = p->ocur. = p->ou.;
   //
   p->po-> = p->ocur. = p->ou.;
   //
   p->po-> = p->ocur. = p->ou.;
   //
   p->po-> = p->ocur. = p->ou.;
   //
   p->po-> = p->ocur. = p->ou.;
   p->po-> = p->ocur. = p->ou.;
   p->po-> = p->ocur. = p->ou.;
   // aggiorna settaggi azioni mouse/tastiera nelle pagine notebook
   MouKbdUpdateFElist(p->pd[IDPG_MOUSE_FUNCS].hwnd);
   MouKbdUpdateFElist(p->pd[IDPG_KEYB_FUNCS].hwnd);
   // aggiorna controlli dialogo
   pgInit(hwnd, &p->ocur);
}

//===========================================================================
// Change the settings to the default values.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID pgDefault(HWND hwnd) {
   EnableToolbarUndoBtn(1);      // abilita undo
   //
   p->po-> = p->ocur. = 0;
   //
   p->po-> = p->ocur. = 0;
   //
   p->po-> = p->ocur. = 0;
   //
   p->po-> = p->ocur. = 0;
   p->po-> = p->ocur. = 0;
   p->po-> = p->ocur. = 0;
   // aggiorna settaggi azioni mouse/tastiera nelle pagine notebook
   if (p->pd[IDPG_MOUSE_FUNCS].hwnd)
      MouKbdUpdateFElist(p->pd[IDPG_MOUSE_FUNCS].hwnd);
   if (p->pd[IDPG_KEYB_FUNCS].hwnd)
      MouKbdUpdateFElist(p->pd[IDPG_KEYB_FUNCS].hwnd);
   // aggiorna controlli dialogo
   if (hwnd) pgInit(hwnd, &p->ocur);
}
