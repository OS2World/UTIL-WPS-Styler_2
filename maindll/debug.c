//==========================================================================\
// debug.c
// funzioni che vengono linkate a smartwin.dll quando Š definito DEBUGGING
// VOID stlrdbgInit(VOID);
// VOID strldbgEnd(VOID);
//==========================================================================/


#pragma strings(readonly)

#include "stlrMain.h"
extern PROCDATA pd;

#ifdef DEBUGGING

//==========================================================================\
// - trova il drive di boot                                                 |
// - apre il file di log per il debugging                                   |
//==========================================================================/

VOID _dbgInit(VOID) {
   ULONG ul;
   CHAR buf[80];
   DosCopy("smwdebug.log", "smwdebug.000", DCPY_EXISTING);
   if (DosOpen("smwdebug.log", &hf, &ul, 0L, FILE_NORMAL,
       OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_REPLACE_IF_EXISTS,
       OPEN_SHARE_DENYNONE | OPEN_ACCESS_READWRITE, NULL)) {
      DosBeep(440, 440);
      return;
   } /* endif */
   logwrite(hf, "------------- Logging started -------------\n");
   strcpy(buf, "00/00/00 - 00:00:00");
   stlrDateTime(buf);
   sprintf(buf + 19, " - Styler/2 Version %d.%d.%d.%d\n",
           g.ver.Mver, g.ver.mver, g.ver.rver, g.ver.bver);
   logwrite(hf, buf);
}


//==========================================================================\
// - terminazione debugging                                                 |
//==========================================================================/

VOID _dbgEnd(VOID) {
   logwrite(hf, "-------------- Logging ended --------------\n");
   DosClose(hf);
}

VOID _dbgCreate1(HWND hwnd, PFRAMECDATA pfd, PCREATESTRUCT pc) {
   CHAR buf[96];
   if (pc) {
      sprintf(buf,
           "\n   CREATE: HWND:0x%08x par:0x%08x own: 0x%08x %.30s\n",
           hwnd, pc->hwndParent, pc->hwndOwner, pc->pszText);
      logwrite(hf, buf);
   } else {
      logwrite(hf, "createstruct na\n");
   } /* endif */
   if (pfd) {
      sprintf(buf, "FrameFlags: %08x\n", pfd->flCreateFlags);
      logwrite(hf, buf);
   } else {
      logwrite(hf, "FrameFlags: not available\n");
   } /* endif */
}

VOID _dbgCreate2(PSTLRFRAME p) {
   CHAR buf[80];
   sprintf(buf, "   PID:%d, HAB:0x%08x\n",
           pd.ppib->pib_ulpid, p->hab);
   logwrite(hf, buf);
}

VOID _dbgCreate3(HWND hwnd, PCREATESTRUCT pc) {
   CHAR buf[80];
   sprintf(buf, "   id: 0x%08x - QWSflags: 0x%08x\n",
           pc->id, WinQueryWindowUShort(hwnd, QWS_FLAGS));
   logwrite(hf, buf);
}

VOID _dbgPrintf(PSZ psz, ...) {
   CHAR buf[300];
   va_list ap;
   va_start(ap, psz);
   vsprintf(buf, psz, ap);
   va_end(ap);
   logwrite(hf, buf);
}

#endif // DEBUGGING
