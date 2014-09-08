//===========================================================================
// frame.c : enhanced WC_FRAME procedure
// messagges : -------------------------------------------------------------
// WM_ACTIVATE
// WM_ADJUSTWINDOWPOS
// WM_CREATE
// WM_DESTROY
// WM_ENABLE
// WM_FOCUSCHANGE
// WM_HITTEST
// WM_INITMENU
// WM_MENUEND
// WM_MINMAXFRAME
// WM_QUERYBORDERSIZE
// WM_QUERYTRACKINFO
// WM_STYLER2
//    (STLR_AUTOROLL, STLR_AUTOUNRLONCLS, STLR_AUTOUNROLL, STLR_DISABLED,
//     STLR_EDITSYSMENU, STLR_EXPIRED, STLR_HIDEALL, STLR_HIDETITLE,
//     STLR_INITCLIENTCLASS, STLR_MOVEMOUPTR, STLR_UNHIDETITLE, STLR_PROCWKL,
//     STLR_PROCWKR, STLR_PROCWKMENU, STLR_REDRAWTITLE, STLR_RESETSKIPACC,
//     STLR_ROLLALL, STLR_ZORDACTIV
//                   (SW2_SFMOVETOBOTTOM, SW2_SFMOVETOTOP, SW2_SFDEFAULT),
//     STLR_SETFRAMEFONT, STLR_SUBCLASSFLDR, STLR_TOGGLEROLL,
//     STLR_TOGGLETITLEHIDE, STLR_INITWIN32)
// WM_REALIZEPALETTE
// WM_SYSCOMMAND (SC_SIZE,SC_MOVE,SC_TOGGLEROLL,SC_TOGGLETITLEHIDE)
// WM_TRACKFRAME
// WM_TRANSLATEACCEL
// WM_UPDATEFRAME
// WM_WINDOWPOSCHANGED
// * FrameDrag
// Note : in comments "õ<featureName>" indicates that the following lines of
//        code are to implement <featureName>
//===========================================================================


#pragma strings(readonly)

// includes -----------------------------------------------------------------

#include "stlrMain.h"


// imported procedures ------------------------------------------------------

#pragma import(WinTrackWindow,,"PMMERGE.DLL", 5466)
BOOL APIENTRY WinTrackWindow(HWND hwnd, PTRACKINFO pti);
#pragma import(WinIsWindowOfClass,,"PMMERGE.DLL",5437)
BOOL APIENTRY WinIsWindowOfClass(HWND hwnd, PSZ pszClass);


// global variables ---------------------------------------------------------

extern PROCDATA pd;


// prototypes ---------------------------------------------------------------

VOID setFrameFont(PSTLRFRAME p);
VOID disableStyler2Cmd(HWND hwnd, PSTLRFRAME p, BOOL fl);
VOID applyNewWindowSettingsCmd(PSTLRFRAME p, ULONG fl);
VOID setFrameCtrls(PSTLRFRAME p);
VOID makeSysMenuItemText(PSETFRAMECTRLS psfc);
VOID setDefFrameCtrlsImage(PSTLRFRAME p);
VOID setUnrolled(PSTLRFRAME p);
VOID showTitleBar(HWND hwnd, PSTLRFRAME p, BOOL bShow);
VOID changeRollState(PSTLRFRAME p, BOOL bRoll);
VOID rollUnrollAllCmd(HWND hwnd, PSTLRFRAME p);
BOOL isWinInScreen(PSTLRFRAME p);
VOID changeZorderOrActiveStateCmd(HWND hwnd, PSTLRFRAME p, ULONG subCmd);
BOOL isMouseOverWindow(HWND hwnd);
VOID unhideTitleCmd(HWND hwnd, PSTLRFRAME p);
VOID hideTitleCmd(HWND hwnd, PSTLRFRAME p);
VOID autoUnrollCmd(HWND hwnd, PSTLRFRAME p);
VOID processWindowsKeys(ULONG action, HWND hwnd);
VOID stlrOpenObject(PSZ pszObject);
VOID warpCenterCmd(ULONG action);
VOID hideAllWindowsCmd(HWND hwnd, PSTLRFRAME p);
VOID checkClientWinClassCmd(HWND hwnd, PSTLRFRAME p, ULONG idWin);
VOID moveMousePtrCmd(HWND hwnd, PSTLRFRAME p);
VOID stlrStopTracking(HWND hwnd);
// Win32 (odin) frame specific procedures
PSTLRFRAME initWin32FrameData(HWND hwnd);
VOID setWin32FrameCtrlsImage(PSTLRFRAME p);
BOOL isWin32FrameWindow(PSZ pszClassName);
VOID superclassWin32frame(HWND hwnd, PSTLRFRAME p);
MRESULT EXPENTRY stlrWin32FrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
// WPS folders
VOID superclassWPSFoldersCmd(HWND hwnd, PSTLRFRAME p);
MRESULT EXPENTRY stlrWpsFolderProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
// VIO windows
VOID superclassVioWinCmd(HWND hwnd, PSTLRFRAME p);
MRESULT EXPENTRY stlrVioFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY stlrVioClientProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
BOOL EXPENTRY vioClipHook(HAB hab, PQMSG pqmsg, USHORT fs);
BOOL EXPENTRY vioTrackHook(HAB hab, PQMSG pqmsg, USHORT fs);
VOID vioPaste(PSTLRFRAME p);
VOID txt2clipvio(HAB hab, PSZ psz);


// defines ------------------------------------------------------------------

// #define _winData(_hwnd_)       (WinQueryWindowPtr(_hwnd_, g.cd.frame.cbData))
// get the address of the frame data and check if it is valid
#define _getFrameData(_hwnd_, _p_) \
   (((_p_) = WinQueryWindowPtr(_hwnd_, g.cd.frame.cbData)) > (PSTLRFRAME)1)

#define _flagAsWin32Frame(_hwnd_) \
   (WinSetWindowPtr(_hwnd_, g.cd.frame.cbData, FL_WIN32FRAME))

// set the address of the frame data in the window words of the menu windows
// part of the frame controls (FID_MENU, FID_SYSMENU and FID_MINMAX)
#define _setFrameDataAddressInMenu(_hwnd_, _p_) \
   (WinSetWindowPtr(_hwnd_, g.cd.menu.cbData, (PVOID)(_p_)))

// WM_CREATE : cast mp2 to PCREATESTRUCT
#define _pCS(x)        ((PCREATESTRUCT)(x))


// keyboard accelerators ----------------------------------------------------
#define KC_BASICFLAGS          (KC_ALT | KC_CTRL | KC_SHIFT)
// estrae puntatore struttura PCHRMSG dal parametro 1 di WM_TRANSLATEACCEL
#define PCHMFROMMP(_mp_)       ((PCHRMSG)&(((PQMSG)(_mp_))->mp1))
// estrae i dati significativi del mp1 del WM_CHAR di WM_TRANSLATEACCEL
#define TRANSACCMP1(_mp_)      ((ULONG)(((PQMSG)(_mp_))->mp1) & 0xffff00ff)
// ricava handle della finestra che ha causato il WM_TRANSLATEACCEL
//#define HWNDFROMACCMSG(_mp_)   (((PQMSG)(_mp_))->hwnd)
// come sopra ma castato a MPARAM
//#define MPHWNDFROMACCMSG(_mp_) ((MPARAM)(((PQMSG)(_mp_))->hwnd))

// mp1 del WM_CHAR generato dalle windows key
#define MP1LEFTWINKEY      0x7e010044
#define MP1RIGHTWINKEY     0x7f010044
#define MP1MENUKEY         0x7c010044

//===========================================================================
// Enhanced WC_FRAME window procedure.
// Parameters --------------------------------------------------------------
// Ordinary window procedure parameters.
// Return value ------------------------------------------------------------
// MRESULT : message result.
//===========================================================================

MRESULT EXPENTRY stlrFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   PSTLRFRAME p;

