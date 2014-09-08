//===========================================================================
// stlrTypes.h : definitions of general data types used by Styler/2
//
// --2003  - Alessandro Felice Cantatore
//===========================================================================


#ifndef _STLR_TYPES_H_
   #define _STLR_TYPES_H_

// cached system values -----------------------------------------------------
#pragma pack(1)
typedef struct {
   SIZEL scr;                  // screen size
   UCHAR ver;                  // OS version
   UCHAR cyTbar;               // titlebar height
   UCHAR xAlign;               // horizontal byte alignment
   UCHAR yAlign;               // vertical byte alignment
   UCHAR drv;                  // boot drive
   UCHAR cxVScroll;            // width of the vertical scrollbar
   UCHAR cyHScroll;            // height of the horizontal scrollbar
} SYSVALS, * PSYSVALS;
#pragma pack()

// default frame controls bitmap handles ------------------------------------
typedef struct {
   HBITMAP sys;        // system menu bitmap handle
   HBITMAP cls;        // close window bitmap handle
   HBITMAP min;        // minimize window bitmap handle
   HBITMAP hide;       // hide window bitmap handle
   HBITMAP rest;       // restore window bitmap handle
   HBITMAP max;        // maximize window bitmap handle
} FRAMECTLBMPS, * PFRAMECTLBMPS;

// essential window class data ----------------------------------------------
typedef struct {
   PFNWP pfnwp;             // indirizzo procedura default finestra
   ULONG cbData;            // dimensione window words originali
} CLASSDATA, * PCLASSDATA;

// superclassed window classes data -----------------------------------------
typedef struct {
   CLASSDATA frame;   // WC_FRAME data
   CLASSDATA tbar;    // WC_TITLEBAR data
   CLASSDATA ef;      // WC_ENTRYFIELD data
   CLASSDATA btn;     // WC_BUTTON data
   CLASSDATA menu;    // WC_MENU data
   CLASSDATA stat;    // WC_STATIC data
   CLASSDATA lbox;    // WC_LISTBOX data
   CLASSDATA combo;   // WC_COMBOBOX data
} SUPERCLASSDATA, * PSUPERCLASSDATA;

// relevant window handles --------------------------------------------------

typedef struct {
   HWND desktop;       // HWND_DESKTOP handle
   HWND winList;       // window list frame window handle
   HWND pref;          // preference notebook window handle
   volatile HWND deskFldr;  // handle of the frame of the desktop folder
   volatile HWND mou;  // handle of the window under the mouse pointer
   volatile HWND menu; // handle of the top menu window used in the sendmsg
                       // hook to avoid that menu dismission by the
                       // autofocus routine
   volatile HWND mainMouWin;  // handle of the main window owning mouse events
                              // this can be a frame window
   volatile HWND lastCreated; // handle of last created window
   volatile HWND top;         // top window handle
   volatile HWND active;      // active frame window handle
   volatile HWND autoFocus;   // handle of the window which started the
                              // autofocus timer
   volatile HWND hideTitle;   // handle of the window which started the
                              // hide-title-bar timer
   volatile HWND unhideTitle; // handle of the window which started the
                              // unhide-title-bar timer
   volatile HWND sysMenu;     // handle of the titlebar context menu
} SYSHWND, * PSYSHWND;

// system state -------------------------------------------------------------
typedef struct {
   UINT trap          : 16; // finta richiesta tempo per cracker
   UINT cinitmenu     : 16; // contatore inizializzazione menu per evitare
                            // che il menu venga dismesso dalla routine di
                            // autofocus
   ULONG now;               // giorni trascorsi dall'1/1/70
   UINT mousebtn      : 8;  // stato bottoni mouse/context-menu/movemouseptr
   UINT onctrl        : 8;  // flag che indica su quale controllo Š il mouse
   UINT expired       : 1;  // object windows failed or test time expired
   UINT ptrHidden     : 1;  // stato visibilit… puntatore mouse
   UINT hidePtrTmr    : 1;  // attivit… timer x nascondere puntatore mouse
//   UINT suspendHitTest: 1;  // flag sospensione elaborazione WM_HITTEST
   UINT resetWPS      : 1;  // indica che la WPS Š stata resettata
   UINT viomsel       : 1;  // indica inizio operazione selezione con mouse
   UINT viomcpy       : 1;  // indica che Š in corso un operazione di mark
                            // e copy con il mouse.
   UINT mou3btns      : 1;  // flag presenza mouse 3 bottoni
   UINT prvwinos2     : 1;  // la precedente finestra con il focus Š winos2
   UINT tracking      : 1;  // evita srotolamento finestre arrotolate
                            // quando spostate tramite drag titlebar
   UINT hideall       : 1;  // indica di nascondere tutte le finestre
   UINT rollall       : 1;  // indica di arrotolare tutte le finestre
   UINT wcbtn         : 1;  // flag che indica di monitorare i msg dei
                            // bottoni per riposizionare pseudomenu warpcenter
                            // sotto il mouse
   UINT winkfldr      : 1;  // flag che indica che una cartella Š stata aperta
                            // tramite una delle windows key per cui deve essere
                            // rilevato WM_ADJUSTWINDOWPOS aggiungendo le flag
                            // SWP_ACTIVATE | SWP_ZORDER
   UINT winkshld      : 1;  // flag che indica che una finestra di comandi Š
                            // stata aperta da una delle window keys per cui
                            // deve essere rilevato WM_ADJUSTWINDOWPOS per
                            // aggiungere flag SWP_ACTIVATE | SWP_ZORDER
   UINT locked        : 1;  // indica attivazione Lockup
   UINT unshift       : 1;  // flag cancellazione stato tasto shift
                            // durante emulazione doppio click mouse
   UINT captured      : 1;  // enhanced combo box procedure: the entryfield
                            // capture the mouse after a button1down event
} STATUS, * PSTATUS;

