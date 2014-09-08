//===========================================================================
// main.c : dll entry point, initialization, termination procedures.
//===========================================================================


#pragma strings(readonly)

// includes -----------------------------------------------------------------

#include "stlrMain.h"
#include "stlrError.h"


// prototypes ---------------------------------------------------------------

ULONG _System stlrInitTerm(ULONG hmod, ULONG flag);
BOOL initialize(ULONG hmod);
VOID cacheSysVals(VOID);
BOOL readProfile(VOID);
VOID setDefaultValues(VOID);
VOID checkExceptions(VOID);
BOOL getDefFrameCtlsBmpHnds(VOID);
BOOL superclassMainPMclasses(VOID);
BOOL initWorkerThreads(VOID);
BOOL initHooks(HMODULE hmod);
ULONG terminate(VOID);

// global variables ---------------------------------------------------------

GLOBAL g;
OPTIONS o;

#ifdef DEBUGGING
   HFILE hf;  /*-*/
#endif

// defines ------------------------------------------------------------------


// ------------------------------ entry point -------------------------------
#pragma entry(stlrInitTerm)

//===========================================================================
// DLL initialization/termination
// Parameters --------------------------------------------------------------
// ULONG hmod : DLL module handle.
// ULONG flag : 0/1 (initialization/termination)
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================

ULONG _System stlrInitTerm(ULONG hmod, ULONG flag) {
   switch (flag) {
      case 0:
         return initialize(hmod);
      case 1:
         return terminate();
      default:
         return 0L;
   } /* endswitch */
   return 1L;
}


//===========================================================================
// DLL initialization routine.
// Parameters --------------------------------------------------------------
// ULONG hmod : current DLL module handle
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================
static
BOOL initialize(ULONG hmod) {

   // initialize the version informations
   _stlrSetCurrentVersion(&g.ver);

   /*-*/ dbgInit();

   // abort if the DLL is already loaded
   if (g.gpd.WPS0) return FALSE;

   // get and cache some system values (this must come before readProfile() !)
   cacheSysVals();

   // abort DLL loading if one of the Ctrl keys is down
   if (stlrKbdSkipLoading()) return FALSE;

   // read the initialization file
   if (!readProfile()) return FALSE;

   // get the default frame controls bitmap handles
   if (!getDefFrameCtlsBmpHnds()) return FALSE;

   // superclasses various PM window classes
   if (!superclassMainPMclasses()) return FALSE;

   // create the worker threads
   if (!initWorkerThreads()) return FALSE;

   // set the input and send message hooks
   if (!initHooks(hmod)) return FALSE;

   o.gen.loaded = 1;  // usato da seconda dll per controllare caricamento
                      // smartwin.dll
   return 1;
}


//===========================================================================
// Cache some system values.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID cacheSysVals(VOID) {
   // ricava id drive di boot
   g.sys.drv = 'A' + sysInfo(QSV_BOOT_DRIVE) - 1;
   // versione sistema operativo (version minor)
   g.sys.ver = sysInfo(QSV_VERSION_MINOR);
   // raccolta informazioni dati globali
   g.sys.xAlign = _sysValue(SV_CXBYTEALIGN);
   g.sys.yAlign = _sysValue(SV_CYBYTEALIGN);
   g.sys.scr.cx = _sysValue(SV_CXSCREEN);
   g.sys.scr.cy = _sysValue(SV_CYSCREEN);
   // rileva presenza mouse 3 bottoni
   g.is.mou3btns = (_sysValue(SV_CMOUSEBUTTONS) == 3);
   // ottiene dimensioni di alcuni controlli
   g.sys.cyTbar = _sysValue(SV_CYTITLEBAR);
   g.sys.cyHScroll = _sysValue(SV_CYHSCROLL);
   g.sys.cxVScroll = _sysValue(SV_CXVSCROLL);
   // giorni trascorsi dall'1/1/70
   g.is.trap = sysInfo(QSV_TIME_LOW) / 86400;
}


//===========================================================================
// Abort loading the DLL if one of the 2 Ctrl keys is pressed.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (abort/go on loading the DLL)
//===========================================================================

