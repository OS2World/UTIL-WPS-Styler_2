//==========================================================================
// trackframe.c: full window drag implementation for Warp 3
//==========================================================================


#pragma strings(readonly)

#include "stlrMain.h"

// prototypes ---------------------------------------------------------------

VOID trkArrowKey(PQMSG pqmsg, PFULLWDRAG pfwd);
VOID trkmouse(PQMSG pqmsg, PFULLWDRAG pfwd);
VOID trkDeltaLeft(LONG dx, PFULLWDRAG pfwd);
VOID trkDeltaRight(LONG dx, PFULLWDRAG pfwd);
VOID trkDeltaBottom(LONG dy, PFULLWDRAG pfwd);
VOID trkDeltaTop(LONG dy, PFULLWDRAG pfwd);


// defines ------------------------------------------------------------------

// coordinate finestra
#define xL        pfwd->rclTrack.xLeft    // x(0) bordo sinistro
#define xR        pfwd->rclTrack.xRight   // x(1) bordo destro
#define cxW       (xR - xL)                   // larghezza finestra
#define xC        (xL + cxW / 2)              // coordinata x centro finestra
#define yB        pfwd->rclTrack.yBottom  // y bordo inferiore
#define yT        pfwd->rclTrack.yTop     // y bordo superiore
#define cyW       (yT - yB)                   // altezza finestra
#define yC        (yB + cyW / 2)              // coordinata y centro finestra
// larghezza bordo ridimensionamento obliquo
#define CXBORDER  (pfwd->p->ptBrd.x + pfwd->p->wrclTitle.cy)
#define CXKBD     pfwd->cxKeyboard        // spostamento x con tastiera
#define CYKBD     pfwd->cyKeyboard        // spostamento y con tastiera
#define PNTx      pfwd->ptl.x             // coordinata x mouse
#define PNTy      pfwd->ptl.y             // coordinata y mouse
#define MAXcx     pfwd->ptlMaxTrackSize.x // massima larghezza finestra
#define MAXcy     pfwd->ptlMaxTrackSize.y // massima altezza finestra
#define MINcx     pfwd->ptlMinTrackSize.x // minima larghezza finestra
#define MINcy     pfwd->ptlMinTrackSize.y // minima altezza finestra
#define cxFRAME   pfwd->p->ptBrd.x      // larghezza bordo finestra
#define cyFRAME   pfwd->p->ptBrd.y      // altezza bordo finestra
#define cyTITLE   pfwd->p->wrclTitle.cy // altezza titolo finestra


//===========================================================================
// Handle full window dragging for Warp 3.
// Parameters --------------------------------------------------------------
// HWND hwnd       : window handle.
// PFULLWDRAG pfwd : full window drag structure.
// PSTLRFRAME p : windows data.
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (tracking succesfull / tracking cancelled)
//===========================================================================

BOOL stlrTrackWindow(HWND hwnd, PFULLWDRAG pfwd, PSTLRFRAME p) {
   // setta msg mouse che termina operazione tracking
   pfwd->msg = (g.is.mousebtn & DOWN1) ? WM_BUTTON1UP :
                ((g.is.mousebtn & DOWN2) ? WM_BUTTON2UP :
                ((g.is.mousebtn & DOWN3)? WM_BUTTON3UP : 0));
   pfwd->p = p;
   pfwd->wr = p->rclCur;
   // inizia loop hook MSGFILTER monitorando msg mouse e tastiera
   WinSetCapture(g.hwnd.desktop, hwnd);
   pfwd->hwndModal = WinQuerySysModalWindow(g.hwnd.desktop);
   WinSetSysModalWindow(g.hwnd.desktop, hwnd);
   WinSetHook(p->hab, HMQ_CURRENT, HK_MSGFILTER, (PFN)trkhook, 0);
   while (WinGetMsg(p->hab, &pfwd->qmsg, NULLHANDLE, 0, 0)) {
      if (p->is.trkquit) {
         WinDispatchMsg(p->hab, &pfwd->qmsg);
         break;
      } /* endif */
      if (!WinCallMsgFilter(p->hab, &pfwd->qmsg, (ULONG)pfwd))
         WinDispatchMsg(p->hab, &pfwd->qmsg);
   } /* endif */
   WinReleaseHook(p->hab, HMQ_CURRENT, HK_MSGFILTER, (PFN)trkhook, 0);
   WinSetSysModalWindow(g.hwnd.desktop, pfwd->hwndModal);
   WinSetCapture(g.hwnd.desktop, NULLHANDLE);
   return ((pfwd->fs & 0xffff) != 0);
}


