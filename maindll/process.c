//===========================================================================
// process.c: procedures automatically called by the system whenever
//            the first message queue of a process is created.
//===========================================================================


#pragma strings(readonly)

// includes -----------------------------------------------------------------

#include "stlrMain.h"
#include "stlrError.h"
#include <umalloc.h>
#include <stdlib.h>


// prototypes ---------------------------------------------------------------

VOID _Export APIENTRY stlrProcessInit(VOID);
VOID APIENTRY processExitHandler(ULONG ul);
BOOL checkExceptions(PSZ pszExe, PSTLREXCEPTIONS pexc);
LONG lookUpException(PBYTE pszExc, PSZ pszKey, PSZ pszExe);
BOOL cmpExe(PSZ pszexe, PSZ pszcmp);
BOOL meminit(VOID);
BOOL memterm(VOID);
void* heapIncrease(Heap_t uhp, size_t* plen, int* pfl);
void heapDecrease(Heap_t uhp, PVOID p, size_t size);


// data segment definition --------------------------------------------------

#pragma data_seg(STLRINSTANCE_DATA)


// global variables ---------------------------------------------------------

PROCDATA pd;
static PVOID pbasemem;
static Heap_t hp;
static volatile INT isem;


// definitions --------------------------------------------------------------


#define RNDUP(n, m)   (((ULONG)(n) + (m) - 1) & ~((m) - 1))

// RAM semaphore lock/unlock flags
#define HEAP_LOCK    1
#define HEAP_UNLOCK  0

#define CB_HEAPBLOCK     0x10000


//===========================================================================
// This is loaded by the system via the OS2.INI SYS_DLLS - LoadPerProcess
// method as it is exported as ordinal number is 1.
// - initialize the process heap
// - get the process data
// - check if this is the PROTSHELL process
// - check if the program is listed in the exceptions
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID _Export APIENTRY stlrProcessInit(VOID) {
   CHAR buf[CCHMAXPATH];
   PTIB ptib;

   if (!meminit()) goto error0;
   // ottiene informazioni relative ai dati del processo

   DosGetInfoBlocks(&ptib, &pd.ppib);
// errore non possibile : "DosGetInfoBlocks returns no values. "
//   if (DosGetInfoBlocks(&ptib, &pd.ppib)) {
//      stlrlog(IDERR_INFOBLOCKS);
//      goto error;
//   } /* endif */
   /*-*/ dbgPrintf2("\n-----> start process: %.200s\n", pd.ppib->pib_pchcmd);
   buf[0] = 0;

   // controlla se Š il primo processo di PMSHELL.EXE --------------------
   if (!g.gpd.WPS0) {
      pd.type = PROC_SHELL1;
      g.gpd.WPS0 = pd.ppib->pib_ulpid;
      // se errore di lettura eccezioni o eccez gen command-win e win-OS2
      if (!checkExceptions(SZPMSH_COMMAND, &pd.we) || pd.we.gen)
         goto error1;                     // libera memoria e termina
   // se Š il secondo processo della WPS ---------------------------------
   } else if (!g.gpd.WPS1 &&
              (pd.ppib->pib_ulppid == g.gpd.WPS0) &&
              cmpExe(pd.ppib->pib_pchcmd, "PMSHELL.EXE")) {
      pd.type = PROC_SHELL2;
      g.gpd.WPS1 = pd.ppib->pib_ulpid;
      // se errore di lettura eccezioni o eccez genPMSHELL o folder
      if (DosQueryModuleName(pd.ppib->pib_hmte, CCHMAXPATH, buf) ||
          !checkExceptions(buf, &pd.we) ||
          pd.we.gen)
         goto error1;                     // libera memoria e termina
   // se Š un processo ordinario -----------------------------------------
   // AGGIUNGERE CODE PER RILEVARE PROCESSI ODIN
   } else {
      // se errore di lettura eccezioni o eccez generale
      if (DosQueryModuleName(pd.ppib->pib_hmte, 260, buf) ||
          !checkExceptions(buf, &pd.we) ||
          pd.we.gen)
         goto error1;                     // libera memoria e termina
   } // end if

   /*-*/ dbgPrintf7("      PID:%d, PPID:%d, WPS0:%d, WPS1:%d\n"
                    "      %s\n"
                    "      %s\n",
                    pd.ppib->pib_ulpid, pd.ppib->pib_ulppid,
                    g.gpd.WPS0, g.gpd.WPS1, pd.ppib->pib_pchcmd, buf);

   // ottiene handle HWND_DESKTOP
   g.hwnd.desktop = WinQueryDesktopWindow(NULLHANDLE, NULLHANDLE);
   // there is no need to register the exit handler for the PROTSHELL process

   if (pd.type != PROC_SHELL1) {
      // registra funzione di clean-up da eseguire al termine del processo
      if (DosExitList(EXLST_ADD | 0x0000AA00,
                      (PFNEXITLIST)processExitHandler)) {
         stlrlog(IDERR_REGEXITFUNC);
         goto error1;
      } /* endif */
   } /* endif */

   return;          // terminazione regolare

error1:
   memterm();
error0:
   pd.ppib = NULL;
}


