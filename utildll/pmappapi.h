// pmappapi.h
// all'offset 0 delle window words delle finestre principali viene memorizzato
// l'indirizzo della struttura wdata. L'indirizzo dei dati utente viene
// memorizzato come membro di WDATA.
// IMPORTANTE: il WM_INITDLG viene processato prima del ritorno da
//             WinLoadDlg

#ifndef PMAPPAPI_H_
   #define PMAPPAPI_H_
   #include <winutil.h>
   #include "pmapi.h"

// flag ricerca dati non presenti nel file INI usata da WgetPrfData
#define GPD_ERROR             ((PVOID)0xffffffff)

// strutture standard usate dall'applicazione ------------------------------

// dimensione buffer help contestuale
#define CBSZHLP   512

// id risorsa stringhe pi— lunghe di 256 byte
#define RCLONGSTRING     301

// struttura dati applicazione condivisi tra tutte le finestre
// questi sono i dati definiti a runtime, mentre i dati di CREATEAPP sono
// costanti definite alla compilazione dell'applicazione stessa
#pragma pack(2)
typedef struct {
   // inizio porzione standard struttura ----------------------------------
   USHORT cb;                  // dimensione struttura
   USHORT fs;                  // riservato per flag aggiuntive
   HAB hab;                    // handle anchor block thread principale
   PID pid;                    // ID processo;
   HMODULE hres;               // handle DLL risorse
   HMQ hmq;                    // handle coda messaggi principale
   HWND hctlhlp;               // handle controllo sotto puntatore mouse
                               // dato usato per l'help contestuale
   HWND hsbar;                 // handle statusbar attiva per messaggi e
                               // help contestuale
   // dati thread object window
   HEV hevObj;                 // handle eventsem avviamento thread
   HAB habObj;                 // hab obj win
   HWND hObj;                  // handle della object window
   ULONG ulsem;                // flag usata per serializzare accesso a
                               // risorse condivise
   int rc;                     // codice da restituire al sistema
   CHAR szhlp[CBSZHLP];        // buffer per help contestuale o messaggi errore
   // fine porzione standard struttura / inizio dati aggiuntivi -----------
} APPDATA, * PAPPDATA;
#pragma pack()


// struttura stato finestra
typedef struct {
   UINT ready       : 1;       // usato per sincronizzazione con object wind
   UINT active      : 1;       // usato per sospendere help contestuale
   UINT enabled     : 1;       // finestra abilitata
   UINT visible     : 1;       // finestra visibile
   UINT max         : 1;       // finestra massimizzata
   UINT min         : 1;       // finestra minimizzata
   UINT             : 0;
} WSTATUS, * PWSTATUS;


// struttura dati dialogo generico con eventuali icona, menubar,
// tabella acceleratori e statusbar
// la funzione pfinit restituisce < 0 in caso di errore altrimenti 1 o 0
// secondo il valore che si vuole sia restituito da WM_INITDLG
#pragma pack(2)

typedef struct _WDATA {
   USHORT cb;                  // dimensioni struttura
   USHORT id;                  // id finestra e risorse (usare stesso id per
                               // dialog template, menu, icona, ecc...
   HWND hwnd;                  // handle finestra dialogo (o client window)
   HWND hown;                  // handle owner dialogo (o frame window)
   HWND hmenu;                 // handle eventuale menubar
   HWND hsbar;                 // handle eventuale statusbar
   HACCEL hacc;                // handle eventuale tabella acceleratori
   HPOINTER hico;              // handle icona applicazione
   PSZ psztxt;                 // titolo finestra (se < 0xffff Š ID stringa)
   PFNWP pf;                   // procedura finestra (client)
   WRECTL wr;                  // posizione dimensioni finestra principale
   ULONG flswp;                // flag SWP_ finestra
   SSIZE smin;                 // dimensioni minime finestra principale
   SSIZE sbrd;                 // dimensioni bordo finestra
   ULONG fcf;                  // flag stili finestra
   PSZ pszfont;                // font default finestra
   WSTATUS is;                 // stato finestra
   INT (*pfinit)(struct _WDATA*);  // funzione di inizializzazione
   BOOL (*pfend)(struct _WDATA*);  // funzione di terminazione
   PAPPDATA pad;               // indirizzo dati generali applicazione
   PVOID pud;                  // indirizzo dati utente
} WDATA, * PWDATA;