// titlebar drag handling structure -----------------------------------------

typedef struct {
   HWND hwnd;         // handle of the titlebar beeing dragged
   MPARAM mpPos;      // mouse position on drag start
   ULONG msg;         // end drag message (WM_BUTTON1UP or WM_BUTTON2UP)
} TITLEBARDRAG, * PTITLEBARDRAG;

// move-mouse-to-focus-window data ------------------------------------------
typedef struct {
   HEV hev;           // event semaphore create by the time critical thread
   volatile MPARAM mpHT;       // mouse position on previous WM_HITTEST
   volatile MPARAM mpMove;     // mouse position on previous WM_MOUSEMOVE
   volatile INT iSem;          // fast memeory semaphore
   HWND hwndNew;      // new created window handle
   HWND hwnd;         // window which is the target of the automatic mouse
                      // movement
   WRECTL wr;         // size of hwnd or mouse initial (wr.cx, wr.cy) and
                      // terminal (wr.x, wr.y) positions
                      // mouse e in wr.cx e wr.cy quella iniziale (increm.)
   POINTL dp;         // mouse motion increment (x 2^16)
   // to cancel the automatic mouse motion if the mouse is moved by the user
   POINTL ptc;        // current mouse position
} MOUMGR, *PMOUMGR;

// timer management thread data ---------------------------------------------

typedef struct {
   HAB hab;            // anchor block handle
   HWND hwnd;          // object window handle
} TIMERTHREAD, * PTIMERTHREAD;

// various flags ------------------------------------------------------------
// flag usata per terminare il tracking in caso di ridimensionamento
// verticale di una finestra arrotolata
#define TRKQUIT    0x10000000

// text strings used for the system menu extensions -------------------------
typedef struct {
   CHAR tbhide[64];    // hide/unhide titlebar
   CHAR winroll[64];   // roll/unroll
   CHAR alt[20];       // Alt
   CHAR ctrl[20];      // Ctrl
   CHAR shift[20];     // Shift
   CHAR ins[20];       // Ins
   CHAR backspace[20]; // Bkspc
   CHAR del[20];       // delete
} TEXTSTRINGS, * PTEXTSTRINGS;

// global data structure ----------------------------------------------------
typedef struct {
   SMWVER ver;         // struttura versione corrente
   GLOBPRCSDATA gpd;   // struttura dati processi attivi
   SYSVALS sys;        // various system values
   FRAMECTLBMPS bmpDef;// default bitmaps used by the titlebar buttons
   SYSHWND hwnd;       // handles of relevant windows
   SUPERCLASSDATA cd;  // superclassed window classes data
   TITLEBARDRAG tbdrag;// titlebar-drag handling
   volatile STATUS is; // system status
   MOUMGR mou;         // mouse management data
   TIMERTHREAD timerThrd;    // timer management thread data
   TEXTSTRINGS text;   // text strings used for system menu extensions
//   PFNWP fldrfproc;    // procedura originale (per subclass.) frame folder
//   ULONG closetime;    // mS dall'avvio al ricevimento ultimo WM_SYSCOMMAND
                       // SC_CLOSE (serve x evitare lo srotolamento delle
                       // finestre alla chiusura di VIEWDOC.EXE o NETSCAPE.EXE
} GLOBAL, * PGLOBAL;


#endif // #ifndef _STLR_TYPES_H_
