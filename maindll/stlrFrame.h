//===========================================================================
// stlrFrame.h : data structures used by the superclassed frame controls
//
// --2003  - Alessandro Felice Cantatore
//===========================================================================


#ifndef _STLR_FRAME_H_
   #define _STLR_FRAME_H_

/* ---------------------------------------------------------------------------
 flags used by the hide-unneeded-folder-scrollers feature
*/
#define CANHIDESCROLL      (CV_TEXT|CV_NAME)
#define WPSFID_HORZSCRL     0x7ff3
#define WPSFID_VERTSCRL     0x7ff9

#define queryMiniCnrInfo(_hwnd_, _pbuf_) \
   WinSendMsg(_hwnd_, CM_QUERYCNRINFO,    \
              (MPARAM)(_pbuf_), (MPARAM)(offsetof(CNRINFO,ptlOrigin)))

#define canHideScroll(_hwnd_, _id_) \
   (_winStyle(_winHWND(_hwnd_, _id_)) & WS_DISABLED)



// resetFrameCtrls data -----------------------------------------------------
// this is used to add extra frame control buttons or extra system menu
// items or to restore the default frame controls
// the structure is dynamically allocated to minimize stack usage
typedef struct {
   CHAR buf[256];
   union {
      MENUITEM mi;
      FRAMECDATA fcd;
   } ;
   BOOL bAddMenu;     // FID_MINMAX added by Styler/2
   HWND hSubMenu;     // handle submenu of the system menu button
   PSZ feature;       // feature description (system menu item text)
   union {
      ULONG virtKey;     // hot key
      ULONG cStlrItems;  // frame controls added by Styler/2 to FID_MINMAX
   } ;
   union {
      ULONG ctrlKey;     // additional keys
      ULONG cCurItems;   // count of current FID_MINMAX items
   } ;
} SETFRAMECTRLS, * PSETFRAMECTRLS;

// RLon    : ä stato attivato il rolling
// rolled  : la finestra nello stato normale ä rollata
// max     : la finestra ä massimizzata
// maxroll : la finestra ä stata rollata mentre era massimizzata
// resroll : la finestra massimizzata rollata ä rollata anche nello stato
//           normale

// rclCur  : posiz/dimens correnti finestra
// rclRes  : posiz/dimens finestra stato normale non rollata
// rclMax  : posiz/dimens finestra massimizzata

