//==========================================================================\
// afcuta00.h: header libreria funzioni creazione applicazioni PM           |
//==========================================================================/

#ifndef AFCUTA00_H_
   #define AFCUTA00_H_

// flag ricerca dati non presenti nel file INI usata da WgetPrfData
#define GPD_ERROR             ((PVOID)0xffffffff)

// api per sbloccare eseguibili lockati
APIRET APIENTRY DosReplaceModule(PSZ pszOld, PSZ pszNew, PSZ pszBkup);

// id risorsa stringhe pi— lunghe di 256 byte
#define RCLONGSTRING     301

// strutture standard usate dall'applicazione ------------------------------

// dimensione buffer help contestuale
#define CBSZHLP   512

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


// status window structure
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
#define MSGLOOP_CNTXTHLP   (APPLS_STSBAR | APPLS_BUBBLEHLP)
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
// id ultimo errore predefinito
#define IERRI_LAST           APPLERR_CREATEWIN

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

// setta il testo di un controllo ricavandolo dalla DLL delle risorse
// versione per stringhe pi— lunghe di 256 byte
#define WsetCtlTxtl(hwnd, id, idstr) \
        (WsetWinTxtl(WinWindowFromID((hwnd), (id)), (idstr)))

// ottiene indirizzo dati finestra
#define WgetWdata(hwnd) ((PWDATA)WinQueryWindowPtr((hwnd), 0))

// funzioni per la gestione dei notebook
// flag comunemente usate nei notebook stile warp 4
#define BFA_COMMON         BFA_PAGEDATA | BFA_PAGEFROMDLGRES | \
                           BFA_STATUSLINE | BFA_MINORTABTEXT
#define BKA_COMMON         BKA_AUTOPAGESIZE | BKS_TABTEXTCENTER | \
                           BKA_STATUSTEXTON | BKA_MINOR

#pragma pack(2)
// struttura minimale dati pagina
typedef struct _PGDAT{
   USHORT cb;    // dimensione struttura
   USHORT fs;    // tipo di tab (0 o BKA_MAJOR o BKA_MINOR
   struct _PGDAT * pprv; // indirizzo struttura pagina precedente
   struct _PGDAT * pnxt; // indirizzo struttura pagina successiva
   HWND hwnd;    // handle dialogo pagina
   ULONG pgid;   // id assegnata dal controllo all'inserimento della pagina
   ULONG fl;     // flag definibili dall'utente
   PVOID pv;     // indirizzo dati definibili dall'utente
} PGDAT, * PPGDAT;

// struttura lista pagine a dimensione variabile. ppd viene riallocato ogni
// volta che si inserisce o si nasconde una pagina
typedef struct {
   HWND hwnd;      // handle controllo notebook
   ULONG cpgs;     // totale pagine inserite
   PPGDAT ppd;     // indirizzo prima struttura dati pagina
} NBK, * PNBK;

// struttura dati inserzione pagine notebook (notebook insertion page)

typedef struct {
   USHORT pgn;   // numero pagina all'interno della tab
   USHORT cpg;   // numero pagine sotto tab corrente
   USHORT idst;  // ID stringa status line
                 // "pagina X di Y" usando i membri isdt, pgn e cpg per
                 // creare il testo opportuno, altrimenti indica inserzione
                 // BKA_LAST BKA_FIRST BKA_NEXT BKA_PREV
   USHORT idmaj; // ID stringa major tab
   USHORT idmin; // ID stringa minor tab
   USHORT fs;    // flag stile pagina 0, BKA_MAJOR, BKA_MINOR
   PFNWP pf;     // indirizzo procedura dialogo pagina
   ULONG iddlg;  // id dialogo da caricare
   ULONG fl;     // flag definite da utente da memorizzare nei dati pagina
   PVOID pv;     // indirizzo dati definibili dall'utente (opzioni pagina)
   PPGDAT* pppg; // eventuale ulteriore indirizzo in cui memorizzare dati pagina
} NBINSPG, * PNBINSPG;
#pragma pack()


#define NBINS_ISFIRST      ((PPGDAT)0xffffffff)    // prima pagina notebook
#define NBINS_PGXOFY       0x8000
// la seguente flag Š usata da WnbkDelPg() per indicare che l'id della pagina
// deve essere interpretato come ordine di inserzione della pagina nel notebook
#define BKA_PAGEORDER      0x10000L
// valore restituito da alcune funzioni in caso di errore
#define NBK_ERROR          0xffffffff
// flag usate da WnbkPgn() per restituire id pagina, posizione inserzione
// pagina o indirizzo struttura dati associati con la pagina
#define NBKQ_PGORD       0x0000 // restituisce posizione inserzione pagina
#define NBKQ_PGID        0x0001 // restituisce id pagina
#define NBKQ_PGDAT       0x0002 // restituisce indirizzo struttura pagina


//==========================================================================\
// logutil.h : funzioni di utilit… per il logging                           |
//==========================================================================/

#define CBLOGBUF 0x1000
#define CBMINBUF 0x4000

typedef struct {
   ULONG cbbuf;         // dimensione buffer
   HWND hwnd;           // finestra in cui eventualmente scrivere messaggi
   PSZ pcur;            // puntatore scrittura nel buffer
   CHAR fname[260];     // nome del file in cui scrivere il log
   ULONG ulstart;       // ms da boot time in cui inizia installazione
   CHAR ach[CBLOGBUF];  // buffer temporaneo di 4 KB
   CHAR buf[4];         // inizio buffer scrittura file log
} LOGFILE, * PLOGFILE;


//==========================================================================\
// instutil.h: funzioni di utilit… per programma di installazione           |
//==========================================================================/

// flag usate da prfinsstr e prfdelstr
#define PRFINCHECK     0x0000   // verifica se stringa Š presente
#define PRFINSTART     0x0001   // inserisce stringa a inizio
#define PRFINEND       0x0002   // inserisce stringa alla fine
#define PRFINZEROEND   0x0004   // i dati devono essere terminati da zero
#define PRFINNOTFOUND  0        // stringa non trovata
#define PRFINFOUND     1        // stringa presente
#define PRFINFERROR    -1       // errore
#define PRFINFOVRFLW   -2       // inserzione impossibile per overflow

// struttura copiatura file