#pragma pack()


// struttura creazione applicazione: ----------------------------------------
//       l'ID della finestra statusbar o per l'help contestuale Š sempre 2

#define FID_STSBAR          2

typedef struct _CREATEAPP {
   // funzione analisi parametri e lettura file inizializzazione
   BOOL (*pfinit)(int, char **, PAPPDATA, PWDATA);
   // funzione chiamata al termine per liberazione risorse
   BOOL (*pfterm)(PAPPDATA pad);
   // procedura object window thread secondario
   PFNWP pfnwpobj;
   PSZ pszres;      // nome DLL contenente risorse
   PSZ psztitle;    // titolo finestra principale (solo standard window)
   PPSZ ppszerr;    // array stringhe visualizzate per errori inizializzazione
   ULONG fl;        // flag applicazione
   ULONG cbad;      // dimensione dati applicazione
   PAPPDATA pad;    // struttura dati applicazione
   PWDATA pw;       // dati finestra principale applicazione
} CREATEAPP, * PCREATEAPP;

// typedef funzioni membro struttura applicazione: --------------------------

// funzione inizializzazione applicazione
typedef BOOL (APPINITFN)(int, char **, PAPPDATA, PWDATA);
typedef APPINITFN * PAPPINITFN;

// funzione inizializzazione interfaccia grafica (chiamata in WM_INITDLG)
// o alla creazione della frame window se finestra standard
typedef INT (APPINITUIFN)(PWDATA);
typedef APPINITUIFN * PAPPINITUIFN;

// funzione salvataggio dati finestra chiamata al WM_CLOSE
typedef BOOL (APPSAVEF)(PWDATA);
typedef APPSAVEF * PAAPPSAVEF;

// funzione terminazione applicazione
typedef BOOL (APPTERMFN)(PAPPDATA);
typedef APPTERMFN * PAPPTERMFN;

// struttura per la creazione di una dialog box modale (castabile a WDATA)
#pragma pack(2)

typedef struct _MODDLG {
   USHORT cb;                  // dimensioni struttura
   USHORT id;                  // id finestra e risorse (usare stesso id per
                               // dialog template, menu, icona, ecc...
   HWND hwnd;                  // handle finestra dialogo (o client window)
   HWND hown;                  // handle owner dialogo (o frame window)
   HWND hmenu;                 // handle eventuale menubar
   HWND hsbar;                 // handle eventuale statusbar
   HACCEL hacc;                // handle eventuale tabella acceleratori
   HPOINTER hico;              // handle icona applicazione
   PSZ psztxt;                 // titolo finestra (se < 0xffff Š ID stringa)
   PFNWP pf;                   // procedura finestra (client)
   WRECTL wr;                  // posizione dimensioni finestra principale
   ULONG flswp;                // flag SWP_ finestra
   SSIZE smin;                 // dimensioni minime finestra principale
   SSIZE sbrd;                 // dimensioni bordo finestra
   ULONG fcf;                  // flag stili finestra
   PSZ pszfont;                // font default finsetra
   WSTATUS is;                 // stato finestra
   INT (*pfinit)(struct _MODDLG*);  // funzione di inizializzazione
   BOOL (*pfend)(struct _MODDLG*);  // funzione di terminazione
   PAPPDATA pad;               // indirizzo dati generali applicazione
   PVOID pud;                  // indirizzo dati utente
   USHORT idok;                // OK
   USHORT idcanc;              // cancel
} MODDLG, * PMODDLG;

#pragma pack()


// stili applicazione: ------------------------------------------------------
#define APPLS_OBJWIN     0x00000001    // crea thread secondario con object
                                       // window
#define APPLS_EXTCTLS    0x00000002    // registra controlli aggiuntivi
#define APPLS_DLGWIN     0x00000000    // (default) usa un dialogo come
                                       // finestra principale applicazione
#define APPLS_STDWIN     0x00000008    // usa una standard window come
                                       // finestra principale applicazione