//===========================================================================
// Full window drag track hook procedure.
// Parameters --------------------------------------------------------------
// HAB hab     : anchor block handle.
// PQMSG pqmsg : message queue.
// PFULLWDRAG pfwd : full window drag structure.
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (don't pass / pass the message to the next hook).
//===========================================================================

BOOL _System trkhook(HAB hab, PQMSG pqmsg, PFULLWDRAG pfwd) {
   switch (pqmsg->msg) {
      case WM_CHAR:
         if ((USHORT)pqmsg->mp1 & KC_KEYUP) break;
         switch (SHORT2FROMMP(pqmsg->mp2)) {
            // cancella spostamento finestra !!! NON CAMBIARE ORDINE !!!
            case VK_ESC:
               WinSetWindowPos(pfwd->p->hwnd, NULLHANDLE,
                               pfwd->wr.x, pfwd->wr.y, pfwd->wr.cx, pfwd->wr.cy,
                               SWP_SIZE | SWP_MOVE);
               pfwd->fs = 0;
               pfwd->p->is.trkquit = 1;
            // fissa spostamento finestra e termina (se non fosse full drag
            case VK_NEWLINE:      // dovrebbe spostare la finestra)
            case VK_ENTER:
               pfwd->p->is.trkquit = 1;
               break;
            // sposta puntatore mouse, finestra o ridimensiona la finestra
            case VK_LEFT:        // secondo valore di pti->fs
            case VK_UP:
            case VK_RIGHT:
            case VK_DOWN:
               trkArrowKey(pqmsg, pfwd);
               break;
         } /* endswitch */
         break;
      // sposta o ridimensiona la finestra controllando che non vengano
      // superati i limiti, se move controlla che il mouse sia sul giusto
      // lato prima di dare inizio al movimento
      case WM_MOUSEMOVE:
         if (PNTx == pqmsg->ptl.x &&
             PNTy == pqmsg->ptl.y) break;
         trkmouse(pqmsg, pfwd);
         break;
      // se tracking iniziato da tastiera memorizza tasto mouse premuto
      case WM_BUTTON1DOWN:
      case WM_BUTTON2DOWN:
      case WM_BUTTON3DOWN:
         if (!pfwd->msg) pfwd->msg = pqmsg->msg + 1;
         break;
      // se msg mouse corrispondente a msg che ha iniziato il tracking termina
      case WM_BUTTON1UP:
      case WM_BUTTON2UP:
      case WM_BUTTON3UP:
         if (pqmsg->msg == pfwd->msg) pfwd->p->is.trkquit = 1;
         break;
      default:
         return FALSE;
   } /* endswitch */
   return TRUE;
}


//===========================================================================
// Handle window tracking keyboard events.
// Parameters --------------------------------------------------------------
// PQMSG pqmsg : message queue handle.
// PFULLWDRAG pfwd : full window drag structure.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID trkArrowKey(PQMSG pqmsg, PFULLWDRAG pfwd) {
   // spostamento finestra memorizza scostamenti e offsetta il rettangolo
   if ((pfwd->fs & TF_MOVE) == TF_MOVE) {
      switch (SHORT2FROMMP(pqmsg->mp2)) {
         case VK_LEFT : PNTx = -CXKBD; PNTy = 0;      break;
         case VK_UP   : PNTx = 0;      PNTy = CYKBD;  break;
         case VK_RIGHT: PNTx = CXKBD;  PNTy = 0;      break;
         case VK_DOWN : PNTx = 0;      PNTy = -CYKBD; break;
      } /* endswitch */
//      if (pfwd->p->is.shield)
//         WinQueryPointerPos(g.hwnd.desktop, &pqmsg->ptl);
      xL += PNTx; xR += PNTx; PNTx += pqmsg->ptl.x;
      yB += PNTy; yT += PNTy; PNTy += pqmsg->ptl.y;
   // ridimensionamento finestra
   } else {
      switch (SHORT2FROMMP(pqmsg->mp2)) {
         case VK_LEFT:
            if (pfwd->fs & TF_LEFT) {
               trkDeltaLeft(-CXKBD, pfwd);
            } else if (pfwd->fs & TF_RIGHT) {
               trkDeltaRight(-CXKBD, pfwd);
            } else {
               _setSysMouPtr((pfwd->fs & TF_TOP)? SPTR_SIZENWSE:
                      ((pfwd->fs & TF_BOTTOM)? SPTR_SIZENESW: SPTR_SIZEWE));
               pfwd->fs |= TF_LEFT;
               goto calcmoveptr;
            } /* endif */
            break;
         case VK_UP:
            if (pfwd->fs & TF_TOP) {
               trkDeltaTop(CYKBD, pfwd);
            } else if (pfwd->fs & TF_BOTTOM) {
               trkDeltaBottom(CYKBD, pfwd);
            } else {
               _setSysMouPtr((pfwd->fs & TF_LEFT)? SPTR_SIZENWSE:
                      ((pfwd->fs & TF_RIGHT)? SPTR_SIZENESW: SPTR_SIZENS));
               pfwd->fs |= TF_TOP;
               goto calcmoveptr;
            } /* endif */
            break;
         case VK_RIGHT:
            if (pfwd->fs & TF_LEFT) {
               trkDeltaLeft(CXKBD, pfwd);
            } else if (pfwd->fs & TF_RIGHT) {
               trkDeltaRight(CXKBD, pfwd);
            } else {
               _setSysMouPtr((pfwd->fs & TF_TOP)? SPTR_SIZENESW:
                      ((pfwd->fs & TF_BOTTOM)? SPTR_SIZENWSE: SPTR_SIZEWE));
               pfwd->fs |= TF_RIGHT;
               goto calcmoveptr;
            } /* endif */
            break;
         case VK_DOWN:
            if (pfwd->fs & TF_TOP) {
               trkDeltaTop(-CYKBD, pfwd);
            } else if (pfwd->fs & TF_BOTTOM) {
               trkDeltaBottom(-CYKBD, pfwd);
            } else {
               _setSysMouPtr((pfwd->fs & TF_LEFT)? SPTR_SIZENESW:
                      ((pfwd->fs & TF_RIGHT)? SPTR_SIZENWSE: SPTR_SIZENS));
               pfwd->fs |= TF_BOTTOM;
               goto calcmoveptr;
            } /* endif */
            break;
      } /* endswitch */
   } /* endif */
   if ((pfwd->fs & TF_MOVE) == TF_MOVE) {
      WinSetWindowPos(pfwd->p->hwnd, 0, xL, yB, cxW, cyW, SWP_MOVE);
      goto moveptr;
   } /* endif */
   WinSetWindowPos(pfwd->p->hwnd, 0, xL, yB, cxW, cyW, SWP_SIZE | SWP_MOVE);
calcmoveptr:
   PNTx = (pfwd->fs & TF_LEFT)? xL: ((pfwd->fs & TF_RIGHT)? xR - 1: xC);
   PNTy = (pfwd->fs & TF_BOTTOM)? yB: ((pfwd->fs & TF_TOP)? yT - 1: yC);
   WinMapWindowPoints(pfwd->hpar, g.hwnd.desktop, &pfwd->ptl, 1);
moveptr:
   WinSetPointerPos(g.hwnd.desktop, PNTx, PNTy);
}