#pragma pack(2)
typedef struct {
   FILESTATUS3 fss;      // dati file origine
   FILESTATUS3 fst;      // dati file destinazione
   CHAR achsrc[260];     // path sorgente file da copiare
   CHAR achtrg[260];     // path destinazione in cui copiare
                         // entrambi i path devono includere slash finale
   HWND hprgrs;          // handle progress window
   USHORT fl;            // modalit… copia
   USHORT cbsrc;         // lunghezza nome path sorgente
   USHORT cbtrg;         // lunghezza nome path destinazione
   USHORT msg;           // msg da scrivere nel file di log
   USHORT to;            // eventuale id messaggio "to:"
   USHORT confirm;       // msg conferma sovrascrittura
   USHORT err;           // msg di errore da mostrare in caso di fallimento
   USHORT csteps;        // numero passi totali procedura
   PULONG pidx;          // indice incremento se non Š NULL viene spedito
                         // messaggio incremento alla progress window
   PLOGFILE plf;
} FILECPY, * PFILECPY;

typedef struct {
   FILECPY fc;           // modalit… usata per copiare i singoli file
   PSZ pdir;             // directory contenente l'albero da copiare
   PSZ pmask;            // maschera file da copiare
   ULONG fl;             // attributi file da copiare
   ULONG cerr;           // numero errori non critici
   USHORT msg;           // messaggio creazione directory;
   USHORT err;           // messaggio errore creazione directory
} TREECPY, *PTREECPY;

#pragma pack()

// flag di settaggio
#define FCPYF_OVWRT       0x0001       // sovrascrive tutti i file
#define FCPYF_OVWCONF     0x0002       // sovrascrive chiedendo conferma
#define FCPYF_OVWCONFNEW  0x0004       // chiede conferma per sovrascrivere
                                       // i file + nuovi
#define FCPYF_BACKUP      0x0008       // crea copia backup file sovrascritti
#define FCPYF_UNLOCK      0x0010       // unlocka i moduli bloccati
#define FCPYF_DEFER       0x0020       // postpone copia al riavvio del sistema
#define FCPYF_STOPONERR   0x0040       // considera tutti gli errori critici
                                       // e in caso di errore mostra messaggio
                                       // errore, restituisce -1 e ferma
                                       // l'elaborazione.
                                       // Altrimenti scrive il messaggio di
                                       // errore solo nel log, incrementa
                                       // l'eventuale controllo di progresso
                                       // e restituisce 0 (errore non critico)
// flag usata internamente da instreecpy
#define FCPYF_ABORT       0x8000       // usata da instreecpy() quando si Š
                                       // verificato un errore nella
                                       // creazione di una directory in modo
                                       // che il programma non tenti di copiare
                                       // file nella directory mancante, ma si
                                       // limiti a riportare il messaggio di
                                       // errore (questo serve solo quando non
                                       // c'Š FCPYF_STOPONERR

// struttura cancellazione file

#pragma pack(2)
typedef struct {
   FILESTATUS3 fs;       // dati file da cancellare
   CHAR path[260];       // path file da cancellare
   HWND hprgrs;          // handle progress window
   USHORT cbpath;        // lunghezza nome path
   USHORT msg;           // msg da scrivere nel file di log
   USHORT err;           // msg di errore da mostrare in caso di fallimento
   USHORT stop;          // se TRUE considera l'errore critico, cioŠ in caso
                         // di errore mostra err e restituisce -1,
                         // altrimenti, se definiti membri progress window,
                         // spedisce messaggio di incremento e restituisce 0
   USHORT csteps;        // numero passi totali procedura
   PULONG pidx;          // indice incremento se non Š NULL viene spedito
                         // messaggio incremento alla progress window
   PLOGFILE plf;
} FILEDEL, * PFILEDEL;

typedef struct {
   FILEDEL fd;           // dati cancellazione file
   PSZ pdir;             // directory contenente l'albero da cancellare
   PSZ pmask;            // maschera file da cancellare
   ULONG fl;             // attributi file da cancellare
   ULONG cerr;           // numero errori non critici
   USHORT msg;           // messaggio rimozione directory;
   USHORT err;           // messaggio errore rimozione directory
} TREEDEL, *PTREEDEL;

#pragma pack()

// strutture per la creazione di oggetti
// dati setup oggetto
typedef struct {
   PSZ pclass;          // classe oggetto
   ULONG ititle;        // id stringa titolo oggetto
   PSZ psetup;          // stringa di setup
   PSZ ploc;            // location
} WPSOBJ, * PWPSOBJ;

// dati per la creazione inclusi messaggi log e progresso
typedef struct {
   PWPSOBJ po;          // indirizzo dati creazione oggetto
   PLOGFILE plf;        // indirizzo dati file di log
   PSZ path;            // path installazione eventualmente da inserire nella
                        // stringa di setup al posto di \t
   CHAR title[256];     // buffer per titolo oggetto
   CHAR setup[1024];    // buffer per setup oggetto
   USHORT msg;          // msg da scrivere nel log per descrivere operazione
   USHORT err;          // msg da scrivere nel log in caso di errore
   BOOL stop;           // se TRUE considera gli errori critici e in caso di
                        // errore restituisce FALSE altrimenti incrementa
                        // il contatore degli errori e restituisce TRUE
   USHORT cerr;         // contatore errori
   PULONG pidx;         // contatore da incrementare per finestra progresso
   HWND hprgrs;         // handle finestra progresso
   ULONG csteps;        // passi elaborazione totali
} CREATOBJ, * PCREATOBJ;



//==========================================================================\
// i path membri della struttura FILECPY devono terminare con lo slash!!!   |
// il membro fl della struttura FILECPY indica:                             |
// - se sovrascrivere sempre i file                                         |
// - se chiedere conferma per sovrascrivere i file + nuovi                  |
// - se effettuare copia di backup dei file da sostituire                   |
// - se il file Š un modulo che deve essere sbloccato                       |
// - se la copia del file deve essere deferita al riavvio (da implementare) |
// parametri:                                                               |
// PFILECPY pfc: dati necessari per copiare file                            |
// typedef struct {                                                         |
//    FILESTATUS3 fss;   // dati file origine                               |
//    FILESTATUS3 fst;   // dati file destinazione                          |
//    CHAR achsrc[260];  // path sorgente file da copiare                   |
//    CHAR achtrg[260];  // path destinazione in cui copiare                |
//                       // entrambi i path devono includere slash finale   |
//    HWND hprgrs;       // handle progress window                          |
//    ULONG fl;          // modalit… copia                                  |
//    USHORT cbsrc;      // lunghezza nome path sorgente                    |
//    USHORT cbtrg;      // lunghezza nome path destinazione                |
//    USHORT msg;        // msg da scrivere nel file di log                 |
//    USHORT to;         // eventuale id messaggio "to:"                    |
//    USHORT confirm;    // msg conferma sovrascrittura                     |
//    USHORT err;        // msg di errore da mostrare in caso di fallimento |
//    PLOGFILE plf;                                                         |
// } FILECPY, * PFILECPY;                                                   |
// PSZ pszfile: nome file da copiare                                        |
// PSZ psznew: se non NULL indica che il nuovo nome del file copiato        |
// valore restituito:                                                       |
// INT rc: -1 = errore critico, 0 = errore non critico, 1 = successo        |
//         -2 = interruzione da finestra progresso                          |
//==========================================================================/

