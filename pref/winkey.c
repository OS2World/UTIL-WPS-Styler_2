//==========================================================================
// .c: pagina impostazioni
//==========================================================================

#pragma strings(readonly)

#include "pref.h"
#include "api.h"

// globals ------------------------------------------------------------------

BOOL skip = FALSE;


// prototypes ---------------------------------------------------------------
VOID WinKeyInit(HWND hwnd, POPTIONS pgo, BOOL fl);
VOID WinKeyControl(HWND hwnd, ULONG idCtl, ULONG ulEvent);
VOID WinKeyCommand(HWND hwnd, ULONG idCtl);
VOID WinKeyUndo(HWND hwnd);


//==========================================================================
// procedura dialogo opzioni windows keys
//==========================================================================

MRESULT EXPENTRY WinKeyDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         p->pd[p->idxPage].hwnd = hwnd;
         WinKeyInit(hwnd, &p->ocur, TRUE);
         // riattiva finestra principale
         setparown(hwnd);
         break;
      case WM_CONTROL:
         if (!skip) WinKeyControl(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1));
         break;
      case WM_COMMAND:
         WinKeyCommand(hwnd, SHORT1FROMMP(mp1));
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
static
VOID WinKeyInit(HWND hwnd, POPTIONS po, BOOL fl) {
   if (fl) {
      INT i;
      CHAR buf[128];
   #ifdef STLR_WARP3
      for (i = ID_WKEYNOACT; i <= ID_WKEYOS2CMD; ++i) {
   #else
      for (i = ID_WKEYNOACT; i <= ID_WKEYWCINFO; ++i) {
   #endif
         GetStr(i, buf, 128);
         DlgLboxInsertItem(hwnd, COMBO_WKLEFT, LIT_END, buf);
         DlgLboxInsertItem(hwnd, COMBO_WKRIGHT, LIT_END, buf);
         DlgLboxInsertItem(hwnd, COMBO_WKMENU, LIT_END, buf);
      } /* endfor */
   } /* endif */
   skip = TRUE;
   DlgLboxSelectItem(hwnd, COMBO_WKLEFT, po->winkbd.left);
   DlgLboxSelectItem(hwnd, COMBO_WKRIGHT, po->winkbd.right);
   DlgLboxSelectItem(hwnd, COMBO_WKMENU, po->winkbd.menu);
   skip = FALSE;
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
VOID WinKeyControl(HWND hwnd, ULONG idCtl, ULONG ulEvent) {
   if (ulEvent == CBN_ENTER) {
      switch (idCtl) {
         case COMBO_WKLEFT:
            p->po->winkbd.left = p->ocur.winkbd.left =
                    DlgLboxQuerySelectedItem(hwnd, COMBO_WKLEFT);
            break;
         case COMBO_WKRIGHT:
            p->po->winkbd.right = p->ocur.winkbd.right =
                    DlgLboxQuerySelectedItem(hwnd, COMBO_WKRIGHT);
            break;
         case COMBO_WKMENU:
            p->po->winkbd.menu = p->ocur.winkbd.menu =
                    DlgLboxQuerySelectedItem(hwnd, COMBO_WKMENU);
            break;
      } /* endswitch */
      EnableToolbarUndoBtn(1);
   } /* endif */
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
VOID WinKeyCommand(HWND hwnd, ULONG idCtl) {
   switch (idCtl) {
      // bottoni toolbar ---------------------------------------------------
      case BTN_UNDO:
         WinKeyUndo(hwnd);
         break;
      case BTN_DEFAULT:
         WinKeyDefault(hwnd);
         break;
   } /* endswitch */
}

static
VOID WinKeyUndo(HWND hwnd) {
   p->pd[p->idxPage].undo = 0;     // disabilita undo
   WinEnableWindow(p->swpCtls[4].hwnd, FALSE);
   //
   p->po->winkbd.left = p->ocur.winkbd.left = p->ou.winkbd.left;
   //
   p->po->winkbd.right = p->ocur.winkbd.right = p->ou.winkbd.right;
   //
   p->po->winkbd.menu = p->ocur.winkbd.menu = p->ou.winkbd.menu;
   // aggiorna controlli dialogo
   WinKeyInit(hwnd, &p->ocur, FALSE);
}


// default

VOID WinKeyDefault(HWND hwnd) {
   EnableToolbarUndoBtn(1);      // abilita undo
   //
   p->po->winkbd.left = p->ocur.winkbd.left = 0;
   //
   p->po->winkbd.right = p->ocur.winkbd.right = 0;
   //
   p->po->winkbd.menu = p->ocur.winkbd.menu = 0;
   //
   // aggiorna controlli dialogo
   if (hwnd) WinKeyInit(hwnd, &p->ocur, FALSE);
}
