//===========================================================================
// utils.c: utility functions.
//===========================================================================


#pragma strings(readonly)

#include "stlrMain.h"


//===========================================================================
// Suspend the WM_HITTEST monitoring and all the timers associated with it.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID stlrStopHitTestCheck(VOID) {

   g.hwnd.lastCreated = NULLHANDLE;  // resetta flag nuova finestra creata
   // se Š attivo autofocus con subopzione "don't bring to top"
   // e hwnd Š attiva ma non al top, la porta al top
   if (g.hwnd.mainMouWin
       && o.mou.focusAuto
       && o.mou.focusNoTop
       && !(g.is.mousebtn & DOWN2)
       && (g.hwnd.mainMouWin == g.hwnd.active)
       && (g.hwnd.mainMouWin != WinQueryWindow(g.hwnd.deskFldr, QW_NEXTTOP)))
      WinSetWindowPos(g.hwnd.mainMouWin,
                      HWND_TOP, 0L, 0L, 0L, 0L, SWP_ZORDER | SWP_NOADJUST);

   // resetta funzione mouse pointer hiding
   _resetHideMouTimer();
   // se puntatore mouse non visibile mostra puntatore mouse
   unhideMouPtr();
   // sospende il timer dell'autofocus
   if (g.hwnd.autoFocus) {
      if (o.mou.focusDelay) _resetAutoFocusTimer();
   } // end if
   // sospende il timer di apparizione della barra del titolo
   if (g.hwnd.unhideTitle) {
      if (o.tbh.unhideDelay)
         WinStopTimer(g.timerThrd.hab, g.timerThrd.hwnd, TID_UNHIDETITLE);
      g.hwnd.unhideTitle = NULLHANDLE;
   } // end if
//   if (g.hwndActMenu) {
//      if (o.dlMenuSelect)
//         WinStopTimer(g.timerThrd.hab, g.timerThrd.hwnd, TID_MENUACTIVATE);
//      g.hwndActMenu = NULLHANDLE;
//   } // end if
}


//===========================================================================
// Remove the clipboard content and insert the text 'psz'.
// Parameters --------------------------------------------------------------
// HAB hab : anchor block handle.
// PSZ psz : text to be inserted into the clipboard.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID copyTextToClipboard(HAB hab, PSZ psz) {
   if (WinOpenClipbrd(hab)) {
      if (WinEmptyClipbrd(hab))
         WinSetClipbrdData(hab, (ULONG)psz, CF_TEXT, CFI_POINTER);
      WinCloseClipbrd(hab);
   } /* endif */
}


//===========================================================================
// Get the clipboard content (if it is CF_TEXT).
// Parameters --------------------------------------------------------------
// HAB hab : anchor block handle.
// Return value ------------------------------------------------------------
// PSZ : current clipboard content, NULL if the clipboard is empty or does
//       not contain text data.
//===========================================================================

PSZ getTextFromClipboard(HAB hab) {
   PSZ psz = NULL;
   PSZ pclipdata;
   if (WinOpenClipbrd(hab)) {
      pclipdata = (PSZ)WinQueryClipbrdData(hab, CF_TEXT);
      if (pclipdata) psz = memLkAlloc(strlen(pclipdata) + 1);
      if (psz) strcpy(psz, pclipdata);
      WinCloseClipbrd(hab);
   } /* endif */
   return psz;
}


//===========================================================================
// Copy the window text to the clipboard.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID winTextToClipboard(HWND hwnd) {
   ULONG ul = WinQueryWindowTextLength(hwnd) + 1;
   PSZ psz;
   if ((ul > 1) && !DosAllocSharedMem((PPVOID)&psz, NULL, ul, FLCLIPMEM)) {
      WinQueryWindowText(hwnd, ul, psz);
      copyTextToClipboard(WinQueryAnchorBlock(hwnd), psz);
   } /* endif */
}


//===========================================================================
// Write data in the error log file or in the debug log file.
// Parameters --------------------------------------------------------------
// HFILE hf : file handle.
// PSZ psz  : data to be written to the file.
// Return value ------------------------------------------------------------
// ULONG : count of written data.
//===========================================================================

ULONG logwrite(HFILE hf, PSZ psz) {
   ULONG ulWrote;
   DosWrite(hf, psz, strlen(psz), &ulWrote);
   return ulWrote;
}


//===========================================================================
// Copy source to target substituting all consecutive \r\n with 1 space.
// Parameters --------------------------------------------------------------
// PSZ target : target (converted) string.
// PSZ source : source (to be converted) string.
// ULONG cch  : count of characters to be converted.
// Return value ------------------------------------------------------------
// ULONG : resulting length of target.
//===========================================================================