INT APIENTRY insfilecpy(PFILECPY pfc, PSZ pszfile, PSZ psznew);

//==========================================================================\
// Cancella un file sbloccandolo se necessario. Se il file non esiste       |
// restituisce TRUE (successo)                                              |
// parametri:                                                               |
// PFILEDEL pfd: struttura dati file da cancellare                          |
// typedef struct {                                                         |
//    FILESTATUS3 fs;   dati file da cancellare                             |
//    CHAR path[260];   path file da cancellare (incluso slash finale)      |
//    HWND hprgrs;      handle progress window                              |
//    USHORT cbpath;    lunghezza nome path sorgente                        |
//    USHORT msg;       msg da scrivere nel file di log                     |
//    USHORT err;       msg di errore da mostrare in caso di fallimento     |
//    USHORT stop;      se TRUE considera l'errore critico, cioŠ in caso    |
//                      di errore mostra err e restituisce -1,              |
//                      altrimenti, se definiti membri progress window,     |
//                      spedisce messaggio di incremento e restituisce 0    |
//    USHORT csteps;    numero passi totali procedura                       |
//    PULONG pidx;      indice incremento se non Š NULL viene spedito       |
//                      messaggio incremento alla progress window           |
//    PLOGFILE plf;                                                         |
// } FILEDEL, * PFILEDEL;                                                   |
// PSZ pszfile: nome file da cancellare                                     |
// valore restituito:                                                       |
// INT rc: -1 = errore critico, 0 = errore non critico, 1 = successo        |
//         -2 = interruzione da finestra progresso                          |
//==========================================================================/

INT APIENTRY insfiledel(PFILEDEL pfd, PSZ pszfile);

//==========================================================================\
// Crea un oggetto scrivendo il resoconto dell'operazione nell'eventuale    |
// file di log ed incrementando l'eventuale finestra di progresso.          |
// In caso di errore se il membro stop Š TRUE restituisce FALSE altrimenti  |
// incrementa il contatore degli errori e restituisce TRUE                  |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/interruzione o errore critico             |
// strutture per la creazione di oggetti                                    |
// ---- dati setup oggetto ------------------------------------------------ |
// typedef struct {                                                         |
//    PSZ pclass;         classe oggetto                                    |
//    ULONG ititle;       id stringa titolo oggetto                         |
//    PSZ psetup;         stringa di setup                                  |
//    PSZ ploc;           location                                          |
// } WPSOBJ, * PWPSOBJ;                                                     |
//                                                                          |
// ---- dati per la creazione inclusi messaggi log e progresso ------------ |
// typedef struct {                                                         |
//    PWPSOBJ po;         indirizzo dati creazione oggetto                  |
//    PLOGFILE plf;       indirizzo dati file di log                        |
//    PSZ path;           path installazione eventualmente da inserire nella|
//                        stringa di setup al posto di \t                   |
//    CHAR title[256];    buffer per titolo oggetto                         |
//    CHAR setup[1024];   buffer per setup oggetto                          |
//    USHORT msg;         msg da scrivere nel log per descrivere operazione |
//    USHORT err;         msg da scrivere nel log in caso di errore         |
//    BOOL stop;          se TRUE considera gli errori critici e in caso di |
//                        errore restituisce FALSE altrimenti incrementa    |
//                        il contatore degli errori e restituisce TRUE      |
//    USHORT cerr;        contatore errori                                  |
//    PULONG pidx;        contatore da incrementare per finestra progresso  |
//    HWND hprgrs;        handle finestra progresso                         |
//    ULONG csteps;       passi elaborazione totali                         |
// } CREATOBJ, * PCREATOBJ;                                                 |
//==========================================================================/

BOOL APIENTRY insmakeobj(PCREATOBJ pco);

//==========================================================================\
// copia un albero di file creando la directory di partenza se non esiste   |
// parametri:                                                               |
// PTREECPY ptc: struttura dati tree da copiare                             |
// typedef struct {                                                         |
//    FILECPY fc;        modalit… usata per copiare i singoli file          |
//    PSZ pdir;          directory contenente l'albero da copiare (solo     |
//                       nome directory senza path e slash)                 |
//    PSZ pmask;         maschera file da copiare                           |
//    ULONG fl;          attributi file da copiare                          |
//    ULONG cerr;        numero errori non critici                          |
//    USHORT msg;        messaggio creazione directory;                     |
//    USHORT err;        messaggio errore creazione directory               |
// } TREECPY, *PTREECPY;                                                    |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore critico o interruzione             |
//==========================================================================/

BOOL APIENTRY instreecpy(PTREECPY ptc);

//==========================================================================\
// rimuove un intero albero di file e directory                             |
// parametri:                                                               |
// PTREEDEL ptd: struttura dati albero da cancellare                        |
// typedef struct {                                                         |
//    FILEDEL fd;       dati cancellazione file                             |
//    PSZ pdir;         directory contenente l'albero da cancellare         |
//    PSZ pmask;        maschera file da cancellare                         |
//    ULONG fl;         attributi file da cancellare                        |
//    ULONG cerr;       numero errori non critici                           |
//    USHORT msg;       messaggio rimozione directory;                      |
//    USHORT err;       messaggio errore rimozione directory                |
// } TREEDEL, *PTREEDEL;                                                    |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore critico o interruzione             |
//==========================================================================/

BOOL APIENTRY instreedel(PTREEDEL ptd);

