//=========================================================================\
// editcol.c : color wheel dlg procedure                                   |
// version 1.1                                                             |
//=========================================================================/

#pragma strings(readonly)

#define INCL_WIN
//#define INCL_GPI
#define INCL_DOSMODULEMGR
#define INCL_DOSMISC
#include <os2.h>
#include <stdlib.h>
#include "common.h"
#include "editcol.h"
#include "api.h"


//=========================================================================\
// WeditColorDlg():                                                      |
// parameters: ----------------------------------------------------------- |
// HWND hParent  : parent handle                                           |
// HWND hOwner   : owner handle                                            |
// PCLRWHEEL pcw : control data structure                                  |
// returned value: ------------------------------------------------------- |
// LONG lColor      : RGB color (success), CLR_ERROR (failure)             |
//=========================================================================/


LONG APIENTRY WeditColorDlg(HWND hParent, HWND hOwner, PCLRWHEEL pcw) {
   PCLRAPPDATA pcad;
   HAB hab;
   HLIB hlib;
   ULONG ulVer;
   LONG lcolor;

   // allocate storage for application data
   if (!(pcad = (PCLRAPPDATA)malloc(sizeof(CLRAPPDATA)))) return CLR_ERROR;
   // initialize CLRAPPDATA structure
   pcad->pcw = pcw;
   hab = WinQueryAnchorBlock(hOwner);
   // window handles
   pcad->hOwner = hOwner;
   // query the OS version
   DosQuerySysInfo(QSV_VERSION_MINOR, QSV_VERSION_MINOR, &ulVer, 4);
   // sets msg value according to OS version
   if (ulVer > 30) {                  // warp 4
      pcad->cwChangedMsg = WM_COLORWHEELCHANGED;
      pcad->cwSetMsg = CWM_COLORWHEELSETVAL;
   } else {                                // warp 3 (warp 2?)
      pcad->cwChangedMsg = WM_COLORWHEELCHANGED3;
      pcad->cwSetMsg = CWM_COLORWHEELSETVAL3;
   } /* endif */
   // copy the start color
   pcad->clrCurr.l = pcad->clrUndo.l = pcw->clr;
   // register the color wheel control and open the dialog
   if (!(hlib = WinLoadLibrary(hab, "WPCONFIG.DLL"))) {
      lcolor = CLR_ERROR;
   } else {   
      lcolor = (LONG)WinDlgBox(hParent, hOwner, EditColorDlgProc, hmod,
                               DLG_CWHEEL, pcad);
      WinDeleteLibrary(hab, hlib);
   } // end if
   free(pcad);
   return lcolor;
}


//=========================================================================\
// EditColorDlgProc(): color wheel dlg procedure                           |
// processed messages: - WM_INITDLG                                        |
//                     - WM_COLORWHEELCHANGED                              |
//                     - WM_CONTROL                                        |
//                     - WM_COMMAND                                        |
//                     - WM_CLOSE                                          |
//                     - WM_DESTROY                                        |
//=========================================================================/

