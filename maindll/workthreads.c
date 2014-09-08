//==========================================================================\
// tmrmgr.c: timer manager and server thread                                |
// VOID _System threadTimerMgr(ULONG ul);
// MRESULT EXPENTRY threadTimerMgrProc(HWND hwnd, ULONG msg,
//                                        MPARAM mp1, MPARAM mp2);
// VOID terminateTimerMgr(HMQ hmq);
//==========================================================================/


#pragma strings(readonly)

// includes -----------------------------------------------------------------

#include "stlrMain.h"
#include "stlrError.h"


// shared bitmap API import -------------------------------------------------

//#pragma import(GpiSetBitmapOwner,,"PMGPI.DLL",620)
//


// defines ------------------------------------------------------------------

#define _resetTimer(_hwnd_, _ulTimer_) \
   WinStopTimer(g.timerThrd.hab, g.timerThrd.hwnd, (_ulTimer_)), \
   WinPostMsg((_hwnd_), WM_STYLER2, \
              (MPARAM)(CMDSRC_SMW | (SHORT)(_ulTimer_)), MPVOID), \
   (_hwnd_) = NULLHANDLE


// prototypes ---------------------------------------------------------------

MRESULT EXPENTRY threadTimerMgrProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID terminateTimerMgr(HMQ hmq);
BOOL freeBitmapResources(VOID);


//===========================================================================
// Time critical thread controlling the automatic mouse movement.
// Parameters --------------------------------------------------------------
// ULONG ul : ignrored.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID _System threadMouMovMgr(ULONG ul) {
   ULONG ulPost;
   POINTL ptl;
   INT i;
   ULONG covrd;   // numero corrente spostamenti mouse da traiettoria impostata
   ULONG cstops;  // numero di movimenti volontari mouse necessari per
                  // cancellare movimento automatico
   if (DosCreateEventSem(NULL, &g.mou.hev, DC_SEM_SHARED, 0)) {
      stlrlog(IDERR_CREATESERVEVSEM);
      return;
   } /* endif */
   DosSetPriority(PRTYS_THREAD, PRTYC_TIMECRITICAL, 31, 0);

   for (;;) {
      restart:      // label su cui punta quando esce dal loop interno
      g.is.mousebtn &= ~MOVEMOUPTR;
      covrd = 0;
      DosWaitEventSem(g.mou.hev, SEM_INDEFINITE_WAIT);
      cstops = (o.mou.moveSteps & 0x2f) >> 3;
      for (i = 0; i < o.mou.moveSteps; ++i) {
         WinQueryPointerPos(HWND_DESKTOP, &ptl);
         // se il mouse Š stato spostato dall'utente x 3 volte
         if (memcmp(&ptl, &g.mou.ptc, sizeof(POINTL)) && ++covrd > cstops)
            break;
         g.mou.wr.cx += g.mou.dp.x;
         g.mou.wr.cy += g.mou.dp.y;
         g.mou.ptc.x = (LONG)(g.mou.wr.cx >> 16);
         g.mou.ptc.y = (LONG)(g.mou.wr.cy >> 16);
         WinSetPointerPos(HWND_DESKTOP, g.mou.ptc.x, g.mou.ptc.y);
         DosSleep(1);
         // se semaforo Š stato resettato perch‚ il puntatore del mouse deve
         // essere spostato su una finestra differente ricomincia dall'inizio
         DosQueryEventSem(g.mou.hev, &ulPost);
         if (!ulPost) goto restart;
      } /* endfor */
      DosResetEventSem(g.mou.hev, &ulPost);
      g.mou.hwndNew = g.mou.hwnd = NULLHANDLE;
   } /* endfor */
   DosCloseEventSem(g.mou.hev);
}


//===========================================================================
// Object window thread to manage timers.
// Parameters --------------------------------------------------------------
// ULONG ul : ignrored.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID _System threadTimerMgr(ULONG ul) {
   HMQ hmq;
   QMSG qmsg;

   DosSetPriority(PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, 31, 0);
   // inizializzazione
   hmq = WinCreateMsgQueue((g.timerThrd.hab = WinInitialize(0)), 0);
   WinCancelShutdown(hmq, TRUE);
   // registra classe WC_TIMERMGRCLASS
   if (!WinRegisterClass(g.timerThrd.hab, WC_TIMERMGRCLASS,
                         threadTimerMgrProc, 0L, 0L))
      terminateTimerMgr(hmq);

   // crea l'object window
   if (!(g.timerThrd.hwnd = WinCreateWindow(HWND_OBJECT, WC_TIMERMGRCLASS,
                                            NULL, 0, 0, 0, 0, 0, NULLHANDLE,
                                            HWND_BOTTOM, 0, NULL, NULL)))
      terminateTimerMgr(hmq);

   // message loop
   while (WinGetMsg(g.timerThrd.hab, &qmsg, NULLHANDLE, 0, 0))
      WinDispatchMsg(g.timerThrd.hab, &qmsg);

   // terminazione
   terminateTimerMgr(hmq);
}


