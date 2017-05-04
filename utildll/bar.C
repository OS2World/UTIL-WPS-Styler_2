//========================================================================\
// bar.c                                                                  |
// FUNCTION: ------------------------------------------------------------ |
// MESSAGE:  ------------------------------------------------------------ |
// WM_CREATE                                                              |
// WM_ADJUSTWINDOWPOS                                                     |
// WM_PAINT                                                               |
// WM_DESTROY                                                             |
//========================================================================/

#include "common.h"
#include "bar.h"
#include "api.h"

// Prototypes functions

MRESULT EXPENTRY EDBarWinProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);


//==========================================================================\
// Class registration                                                       |
//==========================================================================/

BOOL APIENTRY EDBarReg(HAB hab) {
   return WinRegisterClass(hab, WC_BAR, (PFNWP)EDBarWinProc,
                           flWCstc & ~CS_PUBLIC, cbWCstc + 4);
}


//==========================================================================\
// Bar control procedure                                                    |
// per ora ignora tutti gli stili tranne raised o depressed e disegna il    |
// testo (se presente) in una cornice invertita rispetto alla cornice del   |
// gruppo                                                                   |
//==========================================================================/

MRESULT EXPENTRY EDBarWinProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      // Creation: stores text control
      case WM_CREATE: {
         PBARDATA pbd;
         // allocate structure for caching some control data
         if (!(pbd = (PBARDATA)malloc(sizeof(BARDATA)))) return NULL;
         // se presente testo lo azzera
         if (strlen(((PCREATESTRUCT)mp2)->pszText))
            *((PCREATESTRUCT)mp2)->pszText = 0;
         // copy some data
         pbd->fl = ((PCREATESTRUCT)mp2)->flStyle;
         // se bordo non specificato usa il valore 1
         if (!(pbd->fl & CCTL_BORDER7)) pbd->fl |= CCTL_BORDER1;
         if (pbd->fl & BARS_VERTICAL) {          // vertical
            pbd->szl.cx = 2 * (pbd->fl & 0x7);
            pbd->szl.cy = ((PCREATESTRUCT)mp2)->cy;
         } else {                               // horizontal
            pbd->szl.cx = ((PCREATESTRUCT)mp2)->cx;
            pbd->szl.cy = 2 * (pbd->fl & 0x7);
         } /* endif */
         WinSetWindowBits(hwnd, QWL_STYLE, SS_GROUPBOX, 0xffff);
         WinSetWindowPos(hwnd, 0, 0, 0, pbd->szl.cx, pbd->szl.cy,
                         SWP_SIZE | SWP_NOADJUST);
         WinSetWindowPtr(hwnd, cbWCstc, (PVOID)pbd);
         break;
      }
      // se il controllo Š ridimensionato memorizza nuove dimensioni
      case WM_ADJUSTWINDOWPOS:
         if (((PSWP)mp1)->fl & SWP_SIZE) {
            PBARDATA pbd = (PBARDATA)stGetData(hwnd);
            if (pbd) {
               if (pbd->fl & BARS_VERTICAL) {         // vertical
                  ((PSWP)mp1)->cx = pbd->szl.cx;
                  pbd->szl.cy = ((PSWP)mp1)->cy;
               } else {                               // horizontal
                  pbd->szl.cx = ((PSWP)mp1)->cx;
                  ((PSWP)mp1)->cy = pbd->szl.cy;
               } /* endif */
            } /* endif */
         } // end if
         return WinDefWindowProc(hwnd, msg, mp1, mp2);
      // sostituisce totalmente procedura controllo statico
      case WM_PAINT: {
         PBARDATA pbd = (PBARDATA)stGetData(hwnd);
         HPS hps;
         RECTL rcl = {0, 0, pbd->szl.cx, pbd->szl.cy};;
         if (pbd &&
             NULLHANDLE != (hps = WinBeginPaint(hwnd, NULLHANDLE, NULL))) {
            WinDrawBorder(hps, &rcl, pbd->fl & 0x7, pbd->fl & 0x7, 0, 0,
                          (pbd->fl & CCTL_RAISED? 0x400: 0x800));
            WinEndPaint(hps);
         } // end if
         return (MRESULT)FALSE;
      } // end case WM_PAINT
      case WM_DESTROY: {
         PBARDATA pbd = (PBARDATA)stGetData(hwnd);
         if (pbd) free(pbd);
      }  break;
   } /* endswitch */
   return pfnwpWCstc(hwnd, msg, mp1, mp2); // default static controls proc
}  
