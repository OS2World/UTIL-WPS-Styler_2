//===========================================================================
// stlrMacros.h : macros used by Styler/2
//
// --2003  - Alessandro Felice Cantatore
//===========================================================================


#ifndef _STLR_MACROS_H_
   #define _STLR_MACROS_H_

// forever loop -------------------------------------------------------------

#define forever for(;;)


// version number initialization --------------------------------------------

#define _stlrSetCurrentVersion(p) \
  (p)->Mver = CURMAJVER, \
  (p)->mver = CURMINVER, \
  (p)->rver = CURRELVER, \
  (p)->bver = CURBLDVER


// WinQuerySysValue() wrapper -----------------------------------------------

#define _sysValue(fl)         (WinQuerySysValue(HWND_DESKTOP, fl))


// handle errors on DLL load ------------------------------------------------

#define _stlrLoadError(errId, rc)          (stlrlog(errId), (rc))


// check if a window is of class WC_FRAME or inherit from WC_FRAME
#define _isFrameWindow(_hwnd_) \
   ((BOOL)WinSendMsg(_hwnd_, WM_QUERYFRAMEINFO, MPVOID, MPVOID))

// macro to post the special Styler/2 PM message ----------------------------

#define _stlrMsgPost(_hwnd_, _mp1_, _mp2_) \
   (WinPostMsg((_hwnd_), WM_STYLER2, (MPARAM)(_mp1_), (MPARAM)(_mp2_)))


// macro to send the special Styler/2 PM message ----------------------------

#define _stlrMsgSend(_hwnd_, _mp1_, _mp2_) \
   (WinSendMsg((_hwnd_), WM_STYLER2, (MPARAM)(_mp1_), (MPARAM)(_mp2_)))


// macro to directly execute the Styler/2 frame procedure -------------------

#define _stlrRunFrameProc(_hwnd_, _mp1_, _mp2_) \
   (stlrFrameProc((_hwnd_), WM_STYLER2, (MPARAM)(_mp1_), (MPARAM)(_mp2_)))


// macro to broadcast (post) the special Styler/2 PM message ----------------

#define _stlrMsgPostAll(_mp1_, _mp2_) \
   (WinBroadcastMsg(g.hwnd.desktop, WM_STYLER2, \
                    (MPARAM)(_mp1_), (MPARAM)(_mp2_),\
                     BMSG_POST | BMSG_FRAMEONLY | BMSG_DESCENDANTS))



// macro to broadcast (send) the special Styler/2 PM message ----------------

#define _stlrMsgSendAll(_mp1_, _mp2_) \
   (WinBroadcastMsg(g.hwnd.desktop, WM_STYLER2, \
                    (MPARAM)(_mp1_), (MPARAM)(_mp2_),\
                    BMSG_SEND | BMSG_FRAMEONLY | BMSG_DESCENDANTS))


// macro to get a color presparm --------------------------------------------

#define _getPPColor(_hwnd_, _pp_, _pclr_) \
   (WinQueryPresParam((_hwnd_), (_pp_), 0, NULL, \
                      sizeof(ULONG), (_pclr_), QPF_NOINHERIT))


// macro to get the presparm PP_FONTNAMESIZE --------------------------------

#define _getPPFont(_hwnd_, _buf_) \
   (WinQueryPresParam((_hwnd_), PP_FONTNAMESIZE, 0, NULL, \
                      CB_FONTNAME, (_buf_), QPF_NOINHERIT))


// set the presparm PP_FONTNAMESIZE -----------------------------------------

#define _setPPFont(_hwnd_, _font_) \
   (WinSetPresParam((_hwnd_), PP_FONTNAMESIZE, strlen(_font_) + 1, (_font_)))

// load a string from the resourced DLL -------------------------------------

#define _stlrLoadStr(_hmod_, _id_, _buf_) \
   (WinLoadString(0, (_hmod_), (_id_), sizeof(_buf_), (_buf_)))


// load the system pointer 'id' ---------------------------------------------

#define _setSysMouPtr(_id_) \
   (WinSetPointer(g.hwnd.desktop, WinQuerySysPointer(g.hwnd.desktop, (_id_), 0)))

// start the auto-focus timer -----------------------------------------------