BOOL stlrKbdSkipLoading(VOID) {
   if ((WinGetPhysKeyState(HWND_DESKTOP, 0x1d) & 0x8000) ||
       (WinGetPhysKeyState(HWND_DESKTOP, 0x5b) & 0x8000)) {
      DosBeep(1000, 200);
      return TRUE;
   } /* endif */
   return FALSE;
}


//===========================================================================
// Reads SMARTWIN.INI data. Initialize variables to safe default values
// if the file is not found.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================
static
BOOL readProfile(VOID) {
   HINI hini = NULLHANDLE;
   ULONG ul;
   ULONG ulDef = 0;
   APIRET rc;

   if (!(hini = stlrOpenProfile()))
      return _stlrLoadError(IDERR_INIREAD, FALSE);

   // set the profile version number
   if (!PrfQueryProfileInt(hini, SZPRO_OPTIONS, SZPRO_VERSION, 0)) {
      ul = CURINIVER;
      PrfWriteProfileData(hini, SZPRO_OPTIONS, SZPRO_VERSION, &ul, sizeof(ul));
   } /* endif */

   // se errore lettura file ini (perch‚ mancante)
   ul = sizeof(OPTIONS);
   if (!PrfQueryProfileData(hini, SZPRO_OPTIONS, SZPRO_GLOBAL,
                            (PVOID)&o, &ul)) {

      // titlebar settaggi generali --------------------------------------
   #ifdef STLR_WARP3
      PrfQueryProfileString(HINI_USER, "PM_SystemFonts", "WindowTitles",
                            "10.System Proportional.Bold",
                            (PVOID)o.tb.achFont, CB_FONTNAME);
      strcpy(o.dlg.achFont, "8.Helv");
   #else
      PrfQueryProfileString(HINI_USER, "PM_SystemFonts", "WindowTitles",
                            "9.WarpSans Bold",
                            (PVOID)o.tb.achFont, CB_FONTNAME);
      strcpy(o.dlg.achFont, "9.WarpSans");
   #endif
      o.dlg.cbFont = strlen(o.dlg.achFont) + 1;

      ul = sizeof(ULONG);
      setDefaultValues();
      // se legge settaggi opzioni avvio e questo Š TRUE
      if (!PrfQueryProfileData(HINI_USER, OS2INIAPPL,
                               "SETSTARTOPTIONS", (PVOID)&ulDef, &ul)
          ||
          !ulDef)
         o.gen.disabled = 1;
   } // end if
   // get from the resources DLL the bitmaps and the text strings
   initResources(hini, TRUE);
   PrfCloseProfile(hini);
   // set the message events to be ignored to trap following events used
   // for styler/2 features (i.e. if button 3 click is used then button 3
   // down must not be passed to the default procedure)
   o.txt.skipcpy = (USHORT)stlrMouEventToSkip(o.txt.cpyMoumsg);
   o.txt.skipcut = (USHORT)stlrMouEventToSkip(o.txt.cutMoumsg);
   o.txt.skippaste = (USHORT)stlrMouEventToSkip(o.txt.insMoumsg);
   o.txt.skipovwrt = (USHORT)stlrMouEventToSkip(o.txt.ovwMoumsg);
   // check the presence of exceptions
   checkExceptions();
   return TRUE;
}


//===========================================================================
// Get the full name and path of the initialization file, open it and return
// the initialization file handle.
// Note : this procedure is exported.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// HINI : NULLHANDLE in case of error.
//===========================================================================

HINI stlrOpenProfile(VOID) {
   CHAR achIniPath[260];         // path file inizializzazione
   ULONG ul;                     // lunghezza stringa letta
   if (!(ul = PrfQueryProfileString(HINI_USER, OS2INIAPPL, "path",
                                    NULL, achIniPath, 260)))
      return NULLHANDLE;
   strcpy(&achIniPath[--ul], "\\SMARTWIN.INI");
   return PrfOpenProfile(NULLHANDLE, achIniPath);
}