switch (msg) {



/* --------------------------------------------------------------------------
 This message occurs when an application causes the activation or
 deactivation of a window.
 param1
     USHORT  usactive      Active indicator.
 param2
     HWND    hwnd          Window handle.
-------------------------------------------------------------------------- */
case WM_ACTIVATE:
//   /*-*/ dbgPrintf4("%08x, WM_ACTIVATE, %08x, %08x\n", hwnd, mp1, mp2);
   if (mp1) {
      g.hwnd.active = hwnd;
      if (o.gen.disabled) break;
      // õ MouseMoveToFocus -------------------------------------------
      // se non Š stato gi… postato il messaggio per questa finestra e non
      // c'Š una nuova finestra o se questa Š la nuova finestra posta msg
      // per implementare lo spostamento automatico del mouse
      if ((hwnd != g.mou.hwnd) && (!g.mou.hwndNew || (hwnd == g.mou.hwndNew))) {
         g.mou.hwnd = hwnd;
         _stlrMsgPost(hwnd, STLR_MOVEMOUPTR, 0);
      } /* endif */
   } else {
      // õ MouseAutoFocus ---------------------------------------------
      if (_winParent(hwnd) == g.hwnd.desktop)
         g.hwnd.autoFocus = g.hwnd.active = NULLHANDLE;
      // õ WinRolling -------------------------------------------------
      if (!o.gen.disabled
          && o.roll.rollOnUnfocus
          && _getFrameData(hwnd, p)
          && p->is.RLon
          && !(p->is.rolled || p->is.min))
         _stlrMsgPost(hwnd, STLR_AUTOROLL, 0);
   } /* endif */
   break;


/* --------------------------------------------------------------------------
 This message is sent by the WinSetWindowPos call to enable the window to
 adjust its new position or size whenever it is about to be moved.
-------------------------------------------------------------------------- */
case WM_ADJUSTWINDOWPOS:
//   /*-*/ dbgPrintf3("%08x, WM_ADJUSTWINDOWPOS - flags: %08x\n",
//                    hwnd, ((PSWP)mp1)->fl);

   if (!_getFrameData(hwnd, p)) break;
   // õ WinSizePos ----------------------------------------------------
   // if 'smooth horizontal movements' is active the desktop is shifted 1
   // pixel right. This correct the problem:
   if ((hwnd == g.hwnd.deskFldr) && o.sizpos.noalign && !pd.we.sizemove &&
       (((PSWP)mp1)->fl & SWP_MOVE) && !((PSWP)mp1)->x)
      ((PSWP)mp1)->x = -1;
   if (((PSWP)mp1)->fl & SWP_SHOW) {
      // õ DialogFont : inizializzazione relativa a primo WM_SHOW -----
      if (p->is.dialog && !p->is.showinit && !pd.we.frame) {
         heapLock(p->hab, hwnd);
         if (NULL != (p->pszFont = memAlloc(CB_FONTNAME))) {
            // if PP_FONTNAMESIZE present store the current font
            if (0 != (p->cchFont = _getPPFont(hwnd,  p->pszFont))) {
               memRealloc(p->pszFont, p->cchFont);
            } else {
               memFree(p->pszFont);
               p->pszFont = NULL;
            } /* endif */
         } /* endif */
         heapUnlock();
         // se opzioni attive resetta font dialogo
         setFrameFont(p);
         p->is.showinit = 1;
      // õ KbdWinKeys : -----------------------------------------------
      // brings to the top the folders opened via the Windows keys
      } else if (p->is.folder && g.is.winkfldr) {
         ((PSWP)mp1)->fl |= SWP_ACTIVATE | SWP_ZORDER;
         ((PSWP)mp1)->hwndInsertBehind = HWND_TOP;
         g.is.winkfldr = 0;
      } /* endif */
   } /* endif */

   if (((PSWP)mp1)->fl & (SWP_MINIMIZE | SWP_MAXIMIZE | SWP_RESTORE)) break;
   // õ WinRolling ----------------------------------------------------
   // elaborazione opzione "Auto-unroll windows vertically resized"
   // se finestra arrotolata viene ridimensionata lungo l'asse y e il
   // ridimensionamento Š permesso
   if (p->is.rolled && (((PSWP)mp1)->fl & SWP_SIZE) &&
       o.roll.letVsize && (((PSWP)mp1)->cy != p->rclCur.cy)) {
      // autounroll rolled win when resizing, terminate tracking
      if (o.roll.unrollOnVsize && g.is.tracking) {
         WinPostMsg(hwnd, WM_CHAR, (MPARAM)0x01011004, (MPARAM)0x0000011b);
         WinPostMsg(hwnd, WM_CHAR, (MPARAM)0x01011044, (MPARAM)0x0040011b);
         WinPostMsg(hwnd, WM_BUTTON1UP, MPVOID, MPVOID);
         WinPostMsg(hwnd, WM_BUTTON2UP, MPVOID, MPVOID);
         _stlrMsgPost(hwnd, STLR_AUTOUNROLL, 0);
         break;
      } /* endif */
      if (!(o.roll.unrollOnFocus && o.roll.rollOnUnfocus)) {
         setUnrolled(p); // setta RLon a 0, cambia bmp bottone
      } /* endif */
      p->is.rolled = 0;
      p->is.resetparent = 1;
   } /* endif */
   break;


/* --------------------------------------------------------------------------
 This message occurs when an application requests the creation of a window.
 param1
     PVOID          ctldata    Pointer to control data.
 param2
     PCREATESTRUCT  pCREATE    Create structure.
-------------------------------------------------------------------------- */
case WM_CREATE:

   /*-*/ dbgCreate1(hwnd, (PFRAMECDATA)mp1, _pCS(mp2));

   // memorizza handle window list
   if (!g.hwnd.winList) g.hwnd.winList = hwnd;
   // check if this is a global exception window
   if (!pd.ppib) break;
   // check if this is an odin window
   if ((pd.type == PROC_ANY)
       && ((ULONG)_pCS(mp2)->pszClass < 0xffff0000)
       && isWin32FrameWindow(_pCS(mp2)->pszClass)) {
//      pd.type = PROC_WIN32;
      _flagAsWin32Frame(hwnd);
      /*-*/ dbgPrintf1("øøø win32 frame window øøø\n");
      break;
   } /* endif */

   // allocate memory to store the frame data
   if (NULL == (p = memLkAlloc(sizeof(STLRFRAME)))) break;
   memset((PVOID)p, 0, sizeof(STLRFRAME));

   // inizializza vari dati nella struttura dati finestra
   p->hab = WinQueryAnchorBlock(hwnd);  // hab - eventualmente cancellare
   p->hwnd = hwnd;                      // hwnd - eventualmente cancellare

   /*-*/ dbgCreate2(p);
   // memorizza pointer nelle window words
   WinSetWindowPtr(hwnd, g.cd.frame.cbData, (PVOID)p);

   // chiama la procedura di default
   if ((BOOL)g.cd.frame.pfnwp(hwnd, WM_CREATE, mp1, mp2)) {
      WinSetWindowPtr(hwnd, g.cd.frame.cbData, (PVOID)NULL);
      memLkFree(p);
      return (MRESULT)TRUE;
   } /* endif */
   /*-*/ dbgCreate3(hwnd, _pCS(mp2));

   // õ FrameControls : default position of the close button ----------
#ifndef STLR_WARP3
   p->is.tbbclose = 2;
#endif
   // õ WinSizePos ----------------------------------------------------
   // se opzione Smooth horizontal movements Š attiva setta FS_NOBYTEALIGN
   if (o.sizpos.noalign && !pd.we.sizemove)
      WinSetWindowBits(hwnd, QWL_STYLE, FS_NOBYTEALIGN, FS_NOBYTEALIGN);

   // controlla se Š finestra principale
   if (_pCS(mp2)->hwndParent == g.hwnd.desktop) {
      p->is.newwin = 1;
      p->is.deskChild = 1;
      g.mou.hwndNew = hwnd;     // õ MouseMoveToFocus
   } /* endif */
   // memorizza frameflags
   if (mp1) p->ffl = ((PFRAMECDATA)mp1)->flCreateFlags;
   // rileva se la frame Š una dialog window
   p->is.dialog = (WinQueryWindowUShort(hwnd, QWS_FLAGS) & FF_DIALOGBOX) != 0;

   // if this is a dialog window without frame controls and is child of another
   // frame window set the p->is.childdlg flag
   if (p->is.dialog && !(p->ffl & FCF_BASIC) && !p->is.deskChild)
      p->is.childdlg = 1;
   // if this is a Win32 (odin) window complete initialization later
//   if (pd.type == PROC_WIN32) {
//      /*-*/ dbgPrintf3("WIN32 - frame flags %08x, titlebar %08x\n",
//                      p->ffl, WinWindowFromID(hwnd, FID_TITLEBAR));
//      if (p->ffl) {
//         _stlrMsgPost(hwnd, STLR_INITWIN32, 0);
//      } else {
//         WinSetWindowPtr(hwnd, g.cd.frame.cbData, (PVOID)NULL);
//         memLkFree(p);
//         return (MRESULT)FALSE;
//      } /* endif */
//   } else {
   // cache FID_SYSMENU, FIX_MINMAX and FID_MENU handles
   if (!pd.we.tbar) {
      if (p->ffl & FCF_SYSMENU)
         p->hSMenu = _WinHWND(hwnd, FID_SYSMENU);
      if (p->ffl & FCF_LEFTCTLS)
         p->hMinMax = _WinHWND(hwnd, FID_MINMAX);
   } /* endif */
//   if (!pd.we.tbarhide && (p->ffl & FCF_MENU))
//      p->hMenu = _WinHWND(hwnd, FID_MENU);
//   } /* endif */
   // store the window position and size
   p->rclCur.x = _pCS(mp2)->x;
   p->rclCur.cx = _pCS(mp2)->cx;
   p->rclCur.y = _pCS(mp2)->y;
   p->rclCur.cy = _pCS(mp2)->cy;
   p->rclRst = p->rclCur;
   // 1st PMSHELL instance (RUNWORKPLACE) check if is VIO window or WinOS/2
   if (pd.type == PROC_SHELL1) {
      if (p->is.deskChild)
         _stlrMsgPost(hwnd, STLR_INITCLIENTCLASS, _pCS(mp2)->id);
   // 2nd PMSHELL instance
   } else if (pd.type == PROC_SHELL2) {
      // is this the launchpad ?
      if (_pCS(mp2)->id == 0x807) {
         p->is.launchpad = 1;
      // LOCKUP DESKTOP FRAME
      } else if ((_pCS(mp2)->id == 0x400) && p->is.dialog && p->is.deskChild &&
                 !_pCS(mp2)->hwndOwner) {
         p->is.lockupwin = 1;
         g.is.locked = 1;
      // WPS FOLDERS
      } else {
         if (((ULONG)_pCS(mp2)->pszClass < 0xffff0000) &&
             !memcmp(_pCS(mp2)->pszClass, "wpFolder window", 16)) {
            if (!g.hwnd.deskFldr) {
               g.hwnd.deskFldr = hwnd;
            } /* endif */
            p->is.folder = 1;
            // se previste estensioni folder posta messaggio per postporre
            // subclassing procedura folder
            _stlrMsgPost(hwnd, STLR_SUBCLASSFLDR, 0);
         } /* endif */
      } /* endif */
   } /* endif */
   if (!o.gen.disabled && (p->ffl & FCF_TBARBTNS)) {
      p->is.skipuf = 1;
      setFrameCtrls(p);
      if (!pd.we.tbar && o.tb.on)
         setDefFrameCtrlsImage(p);
      p->is.skipuf = 0;
   } /* endif */
   /*-*/ dbgPrintf2("%08x succesfully created!\n", hwnd);
   return (MRESULT)FALSE;



/* --------------------------------------------------------------------------
 window destruction :
 free all the allocated resources
-------------------------------------------------------------------------- */
case WM_DESTROY: {
   if (g.hwnd.lastCreated == hwnd) g.hwnd.lastCreated = NULLHANDLE;
   if (g.hwnd.active == hwnd) g.hwnd.active = NULLHANDLE;
   if (!_getFrameData(hwnd, p)) break;
   /*-*/ dbgPrintf4("   ~DESTROY -> hwnd : %08x - %.20s - PID:%d\n",
                    hwnd, p->achTxt, pd.ppib->pib_ulpid);
   // se finestra lockup resetta flag lockup
   if (p->is.lockupwin) g.is.locked = 0;
   // SOSTITUIRE CON WM_SYSTEMSHUTDOWN (0x0054)
   // rileva shutdown sistema tramite dialogo default WPS
   if ((pd.type == PROC_SHELL2) &&
       (WinQueryWindowUShort(hwnd, QWS_ID) == 0x3ee) &&
       p->is.deskChild && WinQueryWindow(hwnd, QW_OWNER)) {
      _stlrMsgSendAll(STLR_AUTOUNRLONCLS, 0);
   } /* endif */
   // reset the window words of the frame window
   WinSetWindowPtr(hwnd, g.cd.frame.cbData, NULL);
   // free the frame window data
   heapLock(p->hab, hwnd);
   if (p->pszFont) memFree(p->pszFont);
   memFree(p);
   memHeapMin();
   heapUnlock();
   break;
}


/* --------------------------------------------------------------------------
 window enabling/disabling
 avoid disabled windows to get the focus
-------------------------------------------------------------------------- */
case WM_ENABLE:
//   /*-*/ dbgPrintf4("%08x, WM_ENABLE, %08x, %08x\n", hwnd, mp1, mp2);

   if (!g.is.expired && _getFrameData(hwnd, p) && p->is.deskChild) {
      if ((ULONG)mp1) {       // finestra abilitata
         p->is.disabled = 0;
      } else {
         p->is.disabled = 1;
      #ifdef STLR_WARP3
         // this is executed only for full window drag in Warp 3
         if (o.sizpos.fulldrag && g.is.tracking) {
            p->is.trkquit = 1;
            WinReleaseHook(p->hab, HMQ_CURRENT, HK_MSGFILTER, (PFN)trkhook, 0);
            WinSetCapture(g.hwnd.desktop, NULLHANDLE);
            g.is.tracking = 0;
         }  /* endif */
      #endif
         // se timer attivato dalla finestra lo blocca
         if (g.hwnd.autoFocus == hwnd) _resetAutoFocusTimer();
      } /* endif */
   } /* endif */
   break;


/* --------------------------------------------------------------------------
 window focus change
 mp1            : is the handle of the window losing or getting focus
 mp2 - SHORT1   : TRUE/FALSE the window is getting/losing focus
       SHORT2   : WinFocusChange flags
 se la finestra viene attivata ed Š stata precedentemente arrotolata
 sulla barra del titolo la srotola, se la finestra viene disattivata si
 riarrotola
-------------------------------------------------------------------------- */
case WM_FOCUSCHANGE:
//   /*-*/ dbgPrintf4("%08x, WM_FOCUSCHANGE, %08x, %08x\n", hwnd, mp1, mp2);

   // questo assicura che uno sbilanciamento tra WM_INITMENU e WM_MENUEND
   // non faccia riapparire il bug del menu con l'autofocus
   if ((USHORT)mp2) {
      g.hwnd.menu = NULLHANDLE;
      g.is.cinitmenu = 0;
      g.is.mousebtn &= ~MENUOPENED;
   } /* endif */
   if (o.gen.disabled
       ||  ((USHORT)mp2 && (hwnd == g.hwnd.active))
       || !_getFrameData(hwnd, p))
      break;

   if (p->is.deskChild) {
      if ((USHORT)mp2) {
         if (p->is.newwin) {
            p->is.newwin = 0;
            g.hwnd.lastCreated = hwnd;
         } /* endif */
         if (p->is.winos2) g.hwnd.active = hwnd;
         g.is.prvwinos2 = p->is.winos2;
         // se eccezioni mouse e timer autofocus Š attivo, lo blocca
         if (pd.we.mouse && g.hwnd.autoFocus) _resetAutoFocusTimer();
      } /* endif */
   } /* endif */
   // õ WinRolling : unroll on focus
//   if (p->is.RLon && (USHORT)mp2 && o.roll.unrollOnFocus && p->is.rolled &&
   if ((USHORT)mp2 && o.roll.unrollOnFocus && p->is.rolled &&
       !(p->is.min || g.is.tracking))
      _stlrMsgPost(hwnd,  STLR_AUTOUNROLL, 0);
   // õ MouseMoveToFocus
   if ((UCHAR)mp2 && p->is.winos2 && (hwnd != g.mou.hwnd) &&
       (!g.mou.hwndNew || (hwnd == g.mou.hwndNew))) {
      g.mou.hwnd = hwnd;
      _stlrMsgPost(hwnd, STLR_MOVEMOUPTR, 0);
   } /* endif */
   break;


/* --------------------------------------------------------------------------
 mp1 : POINTS pointer position
 ?!?!?! : modificare per rendere compatibile con odin
 Usa il messaggio WM_HITTEST per:
 - dare il focus alla finestra sotto il puntatore del mouse
 - nascondere/mostrare la barra del titolo
-------------------------------------------------------------------------- */
case WM_HITTEST: {
   ULONG mr = (ULONG)g.cd.frame.pfnwp(hwnd, msg, mp1, mp2);
//   /*-*/ dbgPrintf4("%08x, WM_HITTEST, %08x, %08x\n", hwnd, mp1, mp2);

   if (o.gen.disabled || !_getFrameData(hwnd, p)) return (MRESULT)mr;

   // disable maximized window sizing (if the option is set)
   if (o.maxwin.nosize && p->is.max && !(pd.we.maxwin || p->is.shield)) {
      mr = 0;
   // disable vertical sizing of rolled windows (if the option is set)
   } else if (!o.roll.letVsize && p->is.rolled) {
      mr &= ~0xa;
   } /* endif */

   // title hiding ---------------------------------------------------------
   // if titlebar-hide is active
   if (p->is.titleHide) {
      // if the titlebar is hidden and the mouse is over the top window border
      if (p->is.titleHidden &&
          (((INT)mr == 2) ||
          (SHORT2FROMMP(mp1) >= (p->rclCur.cy - p->ptBrd.y - 1) &&
          (SHORT2FROMMP(mp1) < p->rclCur.cy)))) {
         // if the timer is not already running or was started by another window
         if (!g.hwnd.unhideTitle || (g.hwnd.unhideTitle != hwnd)) {
            _startUnhideTitlebarTimer(hwnd);
         } /* endif */
      // if the titlebar is not already hidden and the mouse is not on the title
      } else if (!p->is.titleHidden &&
                 (SHORT2FROMMP(mp1) <
                 (o.tbh.inclMn && p->yMenu? p->yMenu: p->wrclTitle.y))) {
         // if the timer is not already running or was started by another window
         if (!g.hwnd.hideTitle || (g.hwnd.hideTitle != hwnd)) {
            _startHideTitlebarTimer(hwnd);
         } /* endif */
      } /* endif */
   } /* endif */
   return (MRESULT)mr;
} // end case WM_HITTEST


/* --------------------------------------------------------------------------
 This message is sent to a frame window that is being minimized, maximized,
 or restored.
 param1
     PSWP   pswp         Set window position structure.
 param2
     ULONG  ulReserved   Reserved value, should be 0.

 se opzioni window roling attive controlla dimensioni e posizione
 finestra in accordo con il su stato di rolling
-------------------------------------------------------------------------- */
case WM_MINMAXFRAME:
//   /*-*/ dbgPrintf4("%08x, WM_MINMAXFRAME, %08x, %08x\n", hwnd, mp1, mp2);

   if (!_getFrameData(hwnd, p)) break;

   // õ MaximizedWindows : override size and position
   if ((((PSWP)mp1)->fl & SWP_MAXIMIZE) &&
       o.maxwin.on && p->is.deskChild && !pd.we.maxwin && !p->is.shield) {
      ((PSWP)mp1)->x = (((SHORT)o.maxwin.x) < 0)?
                                   0xffff0000 | o.maxwin.x: o.maxwin.x;
      ((PSWP)mp1)->y = (((SHORT)o.maxwin.y) < 0)?
                                   0xffff0000 | o.maxwin.y: o.maxwin.y;
      ((PSWP)mp1)->cx = o.maxwin.cx;
      ((PSWP)mp1)->cy = o.maxwin.cy;
   } /* endif */
   // õ WinRolling
   if (o.roll.on && p->is.RLon) {               // stato rolling attivo
      // ripristino --------------------------------------------------------
      //*** le finestre minimizzate quando sono rollate, vengono sempre
      // ripristinate intere!!! Questo Š l'unico sistema perch‚ funzionino
      // correttamente (per ora)
      if (((PSWP)mp1)->fl & SWP_RESTORE) {
         if (p->is.min) {           // stato precedente minimizzazione
            if (p->is.rolled) {     // precedentemente rolled
               setUnrolled(p); // setta RLon a 0, cambia bmp bottone
               p->is.rolled = 0;
               p->is.resetparent = 1;
            } else {                  // precedentemente non rolled
               if (!(o.roll.unrollOnFocus && o.roll.rollOnUnfocus)) {
                  setUnrolled(p); // setta RLon a 0, cambia bmp bottone
               } /* endif */
            } /* endif */
         } else {                     // stato precedente massimizzazione
            if (!p->is.rolled && o.roll.unrollOnFocus && !o.roll.rollOnUnfocus) {
               setUnrolled(p);
            } else {
               // memorizza coordinate ripristino
               p->rclRst.x = ((PSWP)mp1)->x;
               p->rclRst.y = ((PSWP)mp1)->y;
               p->rclRst.cx = ((PSWP)mp1)->cx;
               p->rclRst.cy = ((PSWP)mp1)->cy;
               if (!o.roll.bottom)
                  ((PSWP)mp1)->y += ((PSWP)mp1)->cy - p->cyRoll;
               ((PSWP)mp1)->cy = p->cyRoll;
               if (!p->is.rolled && (!o.roll.unrollOnFocus || o.roll.rollOnUnfocus)) {
                  p->is.rolled = 1;
                  //p->is.resetparent = 1;
               } /* endif */
            } /* endif */
         } /* endif */
      // massimizzazione ---------------------------------------------------
      } else if (((PSWP)mp1)->fl & SWP_MAXIMIZE) {
         if (p->is.min) {           // precedentemente minimizzata
            if (p->is.rolled) {     // precedentemente rolled
               if (!(o.roll.unrollOnFocus && o.roll.rollOnUnfocus)) {
                  setUnrolled(p); // setta RLon a 0, cambia bmp bottone
               } /* endif */
               p->is.rolled = 0;
               p->is.resetparent = 1;
            } else {                  // precedentemente non rolled
               if (!o.roll.unrollOnFocus || o.roll.rollOnUnfocus) {
                  setUnrolled(p); // setta RLon a 0, cambia bmp bottone
               } /* endif */
            } /* endif */
         } else {                    // precedentemente normale
            if (p->is.rolled) {     // precedentemente rolled
               // modifica opportunamente dimensioni ripristino
               WinSetWindowUShort(hwnd, QWS_XRESTORE, p->rclRst.x);
               WinSetWindowUShort(hwnd, QWS_YRESTORE, p->rclRst.y);
               WinSetWindowUShort(hwnd, QWS_CXRESTORE, p->rclRst.cx);
               WinSetWindowUShort(hwnd, QWS_CYRESTORE, p->rclRst.cy);
               if (!(o.roll.unrollOnFocus && o.roll.rollOnUnfocus)) {
                  setUnrolled(p); // setta RLon a 0, cambia bmp bottone
               } /* endif */
               p->is.rolled = 0;
               p->is.resetparent = 1;
            } else {                  // precedentemente non rolled
               if (!(o.roll.unrollOnFocus && o.roll.rollOnUnfocus)) {
                  setUnrolled(p); // setta RLon a 0, cambia bmp bottone
               } /* endif */
            } /* endif */
         } /* endif */
      // minimizzazione ----------------------------------------------------
      } else {
         // se correntemente arrotolata modifica opportunamente le
         // dimensioni di ripristino
         if (p->is.rolled) {
            WinSetWindowUShort(hwnd, QWS_XRESTORE, p->rclRst.x);
            WinSetWindowUShort(hwnd, QWS_YRESTORE, p->rclRst.y);
            WinSetWindowUShort(hwnd, QWS_CXRESTORE, p->rclRst.cx);
            WinSetWindowUShort(hwnd, QWS_CYRESTORE, p->rclRst.cy);
         } /* endif */
      } /* endif */
   } /* endif */
   break;
// end case WM_MINMAXFRAME

//      case WM_PAINT:
//         return PaintSmartWinFrame(hwnd, mp1, mp2);


/* --------------------------------------------------------------------------
 This message is sent to the frame window to determine the width and height
 of the border of the window.
-------------------------------------------------------------------------- */
case WM_QUERYBORDERSIZE: {
   MRESULT mr = (MRESULT)g.cd.frame.pfnwp(hwnd, WM_QUERYBORDERSIZE, mp1, mp2);
//   /*-*/ dbgPrintf4("%08x, WM_QUERYBORDERSIZE, %08x, %08x\n", hwnd, mp1, mp2);

   if (g.is.expired) return mr;

   if (!_getFrameData(hwnd, p)) return mr;
   p->ptBrd.x = ((PPOINTL)mp1)->x;
   p->ptBrd.y = ((PPOINTL)mp1)->y;
   p->cyRoll = 2 * p->ptBrd.y + p->wrclTitle.cy;

   return mr;
}


/* --------------------------------------------------------------------------
 Styler/2 private message
 this is used to perform the required Styler/2 feature asynchronously
-------------------------------------------------------------------------- */
case WM_STYLER2:
//   /*-*/ dbgPrintf4("%08x, WM_QUERYFRAMEINFO, %08x, %08x\n", hwnd, mp1, mp2);
   if (((ULONG)mp1 < CMDSRC_SMWMIN) || ((ULONG)mp1 > CMDSRC_SMWMAX))
      break;

   switch ((ULONG)mp1) {
   // left Windows logo key pressed ---------------------------------------
      case STLR_PROCWKL:
         processWindowsKeys(o.winkbd.left, (HWND)mp2);
         break;
   // right Windows logo key pressed --------------------------------------
      case STLR_PROCWKR:
         processWindowsKeys(o.winkbd.right, (HWND)mp2);
         break;
   // Windows menu key pressed --------------------------------------------
      case STLR_PROCWKMENU:
         processWindowsKeys(o.winkbd.menu, (HWND)mp2);
         break;
   // warp center command -------------------------------------------------
      case STLR_WARPCENTERCMD:
         warpCenterCmd((ULONG)mp2);
         break;
   // reduce the window height to the titlebar height ---------------------
      case STLR_AUTOROLL:
         if ((NULL != (p = (PSTLRFRAME)mp2)) || _getFrameData(hwnd, p))
            changeRollState(p, TRUE);
         break;
   // complete the initialization of the win32 (odin) windows -------------
      case STLR_INITWIN32:
         return (MRESULT)initWin32FrameData(hwnd);
   // these cases are processed only if valid frame data are present
      default:
         if (!_getFrameData(hwnd, p)) break;
         switch ((ULONG)mp1) {
            case STLR_SETFRAMEFONT:
               if (!pd.we.frame) setFrameFont(p);
               break;
         // superclass the WPS folder windows -----------------------------
            // oltre che dal WM_CREATE viene broadcastato dal programma di
            // preferenze quando si decide di attivare/disattivare gli
            // acceleratori dei folder
            case STLR_SUBCLASSFLDR:
               superclassWPSFoldersCmd(hwnd, p);
               break;
         // update the system menu content --------------------------------
            case STLR_EDITSYSMENU:
               if (!pd.we.tbar) {
                  if (p->ffl & FCF_SYSMENU)
                     p->hSMenu = _WinHWND(hwnd, FID_SYSMENU);
                  if (p->ffl & FCF_LEFTCTLS)
                     p->hMinMax = _WinHWND(hwnd, FID_MINMAX);
               } /* endif */
               p->is.skipuf = 1;
               setFrameCtrls(p);
               p->is.skipuf = 0;
               g.cd.frame.pfnwp(hwnd, WM_UPDATEFRAME, MPVOID, MPVOID);
               break;
         // update window settings ----------------------------------------
            case STLR_REDRAWTITLE:
               applyNewWindowSettingsCmd(p, (ULONG)mp2);
               break;
         // roll/unroll all windows ---------------------------------------
            case STLR_ROLLALL:
               rollUnrollAllCmd(hwnd, p);
               break;
         // change the window Z-order optionally giving it the focus ------
            case STLR_ZORDACTIV:
               changeZorderOrActiveStateCmd(hwnd, p, (ULONG)mp2);
               break;
         // unhide the titlebar -------------------------------------------
            case STLR_UNHIDETITLE:
               unhideTitleCmd(hwnd, p);
               break;
         // hide the titlebar ---------------------------------------------
            case STLR_HIDETITLE:
               hideTitleCmd(hwnd, p);
               break;
         // unroll a window as it gets the focus or gets resized ----------
            case STLR_AUTOUNROLL:
               autoUnrollCmd(hwnd, p);
               break;
         // unroll a window as it is closed -------------------------------
            case STLR_AUTOUNRLONCLS:
               p->is.RLon = 0;
               if (p->is.rolled) changeRollState(p, FALSE);
               break;
         // trial time expiration -----------------------------------------
            case STLR_EXPIRED:
         // enable/disable Styler/2 ---------------------------------------
            case STLR_DISABLED:
               disableStyler2Cmd(hwnd, p, (BOOL)mp2);
               break;
         // hide all the windows from the desktop -------------------------
            case STLR_HIDEALL:
               hideAllWindowsCmd(hwnd, p);
               break;
         // get the class of FID_CLIENT -----------------------------------
            case STLR_INITCLIENTCLASS:
               checkClientWinClassCmd(hwnd, p, (ULONG)mp2);
               break;
         // move the mouse pointer to the focus window --------------------
            case STLR_MOVEMOUPTR:
               moveMousePtrCmd(hwnd, p);
               break;
         } /* endswitch */
      break;
   } /* endswitch */
   return (MRESULT)TRUE;


/* --------------------------------------------------------------------------
 This message is sent to an application whenever changes have been made to
 the display hardware physical color table as a result of another
 application calling WinRealizePalette.
-------------------------------------------------------------------------- */

//case WM_REALIZEPALETTE:
//   if (hwnd == g.hwnd.active)
//      WinPostMsg(g.timerThrd.hwnd, SMWM_RESETBITMAPS, MPVOID, MPVOID);
//   break;


/* --------------------------------------------------------------------------
 Process events from system menu items added by Styler/2.
-------------------------------------------------------------------------- */

case WM_SYSCOMMAND:
//   /*-*/ dbgPrintf4("%08x, WM_SYSCOMMAND, %08x, %08x\n", hwnd, mp1, mp2);
   if (o.gen.disabled || !_getFrameData(hwnd, p))
      break;

   switch ((USHORT)mp1) {
      case SC_TOGGLEROLL:
         toggleRollCmd(hwnd, p);
         return (MRESULT)FALSE;
      case SC_TOGGLETITLEHIDE:
         toggleTitleBarHidingCmd(hwnd, p);
         return (MRESULT)FALSE;
   } /* endswitch */
   break;


/* --------------------------------------------------------------------------
// Se la finestra Š arrotolata, al ritorno dal messaggio WM_QUERYTRACKINFO,
// modifica ptlMinTrackSize.y in modo che sia:
// ptlMinTrackSize.y = p->cyRoll.
// Se Š stile full window drag chiama trkframe(), altrimenti chiama
// WinTrackRect()
-------------------------------------------------------------------------- */
case WM_TRACKFRAME: {
   BOOL rc = FALSE;
   PFULLWDRAG pfwd;
//   /*-*/ dbgPrintf4("%08x, WM_TRACKFRAME, %08x, %08x\n", hwnd, mp1, mp2);
   if (o.gen.disabled || pd.we.sizemove
       || !_getFrameData(hwnd, p)
       || !(pfwd = memLkAlloc(sizeof(FULLWDRAG))))
      break;
   g.is.tracking = 1;
   memset(pfwd, 0, sizeof(FULLWDRAG));
   // if Ctrl is not pressed activate the window
   pfwd->bAct = !(WinGetKeyState(g.hwnd.desktop, VK_CTRL) & 0x8000);
   if (WinSendMsg(hwnd, WM_QUERYTRACKINFO, mp1, (MPARAM)pfwd)) {
      if (p->is.rolled) pfwd->ptlMinTrackSize.y = p->cyRoll;
      // se conseguente ad operazione avviata da tastiera centra il mouse
      // usando l'icona SPTR_MOVE
      if (pfwd->fs & TF_SETPOINTERPOS) {
         pfwd->ptl.x = p->rclCur.cx / 2;
         pfwd->ptl.y = p->rclCur.cy / 2;
         WinMapWindowPoints(hwnd, g.hwnd.desktop, &pfwd->ptl, 1);
         WinSetPointerPos(g.hwnd.desktop, pfwd->ptl.x, pfwd->ptl.y);
         _setSysMouPtr(SPTR_MOVE);
      } else if (mp2) {
         pfwd->ptl.x = MOUSEX(mp2) < 0? 0xffff0000 | MOUSEX(mp2): MOUSEX(mp2);
         pfwd->ptl.y = MOUSEY(mp2) < 0? 0xffff0000 | MOUSEY(mp2): MOUSEY(mp2);
         WinMapWindowPoints(p->hTBar, g.hwnd.desktop, &pfwd->ptl, 1);
      } else {
         WinQueryPointerPos(g.hwnd.desktop, &pfwd->ptl);
      }  /* endif */
      pfwd->hpar = _winParent(hwnd);
      // se full window drag setta direttamente l'hook MSGFILTER
      if (o.sizpos.fulldrag) {
         if (pfwd->bAct) {
            _frameActivate(hwnd);
            pfwd->bAct = FALSE;
         } /* endif */
      #ifdef STLR_WARP3
         rc = stlrTrackWindow(hwnd, pfwd, p);
      #else
         rc = WinTrackWindow(hwnd, (PTRACKINFO)pfwd);
      #endif
      } else {
         rc = WinTrackRect(pfwd->hpar, NULLHANDLE, (PTRACKINFO)pfwd);
      } /* endif */
      if (rc)
         WinSetWindowPos(hwnd, NULLHANDLE,
                         pfwd->rclTrack.xLeft, pfwd->rclTrack.yBottom,
                         pfwd->rclTrack.xRight - pfwd->rclTrack.xLeft,
                         pfwd->rclTrack.yTop - pfwd->rclTrack.yBottom,
                         SWP_SIZE | SWP_MOVE);
   } /* endif */
   g.is.tracking = 0;
   p->is.trkquit = 0;
   if (pfwd->bAct) _frameActivate(hwnd);
   memLkFree(pfwd);
   return (MRESULT)rc;
}


/* --------------------------------------------------------------------------
 Controlla i parametri della coda messaggi, se questa non Š processata
 dalla procedura di default, modificandoli opportunamente se questi
 corrispondono ad una combinazione di tastiera valida.
-------------------------------------------------------------------------- */

case WM_TRANSLATEACCEL:
//   /*-*/ dbgPrintf4("%08x, WM_TRANSLATEACCEL, %08x, %08x\n", hwnd, mp1, mp2);

   if (o.gen.disabled || g.is.locked || !_getFrameData(hwnd, p) || p->is.shield)
      break;
   // prima processa accelleratori default
   if (g.cd.frame.pfnwp(hwnd, msg, mp1, mp2)) return (MRESULT)TRUE;
   msg = 0;
   // se flag ignora accelleratori restituisce FALSE;
   //if (p->is.skipwkacc || g.is.winkfldr) return (MRESULT)FALSE;
   // se Š uno dei tasti win 95 lo processa e restituisce TRUE
   if (TRANSACCMP1(mp1) == MP1LEFTWINKEY) {
      if (o.winkbd.left) {
         ((PQMSG)mp1)->mp1 = (MPARAM)STLR_PROCWKL;
         goto wkeyprocessed;
      } /* endif */
   } else if (TRANSACCMP1(mp1) == MP1RIGHTWINKEY) {
      if (o.winkbd.right) {
         ((PQMSG)mp1)->mp1 = (MPARAM)STLR_PROCWKR;
         goto wkeyprocessed;
      } /* endif */
   } else if (TRANSACCMP1(mp1) == MP1MENUKEY) {
      if (o.winkbd.menu) {
         ((PQMSG)mp1)->mp1 = (MPARAM)STLR_PROCWKMENU;
         goto wkeyprocessed;
      } /* endif */
   // non processa il rilascio dei tasti
   } else if (PCHMFROMMP(mp1)->fs & KC_KEYUP) {
      return (MRESULT)FALSE;
   // non permette di arrotolare o nascondere la barra del titolo del desktop
   } else if (hwnd == g.hwnd.deskFldr) {
      return (MRESULT)FALSE;
   // arrotolamento/srotolamento per tutte le finestre
   } else if (((PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.roll.kbdKC) &&
              (PCHMFROMMP(mp1)->vkey == (o.roll.kbdVK & 0xff))) {
      if (!pd.we.wroll && o.roll.on && o.roll.kbd)
         msg = SC_TOGGLEROLL;
   // eliminazione barra del titolo
   } else if (((PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.tbh.kbdKC) &&
              (PCHMFROMMP(mp1)->vkey == (o.tbh.kbdVK & 0xff))) {
      if (!pd.we.tbarhide && o.tbh.on && o.tbh.kbd)
         msg = SC_TOGGLETITLEHIDE;
   } /* endif */
   if (!msg) return (MRESULT)FALSE;
   ((PQMSG)mp1)->msg = WM_SYSCOMMAND;
   ((PQMSG)mp1)->hwnd = hwnd;
   ((PQMSG)mp1)->mp1 = (MPARAM)msg;
   ((PQMSG)mp1)->mp2 = (MPARAM)2;
   return (MRESULT)TRUE;
wkeyprocessed:
   ((PQMSG)mp1)->msg = WM_STYLER2;
   ((PQMSG)mp1)->mp2 = (MPARAM)((PQMSG)mp1)->hwnd;
   ((PQMSG)mp1)->hwnd = hwnd;
   return (MRESULT)TRUE;


/* --------------------------------------------------------------------------
 aggiorna le flag della frame e se necessario ricava handle bitmap non
 ancora disponibili
-------------------------------------------------------------------------- */
case WM_UPDATEFRAME: {
   BOOL reset = FALSE;
//   /*-*/ dbgPrintf4("%08x, WM_UPDATEFRAME, %08x, %08x\n", hwnd, mp1, mp2);

   if (!_getFrameData(hwnd, p)) break;
   if (p->is.childdlg) return (MRESULT)FALSE;
   if (p->is.skipuf)  return (MRESULT)TRUE;
   if (p->is.rolled) {
      WinSetMultWindowPos(p->hab, p->rwc.ppLc, p->rwc.cLc);
      WinSetMultWindowPos(p->hab, p->rwc.ppRc, p->rwc.cRc);
      repaintWindowR(hwnd);
      return (MRESULT)TRUE;
   } /* endif */
   if ((LONG)mp1 == -1) break;
   if ((BOOL)mp1) {
      if (!pd.we.tbar) {
         // system menu
         if (((ULONG)mp1 & FCF_SYSMENU) && !(p->ffl & FCF_SYSMENU)) {
            if (NULLHANDLE != (p->hSMenu = _WinHWND(hwnd, FID_SYSMENU)))
               reset = TRUE;
            p->ffl |= FCF_SYSMENU;
         } /* endif */
         // minmax
         if (((ULONG)mp1 & FCF_LEFTCTLS) != (p->ffl & FCF_LEFTCTLS)) {
            if (NULLHANDLE != (p->hMinMax = _WinHWND(hwnd, FID_MINMAX))) {
               p->is.minMaxAdd = 0;
               reset = TRUE;
            } /* endif */
            p->ffl |= ((ULONG)mp1 & FCF_LEFTCTLS);
         } /* endif */
      } /* endif */
      // menubar
      if (!pd.we.tbarhide
          && ((ULONG)mp1 & FCF_MENU) && !(p->ffl & FCF_MENU)) {
         p->ffl |= FCF_MENU;
      } /* endif */
      if (reset) {
         p->is.skipuf = 1;
         setFrameCtrls(p);
         if (!pd.we.tbar) setDefFrameCtrlsImage(p);
         p->is.skipuf = 0;
      } /* endif */
   } /* endif */
}  break;


/* --------------------------------------------------------------------------
 Rileva dimensioni e posizione finestra:
 nel caso di uno spostamento o di un ridimensionamento aggiorna in una
 struttura di tipo RECTL la posizione e le dimensioni della finestra.
 Se la finestra viene ridimensionata lungo l'asse y perde lo stato di
 WRAPPED, la flag FF_WRAPPED viene resettata e vengono memorizzate tutte
 le nuove coordinate.
 Nel caso contrario la posizione della finestra allo stato non WRAPPED
 viene aggiornata secondo lo spostamento della finestra mentre la
 dimensione cy non viene alterata dallo stato precedente e viene
 richiamata la procedura di default.
-------------------------------------------------------------------------- */

case WM_WINDOWPOSCHANGED: {
   MRESULT mr;

//   /*-*/ dbgPrintf3("%08x, WM_WINDOWPOSCHANGED - flags: %08x\n",
//                    hwnd, ((PSWP)mp1)->fl);

   // ottiene i dati della finestra controllandone la validit…
   if (pd.we.wroll || !_getFrameData(hwnd, p) || p->is.childdlg) break;

   // se si verifica un cambiamento di posiz o dimens li memorizza
   if (((PSWP)mp1)->fl & (SWP_SIZE | SWP_MOVE)) {
      p->rclCur.x = ((PSWP)mp1)->x;
      p->rclCur.cx = ((PSWP)mp1)->cx;
      p->rclCur.y = ((PSWP)mp1)->y;
      p->rclCur.cy = ((PSWP)mp1)->cy;
   } /* endif */

   // massimizzazione -----------------------------------------------------
   if (((PSWP)mp1)->fl & SWP_MAXIMIZE) {
      p->is.min = 0;
      p->is.max = 1;                // setta flag
      p->is.remax = 0;
      p->rclMax = p->rclCur;      // memorizza dimensioni max
   // se Š una finestra la cui procedura di massimizzazione Š subclassata
   // setta flag in maniera che non venga applicata opzione che impedisce
   // lo spostamento delle finestre massimizzate
      if (o.maxwin.nomove &&
          (o.maxwin.on &&
           (((PSWP)mp1)->cx < o.maxwin.cx ||
           ((PSWP)mp1)->cy < o.maxwin.cy)) ||
          (!o.maxwin.on &&
           (((PSWP)mp1)->cx < g.sys.scr.cx ||
           ((PSWP)mp1)->cy < g.sys.scr.cy)))
         p->is.ovrdnomovemax = 1;
   // minimizzazione o ripristino -----------------------------------------
   } else if (((PSWP)mp1)->fl & SWP_MINIMIZE) {
      p->is.min = 1;
      p->is.max = 0;
      // evita che le nuove minimizzate sospendano l'auto-focus
      if (p->is.newwin) p->is.newwin = 0;
      if (g.hwnd.lastCreated == hwnd) g.hwnd.lastCreated = NULLHANDLE;
   } else if (((PSWP)mp1)->fl & SWP_RESTORE) {
      if (p->is.rolled) {
         int i = 0;
         if (p->hClient) WinShowWindow(p->hClient, FALSE);
         while (i < p->rwc.chhc) WinShowWindow(p->rwc.phhc[i++], FALSE);
      } /* endif */
      p->is.min = 0;
      p->is.max = 0;
      p->is.remax = 0;
      if (!p->is.rolled) p->rclRst = p->rclCur;
   // cambiamento posiz-dimensioni ----------------------------------------
   } else if (((PSWP)mp1)->fl & (SWP_SIZE | SWP_MOVE)) {
      if (p->is.rolled) {         // finestra "arrotolata"
         if (((PSWP)mp1)->cy == ((PSWP)mp1)[1].cy) {  // cy immutato
            if (p->is.max) {
               p->rclMax.y += ((PSWP)mp1)->y - ((PSWP)mp1)[1].y;
               p->rclMax.x = ((PSWP)mp1)->x;
               p->rclMax.cx = ((PSWP)mp1)->cx;
            } else {
               p->rclRst.y += ((PSWP)mp1)->y - ((PSWP)mp1)[1].y;
               p->rclRst.x = ((PSWP)mp1)->x;
               p->rclRst.cx = ((PSWP)mp1)->cx;
            } /* endif */
         } /* endif */
      } else if (p->is.max) {
         p->rclMax = p->rclCur;
      } else {
         p->rclRst = p->rclCur;
      } /* endif */
   } /* endif */
   // questo blocco serve per evitare che eventuali controlli aggiunti ai
   // controlli di default della frame vengano disegnati sopra la titlebar
   // della finestra arrotolata
   // se la finestra Š arrotolata e ridimensionata
   if (p->is.RLon &&
       (((PSWP)mp1)->fl & SWP_SIZE)) {
      INT enlarge = ((PSWP)mp1)[0].cx - ((PSWP)mp1)[1].cx;
      if (enlarge) { // se ridimensionata orizzontalmente
         int i;
         // ridimensiona la barra del titolo
         p->rwc.ppRc[0].cx += enlarge;
         for (i = 1; i < p->rwc.cRc; ++i) {
            p->rwc.ppRc[i].x += enlarge;
         } /* endfor */
      } /* endif */
      if (p->is.rolled && !(((PSWP)mp1)->fl & SWP_MINIMIZE)) {
         if (((PSWP)mp1)[0].cy > ((PSWP)mp1)[1].cy) // solo se srotolamento
            p->is.resetparent = 1; // compatibilit… con PMVIEW
         WinSetMultWindowPos(p->hab, p->rwc.ppLc, p->rwc.cLc);
         WinSetMultWindowPos(p->hab, p->rwc.ppRc, p->rwc.cRc);
         repaintWindowR(hwnd);
         return(MRESULT)FALSE;
      } /* endif */
   } /* endif */

   mr = g.cd.frame.pfnwp(hwnd, msg, mp1, mp2);
   // nasconde/mostra child windows in caso di arrotolamento/srotolamento
   if (p->is.resetparent) {
      INT i = 0;
      p->is.resetparent = 0;
      if (p->hClient) WinShowWindow(p->hClient, TRUE);
      while (i < p->rwc.chhc) WinShowWindow(p->rwc.phhc[i++], TRUE);
   } /* endif */
   return mr;
}


//========================================================================
} /* endswitch */


/* --------------------------------------------------------------------------
 esegue il FrameDrag
-------------------------------------------------------------------------- */

   if (o.sizpos.framedrg                                 // option active
      && !g.is.locked                                    // system not locked
      && (msg == o.sizpos.frameMoumsg)                   // frame drag event
      && (SHORT2FROMMP(mp2) == o.sizpos.frameMoukbd)
      && !o.gen.disabled                                 // styler/2 enabled
      && _getFrameData(hwnd, p)                          // get frame data
      && !p->is.childdlg                                 // frame state
      && !p->is.min                                      // compatible with
      && !pd.we.sizemove                                 // frame drag feature
      && (!(o.maxwin.nomove && p->is.max)
         || pd.we.maxwin
         || p->is.shield
         || p->is.ovrdnomovemax)
      && (SHORT1FROMMP(mp2)                              // mouse positioned
         || ((MOUSEX(mp1) >= 0)                          // on the window border
             && (MOUSEX(mp1) < p->rclCur.cx)
             && (MOUSEY(mp1) >= 0)
             && (MOUSEY(mp1) < p->rclCur.cy)
             && ((MOUSEX(mp1) < 5)
                || (MOUSEX(mp1) > (p->rclCur.cx - 5))
                || (MOUSEY(mp1) < 5)
                || (MOUSEY(mp1) > (p->rclCur.cy - 5))))))
   {
      _setSysMouPtr(SPTR_MOVE);
      WinSendMsg(hwnd, WM_TRACKFRAME, (MPARAM)TF_MOVE, MPVOID);
      return (MRESULT)TRUE;
   } /* endif */


//========================================================================

   return (MRESULT)g.cd.frame.pfnwp(hwnd, msg, mp1, mp2);
}


//===========================================================================
// Reimposta il font usato dai dialoghi.
// Parameters --------------------------------------------------------------
// PSTLRFRAME p : windows data.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID setFrameFont(PSTLRFRAME p) {
   if (p->is.dialog) {
      if (!o.gen.disabled && o.dlg.font &&  // setta font SmartWindows
          (o.dlg.ovrPP || !p->cchFont) &&
          (!p->cchFont || memcmp(o.dlg.achFont, p->pszFont, p->cchFont))) {
          _setPPFont(p->hwnd, o.dlg.achFont);
      } else if (p->cchFont) {     // reset font precedente
          _setPPFont(p->hwnd, p->pszFont);
      } else if (p->is.showinit) {  // se gi… inizializzato
          WinRemovePresParam(p->hwnd, PP_FONTNAMESIZE);
      } /* endif */
   } /* endif */
}