//==========================================================================\
// Concatena un numero variabile di stringhe nel file di log.               |
// Se gli indirizzi delle stringhe sono minori di 0xffff, le stringhe       |
// vengono interpretate come id di stringhe da ricavare dalle risorse.      |
// setta il testo del controllo hwnd alla stringa composta                  |
// aggiunge un carattere di nuova linea e appende la stringa al buffer per  |
// la scrittura del file di Log incrementandone il puntatore al cursore     |
// parametri:                                                               |
// PLOGFILE plf: struttura descrizione file log e finestra log              |
// ULONG fl: se TRUE viene considerato un messaggio di errore.              |
//           In questo caso si assume che plf->ach contenga la descrizione  |
//           dell'operazione fallita, per cui a plf->ach viene concatenato  |
//           "\nFailed" ed il buffer completo viene usato per mostrare      |
//           il messaggio di errore tramite la finestra di progresso.       |
//           Nel buffer del file invece viene scritto solo "Failed\n".      |
// INT carg: numero stringhe (o id stringhe) presenti come argomenti        |
// ... : numero variabile di char* o id stringhe ricavabili da dll risorse  |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

BOOL APIENTRY logAdd(PLOGFILE plf, ULONG fl, INT carg, ...);

//==========================================================================\
// copia pstr in pbuf controllando se Š una vera stringa o Š l'id di una    |
// risorsa e controllando l'overflow del buffer.                            |
// se la stringa Š NULL restituisce l'indirizzo del buffer di partenza      |
// se la lunghezza della stringa da copiare supera la dimensione disponibile|
// nel buffer restituisce NULL                                              |
// parametri:                                                               |
// PSZ pbuf: indirizzo buffer                                               |
// PSZ pstr: stringa da copiare                                             |
// ULONG cb: spazio disponibile nel buffer                                  |
// valore restituito:                                                       |
// PSZ fine buffer per successiva concatenazione                            |
//==========================================================================/

PSZ APIENTRY logCat(PSZ pbuf, PSZ pstr, ULONG cb);

//==========================================================================\
// Scrive messaggio termine operazione loggata.                             |
// Calcola tempo trascorso e lo scrive.                                     |
// Libera struttura dati file log.                                          |
// parametri:                                                               |
// PLOGFILE plf: struttura dati file log                                    |
// PSZ pendstr: stringa descrizione fine operazione (id stringa o stringa)  |
// PSZ psztime: stringa "Elapsed time: 11 hour, 22 minutes and 33 seconds"  |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

BOOL APIENTRY logEnd(PLOGFILE plf, PSZ pendstr, PSZ psztime);

//==========================================================================\
// Alloca struttura dati per scrivere resoconto azioni contemporaneamente   |
// in un file di log e in una finestra.                                     |
// Se la dimensione specificata per il buffer di log in cbbuf Š inferiore a |
// 16 KB viene arrotondata.                                                 |
// Se un file di log Š gi… presente lo cancella.                            |
// parametri:                                                               |
// HWND hwnd: eventuale finestra in cui scrivere dati                       |
// PSZ pfname: nome file di log                                             |
// ULONG cbbuf: dimensione buffer da allocare                               |
// PSZ pdescr: descrizione della procedura di cui si esegue il log          |
//             (per esempio: "Installazione Styler/2").                     |
//             Se minore di 0xffff viene interpretato come id di stringa da |
//             ricavare dalle risorse                                       |
// valore restituito:                                                       |
// PLOGFILE plf: indirizzo struttura dati allocata, NULL in caso di errore  |
//==========================================================================/

PLOGFILE APIENTRY logInit(HWND hwnd, PSZ pfname, ULONG cbbuf, PSZ pdescr);

//==========================================================================\
// Controlla se l'ID di un oggetto Š registrato in OS2.INI.                 |
// parametri:                                                               |
// PSZ pobj: una serie di stringhe di id di oggetti terminate da 0 e        |
//           concatenate. L'ultima stringa Š una stringa vuota              |
// valore restituito:                                                       |
// BOOL rc: TRUE: almeno uno degli oggetti in pobj Š presente               |
//==========================================================================/

BOOL APIENTRY prfChkObj(PSZ pobj);

//==========================================================================\
// Cancella una substringa da un valore memorizzato in un file INI.         |
// La ricerca non Š case insensitive                                        |
// parametri:                                                               |
// HINI hini: handle file ini                                               |
// PSZ app: nome applicazione                                               |
// PSZ key: nome chiave                                                     |
// PSZ str: stringa da rimuovere (la stringa pu• contenere '\0')            |
// ULONG cbstr: lunghezza della stringa                                     |
// PSZ sep: stringa di separatori ammessi                                   |
// ULONG cbsep: lunghezza stringa separatori (permette di usare 0)          |
// ULONG fl: flag che indica se il valore nel file INI Š terminato da 0     |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

BOOL APIENTRY prfdelstr(HINI hini, PSZ app, PSZ key, PSZ str,
                        ULONG cbstr, PSZ sep, ULONG cbsep, ULONG fl);

//==========================================================================\
// Inserisce una stringa tra le altre stringhe presenti nei dati memorizzati|
// in un file INI. Se la flag di inserzione non Š PRFINSTART o PRFINEND si  |
// limita a riportare se la stringa Š presente o meno.                      |
// La ricerca non Š case insensitive                                        |
// parametri:                                                               |
// HINI hini: handle file ini                                               |
// PSZ app: nome applicazione                                               |
// PSZ key: nome chiave                                                     |
// PSZ str: stringa da inserire ricercare (la stringa pu• contenere '\0')   |
// ULONG cbstr: lunghezza della stringa                                     |
// ULONG cbfind: lunghezza stringa da ricercare                             |
// PSZ sep: stringa di separatori ammessi                                   |
// ULONG cbsep: lunghezza stringa separatori (permette di usare 0)          |
// ULONG flins: se TRUE inserisce la stringa nella posizione corrispondente |
//         PRFINCHECK     0x0000    verifica solo se stringa Š presente     |
//         PRFINSTART     0x0001    inserisce stringa a inizio              |
//         PRFINEND       0x0002    inserisce stringa alla fine             |
//         PRFINZEROEND   0x0004    i dati devono essere terminati da zero  |
//         PRFINNOTFOUND  0         stringa non trovata                     |
//         PRFINFOUND     1         stringa presente o inserzione riuscita  |
//         PRFINFERROR    -1        errore                                  |
//         PRFINFOVRFLW   -2        inserzione impossibile per overflow     |
//             a flins (1 = inizio, 2 = fine), se FALSE si limita a         |
//             riportare la presenza della stringa.                         |
// valore restituito:                                                       |
// INT rc: 1 = stringa presente/inserita, 0 = stringa assente, -1 = errore  |
//==========================================================================/