//===========================================================================
// Called by the exit handler:
// - free the resource module.
// - free the heap resources.
// Parameters --------------------------------------------------------------
// ULONG ignored : ignored parameter.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID APIENTRY processExitHandler(ULONG ul) {
   // se terminazione secondo processo PMSHELL resetta flag
   if (pd.type == PROC_SHELL2) {
      g.gpd.WPS1 = 0;
      g.hwnd.deskFldr = NULLHANDLE;
   } // endif
   memterm();
   DosExitList(EXLST_EXIT, (PFNEXITLIST)NULL);
   ul;
}


//===========================================================================
// Return the global process data.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// PPROCDATA : current process data
//===========================================================================

PPROCDATA stlrProcessData(VOID) {
   return &pd;
}


//===========================================================================
// Check if some Styler/2 features must be ignored for the current process.
// Parameters --------------------------------------------------------------
// PSZ pszExe           : executable name.
// PSTLREXCEPTIONS pexc : (output) list of features which must be ignored
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================
static
BOOL checkExceptions(PSZ pszExe, PSTLREXCEPTIONS pexc) {
   PSZ pszExc;
   BOOL rc = FALSE;
   LONG l;

   // se presente una qualsiasi eccezione alloca buffer x leggere dati
   if (!(o.gen.exc || o.dlg.exc || o.tb.exc || o.tbh.exc ||
       o.roll.exc || o.maxwin.exc || o.sizpos.exc || o.txt.exc ||
       o.vio.exc || o.mou.exc))
      return TRUE;
   if (!(pszExc = memAlloc(0x10000))) {
      stlrlog(IDERR_CHKEXCALLOC);
      return rc;
   } /* endif */
   // eccezioni generali
   if (o.gen.exc) {
      if ((l = lookUpException(pszExc, SZEXC_GENERAL, pszExe)) < 0)
         goto end;
      pexc->gen = l;
   } // end if
   if (o.dlg.exc) {
      if ((l = lookUpException(pszExc, SZEXC_MAINWIN, pszExe)) < 0)
         goto end;
      pexc->frame = l;
   } // end if
   if (o.tb.exc) {
      if ((l = lookUpException(pszExc, SZEXC_TBARGEN, pszExe)) < 0)
         goto end;
      pexc->tbar = l;
   } // end if
   if (o.tbh.exc) {
      if ((l = lookUpException(pszExc, SZEXC_TBARHIDE, pszExe)) < 0)
         goto end;
      pexc->tbarhide = l;
   } // end if
   if (o.roll.exc) {
      if ((l = lookUpException(pszExc, SZEXC_WINROLL, pszExe)) < 0)
         goto end;
      pexc->wroll = l;
   } // end if
   if (o.maxwin.exc) {
      if ((l = lookUpException(pszExc, SZEXC_MAXWIN, pszExe)) < 0)
         goto end;
      pexc->maxwin = l;
   } // end if
   if (o.sizpos.exc) {
      if ((l = lookUpException(pszExc, SZEXC_SZMOVE, pszExe)) < 0)
         goto end;
      pexc->sizemove = l;
   } // end if
   if (o.txt.exc) {
      if ((l = lookUpException(pszExc, SZEXC_TXTCTL, pszExe)) < 0)
         goto end;
      pexc->txtctl = l;
   } // end if
   if (o.vio.exc) {
      if ((l = lookUpException(pszExc, SZEXC_VIOWIN, pszExe)) < 0)
         goto end;
      pexc->VIOwin = l;
   } // end if
   if (o.mou.exc) {
      if ((l = lookUpException(pszExc, SZEXC_MOUSE, pszExe)) < 0)
         goto end;
      pexc->mouse = l;
   } // end if
   rc = TRUE;
end:
   memFree(pszExc);
   return rc;
}