//===========================================================================
// Timer management - object window procedure.
// Parameters --------------------------------------------------------------
// Ordinary window procedure parameters.
// Return value ------------------------------------------------------------
// MRESULT : message result.
//===========================================================================

MRESULT EXPENTRY threadTimerMgrProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   if (msg == WM_TIMER) {
      switch (LONGFROMMP(mp1)) {
      // scadenza ritardo sparizione puntatore mouse ---------------------
         case TID_MOUSEPTR:
            g.is.hidePtrTmr = 0;
            WinStopTimer(g.timerThrd.hab, g.timerThrd.hwnd, TID_MOUSEPTR);
            msg = _sysValue(SV_POINTERLEVEL);
            if (!msg) {
               WinShowPointer(HWND_DESKTOP, FALSE);
               WinShowPointer(HWND_DESKTOP, FALSE);
            } else if (msg > 0x7fff) {
               do {
                  WinShowPointer(HWND_DESKTOP, FALSE);
               } while (_sysValue(SV_POINTERLEVEL) > 0x7fff);
               WinShowPointer(HWND_DESKTOP, FALSE);
            } /* endif */
            g.is.ptrHidden = 1;
            break;
      // scadenza ritardo attivazione automatica finestre ----------------
         case TID_AUTOFOCUS:
            _resetTimer(g.hwnd.autoFocus, TID_AUTOFOCUS);
            break;
      // scadenza ritardo sparizione barra titolo ------------------------
         case TID_HIDETITLE:
            _resetTimer(g.hwnd.hideTitle, TID_HIDETITLE);
            break;
      // scadenza ritardo apparizione barra titolo -----------------------
         case TID_UNHIDETITLE:
            _resetTimer(g.hwnd.unhideTitle, TID_UNHIDETITLE);
            break;
      // scadenza ritardo attivazione automatica barra menu --------------
//         case TID_MENUACTIVATE:
//            _resetTimer(g.hwndActMenu, TID_MENUACTIVATE);
//            break;
      } /* endswitch */
      return (MRESULT)FALSE;
   } /* endif */
   // reset all the bitmaps
   if (msg == SMWM_RESETBITMAPS) {
      HINI hini;
      if (o.tb.on || o.tb.htbar || o.tb.roll) {
         if (freeBitmapResources() &&
             (NULLHANDLE != (hini = stlrOpenProfile()))) {
            initResources(hini, FALSE);
            repaintAll();
            PrfCloseProfile(hini);
         } /* endif */
      } /* endif */
      return (MRESULT)FALSE;
   } /* endif */
   if ((msg == SMWM_DISABLE) && ((HWND)mp2 == hwnd)) {
      if ((BOOL)mp1) { // disabilita tutte le funzioni
         o.gen.disabled = 1;
         g.hwnd.menu = 0;
         g.is.cinitmenu = 0;
         freeBitmapResources();
         stlrStopHitTestCheck();
      } else if (!g.is.expired) {         // riabilita funzioni
         HINI hini;
         o.gen.disabled = 0;
         if (NULLHANDLE != (hini = stlrOpenProfile()))
            initResources(hini, FALSE);
      } /* endif */
      WinBroadcastMsg(HWND_DESKTOP, WM_STYLER2, (MPARAM)STLR_DISABLED,
                      (MPARAM)o.gen.disabled,
                      BMSG_POST | BMSG_FRAMEONLY | BMSG_DESCENDANTS);
      return (MRESULT)FALSE;
   } // end if
   if (msg == WM_STYLER2) {
      if ((ULONG)mp1 == STLR_HIDEALL) {
         g.is.hideall = !g.is.hideall;
         _stlrMsgPostAll(STLR_HIDEALL, 0);
      } else if ((ULONG)mp1 == STLR_ROLLALL) {
         g.is.rollall = !g.is.rollall;
         _stlrMsgPostAll(STLR_ROLLALL, 0);
      } /* endif */
      return (MRESULT)FALSE;
   } /* endif */
   return WinDefWindowProc(hwnd, msg, mp1, mp2);
}