//===========================================================================
// Init the option values to the defaults.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID setDefaultValues(VOID) {
   // i settaggi non menzionati sono settati a 0
   // general page options --------------------------------------------
   o.gen.applyoe = 1;
   o.gen.saveoe = 1;
   // dialog window options -------------------------------------------
#ifdef STLR_WARP3
   o.dlg.cbFont = 7;
   memcpy(o.dlg.achFont, "8.Helv", 7);
#else
   o.dlg.cbFont = 11;
   memcpy(o.dlg.achFont, "9.WarpSans", 11);
#endif
   // titlebar options ------------------------------------------------
   o.tb.on = 1;
   o.tb.close = 2;
   o.tb.ovrPP = 1;
   o.tb.menu = 1;
   // active titlebar options
   o.tb.a.bkgnd = TBARBKGNDBMP;
   o.tb.a.border = 1;
   o.tb.a.shade = 1;
   o.tb.a._3DTxt = 1;
   o.tb.a.clrLeftTop.l = RGB_DARKBLUE;    // 0x00000080
   o.tb.a.clrRghtBtm.l = RGB_GRAY;        // 0xcccccc;
   o.tb.a.clrShBkgnd.l = RGB_WHITE;
   o.tb.a.clrFgTxt.l = RGB_WHITE;
   // inactive titlebar options
   o.tb.i.bkgnd = TBARBKGNDBMP;
   o.tb.i.border = 1;
   o.tb.i.shade = 1;
   o.tb.i._3DTxt = 1;
   o.tb.i.clrLeftTop.l = RGB_GRAY;
   o.tb.i.clrRghtBtm.l = RGB_DARKGRAY;
   o.tb.i.clrHiBkgnd.l = RGB_WHITE;
   o.tb.i.clrFgTxt.l = RGB_DARKGRAY;
   o.tb.i.clrBgTxt.l = RGB_WHITE;
   // titlebar hiding options -----------------------------------------
   o.tbh.on = 1;
   o.tbh.sysMn = 0;
   o.tbh.inclMn = 1;
   o.tbh.unhideDelay = 600;
   o.tbh.hideDelay = 900;
   o.tbh.mou = 1;
   o.tbh.moumsg = WM_BUTTON1DBLCLK;
   o.tbh.kbdKC = KC_ALT;
   o.tbh.kbdVK = VK_F2 | 0x3c00;
   // window rolling options ------------------------------------------
   o.roll.on = 1;
   o.roll.unrollOnFocus = 1;
   o.roll.rollOnUnfocus = 1;
   o.roll.letVsize = 1;
   o.roll.unrollOnVsize = 1;
   o.roll.unrollOnClose = 1;
   o.roll.mou = 1;
   o.roll.moumsg = g.is.mou3btns?
                      WM_BUTTON3CLICK: WM_BUTTON2DBLCLK;
   o.roll.kbdKC = KC_ALT;
   o.roll.kbdVK = VK_F1 | 0x3b00;
   o.tb.roll = 1;
   // maximized windows options ---------------------------------------
   o.maxwin.x = -(SHORT)_sysValue(SV_CXSIZEBORDER);
   o.maxwin.y = -(SHORT)_sysValue(SV_CYSIZEBORDER);
   o.maxwin.cx = g.sys.scr.cx + 2 * (-o.maxwin.x);
   o.maxwin.cy = g.sys.scr.cy + 2 * (-o.maxwin.y);
   // sizing moving options -------------------------------------------
   o.sizpos.snapMoumsg = WM_BUTTON1CLICK;
   o.sizpos.snapMoukbd = KC_SHIFT;
   o.sizpos.framedrg = 1;
   o.sizpos.frameMoumsg = WM_BUTTON2DOWN;