//===========================================================================
// Read 'pszKey' data in the 'pszExc' buffer and search an occurrence of
// 'pszExe'.
// Parameters --------------------------------------------------------------
// PBYTE pszExc : OS2.INI data buffer.
// PSZ pszKey   : OS2.INI key string.
// PSZ pszExe   : executable name to look for.
// Return value ------------------------------------------------------------
// LONG: -1/0/+1 (error/no exception found/exception found).
//===========================================================================
static
LONG lookUpException(PBYTE pszExc, PSZ pszKey, PSZ pszExe) {
   ULONG ul = 0x10000;
   ULONG cb;

   memset((PVOID)pszExc, 0, 0x10000);
   if (!PrfQueryProfileData(HINI_USER, OS2INIAPPL, pszKey, pszExc, &ul)) {
      stlrlog(IDERR_CHKEXCREAD);
      return -1; // errore
   } // end if
   if (ul) {
      while (*pszExc) {
         cb = strlen(pszExc) + 1;
         if (!memcmp(pszExc, pszExe, cb)) return 1;
         pszExc += cb;
      } /* endwhile */
   } /* endif */
   return 0;                  // nessuna eccezione corrispondente
}


//===========================================================================
// compara il nome di un eseguibile che pu• eventualmente includere il path
// con quello del semplice eseguibile
// Parameters --------------------------------------------------------------
// PSZ pszexe: nome eseguibile eventualmente incluso path
// PSZ pszcmp: nome eseguibile di paragone
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
// BOOL TRUE: le due stringhe si riferiscono allo stesso eseguibile
//===========================================================================
static
BOOL cmpExe(PSZ pszexe, PSZ pszcmp) {
   PSZ psz = strrchr(pszexe, '\\');
   if (!psz) psz = pszexe;
   else ++psz;
   return (WCS_EQ == WinCompareStrings(0, 0, 0, psz, pszcmp, 0));
}


//===========================================================================
// Memory management.
// The user allocation functions are used to create an heap and manage
// memory allocation from it.
//===========================================================================

//===========================================================================
// Initialize the heap used by _umalloc.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================
static
BOOL meminit(VOID) {
   if (DosAllocMem(&pbasemem, CB_HEAPBLOCK, PAG_READ | PAG_WRITE | PAG_COMMIT) ||
       (NULL == (hp = _ucreate(pbasemem, CB_HEAPBLOCK, _BLOCK_CLEAN,
                              _HEAP_REGULAR, heapIncrease, heapDecrease))) ||
       _uopen(hp)) {
      stlrlog(IDERR_INITPROCHEAP);
      return FALSE;
   } /* endif */
   return TRUE;
}


//===========================================================================
// Free the resources used for the process heap.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================
static
BOOL memterm(VOID) {
   return !(_uclose(hp) || _udestroy(hp, _FORCE) || DosFreeMem(pbasemem));
}