//===========================================================================
// Enable/disable the Styler/2 extensions.
// Parameters --------------------------------------------------------------
// HWND hwnd  : window handle.
// PSTLRFRAME p : window data.
// BOOL fl    : TRUE/FALSE (disable/enable).
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
_Inline
VOID disableStyler2Cmd(HWND hwnd, PSTLRFRAME p, BOOL fl) {
   if (!pd.we.tbar) {
      if (p->ffl & FCF_SYSMENU)
         p->hSMenu = _WinHWND(hwnd, FID_SYSMENU);
      if (p->ffl & FCF_LEFTCTLS)
         p->hMinMax = _WinHWND(hwnd, FID_MINMAX);
   } /* endif */
   // restore the superclassed procedures
   if (p->is.win32) {
      superclassWin32frame(hwnd, p);
   } else if (p->is.shield) {
      superclassVioWinCmd(hwnd, p);
   } else if (p->is.folder) {
      superclassWPSFoldersCmd(hwnd, p);
   } /* endif */
   // styler/2 is going to be disabled ------------------------------
   if (fl) {
      if (p->is.win32) {
         setWin32FrameCtrlsImage(p);
         repaintWindow(p->hTBar);
      } else {
         if (p->is.titleHidden) // se titolo nascosto lo mostra
            showTitleBar(p->hwnd, p, TRUE);
         if (p->is.rolled)      // se finestra rollata unroll
            changeRollState(p, FALSE);
         if (p->ffl) {
            p->is.skipuf = 1;
            setFrameCtrls(p);
            setDefFrameCtrlsImage(p);
            g.cd.frame.pfnwp(p->hwnd, WM_UPDATEFRAME, MPVOID, MPVOID);
            repaintWindow(p->hTBar);
            p->is.skipuf = 0;
         } /* endif */
      } /* endif */
   } else {         // abilitazione ---------------------------------
      applyNewWindowSettingsCmd(p, MODIF_ALLCMNBMP);
   } /* endif */
   if (!pd.we.frame) setFrameFont(p);
}


