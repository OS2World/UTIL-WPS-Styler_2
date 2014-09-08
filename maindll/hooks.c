//==========================================================================\
// hooks.c: input hook x disabilitare elaborazione WM_HITTEST quando un     |
//          tasto del mouse Š premuto e x rimappare il tasto centrale del   |
//          mouse                                                           |
// opzioni processate:                                                      |
//                     o.mou.hide : nasconde/mostra puntatore mouse    |
//                     o.mou.btn3dblClk    : rimappa click tasto centrale       |
// BOOL EXPENTRY postMsgHook(HAB hab, PQMSG pqmsg, USHORT fs);
//==========================================================================/


#pragma strings(readonly)
#pragma import(W32ShowPointer,,"PMMERGE",10018)

// includes -----------------------------------------------------------------

#include "stlrMain.h"


// global variables ---------------------------------------------------------

extern PROCDATA pd;

// defines ------------------------------------------------------------------

#define _frameData(_hwnd_)       (WinQueryWindowPtr(_hwnd_, g.cd.frame.cbData))
#define WM_SYSTEMSHUTDOWN        0x0054


// prototypes ---------------------------------------------------------------

int _System W32ShowPointer(BOOL flag);
VOID resetShiftKey(BOOL bUnShift);
VOID startHideMouPtrTimer(PQMSG pqmsg);
VOID checkMouseMotionForSlidingFocus(PQMSG pqmgs);


//===========================================================================
// Intercept posted messages.
// Parameters --------------------------------------------------------------
// HAB hab     : anchor block handle.
// PQMSG pqmsg : message queue.
// USHORT fs   : ignored.
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (break hook chain/pass the message to the hook chain).
//===========================================================================

BOOL EXPENTRY postMsgHook(HAB hab, PQMSG pqmsg, USHORT fs) {
   if (o.gen.disabled) return FALSE;
   switch (pqmsg->msg) {
      // hooks stato pulsanti mouse
      case WM_BUTTON1DOWN:
         if (!pd.ppib || pd.we.mouse) return FALSE;
         if (!g.is.mousebtn) //g.is.suspendHitTest = 1;
            stlrStopHitTestCheck();
         g.is.mousebtn |= DOWN1;
         break;
      case WM_BUTTON2DOWN:
         if (!pd.ppib || pd.we.mouse) return FALSE;
         if (!g.is.mousebtn) //g.is.suspendHitTest = 1;
            stlrStopHitTestCheck();
         g.is.mousebtn |= DOWN2;
         break;
      case WM_BUTTON3DOWN:
         if (!pd.ppib || pd.we.mouse) return FALSE;
         if (!g.is.mousebtn) //g.is.suspendHitTest = 1;
            stlrStopHitTestCheck();
         g.is.mousebtn |= DOWN3;
         break;
      case WM_BUTTON1UP:
         g.is.mousebtn &= ~DOWN1;
         if (!pd.ppib || pd.we.mouse) return FALSE;
         // se puntatore mouse non visibile mostra puntatore mouse
         unhideMouPtr();
         if (!g.is.mousebtn && o.mou.hide) startHideMouPtrTimer(pqmsg);
         break;
      case WM_BUTTON2UP:
         g.is.mousebtn &= ~DOWN2;
         if (!pd.ppib || pd.we.mouse) return FALSE;
         // se puntatore mouse non visibile mostra puntatore mouse
         unhideMouPtr();
         if (!g.is.mousebtn && o.mou.hide) startHideMouPtrTimer(pqmsg);
         break;
      case WM_BUTTON3UP:
         g.is.mousebtn &= ~DOWN3;
         if (!pd.ppib || pd.we.mouse) return FALSE;
         // se puntatore mouse non visibile mostra puntatore mouse
         unhideMouPtr();
         if (!g.is.mousebtn && o.mou.hide) startHideMouPtrTimer(pqmsg);
         break;
      // mouse motion event:
      // this is used for mouse pointer hiding and sliding focus
      case WM_MOUSEMOVE:
         // mouse moved to a different window
         if (pqmsg->hwnd != g.hwnd.mou) {
            // sliding focus handling
            checkMouseMotionForSlidingFocus(pqmsg);
            g.hwnd.mou = pqmsg->hwnd;
            g.is.onctrl = 0;
         } /* endif */
         if (!pd.ppib || pd.we.mouse) break;
         // se le coordinate x,y sono invariate rispetto alle precedenti
         if (g.mou.mpMove == pqmsg->mp1) break;
         // se puntatore mouse non visibile mostra puntatore mouse
         if (g.is.ptrHidden) unhideMouPtr();
         g.mou.mpMove = pqmsg->mp1;// memorizza posizione mouse
         // se non ci sono tasti mouse premuti: se opzioni esclusione menu
         // o bottone controlla classe appartenenza: se opzioni attive e
         if (o.mou.hide && !g.is.mousebtn)
            startHideMouPtrTimer(pqmsg);
         break;
      case WM_BUTTON3CLICK:
         if (!pd.ppib || pd.we.mouse) return FALSE;
         if (o.mou.btn3dblClk) {
            // se la finestra Š bottone, entryfield o titlebar (e non
            // remapping on titlebar) non converte in doppio click
            if ((g.is.onctrl == OCTLBTN) || (g.is.onctrl == OCTLEF) ||
                ((g.is.onctrl == OCTLTBAR) && !o.mou.btn3title))
               break;
            // posta sequenza msg simulanti BUTTON1DBLCLK
            if (SHORT2FROMMP(pqmsg->mp2) & KC_SHIFT) resetShiftKey(TRUE);
            WinPostMsg(pqmsg->hwnd, WM_BUTTON1DOWN, pqmsg->mp1, MPVOID);
            WinPostMsg(pqmsg->hwnd, WM_BUTTON1UP, pqmsg->mp1, MPVOID);
            WinPostMsg(pqmsg->hwnd, WM_SINGLESELECT, pqmsg->mp1, MPVOID);
            WinPostMsg(pqmsg->hwnd, WM_BUTTON1DBLCLK, pqmsg->mp1, pqmsg->mp2);
            WinPostMsg(pqmsg->hwnd, WM_OPEN, pqmsg->mp1, pqmsg->mp2);
            WinPostMsg(pqmsg->hwnd, WM_BUTTON1UP, pqmsg->mp1, pqmsg->mp2);
            pqmsg->msg = WM_NULL;
         } /* endif */
         break;
      case WM_BUTTON1DBLCLK:
         if (g.is.unshift) resetShiftKey(FALSE);
         break;
      case WM_SYSCOMMAND: // se == SC_CLOSE usa elaborazione WM_QUIT
         if ((USHORT)pqmsg->mp1 != SC_CLOSE) break;
      case WM_QUIT:
         // se opzione rolling e autoroll on close attiva srotola finestra
         if (pd.ppib && o.roll.on && o.roll.unrollOnClose && pqmsg->hwnd) {
            _stlrMsgSend(pqmsg->hwnd, STLR_AUTOUNRLONCLS, 0);
         } // end if
         break;
   } /* endswitch */
   return FALSE;
}


