//===========================================================================
// main.h :
//
// --2003  - Alessandro Felice Cantatore
//===========================================================================


#ifndef _STLR_MAIN_H_
   #define _STLR_MAIN_H_
   #define INCL_WIN
   #define INCL_GPI
   #define INCL_DOS
   #define INCL_DOSMISC
   #define INCL_DOSMODULEMGR
   #define INCL_DOSFILEMGR
   #define INCL_DOSPROCESS
   #define INCL_DOSNLS

   #include <os2.h>
   #include <stdlib.h>
   #include <stdio.h>
   #include <malloc.h>
   #include <string.h>
   #include <stdarg.h>
   #include <winutil.h>
   #include <stddef.h>
   #include "stlrver.h"
   #include "stlrDefs.h"
   #include "stlrProfile.h"
   #include "stlrOptions.h"
   #include "stlrProcess.h"
   #include "stlrFrame.h"
   #include "stlrTypes.h"
   #include "shddraw.h"
   #include "stlrFunctions.h"
   #include "stlrDebug.h"
   #include "stlrMacros.h"

   #ifndef RC_INVOKED
   #endif

// --------------------------------------------------------------------------
// compilazione condizionale


//#define DEBUGGING   2            // x scrittura su file di log
#define INCLUDEFOLDERSACC
#define INCLVIOENH


// --------------------------------------------------------------------------

#ifdef DEBUGGING
   #include <stdio.h>
#endif


// --------------------------------------------------------------------------

#define SZ_RESMODNAME  "\\STLRES13.DLL"


// --------------------------------------------------------------------------

#define WC_TIMERMGRCLASS "stlrTimerMgrClass"

// --------------------------------------------------------------------------


extern GLOBAL g;
extern OPTIONS o;

#ifdef DEBUGGING
   extern HFILE hf;
#endif



#endif // #ifndef _STLR_MAIN_H_