//===========================================================================
// Apply the new window settings (the message is sent by the preferences
// program).
// Parameters --------------------------------------------------------------
// PSTLRFRAME p : window data.
// ULONG fl   : setting to be updated.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID applyNewWindowSettingsCmd(PSTLRFRAME p, ULONG fl) {
   if (!pd.we.tbar) {
      if (p->ffl & FCF_SYSMENU)
         p->hSMenu = _WinHWND(p->hwnd, FID_SYSMENU);
      if (p->ffl & FCF_LEFTCTLS)
         p->hMinMax = _WinHWND(p->hwnd, FID_MINMAX);
   } /* endif */
   p->hSMenu = _WinHWND(p->hwnd, FID_SYSMENU);
   p->hMinMax = _WinHWND(p->hwnd, FID_MINMAX);
   p->is.skipuf = 1;
   // update the titlebar font
   if (p->hTBar && (fl & MODIF_FONT) && !pd.we.tbar)
      _setPPFont(p->hTBar, o.tb.achFont);
   if (p->is.win32) {
      if (!pd.we.tbar) setWin32FrameCtrlsImage(p);
   } else {
      // update the frame controls
      if (fl & (MODIF_CLOSEPOS | MODIF_TBHIDE | MODIF_WROLL))
         setFrameCtrls(p);
      // aggiorna bitmap controlli finestra
      if (!pd.we.tbar) {
         if (p->hSMenu && (fl & MODIF_SYSMN))
            _setFrameCtrlBmp(p->hSMenu, SC_SYSMENU,
                             (o.tb.bmp.sysmenu?
                              o.tb.bmp.sysmenu: g.bmpDef.sys));
         if (p->is.tbbclose && (fl & MODIF_CLOSEBMP))
            _setFrameCtrlBmp((p->is.tbbclose == 1? p->hSMenu: p->hMinMax),
                            SC_CLOSE, (o.tb.bmp.close?
                                       o.tb.bmp.close: g.bmpDef.cls));
         if (p->hMinMax) {
            if (fl & MODIF_MIN)
               _setFrameCtrlBmp(p->hMinMax, SC_MINIMIZE,
                                (o.tb.bmp.min? o.tb.bmp.min: g.bmpDef.min));
            if (fl & MODIF_HIDE)
               _setFrameCtrlBmp(p->hMinMax, SC_HIDE,
                                (o.tb.bmp.hide? o.tb.bmp.hide: g.bmpDef.hide));
            if (fl & MODIF_REST)
               _setFrameCtrlBmp(p->hMinMax, SC_RESTORE,
                                (o.tb.bmp.rest? o.tb.bmp.rest: g.bmpDef.rest));
            if (fl & MODIF_MAX)
               _setFrameCtrlBmp(p->hMinMax, SC_MAXIMIZE,
                                (o.tb.bmp.max? o.tb.bmp.max: g.bmpDef.max));
            if (o.tbh.on && (fl & MODIF_TBHIDEUNHIDE) && !pd.we.tbarhide)
               _setFrameCtrlBmp(p->hMinMax, SC_TOGGLETITLEHIDE,
                                (p->is.titleHide?
                                 o.tb.bmp.unhtbar: o.tb.bmp.htbar));
            if (o.roll.on && (fl & MODIF_WROLLUNROLL) && !pd.we.wroll)
               _setFrameCtrlBmp(p->hMinMax, SC_TOGGLEROLL,
                                (p->is.RLon? o.tb.bmp.unroll: o.tb.bmp.roll));
         } /* endif */
      } /* endif */
      g.cd.frame.pfnwp(p->hwnd, WM_UPDATEFRAME,
                      (MPARAM)(p->is.minMaxAdd? FCF_MINBUTTON: 0), MPVOID);
   } /* endif */
   repaintWindow(p->hTBar);
   p->is.skipuf = 0;
}


//===========================================================================
// Update the state of the window frame controls and of the system menu,
// adding or removing items according to the state of the application
// (exceptions, trial time expiration, disabling) and of the options.
// Parameters --------------------------------------------------------------
// PSTLRFRAME p : WC_FRAME window data.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID setFrameCtrls(PSTLRFRAME p) {
   PSETFRAMECTRLS psfc;
   if (NULL == (psfc = memLkAlloc(sizeof(SETFRAMECTRLS)))) return;
   memset(psfc, 0, sizeof(SETFRAMECTRLS));
//   /*-*/ dbgPrintf3("*setFrameCtrls hMinMax %08x   close:%d\n",
//                   p->hMinMax, p->is.tbbclose);
   // se necessario crea menu minmax e gli aggiunge pulsante
   // close, titlebar hiding e window rolling secondo impostazioni

   if (!o.gen.disabled && !p->hMinMax && p->hSMenu && !p->is.launchpad
//       && o.tb.addMinMax
       && ((o.tb.on && !pd.we.tbar && o.tb.close > 1)
           || (o.tbh.on && !pd.we.tbarhide && o.tb.htbar)
           || (o.roll.on && !pd.we.wroll && o.tb.roll))) {
      p->is.minMaxAdd = 1;
      psfc->bAddMenu = TRUE;      // flag indica che btn min Š temporaneo
      // cannot create an empty MENU so add (and later remove) the minimize btn
      psfc->fcd.cb = sizeof(FRAMECDATA);
      psfc->fcd.flCreateFlags = FCF_MINBUTTON;
      psfc->fcd.hmodResources = 0;
      psfc->fcd.idResources = 0;
      WinCreateFrameControls(p->hwnd, &psfc->fcd, NULL);
      p->hMinMax = _WinHWND(p->hwnd, FID_MINMAX);
   } /* endif */
   // bottone close-> se settaggi modificati -----------------------------
   if (g.is.expired ||           // se smw expired
       o.gen.disabled ||     // o disabled
       !o.tb.on ||           // o disabilitate opzioni generiche tbar
       pd.we.tbar) {           // o eccezione opzioni generiche tbar
      // se warp 3 rimuove eventuale bottone close
   #ifdef STLR_WARP3
      if (p->is.tbbclose) {
         _removeFrameCtrl(((p->is.tbbclose == 1)? p->hSMenu: p->hMinMax),
                          SC_CLOSE);
         p->is.tbbclose = 0;
      } /* endif */
      // se warp 4 cancella se bottone close non in pos di default
   #else
      if (p->is.tbbclose != 2) {
         if (p->is.tbbclose)
            _removeFrameCtrl(((p->is.tbbclose == 1)? p->hSMenu: p->hMinMax),
                            SC_CLOSE);
         // lo reinserisce nella posizione di default se non fakedMinMax
         if (!p->is.minMaxAdd) {
            _insertFrameCtrl(p->hMinMax, &psfc->mi, 0, SC_CLOSE, g.bmpDef.cls);
         } /* endif */
         p->is.tbbclose = 2;
      } else { // ripristina handle bitmap default
         _setFrameCtrlBmp(p->hMinMax, SC_CLOSE, g.bmpDef.cls);
      } /* endif */
   #endif
   // se posizione bottone chiusura cambiato o btn disabilitato
   } else if (o.tb.close != p->is.tbbclose) {
      // se prima != 0 lo rimuove
      if (p->is.tbbclose)
         _removeFrameCtrl(((p->is.tbbclose == 1)? p->hSMenu: p->hMinMax),
                          SC_CLOSE);
      if (o.tb.close) {
         _insertFrameCtrl(((o.tb.close == 1)? p->hSMenu: p->hMinMax),
                       &psfc->mi, ((o.tb.close == 2) ? 0: MIT_END), SC_CLOSE,
                       (o.tb.bmp.close ? o.tb.bmp.close : g.bmpDef.cls));
      } /* endif */
      p->is.tbbclose = o.tb.close;
   // fakedmin senza bottone chiusura (warp 4 -  warp 3 nel caso precedente)
   } else if (psfc->bAddMenu) {
      _insertFrameCtrl(p->hMinMax, &psfc->mi, ((o.tb.close == 2) ? 0: MIT_END),
                       SC_CLOSE,
                       (o.tb.bmp.close ? o.tb.bmp.close : g.bmpDef.cls));
   // nessun cambiamento posizione cambia solo handle
   } else if (o.tb.bmp.close) {
       _setFrameCtrlBmp(p->hMinMax, SC_CLOSE, o.tb.bmp.close);
   } /* endif */

   // titlebar hiding -----------------------------------------------------
   if (g.is.expired ||            // se styler/2 expired
       o.gen.disabled ||      // o disabled
       !o.tbh.on ||           // o disabilitate opzioni tbarhide
       pd.we.tbarhide) {          // o eccezione opzioni tbarhide
      if (p->is.tbbhtbar) {       // eventualmente rimuove btn
         _removeFrameCtrl(p->hMinMax, SC_TOGGLETITLEHIDE);
         p->is.tbbhtbar = 0;
      } /* endif */
      // rimuove voce da submenu system menu
      if (p->hSMenu && p->is.smnhtbar) {
         _removeSysMenuItem(p->hSMenu, SC_TOGGLETITLEHIDE);
         p->is.smnhtbar = 0;
      } /* endif */
   } else {
      if (o.tb.htbar != p->is.tbbhtbar) {
         if (p->is.tbbhtbar) {
            _removeFrameCtrl(p->hMinMax, SC_TOGGLETITLEHIDE);
         } else {
            _insertFrameCtrl(p->hMinMax, &psfc->mi, ((p->is.tbbclose == 2) ? 1: 0),
                    SC_TOGGLETITLEHIDE,
                    (p->is.titleHide? o.tb.bmp.unhtbar : o.tb.bmp.htbar));
         } /* endif */
         p->is.tbbhtbar = o.tb.htbar;
      } /* endif */
      if (o.tbh.sysMn != p->is.smnhtbar) {
         if (p->is.smnhtbar) {
            _removeSysMenuItem(p->hSMenu, SC_TOGGLETITLEHIDE);
         } else {
            if (_getMenuItem(p->hSMenu, SC_SYSMENU, &psfc->mi) &&
                (NULLHANDLE != (psfc->hSubMenu = psfc->mi.hwndSubMenu))) {
               psfc->feature = g.text.tbhide;
               psfc->virtKey = o.tbh.kbd? 0xff & o.tbh.kbdVK: 0;
               psfc->ctrlKey = o.tbh.kbdKC;
               makeSysMenuItemText(psfc);
               _insertSysMenuItem(psfc->hSubMenu, &psfc->mi,
                                  0, SC_TOGGLETITLEHIDE, psfc->buf);
            } /* endif */
         } /* endif */
         p->is.smnhtbar = o.tbh.sysMn;
      } /* endif */
   } /* endif */
   // window rolling ------------------------------------------------------
   // remotion (expiration of trial time, disabling, exceptions)
   if (g.is.expired || o.gen.disabled || !o.roll.on || pd.we.wroll) {
      // if needed remove the frame ctrl button
      if (p->is.tbbroll) {
         _removeFrameCtrl(p->hMinMax, SC_TOGGLEROLL);
         p->is.tbbroll = 0;
      } /* endif */
      // if needed remove the system menu item
      if (p->hSMenu && p->is.smnroll) {
         _removeSysMenuItem(p->hSMenu, SC_TOGGLEROLL);
         p->is.smnroll = 0;
      } /* endif */
   // insertion (enabling the program or state change of the single feature)
   } else {
      if (o.tb.roll != p->is.tbbroll) {
         // if needed remove the frame ctrl button
         if (p->is.tbbroll) {
            _removeFrameCtrl(p->hMinMax, SC_TOGGLEROLL);
         // add the frame ctrl button
         } else {
            _insertFrameCtrl(p->hMinMax, &psfc->mi,
                         ((p->is.tbbclose == 2) ? 1: 0), SC_TOGGLEROLL,
                         (p->is.RLon? o.tb.bmp.unroll: o.tb.bmp.roll));
         } /* endif */
         p->is.tbbroll = o.tb.roll;
      } /* endif */
      // system menu item
      if (o.roll.sysMn != p->is.smnroll) {
         if (p->is.smnroll) {
            _removeSysMenuItem(p->hSMenu, SC_TOGGLEROLL);
         } else {
            if (_getMenuItem(p->hSMenu, SC_SYSMENU, &psfc->mi) &&
                (NULLHANDLE != (psfc->hSubMenu = psfc->mi.hwndSubMenu))) {
               psfc->feature = g.text.winroll;
               psfc->virtKey = o.roll.kbd? 0xff & o.roll.kbdVK: 0;
               psfc->ctrlKey = o.roll.kbdKC;
               makeSysMenuItemText(psfc);
               _insertSysMenuItem(psfc->hSubMenu, &psfc->mi,
                                  0, SC_TOGGLEROLL, psfc->buf);
            } /* endif */
         } /* endif */
         p->is.smnroll = o.roll.sysMn;
      } /* endif */
   } /* endif */
   // if FID_MINMAX was added by Styler/2 remove the minimize button
   if (psfc->bAddMenu) {
      _removeFrameCtrl(p->hMinMax, SC_MINIMIZE);
      g.cd.menu.pfnwp(p->hSMenu, MM_SETITEMATTR,
                      MPFROM2SHORT(SC_MINIMIZE, TRUE),
                      MPFROM2SHORT(MIA_DISABLED, MIA_DISABLED));
   // completely remove FID_MINMAX if no longer needed
   } else if (p->hMinMax && p->is.minMaxAdd) {
      psfc->cStlrItems = (p->is.tbbclose > 1) + p->is.tbbhtbar + p->is.tbbroll;
      psfc->cCurItems = (ULONG)g.cd.menu.pfnwp(p->hMinMax, MM_QUERYITEMCOUNT,
                                               MPVOID, MPVOID);
      // if ther is no longer the system menu and all the remaining items left
      // in FID_MINMAX are Styler/2 items or if FID_MINMAX is empty destroy it
      if ((!p->hSMenu && (psfc->cStlrItems == psfc->cCurItems)) || !psfc->cCurItems) {
         WinDestroyWindow(p->hMinMax);
         p->hMinMax = NULLHANDLE;
         p->is.minMaxAdd = 0;
      } /* endif */
   } /* endif */
   memLkFree(psfc);
