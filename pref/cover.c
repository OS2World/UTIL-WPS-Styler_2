//==========================================================================
// hidetitle.c: pagina impostazioni wrapping
//==========================================================================

#pragma strings(readonly)

#include "pref.h"
#include "api.h"


//==========================================================================
// procedura dialogo opzioni titlebar hiding
// messaggi:
// WM_INITDLG
// WM_WINDOWPOSCHANGED
//==========================================================================

MRESULT EXPENTRY CoverPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         p->pd[p->idxPage].hwnd = hwnd;
         // riattiva finestra principale
         setparown(hwnd);
         // costruisce stringa versione programma
         BuildVersionString(WinWindowFromID(hwnd, TXT_VERSION));
         break;
      case WM_WINDOWPOSCHANGED: {
         if ((((PSWP)mp1)->fl & SWP_SIZE) &&       // se ridimensionamento ma
             !(((PSWP)mp1)->fl & SWP_MINIMIZE) &&  // non minimizzazione
             (((PSWP)mp1)->cy > MINCY)) {
            HWND hVer = WinWindowFromID(hwnd, TXT_VERSION);
            WinSetWindowPos(WinWindowFromID(hwnd, WSBMP_LOGO), NULLHANDLE,
                 (((PSWP)mp1)->cx - p->szlLogo.cx) / 2,
                 (((PSWP)mp1)->cy - p->cyTitle - 5 - p->szlLogo.cy) / 2 +
                 5 + p->cyTitle, 0, 0, SWP_MOVE);
            WinSetWindowPos(hVer, NULLHANDLE,
                            (((PSWP)mp1)->cx - p->szlLogo.cx) / 2,
                            5, p->szlLogo.cx, p->cyTitle,
                            SWP_MOVE | SWP_SIZE);
            return WinDefDlgProc(hwnd, msg, mp1, mp2);
         } /* endif */
      } // end case WM_WINDOWPOSCHANGED
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return (MRESULT)FALSE;
}
