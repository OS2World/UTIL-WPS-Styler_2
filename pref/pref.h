// pref.h

#ifndef PREF_H_
   #define PREF_H_
   #define INCL_WIN
   #define INCL_DOSMISC
   #define INCL_GPI
   #define INCL_DOSMODULEMGR
   #define INCL_DOSSEMAPHORES
   #define INCL_DOSPROCESS
   #define INCL_SHLERRORS
   #include <os2.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <winutil.h>
   #include <afcuti00.h>
   #include <afcutb00.h>
   #include <images.h>
   #include "stlrDefs.h"
   #include "stlrProfile.h"
   #include "stlrOptions.h"
   #include "shddraw.h"
   #include "stlrWinID.h"
   #include "msgs.h"
   #include "commondlg.h"


// procedures imported from smartwin.dll ------------------------------------

typedef PSMWVER (*PFN_VERSION)(VOID);
typedef POPTIONS (*PFN_OPTIONS)(VOID);
typedef HWND (*PFN_OBJECTHWND)(VOID);
typedef PHWND (*PFN_PREFHWND)(VOID);
typedef HINI (*PFN_OPENPROFILE)(VOID);
typedef HBITMAP (*PFN_HBMPFROMDATA)(HPS, PBYTE, PSIZEL);
typedef HBITMAP (*PFN_HBMPFROMMODULE)(HPS, HMODULE, ULONG, PSIZEL);
typedef BOOL (*PFN_BMPDATA)(HINI, PSZ, PBYTE);
typedef HBITMAP (*PFN_HBMPOFSHADE)(HPS, PSHDCREATE, PTBARAIOPT);
//typedef HBITMAP (*PFN_HBMP)(HINI, PSZ, PBYTE, HPS, PSIZEL, HMODULE, ULONG);


// obsolete :
//typedef HBITMAP (GETSHADE)(HWND, PSHDCREATE);
//typedef GETSHADE * PGETSHADE;
//typedef PSHDCREATE (HSHADE)(PTBAROPT, PSIZEL);
//typedef HSHADE * PHSHADE;
//typedef PSHDCREATE (VSHADE)(PTBAROPT, PSIZEL);
//typedef VSHADE * PVSHADE;


// imported procedures ordinals ---------------------------------------------

#define OFN_VERSION                  3
#define OFN_OPTIONS                  5
#define OFN_OBJECTHWND               6
#define OFN_PREFHWND                 7
#define OFN_OPENPROFILE              8
#define OFN_HBMPFROMDATA            12
#define OFN_HBMPFROMMODULE          13
#define OFN_BMPDATA                 14
#define OFN_HBMPOFSHADE             15
//#define OFN_HBMP                    16

// obsolete:
#define OFN_GETSHADE                 11
#define OFN_HSHADE                   12
#define OFN_VSHADE                   13

// macro to get the procedure address
#define getStlrProc(ordinal, pFunc) \
   (DosQueryProcAddr(p->hmod, (ordinal), NULL, (PFN *)(pFunc)))

// STRINGHE INCLUSE NEL MODULO ESEGUIBILE ----------------------------------
#define SZERR_MAINALL    "Main allocation failure!"
#define SZERR_OBJTHREAD  "Failed to start working thread!"
#define SZERR_LOADRES    "Failed to load resources!"
#define SZ_MODULENAME    "SMARTWIN"   // SMARTWIN.DLL

#define CC_FRAMESAMPLE   "ACCFrameSample"  // controlli superclassati
// stringhe mouse tasiera e combinazioni
#define SZ_CMP1           " 1"
#define SZ_CMP2           " 2"
#define SZ_CMP3           " 3"
#define SZ_CMP4           " 4"
#define SZ_CMP5           " 5"
#define SZ_CMP6           " 6"
#define SZ_CMP7           " 7"
#define SZ_CMP8           " 8"
#define SZ_CMP9           " 9"
#define SZ_CMP10          " 10"
#define SZ_CMP11          " 11"
#define SZ_CMP12          " 12"
#define SZ_FKEY           " F"
#define CFKEYS          15     // F1-F12 + Del, Ins, backspace
#define CMOUBTNS         3     // x ora solo 1-3 senza combinazioni