//===========================================================================
// Handle window tracking mouse events
// Parameters --------------------------------------------------------------
// PQMSG pqmsg : message queue handle.
// PFULLWDRAG pfwd : full window drag structure.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID trkmouse(PQMSG pqmsg, PFULLWDRAG pfwd) {
   // spostamento finestra
   if ((pfwd->fs & TF_MOVE) == TF_MOVE) {
      xL += pqmsg->ptl.x - PNTx;
      xR += pqmsg->ptl.x - PNTx;
      yB += pqmsg->ptl.y - PNTy;
      yT += pqmsg->ptl.y - PNTy;
      // sposta la finestra
      WinSetWindowPos(pfwd->p->hwnd, 0, xL, yB, cxW, cyW, SWP_MOVE);
      goto end;
   // se ridimensionamento iniziato da tastiera non esegue alcuna azione
   // finch‚ il mouse non Š sul bordo della finestra
   } else if (!(pfwd->fs & TF_MOVE)) {
      ULONG idptr = 0;  // id nuova icona usata per puntatore mouse
      // altezza bordo ridimensionamento obliquo varia se l'altezza della
      // finestra Š ridotta
      ULONG cybrd = cyFRAME + cyTITLE;
      if (cyW < (3 * cybrd)) cybrd = cyW / 3;
      if (MOUSEX(pqmsg->mp1) < cxFRAME) {
         if (MOUSEY(pqmsg->mp1) < cybrd) {
            pfwd->fs |= TF_LEFT | TF_BOTTOM;
            idptr = SPTR_SIZENESW;
         } else if (MOUSEY(pqmsg->mp1) > (cyW - cybrd)) {
            pfwd->fs |= TF_LEFT | TF_TOP;
            idptr = SPTR_SIZENWSE;
         } else {
            pfwd->fs |= TF_LEFT;
            idptr = SPTR_SIZEWE;
         } /* endif */
      } else if (MOUSEX(pqmsg->mp1) > (cxW - cxFRAME)) {
         if (MOUSEY(pqmsg->mp1) < cybrd) {
            pfwd->fs |= TF_RIGHT | TF_BOTTOM;
            idptr = SPTR_SIZENWSE;
         } else if (MOUSEY(pqmsg->mp1) > (cyW - cybrd)) {
            pfwd->fs |= TF_RIGHT | TF_TOP;
            idptr = SPTR_SIZENESW;
         } else {
            pfwd->fs |= TF_RIGHT;
            idptr = SPTR_SIZEWE;
         } /* endif */
      } else if (MOUSEY(pqmsg->mp1) < cyFRAME) {
         if (MOUSEX(pqmsg->mp1) < CXBORDER) {
            pfwd->fs |= TF_BOTTOM | TF_LEFT;
            idptr = SPTR_SIZENESW;
         } else if (MOUSEX(pqmsg->mp1) > (cxW - CXBORDER)) {
            pfwd->fs |= TF_BOTTOM | TF_RIGHT;
            idptr = SPTR_SIZENWSE;
         } else {
            pfwd->fs |= TF_BOTTOM;
            idptr = SPTR_SIZENS;
         } /* endif */
      } else if (MOUSEY(pqmsg->mp1) > (cyW - cyFRAME)) {
         if (MOUSEX(pqmsg->mp1) < CXBORDER) {
            pfwd->fs |= TF_TOP | TF_LEFT;
            idptr = SPTR_SIZENWSE;
         } else if (MOUSEX(pqmsg->mp1) > (cxW - CXBORDER)) {
            pfwd->fs |= TF_TOP | TF_RIGHT;
            idptr = SPTR_SIZENESW;
         } else {
            pfwd->fs |= TF_TOP;
            idptr = SPTR_SIZENS;
         } /* endif */
      } /* endif */
      if (idptr) _setSysMouPtr(idptr);
      pfwd->ptl = pqmsg->ptl;
      return;
   // ridimensionamento
   } else {
      if (pfwd->fs & TF_LEFT) {
         if ((MOUSEX(pqmsg->mp1) < cxFRAME && pqmsg->ptl.x < PNTx) ||
             (MOUSEX(pqmsg->mp1) >= 0 && pqmsg->ptl.x > PNTx))
            trkDeltaLeft(pqmsg->ptl.x - PNTx, pfwd);
      } else if (pfwd->fs & TF_RIGHT) {
         if ((MOUSEX(pqmsg->mp1) > (cxW - cxFRAME) && pqmsg->ptl.x > PNTx) ||
             (MOUSEX(pqmsg->mp1) <= cxW && pqmsg->ptl.x < PNTx))
            trkDeltaRight(pqmsg->ptl.x - PNTx, pfwd);
      } /* endif */
      if (pfwd->fs & TF_TOP) {
         if ((MOUSEY(pqmsg->mp1) > (cyW - cyFRAME) && pqmsg->ptl.y > PNTy) ||
             (MOUSEY(pqmsg->mp1) <= cyW && pqmsg->ptl.y < PNTy))
            trkDeltaTop(pqmsg->ptl.y - PNTy, pfwd);
      } else if (pfwd->fs & TF_BOTTOM) {
         if ((MOUSEY(pqmsg->mp1) < cyFRAME && pqmsg->ptl.y < PNTy) ||
             (MOUSEY(pqmsg->mp1) >= 0 && pqmsg->ptl.y > PNTy))
            trkDeltaBottom(pqmsg->ptl.y - PNTy, pfwd);
      } /* endif */
   } /* endif */
   // sposta la finestra
   WinSetWindowPos(pfwd->p->hwnd, 0, xL, yB, cxW, cyW, SWP_SIZE | SWP_MOVE);
