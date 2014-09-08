//===========================================================================
// stlrFunctions.h : prototypes of the functions used by the Styler/2 DLL
//
// --2003  - Alessandro Felice Cantatore
//===========================================================================


#ifndef _STLR_FUNCTIONS_H_
   #define _STLR_FUNCTIONS_H_


// main.c -------------------------------------------------------------------

BOOL stlrKbdSkipLoading(VOID);
HINI stlrOpenProfile(VOID);
ULONG stlrMouEventToSkip(ULONG msg);
BOOL stlrSuperclass(PSZ pszClass, PCLASSDATA pcd, PFNWP pfnwpNew,
                    LONG cbWin, ULONG id1, ULONG id2);

// button.c -----------------------------------------------------------------

MRESULT EXPENTRY stlrButtonProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

// combobox.c ---------------------------------------------------------------

MRESULT EXPENTRY stlrComboProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

// entry.c ------------------------------------------------------------------

MRESULT EXPENTRY stlrEntryFieldProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

// export.c ----------------------------------------------------------------

PSMWVER stlrVersion(VOID);
PHWND stlrPrefWindow(VOID);
POPTIONS stlrOptions(VOID);
PGLOBAL stlrGlobals(VOID);
HWND stlrObjectWindow(VOID);
HBITMAP stlrHBmpFromBmpData(HPS hps, PBYTE pbmp, PSIZEL psize);
HBITMAP stlrHBmpFromModule(HPS hps, HMODULE hmod, ULONG id, PSIZEL psize);
BOOL stlrBmpData(HINI hini, PSZ pszKey, PBYTE pdata);
HBITMAP stlrShadeHBmp(HPS hps, PSHDCREATE psc, PTBARAIOPT ptb);
HBITMAP stlrHBmp(HINI hini, PSZ key, PBYTE p, HPS hps,
                 PSIZEL psize, HMODULE hmod, ULONG id);

// frame.c ------------------------------------------------------------------

MRESULT EXPENTRY stlrFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID toggleTitleBarHidingCmd(HWND hwnd, PSTLRFRAME p);
VOID toggleRollCmd(HWND hwnd, PSTLRFRAME p);
MRESULT EXPENTRY stlrWpsFolderProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

// hooks.c ------------------------------------------------------------------

BOOL EXPENTRY postMsgHook(HAB hab, PQMSG pqmsg, USHORT fs);
VOID unhideMouPtr(VOID);
VOID EXPENTRY sendMsgHook(HAB hab, PSMHSTRUCT psmh, BOOL fInterTask);

// listbox.c ----------------------------------------------------------------

MRESULT EXPENTRY stlrListboxProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

// menu.c -------------------------------------------------------------------

MRESULT EXPENTRY stlrMenuProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

// mle.c --------------------------------------------------------------------

MRESULT EXPENTRY stlrMLEproc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

// process.c ----------------------------------------------------------------

PPROCDATA stlrProcessData(VOID);
VOID heapLock(HAB hab, HWND hwnd);
VOID heapUnlock(VOID);
PVOID memAlloc(ULONG cb);
PVOID memLkAlloc(ULONG cb);
VOID memLkFree(PVOID pv);
PVOID memLkRealloc(PVOID p, ULONG cb);
VOID memHeapMin(VOID);

// static.c -----------------------------------------------------------------

MRESULT EXPENTRY stlrStaticProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

// trackframe.c -------------------------------------------------------------

BOOL stlrTrackWindow(HWND hwnd, PFULLWDRAG pfwd, PSTLRFRAME p);
BOOL _System trkhook(HAB hab, PQMSG pqmsg, PFULLWDRAG pfwd);

// titlebar.c : funzioni titlebar -------------------------------------------

MRESULT EXPENTRY stlrTitleBarProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

// utils.c ------------------------------------------------------------------

VOID stlrStopHitTestCheck(VOID);
VOID copyTextToClipboard(HAB hab, PSZ psz);
VOID winTextToClipboard(HWND hwnd);
PSZ getTextFromClipboard(HAB hab);
ULONG logwrite(HFILE hf, PSZ psz);
ULONG psznconvcpy(PSZ target, PSZ source, ULONG cch);
HFILE stlrOpenLog(VOID);
BOOL stlrDateTime(PSZ psz);
BOOL stlrlog(ULONG ulmsg);

// workerthreads.c ----------------------------------------------------------
VOID _System threadMouMovMgr(ULONG ul);
VOID _System threadTimerMgr(ULONG ul);
VOID initResources(HINI hini, BOOL bLoadStrings);



#endif // #ifndef _STLR_FUNCTIONS_H_