//   /*-*/ dbgPrintf4("setFrameCtrls(%08x) hMinMax : %08x minMaxAdd : %d\n",
//                      p->hwnd, p->hMinMax, p->is.minMaxAdd);
}


//===========================================================================
// Build the text for the item to be added to the system menu according to
// the current feature and current accelerator keys and insert the menuitem.
// Parameters --------------------------------------------------------------
// PSETFRAMECTRLS psfc : data used to update the frame controls and sysmenu.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID makeSysMenuItemText(PSETFRAMECTRLS psfc) {
   strcpy(psfc->buf, psfc->feature);
   if (psfc->virtKey) {
      if (psfc->ctrlKey & KC_ALT) strcat(psfc->buf, g.text.alt);
      if (psfc->ctrlKey & KC_CTRL) strcat(psfc->buf, g.text.ctrl);
      if (psfc->ctrlKey & KC_SHIFT) strcat(psfc->buf, g.text.shift);
      switch (psfc->virtKey) {
         case VK_INSERT:    strcat(psfc->buf, g.text.ins);       break;
         case VK_BACKSPACE: strcat(psfc->buf, g.text.backspace); break;
         case VK_DELETE:    strcat(psfc->buf, g.text.del);       break;
         default:
            sprintf(psfc->buf + strlen(psfc->buf),
                    "F%d", psfc->virtKey - VK_F1 + 1);
            break;
      } /* endswitch */
   } /* endif */
}


//===========================================================================
// Change the image used by FID_SYSMENU and FID_MINMAX.
// Parameters --------------------------------------------------------------
// PSTLRFRAME p : WC_FRAME data.
// Return value ------------------------------------------------------------
// VOID.
//===========================================================================
static
VOID setDefFrameCtrlsImage(PSTLRFRAME p) {
   if (p->ffl & FCF_SYSMENU)
      _setFrameCtrlBmp(p->hSMenu, SC_SYSMENU,
                 (o.tb.bmp.sysmenu ? o.tb.bmp.sysmenu : g.bmpDef.sys));
   if (p->ffl & FCF_MINBUTTON)
      _setFrameCtrlBmp(p->hMinMax, SC_MINIMIZE,
                 (o.tb.bmp.min ? o.tb.bmp.min : g.bmpDef.min));
   if (p->ffl & FCF_HIDEBUTTON)
      _setFrameCtrlBmp(p->hMinMax, SC_HIDE,
                 (o.tb.bmp.hide ? o.tb.bmp.hide : g.bmpDef.hide));
   if (p->ffl & FCF_MAXBUTTON)
      _setFrameCtrlBmp(p->hMinMax, SC_MAXIMIZE,
                 (o.tb.bmp.max ? o.tb.bmp.max : g.bmpDef.max));
   if (p->ffl & (FCF_MINMAX | FCF_HIDEBUTTON))
      _setFrameCtrlBmp(p->hMinMax, SC_RESTORE,
                 (o.tb.bmp.rest ? o.tb.bmp.rest : g.bmpDef.rest));
}


//===========================================================================
// Reset the rolling flag of the window to unrolled and if the window-roll
// titlebar button is used updates the image according to the new window
// state.
// Parameters --------------------------------------------------------------
// PSTLRFRAME p : frame windows data.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID setUnrolled(PSTLRFRAME p) {
   p->is.RLon = 0;
   if (p->hMinMax && p->is.tbbroll) {
      p->is.skipuf = 1;
      _setFrameCtrlBmp(p->hMinMax, SC_TOGGLEROLL, o.tb.bmp.roll);
      p->is.skipuf = 0;
   } /* endif */
}


//===========================================================================
// Hide/unhide the title bar.
// Parameters --------------------------------------------------------------
// HWND hwnd  : window handle.
// PSTLRFRAME p : windows data.
// BOOL bShow : TRUE/FALSE (show/hide the titlebar).
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID showTitleBar(HWND hwnd, PSTLRFRAME p, BOOL bShow) {
   SWP swp;
   if (bShow) {
      p->is.tbhilited = (WinQueryWindowUShort(hwnd, QWS_FLAGS) & FF_ACTIVE) > 0;
      p->is.titleHidden = 0;                      // resetta bShow
      WinSetParent(p->hTBar, hwnd, FALSE);        // titlebar
      if (p->hSMenu)  WinSetParent(p->hSMenu, hwnd, FALSE);
      if (p->hMinMax) WinSetParent(p->hMinMax, hwnd, FALSE);
      if (p->is.menuIncluded && p->hMenu) {     // menu
         WinSetParent(p->hMenu, hwnd, FALSE);
         p->is.menuIncluded = 0;
      } // end if
   } else {
      p->is.titleHidden = 1;
      WinSetParent(p->hTBar, HWND_OBJECT, FALSE);
      if (p->hSMenu) WinSetParent(p->hSMenu, HWND_OBJECT, FALSE);
      if (p->hMinMax) WinSetParent(p->hMinMax, HWND_OBJECT, FALSE);
      if (o.tbh.inclMn
          && (NULLHANDLE != (p->hMenu = _WinHWND(hwnd, FID_MENU)))) {
         WinQueryWindowPos(p->hMenu, &swp);
         p->yMenu = swp.y;
         WinSetParent(p->hMenu, HWND_OBJECT, FALSE);
         p->is.menuIncluded = 1;
      } // end if
   } // end if
   g.cd.frame.pfnwp(hwnd, WM_UPDATEFRAME, MPVOID, MPVOID);
   // if a VIO window or a not-resizeable dialog or a folder
   if (p->is.shield ||
       (p->is.dialog &&
        !(WinQueryWindowULong(hwnd, QWL_STYLE) & FS_SIZEBORDER)) ||
       (p->is.folder && o.tbh.redfld)) {
      WinSetWindowPos(hwnd, NULLHANDLE,      // aggiunge o sottrae
                      p->rclCur.x,           // l'altezza della titlebar
                      p->rclCur.y,           // secondo se deve mostrare
                      p->rclCur.cx,          // o nascondere la titlebar
                      (bShow? p->rclCur.cy + g.sys.cyTbar:
                      p->rclCur.cy - g.sys.cyTbar),
                      SWP_NOADJUST | SWP_SIZE);
   } // end if
   // forza repaint
   repaintWindowR(hwnd);
}


//===========================================================================
// Change the roll state of a window.
// Parameters --------------------------------------------------------------
// PSTLRFRAME p : window data.
// BOOL bRoll : TRUE/FALSE set/reset roll mode.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID changeRollState(PSTLRFRAME p, BOOL bRoll) {
   SWP swp;
   HWND hNext;
   HENUM henum;
   // set roll state ------------------------------------------------------
   if (bRoll) {
      p->is.rolled = 1;
      // nuove coordinate y, cy
      if (!o.roll.bottom) p->rclCur.y += p->rclCur.cy - p->cyRoll;
      p->rclCur.cy = p->cyRoll;
      // nasconde la client window
      if (p->hClient) WinShowWindow(p->hClient, FALSE);
      for (henum = WinBeginEnumWindows(p->hwnd), // enumera children
           hNext = WinGetNextWindow(henum),
           p->rwc.chhc = 0, p->rwc.cLc = 0, p->rwc.cRc = 1;
           hNext != NULLHANDLE;
           hNext = WinGetNextWindow(henum)) {
         if (WinIsWindowVisible(hNext)) {          // se finestra visibile
            WinQueryWindowPos(hNext, &swp);
            // modifica flag
            swp.fl = SWP_SIZE | SWP_MOVE | SWP_SHOW;
            // se Š la SWP della barra del titolo lo memorizza come primo
            // elemento di p->rwc.ppRc
            if (hNext == p->hTBar) {
               swp.y = p->ptBrd.y;     // modifica coordinata y
               p->rwc.ppRc[0] = swp;
            // se controllo titlebar memorizza SWP corrispondente
            } else if (swp.y >= p->wrclTitle.y) {
               swp.y = p->ptBrd.y;     // modifica coordinata y
               // se controllo a sinistra titlebar
               if (swp.x < p->wrclTitle.x && p->rwc.cLc < CSWPRLD) {
                  p->rwc.ppLc[p->rwc.cLc++] = swp;
               } else if (p->rwc.cRc < CSWPRLD) {
                  p->rwc.ppRc[p->rwc.cRc++] = swp;
               } /* endif */
            // se non dialogo o controllo sotto barra titolo memorizza hwnd
            } else if (p->rwc.chhc < CH_CTLS &&    // e nasconde controllo
                       (!p->is.dialog || (swp.y <= p->cyRoll))) {
               p->rwc.phhc[p->rwc.chhc] = hNext;
               WinShowWindow(p->rwc.phhc[p->rwc.chhc++], FALSE);
            } // end if
         } // end if
      } // end for
      WinEndEnumWindows(henum);
   } else { // srotolamento -----------------------------------------------
      p->rclCur = p->is.max? p->rclMax: p->rclRst;
      p->is.rolled = 0;
      p->is.resetparent = 1;   // flag resettaggio visibilit… win enumerate
   } // end if
   WinSetWindowPos(p->hwnd, 0, p->rclCur.x, p->rclCur.y,
                   p->rclCur.cx, p->rclCur.cy,
                   (p->is.shield? SWP_SIZE | SWP_MOVE:
                                    SWP_SIZE | SWP_MOVE | SWP_NOADJUST));
}


//===========================================================================
// Change the titlebar hiding state of the window.
// Parameters --------------------------------------------------------------
// HWND hwnd  : window handle.
// PSTLRFRAME p : window data.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID toggleTitleBarHidingCmd(HWND hwnd, PSTLRFRAME p) {
   if (p->is.RLon) {
      WinAlarm(g.hwnd.desktop, WA_WARNING);
      return;
   } /* endif */
   if (p->is.titleHide) { // disattiva title hiding
      p->is.titleHide = 0;
      if (p->is.titleHidden) showTitleBar(hwnd, p, TRUE);
   // altrimenti se non Š rolled e exclude rolled windows attiva title
   } else {    // hiding
      p->is.titleHide = 1;
      showTitleBar(hwnd, p, FALSE);
   } /* endif */
   if (p->hMinMax && p->is.tbbhtbar) {
      p->is.skipuf = 1;
      _setFrameCtrlBmp(p->hMinMax, SC_TOGGLETITLEHIDE,
            (p->is.titleHide? o.tb.bmp.unhtbar : o.tb.bmp.htbar));
      p->is.skipuf = 0;
      // provare a usare WinShowWindow() al posto di WM_UPDATEFRAME?
      g.cd.frame.pfnwp(hwnd, WM_UPDATEFRAME, MPVOID, MPVOID);
   } /* endif */
}


//===========================================================================
// Change the roll activation of the window.
// Parameters --------------------------------------------------------------
// HWND hwnd  : window handle.
// PSTLRFRAME p : window data.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID toggleRollCmd(HWND hwnd, PSTLRFRAME p) {
   // se finestra in auto-roll state non Š rollata e il tasto
   // maiuscolo Š premuto viene ri-rollata
   if (p->is.RLon && o.roll.unrollOnFocus &&
       o.roll.rollOnUnfocus && !p->is.rolled &&
       WinGetKeyState(HWND_DESKTOP, VK_SHIFT) & 0x8000) {
       changeRollState(p, TRUE);
       return;
   } /* endif */
   p->is.RLon = !p->is.RLon;
   // se si sta arrotolando finestra con titlebar hiding attivo
   if (p->is.RLon && p->is.titleHide) {
      p->is.titleHide = 0;     // lo disattiva
      // se barra titolo nascosta (rolling tramite accel) la mostra
      if (p->is.titleHidden) showTitleBar(p->hwnd, p, TRUE);
      // ripristina icona titlebar hiding
      if (p->hMinMax && p->is.tbbhtbar) {
         p->is.skipuf = 1;
         _setFrameCtrlBmp(p->hMinMax, SC_TOGGLETITLEHIDE,
                          o.tb.bmp.htbar);
         p->is.skipuf = 0;
      } /* endif */
   } /* endif */
   if (p->is.RLon != p->is.rolled) changeRollState(p, p->is.RLon);
   // cambia icona roll/unroll
   if (p->hMinMax && p->is.tbbroll) {
      p->is.skipuf = 1;
      _setFrameCtrlBmp(p->hMinMax, SC_TOGGLEROLL,
                  (p->is.RLon? o.tb.bmp.unroll : o.tb.bmp.roll));
      p->is.skipuf = 0;
      WinShowWindow(p->hMinMax, TRUE);
   } /* endif */
}


//===========================================================================
// Roll/unroll all windows. When rolling all windows, not-already-rolled
// windows are flagged so that on the next unroll-all-windows command
// they are not unrolled.
// Parameters --------------------------------------------------------------
// HWND hwnd  : window handle.
// PSTLRFRAME p : window data.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID rollUnrollAllCmd(HWND hwnd, PSTLRFRAME p) {
   // se finestra principale, opzioni rolling attive e non eccezioni
   if (p->is.deskChild && o.roll.on && !pd.we.wroll
       && (hwnd != g.hwnd.deskFldr)) {
      // rolling all windows :
      if (g.is.rollall) {
         // if the window is visible and not yet rolled
         if (isWinInScreen(p) && WinIsWindowVisible(hwnd) && !p->is.rolled) {
            // se rolling status attivo ri-rolla la finestra
            if (p->is.RLon) {
               _stlrRunFrameProc(hwnd, STLR_AUTOROLL, p);
            // altrimenti setta la finestra in rolling state
            } else {
               toggleRollCmd(hwnd, p);
               // this flag allows to tell which window was not already
               // rolled and must be unrolled on the next
               // 'unroll all windows' command
               p->is.allrolled = 1;
            } /* endif */
         } /* endif */
      // unrolling all windows :
      } else {
         if (p->is.allrolled && p->is.rolled) {
            toggleRollCmd(hwnd, p);
            p->is.allrolled = 0;
         } /* endif */
      } /* endif */
   } /* endif */
}



//===========================================================================
// Return TRUE if the window rectangle intersect the screen.
// Parameters --------------------------------------------------------------
// PSTLRFRAME p : window data.
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (window intersecting/not intersecting the screen).
//===========================================================================
_Inline
BOOL isWinInScreen(PSTLRFRAME p) {
   LONG dcx = g.sys.scr.cx / 2 - (p->rclCur.x + p->rclCur.cx / 2);
   LONG dcy = g.sys.scr.cy / 2 - (p->rclCur.y + p->rclCur.cy / 2);
   LONG maxdcx = (g.sys.scr.cx + p->rclCur.cx) / 2;
   LONG maxdcy = (g.sys.scr.cy + p->rclCur.cy) / 2;
   return (dcx < maxdcx) && (dcx > -maxdcx) && (dcy < maxdcy) && (dcy > -maxdcy);
}


//===========================================================================
// According to the subcommand change the window Z-order or activation state.
// Parameters --------------------------------------------------------------
// HWND hwnd    : window handle.
// PSTLRFRAME p : window data.
// ULONG subCmd : subcommand.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID changeZorderOrActiveStateCmd(HWND hwnd, PSTLRFRAME p, ULONG subCmd) {
   switch (subCmd) {
      // push the window back (one position or to the bottom)
      case SW2_SFMOVETOBOTTOM:
         WinSetWindowPos(hwnd,
             (o.sizpos.Zone? WinQueryWindow(hwnd, QW_NEXTTOP): HWND_BOTTOM),
              0, 0, 0, 0, SWP_DEACTIVATE | SWP_ZORDER);
         break;
      // put the window on the top
      case SW2_SFMOVETOTOP:
         _frameActivate(hwnd);
         break;
      // activate the window optionally without changing the Z-order
      default:   // STLR_SFDEFAULT
         if (isMouseOverWindow(hwnd)) {
            HWND hPrev = WinQueryWindow(hwnd, QW_PREV);
            WinSetWindowPos((p->is.disabled?
                               WinQueryWindow(hwnd, QW_NEXTTOP): hwnd),
                            (o.mou.focusNoTop && hPrev &&
                               !g.is.prvwinos2 && !p->is.winos2?
                                  hPrev: HWND_TOP),
                            0, 0, 0, 0, SWP_ZORDER | SWP_ACTIVATE);
         } /* endif */
         break;
   } // end switch
}


//===========================================================================
// Return TRUE if the mouse is over a window (desktop child).
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (mouse over/outside window rectanglesuccess/error)
//===========================================================================
_Inline
BOOL isMouseOverWindow(HWND hwnd) {
   POINTL ptl;
   WinQueryPointerPos(g.hwnd.desktop, &ptl);
   return hwnd == WinWindowFromPoint(g.hwnd.desktop, &ptl, FALSE);
}


//===========================================================================
// Unhide the titlebar.
// Parameters --------------------------------------------------------------
// HWND hwnd  : window handle.
// PSTLRFRAME p : window data.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID unhideTitleCmd(HWND hwnd, PSTLRFRAME p) {
   if (isMouseInRect(hwnd, 0, p->rclCur.cy - p->ptBrd.y - 1,
                     p->rclCur.cx, p->rclCur.cy))
      showTitleBar(hwnd, p, TRUE);
}


//===========================================================================
// Hide the titlebar.
// Parameters --------------------------------------------------------------
// HWND hwnd  : window handle.
// PSTLRFRAME p : window data.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID hideTitleCmd(HWND hwnd, PSTLRFRAME p) {
   if (!isMouseInRect(hwnd,
                   0, (o.tbh.inclMn && p->yMenu?  p->yMenu: p->wrclTitle.y),
                   p->rclCur.cx, p->rclCur.cy))
      showTitleBar(hwnd, p, FALSE);
}


//===========================================================================
// Auto-unroll a window when it gets the focus or gets resized vertically.
// Parameters --------------------------------------------------------------
// HWND hwnd  : window handle.
// PSTLRFRAME p : window data.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID autoUnrollCmd(HWND hwnd, PSTLRFRAME p) {
   changeRollState(p, FALSE);
   if (!o.roll.rollOnUnfocus) {
      p->is.RLon = 0;
      if (p->hMinMax && p->is.tbbroll) {
         p->is.skipuf = 1;
         _setFrameCtrlBmp(p->hMinMax, SC_TOGGLEROLL, o.tb.bmp.roll);
         p->is.skipuf = 0;
         g.cd.frame.pfnwp(hwnd, WM_UPDATEFRAME, MPVOID, MPVOID);
      } /* endif */
   } /* endif */
}