//===========================================================================
// Used during double click emulation to avoid to open multiple objects if
// the shift key is pressed.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// VOID
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
// VOID
//===========================================================================
static
VOID resetShiftKey(BOOL bUnShift) {
   CHAR ach[256];
   g.is.unshift = bUnShift;
   WinSetKeyboardStateTable(g.hwnd.desktop, ach, 0);
   ach[VK_SHIFT] = (!bUnShift) << 7;
   WinSetKeyboardStateTable(g.hwnd.desktop, ach, 1);
}


//===========================================================================
// Hide the mouse pointer.
// Parameters --------------------------------------------------------------
// PQMSG pqmsg : message queue.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID startHideMouPtrTimer(PQMSG pqmsg) {
   if ((o.mou.hideNotOnMenu && (g.is.onctrl == OCTLMENU)) ||
       (o.mou.hideNotOnBtn && (g.is.onctrl == OCTLBTN))) {
      _resetHideMouTimer();
      return;
   } // end if
   g.is.hidePtrTmr = 1;
   WinStartTimer(g.timerThrd.hab, g.timerThrd.hwnd, TID_MOUSEPTR, o.mou.hideDelay);
}


VOID hideMouPtr(VOID) {
   INT i = 100;       // limit loop iterations
   if (0 >= (SHORT)_sysValue(SV_POINTERLEVEL))
      while (--i && (W32ShowPointer(FALSE) >= 0)) ;
   W32ShowPointer(FALSE);
}


//===========================================================================
// make the mouse pointer visible
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID unhideMouPtr(VOID) {
   if (0 < (SHORT)_sysValue(SV_POINTERLEVEL))
      while (W32ShowPointer(TRUE)) ;

//   INT show;
//   if (g.is.ptrHidden) {
//      for (show = _sysValue(SV_POINTERLEVEL);
//           show && (show <= 0x7fff);
//           show = _sysValue(SV_POINTERLEVEL)) {
//         WinShowPointer(HWND_DESKTOP, TRUE);
//      } /* endfor */
//   } /* endif */
//   g.is.ptrHidden = 0;
}


