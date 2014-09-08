//==========================================================================
// maxwin.c: pagina impostazioni maximized windows
//==========================================================================

#pragma strings(readonly)

#include "pref.h"
#include "api.h"

// prototypes ---------------------------------------------------------------
VOID MaxWinInit(HWND hwnd, POPTIONS po);
VOID MaxWinControl(HWND hwnd, ULONG idCtl, ULONG ulEvent);
VOID MaxWinSetMainDep(HWND hwnd, BOOL flag);
VOID MaxWinCommand(HWND hwnd, ULONG idCtl);
VOID MaxWinUndo(HWND hwnd);


//==========================================================================
// procedura dialogo opzioni titlebar hiding
// messaggi:
// WM_INITDLG
// WM_CONTROL
//==========================================================================

MRESULT EXPENTRY MaxWinOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         p->pd[p->idxPage].hwnd = hwnd;
         MaxWinInit(hwnd, &p->ocur);
         // riattiva finestra principale
         setparown(hwnd);
         break;
      case WM_CONTROL:
         MaxWinControl(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1));
         break;
      case WM_COMMAND:
         MaxWinCommand(hwnd, SHORT1FROMMP(mp1));
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

VOID MaxWinInit(HWND hwnd, POPTIONS po) {
   DlgSpinSetValue(hwnd, SPIN_MXX,
      (((SHORT)po->maxwin.x < 0)? 0xffff0000 | po->maxwin.x: po->maxwin.x));
   DlgSpinSetValue(hwnd, SPIN_MXY,
      (((SHORT)po->maxwin.y < 0)? 0xffff0000 | po->maxwin.y: po->maxwin.y));
   DlgSpinSetValue(hwnd, SPIN_MXWIDHT, po->maxwin.cx);
   DlgSpinSetValue(hwnd, SPIN_MXHEIGHT, po->maxwin.cy);
   WinCheckButton(hwnd, CHK_MXNOMOVE, po->maxwin.nomove);
   WinCheckButton(hwnd, CHK_MXNOSIZE, po->maxwin.nosize);
   MaxWinSetMainDep(hwnd, po->maxwin.on);
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

VOID MaxWinControl(HWND hwnd, ULONG idCtl, ULONG ulEvent) {
   switch (idCtl) {
      case CHK_MXOVERSZPOS:        // checkbox abilitazione opzioni
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->maxwin.on = p->ocur.maxwin.on =
                             WinQueryButtonCheckstate(hwnd, CHK_MXOVERSZPOS);
            MaxWinSetMainDep(hwnd, p->ocur.maxwin.on);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_MXNOMOVE:           // checkbox don't allow moving
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->maxwin.nomove = p->ocur.maxwin.nomove =
                             WinQueryButtonCheckstate(hwnd, CHK_MXNOMOVE);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_MXNOSIZE:           // checkbox don't allow sizing
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->maxwin.nosize = p->ocur.maxwin.nosize =
                             WinQueryButtonCheckstate(hwnd, CHK_MXNOSIZE);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case SPIN_MXX:     // spinbutton x
         if (ulEvent == SPBN_CHANGE) {
            LONG l;
            WinSendDlgItemMsg(hwnd, idCtl, SPBM_QUERYVALUE, (MPARAM)&l,
                              MPFROM2SHORT(0, SPBQ_ALWAYSUPDATE));
            if (l != p->ocur.maxwin.x) {
               p->ocur.maxwin.x = l;
               EnableToolbarUndoBtn(1);
            } /* endif */
         } else if (ulEvent == SPBN_KILLFOCUS) {
            p->po->maxwin.x = p->ocur.maxwin.x;
         } /* endif */
         break;
      case SPIN_MXY:     // spinbutton y
         if (ulEvent == SPBN_CHANGE) {
            LONG l;
            WinSendDlgItemMsg(hwnd, idCtl, SPBM_QUERYVALUE, (MPARAM)&l,
                              MPFROM2SHORT(0, SPBQ_ALWAYSUPDATE));
            if (l != p->ocur.maxwin.y) {
               p->ocur.maxwin.y = l;
               EnableToolbarUndoBtn(1);
            } /* endif */
         } else if (ulEvent == SPBN_KILLFOCUS) {
            p->po->maxwin.y = p->ocur.maxwin.y;
         } /* endif */
         break;
      case SPIN_MXWIDHT:     // spinbutton cx
         if (ulEvent == SPBN_CHANGE) {
            LONG l;
            WinSendDlgItemMsg(hwnd, idCtl, SPBM_QUERYVALUE, (MPARAM)&l,
                              MPFROM2SHORT(0, SPBQ_ALWAYSUPDATE));
            if (l != p->ocur.maxwin.cx) {
               p->ocur.maxwin.cx = l;
               EnableToolbarUndoBtn(1);
            } /* endif */
         } else if (ulEvent == SPBN_KILLFOCUS) {
            p->po->maxwin.cx = p->ocur.maxwin.cx;
         } /* endif */
         break;
      case SPIN_MXHEIGHT:     // spinbutton cy
         if (ulEvent == SPBN_CHANGE) {
            LONG l;
            WinSendDlgItemMsg(hwnd, idCtl, SPBM_QUERYVALUE, (MPARAM)&l,
                              MPFROM2SHORT(0, SPBQ_ALWAYSUPDATE));
            if (l != p->ocur.maxwin.cy) {
               p->ocur.maxwin.cy = l;
               EnableToolbarUndoBtn(1);
            } /* endif */
         } else if (ulEvent == SPBN_KILLFOCUS) {
            p->po->maxwin.cy = p->ocur.maxwin.cy;
         } /* endif */
         break;
   } /* endswitch */
}


//==========================================================================
// processa il messaggio WM_COMMAND relativo bottoni Undo e Default
// HWND hwnd: handle dialogo
// ULONG idCtl: identit… controllo
// valore restituito:
// VOID
//==========================================================================

VOID MaxWinCommand(HWND hwnd, ULONG idCtl) {
   switch (idCtl) {
      // bottoni toolbar ---------------------------------------------------
      case BTN_UNDO:
         MaxWinUndo(hwnd);
         break;
      case BTN_DEFAULT:
         MaxWinDefault(hwnd);
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

VOID MaxWinSetMainDep(HWND hwnd, BOOL flag) {
   WinCheckButton(hwnd, CHK_MXOVERSZPOS, flag);
   WinEnableControl(hwnd, TXT_MXX, flag);
   WinEnableControl(hwnd, SPIN_MXX, flag);
   WinEnableControl(hwnd, TXT_MXY, flag);
   WinEnableControl(hwnd, SPIN_MXY, flag);
   WinEnableControl(hwnd, TXT_MXWIDHT, flag);
   WinEnableControl(hwnd, SPIN_MXWIDHT, flag);
   WinEnableControl(hwnd, TXT_MXHEIGHT, flag);
   WinEnableControl(hwnd, SPIN_MXHEIGHT, flag);
}


// undo

VOID MaxWinUndo(HWND hwnd) {
   p->pd[p->idxPage].undo = 0;     // disabilita undo
   WinEnableWindow(p->swpCtls[4].hwnd, FALSE);
   // bottone abilitazione/disabilitazione
   p->po->maxwin.on = p->ocur.maxwin.on = p->ou.maxwin.on;
   // posizione/dimensioni finestre massimizzate
   p->po->maxwin.x = p->ocur.maxwin.x = p->ou.maxwin.x;
   p->po->maxwin.y = p->ocur.maxwin.y = p->ou.maxwin.y;
   p->po->maxwin.cx = p->ocur.maxwin.cx = p->ou.maxwin.cx;
   p->po->maxwin.cy = p->ocur.maxwin.cy = p->ou.maxwin.cy;
   // spostamento/ridimensionamento finestre massimizzate
   p->po->maxwin.nomove = p->ocur.maxwin.nomove = p->ou.maxwin.nomove;
   p->po->maxwin.nosize = p->ocur.maxwin.nosize = p->ou.maxwin.nosize;
   // aggiorna controlli dialogo
   MaxWinInit(hwnd, &p->ocur);
}


// default

VOID MaxWinDefault(HWND hwnd) {
   EnableToolbarUndoBtn(1);      // abilita undo
   // bottone abilitazione/disabilitazione
   p->po->maxwin.on = p->ocur.maxwin.on = 0;
   // posizione/dimensioni finestre massimizzate
   p->po->maxwin.x = p->ocur.maxwin.x = - p->ptlBrd.x;
   p->po->maxwin.y = p->ocur.maxwin.y = - p->ptlBrd.y;
   p->po->maxwin.cx = p->ocur.maxwin.cx = uGetSval(SV_CXFULLSCREEN) +
                                      2 * p->ptlBrd.x;
   p->po->maxwin.cy = p->ocur.maxwin.cy = uGetSval(SV_CYFULLSCREEN) +
                                      2 * p->ptlBrd.y;
   // spostamento/ridimensionamento finestre massimizzate
   p->po->maxwin.nomove = p->ocur.maxwin.nomove = 0;
   p->po->maxwin.nosize = p->ocur.maxwin.nosize = 0;
   // aggiorna controlli dialogo
   if (hwnd) MaxWinInit(hwnd, &p->ocur);
}