ULONG psznconvcpy(PSZ target, PSZ source, ULONG cch) {
   ULONG i;
   for (i = 0; *source && i < cch - 1; ++i) {
      if (*source == '\r' || *source == '\n') {
         while(*source == '\r' || *source == '\n') ++source;
         *target++ = ' ';
         continue;
      } /* endif */
      *target++ = *source++;
   } // end for
   return i;
}


//===========================================================================
// Open the log file.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// HFILE : log file handle.
//===========================================================================

HFILE stlrOpenLog(VOID) {
   ULONG ul;
   CHAR buf[16];
   HFILE hf = 0;
   buf[0] = g.sys.drv;
   strcpy(&buf[1], ":\\Styler2.log");
   if (DosOpen(buf, &hf, &ul, 0L, FILE_NORMAL,
               OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
               OPEN_SHARE_DENYNONE | OPEN_ACCESS_WRITEONLY, NULL) ||
       DosSetFilePtr(hf, 0, FILE_END, &ul)) {
      if (hf) DosClose(hf);
      return NULLHANDLE;
   } // end if
   return hf;
}


//===========================================================================
// Write current date and time (according to national settings) to the buffer.
// Parameters --------------------------------------------------------------
// PSZ psz : output buffer.
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================

BOOL stlrDateTime(PSZ psz) {
   COUNTRYCODE cc = {0};
   COUNTRYINFO ci = {0};
   DATETIME dt;
   ULONG ul = 0;
   // rileva informazioni relative formato dati secondo nazionalit…
   if (DosQueryCtryInfo(sizeof(COUNTRYINFO), &cc, &ci, &ul))
      return FALSE;
   DosGetDateTime(&dt);
   psz[2] = psz[5] = *ci.szDateSeparator;
   psz[11] += dt.hours / 10;
   psz[12] += dt.hours % 10;
   psz[13] = psz[16] = *ci.szTimeSeparator;
   psz[14] += dt.minutes / 10;
   psz[15] += dt.minutes % 10;
   psz[17] += dt.seconds / 10;
   psz[18] += dt.seconds % 10;
   dt.year %= 100;
   switch (ci.fsDateFmt) {
      case(1):                                         /* dd/mm/yy */
         psz[0] += dt.day / 10; psz[1] += dt.day % 10;
         psz[3] += dt.month / 10; psz[4] += dt.month % 10;
         psz[6] += dt.year / 10; psz[7] += dt.year % 10;
         break;
      case(2):                                         /* yy/mm/dd */
         psz[0] += dt.year / 10; psz[1] += dt.year % 10;
         psz[3] += dt.month / 10; psz[4] += dt.month % 10;
         psz[6] += dt.day / 10; psz[7] += dt.day % 10;
         break;
      default:                                         /* mm/dd/yy */
         psz[0] += dt.month / 10; psz[1] += dt.month % 10;
         psz[3] += dt.day / 10; psz[4] += dt.day % 10;
         psz[6] += dt.year / 10; psz[7] += dt.year % 10;
         break;
   } /* endswitch */
   return TRUE;
}


//===========================================================================
// Write an error message in the log file.
// Parameters --------------------------------------------------------------
// ULONG ulmsg : message id.
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================

BOOL stlrlog(ULONG ulmsg) {
   HFILE hf;
   CHAR ach[32];
   if (!ulmsg) return TRUE;
   if (!(hf = stlrOpenLog())) goto err0;
   strcpy(ach, "Styler/2 v. 00.00.00.00\n");
   ach[12] = g.ver.Mver / 10;
   ach[13] = g.ver.Mver % 10;
   ach[15] = g.ver.mver / 10;
   ach[16] = g.ver.mver % 10;
   ach[18] = g.ver.rver / 10;
   ach[19] = g.ver.rver % 10;
   ach[21] = g.ver.bver / 10;
   ach[22] = g.ver.bver % 10;
   if (!logwrite(hf, ach)) goto err1;
   strcpy(ach, " 00/00/00 - 00:00:00 - #000\n");
   if (!stlrDateTime(ach + 1)) goto err1;
   ach[24] = '0' + ulmsg / 100;
   ach[25] = '0' + (ulmsg % 100) / 10;
   ach[26] = '0' + ulmsg % 10;
   if (!logwrite(hf, ach)) goto err1;
   DosClose(hf);
   return TRUE;
err1:
   DosClose(hf);
err0:
   DosBeep(440, 500);
   return FALSE;
}



