//===========================================================================
// Enhanced WC_TITLEBAR procedure.
//===========================================================================


#pragma strings(readonly)


// includes -----------------------------------------------------------------

#include "stlrMain.h"


// global variables ---------------------------------------------------------

extern PROCDATA pd;


// prototypes ---------------------------------------------------------------

VOID DrawTitlebar(PSTLRFRAME p);


// defines ------------------------------------------------------------------

//#define tbarGetdata(hwnd)                                        \
//   ((PSTLRFRAME)WinQueryWindowPtr(WinQueryWindowULong((hwnd),    \
//                                               g.cd.tbar.cbData),\
//                                g.cd.frame.cbData))

// get the data of the frame window owning the titlebar
#define _getFrameData(_hwnd_, _p_) \
   (NULL != ((_p_) = WinQueryWindowPtr(_hwnd_, g.cd.tbar.cbData)))

// allocate and initialize the frame data of win32 frame windows
#define _initWin32Frame(_hwnd_) \
   ((PSTLRFRAME)_stlrMsgSend(_hwnd_, STLR_INITWIN32, TRUE))

// reset the WC_TITLEBAR system default procedure
#define _resetDefaultProc(_hwnd_) \
   if (WinQueryWindowPtr(_hwnd_, QWP_PFNWP) == (PVOID)stlrTitleBarProc) \
      WinSetWindowPtr(_hwnd_, QWP_PFNWP, (PVOID)g.cd.tbar.pfnwp)


//===========================================================================
// Enhanced titlebar procedure.
// Parameters --------------------------------------------------------------
// Ordinary window procedure parameters.
// Return value ------------------------------------------------------------
// MRESULT.
//===========================================================================