//===========================================================================
// Termination of the timer manager thread.
// Parameters --------------------------------------------------------------
// HMQ hmq : message queue handle.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID terminateTimerMgr(HMQ hmq) {
   g.is.expired = 1;
   if (g.timerThrd.hwnd) WinDestroyWindow(g.timerThrd.hwnd);
   WinDestroyMsgQueue(hmq);
   WinTerminate(g.timerThrd.hab);
   DosExit(EXIT_THREAD, 0);
}


//===========================================================================
// Free all bitmap resources.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

BOOL freeBitmapResources(VOID) {
   HPS hps;
   if (NULLHANDLE != (hps = WinGetScreenPS(HWND_DESKTOP))) {
      if (o.tb.on) {
         _stlrFreeGlobalBitmap(hps, o.tb.a.hbmp, g.timerThrd.hab);
         _stlrFreeGlobalBitmap(hps, o.tb.i.hbmp, g.timerThrd.hab);
         _stlrFreeGlobalBitmap(hps, o.tb.bmp.sysmenu, g.timerThrd.hab);
         _stlrFreeGlobalBitmap(hps, o.tb.bmp.close, g.timerThrd.hab);
         _stlrFreeGlobalBitmap(hps, o.tb.bmp.hide, g.timerThrd.hab);
         _stlrFreeGlobalBitmap(hps, o.tb.bmp.min, g.timerThrd.hab);
         _stlrFreeGlobalBitmap(hps, o.tb.bmp.rest, g.timerThrd.hab);
         _stlrFreeGlobalBitmap(hps, o.tb.bmp.max, g.timerThrd.hab);
      } /* endif */
      if (o.tb.htbar) {
         _stlrFreeGlobalBitmap(hps, o.tb.bmp.htbar, g.timerThrd.hab);
         _stlrFreeGlobalBitmap(hps, o.tb.bmp.unhtbar, g.timerThrd.hab);
      } /* endif */
      if (o.tb.roll) {
         _stlrFreeGlobalBitmap(hps, o.tb.bmp.roll, g.timerThrd.hab);
         _stlrFreeGlobalBitmap(hps, o.tb.bmp.unroll, g.timerThrd.hab);
      } /* endif */
      WinReleasePS(hps);
      return TRUE;
   } /* endif */
   return FALSE;
}


//===========================================================================
// Get from smartwin.ini or from the resources DLL the handle of the bitmaps
// to be used to draw the titlebar and the titlebar buttons.
// Get from the resources DLL the text strings used for the system menu
// extensions.
// Parameters --------------------------------------------------------------
// HINI hini         : smartwin.ini handle.
// BOOL bLoadStrings : TRUE/FALSE (load/don't load system menu strings)
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================