//===========================================================================
// Callback procedure used to increase the user heap size.
// Return value ------------------------------------------------------------
// PVOID newly allocated additional heap
//===========================================================================
static
void* heapIncrease(Heap_t uhp, size_t* plen, int* pfl) {
   PVOID p = NULL;
   *plen = (size_t)RNDUP(*plen, CB_HEAPBLOCK);
   *pfl = _BLOCK_CLEAN;
   DosAllocMem(&p, *plen, PAG_COMMIT | PAG_READ | PAG_WRITE);
   return p;
}


//===========================================================================
// Callback procedure used to decrease the user heap size.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
void heapDecrease(Heap_t uhp, PVOID p, size_t size) {
   DosFreeMem(p);
}


//===========================================================================
// heapLock : suspends the current thread, giving CPU time to other threads,
//            until the HEAP access semaphore is unlocked by the thread
//            currently owning it.
// Parameters --------------------------------------------------------------
// HAB hab   : anchor block handle
// HWND hwnd : window handle
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID heapLock(HAB hab, HWND hwnd) {
   QMSG qmsg;
   while (__lxchg(&isem, HEAP_LOCK)) {
      DosSleep(1);
      while (WinPeekMsg(hab, &qmsg, hwnd, 0, 0, PM_REMOVE))
         WinDispatchMsg(hab, &qmsg);
   } /* endwhile */
}


//===========================================================================
// heapUnlock : unlocks the integer used to serialize the HEAP access
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID heapUnlock(VOID) {
   __lxchg(&isem, HEAP_UNLOCK);
}


//===========================================================================
// Allocates memory as malloc() without serializing the heap access.
// Parameters --------------------------------------------------------------
// ULONG cb : size of storage to be allocated.
// Return value ------------------------------------------------------------
// PVOID : allocated storage address or NULL in case of error.
//===========================================================================

PVOID memAlloc(ULONG cb) {
   return _umalloc(hp, cb);
}


//===========================================================================
// Allocates memory as malloc() serializing the heap access.
// Parameters --------------------------------------------------------------
// ULONG cb : size of storage to be allocated.
// Return value ------------------------------------------------------------
// PVOID : allocated storage address or NULL in case of error.
//===========================================================================

PVOID memLkAlloc(ULONG cb) {
   PVOID pv = NULL;
   if (!cb) return NULL;
   // if the RAM semaphore is locked gives CPU time to the other threads
   // so they can finish their job and release the semaphore
   while (__lxchg(&isem, HEAP_LOCK)) DosSleep(1);
   pv = _umalloc(hp, cb);
   __lxchg(&isem, HEAP_UNLOCK);
   return pv;
}


//===========================================================================
// Release the storage allocated via memLkAlloc.
// Parameters --------------------------------------------------------------
// PVOID : address of the storage to be freed.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID memLkFree(PVOID pv) {
   if (pv) {
      while (__lxchg(&isem, HEAP_LOCK)) DosSleep(1);
      free(pv);
      __lxchg(&isem, HEAP_UNLOCK);
   } /* endif */
}


//===========================================================================
// memLkRealloc : exported multithread memory allocation function
//                (works as realloc)
//                Serialized HEAP access.
// Parameters --------------------------------------------------------------
// PVOID p  : address to be reallocated.
// ULONG cb : count of bytes of memory to be allocated
// Return value ------------------------------------------------------------
// PVOID : address of allocated memory or NULL in case of error
//===========================================================================

PVOID memLkRealloc(PVOID p, ULONG cb) {
   // if the RAM semaphore is locked gives CPU time to the other threads
   // so they can finish their job and release the semaphore
   while (__lxchg(&isem, HEAP_LOCK)) DosSleep(1);
   realloc(p, cb);
   __lxchg(&isem, HEAP_UNLOCK);
   return p;
}


//===========================================================================
// Minimize the heap usage (not-serialized HEAP access).
// Parameters --------------------------------------------------------------
// VOID.
// Return value ------------------------------------------------------------
// VOID.
//===========================================================================

VOID memHeapMin(VOID) {
   _uheapmin(hp);
}
