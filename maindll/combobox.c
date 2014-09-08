//===========================================================================
// combobox.c : enhanced combobox window procedure.
// Use mouse gestures to display the listbox part of the drop down boxes.
// Notes:
// -1) no enhancement is needed when the style is CBS_SIMPLE.
// -2) combination boxes use superclassed entryfields. Two different window
//     procedures are used for the CBS_DROPDOWN and the CBS_DROPDOWNLIST
//     styles.
// To do:
// -1) autoresize listboxes to fit the content.
// -2) change the lisbox position to make it entirely visible.
// -3) prevent disabled combination boxes to respond to WM_CHAR events.
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
PFNWP pDefDropDownEntryProc1, pDefDropDownEntryProc2;


//===========================================================================
// Simulate a mouse button down event on the button part of a drop down list.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static VOID
emulateDropDownEvent(HWND hwnd) {
   RECTL r;
   HWND hParent;
   hParent = _winParent(hwnd);
   WinQueryWindowRect(hParent, &r);
   _mouRelease();
   WinPostMsg(hwnd, WM_BUTTON1UP, g.mou.mpMove, MPVOID);
   WinPostMsg(hParent, WM_BUTTON1DOWN,
              MPFROM2SHORT(r.xRight - 3, r.yTop - 3), MPVOID);
}


//===========================================================================
// Enhanced window procedure used by the entry field part of combination
// boxes of  style.
// If the mouse moved downwards and the vertical movement is greater than
// the horizontal movement the listbox part of the combobox is displayed.
// Parameters --------------------------------------------------------------
// Ordinary window procedure parameters.
// Return value ------------------------------------------------------------
// MRESULT.
//===========================================================================
static MRESULT EXPENTRY
pNewDropDownEntryProc1(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   if ((msg == WM_BUTTON1DOWN) && _isEnhancementOn()) {
      _mouCapture(hwnd);
   } else if ((msg == WM_BUTTON1MOTIONSTART)
       && _isEnhancementOn()
       && ((MOUSEY(mp1) - MOUSEY(g.mou.mpMove))
           > abs(MOUSEX(mp1) - MOUSEX(g.mou.mpMove))))
      emulateDropDownEvent(hwnd);
   return pDefDropDownEntryProc1(hwnd, msg, mp1, mp2);
}


//===========================================================================
// Enhanced window procedure used by the entry field part of combination
// boxes of  style.
// If the mouse moved downwards and the vertical movement is greater than
// the horizontal movement the listbox part of the combobox is displayed.
// Parameters --------------------------------------------------------------
// Ordinary window procedure parameters.
// Return value ------------------------------------------------------------
// MRESULT.
//===========================================================================
static MRESULT EXPENTRY
pNewDropDownEntryProc2(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   if ((msg == WM_BUTTON1DOWN) && _isEnhancementOn()) {
      _mouCapture(hwnd);
   } else if ((msg == WM_BUTTON1UP) && g.is.captured) {
      _mouRelease();
   } else if ((msg == WM_BUTTON1MOTIONSTART)
       && _isEnhancementOn()
       && ((MOUSEY(mp1) - MOUSEY(g.mou.mpMove))
           > abs(MOUSEX(mp1) - MOUSEX(g.mou.mpMove))))
      emulateDropDownEvent(hwnd);
   return pDefDropDownEntryProc2(hwnd, msg, mp1, mp2);
}


//===========================================================================
// Enhanced combobox procedure.
// If the combobox style is CBS_DROPDOWN or CBS_DROPDOWNLIST, the window
// procedure of the entry field part of the combobox is superclassed to
// make it sensitive to "mouse gestures".
// Parameters --------------------------------------------------------------
// Ordinary window procedure parameters.
// Return value ------------------------------------------------------------
// MRESULT.
//===========================================================================
MRESULT EXPENTRY
stlrComboProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   if (msg == WM_CREATE) {
      // execute the default procedure first (in case of error abort)
      if (g.cd.combo.pfnwp(hwnd, msg, mp1, mp2)) return (MRESULT)TRUE;
      // superclass the entryfield part according to the style
      if (((PCREATESTRUCT)(mp2))->flStyle & CBS_DROPDOWN) {
         pDefDropDownEntryProc1 =
             WinSubclassWindow(WinWindowFromID(hwnd, CBID_EDIT),
                               pNewDropDownEntryProc1);
      } else if (((PCREATESTRUCT)(mp2))->flStyle & CBS_DROPDOWNLIST) {
         pDefDropDownEntryProc2 =
             WinSubclassWindow(WinWindowFromID(hwnd, CBID_EDIT),
                               pNewDropDownEntryProc2);
      } /* endif */
      return (MRESULT)FALSE;
   } /* endif */
   return g.cd.combo.pfnwp(hwnd, msg, mp1, mp2);
}
