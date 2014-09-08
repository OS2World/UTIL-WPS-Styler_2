//===========================================================================
// mle.c: mle process initialization callback and enhanced WC_MLE procedure.
//===========================================================================


#pragma strings(readonly)

// includes -----------------------------------------------------------------

#include "stlrMain.h"
#include "stlrError.h"


// prototypes ---------------------------------------------------------------

VOID _Export APIENTRY stlrProcessInit(VOID);


// data segment definition --------------------------------------------------

#pragma data_seg(STLRINSTANCE_DATA)


// global variables ---------------------------------------------------------

static PPROCDATA ppd;
extern PGLOBAL pg;
extern POPTIONS po;
extern CLASSDATA cdMLE;


// defines ------------------------------------------------------------------

#define _isOptionActive() \
   (ppd->ppib && !ppd->we.txtctl && !po->gen.disabled && \
    po->txt.on && po->txt.mle)

#define _isCopyEvent(_msg_, _mp_) \
   (((_msg_) == po->txt.cpyMoumsg) && (SHORT2FROMMP(_mp_) == po->txt.cpyMoukbd))

#define _isCutEvent(_msg_, _mp_) \
   (((_msg_) == po->txt.cutMoumsg) && (SHORT2FROMMP(_mp_) == po->txt.cutMoukbd))

#define _isPasteInsertEvent(_msg_, _mp_) \
   (((_msg_) == po->txt.insMoumsg) && (SHORT2FROMMP(_mp_) == po->txt.insMoukbd))

#define _isPasteOverwriteEvent(_msg_, _mp_) \
   (((_msg_) == po->txt.ovwMoumsg) && (SHORT2FROMMP(_mp_) == po->txt.ovwMoukbd))


//===========================================================================
// This procedure is exported as ordinal @1 and is automatically called by
// the system on creation of a PM message queue.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// VOID
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
// VOID
//===========================================================================

VOID _Export APIENTRY stlrProcessInit(VOID) {
   ppd = stlrProcessData();
}


//===========================================================================
// Superclassed MLE procedure.
// Parameters --------------------------------------------------------------
// ordinary window procedure parameters.
// Return value ------------------------------------------------------------
// MRESULT : message result depending on the message and window state.
//===========================================================================

MRESULT EXPENTRY stlrMLEproc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   if (_isOptionActive()) {
      // override the default procedure for the mouse-button-down messages
      // corresponding to the event selected for Styler/2 enhancements
      if ((msg == po->txt.skipcpy) && (SHORT2FROMMP(mp2) == po->txt.cpyMoukbd))
         return (MRESULT)FALSE;
      if ((msg == po->txt.skipcut) && (SHORT2FROMMP(mp2) == po->txt.cutMoukbd))
         return (MRESULT)FALSE;
      if ((msg == po->txt.skippaste) && (SHORT2FROMMP(mp2) == po->txt.insMoukbd))
         return (MRESULT)FALSE;
      if ((msg == po->txt.skipovwrt) && (SHORT2FROMMP(mp2) == po->txt.ovwMoukbd))
         return (MRESULT)FALSE;
      // copy selected text to the clipboard
      if (_isCopyEvent(msg, mp2))
         return cdMLE.pfnwp(hwnd, MLM_COPY, MPVOID, MPVOID);
      // cut selected text copying it to the clipboard
      if (_isCutEvent(msg, mp2))
         return cdMLE.pfnwp(hwnd, MLM_CUT, MPVOID, MPVOID);
      // paste clipboard content without overwriting the text
      if (_isPasteInsertEvent(msg, mp2)) {
         // unselect the text without moving the cursor
         cdMLE.pfnwp(hwnd, MLM_SETSEL,
                     (MPARAM)cdMLE.pfnwp(hwnd, MLM_QUERYSEL,
                                         (MPARAM)MLFQS_CURSORSEL, MPVOID),
                     (MPARAM)-1);
         cdMLE.pfnwp(hwnd, MLM_PASTE, MPVOID, MPVOID);
         return (MRESULT)TRUE;
      } /* endif */
      // paste the clipboard content overwriting the selected text
      if (_isPasteOverwriteEvent(msg, mp2))
         return cdMLE.pfnwp(hwnd, MLM_PASTE, MPVOID, MPVOID);
   } /* endif */
   return (MRESULT)cdMLE.pfnwp(hwnd, msg, mp1, mp2);
}
