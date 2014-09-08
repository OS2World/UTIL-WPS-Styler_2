//==========================================================================\
// button.c: nuova procedura WC_BUTTON                                      |
// funzioni : ------------------------------------------------------------- |
// MRESULT EXPENTRY stlrButtonProc(HWND hwnd, ULONG msg,
//                                    MPARAM mp1, MPARAM mp2);
// messaggi : ------------------------------------------------------------- |
//==========================================================================/


#pragma strings(readonly)

#include "stlrMain.h"
extern PROCDATA pd;

// prototipi funzioni

//==========================================================================\
// nuova procedura bottoni                                                  |
//==========================================================================/

MRESULT EXPENTRY stlrButtonProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   if (msg == WM_MOUSEMOVE) {
      if (g.is.onctrl != OCTLBTN) {
         g.is.onctrl = OCTLBTN;
         _resetHideMouTimer();
      } /* endif */
   } else if (msg == o.txt.skipcpy) {
      // non fa intercettare alla procedura di default il WM_BUTTONxDOWN
      // corrispondente ad evento selezionato per copia nella clipboard
      if ((SHORT2FROMMP(mp2) == o.txt.cpyMoukbd) &&
          pd.ppib && !pd.we.txtctl && !o.gen.disabled &&
          o.txt.on && o.txt.btn)
         return (MRESULT)FALSE;
   } else if (msg == o.txt.cpyMoumsg) {
      if ((SHORT2FROMMP(mp2) == o.txt.cpyMoukbd) &&
          pd.ppib && !pd.we.txtctl && !o.gen.disabled &&
          o.txt.on && o.txt.btn) {
         winTextToClipboard(hwnd);
         return (MRESULT)TRUE;
      } /* endif */
   } /* endif */
defaultproc:
   return (MRESULT)g.cd.btn.pfnwp(hwnd, msg, mp1, mp2);
}