//===========================================================================
// Execute the operation assigned to a given Windows key.
// Parameters --------------------------------------------------------------
// ULONG action : action to be executed.
// HWND hwnd    : focus window detecting the keyboard event.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID processWindowsKeys(ULONG action, HWND hwnd) {
   switch (action) {
      case IWKEY_HIDEWIN:
         g.is.hideall = !g.is.hideall;
         _stlrMsgPostAll(STLR_HIDEALL, 0);
         break;
      case IWKEY_ROLLWIN:
         g.is.rollall = !g.is.rollall;
         _stlrMsgPostAll(STLR_ROLLALL, 0);
         // se arrotolamento sospende autofocus trasferendo il focus al desktop
         if (g.is.rollall) {
            g.hwnd.lastCreated = g.hwnd.deskFldr;
            WinSetWindowPos(g.hwnd.deskFldr, 0, 0, 0, 0, 0, SWP_ACTIVATE);
         } /* endif */
         break;
      case IWKEY_WINLIST:
         // mostra nasconde la window list
         if (WinQueryWindowULong(g.hwnd.winList, QWL_STYLE) & WS_VISIBLE) {
            WinShowWindow(g.hwnd.winList, FALSE);
         } else {
            WRECTL wrcl;
            WinQueryWindowRect(g.hwnd.winList, (PRECTL)&wrcl);
            WinQueryPointerPos(g.hwnd.desktop, (PPOINTL)&wrcl);
            wrcl.x -= wrcl.cx / 2;
            wrcl.y -= wrcl.cy / 2;
            if (wrcl.x < 0)
               wrcl.x = 0;
            else if ((wrcl.x + wrcl.cx) > g.sys.scr.cx)
               wrcl.x = g.sys.scr.cx - wrcl.cx;
            if (wrcl.y < 0)
               wrcl.y = 0;
            else if ((wrcl.y + wrcl.cy) > g.sys.scr.cy)
               wrcl.y = g.sys.scr.cy - wrcl.cy;
            WinSetWindowPos(g.hwnd.winList, 0, wrcl.x, wrcl.y, 0, 0, SWP_MOVE);
            WinSetWindowPos(g.hwnd.winList, HWND_TOP, 0, 0, 0, 0,
                            SWP_SHOW | SWP_ACTIVATE | SWP_ZORDER);
         } /* endif */
         break;
      case IWKEY_CTXTMENU:
         WinPostMsg(hwnd, WM_CONTEXTMENU, MPVOID, (MPARAM)0x00010000);
         break;
      case IWKEY_LOCKUP:
         WinPostMsg(g.hwnd.deskFldr, WM_COMMAND, (MPARAM)0x2c1, (MPARAM)2);
         break;
      case IWKEY_FLDDRIVE:
         g.is.winkfldr = 1;
         stlrOpenObject("<WP_DRIVES>");
         break;
      case IWKEY_FLDSYSSETUP:
         g.is.winkfldr = 1;
         stlrOpenObject("<WP_CONFIG>");
         break;
      case IWKEY_OS2CMD:
         g.is.winkshld = 1;
         stlrOpenObject("<WP_OS2WIN>");
         break;
      // defer warp center commands
      default:
         _stlrMsgPost(g.hwnd.deskFldr, STLR_WARPCENTERCMD, action);
         break;
   } /* endswitch */
}


//===========================================================================
// Open the WPS object 'pszObject'.
// Parameters --------------------------------------------------------------
// PSZ pszObject : object ID or object full path name.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID stlrOpenObject(PSZ pszObject) {
   WinOpenObject(WinQueryObject(pszObject), 0, TRUE);
}


//===========================================================================
// Execute a warp center operation simulating mouse events on the warp
// center window.
// Parameters --------------------------------------------------------------
// ULONG action : action to be performed.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID warpCenterCmd(ULONG action) {
   HWND hwnd = WinWindowFromID(g.hwnd.desktop, 0x555);
   HWND hprvfocus = WinQueryActiveWindow(g.hwnd.desktop);
   CHAR buf[4];
   if (WinQueryClassName(hwnd, 4, buf) && !memcmp(buf, "#3", 3)) {
      RECTL rcl;
      ULONG ul1;
      PSZ pbuf;
      WinQueryWindowRect(hwnd, &rcl);
      switch (action) {
         case IWKEY_WCDESKMENU:
            ul1 = 0x00020002;
            break;
         case IWKEY_WCWINLIST:
            ul1 = rcl.yTop > 22? 0x0002003c: 0x00020034;
            break;
         case IWKEY_WCPROCLIST:
            if (!(pbuf = memLkAlloc(256))) return;
            g.is.wcbtn = 1;  // forza riposizionamento menu warpcenter
            ul1 = rcl.yTop > 22? 0x0002003c: 0x00020034;// ul2 = 0x00100000;
            WinSetKeyboardStateTable(HWND_DESKTOP, pbuf, FALSE);
            pbuf[VK_CTRL] |= 0x80;
            WinSetKeyboardStateTable(HWND_DESKTOP, pbuf, TRUE);
            WinSendMsg(hwnd, WM_BUTTON1DOWN, (MPARAM)ul1, (MPARAM)0x00100000);
            WinSendMsg(hwnd, WM_BUTTON1UP, (MPARAM)ul1, (MPARAM)0x00100000);
            WinSetKeyboardStateTable(HWND_DESKTOP, pbuf, FALSE);
            pbuf[VK_CTRL] = 0;
            WinSetKeyboardStateTable(HWND_DESKTOP, pbuf, TRUE);
            memLkFree(pbuf);
            return;
         case IWKEY_WCTRAYLIST:
            ul1 = rcl.yTop > 22? 0x0002017d: 0x00020141;
            break;
         case IWKEY_WCNEXTTRAY:
            ul1 = 0x00020000 + rcl.xRight - (rcl.yTop > 22? 122: 118);
            break;
         case IWKEY_WCINFOMENU:
            ul1 = 0x00020000 + rcl.xRight - 106;
            break;
         default:
            return;
      } /* endswitch */
      WinPostMsg(hwnd, WM_BUTTON1DOWN, (MPARAM)ul1, MPVOID);
      WinPostMsg(hwnd, WM_BUTTON1UP, (MPARAM)ul1, MPVOID);
      // se cambiamento tray restituisce focus a finestra precedente
      if (action == IWKEY_WCNEXTTRAY)
         _stlrMsgPost(hprvfocus, STLR_ZORDACTIV, 0);
   } /* endif */

}



//===========================================================================
// Hide all windows.
// Parameters --------------------------------------------------------------
// HWND hwnd  : window handle.
// PSTLRFRAME p : window data.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID hideAllWindowsCmd(HWND hwnd, PSTLRFRAME p) {
   // if this is a desktop child and not the WPS folder WP_DESKTOP
   if (p->is.deskChild && (hwnd != g.hwnd.deskFldr)) {
      // hide the window
      if (g.is.hideall) {
         // se la finestra Š visibile nello schermo corrente la nasconde
         if (isWinInScreen(p) && WinIsWindowVisible(hwnd)) {
//            WinShowWindow(hwnd, FALSE);
            WinSetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_MINIMIZE);
            if (p->is.max) p->is.remax = 1;
            p->is.allhidden = 1;
         } /* endif */
      // unhide the window
      } else {
         if (p->is.allhidden) {
            WinSetWindowPos(hwnd, 0, 0, 0, 0, 0,
                            (p->is.remax ? SWP_MAXIMIZE : SWP_RESTORE));
//            WinShowWindow(hwnd, TRUE);
            p->is.allhidden = 0;
         } /* endif */
      } /* endif */
   } /* endif */
}


//===========================================================================
// Get the class of the client window to check if it is a VIO window or
// a Win-OS/2 window.
// Parameters --------------------------------------------------------------
// HWND hwnd   : window handle.
// PSTLRFRAME p  : window data.
// ULONG idWin : frame window id.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID checkClientWinClassCmd(HWND hwnd, PSTLRFRAME p, ULONG idWin) {
   if ((p->hClient = WinWindowFromID(hwnd, FID_CLIENT)) != 0) {
      if (WinIsWindowOfClass(p->hClient, "Shield")) {
      // ripristina la procedura di default per le finestre DOS avviate
      // da una sessione winOS2 e per le sesssioni OS2 a schermo intero
         if ((!p->rclCur.cx && (idWin == 3)) || (idWin == 10)) {
            WinSetWindowPtr(hwnd, g.cd.frame.cbData, (PVOID)0L);
            memLkFree(p);
         } else {
            p->is.shield = 1;
            dbgPrintf2("%08x is a command window\n", hwnd);
            superclassVioWinCmd(hwnd, p);
         } /* endif */
      } else if (WinIsWindowOfClass(p->hClient, "SeamlessClass")) {
         p->is.winos2 = 1;
         // ignore this window regarding the autofocus feature
         if (!(p->ffl & FCF_SYSMENU) && (!_winOwner(hwnd)))
            p->is.winos2skip = 1;
      } /* endif */
   } /* endif */
}


//===========================================================================
// Set data to move the pointer to the focus window.
// Parameters --------------------------------------------------------------
// HWND hwnd  : window handle.
// PSTLRFRAME p : window data.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID moveMousePtrCmd(HWND hwnd, PSTLRFRAME p) {
   HEV hev = g.mou.hev;
   ULONG ulPost;
   HWND hwndtarget;        // finestra al cui centro sar… spostato il mouse
   HWND hNext;
   HENUM henum;
   QMSG qmsg;
   if (o.mou.moveToFocus && !pd.we.mouse && p->is.deskChild &&
       ((g.mou.hwndNew == hwnd) || (_winStyle(hwnd) & WS_VISIBLE)) &&
       // non c'Š alcuna subopzione (opz solo dialogo o solo nuove finestre)
       (!(o.mou.moveToNew || o.mou.moveToDlg) ||
       // nuova finestra e opzione solo nuove finestre
        ((g.mou.hwndNew == hwnd) && o.mou.moveToNew) ||
       // dialogo e opzione solo su dialogo
        (p->is.dialog && o.mou.moveToDlg)) &&
      // se il mouse non Š gi… sulla finestra lo sposta
       !isMouseInRect(hwnd, 0, 0, p->rclCur.cx, p->rclCur.cy)) {

      hwndtarget = WinQueryWindowULong(hwnd, QWL_DEFBUTTON);
      // se Š un dialogo, manca il bottone di default ed Š opzione solo dialoghi
      // e ignora dialoghi senza bottoni, controlla presenza bottoni
      if (p->is.dialog && !hwndtarget && o.mou.moveToDlg && o.mou.moveNotBtnles) {
         henum = WinBeginEnumWindows(hwnd);
         while (NULLHANDLE != (hNext = WinGetNextWindow(henum))) {
            if (DLGC_BUTTON & _getCtrlType(hNext)) break;
         } /* endwhile */
         if (!hNext) return;
      } // end if
      if (!hwndtarget) hwndtarget = hwnd;
      // serializza accesso alla struttura
      while (__lxchg(&g.mou.iSem, 1)) {
         DosSleep(1);
         while (WinPeekMsg(p->hab, &qmsg, hwnd, 0, 0, PM_REMOVE))
            WinDispatchMsg(p->hab, &qmsg);
      } /* endwhile */
      // apre il semaforo per sbloccare il server thread
      if (DosOpenEventSem(NULL, &hev)) goto end;
      if (DosQueryEventSem(hev, &ulPost)) goto end1;
      // se il server thread Š gi… occupato lo blocca
      if (ulPost && DosResetEventSem(hev, &ulPost)) goto end1;
   //   if (g.mou.hwndNew == p->hwnd) g.mou.hwndNew = NULLHANDLE;
      WinQueryWindowRect(hwndtarget, (PRECTL)&g.mou.wr);
      if (!g.is.mousebtn) stlrStopHitTestCheck();
      g.is.mousebtn |= MOVEMOUPTR;
      // ricava posizione finale rimappando punto centrale finestra move-to
      g.mou.wr.x = g.mou.wr.cx / 2;
      g.mou.wr.y = g.mou.wr.cy / 2;
      WinMapWindowPoints(hwndtarget, HWND_DESKTOP, (PPOINTL)&g.mou.wr, 1);
      //// ricava posizione corrente mouse
      WinQueryPointerPos(HWND_DESKTOP, ((PPOINTL)&g.mou.wr) + 1);
      // ricava incremento (* 2^16)
      g.mou.dp.x = ((g.mou.wr.x - g.mou.wr.cx) << 16) / o.mou.moveSteps;
      g.mou.dp.y = ((g.mou.wr.y - g.mou.wr.cy) << 16) / o.mou.moveSteps;
      // equalizza posizione corrente moltiplicandola * 10000 (questa verr…
      // incrementata di (g.mou.wr.dp.x, g.mou.wr.dp.y) ad ogni WM_TIMER
      g.mou.ptc.x = g.mou.wr.cx;
      g.mou.ptc.y = g.mou.wr.cy;
      g.mou.wr.cx <<= 16;
      g.mou.wr.cy <<= 16;
   //   g.mmfw.covr = 0;
      // sblocca il thread di spostamento del mouse
      DosPostEventSem(hev);
   end1:
      DosCloseEventSem(hev);
   end:
      __lxchg(&g.mou.iSem, 0);
   } /* endif */
}


//===========================================================================
// Interrupt window tracking (sizing/moving).
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID stlrStopTracking(HWND hwnd) {
   WinPostMsg(hwnd, WM_CHAR, (MPARAM)0x01011004, (MPARAM)0x0000011b);
   WinPostMsg(hwnd, WM_CHAR, (MPARAM)0x01011044, (MPARAM)0x0040011b);
   WinPostMsg(hwnd, WM_BUTTON1UP, MPVOID, MPVOID);
   WinPostMsg(hwnd, WM_BUTTON2UP, MPVOID, MPVOID);
}




/***************************************************************************/
// win32 (odin) windows extensions


//===========================================================================
// Superclass win32 (odin) frame windows to implement some Styler/2 features.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// PSTLRFRAME p : window data.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID superclassWin32frame(HWND hwnd, PSTLRFRAME p) {
   if (!o.gen.disabled) {                   // abilitazione
      p->pffrm = WinSubclassWindow(hwnd, stlrWin32FrameProc);
   } else if (p->pffrm) {                   // disabilitazione
      WinSetWindowPtr(hwnd, QWP_PFNWP, (PVOID)p->pffrm);
   } /* endif */
}


//===========================================================================
// Superclassed win32 (odin) frame window procedure.
// Parameters --------------------------------------------------------------
// Ordinary window procedure parameters.
// Return value ------------------------------------------------------------
// MRESULT
//===========================================================================
static
MRESULT EXPENTRY stlrWin32FrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   PSTLRFRAME p;
   if (!_getFrameData(hwnd, p)) return (MRESULT)FALSE;
   if (msg == WM_STYLER2) {
      if ((ULONG)mp1 == STLR_EDITSYSMENU)
         return (MRESULT)TRUE;
   } else if (msg == WM_UPDATEFRAME) {
//      if (!p->hMinMax) initWin32FrameData(hwnd, FALSE);
      return (MRESULT)g.cd.frame.pfnwp(hwnd, msg, mp1, mp2);
   } else if (msg == WM_MOUSEMOVE) {
      SHORT x = SHORT1FROMMP(mp1);
      SHORT y = SHORT1FROMMP(mp1);
      // disable maximized window sizing (if the option is set)
      if (o.maxwin.nosize && p->is.max && !pd.we.maxwin &&
          (((x >= 0) && (x < p->ptBrd.x)) ||
           ((x >= (p->rclCur.cx - p->ptBrd.x)) && (x < p->rclCur.cx)) ||
           ((y >= 0) && (y < p->ptBrd.y)) ||
           ((y >= (p->rclCur.cy - p->ptBrd.y)) && (y < p->rclCur.cy))))
         return WinDefWindowProc(hwnd, msg, mp1, mp2);
      // disable vertical sizing of rolled windows (if the option is set)
//      if (!o.roll.letVsize && p->is.rolled &&
//          (((y >= 0) && (y < p->ptBrd.y)) ||
//           ((y >= (p->rclCur.cy - p->ptBrd.y)) && (y < p->rclCur.cy))))
//         return WinDefWindowProc(hwnd, msg, mp1, mp2);
   } else if (msg == WM_WINDOWPOSCHANGED) {
      SWP swp;
      INT delta;
/*-*/dbgPrintf2("W32FRAMECTRLS : WM_WINDOWPOSCHANGED - flags : %08x\n",
                ((PSWP)mp1)->fl);
      WinQueryWindowPos(hwnd, &swp);
/*-*/dbgPrintf5("   frame window: (%d,%d)(%d,%d)\n",
                swp.x, swp.y, swp.cx, swp.cy);

      if (((PSWP)mp1)->fl & (SWP_SIZE | SWP_MOVE)) {
         p->pffrm(hwnd, msg, mp1, mp2);
         if (NULLHANDLE != (p->hMinMax = WinWindowFromID(hwnd, FID_MINMAX))) {
            WinQueryWindowPos(p->hMinMax, &swp);
            if (0 != (delta = ((((PSWP)mp1)->cx - (swp.x + swp.cx))
                              - (((PSWP)mp1)->cy - (swp.y + swp.cy))))) {
               WinSetWindowPos(p->hTBar, 0, 0, 0,
                               p->wrclTitle.cx + delta, p->wrclTitle.cy,
                               SWP_SIZE);
               WinSetWindowPos(p->hMinMax, 0, swp.x + delta, swp.y, 0, 0, SWP_MOVE);
            } /* endif */
         } /* endif */
         return (MRESULT)FALSE;
      } /* endif */
///*-*/dbgPrintf2("W32FRAMECTRLS : WM_WINDOWPOSCHANGED - hMinMax:%08x\n",
//                p->hMinMax);
///*-*/dbgPrintf6("   menuminmax(%d,%d)(%d,%d) delta: %d\n",
//                swp.x, swp.y, swp.cx, swp.cy,
//                ((r.xRight - (swp.x + swp.cx)) - (r.yTop - (swp.y + swp.cy))));
///*-*/dbgPrintf3("   frame window: (%d,%d)\n", r.xRight, r.yTop);
///*-*/dbgPrintf5("   title window: (%d,%d)(%d,%d)\n",
//                p->wrclTitle.x, p->wrclTitle.y, p->wrclTitle.cx, p->wrclTitle.cy);
   } else if (msg == o.sizpos.frameMoumsg) {
      if (o.sizpos.framedrg &&
          (SHORT2FROMMP(mp2) == o.sizpos.frameMoukbd) &&
          !o.gen.disabled && !p->is.min && !pd.we.sizemove) {
         RECTL r;
         WinQueryWindowRect(hwnd, &r);
//         /*-*/dbgPrintf3("trackframe rectangle : %d, %d\n", r.xRight, r.yTop);
         if ((MOUSEX(mp1) >= 0) && (MOUSEX(mp1) < r.xRight) &&
             (MOUSEY(mp1) >= 0) && (MOUSEY(mp1) < r.yTop) &&
              ((MOUSEX(mp1) < 5) || (MOUSEX(mp1) > (r.xRight - 5)) ||
               (MOUSEY(mp1) < 5) || (MOUSEY(mp1) > (r.yTop - 5)))) {
            _setSysMouPtr(SPTR_MOVE);
//            /*-*/ dbgPrintf1("trackframe confirmed\n");
            return (MRESULT)WinSendMsg(hwnd, WM_TRACKFRAME, (MPARAM)TF_MOVE, MPVOID);
         } /* endif */
      } /* endif */
   } /* endif */
   return (MRESULT)p->pffrm(hwnd, msg, mp1, mp2);
}