INT APIENTRY prfinsstr(HINI hini, PSZ app, PSZ key, PSZ str,
                       ULONG cbstr, PSZ sep, ULONG cbsep, ULONG flins);

//==========================================================================\
// ricava il titolo della finestra dall'ID della stringa, setta il testo    |
// della titlebar e aggiunge la finestra alla window list                   |
// parametri:                                                               |
// HWND hwnd: handle finestra da aggiungere alla window list                |
// PSZ psz: se < 0xffff viene interpretato come id stringa altrimenti come  |
//          stringa                                                         |
// valore restituito:                                                       |
// BOOL rc: FALSE/TRUE = errore/successo                                    |
//==========================================================================/

BOOL APIENTRY WaddToWinList(HWND hwnd, PSZ psz);

//==========================================================================\
// modifica la struttura TRACKINFO per controllare la dimensione minima     |
// finestra. I membri di ps devono essere aggiornati subito dopo            |
// la creazione del dialogo con la dimensione iniziale del dialogo che deve |
// essere pari alla dimensione minima del dialogo                           |
// parametri:                                                               |
// HWND, MPARAM, MPARAM                                                     |
// PSSIZE ps: struttura dimensioni minime finestra                          |
// valore restituito:                                                       |
// MRESULT mr restituito dalla WinDefDlgProc()                              |
//==========================================================================/

MRESULT APIENTRY WcheckMinSize(HWND hwnd, MPARAM mp1, MPARAM mp2, PSSIZE ps);

//==========================================================================\
// Copia cstr stringhe in trg concatenandole                                |
// Restituisce un puntatore alla fine della stringa.                        |
// Se l'indirizzo di una stringa Š minore di 0xffff lo considera l'id di    |
// una stringa che deve essere ricavata dalla dll delle risorse             |
// parametri:                                                               |
// PSZ trg: stringa target                                                  |
// PSZ src: stringa sorgente                                                |
// INT cstr: numero argomenti variabili                                     |
// valore restituito:                                                       |
// PSZ p: indirizzo fine stringa                                            |
//==========================================================================/

PSZ APIENTRY Wcpycat(PSZ trg, INT cstr, ...);

//==========================================================================\
// Crea un applicazione:                                                    |
// - alloca la memoria necessaria a memorizzare dati usati dall'applicazione|
// - esegue inizializzazione default:                                       |
//   - se flag objwin crea secondo thread con object window                 |
//   - se specificato nome dll la carica e ne memorizza l'handle            |
//   - ricava anchor block                                                  |
//   - se specificata flag registra controlli PM                            |
// parametri usati:                                                         |
// - flag creazione thread secondario e object window                       |
// - nome DLL risorse                                                       |
// - flag controlli da registrare                                           |
// codici messaggi errore:                                                  |
// 1 : errore allocazione                                                   |
// 2 : errore creazione thread secondario con object window                 |
// 3 : errore caricamento modulo risorse                                    |
// 4 : errore inizializzazione controlli PM estesi                          |
// 5 : errore creazione finestra principale applicazione                    |
// valore restituito:                                                       |
// in caso di errore flagga il membro rc di APPDATA al valore dell'errore   |
// e restituisce true per permettere a WrunApp di liberare le risorse       |
// BOOL: FALSE = errore allocazione APPDATA                                 |
//       TRUE l'esito della procedura viene riportato da pca->pad->rc       |
//==========================================================================/

ULONG APIENTRY WcreateApp(PCREATEAPP pca, int argc, char ** argv);

//==========================================================================\
// esegue inizializzazione default dialogo:                                 |
// - memorizza eventuale handle statusbar                                   |
// - setta icona dialogo                                                    |
// - setta barra menu                                                       |
// - setta accelleratori                                                    |
// - aggiorna frame                                                         |
// - resetta font dialogo                                                   |
// - calcola dimensioni minime                                              |
// - ridimensiona e riposiziona la finestra mostrandola sullo schermo       |
// parametri:                                                               |
// PWDATA pw: dati finestra dialogo                                         |
// ULONG fl: flag inizializzazion dialogo (stesse usate in CREATEAPP)       |
// ULONG idres: id risorse (menu, icona, acceleratori) usate nel dialogo    |
// PSZ pszfont: font da usare nel dialogo                                   |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID APIENTRY WdefDlgInit(PWDATA pw);

//==========================================================================\
// Procedura di default dei dialoghi creati con WloadAppDlg() o con WdlgBox()
//==========================================================================/

MRESULT EXPENTRY WdefDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

//==========================================================================\
// procedura default dialoghi modali con propria statusbar                  |
//==========================================================================/

MRESULT EXPENTRY WdefModProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

//==========================================================================\
// procedura di default della object window                                 |
// aggiungere variabile per controllare disabilitazione help contestuale    |
// nel caso in cui l'help venga implementato sotto forma di finestre popup  |
//==========================================================================/

MRESULT EXPENTRY WdefObjWinProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

//==========================================================================\
// crea una finestra di dialogo modale con la sua status bar memorizzando   |
// l'handle di quest'ultima come statusbar attiva dell'applicazione         |
// La struttura WDATA deve essere allocata e inizializzata prima della      |
// chiamata                                                                 |
// parametri:                                                               |
// HWND hownr: handle owner window                                          |
// PFNWP pfnwp: procedura dialogo                                           |
// ULONG id: id dialogo                                                     |
// PVOID pb: indirizzo dati da memorizzare nelle window words del dialogo   |
// ULONG idbar: id statusbar che verr… resa attiva                          |
//==========================================================================/

ULONG WdlgBox(HWND hpar, HWND hown, PMODDLG pm);