#define _startAutoFocusTimer(_hwnd_)                         \
   g.hwnd.autoFocus = (_hwnd_),                              \
   WinStartTimer(g.timerThrd.hab, g.timerThrd.hwnd,          \
                 TID_AUTOFOCUS, o.mou.focusDelay)


// start the hide-titlebar timer --------------------------------------------

#define _startHideTitlebarTimer(_hwnd_)                      \
   g.hwnd.hideTitle = (_hwnd_),                              \
   WinStartTimer(g.timerThrd.hab, g.timerThrd.hwnd,          \
                 TID_HIDETITLE, o.tbh.hideDelay)


// start the unhide-titlebar timer ------------------------------------------

#define _startUnhideTitlebarTimer(_hwnd_)                    \
   g.hwnd.unhideTitle = (_hwnd_),                            \
   WinStartTimer(g.timerThrd.hab, g.timerThrd.hwnd,          \
                 TID_UNHIDETITLE, o.tbh.unhideDelay)


// reset the auto-focus timer -----------------------------------------------

#define _resetAutoFocusTimer()                               \
   g.hwnd.autoFocus = NULLHANDLE,                            \
   WinStopTimer(g.timerThrd.hab, g.timerThrd.hwnd, TID_UNHIDETITLE)


// reset the mouse-pointer-hiding timer -------------------------------------

#define _resetHideMouTimer()                                 \
      if (g.is.hidePtrTmr)                                   \
         g.is.hidePtrTmr = 0,                                \
         WinStopTimer(g.timerThrd.hab, g.timerThrd.hwnd, TID_MOUSEPTR)


// get the position of a frame control --------------------------------------

#define _posFrameCtrl(_hwnd_, _id_) \
   ((SHORT)g.cd.menu.pfnwp((_hwnd_), MM_ITEMPOSITIONFROMID, \
                          MPFROM2SHORT((_id_), FALSE), MPVOID))


// check if a frame control is present --------------------------------------

#define _isFrameCtrlValid(_hwnd_, _id_) \
   ((BOOL)g.cd.menu.pfnwp((_hwnd_), MM_ISITEMVALID, \
                          MPFROM2SHORT((_id_), FALSE), MPVOID))


// change the bitmap used to paint a frame window control -------------------

#define _setFrameCtrlBmp(_hmenu_, _id_, _hbmp_) \
(g.cd.menu.pfnwp((_hmenu_), MM_SETITEMHANDLE, (MPARAM)(_id_), (MPARAM)(_hbmp_)))


// remove a frame window control --------------------------------------------

#define _removeFrameCtrl(_hmenu_, _id_) \
(g.cd.menu.pfnwp((_hmenu_), MM_DELETEITEM, MPFROM2SHORT((_id_), FALSE), MPVOID))


// remove a system menu item ------------------------------------------------

#define _removeSysMenuItem(_hmenu_, _id_) \
(g.cd.menu.pfnwp((_hmenu_), MM_DELETEITEM, MPFROM2SHORT((_id_), TRUE), MPVOID))


// insert a frame window control --------------------------------------------
// menu handle, PMENUITEM, position, id, handle bitmap

#define _insertFrameCtrl(_hmenu_, _pmi_, _pos_, _id_, _hbmp_) \
   (_pmi_)->iPosition = (_pos_),                              \
   (_pmi_)->afStyle = MIS_SYSCOMMAND | MIS_BITMAP,            \
   (_pmi_)->afAttribute = 0,                                  \
   (_pmi_)->id = (_id_),                                      \
   (_pmi_)->hwndSubMenu = 0,                                  \
   (_pmi_)->hItem = (_hbmp_),                                 \
   g.cd.menu.pfnwp((_hmenu_), MM_INSERTITEM, (MPARAM)(_pmi_), MPVOID)


// insert an item in the system menu ----------------------------------------

#define _insertSysMenuItem(_hmenu_, _pmi_, _pos_, _id_, _ptext_) \
   (_pmi_)->iPosition = (_pos_),                                \
   (_pmi_)->afStyle = MIS_SYSCOMMAND | MIS_TEXT,                \
   (_pmi_)->afAttribute = 0,                                    \
   (_pmi_)->id = (_id_),                                        \
   (_pmi_)->hwndSubMenu = 0,                                    \
   (_pmi_)->hItem = 0,                                          \
   g.cd.menu.pfnwp((_hmenu_), MM_INSERTITEM, (MPARAM)(_pmi_), (MPARAM)(_ptext_))

