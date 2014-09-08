//===========================================================================
// static.c: enhanced WC_STATIC window class procedure.
//===========================================================================


#pragma strings(readonly)

// includes -----------------------------------------------------------------

#include "stlrMain.h"


// global variables ---------------------------------------------------------

extern PROCDATA pd;


//===========================================================================
// Enhanced WC_STATIC window class procedure.
// Parameters --------------------------------------------------------------
// Ordinary window procedure parameters.
// Return value ------------------------------------------------------------
// MRESULT : message result.
//===========================================================================

MRESULT EXPENTRY stlrStaticProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   if (msg == o.txt.skipcpy) {
      // non fa intercettare alla procedura di default il WM_BUTTONxDOWN
      // corrispondente all'evento selezionato per copiare il testo
      if ((SHORT2FROMMP(mp2) == o.txt.cpyMoukbd) &&
          pd.ppib && !pd.we.txtctl && !o.gen.disabled &&
          o.txt.on && o.txt.stat &&
          (_winStyle(hwnd) & SS_TEXT))
         return (MRESULT)FALSE;
   } else if (msg == o.txt.cpyMoumsg) {
      if ((SHORT2FROMMP(mp2) == o.txt.cpyMoukbd) &&
          pd.ppib && !pd.we.txtctl && !o.gen.disabled &&
          o.txt.on && o.txt.stat) {
         winTextToClipboard(hwnd);
         return (MRESULT)TRUE;
      } /* endif */
   } /* endif */
   return (MRESULT)g.cd.stat.pfnwp(hwnd, msg, mp1, mp2);
}