//===========================================================================
// Complete the initialization of the win32 (odin) windows.
// Parameters --------------------------------------------------------------
// HWND hwnd      : window handle.
// BOOL initFrame : TRUE/FALSE (init frame data/minmax data).
// Return value ------------------------------------------------------------
// PSTLRFRAME : address of alloacated frame data.
//===========================================================================
static
PSTLRFRAME initWin32FrameData(HWND hwnd) {
   PSTLRFRAME p;
   SWP swp;
   // initialization of frame data (sytem menu and minmax are created later)
   // allocate memory to store the frame data
   if (NULL == (p = memLkAlloc(sizeof(STLRFRAME)))) return NULL;
   memset((PVOID)p, 0, sizeof(STLRFRAME));
   // inizializza vari dati nella struttura dati finestra
   p->hab = WinQueryAnchorBlock(hwnd);  // hab - eventualmente cancellare
   p->hwnd = hwnd;                      // hwnd - eventualmente cancellare
   p->is.win32 = 1;                     // a win32 (odin) frame window
   // memorizza pointer nelle window words
   WinSetWindowPtr(hwnd, g.cd.frame.cbData, (PVOID)p);
#ifndef STLR_WARP3
   p->is.tbbclose = 2;
#endif
   // se opzione Smooth horizontal movements Š attiva setta FS_NOBYTEALIGN
   if (o.sizpos.noalign && !pd.we.sizemove)
      WinSetWindowBits(hwnd, QWL_STYLE, FS_NOBYTEALIGN, FS_NOBYTEALIGN);
   // controlla se Š finestra principale
   if (_winParent(hwnd) == g.hwnd.desktop) {
      p->is.newwin = 1;
      p->is.deskChild = 1;
      g.mou.hwndNew = hwnd;     // õ MouseMoveToFocus
   } /* endif */
   // store the window position and size
   if (WinQueryWindowPos(hwnd, &swp)) {
      p->rclCur.x = swp.x;
      p->rclCur.cx = swp.cx;
      p->rclCur.y = swp.y;
      p->rclCur.cy = swp.cy;
      p->rclRst = p->rclCur;
   } /* endif */
   // superclass the frame procedure
   if (!o.gen.disabled) superclassWin32frame(hwnd, p);
   /*-*/ dbgPrintf3("win32 frame data initialized (%08x - %08x)\n", hwnd, p);
   return p;
}


//===========================================================================
//
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// VOID
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
// VOID
//===========================================================================
static
VOID setWin32FrameCtrlsImage(PSTLRFRAME p) {
   if (_isFrameCtrlValid(p->hMinMax, SC_CLOSE))
      _setFrameCtrlBmp(p->hMinMax, SC_CLOSE,
                       (o.tb.bmp.close ? o.tb.bmp.close : g.bmpDef.cls));
   if (_isFrameCtrlValid(p->hMinMax, SC_MINIMIZE))
      _setFrameCtrlBmp(p->hMinMax, SC_MINIMIZE,
                       (o.tb.bmp.min ? o.tb.bmp.min : g.bmpDef.min));
   if (_isFrameCtrlValid(p->hMinMax, SC_HIDE))
      _setFrameCtrlBmp(p->hMinMax, SC_HIDE,
                       (o.tb.bmp.hide ? o.tb.bmp.hide : g.bmpDef.hide));
   if (_isFrameCtrlValid(p->hMinMax, SC_MAXIMIZE))
      _setFrameCtrlBmp(p->hMinMax, SC_MAXIMIZE,
                       (o.tb.bmp.max ? o.tb.bmp.max : g.bmpDef.max));
   if (_isFrameCtrlValid(p->hMinMax, SC_RESTORE))
      _setFrameCtrlBmp(p->hMinMax, SC_RESTORE,
                       (o.tb.bmp.rest ? o.tb.bmp.rest : g.bmpDef.rest));
   WinSetWindowPos(p->hwnd, 0, 0, 0, 0, 0, SWP_MINIMIZE);
   WinSetWindowPos(p->hwnd, 0, 0, 0, 0, 0, SWP_RESTORE);
}


//===========================================================================
// Check if the current frame window is a Win32 (Odin) window.
// Parameters --------------------------------------------------------------
// HAB hab          : anchor block handle.
// PSZ pszClassName : class name.
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================
_Inline
BOOL isWin32FrameWindow(PSZ pszClassName) {
   CLASSINFO ci;
   if (WinQueryClassInfo(NULLHANDLE, pszClassName, &ci)) {
      return ci.flClassStyle == CS_FRAME;
   } /* endif */
   return TRUE; // something is wrong with this window
}


/***************************************************************************/
// estensioni "wpFolder window"


/*
 Superclass/unsuperclass the WPS folder windows.
called just after window creation or on disabling/enabling styler/2
 Parameters --------------------------------------------------------------
 HWND hwnd  : window handle.
 PSTLRFRAME p : window data.
 Return value ------------------------------------------------------------
 VOID
*/
static
VOID superclassWPSFoldersCmd(HWND hwnd, PSTLRFRAME p) {
   if (o.wpskbd.on && !o.gen.disabled)
   {
      p->pffrm = WinSubclassWindow(hwnd, stlrWpsFolderProc);
   }
   else if (p->pffrm)
   {
      WinSetWindowPtr(hwnd, QWP_PFNWP, (PVOID)p->pffrm);
      p->pffrm = NULL;
   } /* endif */
}