// messaggi privati ("WM_USER + 10, 11, 12, 13" riservati!!!!)
#define WM_BUBBLEHELP        WM_USER        // help contestuale
#define OWM_BSDLBOXFILL      WM_USER + 1    // riempimento listbox bitmap
#define OWM_BSDIMPORT        WM_USER + 2    // import bitmap
#define WM_DLGDISMISS        WM_USER + 3    // chiusura dialogo
#define OWM_DEFALL           WM_USER + 4    // avvia settaggio dati default
                                            // per tutte le pagine
#define WM_QUERYISTANCE      WM_USER + 97   // controlla istanza gia' attiva

// usato x mostrare messaggi di errore nella statusbar
#define OWM_SHOWERROR        WM_USER + 5

//#define WM_FSMPLCMD          WM_USER + 10   // WM_COMMAND child framesample

#define TID_CONTEXTHELP  100

// indice tipo di bitmap
#define BMPATBAR                 0
#define BMPITBAR                 1
#define BMPSMENU                 2
#define BMPCLOSE                 3
#define BMPHIDE                  4
#define BMPMIN                   5
#define BMPREST                  6
#define BMPMAX                   7
#define BMPROLL                  8
#define BMPUNROLL                9
#define BMPHTBAR                 10
#define BMPUNHTBAR               11
#define TOTBMP                   12        // totale bitmaps

//==========================================================================\
// dati identificazione pagine notebook                                     |
//==========================================================================/


#define BFA_COMMON         BFA_PAGEDATA | BFA_PAGEFROMDLGRES | BFA_STATUSLINE
#define BKA_COMMON         BKA_AUTOPAGESIZE | BKS_TABTEXTCENTER | BKA_STATUSTEXTON

#define IDPG_COVER               0  // pagina copertina
#define IDPG_GENERAL             1  // preferenze generali
#define IDPG_GEN_EXC             2  // eccezioni generali
#define IDPG_MAINWIN             3  // pagina Main windows
#define IDPG_MAINWIN_EXC         4  // eccezioni pagina Main windows
#define IDPG_TBAR_ACTWIN         5  // titlebar pagina attiva
#define IDPG_TBAR_INACTWIN       6  // titlebar pagina inattiva
#define IDPG_TBARG_EXC           7  // eccezioni titlebar
#define IDPG_TBAR_HIDING         8  // titlebar hiding
#define IDPG_TBARH_EXC           9  // eccezioni titlebar hiding
#define IDPG_ROLLWIN            10  // window rolling
#define IDPG_WROLL_EXC          11  // eccezioni window rolling
#define IDPG_MAXWIN             12  // finestre massimizzate
#define IDPG_MAXWIN_EXC         13  // eccezioni finestre massimizzate
#define IDPG_SIZEMOVE           14  // opzioni movimento spostamento finestre
#define IDPG_SZMOVE_EXC         15  // eccezioni size/move
#define IDPG_TXTCTLGEN          16  // opzioni controlli testo generiche
#define IDPG_TXTG_EXC           17  // eccezioni generiche controlli testo
#define IDPG_VIOWIN             18  // opzioni finestre VIO
#define IDPG_VIO_EXC            19  // eccezioni finestre VIO
#define IDPG_MOUSE_GENOPTS      20  // mouse opzioni generali
#define IDPG_MOUSEG_EXC         21  // eccezioni mouse generiche
#define IDPG_MOUSE_FUNCS        22  // funzioni eseguite con mouse
#define IDPG_KEYB_FUNCS         23  // funzioni eseguite da tastiera
#define IDPG_KEYB_WKEY          24  // funzioni assegnate alle key win95
#define CPAGES                  25  // numero pagine

// typedef funzioni x settaggi default singole pagine
typedef VOID (SETDEFFUNC)(HWND);
typedef SETDEFFUNC * PSETDEFFUNC;