VOID initResources(HINI hini, BOOL bLoadStrings) {
   HMODULE hmod;
   CHAR buf[CCHMAXPATH];
   ULONG ul;
   PBYTE p;
   BOOL rc = FALSE;
   HPS hps;
//   /*-*/ dbgPrintf2("--- initResources(%d)\n", bLoadStrings);
   // get the path of the resource DLL
   if (0 == (ul = PrfQueryProfileString(HINI_USER, OS2INIAPPL, "path",
                                        NULL, buf, sizeof(buf))))
      goto exit0;
   strcpy(buf + ul - 1, SZ_RESMODNAME);
   // load the resource DLL
   if (DosLoadModule(NULL, 0, buf, &hmod))
      goto exit0;
//   /*-*/ dbgPrintf2("--- initResources() res module handle: %08x\n", hmod);

   // get the resource strings
   if (bLoadStrings) {
      _stlrLoadStr(hmod, ISSMN_SYSMNTBHIDE, g.text.tbhide);
      _stlrLoadStr(hmod, ISSMN_SYSMNRLUNRL, g.text.winroll);
      _stlrLoadStr(hmod, ISSMN_SYSMNALT, g.text.alt);
      _stlrLoadStr(hmod, ISSMN_SYSMNCTL, g.text.ctrl);
      _stlrLoadStr(hmod, ISSMN_SYSMNSHFT, g.text.shift);
      _stlrLoadStr(hmod, ISSMN_SYSMNINS, g.text.ins);
      _stlrLoadStr(hmod, ISSMN_SYSMNBKSPC, g.text.backspace);
      _stlrLoadStr(hmod, ISSMN_SYSMNDEL, g.text.del);
//      /*-*/ dbgPrintf6(">>> %s %s %s %s %s\n",
//                      g.text.tbhide, g.text.winroll,
//                      g.text.alt, g.text.ctrl, g.text.shift);
   } /* endif */
   if (NULLHANDLE == (hps = WinGetScreenPS(HWND_DESKTOP)))
      goto exit1;
   if (DosAllocMem((PPVOID)&p, 0x10000, PAG_READ | PAG_WRITE | PAG_COMMIT))
      goto exit2;
   // get the image handles
   // the image index depends on the titlebar height
   ul = g.sys.cyTbar > 18;
   if (!o.gen.disabled) {
      // if titlebar options are active
      if (o.tb.on) {
         // active titlebar -----------------------------------------
         //
         if (o.tb.a.bkgnd == TBARBKGNDBMP) {
            o.tb.a.hbmp = stlrHBmp(hini, SZPRO_BMPATBARDATA, p, hps,
                                       &o.tb.a.size, hmod, IDBMP_TBA);
         } else if (o.tb.a.bkgnd == TBARBKGNDSHADE) {
            o.tb.a.hbmp = stlrShadeHBmp(hps, (PSHDCREATE)p, &o.tb.a);
            GpiSetBitmapOwner(hps, o.tb.a.hbmp, 0);  // make global
         } /* endif */
         // inactive titlebar ---------------------------------------
         if (o.tb.i.bkgnd == TBARBKGNDBMP) {
            o.tb.i.hbmp = stlrHBmp(hini, SZPRO_BMPITBARDATA, p, hps,
                                       &o.tb.i.size, hmod, IDBMP_TBI);
         } else if (o.tb.i.bkgnd == TBARBKGNDSHADE) {
            o.tb.i.hbmp = stlrShadeHBmp(hps, (PSHDCREATE)p, &o.tb.i);
            GpiSetBitmapOwner(hps, o.tb.i.hbmp, 0);  // make global
         } /* endif */
         // system menu ---------------------------------------------
         o.tb.bmp.sysmenu = stlrHBmp(hini, SZPRO_BMPSMENUDATA, p, hps,
                                         NULL, hmod, 0);
         // close window button -------------------------------------
      #ifdef STLR_WARP3
         o.tb.bmp.close = stlrHBmp(hini, SZPRO_BMPCLOSEDATA, p, hps,
                                       NULL, hmod, IDBMP_CLOSE18 + ul);
      #else
         o.tb.bmp.close = stlrHBmp(hini, SZPRO_BMPCLOSEDATA, p, hps,
                                       NULL, hmod, 0);
      #endif
         // hide window button --------------------------------------
         o.tb.bmp.hide = stlrHBmp(hini, SZPRO_BMPHIDEDATA, p, hps,
                                        NULL, hmod, 0);
         // minimize window button ----------------------------------
         o.tb.bmp.min = stlrHBmp(hini, SZPRO_BMPMINDATA, p, hps,
                                        NULL, hmod, 0);
         // restore window button -----------------------------------
         o.tb.bmp.rest = stlrHBmp(hini, SZPRO_BMPRESTDATA, p, hps,
                                        NULL, hmod, 0);
         // maximize window button ----------------------------------
         o.tb.bmp.max = stlrHBmp(hini, SZPRO_BMPMAXDATA, p, hps,
                                        NULL, hmod, 0);
      } /* endif */
      if (o.tb.htbar) {
         // hide titlebar button ------------------------------------
         o.tb.bmp.htbar = stlrHBmp(hini, SZPRO_BMPHTBARDATA, p, hps,
                                        NULL, hmod, IDBMP_HT18 + ul);
         // unhide titlebar button ----------------------------------
         o.tb.bmp.unhtbar = stlrHBmp(hini, SZPRO_BMPUNHTBARDATA, p, hps,
                                        NULL, hmod, IDBMP_UHT18 + ul);
      } /* endif */
      if (o.tb.roll) {
         // roll window button --------------------------------------
         o.tb.bmp.roll = stlrHBmp(hini, SZPRO_BMPROLLDATA, p, hps,
                                        NULL, hmod, IDBMP_RL18 + ul);
         // unroll window button ------------------------------------
         o.tb.bmp.unroll = stlrHBmp(hini, SZPRO_BMPUNROLLDATA, p, hps,
                                        NULL, hmod, IDBMP_URL18 + ul);
      } /* endif */
   } /* endif */
   rc = TRUE;
   DosFreeMem(p);
exit2:
   DosFreeModule(hmod);
exit1:
   WinReleasePS(hps);
exit0:
   if (!rc) stlrlog(IDERR_GETRESMOD);
}