//     o.sizpos.frameMoukbd = KC_SHIFT;
   o.sizpos.Zmove = 1;
   o.sizpos.ZordMoumsg = WM_BUTTON2CLICK;
   o.sizpos.ZordMoukbd = KC_SHIFT;
   // text controls options -------------------------------------------
   // entryfield
   o.txt.efsel = 1;
   o.txt.selMoumsg = g.is.mou3btns?
                         WM_BUTTON3CLICK: WM_BUTTON1DBLCLK;
   o.txt.cpyMoumsg = WM_BUTTON1CLICK;
   o.txt.cpyMoukbd = KC_CTRL;
   o.txt.cutMoumsg = WM_BUTTON1CLICK;
   o.txt.cutMoukbd = KC_ALT;
   o.txt.insMoumsg = WM_BUTTON1CLICK;
   o.txt.insMoukbd = KC_ALT | KC_SHIFT;
   o.txt.ovwMoumsg = WM_BUTTON2CLICK;
   o.txt.ovwMoukbd = KC_SHIFT;
   // VIO windows options ---------------------------------------------
   o.vio.cpyMoumsg = WM_BUTTON1DOWN;
   o.vio.cpyMoukbd = KC_CTRL;
   o.vio.cpyaMoumsg = WM_BUTTON1CLICK;
   o.vio.cpyaMoukbd = KC_CTRL | KC_SHIFT;
   o.vio.insMoumsg = WM_BUTTON1CLICK;
   o.vio.insMoukbd = KC_SHIFT;
   o.vio.cpyKbdVK = VK_INSERT | 0x6800;
   o.vio.cpyKbdKC = KC_CTRL;
   o.vio.cpyaKbdVK = VK_INSERT | 0x6800;
   o.vio.cpyaKbdKC = KC_CTRL | KC_SHIFT;
   o.vio.insKbdVK = VK_INSERT | 0x6800;
   o.vio.insKbdKC = KC_SHIFT;
   // mouse options ---------------------------------------------------
   // sliding focus options
   o.mou.focusAuto = 1;
   o.mou.focusDelay = 600;
   o.mou.focusNoDesk = 1;
   o.mou.focusNoTop = 0;
   o.mou.focusNoNew = 1;
   // mouse pointer hiding options
   o.mou.hide = 1;
   o.mou.hideDelay = 1000;
   o.mou.hideNotOnMenu = 1;
   o.mou.hideNotOnBtn = 1;
   // remap bottone 3 mouse
   o.mou.btn3dblClk = g.is.mou3btns? 1: 0;
   o.mou.moveSteps = (g.sys.ver > 40) ? 100 : 20;
   // keyboard options ------------------------------------------------
   o.wpskbd.setsKC = KC_CTRL;
   o.wpskbd.setsK = VK_F12 | 0x5800;
   o.wpskbd.parentK = VK_BACKSPACE | 0x0e00;
   o.wpskbd.findKC = KC_CTRL;
   o.wpskbd.findK = VK_F1 | 0x3b00;
   o.wpskbd.srtnameK = VK_F2 | 0x3c00;
   o.wpskbd.srtextK = VK_F3 | 0x3d00;
   o.wpskbd.srttypeKC = KC_CTRL;
   o.wpskbd.srttypeK = VK_F2 | 0x3c00;
   o.wpskbd.srtrnameKC = KC_CTRL;
   o.wpskbd.srtrnameK = VK_F3 | 0x3d00;
   o.wpskbd.srtsizeKC = KC_CTRL;
   o.wpskbd.srtsizeK = VK_F4 | 0x3e00;
   o.wpskbd.srtwriteK = VK_F4 | 0x3e00;
   o.wpskbd.srtaccKC = KC_CTRL;
   o.wpskbd.srtaccK = VK_F5 | 0x3f00;
   o.wpskbd.srtcreatKC = KC_CTRL;
   o.wpskbd.srtcreatK = VK_F6 | 0x4000;
   o.wpskbd.arrangeK = VK_F6 | 0x4000;
   o.wpskbd.refreshK = VK_F5 | 0x3f00;
}


//===========================================================================
// Determines which mouse event must not be passed to the window procedure
// in order to let Styler/2 trap a successive composite event (i.e.
// WM_BUTTONCLICK is never received if the procedure intercepts WM_BUTTONDOWN).
// Parameters --------------------------------------------------------------
// ULONG msg : mouse event needed to implement a given Styler/2 feature.
// Return value ------------------------------------------------------------
// ULONG : mouse event to be ignored.
//===========================================================================