//==========================================================================\
// Alloca la struttura FILEDLG settandone opportunamente i membri. Se non   |
// ci sono errori e il file viene chiuso tramite il bottone OK, restituisce |
// l'indirizzo della struttura stessa, che dovr… essere liberata dopo l'uso,|
// oppure NULL in caso di errore.                                           |
// parametri:                                                               |
// HWND hwnd: handle owner window                                           |
// PSZ pszFile: in/out specifiche file da ricercare/nome file trovato       |
// PSZ pszext: eventuale maschera (es: *.txt) che viene sostituita al nome  |
//             file in pszFile all'apertura del dialogo se non NULL         |
// ULONG idTitle: titolo dialogo (se 0 usa default)                         |
// ULONG idBtn: titolo bottone (se 0 usa default)                           |
// ULONG fl: FDS_* flag file dialog                                         |
//           3 configurazioni tipiche:                                      |
//           FDLGS_OPEN                                                     |
//           FDLSG_OPENMULTI                                                |
//           FDLGS_SAVE                                                     |
// valore restituito:                                                       |
// PFILEDLG struttura file dialog allocata o NULL se errore o si preme cancel
//==========================================================================/

PFILEDLG APIENTRY Wfdlg(HWND hwnd, PSZ pszFile, PSZ pszext,
                        ULONG idTitle, ULONG idBtn, ULONG fl);

//==========================================================================\
// restituisce l'indirizzo della struttura dati creazione applicazione      |
// parametri:                                                               |
// VOID                                                                     |
// valore restituito:                                                       |
// PCREATEAPP pa;                                                           |
//==========================================================================/

PCREATEAPP APIENTRY WgetCreationData(VOID);

//==========================================================================\
// restituisce la stringa idstr ricavandola dalla DLL delle risorse della   |
// corrente applicazione                                                    |
// parametri:                                                               |
// ULONG idstr: id stringa da restituire                                    |
// valore restituito:                                                       |
// ULONG cb: il numero dei caratteri copiati nel buffer                     |
//==========================================================================/

ULONG APIENTRY WgetChlpStr(ULONG idstr);

//==========================================================================\
// restituisce la stringa idstr ricavandola dalla DLL delle risorse della   |
// corrente applicazione e copiandola nel buffer puntato da psz             |
// parametri:                                                               |
// ULONG idstr: id stringa da restituire                                    |
// PSZ psz: indirizzo buffer                                                |
// ULONG cbpsz: dimensione buffer                                           |
// valore restituito:                                                       |
// ULONG cb: il numero dei caratteri copiati nel buffer escluso lo zero di  |
//           terminazione                                                   |
//==========================================================================/

ULONG APIENTRY WgetStr(ULONG idstr, PSZ psz, ULONG cbpsz);

//==========================================================================\
// Ricava un testo che pu• essere pi— lungo di 256 byte dalla risorsa       |
// RCLONGSTRING definita come 301                                           |
// parametri:                                                               |
// ULONG idstr: id stringa da restituire                                    |
// PSZ psz: indirizzo buffer                                                |
// ULONG cbpsz: dimensione buffer                                           |
// valore restituito:                                                       |
// ULONG cb: il numero dei caratteri copiati nel buffer                     |
//==========================================================================/

ULONG APIENTRY WgetTxt(ULONG idstr, PSZ psz, ULONG cbpsz);

//==========================================================================\
// Se si verifica un errore durante l'inizializzazione del programma mostra |
// una message box ricavando il messaggio di errore dal relativo membro     |
// della struttura CREATEAPP.                                               |
//==========================================================================/

ULONG APIENTRY WinitErr(ULONG ulerr);

//==========================================================================\
// mostra un massimo di 256 byte di testo ricavandolo da una risorsa stringa|
// della dll delle risorse.                                                 |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idstr: id stringa da mostrare                                      |
// BOOL fl: flag:  PMPRNT_ERROR (default) = errore                          |
//                 PMPRNT_WARN = stile Warning                              |
//                 PMPRNT_INFO = stile info                                 |
//                 PMPRNT_QUERY = richiesta conferma                        |
//          in modalit… PMPRNT_QUERY si possono usare le seguenti flag per  |
//          mostrare un'icona differente da quella di default (?) :         |
//          PMPRNT_QWARNICO = icona warning (!)                             |
//          PMPRNT_QINFOICO = icona informazioni (i)                        |
//          PMPRNT_QERRORICO = icona errore                                 |
//          PMPRNT_QQUERYICO = icona query (?) (default)                    |
// valore restituito:                                                       |
// BOOL rc: il valore restituito da Wprint                                  |
//==========================================================================/

BOOL APIENTRY WmsgBox(HWND hwnd, ULONG idstr, ULONG fl);

//==========================================================================\
// mostra un massimo di 64 KB di testo ricavandolo dalla risorsa            |
// RCLONGSTRING della dll delle risorse.                                    |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idstr: id stringa da mostrare                                      |
// BOOL fl: flag:  PMPRNT_ERROR (default) = errore                          |
//                 PMPRNT_WARN = stile Warning                              |
//                 PMPRNT_INFO = stile info                                 |
//                 PMPRNT_QUERY = richiesta conferma                        |
//          in modalit… PMPRNT_QUERY si possono usare le seguenti flag per  |
//          mostrare un'icona differente da quella di default (?) :         |
//          PMPRNT_QWARNICO = icona warning (!)                             |
//          PMPRNT_QINFOICO = icona informazioni (i)                        |
//          PMPRNT_QERRORICO = icona errore                                 |
//          PMPRNT_QQUERYICO = icona query (?) (default)                    |
// valore restituito:                                                       |
// BOOL rc: il valore restituito da Wprint                                  |
//==========================================================================/

BOOL APIENTRY WmsglBox(HWND hwnd, ULONG idstr, ULONG fl);

//==========================================================================\
// cancella una o + pagine dal notebook, prima cancella le pagine poi       |
// libera la memoria allocata per i dati della pagina                       |
// parametri:                                                               |
// PNBK pnbk: struttura dati notebook                                       |
// ULONG id: id pagina                                                      |
// ULONG fl: 0 libera tutta la memoria allocata senza spedire msg           |
//           BKA_SINGLE = cancella solo una pagina                          |
//           BKA_TAB = se la pagina corrente Š major tab cancella tutte     |
//                     pagine successive fino a prossima major tab          |
//           BKA_ALL = cancella tutte le pagine                             |
// valore restituito:                                                       |
// BOOL risultato del messaggio                                             |
//==========================================================================/

VOID APIENTRY WnbkDelPg(PNBK pnbk, ULONG id, ULONG fl);

//==========================================================================\
// riempie sequenzialmente di pagine un controllo notebook:                 |
// parametri:                                                               |
// PNBK pnbk: indirizzo struttura dati notebook                             |
// PNBINSPG pnbi: indirizzo array strutture NBINSPG                         |
// PPGLIST ppl: indirizzo struttura lista pagine                            |
// ULONG ci: numero di pagine (elementi array) da inserire                  |
// BOOL fl: indica necessit… calcolare e settare dimensioni tab             |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE : successo/errore                                    |
//==========================================================================/