typedef struct {
   ULONG ulPg;                      // page ID
   HWND hwnd;                       // handle dialogo pagina
   UINT apply  : 1;                 // indica abilitazione bottone apply
   UINT undo   : 1;                 // indica abilitazione bottone undo
   UINT def    : 1;                 // indica abilitazione bottone default
//   UINT init   : 1;                 // indica inizializzazione avvenuta (TRUE)
   UINT        : 0;
   // array di puntatori alle funzioni di settaggio dei valori di default
//   VOID (*defFunc)(HWND hwnd, PVOID pd);
   PSETDEFFUNC defFunc;
} PAGEDATA, * PPAGEDATA;


// usata per dialogo selezione bitmap
#pragma pack(1)
typedef struct {
   ULONG cbyte;              // massima dimensione ammessa x bitmap
   USHORT cx;                // in/out massima larghezza bitmap
   UCHAR idx;                // indice tipo bitmap
   UCHAR set;                // indica quali bottoni spuntare
} BMPSELDLGINFO, * PBMPSELDLGINFO;

//#define STBMP_DEFAULT    0    // handle loaded from the resource DLL
//#define STBMP_INIDATA    1    // handle laoded from SMARTWIN.INI data
//#define STBMP_GLOBAL     2    // global bitmap handle
//#define STBMP_FILE       3    // bitmap loaded from file

// temporarely used to store bitmap data
typedef struct {
//   ULONG flag;                 // default|global|normal bitmap
   CHAR ach[CCHMAXPATHCOMP];   // bitmap file name
   HBITMAP hbmp;               // bitmap handle
   PBYTE p;                    // bitmap data
   INT cb;                     // bitmap data size
} STLRBMP, * PSTLRBMP;

#pragma pack()
//// usata per dialogo selezione bitmap
//typedef struct {
//   UINT idx   : 4;                  // indice tipo bitmap
//   UINT set   : 5;                  // indica quali bottoni spuntare
//   UINT cx    : 9;                  // in/out massima larghezza bitmap
//                                    // bmp preview nella listbox
//   UINT cbyte : 14;                 // massima dimensione ammessa x bitmap
//} BMPSELDLGINFO, * PBMPSELDLGINFO;

// struttura per il settaggio dei colori
typedef struct {
   CLRWHEEL clrw;
   CHAR achtitle[80];
   CHAR achok[32];
   CHAR achundo[32];
   CHAR achcanc[32];
   CHAR achred[24];
   CHAR achgrn[24];
   CHAR achblu[24];
   LONG lclr;
} SETCOLOR, * PSETCOLOR;

typedef struct {
   UINT osver        :  8;
   UINT mainwinOK    :  1;          // finestra principale inizializzata
   UINT mou3btns     :  1;
   UINT active       :  1;
   UINT objOK        :  1;          // stato object window
   UINT closing      :  1;          // programma Š in fase di chiusura
   UINT bsdbusy      :  1;          // bitmap selection dialog working
   UINT bsdend       :  1;          // bitmap selection dialog end
} PREFSTATUS, * PPREFSTATUS;