end:
   pfwd->ptl = pqmsg->ptl;  // aggiorna posizione corrente mouse
}


//===========================================================================
// Update the tracking rectangle according to the movement of the left
// window border. dx is negative when the border is moved to the left
// and positive when the border is moved to the right.
// Parameters --------------------------------------------------------------
// ULONG dx : border movement in pixels.
// PFULLWDRAG pfwd : full window drag structure.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
_Inline
VOID trkDeltaLeft(LONG dx, PFULLWDRAG pfwd) {
   LONG newcx = cxW - dx;
   xL = newcx > MAXcx? xR - MAXcx: (newcx < MINcx? xR - MINcx: xL + dx);
}


//===========================================================================
// Update the tracking rectangle according to the movement of the right
// window border. dx is negative when the border is moved to the left
// and positive when the border is moved to the right.
// Parameters --------------------------------------------------------------
// ULONG dx : border movement in pixels.
// PFULLWDRAG pfwd : full window drag structure.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
_Inline
VOID trkDeltaRight(LONG dx, PFULLWDRAG pfwd) {
   LONG newcx = cxW + dx;
   xR = newcx > MAXcx? xL + MAXcx: (newcx < MINcx? xL + MINcx: xR + dx);
}


//===========================================================================
// Update the tracking rectangle according to the movement of the bottom
// window border. dy is negative when the border is moved to the bottom
// and positive when the border is moved to the top.
// Parameters --------------------------------------------------------------
// ULONG dy : border movement in pixels.
// PFULLWDRAG pfwd : full window drag structure.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
_Inline
VOID trkDeltaBottom(LONG dy, PFULLWDRAG pfwd) {
   LONG newcy = cyW - dy;
   yB = newcy > MAXcy? yT - MAXcy: (newcy < MINcy? yT - MINcy: yB + dy);
}


//===========================================================================
// Update the tracking rectangle according to the movement of the top
// window border. dy is negative when the border is moved to the bottom
// and positive when the border is moved to the top.
// Parameters --------------------------------------------------------------
// ULONG dy : border movement in pixels.
// PFULLWDRAG pfwd : full window drag structure.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
_Inline
VOID trkDeltaTop(LONG dy, PFULLWDRAG pfwd) {
   LONG newcy = cyW + dy;
   yT = newcy > MAXcy? yB + MAXcy: (newcy < MINcy? yB + MINcy: yT + dy);
}