BOOL APIENTRY WnbkFill(PNBK pnbk, PNBINSPG pnbi, BOOL fl);

//==========================================================================\
// inserisce sequenzialmente una pagina controllando la massima lunghezza   |
// delle stringhe usate nelle tab primarie e secondarie                     |
// parametri:                                                               |
// PNBK pnbk: indirizzo struttura dati notebook                             |
// PBOOKPAGEINFO pbpi: indirizzo struttura dati notebook                    |
// PNBINSPG pnbi: indirizzo struttura dati inserzione pagina                |
// PPGDAT pprev: punta alla struttura pagina precedente                     |
// PULONG pcxM: usato per calcolo larghezza major tabs                      |
// PULONG pcxm: usato per calcolo larghezza minor tabs                      |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

PPGDAT APIENTRY WnbkInitPg(PNBK pnbk, PBOOKPAGEINFO pbpi, PNBINSPG pnbi,
                           PPGDAT pprev, PULONG pcxM, PULONG pcxm);

//==========================================================================\
// Alloca una struttura pagedata, la inizializza ed inserisce una pagina    |
// nel notebook. In caso di errore libera la struttura e restituisce NULL   |
// La pagina viene sempre inserita dopo la pagina la cui struttura ppgdata  |
// viene passata come parametro                                             |
// parametri:                                                               |
// PNBK pnbk: indirizzo struttura dati notebook                             |
// PBOOKPAGEINFO pbpi: indirizzo struttura dati notebook                    |
// PNBINSPG pnbi: indirizzo struttura dati inserzione pagina                |
// PPGDAT pprev: punta alla struttura pagina precedente                     |
// ULONG ulpos: la posizione Š relativa a questa pagina (a meno che         |
//              pnbi->fsbka non sia BKA_LAST o BKA_FIRST)                   |
// ULONG flins: flag inserzione: BKA_LAST BKA_FIRST BKA_NEXT BKA_PREV       |
//              pu• anche essere NBIS_PGXOFY che indica di inserire la      |
//              stringa "page x of y" ricavandola da pnbi->idst, pnbi->pgn  |
//              e pnbi->cpg.                                                |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

PPGDAT APIENTRY WnbkInsertPg(PNBK pnbk, PBOOKPAGEINFO pbpi, PNBINSPG pnbi,
                             PPGDAT pprev, ULONG flins);

//==========================================================================\
// calcola la dimensione minima di un notebook in base a quella tipica di   |
// un dialogo che costituisce la sua pagina.                                |
// parametri:                                                               |
// PWDATA pw: struttura dati pagina                                         |
// ULONG cxm: larghezza minima interna pagina                               |
// ULONG cym: altezza minima interna pagina                                 |
// BOOL fl: TRUE/FALSE = espressione dimensioni in pixel/dialog units       |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

BOOL APIENTRY WnbkMinSize(PWDATA pw, ULONG cxm, ULONG cym, BOOL fl);

//==========================================================================\
// restituisce il numero della pagina corrispondente all'ID specificato     |
// (fl == FALSE) o l'id della pagina inserita alla posizione specificata    |
// (fl == TRUE).                                                            |
// PNBK pnbk: struttura dati notebook                                       |
// ULONG id: numero o id pagina                                             |
// ULONG fl: NBKQ_PGORD restituisce posizione inserzione pagina             |
//           NBKQ_PGID restituisce id pagina                                |
//           NBKQ_PGDAT restituisce indirizzo dati associati con pagina     |
// ULONG id pagina o posizione inserzione pagina secondo flag specificata   |
//       restituisce 0xffffffff in caso di errore                           |
//==========================================================================/

ULONG APIENTRY WnbkPgn(PNBK pnbk, ULONG id, ULONG fl);

//==========================================================================\
// trova nella stringa "pagina 1 di 2" i numeri 1 e 2 e li sostituisce      |
// con ulPg e cPgs.                                                         |
// parametri:                                                               |
// ULONG idstr: id stringa "pagina 1 di 2"                                  |
// ULONG ulPg: indice pagina corrente                                       |
// ULONG cPgs: totale pagine                                                |
// PSZ psz: buffer in cui si copia la stringa (256 caratteri)               |
//==========================================================================/

PSZ APIENTRY WpageXofY(ULONG idstr, ULONG ulPg, ULONG cPgs, PSZ psz);

//==========================================================================\
// esegue il loop messaggi dell'applicazione e al termine, dopo aver fatto  |
// terminare l'eventuale thread secondario, libera le risorse               |
// parametri:                                                               |
// VOID                                                                     |
// valore restituito:                                                       |
// int: codice restituito da applicazione, tipicamente 0 = successo         |
//==========================================================================/

INT APIENTRY WrunApp(PCREATEAPP pca);

//==========================================================================\
// setta nelle window words del controllo id l'id della stringa di help     |
// contestuale                                                              |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idctl: id controllo                                                |
// ULONG idstr: id stringa                                                  |
// valore restituito:                                                       |
// BOOL:    TRUE  = errore                                                  |
//          FALSE = successo                                                |
//==========================================================================/

BOOL APIENTRY WsetChlp(HWND hwnd, ULONG idctl, ULONG idstr);

//==========================================================================\
// setta nelle window words dei controlli componenti una compound listbox   |
// l'id della corrispondente stringa di help contestuale                    |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idctl: id controllo                                                |
// ULONG idcpt: id help caption o check box facente parte del controllo     |
// ULONG idlst: id help listbox                                             |
// ULONG identry: id help entryfield                                        |
// ULONG idadd: id help bottone add                                         |
// ULONG idrem: id help bottone remove                                      |
// ULONG idfnd: id help bottone find                                        |
// valore restituito:                                                       |
// BOOL:    TRUE  = errore                                                  |
//          FALSE = successo                                                |
//==========================================================================/

VOID APIENTRY WsetChlpClbox(HWND hwnd, ULONG idctl, ULONG idcpt, ULONG idlst,
                     ULONG identry, ULONG idadd, ULONG idrem, ULONG idfnd);