typedef struct {
   PID pid;                    // ID processo;
   HMODULE hres;               // handle DLL risorse
   HMODULE hmod;               // handle di smartwin.dll (se caricata in memoria)
   HAB hab;
   HMQ hmq;
   // dati thread object window
   TID tidObj;                 // thread ID della object window
   HEV hevObj;                 // handle eventsem avviamento thread
   HAB habObj;                 // hab obj win
   HMQ hmqObj;                 // hmq obj win
   QMSG qmsgObj;               //
   HWND hObj;                  // handle della object window

   HWND hTmrObj;               // handle object window gestore timer
   HWND hMain;                 // handle finestra principale
   HWND hHelp;                 // handle help instance
   HWND hBook;                 // handle controllo notebook
   HWND hSbar;                 // handle controllo status bar
   HWND hCtlHelp;              // handle finestra generatrice evento BUBBLEHELP
   HINI hini;                  // handle file inizializzazione
   RECTL rcl;                  // posiz/dimens minime finestra principale
   WRECTL wrMain;              // posiz/dimens finestra principale
   SIZEL szlLogo;              // dimensioni logo
   WRECTL wrBmpSel;            // posiz/dimens dialogo selezione bitmap
   WRECTL wrFE;                // posiz/dimens dialogo eventi/azioni mouse/keyb
   ULONG cxM;                  // larghezza major tab
   ULONG cxm;                  // larghezza minor tab
   ULONG flFE;                 // flag stile dialogo eventi/azioni mouse/keyb
   ULONG cyTitle;              // coordinate esempio barra titolo finestra
   POINTL ptlBrd;              // spessore bordi finestra
   SWP swpCtls[CMAINCTLS];     // posiz/dimens controlli pag princ
   PREFSTATUS is;              // stato sistema e applicazione
   POPTIONS po;                // indirizzo opzioni globali in shared memory
   OPTIONS ocur;               // opzioni globali correnti
   OPTIONS ou;                 // opzioni globali ad avvio (per undo)
   BMPSELDLGINFO bsdi;         // usata x dialogo selezione bmp
   STLRBMP abmp[TOTBMP];       // styler/2 bitmaps data array
   STLRBMP abmpu[TOTBMP];      // styler/2 bitmaps data array (for UNDO)
//   HBITMAP ahbmp[TOTBMP];      // array handle bmp sfondo titlebar e controlli
//   CHAR achBmp[TOTBMP][256];   // array nome file bitmap barratitolo-controlli
//   CHAR achBmpu[TOTBMP][256];  // array nome file bitmap barratitolo-controlli
//                               // usato x l'UNDO
   CHAR impBmp[260];           // percorso importazione bitmap (BmpSelectDlg)
   PAGEDATA pd[CPAGES];            // array page ID
   ULONG idxPage;                  // indice pagina corrente nell'array
   CHAR cntxt[256];                // buffer usato x l'help contestuale
   CHAR err[256];                  // buffer usato x i messaggi di errore
   CHAR buf[512];                  // buffer generico
   PSZ fkeys[CFKEYS];              // F1-F12 + Ins-bkspc-del
   PSZ moubtns[CMOUBTNS];          // 1-3 + combinazioni
   char bksp[32];                  // stringa "backspace"
   char del[32];                   // stringa "delete"
   char ins[32];                   // stringa "insert"
} APPDATA, * PAPPDATA;

// struttura usata per caricare una bitmap da un file
typedef struct {
   FILEDLG fdlg;
   FILESTATUS3 fs;
} LOADBMPFILE, * PLOADBMPFILE;

extern PAPPDATA p;
// procedures imported from smartwin.dll
extern PFN_PREFHWND stlrFnPrefHwnd;
extern PFN_OPTIONS stlrFnOptions;
extern PFN_OPENPROFILE stlrFnOpenProfile;
extern PFN_OBJECTHWND stlrFnObjectHwnd;
extern PFN_VERSION stlrFnVersion;
extern PFN_HBMPFROMDATA stlrHbmpFromData;
extern PFN_HBMPFROMMODULE stlrHbmpFromModule;
extern PFN_BMPDATA stlrBmpDataFromPrf;
extern PFN_HBMPOFSHADE stlrHbmpOfShade;

//extern PGETSHADE strlFnShade;
//extern PHSHADE stlrFnHShade;
//extern PVSHADE stlrFnVShade;


//==========================================================================\
// dati relativi a smartwin.dll                                             |
//==========================================================================/


// struttura x definire il carattere 9.WarpSans

typedef struct {
   ULONG ulSize;
   ULONG ulIdx;
   ULONG ulLength;
   CHAR achFont[11];
} WARPSANS;

#include "macros.h"
// funzioni

// cover.c
MRESULT EXPENTRY CoverPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

// dlgwin.c
MRESULT EXPENTRY MainWinOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID MainWinDefault(HWND hwnd);

// exceptions.c
MRESULT EXPENTRY ExceptionsProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