#define APPLS_CONTXTHLP  0x00000020    // l'applicazione mostra i messaggi e
                                       // l'help contestuale nella statusbar
                                       // o in una finestra popup
// stili finestra applicazione

//   #define FCF_TITLEBAR               0x00000001L
//   #define FCF_SYSMENU                0x00000002L
//   #define FCF_MENU                   0x00000004L
//   #define FCF_SIZEBORDER             0x00000008L
//   #define FCF_MINBUTTON              0x00000010L
//   #define FCF_MAXBUTTON              0x00000020L
//   #define FCF_VERTSCROLL             0x00000040L
//   #define FCF_HORZSCROLL             0x00000080L
//   #define FCF_DLGBORDER              0x00000100L
//   #define FCF_BORDER                 0x00000200L
//   #define FCF_SHELLPOSITION          0x00000400L
//   #define FCF_TASKLIST               0x00000800L
//   #define FCF_NOBYTEALIGN            0x00001000L
//   #define FCF_NOMOVEWITHOWNER        0x00002000L
//   #define FCF_ICON                   0x00004000L
//   #define FCF_ACCELTABLE             0x00008000L
//   #define FCF_SYSMODAL               0x00010000L
//   #define FCF_SCREENALIGN            0x00020000L
//   #define FCF_MOUSEALIGN             0x00040000L
//   #define FCF_HIDEBUTTON             0x01000000L
#define FCF_BUBBLEHLP  0x02000000      // l'applicazione mostra l'help
                                       // l'help contestuale nella statusbar
                                       // contestuale in una finestra popup
//   #define FCF_CLOSEBUTTON            0x04000000L
#define FCF_FONT       0x10000000      // usa font specificato (se NULL usa
                                       // 9.WarpSans/8.Helv per warp4/3
#define FCF_STSBAR     0x20000000      // l'applicazione mostra i messaggi e
//   #define FCF_AUTOICON               0x40000000L
// la flag FCF_AUTOICON assume un significato speciale quando la finestra
// principale Š un dialogo, in quanto indica che l'icona Š presente
// nell'eseguibile stesso con ID 1 e non nel modulo esterno con lo stesso
// ID del dialogo
//   #define FCF_DBE_APPSTAT            0x80000000L

// flag valide solo quando la finestra principale Š un dialogo:
#define FCF_STDDFLAGS   (FCF_MENU | FCF_ACCELTABLE)

// tipo di msg loop
#define MSGLOOP_STANDARD   0x0    // loop messaggi semplice: termina al WM_QUIT
#define MSGLOOP_CHKQUIT    0x80   // indica che il loop dei messaggi deve
                                  // controllare WM_QUIT
#define MSGLOOP_CNTXTHLP   (FCF_BUBBLEHLP | FCF_STSBAR)
                                  // indica che il loop dei messaggi deve
                                  // controllare i movimenti del mouse per
                                  // l'implementazione dell'help contestuale

// dimensione stack thread object window (16 KB)
#define THREADSTACKSZ     0x4000L

// messaggio per help contestuale
#define WM_BUBBLEHELP     0xa500
// mostra per 10 secondi un messaggio nella statusbar
#define WM_SHOWSBARMSG    0xa501
// messaggio per terminare dialogo modale creato con WdlgBox
#define WM_DLGDISMISS     0xa501
// id timer per help contestuale
#define TID_CONTEXTHLP    0x7ad3

// codici di errore di inizializzazione:
// errore nullo utilizzato quando l'errore si verifica nella procedura
// di inizializzazione registrata dall'utente. Codice e messaggio di errore
// devono essere implementati nella procedura utente stessa
#define APPLERR_NULL         0
// impossibile allocare memoria per struttura dati applicazione
#define APPLERR_ALLOC        1
// impossibile avviare il thread secondario
#define APPLERR_THREAD       2
// impossibile caricare modulo risorse
#define APPLERR_LOADMOD      3
// impossibile registrare controlli PM estesi
#define APPLERR_REGCTLS      4
// impossibile creare finestra principale applicazione
#define APPLERR_CREATEWIN    5
// impossibile memorizzare nelle window words puntatore dati applicazione
//#define APPLERR_SETWINPTR    6
// id ultimo errore predefinito
#define IERRI_LAST           APPLERR_CREATEWIN
//#define APPLERR_
//#define APPLERR_