#define CSWPRLD        16     // numero max controlli sx/dx titlebar
#define CH_CTLS        128    // numero elementi array handle controlli
#pragma pack(1)
typedef struct {
   USHORT viocx;          // usato per la funzione di eliminazione della
                          // scrollbar orizzontale dalla finestra VIO
   UCHAR viomark;         // indica che la finestra vio ha in corso marking
                          // vedi commento WM_TRANSLATEACCEL in winproc.c
   UCHAR tbbclose;        // stato bottone chiusura
//   UINT active      : 1;  // ha ricevuto messaggio WM_ACTIVATE
   UINT dialog      : 1;  // finestra dialogo
   UINT childdlg    : 1;  // finestra dialogo senza controlli nä bordo
                          // figlia di un'altra finestra
   UINT deskChild   : 1;  // la parent window ä HWND_DESKTOP
   UINT shield      : 1;  // ä una frame di una finestra VIO
   UINT folder      : 1;  // la frame ä "wpFolder window"
   UINT winos2      : 1;  // winOS2 window
   UINT winos2skip  : 1;  // winOS2 window to be ignored
   UINT dde         : 1;  // classe "PMDde Super Agent"
   UINT win32       : 1;  // win32 frame
   UINT min         : 1;  // finestra minimizzata
   UINT max         : 1;  // finestra ä massimizzata
   UINT remax       : 1;  // hide/show all window: maximize on re-show
   UINT titleHide   : 1;
   UINT titleHidden : 1;
   UINT menuIncluded: 1;  // mostra menu precedentemente nascosto anche
                          // se cambiano impostazioni globali relative
   UINT RLon        : 1;  // stile winrolling abilitato
   UINT rolled      : 1;  // finestra correntemente rolled(1)/unrolled(0)
   UINT maxroll     : 1;  // finestra rollata mentre era massimizzata
   UINT resroll     : 1;  // finestra max rolled, rolled anche normalmente
   UINT tbhilited   : 1;  // barra titolo attiva
   UINT resetparent : 1;  // hwnd deve essere resettato come parent child wins
//   UINT freeclient  : 1;  // non ä dlg e l'owner di FID_CLIENT == NULLHANDLE
//   UINT SMWfont     : 1;  // indica settaggio font SmartWindows in corso
   UINT disabled    : 1;  // finestra abilitata/disabilitata da modal dlg
   UINT tbactpp     : 1;  // ACTIVETITLE persparm presente
   UINT tbictpp     : 1;  // INACTIVETITLE persparm presente
   UINT newwin      : 1;  // finestra appena creata
                          // (resettata al primo WM_FOCUSCHANGE)
   UINT visible     : 1;  // flag visibilita' finestra
   UINT clientinit  : 1;  // inizializzazione dati relativi client window
   UINT skipuf      : 1;  // ignora WM_UPDATEFRAME generato da MM_SETITEMHANDLE
//   UINT ffrcr       : 1;  // ricorsione WM_FORMATFRAME
//   UINT tbarmenu    : 1;  // CONTEXTMENU sulla barra del titolo
//   UINT tbcinit     : 1;  // flag inizializzazione sysmenu e minmax
   UINT minMaxAdd   : 1;  // Styler/2 added FID_MINMAX
   UINT tbbhtbar    : 1;  // bottone x nascondere barra titolo
   UINT smnhtbar    : 1;  // item sysmenu x nascondere titlebar
   UINT tbbroll     : 1;  // bottone x arrotolare finestra
   UINT smnroll     : 1;  // item sysmenu x arrotolare finestra
   UINT launchpad   : 1;  // la finestra ä la launchpad o un cassetto
   UINT showinit    : 1;  // inizializzazione finestra al primo WM_SHOW
   UINT fcschngd    : 1;  // la finestra ha appena acquisito il focus
   UINT ovrdnomovemax : 1;// override funzione no move maximized windows
                          // se la finestra massimizzata ha dimensioni inferiore
                          // a quella di default
   UINT fldredit    : 1;  // indica che ä in corso l'editazione del titolo
                          // di un oggetto contenuto in un folder. Serve per
                          // sospendere gli acceleratori di tastiera
   UINT vioins      : 1;  // indica che la finestra vio ä in autoinsert mode
   UINT allhidden   : 1;  // indica finestra nascosta dal comando hide all
   UINT allrolled   : 1;  // indica finestra arrotolata dal comando roll all
   UINT trkquit     : 1;  // se la finestra ä in tracking indica di interromperlo
   UINT lockupwin   : 1;  // indica che ä la finestra di lockup
   UINT test        : 1;
   UINT             : 0;
} WINSTATUS, * PWINSTATUS;
#pragma pack()
// rolled window controls ---------------------------------------------------
// struttura controlli finestra arrotolata usati x nascondere e riposizionare
// i controlli
typedef struct {
   ULONG cLc;            // numero controlli alla sinistra della titlebar
   SWP ppLc[CSWPRLD];    // array strutture SWP controlli
   ULONG cRc;            // numero controlli alla destra della titlebar
   SWP ppRc[CSWPRLD];    // array strutture SWP controlli
   ULONG chhc;           // numero controlli da nascondere
   HWND phhc[CH_CTLS];   // array handle controlli da nascondere
} RLDWINCTLS, * PRLDWINCTLS;

