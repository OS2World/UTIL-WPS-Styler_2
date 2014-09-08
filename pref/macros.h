//===========================================================================
// stlrMacros.h : macros used by Styler/2
//
// --2003  - Alessandro Felice Cantatore
//===========================================================================


#ifndef _STLR_PREFMACROS_H_
   #define _STLR_PREFMACROS_H_

// forever loop -------------------------------------------------------------

#define forever for(;;)

#ifndef DEMOVERSION
// macro to post the special Styler/2 PM message ----------------------------

   #define _stlrMsgPost(_hwnd_, _mp1_, _mp2_) \
   (WinPostMsg((_hwnd_), WM_STYLER2, (MPARAM)(_mp1_), (MPARAM)(_mp2_)))


// macro to send the special Styler/2 PM message ----------------------------

   #define _stlrMsgSend(_hwnd_, _mp1_, _mp2_) \
   (WinSendMsg((_hwnd_), WM_STYLER2, (MPARAM)(_mp1_), (MPARAM)(_mp2_)))


// macro to broadcast (post) the special Styler/2 PM message ----------------

   #define _stlrMsgPostAll(_mp1_, _mp2_) \
   (WinBroadcastMsg(HWND_DESKTOP, WM_STYLER2, \
                    (MPARAM)(_mp1_), (MPARAM)(_mp2_),\
                     BMSG_POST | BMSG_FRAMEONLY | BMSG_DESCENDANTS))



// macro to broadcast (send) the special Styler/2 PM message ----------------

   #define _stlrMsgPSendAll(_mp1_, _mp2_) \
   (WinBroadcastMsg(HWND_DESKTOP, WM_STYLER2, \
                    (MPARAM)(_mp1_), (MPARAM)(_mp2_),\
                    BMSG_SEND | BMSG_FRAMEONLY | BMSG_DESCENDANTS))

#else
   #define _stlrMsgPost(_hwnd_, _mp1_, _mp2_)         ((void)0)
   #define _stlrMsgSend(_hwnd_, _mp1_, _mp2_)         ((void)0)
   #define _stlrMsgPostAll(_mp1_, _mp2)               ((void)0)
   #define _stlrMsgPSendAll(_mp1_, _mp2_)             ((void)0)

#endif


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


// get the type of standard PM controls -------------------------------------

#define _getCtrlType(_hwnd_) \
   ((ULONG)(WinSendMsg((_hwnd_), WM_QUERYDLGCODE, MPVOID, MPVOID)))

// WinQueryWindow wrappers --------------------------------------------------

#define _winParent(_hwnd_)             (WinQueryWindow(_hwnd_, QW_PARENT))
#define _winStyle(_hwnd_)              (WinQueryWindowULong(_hwnd_, QWL_STYLE))

// repaint the window -------------------------------------------------------

#define repaintWindow(_hwnd_) \
   WinInvalidateRect((_hwnd_), NULL, FALSE)

#define repaintWindowR(_hwnd_) \
   WinInvalidateRect((_hwnd_), NULL, TRUE)

// repaint all the screen ---------------------------------------------------

#define repaintAll() \
   WinInvalidateRect(g.hwnd.desktop, NULL, TRUE)

// --------------------------------------------------------------------------

#endif // #ifndef _STLR_PREFMACROS_H_
