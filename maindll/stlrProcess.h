//===========================================================================
// stlrProcess.h : process relative data types and definitions
//
// --2003  - Alessandro Felice Cantatore
//===========================================================================


#ifndef _STLR_PROCESS_H_
   #define _STLR_PROCESS_H_

// features exceptions ------------------------------------------------------
typedef struct {
UINT gen            : 1;
UINT frame          : 1;
UINT tbar           : 1;
UINT btns           : 1;  // bottoni barra titolo
UINT tbarhide       : 1;
UINT wroll          : 1;
UINT maxwin         : 1;
UINT sizemove       : 1;
UINT txtctl         : 1;
UINT VIOwin         : 1;
UINT mouse          : 1;
UINT                : 0;
} STLREXCEPTIONS, * PSTLREXCEPTIONS;

// global process data (PROTSHELL-RUNWORKPLACE) -----------------------------
typedef struct {
   PID WPS0;
   PID WPS1;
//   STLREXCEPTIONS wewinos2;
//   STLREXCEPTIONS wefolder;
} GLOBPRCSDATA, * PGLOBPRCSDATA;

// alias used for kind of applications running in the PMSHELL processes: ----
#define SZPMSH_WINOS2             "<win-OS/2>"       // sessioni winOS/2
#define SZPMSH_COMMAND            "<Command window>" // sessioni comando
#define SZPMSH_FOLDER             "<Folder>"         // cartelle

// struttura dati bitmap barra titolo (raddoppiata x barra titolo attiva e no
typedef struct {
   HBITMAP hbmp;
   ULONG cx;
} TBBMP, * PTBBMP;

// process data -------------------------------------------------------------
// this is global within the process (i.e. not shared among processes)
typedef struct {
   PPIB ppib;            // address of Process Information Block
   STLREXCEPTIONS we;    // exceptions for this process
   ULONG type;           // process type
} PROCDATA, *PPROCDATA;

//typedef struct {
//   PPIB ppib;
//   PTIB ptib;
//   WINEXCEPTIONS we;
//   CHAR achMod[260];
//   ULONG cmnupdtime;        // tempo ricezione msg aggiornamento bmp handle
//   TBBMP atb, itb;          // strutture x barra titolo attiva e inattiva
//   ULONG chbmp;             // numero di bitmap handle memorizzate
//   PHBITMAP phbmp;
//   HMODULE hmod;            // handle modulo risorse
//} PROCDATA, *PPROCDATA;


// process types ------------------------------------------------------------

enum {
   PROC_ANY,            // ordinary PM process
   PROC_SHELL1,         // first instance of PMSHELL (PROTSHELL)
   PROC_SHELL2,         // second instance of PMSHELL (RUNWORKPLACE)
   PROC_WIN32           // odin process
} ;


// MACROs -------------------------------------------------------------------

#define memFree(p)              free(p)
#define memRealloc(p, cb)       realloc((p), (cb))

// prototypes ---------------------------------------------------------------

VOID heapLock(HAB hab, HWND hwnd);
VOID heapUnlock(VOID);
PVOID memAlloc(ULONG cb);
PVOID memLkAlloc(ULONG cb);
VOID memLkFree(PVOID pv);
PVOID memLkRealloc(PVOID p, ULONG cb);
VOID memHeapMin(VOID);


// --------------------------------------------------------------------------

#endif // #ifndef _STLR_PROCESS_H_