typedef struct {
   MENUITEM mi;              // usato x caching riposizionamento controlli fin
//   ULONG flwctl;             // flag modifiche controlli finestra
   HAB hab;                  // HAB della frame
   HWND hwnd;                // handle frame
   ULONG ffl;                // frameflags
   ULONG fstyle;             // window style
   HWND hSMenu;              // handle FID_SYSMENU
   HWND hSsubMenu;           // handle submenu system menu
//   HWND hwMenu;              // handle menu window (hide/minimizi
   HWND hTBar;               // handle FID_TITLEBAR
   TBBMP atb;                // dati titlebar attiva
   TBBMP itb;                // dati titlebar inattiva
//   HWND hOwner;              // handle owner window
   HWND hMinMax;             // handle FID_MINMAX
   HWND hMenu;               // handle FID_MENU
   HWND hClient;             // handle FID_CLIENT
   WINSTATUS is;             // stato finestra corrente
   CHAR achTxt[260];         // testo titolo finestra
   ULONG cchTxt;             // lunghezza stringa titolo
   LONG yfont;               // fm.lMaxDescender
   LONG cyfont;              // fm.lMaxBaselineExt
   LONG cxfont;              // larghezza intera stringa
   PSZ pszFont;              // nome font usato nei dialoghi
   ULONG cchFont;            // dimensione buffer font
   RLDWINCTLS rwc;           // dati controlli finestra arrotolata
//   HWND hCtl[CH_CTLS];       // array handle controlli nascosti se rolling
   POINTL ptBrd;             // spessore x,y bordo finestra
   ULONG cyRoll;             // altezza finestra rollata
   ULONG yMenu;              // coordinata y barra menu
   WRECTL rclRst;            // rettangolo posiz/dimens restore
   WRECTL rclCur;            // rettangolo posiz/dimens reali
   WRECTL rclMax;            // rettangolo finestra massimizzata
   WRECTL wrclTitle;         // dimensioni titlebar
   SIZEL szlssbmn;           // dimensioni system submenu
   HPS hps;                  // spazio presentazione
   TRACKINFO ti;             // caching x evitare stack overflow
   PFNWP pffrm;              // procedura originale frame
   PFNWP pfclnt;             // procedura originale client
} STLRFRAME, * PSTLRFRAME;


#pragma pack(1)
typedef struct {
   SIZEL size;               // window size
   PSZ pszText;              // window text
   USHORT cbText;            // text length in bytes
   USHORT cxText;            // text width in pixel
   UCHAR cyText;             // text height in pixel
   UINT isHilited ;
   UINT isPPa;
} STLRTITLEBAR, * PSTLRTITLEBAR;
#pragma pack()

// frame-tracking/full-window-drag ------------------------------------------
typedef struct {
   // struttura TRACKINFO standard
   LONG    cxBorder;
   LONG    cyBorder;
   LONG    cxGrid;
   LONG    cyGrid;
   LONG    cxKeyboard;
   LONG    cyKeyboard;
   RECTL   rclTrack;
   RECTL   rclBoundary;
   POINTL  ptlMinTrackSize;
   POINTL  ptlMaxTrackSize;
   ULONG   fs;
   // fine struttura standard
   BOOL bAct;    // if TRUE the frame window must be activated
   PSTLRFRAME p; // dati finestra in corso di tracking
   HWND hpar;    // parent window
   QMSG qmsg;    // handle message queue usato nell'hook TRACKFRAME
   WRECTL wr;    // posizione e dimensioni originali finestra
   ULONG msg;    // msg WM_BUTTONxUP chiusura tracking
   POINTL ptl;   // posizione precedente mouse
   HWND hwndModal;
} FULLWDRAG, *PFULLWDRAG;

#define FL_WIN32FRAME       ((PSTLRFRAME)1)
#define isWin32Frame(_hwnd_) \
   (FL_WIN32FRAME == WinQueryWindowPtr(_hwnd_, g.cd.frame.cbData)))

#endif // #ifndef _STLR_FRAME_H_