//===========================================================================
// New window procedure for the WPS folders.
// Parameters --------------------------------------------------------------
// Ordinary window procedure parameters.
// Return value ------------------------------------------------------------
// MRESULT
//===========================================================================
static
MRESULT EXPENTRY stlrWpsFolderProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   PSTLRFRAME p;
   if (!_getFrameData(hwnd, p)) return (MRESULT)FALSE;

   // rileva inizio/fine editazione titolo oggetto --------------------------
   if (msg == WM_CONTROL) {
      if (SHORT2FROMMP(mp1) == CN_BEGINEDIT)
         p->is.fldredit = 1;
      else if (SHORT2FROMMP(mp1) == CN_ENDEDIT)
         p->is.fldredit = 0;
   // acceleratori tastiera: ------------------------------------------------
   // ctrl-e ordina per estensione, F5 aggiorna F6 ordina,
   // backspace apre parent
   } else if (msg == WM_TRANSLATEACCEL) {
      // se Š seleziona/copia tramite tastiera
      if (!p->is.fldredit) {
         ULONG mpscmd = 0;
         // prima processa accelleratori default (arrotolamento/srotolamento
         // e eliminazione titlebar sono effettuati dalla procedura frame)
         if (p->pffrm(hwnd, msg, mp1, mp2)) return (MRESULT)TRUE;
         // non processa il rilascio dei tasti
         if (PCHMFROMMP(mp1)->fs & KC_KEYUP) return (MRESULT)FALSE;
         // shift-ins: crea file con contenuto clipboard solo warp 4
         if ((PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == KC_SHIFT &&
             (PCHMFROMMP(mp1)->fs & KC_VIRTUALKEY) &&
             (PCHMFROMMP(mp1)->vkey) == VK_INSERT) {
            mpscmd = 0x2cb;
         // apertura propriet… folder
         } else if (o.wpskbd.sets &&
              (PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.wpskbd.setsKC &&
              (PCHMFROMMP(mp1)->vkey) == (o.wpskbd.setsK & 0xff)) {
            mpscmd = 0x70;
         // acceleratore open parent
         } else if (o.wpskbd.parent &&
              (PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.wpskbd.parentKC &&
              (PCHMFROMMP(mp1)->vkey) == (o.wpskbd.parentK & 0xff)) {
            mpscmd = 0x2ca;
         // acceleratore find
         } else if (o.wpskbd.find &&
              (PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.wpskbd.findKC &&
              (PCHMFROMMP(mp1)->vkey) == (o.wpskbd.findK & 0xff)) {
            mpscmd = 0x8;
         // acceleratore sort by name
         } else if (o.wpskbd.srtname &&
              (PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.wpskbd.srtnameKC &&
              (PCHMFROMMP(mp1)->vkey) == (o.wpskbd.srtnameK & 0xff)) {
            mpscmd = 0x1770;
         // acceleratore sort by extension (tramite NPS WPS enhancer)
         } else if (o.wpskbd.srtext &&
              (PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.wpskbd.srtextKC &&
              (PCHMFROMMP(mp1)->vkey) == (o.wpskbd.srtextK & 0xff)) {
            mpscmd = 0x9d87;
         // acceleratore sort by type
         } else if (o.wpskbd.srttype &&
              (PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.wpskbd.srttypeKC &&
              (PCHMFROMMP(mp1)->vkey) == (o.wpskbd.srttypeK & 0xff)) {
            mpscmd = 0x1771;
         // acceleratore sort by real name
         } else if (o.wpskbd.srtrname &&
              (PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.wpskbd.srtrnameKC &&
              (PCHMFROMMP(mp1)->vkey) == (o.wpskbd.srtrnameK & 0xff)) {
            mpscmd = 0x1777;
         // acceleratore sort by size
         } else if (o.wpskbd.srtsize &&
              (PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.wpskbd.srtsizeKC &&
              (PCHMFROMMP(mp1)->vkey) == (o.wpskbd.srtsizeK & 0xff)) {
            mpscmd = 0x1778;
         // acceleratore sort by last write date
         } else if (o.wpskbd.srtwrite &&
              (PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.wpskbd.srtwriteKC &&
              (PCHMFROMMP(mp1)->vkey) == (o.wpskbd.srtwriteK & 0xff)) {
            mpscmd = 0x1779;
         // acceleratore sort by last access date
         } else if (o.wpskbd.srtacc &&
              (PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.wpskbd.srtaccKC &&
              (PCHMFROMMP(mp1)->vkey) == (o.wpskbd.srtaccK & 0xff)) {
            mpscmd = 0x177b;
         // acceleratore sort by last create date
         } else if (o.wpskbd.srtcreat &&
              (PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.wpskbd.srtcreatKC &&
              (PCHMFROMMP(mp1)->vkey) == (o.wpskbd.srtcreatK & 0xff)) {
            mpscmd = 0x177d;
         // acceleratore arrange
         } else if (o.wpskbd.arrange &&
              (PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.wpskbd.arrangeKC &&
              (PCHMFROMMP(mp1)->vkey) == (o.wpskbd.arrangeK & 0xff)) {
            mpscmd = 0x135;
         // acceleratore refresh
         } else if (o.wpskbd.refresh &&
              (PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.wpskbd.refreshKC &&
              (PCHMFROMMP(mp1)->vkey) == (o.wpskbd.refreshK & 0xff)) {
            mpscmd = 0x1f7;
         } /* endif */
         if (mpscmd) {
            ((PQMSG)mp1)->msg = WM_COMMAND;
            ((PQMSG)mp1)->hwnd = hwnd;
            ((PQMSG)mp1)->mp1 = (MPARAM)mpscmd;
            ((PQMSG)mp1)->mp2 = (MPARAM)2;
            return (MRESULT)TRUE;
         } /* endif */
         return (MRESULT)FALSE;
      } /* endif */
   } /* endif */
   return (MRESULT)p->pffrm(hwnd, msg, mp1, mp2);
}


/***************************************************************************/
// command line window extensions


//===========================================================================
// Superclass VIO windows to perform the clipboard operations via the mouse
// or the keyboard. If original window procedure address is already set
// un-superclass the window, otherwise superclass it.
// Parameters --------------------------------------------------------------
// HWND hwnd  : window handle.
// PSTLRFRAME p : window data.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID superclassVioWinCmd(HWND hwnd, PSTLRFRAME p) {
   if (!o.gen.disabled) {                   // abilitazione
      p->pffrm = WinSubclassWindow(hwnd, stlrVioFrameProc);
      p->pfclnt = WinSubclassWindow(p->hClient, stlrVioClientProc);
   } else if (p->pffrm) {                   // disabilitazione
      WinSetWindowPtr(p->hwnd, QWP_PFNWP, (PVOID)p->pffrm);
      WinSetWindowPtr(p->hClient, QWP_PFNWP, (PVOID)p->pfclnt);
   } /* endif */
}


//===========================================================================
// shield.c: procedura subclassata finestre di classe shield per
// implementazione esecuzione operazioni clipboard tramite mouse
// Le finestre VIO richiedono un particolare trattamento in quanto:
// -1) la flag KC_VIRTUALKEY non viene mai settata:
// -2) le virtual key possono essere riconosciute solo tramite il relativo
//     scancode
// -3) in questa implementazione lo scancode della key corrispondente al
//     settaggio prescelto per l'arrotolamento e srotolamento delle finestre
//     o all'eliminazione della barra del titolo, viene shiftato di 8 bit
//     a sinistra e memorizzato insieme al codice della virtualkey
//     corrispondente
// -4) dato che la procedura di default delle vio windows rileva lo stato
//     del tasto shift, eliminando gli ultimi 2 caratteri se questo Š
//     premuto, nel caso l'operazione di paste avvenga tramite shift,
//     l'operazione stessa viene posticipata al rilascio del tasto shift
//     stesso
// (*) modifica procedura copia tramite tastiera:
//     la combinazione di tasti scelta per la copia indica solo inizio
//     marking la copia vera e propria viene effettuata quando, terminata la
//     selezione del testo, verr… rilasciato il tasto shift.
//     a) se flag inizio marking non attiva invia syscommand a se stesso
//     b) quando syscommand viene rilevato:
//        se flag == 0 viene incrementata
//        se flag == 1 (cancellazione marking) viene resettata a 0
//     c) quando shift viene premuto se flag inizio marking == 1
//        la incrementa
//     d) quando shift viene rilasciato se flag inizio marking = 2 copia
//        testo selezionato ed in ogni caso resetta flag
// (*) la rilevazione del rilascio del tasto shift deve essere fatta nella
//     procedura dell'hook della finestra perch‚ il messaggio non raggiunge
//     la procedura della frame della finestra comandi
//===========================================================================



#define PTRKINF           ((PTRACKINFO)mp2)

//===========================================================================
// Enhanced VIO window WC_FRAME procedure.
// Parameters --------------------------------------------------------------
// Ordinary window procedure parameters.
// Return value ------------------------------------------------------------
// MRESULT
//===========================================================================
static
MRESULT EXPENTRY stlrVioFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   PSTLRFRAME p;
   if (!_getFrameData(hwnd, p)) return (MRESULT)FALSE;
   // -----------------------------------------------------------------------
   // spostamento-ridimensionamento finestre VIO
   if (msg == WM_TRACKFRAME) {
      MRESULT mr;
      WinSetHook(p->hab, HMQ_CURRENT, HK_INPUT, (PFN)vioTrackHook, 0);
      mr = stlrFrameProc(hwnd, msg, mp1, mp2);
      // se premuto shift simula click in tracking di default
      // per salvare la posizione e dimensione della finestra
      if (WinGetKeyState(g.hwnd.desktop, VK_SHIFT) & 0x8000) {
         WinPostMsg(hwnd, WM_BUTTON1DOWN, MPVOID, MPVOID);
         WinPostMsg(hwnd, WM_BUTTON1UP, MPVOID, MPVOID);
         p->pffrm(hwnd, msg, mp1, mp2);
      } /* endif */
      WinReleaseHook(p->hab, HMQ_CURRENT, HK_INPUT, (PFN)vioTrackHook, 0);
      return mr;
   // se minimizzazione, massimizzazione o ripristiono, srotola sempre la finestra
   } else if (msg == WM_MINMAXFRAME) {
      if (p->is.rolled) {
         changeRollState(p, FALSE);
         if (!o.roll.unrollOnFocus || !o.roll.rollOnUnfocus)
            setUnrolled(p); // setta RLon a 0, cambia bmp bottone
      } /* endif */
   // se il messaggio Š chiamato in seguito ad un ridimensionamento causato
   // dal comando MODE resetta la dimensione della finestra alla massima
   } else if (msg == WM_ADJUSTWINDOWPOS) {
      if (!g.is.tracking && !p->is.rolled &&
          (((PSWP)mp1)->fl & SWP_SIZE) && p->is.viocx) {
         ((PSWP)mp1)->cx = p->is.viocx;
         p->is.viocx = 0;
      } else if (g.is.winkshld) {
         g.is.winkshld = 0;
         _stlrMsgPost(hwnd, STLR_ZORDACTIV, SW2_SFMOVETOTOP);
      } /* endif */
   } else if (msg == WM_QUERYTRACKINFO) {
      p->pffrm(hwnd, msg, mp1, mp2);
      // elimina la scrollbar orizontale
      if (o.vio.noscrl && !g.is.tracking && !p->is.rolled &&
          (PTRKINF->rclTrack.xRight - PTRKINF->rclTrack.xLeft) <
          PTRKINF->ptlMaxTrackSize.x)
         p->is.viocx = PTRKINF->ptlMaxTrackSize.x;
      return (MRESULT)TRUE;
   // ---------------------- operazioni clipboard ---------------------------
   // rileva inizio/cancellazione selezione ---------------------------------
   } else if (msg == WM_SYSCOMMAND) {
      if ((USHORT)mp1 == SC_VIOMARK) {
         // se Š selezione/copia tramite tastiera
         if (p->is.viomark) {
            // se inizio selezione setta flag
            if (p->is.viomark == 1) p->is.viomark++;
            // se cancellazione operazione selezione resetta flag
            else if (p->is.viomark > 1) p->is.viomark = 0;
         } /* endif */
   #ifdef STLR_WARP3
      // se copia cancella la coppia di caratteri \r\n che warp 3 aggiunge
      } else if ((USHORT)mp1 == SC_VIOCOPY) {
         PSZ psz;
         p->pffrm(hwnd, msg, mp1, mp2);  // esegue procedura di default
         // ottiene contenuto originario cipboard
         if (NULL != (psz = getTextFromClipboard(p->hab))) {
            PSZ pmem;
            ULONG cb = strlen(psz) - 2;
            // rimuove coppia "\r\n" se presente
            if (!memcmp(psz + cb, "\r\n", 2) &&
                !DosAllocSharedMem((PPVOID)&pmem, NULL, cb + 1, FLCLIPMEM)) {
               *(psz + cb) = 0;
              strcpy(pmem, psz);
              // modifica il contenuto della clipboard
              txt2clipvio(p->hab, pmem);
            } /* endif */
            memLkFree(psz);
         } /* endif */
         return (MRESULT)FALSE;
   #endif
      } else {
         return (MRESULT)p->pffrm(hwnd, msg, mp1, mp2);
      } /* endif */
   // implementazione operazioni clipboard tramite tastiera -----------------
   // insertmode e accelleratori per window rolling e titlebar hiding
   } else if (msg == WM_TRANSLATEACCEL) {
      // chiama procedura default
      ULONG mpscmd = 0;
      if (p->pffrm(hwnd, msg, mp1, mp2)) return (MRESULT)TRUE;
      // controlla abilitazione/disabilitazione INSERT MODE (Alt + Ins)
      if (TRANSACCMP1(mp1) == 0x68010024) {
         if (o.vio.ins) {
            p->is.vioins = !p->is.vioins;
            p->pfclnt(hwnd, WM_CHAR, (MPARAM)0x68010004, (MPARAM)0x00a052e0);
            p->pfclnt(hwnd, WM_CHAR, (MPARAM)0x68010144, (MPARAM)0x00e052e0);
         } /* endif */
      // se Š uno dei tasti win 95 lo processa e restituisce TRUE
      } else if (TRANSACCMP1(mp1) == MP1LEFTWINKEY) {
         if (o.winkbd.left) {
            ((PQMSG)mp1)->mp1 = (MPARAM)STLR_PROCWKL;
            goto wkeyprocessed;
         } /* endif */
      } else if (TRANSACCMP1(mp1) == MP1RIGHTWINKEY) {
         if (o.winkbd.right) {
            ((PQMSG)mp1)->mp1 = (MPARAM)STLR_PROCWKR;
            goto wkeyprocessed;
         } /* endif */
      } else if (TRANSACCMP1(mp1) == MP1MENUKEY) {
         if (o.winkbd.menu) {
            ((PQMSG)mp1)->mp1 = (MPARAM)STLR_PROCWKMENU;
            goto wkeyprocessed;
         } /* endif */
      // se non eccezioni finestre VIO e opzione operaz. clipbrd. tastiera
      } else if (o.vio.kbd) {
         // se si preme shift ed Š gi… in modalit… selezione
         if ((p->is.viomark == 2) &&
             ((TRANSACCMP1(mp1) == 0x2a01000c) ||
              (TRANSACCMP1(mp1) == 0x3601000c))) {
            p->is.viomark++;
            return (MRESULT)FALSE;
         // se rilascio shift in modalit… selezione: copia testo selezionato
         } else if ((p->is.viomark == 3) &&
              ((TRANSACCMP1(mp1) == 0x2a010044) ||
               (TRANSACCMP1(mp1) == 0x36010044))) {
            p->is.viomark = 0;
            mpscmd = SC_VIOCOPY;
         // se rilascio tasto shift implica operazione paste la esegue
         } else if (PCHMFROMMP(mp1)->fs & KC_KEYUP) {
            return (MRESULT)FALSE;
         // se evento inizio operazione selezione testo
         } else if ((PCHMFROMMP(mp1)->scancode == (o.vio.cpyKbdVK >> 8)) &&
                    ((PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.vio.cpyKbdKC)) {
            // se operazione marcatura gi… in corso restituisce FALSE
            if (p->is.viomark) return FALSE;
            // altrimenti inizializza flag e resetta id messaggio
            p->is.viomark = 1;
            mpscmd = SC_VIOMARK;
         // se evento copia tutto il contenuto della VIO window
         } else if (PCHMFROMMP(mp1)->scancode == (o.vio.cpyaKbdVK >> 8) &&
                    ((PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.vio.cpyaKbdKC)) {
            mpscmd = SC_VIOCPYALL;
         // incolla contenuto clipboard
         } else if ((PCHMFROMMP(mp1)->scancode == (o.vio.insKbdVK >> 8)) &&
                    ((PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.vio.insKbdKC)) {
            // se paste tramite shift posticipa a quando shift viene rilasciato
            if (PCHMFROMMP(mp1)->fs & KC_SHIFT) {
               memset(mp1, 0, 16);
               vioPaste(p);
               return (MRESULT)TRUE;
            } else {
               mpscmd = SC_VIOPASTE;
            } /* endif */
         } /* endif */
         if (mpscmd) goto postscmd;
      } /* endif */
      // se evento arrotolamento/srotolamento finestre
      if (PCHMFROMMP(mp1)->fs & KC_KEYUP) return (MRESULT)FALSE;
      if ((PCHMFROMMP(mp1)->scancode == (o.roll.kbdVK >> 8)) &&
          ((PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.roll.kbdKC)) {
         if (!pd.we.wroll && o.roll.on && o.roll.kbd)
            mpscmd = SC_TOGGLEROLL;
      // se evento eliminazione titlebar
      } else if ((PCHMFROMMP(mp1)->scancode == (o.tbh.kbdVK >> 8)) &&
                 ((PCHMFROMMP(mp1)->fs & KC_BASICFLAGS) == o.tbh.kbdKC)) {
         if (!pd.we.tbarhide && o.tbh.on && o.tbh.kbd)
            mpscmd = SC_TOGGLETITLEHIDE;
      } /* endif */
      if (!mpscmd) return (MRESULT)FALSE;
postscmd:
      ((PQMSG)mp1)->msg = WM_SYSCOMMAND;
      ((PQMSG)mp1)->hwnd = hwnd;
      ((PQMSG)mp1)->mp1 = (MPARAM)mpscmd;
      ((PQMSG)mp1)->mp2 = (MPARAM)2;
      return (MRESULT)TRUE;
   } /* endif */
   return (MRESULT)p->pffrm(hwnd, msg, mp1, mp2);
wkeyprocessed:
   ((PQMSG)mp1)->msg = WM_STYLER2;
   ((PQMSG)mp1)->mp2 = (MPARAM)((PQMSG)mp1)->hwnd;
   ((PQMSG)mp1)->hwnd = hwnd;
   return (MRESULT)TRUE;
}


//===========================================================================
// nuova procedura vio windows: client window
// V. 1.0.2.31: la procedura viene modificata per permettere selezione e
// copia solo con un tasto del mouse senza che ci• interferisca con altre
// operazioni come click normale:
// Nota: - la differenza tra i messaggi WM_BUTTONxDOWN, WM_BUTTONxUP,
//         WM_BUTTONxMOTIONSTART Š costante indipendentemente dal bottone
//         del mouse ed Š:
//         WM_BUTTONxUP - WM_BUTTONxDOWN = 1
//         WM_BUTTONxMOTIONSTART - WM_BUTTONxDOWN = 0x3a0
//       - la posizione del mouse riportata in mp1 nel messaggio
//         WM_BUTTONxMOTIONSTART corrisponde alla posizione verificatasi
//         in corrispondenza al relativo WM_BUTTONxDOWN
// -1) alla rilevazione del WM_BUTTONxMOTIONSTART corrispondente al bottone
//     mouse prescelto, setta flag inizio selezione, posta a stesso
//     messaggio corrispondente WM_BUTTONxUP, WM_SYSCOMMAND alla frame e
//     infine WM_BUTTON1DOWN a se stessa
// -2) nella procedura dell'hook, quando rileva messaggio WM_BUTTONxUP
//     corrispondente al bottone del mouse prescelto, se flag inizio
//     selezione setta flag copia, se flag copia trasforma messaggio
//     in WM_BUTTON1UP, posta SYSCOMMAND per copiare selezione nella
//     clipboard e resetta flag
//===========================================================================
static
MRESULT EXPENTRY stlrVioClientProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   PSTLRFRAME p;
   if (!_getFrameData(_winParent(hwnd), p)) return(MRESULT)FALSE;
   // õ Make insert mode the default ------------------------------------
   // if one of the 'Enter' keys has been released post keyboard events
   // to restore the insert mode
   if ((msg == WM_CHAR) && o.vio.ins && p->is.vioins &&
       ((((ULONG)mp1 & 0xffff00ff) == 0x1c010044) ||
        (((ULONG)mp1 & 0xffff00ff) == 0x5a010044))) {
      p->pfclnt(hwnd, WM_CHAR, (MPARAM)0x68010004, (MPARAM)0x00a052e0);
      p->pfclnt(hwnd, WM_CHAR, (MPARAM)0x68010144, (MPARAM)0x00e052e0);
      return (MRESULT)p->pfclnt(hwnd, msg, mp1, mp2);
   } /* endif */
   // õ Enable mouse copy/paste -----------------------------------------
   // modificare quando si user… per implementare altre funzionalit…
   if (o.vio.mou) {
      // WM_BUTTONxMOTIONSTART corrispondente a tasto prescelto ed
      // eventuale combinazione tastiera
      if ((msg == (o.vio.cpyMoumsg + 0x3a0)) &&
          (SHORT2FROMMP(mp2) == o.vio.cpyMoukbd)) {
         g.is.viomsel = 1;  // flag inizio selezione
         //*****++++ PROVARE A CAMBIARE MP1 E MP2 INVECE DI USARE LA FLAG
         WinPostMsg(hwnd, o.vio.cpyMoumsg + 1, mp1, (MPARAM)0xffffffff);
         WinPostMsg(p->hwnd, WM_SYSCOMMAND, (MPARAM)SC_VIOMARK, (MPARAM)2);
         WinPostMsg(hwnd, WM_BUTTON1DOWN, mp1, MPVOID);
         WinSetHook(p->hab, HMQ_CURRENT, HK_INPUT, (PFN)vioClipHook, 0);
         return (MRESULT)TRUE;
      // se rileva evento corrispondente a copy all
      } else if ((msg == o.vio.cpyaMoumsg) &&
                 (SHORT2FROMMP(mp2) == o.vio.cpyaMoukbd)) {
         WinPostMsg(p->hwnd, WM_SYSCOMMAND, (MPARAM)SC_VIOCPYALL, (MPARAM)2);
         return (MRESULT)TRUE;
      // se rileva evento corrispondente a paste:
      } else if ((msg == o.vio.insMoumsg) &&
                 (SHORT2FROMMP(mp2) == o.vio.insMoukbd)) {
         // se premuto shift aggiunge 2 caratteri alla clipboard per evitare
         // cancellazione degli ultimi 2 e poi ripristina testo originario
         if (o.vio.insMoukbd & KC_SHIFT) {
            vioPaste(p);
         // altrimenti trasmette semplicemente messaggio di paste
         } else {
            p->pffrm(p->hwnd, WM_SYSCOMMAND, (MPARAM)SC_VIOPASTE, (MPARAM)2);
         } /* endif */
         return (MRESULT)TRUE;
      } /* endif */
   } /* endif */
   return (MRESULT)p->pfclnt(hwnd, msg, mp1, mp2);
}


//===========================================================================
// procedura di hook usata dalle finestre VIO per alcune operazioni della
// clipboard
// Parameters --------------------------------------------------------------
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================
static
BOOL EXPENTRY vioClipHook(HAB hab, PQMSG pqmsg, USHORT fs) {
   // se messaggio corrispondente a rilascio bottone mouse che ha dato
   // inizio ad operazione di mark-copy scarica il primo msg
   if (pqmsg->msg == (o.vio.cpyMoumsg + 1)
       && ((ULONG)pqmsg->mp2 != 0xffffffff)) {
//      // first occurrence is message posted in stlrVioClientProc()
//      // before setting the hook
//      if (g.is.viomsel) {
//         g.is.viomcpy = 1;
//         g.is.viomsel = 0;
//         return FALSE;
//      } else if (g.is.viomcpy) {
//         g.is.viomcpy = 0;
         // trasforma messaggio in motion end
         WinPostMsg(pqmsg->hwnd, WM_BUTTON1UP, pqmsg->mp1, pqmsg->mp2);
         WinPostMsg(pqmsg->hwnd, WM_ENDSELECT, pqmsg->mp1, pqmsg->mp2);
         WinPostMsg(WinWindowFromID(_winParent(pqmsg->hwnd), FID_SYSMENU),
                    WM_SYSCOMMAND, (MPARAM)SC_VIOCOPY, (MPARAM)2);
         g.is.mousebtn = 0;
         WinReleaseHook(WinQueryAnchorBlock(pqmsg->hwnd),
                        HMQ_CURRENT, HK_INPUT, (PFN)vioClipHook, 0);
         return FALSE;
//      } /* endif */
   } /* endif */
   return FALSE;
}


//===========================================================================
// procedura di hook usata dalle finestre VIO durante lo spostamento/
// ridimensionamento per tradurre i codici di scansione dei tasti nelle
// corrispetive virtual key
// Parameters --------------------------------------------------------------
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================

BOOL EXPENTRY vioTrackHook(HAB hab, PQMSG pqmsg, USHORT fs) {
   if (pqmsg->msg == WM_CHAR) {
      switch (((ULONG)pqmsg->mp1 & 0xff000040)) {
         case 0x1c000000:
            pqmsg->mp2 = MPFROM2SHORT((USHORT)pqmsg->mp2, VK_NEWLINE);
            break;
         case 0x5a000000:
            pqmsg->mp2 = MPFROM2SHORT((USHORT)pqmsg->mp2, VK_ENTER);
            break;
         case 0x01000000:
            pqmsg->mp2 = MPFROM2SHORT((USHORT)pqmsg->mp2, VK_ESC);
            break;
         case 0x63000000:
         case 0x4b000000:
            pqmsg->mp2 = MPFROM2SHORT((USHORT)pqmsg->mp2, VK_LEFT);
            break;
         case 0x61000000:
         case 0x48000000:
            pqmsg->mp2 = MPFROM2SHORT((USHORT)pqmsg->mp2, VK_UP);
            break;
         case 0x64000000:
         case 0x4d000000:
            pqmsg->mp2 = MPFROM2SHORT((USHORT)pqmsg->mp2, VK_RIGHT);
            break;
         case 0x66000000:
         case 0x50000000:
            pqmsg->mp2 = MPFROM2SHORT((USHORT)pqmsg->mp2, VK_DOWN);
            break;
         default:
            pqmsg->msg = WM_NULL;
            break;
      } /* endswitch */
   } /* endif */
   return FALSE;
}


//===========================================================================
// inserisce il testo nelle finestre comandi os2 e dos quando l'inserimento
// Š comandato da un evento che include il tasto shift.
// Per evitare che gli ultimi 2 caratteri contenuti nella clipboard vengano
// tagliati, si aggiungono 2 spazi al contenuto della clipboard, si inserisce
// il contenuto tramite SC_VIOPASTE, si ripristina il contenuto originario
// Nota:
// praticamente Š sufficiente che il buffer della memoria condivisa sia di
// 2 byte maggiore della diemensione della stringa da inserire. Dopo aver
// modificato il contenuto della clipboard con questo buffer "maggiorato"
// non c'Š alcun bisogno di ripristinare il contenuto originale!
// Parameters --------------------------------------------------------------
// PSTLRFRAME p : windows data.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID vioPaste(PSTLRFRAME p) {
//   /*-*/ dbgPrintf1("-------------- VIO PASTE --------------\n");
   PSZ psz = getTextFromClipboard(p->hab);  // ottiene contenuto originario cipboard
   if (psz) {
      PSZ pmem;
      ULONG cb = strlen(psz) + 1;
//      /*-*/ dbgPrintf3("   clipboard text : %.40s (%d) bytes\n", psz, cb);
      // aggiunge 2 spazi
      if (!DosAllocSharedMem((PPVOID)&pmem, NULL, cb + 2, FLCLIPMEM)) {
         strcpy(pmem, psz);
//         /*-*/ dbgPrintf1("   shared memory allocated\n");
         // modifica il contenuto della clipboard
         txt2clipvio(p->hab, pmem);
//         /*-*/ dbgPrintf1("   clipboard content updated\n");
         // chiama procedura default per incollare il testo
         p->pffrm(p->hwnd, WM_SYSCOMMAND, (MPARAM)SC_VIOPASTE, (MPARAM)2);
      } /* endif */
      memLkFree(psz);
   } /* endif */
}


//===========================================================================
// inserisce il testo psz nella clipboard senza vuotarne il contenuto
// Š usato per evitare che le finestre VIO cancellino gli ultimi 2
// caratteri nelle operaioni di paste in cui si usa il tasto shift
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// VOID
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID txt2clipvio(HAB hab, PSZ psz) {
   if (WinOpenClipbrd(hab)) {
      WinSetClipbrdData(hab, (ULONG)psz, CF_TEXT, CFI_POINTER);
      WinCloseClipbrd(hab);
   } /* endif */
}

