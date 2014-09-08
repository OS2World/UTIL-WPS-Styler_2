//===========================================================================
// entry.c : enhanced entry field window procedure.
//===========================================================================


#pragma strings(readonly)


// includes -----------------------------------------------------------------

#include "stlrMain.h"


// defines ------------------------------------------------------------------

#define _isEnhancementOn() \
   (pd.ppib && !pd.we.txtctl && !o.gen.disabled)

#define _isOptionActive() \
   (o.txt.on && o.txt.ef)

#define _isCopyEvent(_msg_, _mp_) \
   (((_msg_) == o.txt.cpyMoumsg) && (SHORT2FROMMP(_mp_) == o.txt.cpyMoukbd))

#define _isCutEvent(_msg_, _mp_) \
   (((_msg_) == o.txt.cutMoumsg) && (SHORT2FROMMP(_mp_) == o.txt.cutMoukbd))

#define _isPasteInsertEvent(_msg_, _mp_) \
   (((_msg_) == o.txt.insMoumsg) && (SHORT2FROMMP(_mp_) == o.txt.insMoukbd))

#define _isPasteOverwriteEvent(_msg_, _mp_) \
   (((_msg_) == o.txt.ovwMoumsg) && (SHORT2FROMMP(_mp_) == o.txt.ovwMoukbd))

// global variables ---------------------------------------------------------

extern PROCDATA pd;




//===========================================================================
// Enhanced entry field procedure.
// Parameters --------------------------------------------------------------
// Ordinary window procedure parameters.
// Return value ------------------------------------------------------------
// MRESULT.
//===========================================================================

MRESULT EXPENTRY stlrEntryFieldProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   if (_isEnhancementOn()) {
      // select the whole content of the entry field :
      if ((msg == o.txt.selMoumsg) && (SHORT2FROMMP(mp2) == o.txt.selMoukbd)) {
         // give the focus to the entry field if it is not already the focus window
         if (hwnd != WinQueryFocus(HWND_DESKTOP)) WinSetFocus(HWND_DESKTOP, hwnd);
         g.cd.ef.pfnwp(hwnd, EM_SETSEL, MPFROMLONG(0xffff0000), MPVOID);
         return (MRESULT)TRUE;
      } /* endif */
      if (_isOptionActive()) {
         // override the default procedure for the mouse-button-down messages
         // corresponding to the event selected for Styler/2 enhancements
         if ((msg == o.txt.skipcpy) && (SHORT2FROMMP(mp2) == o.txt.cpyMoukbd))
            return (MRESULT)FALSE;
         if ((msg == o.txt.skipcut) && (SHORT2FROMMP(mp2) == o.txt.cutMoukbd))
            return (MRESULT)FALSE;
         if ((msg == o.txt.skippaste) && (SHORT2FROMMP(mp2) == o.txt.insMoukbd))
            return (MRESULT)FALSE;
         if ((msg == o.txt.skipovwrt) && (SHORT2FROMMP(mp2) == o.txt.ovwMoukbd))
            return (MRESULT)FALSE;
         // copy selected text to the clipboard
         if (_isCopyEvent(msg, mp2))
            return g.cd.ef.pfnwp(hwnd, EM_COPY, MPVOID, MPVOID);
         // cut selected text copying it to the clipboard
         if (_isCutEvent(msg, mp2))
            return g.cd.ef.pfnwp(hwnd, EM_CUT, MPVOID, MPVOID);
         // paste clipboard content without overwriting the text
         if (_isPasteInsertEvent(msg, mp2)) {
            MPARAM mp = g.cd.ef.pfnwp(hwnd, EM_QUERYSEL, MPVOID, MPVOID);
            g.cd.ef.pfnwp(hwnd, EM_SETSEL, MPVOID, MPVOID);
            g.cd.ef.pfnwp(hwnd, EM_PASTE, MPVOID, MPVOID);
            g.cd.ef.pfnwp(hwnd, EM_SETSEL, mp, MPVOID);
            return (MRESULT)TRUE;
         } /* endif */
         // paste the clipboard content overwriting the selected text
         if (_isPasteOverwriteEvent(msg, mp2))
            return g.cd.ef.pfnwp(hwnd, EM_PASTE, MPVOID, MPVOID);
      } /* endif */
   } /* endif */
      // set the mouse-over-entry-field flag
   if (msg == WM_MOUSEMOVE) {
      if (g.is.onctrl != OCTLEF) {
         g.is.onctrl = OCTLEF;
         _resetHideMouTimer();
      } /* endif */
   } /* endif */
   return (MRESULT)g.cd.ef.pfnwp(hwnd, msg, mp1, mp2);
}
