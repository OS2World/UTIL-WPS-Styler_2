//===========================================================================
// listbox.c: enhanced WC_LISTBOX procedure.
//===========================================================================


#pragma strings(readonly)

// includes -----------------------------------------------------------------

#include "stlrMain.h"


// global variables ---------------------------------------------------------

extern PROCDATA pd;


// prototypes ---------------------------------------------------------------

VOID copyListBoxItemsToClipBoard(HWND hwnd);


// defines ------------------------------------------------------------------

#define _isOptionActive() \
   (pd.ppib && !pd.we.txtctl && !o.gen.disabled && \
    o.txt.on && o.txt.lbox)

#define _itemText(_hwnd_, _i_, _cb_, _p_) \
   ((SHORT)g.cd.lbox.pfnwp(_hwnd_, LM_QUERYITEMTEXT, \
                           MPFROM2SHORT((_i_), (_cb_)), (MPARAM)(_p_)))

#define _itemTextLength(_hwnd_, _i_) \
   ((SHORT)g.cd.lbox.pfnwp((_hwnd_), LM_QUERYITEMTEXTLENGTH, \
                           (MPARAM)(_i_), MPVOID))

#define _itemSelectedIndex(_hwnd_, _i_) \
   ((SHORT)g.cd.lbox.pfnwp((_hwnd_), LM_QUERYSELECTION, (MPARAM)(_i_), MPVOID))

#define _allocSharedMem(_p_, _cb_) \
   (!DosAllocSharedMem((PPVOID)&(_p_), NULL, (_cb_), FLCLIPMEM))


//===========================================================================
// Enhanced listbox procedure.
// Parameters --------------------------------------------------------------
// Ordinary window procedure parameters.
// Return value ------------------------------------------------------------
// MRESULT : message result.
//===========================================================================

MRESULT EXPENTRY stlrListboxProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   // override the default procedure for the mouse-button-down messages
   // corresponding to the event selected for activating an option
   if (msg == o.txt.skipcpy) {
      if ((SHORT2FROMMP(mp2) == o.txt.cpyMoukbd) && _isOptionActive()) {
         return (MRESULT)FALSE;
      } /* endif */
   // copy selected text to the clipboard
   } else if (msg == o.txt.cpyMoumsg) {
      if ((SHORT2FROMMP(mp2) == o.txt.cpyMoukbd) && _isOptionActive()) {
         copyListBoxItemsToClipBoard(hwnd);
         return (MRESULT)FALSE;
      } /* endif */
   } /* endif */
   return (MRESULT)g.cd.lbox.pfnwp(hwnd, msg, mp1, mp2);
}


//===========================================================================
// Get the text of all the selected items, allocate shared memory, copy the
// text to the clipboard.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID copyListBoxItemsToClipBoard(HWND hwnd) {
   SHORT i;
   PSZ pmem, p;
   INT cb, cbTot, cItems;
   BOOL single;
   // check if this is a single selection or extended/multiple selection listbox
   single = !(WinQueryWindowULong(hwnd, QWL_STYLE)
              & (LS_EXTENDEDSEL | LS_MULTIPLESEL));
   cbTot = 0;
   // count total selected items and needed allocation space
   for (cItems = 0, cbTot = 1, i = _itemSelectedIndex(hwnd, LIT_FIRST);
        i >= 0;
        i = _itemSelectedIndex(hwnd, i)) {
      if ((cb = _itemTextLength(hwnd, i)) < 0) break;
      cItems++;
      cbTot += cb;
      if (single) break;
   } /* endfor */
   // add space for "\r\n" to separate the items
   if (cItems > 1) cbTot += (cItems - 1) * 2;
   // single selection list box
   // allocate shared memory and get the text of the item(s)
   if (cItems && _allocSharedMem(pmem, cbTot)) {
      for (p = pmem, i = _itemSelectedIndex(hwnd, LIT_FIRST);
           i >= 0;
           i = _itemSelectedIndex(hwnd, i)) {
         // in case of error do not copy all items
         if ((cb = _itemText(hwnd, i, cbTot, p)) < 0) break;
         p += cb;
         if (single) break;
         // use "\r\n" to separate the items
         if (--cItems) *p++ = '\r', *p++ = '\n';
      } /* endfor */
      *p = 0;
      copyTextToClipboard(WinQueryAnchorBlock(hwnd), pmem);
   } /* endif */
}


