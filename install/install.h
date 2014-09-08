// install.h

#ifndef INSTALL_H_
   #define INSTALL_H_
   #define INCL_DOS
   #define INCL_WIN
   #include <os2.h>
   #include <string.h>
   #include <stdlib.h>
   #include <stdio.h>
   #include <winutil.h>
   #include <afcuta00.h>
   #include <afcutb00.h>
   #include <afcuti00.h>
   #include "msgs.h"
   #include "institems.h"
   #include "stlrDefs.h"
   #include "stlrWinID.h"
   #include "stlrOptions.h"
   #include "stlrOldOptions.h"
   #include "stlrProfile.h"

// controllare che in pref.h sia definito allo stesso modo
#define WM_QUERYISTANCE      WM_USER + 97   // controlla istanza gia' attiva

// flag file
#define INSTFLAGFILE       "styler.@"
// LOG file
#define SZ_LOGFILE         "smwsetup.log"
// licenza
#define SZ_INSTLICTXT      "license.txt"

// applicazione in OS2.INI
#define SZ_USER_APP        "Smart Windows"
#define SZ_USER_KEYPATH    "path"
// path smartwin.dll
#define SZ_RUNDLLPATH      ":\\OS2\\DLL\\SMARTWIN.DLL"
#define SZ_RUNDLL1PATH     ":\\OS2\\DLL\\STYLER20.DLL"
// path default installazione
#define SZ_DEFINSTPATH     ":\\Styler2"
// messaggi di errore di inizializzazione:
#define SZERR            "Error!\t"
#define SZERRI_ALLOC     SZERR"Failed to allocate application data structure."
#define SZERRI_OBJWIN    SZERR"Failed to create object window thread."
#define SZERRI_LOADMOD   SZERR"Failed to load resource library."
#define SZERRI_REGCTLS   SZERR"Failed to create controls."
#define SZERRI_CREATWN   SZERR"Failed to create the main window."
#define SZERRI_LOADPROC  SZERR"Procedure load failure."
//#define SZERRI_STOREAD SZERR"Errore registrazione indirizzo dati finestra."

// funzioni ottenute tramite DosQueryProcAddr() ---------------------------
typedef PHWND (EXPENTRY FN_CHKOPENPREF)(VOID);
typedef FN_CHKOPENPREF * PFN_CHKOPENPREF;
#define OFN_OPENPREF                 3


// struttura usata internamente dall'applicazione
typedef struct {
   APPDATA a;
   HWND hwnd;              // handle finestra principale applicazione
   HWND hHelp;
   ULONG idpage;           // indice pagina corrente
   ULONG fl;               // flag installa/refresh/disinstalla
   ULONG flprgrs;          // tipo operazione (monitorata da progressbar)
   ULONG cbspath;          // lunghezza sourcepath
   ULONG cbtpath;          // lunghezza targetpath
   CHAR spath[260];        // source path
   CHAR tpath[260];        // target path
   PFN_CHKOPENPREF pstlrfn;// funzione per ricavare handle programma preferenze
   ULONG cMbDrv[26];       // dimensione in MB x i vari drive presenti
   PLOGFILE plf;           // indirizzo struttura dati file log
   ULONG cerrors;          // conteggio errori non critici
   CHAR boot;              // boot drive
} INSTALLDATA, *PINSTALLDATA;