#define SZ_NULL ""

// stili file dialog apri, apri multiplo, salva:
#define FDLGS_OPEN            FDS_OPEN_DIALOG
#define FDLSG_OPENMULTI       (FDS_OPEN_DIALOG | FDS_MULTIPLESEL)
#define FDLGS_SAVE            (FDS_SAVEAS_DIALOG | FDS_ENABLEFILELB)

// MACRO:
// setta un testo tipo definizione: dato
#define WsetCltCmpTxt(hwnd, id, idstr, psz) \
        (WsetWcompTxt(WinWindowFromID((hwnd), (id)), (idstr), (psz)))

// setta il testo di un controllo ricavandolo dalla DLL delle risorse
#define WsetCtlTxt(hwnd, id, idstr) \
        (WsetWinTxt(WinWindowFromID((hwnd), (id)), (idstr)))

// variabile globale:
extern PCREATEAPP pa;     // indirizzo struttura dati creazione applicazione


// prototipi funzioni
// pmappapi.c
ULONG APIENTRY WcreateApp(PCREATEAPP pca, int argc, char ** argv);
VOID APIENTRY WdefDlgInit(PWDATA pw);
MRESULT EXPENTRY WdefDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY WdefModProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY WdefObjWinProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
ULONG WdlgBox(HWND hpar, HWND hown, PMODDLG pm);
INT APIENTRY WrunApp(PCREATEAPP pca);
// pmappaux.c
VOID Wapploop(PQMSG pqmsg, ULONG fl);
BOOL WloadAppDlg(PCREATEAPP pca);
VOID _System objThrdProc(ULONG ul);
BOOL startObjThrd(PCREATEAPP pca);
VOID WfreeWres(PWDATA pw);
// pmapputil.c
PVOID APIENTRY getPrfData(HINI hini, PSZ pszApp, PSZ pszKey, PULONG pul);
BOOL APIENTRY WaddToWinList(HWND hwnd, PSZ psz);
MRESULT APIENTRY WcheckMinSize(HWND hwnd, MPARAM mp1, MPARAM mp2, PSSIZE ps);
PSZ APIENTRY Wcpycat(PSZ trg, INT cstr, ...);

PFILEDLG APIENTRY Wfdlg(HWND hwnd, PSZ pszFile, PSZ pszext,
                        ULONG idTitle, ULONG idBtn, ULONG fl);
PCREATEAPP APIENTRY WgetCreationData(VOID);
ULONG APIENTRY WgetChlpStr(ULONG idstr);
ULONG APIENTRY WgetStr(ULONG idstr, PSZ psz, ULONG cbpsz);
ULONG APIENTRY WgetTxt(ULONG idstr, PSZ psz, ULONG cbpsz);
ULONG APIENTRY WinitErr(ULONG ulerr);
BOOL APIENTRY WmsgBox(HWND hwnd, ULONG idstr, ULONG fl);
BOOL APIENTRY WmsglBox(HWND hwnd, ULONG idstr, ULONG fl);
VOID APIENTRY WsetMinSize(PWDATA pw);
VOID APIENTRY WsetWcompTxt(HWND hwnd, ULONG ids, PSZ pszData);
ULONG APIENTRY WsetWinTxt(HWND hwnd, ULONG idstr);
ULONG APIENTRY WsetWinTxtl(HWND hwnd, ULONG idstr);
BOOL APIENTRY WshowInf(HWND hwnd, ULONG idinf, ULONG idarg);
VOID APIENTRY WshowMsg(ULONG idmsg, BOOL fl, HWND hsbar);
MRESULT APIENTRY WupdBrdSize(HWND hwnd, MPARAM mp1, MPARAM mp2,
                             PSSIZE ps, PSSIZE pb);
// helptable id
// title of the help window
// name of the help file
HWND APIENTRY WAppInitHelp(PCREATEAPP pca,
                  ULONG idHelpTable, PSZ pszTitle, PSZ pszHelpFile);


#endif
