//===========================================================================
// menu.c : enhanced WC_MENU procedure
//===========================================================================


#pragma strings(readonly)

// includes -----------------------------------------------------------------

#include "stlrMain.h"


// globals ------------------------------------------------------------------

extern PROCDATA pd;


// defines ------------------------------------------------------------------

//#define menuGetdata(hwnd)                                       \
//   ((PSTLRFRAME)WinQueryWindowPtr(WinQueryWindowULong((hwnd),     \
//                                              g.cd.menu.cbData),\
//                                g.cd.frame.cbData))

#define _getFrameData(_hwnd_, _p_) \
   (((_p_) = WinQueryWindowPtr(_winOwner(_hwnd_), g.cd.frame.cbData)) \
             > (PSTLRFRAME)1)

//===========================================================================
// Enhaced WC_MENU window procedure.
// Parameters --------------------------------------------------------------
// Ordinary window procedure parameters.
// Return value ------------------------------------------------------------
// MRESULT.
//===========================================================================

MRESULT EXPENTRY stlrMenuProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   PSTLRFRAME p;
   switch (msg) {

// note: interception of WM_CREATE is not useful as all menus are created
//       with the same style and the FID_MENU id.
//       The real menu id, for the frame controls, is set just before the
//       first MM_INSERTITEM message.

//case WM_CREATE:
//   /*-*/ dbgPrintf6("->menu creation : %08x "
//                    "(parent: %08x, style: %08x, id: %08x, data: %08x)\n",
//                    hwnd,
//                    ((PCREATESTRUCT)mp2)->hwndParent,
//                    ((PCREATESTRUCT)mp2)->flStyle,
//                    ((PCREATESTRUCT)mp2)->id,
//                    ((PCREATESTRUCT)mp2)->pCtlData);
//   break;

//case MM_DELETEITEM:
//   /*-*/ dbgPrintf3("MM_DELETEITEM %08x id: %04x\n", hwnd, SHORT1FROMMP(mp1));
//   break;
//===========================================================================
// menu item insertion
// if hwnd is the handle of the FID_MINMAX substitutes the bitmap handle

case MM_INSERTITEM:
//   /*-*/ dbgPrintf4("MM_INSERTITEM %08x pos %d id %04x\n",
//                    hwnd, ((PMENUITEM)mp1)->iPosition, ((PMENUITEM)mp1)->id);
   if (!o.gen.disabled
       && pd.ppib
       && !pd.we.tbar
       && o.tb.on
       && (FID_MINMAX == _WinID(hwnd))) {
      switch (((PMENUITEM)mp1)->id) {
         case SC_RESTORE:
            if (o.tb.bmp.rest)
               ((PMENUITEM)mp1)->hItem = o.tb.bmp.rest;
         #ifdef STLR_WARP3
            if (o.tb.close == 3) {
               ((PMENUITEM)mp1)->iPosition = _posFrameCtrl(hwnd, SC_CLOSE);
//               if (((PMENUITEM)mp1)->iPosition != MIT_NONE)
//                  ((PMENUITEM)mp1)->iPosition--;
            } else {
               ((PMENUITEM)mp1)->iPosition = MIT_END;
            } /* endif */
         #endif
            break;
         case SC_MINIMIZE:
            if (o.tb.bmp.min)
               ((PMENUITEM)mp1)->hItem = o.tb.bmp.min;
         #ifdef STLR_WARP3
            ((PMENUITEM)mp1)->iPosition = 0;
            if ((o.tb.close == 2) && _isFrameCtrlValid(hwnd, SC_CLOSE))
               ((PMENUITEM)mp1)->iPosition++;
            if (_isFrameCtrlValid(hwnd, SC_TOGGLEROLL))
               ((PMENUITEM)mp1)->iPosition++;
            if (_isFrameCtrlValid(hwnd, SC_TOGGLETITLEHIDE))
               ((PMENUITEM)mp1)->iPosition++;
         #endif
            break;
         case SC_MAXIMIZE:
            if (o.tb.bmp.max)
               ((PMENUITEM)mp1)->hItem = o.tb.bmp.max;
         #ifdef STLR_WARP3
            if (o.tb.close == 3) {
               ((PMENUITEM)mp1)->iPosition = _posFrameCtrl(hwnd, SC_CLOSE);
//               if (((PMENUITEM)mp1)->iPosition != MIT_NONE)
//                  ((PMENUITEM)mp1)->iPosition--;
            } else {
               ((PMENUITEM)mp1)->iPosition = MIT_END;
            } /* endif */
         #endif
            break;
         case SC_HIDE:
            if (o.tb.bmp.hide)
               ((PMENUITEM)mp1)->hItem = o.tb.bmp.hide;
         #ifdef STLR_WARP3
            ((PMENUITEM)mp1)->iPosition = 0;
            if ((o.tb.close == 2) && _isFrameCtrlValid(hwnd, SC_CLOSE))
               ((PMENUITEM)mp1)->iPosition++;
            if (_isFrameCtrlValid(hwnd, SC_TOGGLEROLL))
               ((PMENUITEM)mp1)->iPosition++;
            if (_isFrameCtrlValid(hwnd, SC_TOGGLETITLEHIDE))
               ((PMENUITEM)mp1)->iPosition++;
         #endif
            break;
         case SC_CLOSE:
            if (o.tb.bmp.close)
               ((PMENUITEM)mp1)->hItem = o.tb.bmp.close;
            // move the close button in win32 windows
            if (o.tb.close == 3)
               ((PMENUITEM)mp1)->iPosition = MIT_END;
            break;
      } /* endswitch */
   } /* endif */
   break;


//===========================================================================
// con warp 4 impedisce che il bottone di chiusura della finestra venga
// reinserito quando Š disabilitato o spostato a sinistra
#ifndef STLR_WARP3
case MM_ISITEMVALID:
   if ((SHORT1FROMMP(mp1) == SC_CLOSE)
       && !pd.we.tbar
       && !o.gen.disabled
       && o.tb.on
       && (_WinID(hwnd) == FID_MINMAX)
       && _getFrameData(hwnd, p)
       && ((o.tb.close < 2) || p->is.skipuf))
      return (MRESULT)TRUE;
   break;
#endif

//===========================================================================
// memorizza coordinata y barra menu

//case WM_ADJUSTWINDOWPOS:
//   // memorizza coordinata y FID_MENU
//   if ((((PSWP)mp1)->fl & SWP_MOVE)
//       && !pd.we.tbarhide
//       && _getFrameData(hwnd, p)
//       && (p->hMenu == hwnd)) {
//      // a trick to avoid allocating another variable on the stack
//      msg = (ULONG)g.cd.menu.pfnwp(hwnd, WM_ADJUSTWINDOWPOS, mp1, mp2);
//      p->yMenu = ((PSWP)mp1)->y;
//      return (MRESULT)msg;
//   } /* endif */
//   break;


//===========================================================================

case WM_DESTROY:
   /*-*/ dbgPrintf2("menu destruction -> %08x\n", hwnd);
   if (_getFrameData(hwnd, p)) {
      if (hwnd == p->hSMenu) {
         p->hSMenu = 0;
         p->ffl &= ~FCF_SYSMENU;
         if (p->is.minMaxAdd)
            WinDestroyWindow(p->hMinMax), p->hMinMax = 0;
      } else if (hwnd == p->hMinMax) {
         p->hMinMax = 0;
         p->ffl &= ~(FCF_MINMAX | FCF_HIDEBUTTON | FCF_CLOSEBUTTON);
         p->is.minMaxAdd = 0;
      } else if (hwnd == p->hMenu) {
         p->ffl &= ~FCF_MENU;
         p->hMenu = 0;
         p->yMenu = 0;
      } /* endif */
   } /* endif */
   break;


//===========================================================================
// setta flag mouse sopra menu, se non Š actionbar setta flag per impedire
// perdita focus all'owner del menu

case WM_MOUSEMOVE:
   if (g.is.onctrl != OCTLMENU) {
      g.is.onctrl = OCTLMENU;
      _resetHideMouTimer();
   } /* endif */
   break;

//===========================================================================
// system default WC_MENU procedure :

   } /* endswitch */
   return (MRESULT)g.cd.menu.pfnwp(hwnd, msg, mp1, mp2);
}