// il programma di installazione pu• mostrare le seguenti pagine:
#define INSTPG_LOGO    0   // logo applicazione da installare
#define INSTPG_SELECT  1   // selezione installazione/refresh/disinstallazione
#define INSTPG_LIC     2   // testo licenza
#define INSTPG_PATH    3   // selezione path installazione
#define INSTPG_START   4   // scritta clicca su Avanti per iniziare l'install..
#define INSTPG_PRGRS   5   // dialogo progresso
#define INSTPG_LOG     6   // pagina lettura log inst/ref/disinst
// flag azione:
#define INSFL_INST       0x00000001  // presenza file styler.@
#define INSFL_RFRSH      0x00000002  // presenza id oggetti in OS2.INI
#define INSFL_UNINST     0x00000004  // presenza precedente installazione
#define INSFL_RUNDLL     0x00000008  // smartwin.dll presente in \os2\dll
#define INSFL_LOADEDDLL  0x00000010  // smartwin.dll Š caricata in memoria
#define INSFL_USERAPP    0x00000020  // presenza path in OS2.INI
#define INSFL_DESKOBJS   0x00000040  // presenza settaggi desktop objects
// azione selezionata:
#define INSSEL_INST      0x00010000  // installazione
#define INSSEL_RFRSH     0x00020000  // rinfresco oggetti desktop
#define INSSEL_UININST   0x00040000  // disinstallazione
#define INSSEL_REMBMPS   0x00080000  // conferma cancellazione albero bitmaps

// flag varie
#define INSFL_SKIPCTL    0x10000000  // indica di non processare WM_CONTROL

// lista oggetti da ricercare in OS2.INI
#define SZ_DESKOBJLIST  "<SMARTWIN_PROPERTIES>\0<SMARTWIN_PROPERTIES1>\0"    \
                        "<SMARTWIN_README>\0<SMARTWIN_SETUP>\0"              \
                        "<SMARTWIN_REGISTRATION>\0<SMARTWIN_DOCUMENTATION>\0"\
                        "<SMARTWIN_FOLDER>\0"
// macro per cast da PAPPDATA a PINSTALLDATA
#define MKP(x)      ((PINSTALLDATA)(x))

// macro per log procedure installazione/aggiornamento/disinstallazione
#define LOGERR     logAdd(pid->plf, TRUE, 1, ID_INSTPROCERR)
// errore non critico
#define LOGNCERR     logAdd(pid->plf, 0, 1, ID_INSTPROCERR)

// prototipi funzioni
// iniconv.c
VOID updateIni(VOID);
// instaux.c
BOOL instinit(int argc, char** argv, PAPPDATA pad, PWDATA pw);
INT instinitUI(PWDATA pw);
VOID cmdBack(PWDATA pw);
VOID cmdFwd(PWDATA pw);
VOID cmdCanc(PWDATA pw);
// object.c
MRESULT EXPENTRY instobjproc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
// instpages.c
VOID instPgSel(PWDATA pw, PINSTALLDATA pid, BOOL fl);
VOID instPgLic(PWDATA pw, PINSTALLDATA pid, BOOL fl);
VOID instPgPath(PWDATA pw, PINSTALLDATA pid, BOOL fl);
VOID instPgConf(PWDATA pw, PINSTALLDATA pid, BOOL fl);
VOID instPgStart(PWDATA pw, PINSTALLDATA pid);
VOID instPgLog(PWDATA pw, PINSTALLDATA pid);
// util.c
VOID progress(HWND hp, HWND ho, PINSTALLDATA pid, ULONG id);
VOID checkPath(HWND hwnd, PINSTALLDATA pid, BOOL fl);
BOOL startProcessing(PINSTALLDATA pid, HWND hprgs);
BOOL endProcessing(PINSTALLDATA pid, HWND hprgs, BOOL fl);
BOOL instprfedit(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps,
                 PSZ app, PSZ key, PSZ val);
ULONG instinstcsteps(PINSTALLDATA pid);
ULONG instrfrshcsteps(PINSTALLDATA pid);
ULONG instuninstcsteps(PINSTALLDATA pid, HWND hprgs);
BOOL instcloseprefapp(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
BOOL instaddprfitems(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
BOOL instdelobsolete(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
BOOL instcopyfiles(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
BOOL instcopytree(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
BOOL instcreatobjs(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
BOOL instdelprfitems(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
BOOL instdeltree(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
BOOL instdelfiles(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
BOOL instdelobjs(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
BOOL instdelinstpath(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);

#endif
