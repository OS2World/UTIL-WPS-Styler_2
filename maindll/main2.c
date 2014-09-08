//===========================================================================
// main2.c: extend PM controls loaded via PMCTLS.DLL
//===========================================================================


#pragma strings(readonly)

// includes -----------------------------------------------------------------

#include "stlrMain.h"
#include "stlrError.h"


// prototypes ---------------------------------------------------------------

ULONG _System stlrInitTerm(ULONG hmod, ULONG flag);
ULONG initialize(VOID);

// global variables ---------------------------------------------------------

PGLOBAL pg;
POPTIONS po;
CLASSDATA cdMLE;


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
         return initialize();
      case 1:
         return 1;
      default:
         return 0L;
   } /* endswitch */
   return 1L;
}


//===========================================================================
// DLL initialization routine.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// ULONG : 1/0 (success/error)
//===========================================================================
static
ULONG initialize(VOID) {
   CLASSINFO ci;

   // termina se DLL gi… caricata in memoria o errore di registrazione
   if (pg) return 0;

   pg = stlrGlobals();
   po = stlrOptions();
   if (!pg || !po->gen.loaded) return 0;

   // abort DLL loading if one of the Ctrl keys is down
   if (stlrKbdSkipLoading()) return 0;

   return (ULONG)stlrSuperclass(WC_MLE, &cdMLE, stlrMLEproc, 0,
                                IDERR_MLECLASS, IDERR_NEWMLE);
}