// get menu item data -------------------------------------------------------

#define _getMenuItem(_hmenu_, _id_, _pmi_) \
   g.cd.menu.pfnwp((_hmenu_), MM_QUERYITEM, \
                   MPFROM2SHORT(SC_SYSMENU, TRUE), (MPARAM)(_pmi_))


// DosQuerySysInfo wrapper --------------------------------------------------

_Inline ULONG sysInfo(ULONG fl) {
   DosQuerySysInfo(fl, fl, &fl, sizeof(ULONG));
   return fl;
}


// capture the mouse pointer ------------------------------------------------
#define _mouCapture(_hwnd_) \
   WinSetCapture(g.hwnd.desktop, _hwnd_), g.is.captured = 1

#define _mouRelease() \
   WinSetCapture(g.hwnd.desktop, NULLHANDLE), g.is.captured = 0

// return TRUE if the mouse is in the rectangle relative to hwnd ------------
_Inline BOOL isMouseInRect(HWND hwnd, INT x0, INT y0, INT x1, INT y1) {
   POINTL p;
   WinQueryPointerPos(HWND_DESKTOP, &p);
   WinMapWindowPoints(HWND_DESKTOP, hwnd, &p, 1);
   return ((p.x >= x0) && (p.x < x1) && (p.y >= y0) && (p.y < y1));
}


// get the type of standard PM controls -------------------------------------

#define _getCtrlType(_hwnd_) \
   ((ULONG)(WinSendMsg((_hwnd_), WM_QUERYDLGCODE, MPVOID, MPVOID)))

// WinQueryWindow wrappers --------------------------------------------------

#define _winParent(_hwnd_)             (WinQueryWindow(_hwnd_, QW_PARENT))
#define _winOwner(_hwnd_)              (WinQueryWindow(_hwnd_, QW_OWNER))
#define _winStyle(_hwnd_)              (WinQueryWindowULong(_hwnd_, QWL_STYLE))
#define _WinTopWin(_hwnd_)             (WinQueryWindow(_hwnd_, QW_TOP))
#define _WinID(_hwnd_)                 (WinQueryWindowUShort(_hwnd_, QWS_ID))
#define _WinHWND(_hwnd_, _id_)         (WinWindowFromID(_hwnd_, _id_))

// repaint the window -------------------------------------------------------

#define repaintWindow(_hwnd_) \
   WinInvalidateRect((_hwnd_), NULL, FALSE)

#define repaintWindowR(_hwnd_) \
   WinInvalidateRect((_hwnd_), NULL, TRUE)

// repaint all the screen ---------------------------------------------------

#define repaintAll() \
   WinInvalidateRect(g.hwnd.desktop, NULL, TRUE)

// activate a frame window and make it the top window
#define _frameActivate(_hwnd_) \
   (WinSetWindowPos((_hwnd_), HWND_TOP, 0, 0, 0, 0, SWP_ZORDER | SWP_ACTIVATE))

// obsolete (to be replaced) ------------------------------------------------
//
//#define ABORTLOAD(x)   {stlrlog(x); return 0L;}
//
//// terminazione caricamento DLL
//#ifdef DEBUGGING
//   #define DPRINT(func, err, ret) {sprintf(buf, "%s#%d\n", (func), (err));\
//                                   logwrite(hf, buf);\
//                                   return (ret);}
//#endif
// restituisce un handle da un puntatore ai dati di una bitmap
//
//#define GetBmpHandle(hps, pbmp) \
// (HBITMAP)(GpiCreateBitmap((hps),\
//           (PBITMAPINFOHEADER2)&((PBITMAPFILEHEADER2)(pbmp))->bmp2,\
//           CBM_INIT, ((PBYTE)(pbmp)) + ((PBITMAPFILEHEADER2)(pbmp))->offBits,\
//           (PBITMAPINFO2)&((PBITMAPFILEHEADER2)(pbmp))->bmp2))
//
//#define MOUSEX(mp)          ((SHORT)(ULONG)(mp))
//#define MOUSEY(mp)          ((SHORT)((ULONG)mp >> 16))
//

#endif // #ifndef _STLR_MACROS_H_