// general.c
MRESULT EXPENTRY GeneralOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID GeneralApplyAll(VOID);
VOID GeneralDefault(HWND hwnd);

// hidetitle.c
MRESULT EXPENTRY TitleBarHideOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID TitleBarHideDefault(HWND hwnd);

// initnbk.c
BOOL FillNotebook(VOID);
VOID ShowNotebook(VOID);

// maxwin.c
MRESULT EXPENTRY MaxWinOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID MaxWinDefault(HWND hwnd);

// mouseopt.c
MRESULT EXPENTRY MouseOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID MouseOptDefault(HWND hwnd);

// object.c
BOOL StartObjectThread(VOID);

// sizemove.c
MRESULT EXPENTRY SizeMoveOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID SizeMoveDefault(HWND hwnd);

// titlebar.c
MRESULT EXPENTRY TitleBarMainOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID TbarMainOptApply(HWND hwnd);
VOID TbarMainOptDefault(HWND hwnd);

// txtctlgen.c
MRESULT EXPENTRY TxtCtlGenOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID TxtCtlGenDefault(HWND hwnd);

// util.c
VOID SetCtlCompTxt(HWND hwnd, ULONG ulBtn, PSZ pszTerm, PSZ pszData);
INT ReadBmpFile(PSZ pszFile, PBYTE buf);
ULONG ACCWinDlgBox(HWND hOwnr, PFNWP pfnwp, ULONG ulId, PVOID pb);
VOID SetMouEventTxt(ULONG idstr, ULONG ulMou, ULONG flKey, HWND hwnd, ULONG idctl);
VOID SetKbdEventTxt(ULONG idstr, ULONG ulVK, ULONG flKey, HWND hwnd, ULONG idctl);
LONG hwnd2pageIdx(HWND hwnd, PPAGEDATA ppd);
VOID EnableToolbarApplyBtn(BOOL fl);
VOID EnableToolbarUndoBtn(BOOL fl);
VOID ResetTBarFont(HWND hwnd, PSZ pszFont);
ULONG GetStr(ULONG id, PSZ buf, ULONG cch);
ULONG MsgBox(HWND hwnd, ULONG ulid, ULONG fl);
ULONG SetCtlTxt(HWND hwnd, ULONG idctl, ULONG idstr);
ULONG SetWinTxt(HWND hwnd, ULONG idstr);
ULONG ShowErr(ULONG iderr);
VOID BuildVersionString(HWND hwnd);
HAPP WstartApp(HWND hwnd, PSZ pszAppl, PSZ pszParm, PSZ pszPath, ULONG fl);
BOOL WshowInf(HWND hwnd, ULONG idinf, ULONG idarg);
ULONG setFont(HWND hwnd, ULONG istrs, PSZ pszfont);
VOID setparown(HWND hwnd);
VOID stlrCloseProfile(VOID);
VOID bitmapSizeFromHbmp(HBITMAP hbmp, PSIZEL psize);
VOID prfStoreBitmapData(ULONG idx);
VOID resetGlobalBitmap(HPS hps, INT idx);
VOID featureBtnSetCmd(HWND hwnd, ULONG id);
VOID featureBtnSetBmp(HWND hwnd, HPS hps, ULONG idBmp);
HINI openProfile(BOOL bHandleError);
VOID freeBmpRes(ULONG idx, BOOL primary);
ULONG undoFrameCtrlBmp(HWND hwnd, HPS hps, ULONG idBmp, ULONG flag);
ULONG defaultFrameCtrlBmp(HWND hwnd, HPS hps, ULONG idBmp, ULONG flag);
VOID initHelp(VOID);
VOID endHelp(VOID);


// viowin.c
MRESULT EXPENTRY VIOwinOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID VIOwinDefault(HWND hwnd);

// winkey.c
MRESULT EXPENTRY WinKeyDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID WinKeyDefault(HWND hwnd);

// wroll.c
MRESULT EXPENTRY WinRollOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID WinRollDefault(HWND hwnd);

// MRESULT EXPENTRY (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);


#endif  // ifndef PREF_H_