ULONG stlrMouEventToSkip(ULONG msg) {
   switch (msg) {
   case WM_BUTTON1CLICK:
   case WM_BUTTON1DBLCLK:
      return WM_BUTTON1DOWN;
   case WM_BUTTON2CLICK:
   case WM_BUTTON2DBLCLK:
      return WM_BUTTON2DOWN;
   case WM_BUTTON3CLICK:
   case WM_BUTTON3DBLCLK:
      return WM_BUTTON3DOWN;
   } /* endswitch */
   return 0;
}


//===========================================================================
// Check the exceptions presence.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID checkExceptions(VOID) {
   BOOL rc;
   ULONG ul;
   // rileva presenza eccezioni generali
   rc = PrfQueryProfileSize(HINI_USER, OS2INIAPPL, SZEXC_GENERAL, &ul);
   o.gen.exc = rc && ul > 0;
   // rileva presenza eccezioni "Windows"
   rc = PrfQueryProfileSize(HINI_USER, OS2INIAPPL, SZEXC_MAINWIN, &ul);
   o.dlg.exc = rc && ul > 0;
   // rileva presenza eccezioni "titlebar enhancements"
   rc = PrfQueryProfileSize(HINI_USER, OS2INIAPPL, SZEXC_TBARGEN, &ul);
   o.tb.exc = rc && ul > 0;
   // rileva presenza eccezioni "titlebar hiding"
   rc = PrfQueryProfileSize(HINI_USER, OS2INIAPPL, SZEXC_TBARHIDE, &ul);
   o.tbh.exc = rc && ul > 0;
   // rileva presenza eccezioni "window rolling"
   rc = PrfQueryProfileSize(HINI_USER, OS2INIAPPL, SZEXC_WINROLL, &ul);
   o.roll.exc = rc && ul > 0;
   // rileva presenza eccezioni "Maximized windows"
   rc = PrfQueryProfileSize(HINI_USER, OS2INIAPPL, SZEXC_MAXWIN, &ul);
   o.maxwin.exc = rc && ul > 0;
   // rileva presenza eccezioni "Sizing/moving"
   rc = PrfQueryProfileSize(HINI_USER, OS2INIAPPL, SZEXC_SZMOVE, &ul);
   o.sizpos.exc = rc && ul > 0;
   // rileva presenza eccezioni "text controls"
   rc = PrfQueryProfileSize(HINI_USER, OS2INIAPPL, SZEXC_TXTCTL, &ul);
   o.txt.exc = rc && ul > 0;
   // rileva presenza eccezioni "VIO windows"
   rc = PrfQueryProfileSize(HINI_USER, OS2INIAPPL, SZEXC_VIOWIN, &ul);
   o.vio.exc = rc && ul > 0;
   // rileva presenza eccezioni "mouse esceptions"
   rc = PrfQueryProfileSize(HINI_USER, OS2INIAPPL, SZEXC_MOUSE, &ul);
   o.mou.exc = rc && ul > 0;
}