MRESULT EXPENTRY stlrTitleBarProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   PSTLRFRAME p;
   /* -------------------------------------------------------------------
    window creation :
    store the handle of the owner window, the initial titlebar size,
    convert the title substituting \r\n with 1 space, update the titlebar
    font
   ------------------------------------------------------------------- */
   if (msg == WM_CREATE) {
      // if this is a global exception window, if the window ID is not
      // FID_TITLEBAR or if the owner window is not a frame, go to the
      // default system titlebar procedure
      if (!pd.ppib
          || (((PCREATESTRUCT)mp2)->id != FID_TITLEBAR)
          || !_isFrameWindow(((PCREATESTRUCT)mp2)->hwndOwner))
         goto defaultTitlebarProcedure;
      // get the frame data
      p = (PSTLRFRAME)WinQueryWindowPtr(((PCREATESTRUCT)mp2)->hwndOwner,
                                        g.cd.frame.cbData);
//      /*-*/ dbgPrintf5("*** pd.ppib:%08x, id: %04x, isOwnerFrame: %d, p: %08x\n",
//                       pd.ppib, ((PCREATESTRUCT)mp2)->id,
//                       _isFrameWindow(((PCREATESTRUCT)mp2)->hwndOwner), p);
      // if the frame window is a win32frame initialize the frame data now
      if (!p
          || ((p == FL_WIN32FRAME)
              && (NULL
                  == (p = _initWin32Frame(((PCREATESTRUCT)mp2)->hwndOwner)))))
         goto defaultTitlebarProcedure;
      // memorizza handle frame window
      WinSetWindowPtr(hwnd, g.cd.tbar.cbData, p);
      p->hTBar = hwnd;
      if (((PCREATESTRUCT)mp2)->pszText)
         p->cchTxt = psznconvcpy(p->achTxt, ((PCREATESTRUCT)mp2)->pszText, 260);
      p->wrclTitle.x = ((PCREATESTRUCT)mp2)->x;
      p->wrclTitle.y = ((PCREATESTRUCT)mp2)->y;
      p->wrclTitle.cx = ((PCREATESTRUCT)mp2)->cx;
      p->wrclTitle.cy = ((PCREATESTRUCT)mp2)->cy;
      // cambia font secondo preferenze
      if (o.tb.on && !(o.gen.disabled || pd.we.tbar))
         _setPPFont(hwnd, o.tb.achFont);

   /* -------------------------------------------------------------------
    hiliting/unhiliting of the titlebar :
    store the handle of the owner window, the initial titlebar size,
    convert the title substituting \r\n with 1 space, update the titlebar
    font
   ------------------------------------------------------------------- */
   } else if (msg == TBM_SETHILITE) {
      if (_getFrameData(hwnd, p)) {
         p->is.tbhilited = (BOOL)mp1;
         if (!o.gen.disabled && o.tb.on && !pd.we.tbar) {
            if (o.tb.ovrPP
                || (p->is.tbhilited && !p->is.tbactpp)
                || !(p->is.tbhilited || p->is.tbictpp)) {
               if (WinIsWindowShowing(hwnd)) {
                  p->hps = WinGetPS(hwnd);
                  DrawTitlebar(p);
                  WinReleasePS(p->hps);
               } /* endif */
               return (MRESULT)TRUE;
            // Š solo apparentemente ridondante, in realt… Š necessario quando
            // non Š ovrPP ed Š presente solo PP att. o inatt.
            } else if (!(p->is.tbactpp && p->is.tbictpp)) {
               repaintWindow(hwnd);
            } /* endif */
         } /* endif */
      } /* endif */

   /* -------------------------------------------------------------------
    titlebar font/color changed
    - if the font changed repaint the window
    - if any color changed reset the presparm presence flag
   ------------------------------------------------------------------- */
   } else if (msg == WM_PRESPARAMCHANGED) {
      if (!pd.we.tbar && _getFrameData(hwnd, p)) {
         ULONG ul;
         switch ((LONG)mp1) {
            case PP_FONTNAMESIZE:
               p->cyfont = 0;
               if (o.gen.disabled) break;
               if (WinIsWindowShowing(hwnd)) repaintWindow(hwnd);
               return (MRESULT)FALSE;
            case PP_ACTIVETEXTBGNDCOLOR:
            case PP_ACTIVETEXTBGNDCOLORINDEX:
            case PP_ACTIVECOLOR:
            case PP_ACTIVECOLORINDEX:
               p->is.tbactpp = _getPPColor(hwnd, (ULONG)mp1, &ul) != 0;
               break;
            case PP_INACTIVETEXTBGNDCOLOR:
            case PP_INACTIVETEXTBGNDCOLORINDEX:
            case PP_INACTIVECOLOR:
            case PP_INACTIVECOLORINDEX:
               p->is.tbictpp = _getPPColor(hwnd, (ULONG)mp1, &ul) != 0;
               break;
         } /* endswitch */
      } /* endif */

   /* -------------------------------------------------------------------
    the titlebar text changed. If this is a Win-OS/2 window set the
    window words to 0 so that the enhancements are skipped, otherwise
    store the new text.
   ------------------------------------------------------------------- */
   } else if (msg == WM_SETWINDOWPARAMS) {
      if (!pd.we.tbar && _getFrameData(hwnd, p)) {
         // unsubclass winOS2 windows titlebars
         if (p->is.winos2) {
            _resetDefaultProc(hwnd);
//            WinSetWindowULong(hwnd, g.cd.tbar.cbData, 0);
         } else {
            if ((PWNDPARAMS)mp1 &&
                (((PWNDPARAMS)mp1)->fsStatus & WPM_TEXT) &&
                ((PWNDPARAMS)mp1)->pszText) {
               p->cchTxt = psznconvcpy(p->achTxt, ((PWNDPARAMS)mp1)->pszText, 260);
               p->cyfont = 0;
            } /* endif */
         } /* endif */
      } /* endif */

   /* -------------------------------------------------------------------
    titlebar size/position changed : store new size and position
   ------------------------------------------------------------------- */
   } else if (msg == WM_WINDOWPOSCHANGED) {
      if ((((PSWP)mp1)->fl & (SWP_SIZE | SWP_MOVE))
          && _getFrameData(hwnd, p)) {
         p->wrclTitle.x = ((PSWP)mp1)->x;
         p->wrclTitle.y = ((PSWP)mp1)->y;
         p->wrclTitle.cx = ((PSWP)mp1)->cx;
         p->wrclTitle.cy = ((PSWP)mp1)->cy;
      } /* endif */

// **************************************************************************
// these messages are processed only if Styler/2 is not disabled:

   } else if (!o.gen.disabled) {

   /* -------------------------------------------------------------------
    check if this is the roll-the-window mouse event :
   ------------------------------------------------------------------- */
      if ((msg == o.roll.moumsg) && (SHORT2FROMMP(mp2) == o.roll.moukbd)) {
         if (!pd.we.wroll
             && o.roll.mou
             && _getFrameData(hwnd, p)
             && !p->is.win32
             && (o.roll.on || p->is.RLon)) {
            toggleRollCmd(p->hwnd, p);
            return (MRESULT)TRUE;
         } /* endif */

   /* -------------------------------------------------------------------
    check if this is the hide-titlebar mouse event :
   ------------------------------------------------------------------- */
      } else if ((msg == o.tbh.moumsg) && (SHORT2FROMMP(mp2) == o.tbh.moukbd)) {
         if (!pd.we.tbarhide
             && o.tbh.mou
             && _getFrameData(hwnd, p)
             && !p->is.win32
      // opzione titlehide attiva e non dialogo o disattivazione titlehide
             && ((o.tbh.on && !(p->is.dialog && o.tbh.noDlg)) || p->is.titleHide)) {
            toggleTitleBarHidingCmd(p->hwnd, p);
            return (MRESULT)TRUE;
         } /* endif */

   /* -------------------------------------------------------------------
    check if this is the change-Zorder mouse event :
   ------------------------------------------------------------------- */
      } else if ((msg == o.sizpos.ZordMoumsg)
                 && (SHORT2FROMMP(mp2) == o.sizpos.ZordMoukbd)) {
         if (!pd.we.sizemove
             && o.sizpos.Zmove
             && _getFrameData(hwnd, p)) {
            _stlrMsgPost(p->hwnd, STLR_ZORDACTIV, SW2_SFMOVETOBOTTOM);
            return (MRESULT)TRUE;
         } /* endif */

   /* -------------------------------------------------------------------
    check if this is the snap-to-screen-edge mouse event :
   ------------------------------------------------------------------- */
      } else if ((msg == o.sizpos.snapMoumsg)
                 && (SHORT2FROMMP(mp2) == o.sizpos.snapMoukbd)) {
         if (!pd.we.sizemove
             && o.sizpos.snap
             && _getFrameData(hwnd, p)) {
            // get the rectangle of the window and of its parent
            RECTL r, rParent;
            WinQueryWindowRect(p->hwnd, &r);
            WinQueryWindowRect(_winParent(p->hwnd), &rParent);
            // click on the left side of the titlebar
            if (MOUSEX(mp1) < (p->wrclTitle.cx / 3)) {
               r.xLeft = -p->ptBrd.x;
            // click on the right side of the titlebar
            } else if (MOUSEX(mp1) > (p->wrclTitle.cx * 2 / 3)) {
               r.xLeft = rParent.xRight - r.xRight + p->ptBrd.x;
            // click on the (horizontal) middle of the titlebar
            } else {
               r.xLeft = (rParent.xRight - r.xRight) / 2;
            } /* endif */
            // click on the bottom of the titlebar
            if (MOUSEY(mp1) < (g.sys.cyTbar / 3)) {
               r.yBottom = -p->ptBrd.y;
            // click on the top of the titlebar
            } else if (MOUSEY(mp1) > (g.sys.cyTbar * 2 / 3)) {
               r.yBottom = rParent.yTop - r.yTop + p->ptBrd.y;
            // click on the (vertical) middle of the titlebar
            } else {
               r.yBottom = (rParent.yTop - r.yTop) / 2;
            } /* endif */
            WinSetWindowPos(p->hwnd, 0, r.xLeft, r.yBottom, 0, 0,
                            SWP_MOVE | SWP_NOADJUST);
         } /* endif */

   /* -------------------------------------------------------------------
    enhanced window painting
   ------------------------------------------------------------------- */
      } else if (msg == WM_PAINT) {
         if (!pd.we.tbar
             && o.tb.on
             && _getFrameData(hwnd, p)
             && (o.tb.ovrPP || (p->is.tbhilited && !p->is.tbactpp)
                 || !(p->is.tbhilited || p->is.tbictpp))) {
            p->hps = WinBeginPaint(hwnd, NULLHANDLE, NULL);
            DrawTitlebar(p);
            WinEndPaint(p->hps);
            return (MRESULT)FALSE;
         } /* endif */

   /* -------------------------------------------------------------------
    override the default window tracking :
   ------------------------------------------------------------------- */
      } else if ((msg == WM_BUTTON1DOWN) || (msg == WM_BUTTON2DOWN)) {
         if (_getFrameData(hwnd, p)) {
            // override window motion by titlebar drag :
            // store the current mouse position (mp1)
            // the window motion will start only when the mouse will move
            g.tbdrag.mpPos = mp1;
            // if the window is not already moving store which mouse button
            // has to be released to stop the titlebar drag
            if (!g.tbdrag.hwnd) g.tbdrag.msg = ++msg;
            // store the handle of the titlebar (mouse motion events are not
            // processed when this is NULLHANDLE)
            g.tbdrag.hwnd = hwnd;
            // cattura i msg successivi del mouse
            WinSetCapture(HWND_DESKTOP, hwnd);
            return (MRESULT)TRUE;
         } /* endif */

   /* -------------------------------------------------------------------
    terminate window motion through titlebar drag events :
   ------------------------------------------------------------------- */
      } else if ((msg == WM_BUTTON1UP) || (msg == WM_BUTTON2UP)) {
         if ((g.tbdrag.hwnd == hwnd)
              && (g.tbdrag.msg == msg)
              && _getFrameData(hwnd, p)) {
            HWND hwndParent;
            WinSetCapture(HWND_DESKTOP, NULLHANDLE);
            g.tbdrag.msg = 0;
            g.tbdrag.hwnd = NULLHANDLE;
            // if the Ctrl key is not pressed and the window is not yet active
            // end the window is a size-move feature exception or the event
            // is not related to the Z-order motion feature or the snap-to-
            // parent-edge feature activate the window bringing it to the top
            if (!(SHORT2FROMMP(mp2) & KC_CTRL)
                && (g.hwnd.active != p->hwnd)
                && (pd.we.sizemove
                    || (!(o.sizpos.Zmove
                          && (msg == (o.sizpos.ZordMoumsg - 0x3a1))
                          && (SHORT2FROMMP(mp2) == o.sizpos.ZordMoukbd))
                        && (!(o.sizpos.snap
                          && (msg == (o.sizpos.snapMoumsg - 0x3a1))
                          && (SHORT2FROMMP(mp2) == o.sizpos.snapMoukbd))))))
               WinSetWindowPos(p->hwnd, HWND_TOP, 0, 0, 0, 0,
                               SWP_ACTIVATE | SWP_ZORDER);
         } /* endif */

   /* -------------------------------------------------------------------
    open the system menu on the titlebar :
   ------------------------------------------------------------------- */
      } else if (msg == WM_CONTEXTMENU) {
         if (!pd.we.tbar
             && o.tb.menu
             && _getFrameData(hwnd, p)) {
            // if this is the titlebar of a WPS folder simulate a context
            // menu event on the client window
            if (p->is.folder) {
               p->hClient = WinWindowFromID(p->hwnd, FID_CLIENT);
               WinPostMsg(p->hClient, WM_CONTEXTMENU,
                          (MPARAM)0x7fff7fff, MPVOID);
            // otherwise get the handle of the system menu and bring it to
            // the top via WinPopupMenu()
            } else if (p->hSMenu) {
               POINTL ptl;
               WinQueryPointerPos(g.hwnd.desktop, &ptl);
               g.cd.menu.pfnwp(p->hSMenu, MM_QUERYITEM,
                               (MPARAM)SC_SYSMENU, MPFROMP(&p->mi));
               g.hwnd.sysMenu = p->mi.hwndSubMenu;
               WinPopupMenu(g.hwnd.desktop, p->hSMenu, g.hwnd.sysMenu,
                            ptl.x, ptl.y, 0, PU_HCONSTRAIN | PU_VCONSTRAIN |
                            PU_NONE | PU_KEYBOARD | PU_MOUSEBUTTON1);
            } /* endif */
         } /* endif */
         return (MRESULT)TRUE;

   /* -------------------------------------------------------------------
    trap WM_MOUSEMOVE to implement various features :
    - do not move maximized windows
   ------------------------------------------------------------------- */
      } else if (msg == WM_MOUSEMOVE) {
         // if mouse button 1 or 2 were pressed and then the mouse position
         // changed
         if ((g.tbdrag.hwnd == hwnd)
             && (g.tbdrag.mpPos != mp1)
             && _getFrameData(hwnd, p)) {
            WinSetCapture(HWND_DESKTOP, NULLHANDLE);
            g.tbdrag.hwnd = NULLHANDLE;
            g.tbdrag.msg = 0;
            // if this is a maximized window and the don't-move-max-window
            // feature is set ignore the event
            if (o.maxwin.nomove
                && p->is.max
                && !(pd.we.maxwin || p->is.shield || p->is.ovrdnomovemax))
               return (MRESULT)TRUE;
            WinSendMsg(p->hwnd, WM_TRACKFRAME, (MPARAM)TF_MOVE, g.tbdrag.mpPos);
            // ???????????????????????????????
            // this is needed because the next WM_BUTTONxUP is captured by
            // the WM_TRACKFRAME processing
//            if (!(SHORT2FROMMP(mp2) & KC_CTRL))
//               WinSetWindowPos(p->hwnd, HWND_TOP, 0, 0, 0, 0, SWP_ACTIVATE | SWP_ZORDER);
            return (MRESULT)TRUE;
         } else if (g.is.onctrl != OCTLTBAR) {
            g.is.onctrl = OCTLTBAR;
            // ??????????????? this should be redundant
//            _resetHideMouTimer();
         } /* endif */
      } /* endif */
   // -------------------------------------------------------------------
   } /* endif */