MRESULT EXPENTRY EditColorDlgProc(HWND hwnd, ULONG msg,
                                  MPARAM mp1, MPARAM mp2) {
   switch (msg) {
   // init -----------------------------------------------------------------
      case WM_INITDLG: {
         PCLRAPPDATA pcad = (PCLRAPPDATA)mp2;
         WinSetWindowPtr(hwnd, 0L, (PVOID)mp2);
         // ove necessario cambia caption comonenti dialogo
         if (pcad->pcw->pszTitle) WinSetWindowText(hwnd, pcad->pcw->pszTitle);
         if (pcad->pcw->pszOK)
            WinSetDlgItemText(hwnd, BTN_OK, pcad->pcw->pszOK);
         if (pcad->pcw->pszUndo)
            WinSetDlgItemText(hwnd, BTN_UNDO, pcad->pcw->pszUndo);
         if (pcad->pcw->pszCancel)
            WinSetDlgItemText(hwnd, BTN_CANCEL, pcad->pcw->pszCancel);
         if (pcad->pcw->pszRed)
            WinSetDlgItemText(hwnd, TXT_RED, pcad->pcw->pszRed);
         if (pcad->pcw->pszGreen)
            WinSetDlgItemText(hwnd, TXT_GREEN, pcad->pcw->pszGreen);
         if (pcad->pcw->pszBlue)
            WinSetDlgItemText(hwnd, TXT_BLUE, pcad->pcw->pszBlue);
         if (pcad->pcw->FontNameSize)
            WsetDlgFonts(hwnd, NULLHANDLE, pcad->pcw->FontNameSize);
         SetSpins(hwnd, pcad->clrUndo); // initialize spinbutton
         // initialize the color wheel control
         WinSendDlgItemMsg(hwnd, CWHEEL, pcad->cwSetMsg,
                           (MPARAM)pcad->clrUndo.l, MPVOID);
         pcad->is.upd = 1;            // sets the update flag
         // show the dialog
         if (pcad->pcw->fl & CWDS_CENTER) {
            WcenterInWindow(hwnd, pcad->pcw->hPos);
         } else {
            SWP swp;
            swp.hwnd = pcad->pcw->hPos;
            swp.x = pcad->pcw->ptl.x;
            swp.y = pcad->pcw->ptl.y;
            swp.cx = swp.cy = 0;
            swp.fl = SWP_MOVE | SWP_SHOW | SWP_ACTIVATE | SWP_ZORDER;
            swp.hwndInsertBehind = HWND_TOP;
            WsetRelativePos(hwnd, &swp);
         } // end if
         break;
      } // end case WM_INITDLG
   // a new color has been set through the color wheel ---------------------
      case WM_COLORWHEELCHANGED3:
      case WM_COLORWHEELCHANGED: {
         PCLRAPPDATA pcad = WinQueryWindowPtr(hwnd, 0L);
         pcad->clrCurr.l = (LONG)mp1;
         pcad->is.upd = 0;           // reset update flag
         SetSpins(hwnd, pcad->clrCurr);
         pcad->is.upd = 1;            // set update flag
         // notify new color to owner
         WinSendMsg(pcad->hOwner, msg, mp1, mp2);
         break;
      } // end case WM_COLORWHEELCHANGED
   // spinbutton value has changed -----------------------------------------
      case WM_CONTROL:
         if ((SHORT1FROMMP(mp1) == SPN_RED ||      // if a spin button
              SHORT1FROMMP(mp1) == SPN_GREEN ||    // originated SPBN_CHANGE
              SHORT1FROMMP(mp1) == SPN_BLUE)) {
            PCLRAPPDATA pcad = WinQueryWindowPtr(hwnd, 0L);
            ULONG ul;
            switch (SHORT2FROMMP(mp1)) {
               case SPBN_UPARROW:
               case SPBN_DOWNARROW: pcad->is.spin = 1; break;
               case SPBN_ENDSPIN: pcad->is.spin = 0;
               case SPBN_CHANGE:
                  if (pcad->is.spin || !pcad->is.upd) break;
                  WinSendMsg(HWNDFROMMP(mp2), SPBM_QUERYVALUE,
                             (MPARAM)&ul, MPFROM2SHORT(0, SPBQ_ALWAYSUPDATE));
                  switch (SHORT1FROMMP(mp1)) {
                     case SPN_RED:   pcad->clrCurr.rgb.red = (BYTE)ul; break;
                     case SPN_GREEN: pcad->clrCurr.rgb.grn = (BYTE)ul; break;
                     case SPN_BLUE:  pcad->clrCurr.rgb.blu = (BYTE)ul; break;
                  } /* endswitch */
                  // update colorwheel
                  WinSendDlgItemMsg(hwnd, CWHEEL, pcad->cwSetMsg,
                                    (MPARAM)pcad->clrCurr.l, MPVOID);
                  // notify owner
                  WinSendMsg(pcad->hOwner, pcad->cwChangedMsg,
                             (MPARAM)pcad->clrCurr.l, MPVOID);
                  break;
            } // end switch
         } // end if
         break;
   // user clicked on a PUSHBUTTON -----------------------------------------
      case WM_COMMAND: {
         PCLRAPPDATA pcad = WinQueryWindowPtr(hwnd, 0L);
         switch ((USHORT)mp1) {
            case BTN_OK:            // OK: return current color
               WinDismissDlg(hwnd, pcad->clrCurr.l);
               break;
            case BTN_UNDO:          // UNDO: restore start color
               SetSpins(hwnd, pcad->clrUndo);
               WinSendDlgItemMsg(hwnd, CWHEEL, pcad->cwSetMsg,
                                 (MPARAM)pcad->clrUndo.l, MPVOID);
               break;
            default:                // CANCEL: return start color
               WinSendMsg(pcad->hOwner, pcad->cwChangedMsg,
                          (MPARAM)pcad->clrUndo.l, MPVOID);
               WinDismissDlg(hwnd, pcad->clrUndo.l);
               break;
         } /* endswitch */
         break;
      } // end case WM_COMMAND
   // user closed the "edit color" dialog window ---------------------------
      case WM_CLOSE: {              // return start color
         PCLRAPPDATA pcad = WinQueryWindowPtr(hwnd, 0L);
         WinSendMsg(pcad->hOwner, pcad->cwChangedMsg,
                    (MPARAM)pcad->clrUndo.l, MPVOID);
         WinDismissDlg(hwnd, pcad->clrUndo.l);
         break;
      } // end case WM_CLOSE
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return (MRESULT)FALSE;
}


//=========================================================================\
// SetSpins() : sets the spinbutton values on initialization and when a    |
//              new color is set through the Color Wheel control           |
// parameters: ----------------------------------------------------------- |
// HWND hwnd  : window handle                                              |
// CLR clr    : RGB color structure containing new RGB values              |
// returned value: ------------------------------------------------------- |
// VOID                                                                    |
//=========================================================================/


VOID SetSpins(HWND hwnd, CLR clr) {
   WinSendDlgItemMsg(hwnd, SPN_RED, SPBM_SETCURRENTVALUE,
                     (MPARAM)clr.rgb.red, MPVOID);
   WinSendDlgItemMsg(hwnd, SPN_GREEN, SPBM_SETCURRENTVALUE,
                     (MPARAM)clr.rgb.grn, MPVOID);
   WinSendDlgItemMsg(hwnd, SPN_BLUE, SPBM_SETCURRENTVALUE,
                     (MPARAM)clr.rgb.blu, MPVOID);
}