//===========================================================================
// Get the handles used for the default bitmaps used to paint the frame
// controls.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================
static
BOOL getDefFrameCtlsBmpHnds(VOID) {
   HMQ hmq;
   HAB hab;
   BOOL rc = FALSE;
   CLASSINFO ci;
   HWND hwnd, hmn;
   FRAMECDATA fcd = {0};
   MENUITEM mi;
   // crea finestra fittizia x ricavare handle bmp controlli default finestra
   hmq = WinCreateMsgQueue((hab = WinInitialize(0)), 0);
   // dati classe menu
   if (!WinQueryClassInfo(NULLHANDLE, WC_MENU, &ci)) goto end;
   g.cd.menu.pfnwp = ci.pfnWindowProc;
   g.cd.menu.cbData = ci.cbWindowData;
   // crea finestra fittizia
   fcd.cb = sizeof(FRAMECDATA);
   fcd.flCreateFlags = FCF_SYSMENU | FCF_MINMAX | FCF_HIDEBUTTON;
   if ((hwnd = WinCreateWindow(HWND_DESKTOP, WC_FRAME, "", 0, 0, 0, 0, 0, 0,
                               HWND_BOTTOM, 0, &fcd, NULL)) != NULLHANDLE) {
      // ricava handle bitmap titlebar button default
      hmn = WinWindowFromID(hwnd, FID_SYSMENU);
      g.cd.menu.pfnwp(hmn, MM_QUERYITEM, (MPARAM)SC_SYSMENU, (MPARAM)&mi);
      g.bmpDef.sys = mi.hItem;
      hmn = WinWindowFromID(hwnd, FID_MINMAX);
   #ifndef STLR_WARP3
      g.cd.menu.pfnwp(hmn, MM_QUERYITEM, (MPARAM)SC_CLOSE, (MPARAM)&mi);
      g.bmpDef.cls = mi.hItem;
   #endif
      g.cd.menu.pfnwp(hmn, MM_QUERYITEM, (MPARAM)SC_MINIMIZE, (MPARAM)&mi);
      g.bmpDef.min = mi.hItem;
      g.cd.menu.pfnwp(hmn, MM_QUERYITEM, (MPARAM)SC_HIDE, (MPARAM)&mi);
      g.bmpDef.hide = mi.hItem;
      g.cd.menu.pfnwp(hmn, MM_QUERYITEM, (MPARAM)SC_MAXIMIZE, (MPARAM)&mi);
      g.bmpDef.max = mi.hItem;
      WinSetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_MAXIMIZE);
      g.cd.menu.pfnwp(hmn, MM_QUERYITEM, (MPARAM)SC_RESTORE, (MPARAM)&mi);
      g.bmpDef.rest = mi.hItem;
      WinDestroyWindow(hwnd);
      rc = TRUE;
   } // end if
   end:
   WinDestroyMsgQueue(hmq);
   WinTerminate(hab);
   if (!rc) stlrlog(IDERR_BMPMINMAX);
   return rc;
}


//===========================================================================
// Superclass the main PM window classes.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================
static
BOOL superclassMainPMclasses(VOID) {
   if (!stlrSuperclass(WC_FRAME, &g.cd.frame, stlrFrameProc, 4,
                          IDERR_FRAMECLASS, IDERR_NEWFRAME))
      return FALSE;
   if (!stlrSuperclass(WC_TITLEBAR, &g.cd.tbar, stlrTitleBarProc, 4,
                          IDERR_TITLEBARCLASS, IDERR_NEWTITLEBAR))
      goto restoreWC_FRAME;
   if (!stlrSuperclass(WC_ENTRYFIELD, &g.cd.ef, stlrEntryFieldProc, 4,
                          IDERR_ENTRYFCLASS, IDERR_NEWENTRYF))
      goto restoreWC_TITLEBAR;
   if (!stlrSuperclass(WC_BUTTON, &g.cd.btn, stlrButtonProc, 4,
                          IDERR_BTNCLASS, IDERR_NEWBTN))
      goto restoreWC_ENTRYFIELD;
   if (!stlrSuperclass(WC_MENU, &g.cd.menu, stlrMenuProc, 4,
                          IDERR_MENUCLASS, IDERR_NEWMENU))
      goto restoreWC_BUTTON;
   if (!stlrSuperclass(WC_STATIC, &g.cd.stat, stlrStaticProc, 0,
                          IDERR_STATICCLASS, IDERR_NEWSTATIC))
      goto restoreWC_MENU;
   if (!stlrSuperclass(WC_LISTBOX, &g.cd.lbox, stlrListboxProc, 0,
                          IDERR_LBOXCLASS, IDERR_NEWLBOX))
      goto restoreWC_STATIC;
   if (!stlrSuperclass(WC_COMBOBOX, &g.cd.combo, stlrComboProc, 0,
                          IDERR_COMBOCLASS, IDERR_NEWCOMBO))
      goto restoreWC_COMBOBOX;
   return TRUE;

   // in case of error restore the original window classes procedures
restoreWC_COMBOBOX:
   stlrSuperclass(WC_COMBOBOX, NULL, g.cd.combo.pfnwp, 0, 0, 0);
restoreWC_STATIC:
   stlrSuperclass(WC_STATIC, NULL, g.cd.stat.pfnwp, 0, 0, 0);
restoreWC_MENU:
   stlrSuperclass(WC_MENU, NULL, g.cd.menu.pfnwp, -4, 0, 0);
restoreWC_BUTTON:
   stlrSuperclass(WC_BUTTON, NULL, g.cd.btn.pfnwp, -4, 0, 0);
restoreWC_ENTRYFIELD:
   stlrSuperclass(WC_ENTRYFIELD, NULL, g.cd.ef.pfnwp, -4, 0, 0);
restoreWC_TITLEBAR:
   stlrSuperclass(WC_TITLEBAR, NULL, g.cd.tbar.pfnwp, -4, 0, 0);
restoreWC_FRAME :
   stlrSuperclass(WC_FRAME, NULL, g.cd.frame.pfnwp, -4, 0, 0);
   return FALSE;
}