//===========================================================================
// Handle mouse motion events on win32 (odin) windows.
// This is necessary as win32 frames do not handle WM_HITTEST as they have
// no CS_HITTEST style.
// Parameters --------------------------------------------------------------
// PQMSG pqmsg : message queue.
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
// VOID
//===========================================================================
_Inline
VOID checkMouseMotionForSlidingFocus(PQMSG pqmsg) {
   HWND hwnd, hparent;
   PSTLRFRAME p;
   BOOL isFrameWin;
   // get the handle of the main window (i.e. the direct child of HWND_DESKTOP)
   for (hwnd = pqmsg->hwnd; ;hwnd = hparent) {
      if (NULLHANDLE == (hparent = _winParent(hwnd))) return;
      if (hparent == g.hwnd.desktop) break;
   } /* endfor */
   // if the main window is a frame window gets the frame data
   p = (isFrameWin = _isFrameWindow(hwnd)) ? _frameData(hwnd) : NULL;
   // mouse exceptions or general exceptions or not frame window
   if (!pd.ppib || pd.we.mouse || (isFrameWin && (p <= FL_WIN32FRAME))) {
      if (hwnd == g.hwnd.lastCreated) g.hwnd.lastCreated = NULLHANDLE;
      // reset the hide-mouse-pointer timer
      _resetHideMouTimer();
      // if the mouse pointer is hidden show it
      unhideMouPtr();
   // sliding focus feature
   } else if (!isFrameWin
              ||
              (!p->is.winos2skip && !p->is.min && p->is.deskChild)) {
      g.hwnd.mainMouWin = hwnd;
      // if no mouse button is pressed and the sliding focus option is active
      if (!g.is.mousebtn
          && o.mou.focusAuto
          && !(o.mou.focusNoDesk && (hwnd == g.hwnd.deskFldr))) {
         // if the ignore-new-windows option is active and this is the last
         // created window (i.e. the 'new' window) reset the flag
         if (o.mou.focusNoNew && g.hwnd.lastCreated) {
            if (hwnd == g.hwnd.lastCreated) {
               g.hwnd.lastCreated = NULLHANDLE;
            // if the sliding focus timer was already running stop it
            } else if (g.hwnd.autoFocus) {
               _resetAutoFocusTimer();
            } /* endif */
         } else if (hwnd == g.hwnd.active) {
            if (g.hwnd.autoFocus) {
               _resetAutoFocusTimer();
            } /* endif */
         // if this is not the active window restart the auto-focus timer
         } else if (g.hwnd.autoFocus != hwnd) {
            _startAutoFocusTimer(hwnd);
         } /* endif */
      } /* endif */
   } /* endif */
}


//==========================================================================\
// lo scopo della routine di autofocus Š di rilevare l'apertura dei
// submenu o dei menu popup evitando che gli stessi vengano dismessi dalla
// routine di autofocus|
// poi verranno aggiunte funzioni per estendere l'autofocus al drag & drop
// per evitare sbilanciamenti nel loop di initmenu-endmenu azzera il
// contatore ogni volta che
// Nel caso siano aggiunte le estensioni dei folder, controlla qui
//==========================================================================/

VOID EXPENTRY sendMsgHook(HAB hab, PSMHSTRUCT psmh, BOOL fInterTask) {
   if (o.gen.disabled) return;
   // quando viene aperto un submenu o un
   if (psmh->msg == WM_INITMENU) {
      if (!pd.ppib || pd.we.mouse) return;
      if (g.hwnd.menu != (HWND)psmh->mp2) {
         g.hwnd.menu = (HWND)psmh->mp2;
         if (!g.is.cinitmenu++) g.is.mousebtn |= MENUOPENED;
      } /* endif */
   } else if (psmh->msg == WM_MENUEND) {
      // resetta stile menu systema mostrato tramite click su titlebar
      if (g.hwnd.sysMenu == (HWND)psmh->mp2) {
         WinSetWindowBits(g.hwnd.sysMenu, QWL_STYLE, 0, 0x18);
         g.hwnd.sysMenu = NULLHANDLE;
      } /* endif */
      if (!pd.ppib || pd.we.mouse) return;
      if (g.hwnd.menu == (HWND)psmh->mp2) {
         g.hwnd.menu = WinQueryWindow((HWND)psmh->mp2, QW_OWNER);
         if (!--g.is.cinitmenu) g.is.mousebtn &= ~MENUOPENED;
      } /* endif */
#ifdef DEBUGGING
   } else if (psmh->msg == WM_SYSTEMSHUTDOWN) {
      dbgPrintf4("WM_SYSTEMSHUTDOWN: hwnd: %08x, mp1: %08x, mp2: %08x\n",
                 psmh->hwnd, psmh->mp1, psmh->mp2);
#endif
   } /* endif */
}