defaultTitlebarProcedure:
   return g.cd.tbar.pfnwp(hwnd, msg, mp1, mp2);
}


//===========================================================================
// Draw the titlebar.
// Parameters --------------------------------------------------------------
// PSTLRFRAME p : windows data.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID DrawTitlebar(PSTLRFRAME p) {
   PTBARAIOPT ptbo = p->is.tbhilited? &o.tb.a : &o.tb.i;
   POINTL apt[3];
   INT i;
   GpiCreateLogColorTable(p->hps, 0, LCOLF_RGB, 0, 0, NULL);
   // background ----------------------------------------------------------
   switch (ptbo->bkgnd) {
      // shade --------------------------------------------------------
      case TBARBKGNDSHADE:
         if (ptbo->shade) {         // sfumatura orizzontale
            WsetRect(apt, 0, 0, p->wrclTitle.cx, p->wrclTitle.cy);
            WinDrawBitmap(p->hps, ptbo->hbmp, NULL, apt, 0, 0, DBM_STRETCH);
         } else {                        // sfumatura verticale
            WsetRect(apt, 0, 0, p->wrclTitle.cx, SHADEH_CY);
            for (i = 0; i < p->wrclTitle.cy; i += 2) {
               WinDrawBitmap(p->hps, ptbo->hbmp, NULL, apt, 0, 0, DBM_STRETCH);
               ((PRECTL)apt)->yBottom += SHADEH_CY;
               ((PRECTL)apt)->yTop += SHADEH_CY;
            } /* endfor */
         } /* endif */
         break;
      // bitmap -------------------------------------------------------
      case TBARBKGNDBMP:
         WsetRect(&apt[1], 0, 0, p->wrclTitle.cx, p->wrclTitle.cy);
         if (ptbo->strch) {
            WinDrawBitmap(p->hps, ptbo->hbmp, NULL, &apt[1], 0, 0, DBM_STRETCH);
         } else {
            apt[0].x = apt[0].y = 0;
            while (apt[0].x <= p->wrclTitle.cx) {
               WinDrawBitmap(p->hps, ptbo->hbmp, (PRECTL)&apt[1], apt, 0, 0, 0);
               apt[0].x += ptbo->size.cx;
               apt[2].x = p->wrclTitle.cx - apt[0].x;
            } /* endwhile */
         } /* endif */
         break;
      // solid color --------------------------------------------------
      default:
         apt[0].x = p->wrclTitle.cx - 1;
         apt[0].y = p->wrclTitle.cy - 1;
         GpiSetColor(p->hps, ptbo->clrLeftTop.l);
         GpiBox(p->hps, DRO_OUTLINEFILL, apt, 0L, 0L);
         break;
   } /* endswitch */
   // text ----------------------------------------------------------------
   if (!p->cyfont) {
      GpiQueryTextBox(p->hps, p->cchTxt, p->achTxt, 3, apt);
      p->yfont = - apt[1].y;
      p->cyfont = apt[0].y - apt[1].y;
      p->cxfont = apt[2].x - apt[0].x;
   } /* endif */
   // se il testo Š pi— largo della finestra viene comunque allineato a sin.
   i = (p->cxfont < p->wrclTitle.cx) ? 8: 0;
   apt[0].x = (ptbo->center && i)? (p->wrclTitle.cx - p->cxfont) / 2 + 1: i + 3;
   apt[0].y = (p->wrclTitle.cy - p->cyfont) / 2 + p->yfont;
   apt[1].x = 0;
   apt[1].y = 0;
   apt[2].x = p->wrclTitle.cx - 1;
   apt[2].y = p->wrclTitle.cy - 1;
   if (ptbo->_3DTxt) {                         // disegna ombra testo
      GpiSetColor(p->hps, ptbo->clrBgTxt.l);
      GpiCharStringPosAt(p->hps, apt, (PRECTL)(apt + 1), CHS_CLIP,
                         p->cchTxt, p->achTxt, NULL);
   } /* endif */
   apt[0].x--;
   apt[0].y++;
   GpiSetColor(p->hps, ptbo->clrFgTxt.l);
   GpiCharStringPosAt(p->hps, apt, (PRECTL)(apt + 1), CHS_CLIP,
                      p->cchTxt, p->achTxt, NULL);
   // border --------------------------------------------------------------
   if (ptbo->border) {
      GpiMove(p->hps, &apt[1]);
      apt[1].y = apt[2].y;
      GpiSetColor(p->hps, ptbo->clrHiBkgnd.l);
      GpiPolyLine(p->hps, 2, &apt[1]);
      apt[1].x = apt[2].x;
      apt[1].y = 0;
      apt[2].x = 1;
      apt[2].y = 0;
      GpiSetColor(p->hps, ptbo->clrShBkgnd.l);
      GpiPolyLine(p->hps, 2, &apt[1]);
   } /* endif */
}
