// commonbs.h

#ifndef COMMONSB_H_
   #define COMMONSB_H_

#define DOS_INFO   0x100

#define ALLFILES                    FILE_ARCHIVED | FILE_DIRECTORY |\
                                    FILE_SYSTEM | FILE_HIDDEN |\
                                    FILE_READONLY 
#define CBFINFO           (sizeof(FILESTATUS3))
#define FLFACCLIMIT       (FILE_READONLY | FILE_SYSTEM | FILE_HIDDEN)

extern ULONG ulsem;

// struttura usata da ProcessTree

//==========================================================================\
// Processa (opzionalmente in modo ricorsivo) il contenuto di una directory.|
// La funzione usa la struttura PROCESSTREEDATA:                            |
// typedef struct _PROCESSTREEDATA {                                        |
//    FILEFINDBUF3 fBuf;       // dati file trovato                         |
//    CHAR achPath[260];       // file specifiche file da ricercare         |
//    CHAR achMask[80];        // maschera di ricerca                       |
//    ULONG flfind;            // attributi file da ricercare               |
//    PVOID pdata;             // user data                                 |
//    LONG (* pffile)(struct _PROCESSTREEDATA * pptd);    // funzione       |
//    LONG (* pfdirpre)(struct _PROCESSTREEDATA * pptd);  // funzione       |
//    LONG (* pfdirpost)(struct _PROCESSTREEDATA * pptd); // funzione       |
// } PROCESSTREEDATA, * PPROCESSTREEDATA;                                   |
// Quando la funzione viene chiamata in modo ricorsivo Š necessario         |
// specificare la maschera di ricerca in achMask, mentre achPath deve       |
// contenere il path completo di partenza seguito da "\\*".                 |
// In tal caso, per ogni directory trovata, viene modificato achPath        |
// aggiungendo il nome della directory seguito da "\\*", viene eseguita     |
// la funzione pfdirpre() (se non NULL), viene richiamata ricorsivamente    |
// ioProcessTree() e infine viene richiamata la funzione pfdirpost()        |
// (se non NULL), mentre per ogni file trovato, se questo soddisfa la       |
// maschera di ricerca achMask, viene richiamata la funzione pffile.        |
// Nel modo non ricorsivo per ogni file e directory soddisfacente la        |
// maschera achMask viene richiamato pffile()                               |
// Tutte le funzioni (pffile, pfdirpre, pfdirpost) devono restituire        |
// il numero di file processati (>= 0) oppure -1 in caso di errore.         |
// valore restituito:                                                       |
// LONG: >= 0 indica numero file trovati, < 0 indica errore                 |
//==========================================================================/

typedef struct _PROCESSTREEDATA {
   FILEFINDBUF3 fBuf;       // dati file trovato
   CHAR achPath[260];       // file specifiche file da ricercare
   CHAR achMask[80];        // maschera di ricerca
   ULONG flfind;            // flag ricerca
   PVOID pdata;             // user data
   LONG (* pffile)(struct _PROCESSTREEDATA * pptd);
   LONG (* pfdirpre)(struct _PROCESSTREEDATA * pptd);
   LONG (* pfdirpost)(struct _PROCESSTREEDATA * pptd);
} PROCESSTREEDATA, * PPROCESSTREEDATA;


// various:
PVOID APIENTRY tsmalloc(ULONG cb);
VOID APIENTRY tsfree(PVOID pv);
INT APIENTRY tsheapmin(VOID);
BOOL APIENTRY uExeFullName(PSZ pszPath, ULONG cch);
BOOL APIENTRY uEditExeExt(PSZ pszPath, ULONG cch, PSZ pszExt);
BOOL APIENTRY uDateTime(PSZ pszdate, PSZ psztime);
LONG APIENTRY uGetSval(LONG lID);
ULONG APIENTRY ucrc(PBYTE pb, ULONG cb);
VOID APIENTRY upb2an(PBYTE pin, PBYTE pout, ULONG cb, ULONG cxline);
VOID APIENTRY uan2pb(PBYTE pin, PBYTE pout);

// string utilities
VOID APIENTRY catuch2str(PSZ psz, ULONG ul);
LONG APIENTRY linedel(PSZ pszline);
PSZ APIENTRY lin1str(PSZ psz, PSZ substr);
ULONG APIENTRY memdel(PBYTE pdata, ULONG cbdata, PBYTE pdel, ULONG cbdel,
                      PBYTE psep, ULONG cbsep);
PBYTE APIENTRY memfind(PBYTE pdata, ULONG cbdata, PBYTE pfind, ULONG cbfind,
                       PBYTE psep, ULONG cbsep);
PSZ APIENTRY pszlastchr(PSZ psz, CHAR ch);
BOOL APIENTRY pszwldcmp(char* mask, char* cmp);
char* APIENTRY pszcpy(char* target, char* source);
PSZ APIENTRY pszcpycat(PSZ trg, INT cstr, ...);
ULONG APIENTRY pszncpy(PSZ target, PSZ source, ULONG cch);
int APIENTRY pszicmp(const char* s1, const char* s2);
int APIENTRY psznicmp(const char* s1, const char* s2, unsigned int n);
ULONG APIENTRY strdel(PSZ pszbuf, PSZ pszstr, ULONG cbbuf);
PSZ APIENTRY stristr(PSZ str, PSZ substr, ULONG cb);
LONG APIENTRY strrepl(PSZ ins, PSZ psznew, PSZ endch);
ULONG APIENTRY strins(PSZ psz, PSZ substr, PSZ ins, ULONG cb);

// io utilities
#define ioFNameFromPath(psz)            pszlastchr((psz), '\\')
BOOL APIENTRY ioBackup(PSZ pszFile, PSZ pszExt);
LONG APIENTRY ioCountTree(PSZ path, PSZ pmask, ULONG flattr, BOOL fl);
PSZ APIENTRY ioEditExt(PSZ pszFile, PSZ pszExt);
PSZ APIENTRY ioF2psz(PSZ pszFile, PULONG pcb);
BOOL APIENTRY ioFExists(PSZ pszFile, PULONG pul);
INT APIENTRY ioFilOlder(PFDATE pfds, PFTIME pfts, PFDATE pfdt, PFTIME pftt);
BOOL APIENTRY ioFlushBuf(PSZ pszFile, PSZ pbuf, PSZ* ppend);
BOOL APIENTRY ioMakePath(PSZ pszPath);
LONG APIENTRY ioProcessTree(PPROCESSTREEDATA pptd);
ULONG APIENTRY ioPsz2f(PSZ pszFile, PSZ pdata, ULONG cb);
PSZ APIENTRY ioUniqFName(PSZ pszFile);

// funzioni implementate in lock.obj:
VOID APIENTRY lockincr(PULONG pul);
VOID APIENTRY lockdecr(PULONG pul);
ULONG APIENTRY lockxchng(PULONG pul, ULONG val);

#endif