//==========================================================================\
// setta nelle window words dei controlli componenti una combobox           |
// l'id della corrispondente stringa di help contestuale                    |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idctl: id controllo                                                |
// ULONG idstr: id stringa                                                  |
// valore restituito:                                                       |
// BOOL:    TRUE  = errore                                                  |
//          FALSE = successo                                                |
//==========================================================================/

BOOL APIENTRY WsetChlpCombo(HWND hwnd, ULONG idctl, ULONG idstr);

//==========================================================================\
// setta nelle window words dei controlli componenti una enhanced listbox   |
// l'id della corrispondente stringa di help contestuale                    |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idctl: id controllo                                                |
// ULONG idstr: id stringa                                                  |
// valore restituito:                                                       |
// BOOL:    TRUE  = errore                                                  |
//          FALSE = successo                                                |
//==========================================================================/

VOID APIENTRY WsetChlpElbox(HWND hwnd, ULONG idctl, ULONG idstr);

//==========================================================================\
// setta nelle window words dei controlli componenti una morebox            |
// l'id della corrispondente stringa di help contestuale                    |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idctl: id controllo                                                |
// ULONG idcpt: id help caption o check box facente parte del controllo     |
// ULONG idswp: id help bottone swap                                        |
// ULONG idfnd: id help bottone find                                        |
// valore restituito:                                                       |
// BOOL:    TRUE  = errore                                                  |
//          FALSE = successo                                                |
//==========================================================================/

VOID APIENTRY WsetChlpMorebox(HWND hwnd, ULONG idctl,
                             ULONG idcpt, ULONG idswp, ULONG idfnd);

//==========================================================================\
// setta nelle window words dei controlli da istart a iend l'id delle       |
// corrispondenti stringhe di help contestuale                              |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idstart: id primo controllo                                        |
// ULONG idend: id ultimo controllo                                         |
// ULONG idstr: id stringa                                                  |
// valore restituito:                                                       |
// BOOL:    TRUE  = errore                                                  |
//          FALSE = successo                                                |
//==========================================================================/

BOOL APIENTRY WsetChlpMulti(HWND hwnd, ULONG idstart,
                            ULONG idend, ULONG idstr);

//==========================================================================\
// setta nelle window words dei controlli componenti lo spinbutton          |
// l'id della corrispondente stringa di help contestuale                    |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idctl: id controllo                                                |
// ULONG idstr: id stringa                                                  |
// valore restituito:                                                       |
// BOOL:    TRUE  = errore                                                  |
//          FALSE = successo                                                |
//==========================================================================/

BOOL APIENTRY WsetChlpSpin(HWND hwnd, ULONG idctl, ULONG idstr);

//==========================================================================\
// inizializza dimensioni minime dialogo in base alla dimensione di default |
// e memorizza spessore bordi dialogo                                       |
// parametri:                                                               |
// PWDATA pw: dati finestra                                                 |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID APIENTRY WsetMinSize(PWDATA pw);

//==========================================================================\
// Imposta il testo di una finestra come:                                   |
// "pszTerm" "pszData"                                                      |
// se pszData Š NULL lo sostituisce con "-"                                 |
// parametri:                                                               |
// PAPPDATA pad: struttura dati applicazione                                |
// HWND hwnd: handle finestra                                               |
// ULONG ids: id stringa (font/bitmap)                                      |
// PSZ pszData: dati (es. "5.System VIO", "tile.bmp")                       |
// Nota: la lunghezza totale delle 2 stringhe non deve superare i 255 byte  |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID APIENTRY WsetWcompTxt(HWND hwnd, ULONG ids, PSZ pszData);

//==========================================================================\
// setta il testo di una finestra ricavandolo da una risorsa stringa        |
// della DLL delle risorse.                                                 |
// restituisce il valore restituito da WgetStr                              |
// parametri:                                                               |
// HWND hwnd: handle finestra                                               |
// ULONG idstr: id stringa da usare come testo per la finestra              |
// valore restituito:                                                       |
// ULONG cb: numero caratteri ottenuti da WgetStr()                         |
//==========================================================================/

ULONG APIENTRY WsetWinTxt(HWND hwnd, ULONG idstr);

//==========================================================================\
// Setta il testo di una finestra ricavandolo da una risorsa                |
// RCLONGSTRING della DLL delle risorse                                     |
// Restituisce la lunghezza del testo.                                      |
// parametri:                                                               |
// HWND hwnd: handle finestra                                               |
// ULONG idstr: id stringa da usare come testo per la finestra              |
// valore restituito:                                                       |
// ULONG cb: numero caratteri ottenuti da WgetStr()                         |
//==========================================================================/

ULONG APIENTRY WsetWinTxtl(HWND hwnd, ULONG idstr);

//==========================================================================\
// apre o porta in primo piano una finestra di un file inf passandogli come |
// parametro la stringa opportuna                                           |
// parametri:                                                               |
// HWND hwnd: finestra che riceve notificazione alla chiusura del file INF  |
// ULONG idinf: id stringa nome file inf                                    |
// ULONG idarg: id stringa che contiene argomento da visualizzare           |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

BOOL APIENTRY WshowInf(HWND hwnd, ULONG idinf, ULONG idarg);

//==========================================================================\
// mostra messaggio per 10 secondi nella statusbar generando un WA_NOTE,    |
// WA_WARNING o WA_ERROR                                                    |
// parametri:                                                               |
// ULONG iderr: id stringa messaggio                                        |
// ULONG fl: id messaggio acustico (WA_WARNING, WA_NOTE, WA_ERROR)          |
// HWND hsbar: handle statusbar                                             |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID APIENTRY WshowMsg(ULONG idmsg, BOOL fl, HWND hsbar);

//==========================================================================\
// aggiorna dimensioni minime dialogo a seguito cambiamento spessore bordo  |
// finestra                                                                 |
// parametri:                                                               |
// HWND, MPARAM                                                             |
// PSSIZE ps = dimensioni minime finestra                                   |
// PSSIZE pb = dimensioni bordo finestra                                    |
// valore restituito:                                                       |
// MRESULT restituito dalla WinDefDlgProc()                                 |
//==========================================================================/

MRESULT APIENTRY WupdBrdSize(HWND hwnd, MPARAM mp1, MPARAM mp2,
                             PSSIZE ps, PSSIZE pb);

// helptable id
// title of the help window
// name of the help file
HWND APIENTRY WAppInitHelp(PCREATEAPP pca,
                  ULONG idHelpTable, PSZ pszTitle, PSZ pszHelpFile);

#endif