//===========================================================================
// Superclass a PM window class optionally storing the data of the original
// class in the 'pcd' parameter.
// Parameters --------------------------------------------------------------
// PSZ pszClass   : PM window class which must be superclassed
// PCLASSDATA pcd : storage for the default class data
// PFNWP pfnwpNew : new window class procedure
// LONG cbWin     : needed window words size increment
// ULONG id1      : error code 1
// ULONG id2      : error code 2
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================

BOOL stlrSuperclass(PSZ pszClass, PCLASSDATA pcd, PFNWP pfnwpNew,
                    LONG cbWin, ULONG id1, ULONG id2) {
   CLASSINFO ci;
   if (!WinQueryClassInfo(NULLHANDLE, pszClass, &ci))
      return _stlrLoadError(id1, FALSE);
   if (pcd) {
      pcd->pfnwp = ci.pfnWindowProc;
      pcd->cbData = ci.cbWindowData;
   } /* endif */
   if (!WinRegisterClass(NULLHANDLE, pszClass, pfnwpNew,
                         ci.flClassStyle, ci.cbWindowData + cbWin))
      return _stlrLoadError(id2, FALSE);
   return TRUE;
}


//===========================================================================
// Start the worker threads.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================
static
BOOL initWorkerThreads(VOID) {
   TID tidTimeMgr = 0;
   TID tidMouMgr = 0;
   // crea thread timers
   if (DosCreateThread(&tidTimeMgr, (PFNTHREAD)threadTimerMgr,0, 0, 8192))
      return _stlrLoadError(IDERR_CREATETIMERMGR, FALSE);
   // crea thread server per spostamenti mouse
   if (DosCreateThread(&tidMouMgr, (PFNTHREAD)threadMouMovMgr, 0, 0, 8192)) {
      DosKillThread(tidTimeMgr);
      return _stlrLoadError(IDERR_CREATESERVERTHRD, FALSE);
   } /* endif */
   return TRUE;
}


//===========================================================================
// Initialize the event and the send message hooks.
// Parameters --------------------------------------------------------------
// HMODULE hmod : the handle of this module
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================
static
BOOL initHooks(HMODULE hmod) {
   // inizializzazione input hook
   if (!WinSetHook(0, 0, HK_INPUT, (PFN)postMsgHook, hmod))
      return _stlrLoadError(IDERR_CREATEINPUTHOOK , FALSE);
   if(!WinSetHook(0, 0, HK_SENDMSG, (PFN)sendMsgHook, hmod)) {
      WinReleaseHook(0, 0, HK_INPUT, (PFN)postMsgHook, hmod);
      return _stlrLoadError(IDERR_CREATESNMSGHOOK, FALSE);
   } /* endif */
   return TRUE;
}


//===========================================================================
// DLL termination procedure.
// Do nothing as the system has already been shutdown when this is executed.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// ULONG : always return 1.
//===========================================================================
static
ULONG terminate(VOID) {
   // WinDestroyWindow(g.timerThrd.hwnd);
   // WinReleaseHook(NULLHANDLE, NULLHANDLE, HK_INPUT, (PFN)postMsgHook, g.hmod);
   // WinReleaseHook(NULLHANDLE, NULLHANDLE, HK_SENDMSG, (PFN)sendMsgHook, g.hmod);
   return 1;
}
