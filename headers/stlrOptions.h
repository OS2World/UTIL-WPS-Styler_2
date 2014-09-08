//===========================================================================
// stlrOptions.h : data structures used to handle Styler/2 options
//
// --2003  - Alessandro Felice Cantatore
//
// Note: to look for features implementation search in the code:
// õ                    :
// õ                    :
// õ                    :
// õ DialogFont         : font used for dialog windows
// õ                    :
// õ                    :
// õ                    :
// õ FrameControls      :
// õ                    :
// õ                    :
// õ MaximizedWindows   :
// õ                    :
// õ MouseAutoFocus     : give focus to the window under the mouse pointer
// õ                    :
// õ MouseMoveToFocus   : move the mouse pointer to the focus window
// õ                    :
// õ                    :
// õ                    :
// õ                    :
// õ                    :
// õ                    :
// õ                    :
// õ                    :
// õ                    :
// õ WinRolling         : arrotolamento/srotolamento finestre
// õ                    :
// õ                    :
// õ WinSizePos         : windows size-position enhancements
// õ                    :
//
//===========================================================================


#ifndef _STLR_OPTIONS_H_
   #define _STLR_OPTIONS_H_


#define CB_FONTNAME   64 // size of the buffer reserved for the font name

// general page options -----------------------------------------------------
typedef struct  {
   UINT disabled      : 1;  // disabilita tutte funzioni smart windows
   UINT applyoe       : 1;  // applica nuovi settaggi alla chiusura preferenze
   UINT saveoe        : 1;  // salva nuovi settaggi alla chiusura preferenze
   UINT exc           : 1;  // indica presenza eccezioni generali
   UINT loaded        : 1;  // indica che la dll Š stata caricata
                            // utilizzato dalla seconda dll
   UINT               : 0;
   int lastpage;            // indice ultima pagina selezionata (-1 = disabil)

} GENERALO, * PGENEARALO;

// dialog windows options ---------------------------------------------------
typedef struct {
   UINT cbFont        : 8; // lunghezza stringa FontNameSize + 1
   UINT font          : 1;  // usa il font specificato x tutte le finestre
   UINT ovrPP         : 1;  // override presentation parameters
   UINT exc           : 1;  // indica presenza eccezioni main windows
   UINT               : 0;
   CHAR achFont[CB_FONTNAME];  // font usato nei dialoghi e nelle frame windows
} DIALOGO, * PDIALOGO;

// active/inactive titlebar options -----------------------------------------
// titlebar background: 0 = tinta unita, 1 = sfumatura, 2 = bitmap
// border: 0 = piatto, 1 = in rilievo
// direzione sfumatura: 0 = orizzontale, 1 = verticale
// testo 3D: 0 = piatto, 1 = 3D

typedef struct {
   // opzioni
   UINT bkgnd      : 8;     // titlebar background
   UINT border     : 1;     // titlebar presenza bordo
   UINT shade      : 1;     // titlebar direzione sfumatura (1 = verticale)
   UINT _3DTxt     : 1;     // titlebar testo 3D
   UINT shd3       : 1;     // usa pseudo shade 3 colori (1 = 3 colori)
   UINT strch      : 1;     // adatta la bitmap alla dimensione della titlebar
   UINT center     : 1;     // indica di centrare il testo
   UINT            : 0;
   // colori
   CLR clrLeftTop;          // colore solid background/left-shade/top-shade
   CLR clrRghtBtm;          // colore right/bottom shade
   CLR clrHiBkgnd;          // colore hilite background
   CLR clrShBkgnd;          // colore shadow background
   CLR clrFgTxt;            // colore foreground testo
   CLR clrBgTxt;            // colore background testo
   // altri dati titolo
   HBITMAP hbmp;            // handle bmp bkgnd titlebar
   SIZEL size;              // dimensioni bmp background
} TBARAIOPT, * PTBARAIOPT;

#define CBTBAIOPTCMP (sizeof(TBARAIOPT) - sizeof(HBITMAP) - sizeof(SIZEL))

// titlebar button bitmaps --------------------------------------------------

typedef struct {
   HBITMAP sysmenu;         // dati bitmap system menu
   HBITMAP close;           // bitmap bottone chiusura
   HBITMAP hide;            // bitmap bottone per nascondere
   HBITMAP min;             // bitmap bottone minimizzazione
   HBITMAP rest;            // bitmap bottone ripristino
   HBITMAP max;             // bitmap bottone massimizzazione
   HBITMAP htbar;           // bitmap bott. titlebar hide
   HBITMAP unhtbar;         // bitmap bott. titlebar unhide
   HBITMAP roll;            // bitmap bott. arrotolamento finestre
   HBITMAP unroll;          // bitmap bott. srotolamento finestre
} TBBBMPS, * PTBBBMPS;

// titlebar button options --------------------------------------------------
typedef struct {
   TBARAIOPT a;             // active titlebar options
   TBARAIOPT i;             // inactive titlebar options
   TBBBMPS bmp;             // handle of the bitmaps used to draw titlebar buttons
   CHAR achFont[CB_FONTNAME];       // font default barra titolo
   UINT close      : 8;     // 0 = bottone chiusura assente,
                            // 1 = bottone chiusura item del systemmenu,
                            // 2 = bottone chiusura posizione default,
                            // 3 = bottone chiusura posizione win95
   UINT on         : 1;     // indica se opzioni attive
   UINT htbar      : 1;     // TRUE aggiunge bottone titlebar hiding
   UINT roll       : 1;     // TRUE aggiunge bottone arrotolamento finestre
   UINT ovrPP      : 1;     // override presentation parameters
   UINT menu       : 1;     // system menu come menu contestuale titlebar
   UINT exc        : 1;     // indica presenza eccezioni
   UINT addMinMax  : 1;     // version 1.7.4.0 : add the close button (and
                            // other optional buttons) if the system menu
                            // is present
   UINT            : 0;
} TITLEBARO, * PTITLEBARO;     /* tb, ptb */

// titlebar hiding options --------------------------------------------------
typedef struct {
   UINT on            : 1;  // modo titlehiding abilitato/disabilitato
   UINT exc           : 1;  // indica presenza eccezioni
   UINT sysMn         : 1;  // aggiunge voce hide titlebar al menu di sistema
   UINT mou           : 1;  // attivazione tramite mouse abilitata
   UINT kbd           : 1;  // attivazione tramite keyboard abilitata
   UINT noDlg         : 1;  // esclude il titlebar hiding x le dialog windows
   UINT inclMn        : 1;  // nasconde anche il menu
   UINT redfld        : 1;  // riduce l'altezza dei folder
   UINT               : 0;
   ULONG unhideDelay;       // ritardo apparizione titlebar nascosto (0-2000)
   ULONG hideDelay;         // ritardo hiding titlebar (0-2000)
   UINT moumsg        : 16; // messaggio mouse x nascondere barratitolo
   UINT moukbd        : 16; // tasti associati con messaggio mouse (KC)
   UINT kbdKC         : 16; // KC_ flags
   UINT kbdVK         : 16; // VK_ flags
} TBARHIDEO, * PTBARHIDEO;

// window rolling -----------------------------------------------------------
typedef struct {
   UINT on            : 1;  // window rooling abilitato
   UINT exc           : 1;  // indica presenza eccezioni
   UINT sysMn         : 1;  // aggiunge voce roll window al menu di sistema
   UINT mou           : 1;  // attivazione tramite mouse abilitata
   UINT kbd           : 1;  // attivazione tramite keyboard abilitata
   UINT unrollOnFocus : 1;  // autounroll when getting focus
   UINT rollOnUnfocus : 1;  // autoroll when losing focus
   UINT letVsize      : 1;  // allow vertical resizing of rolled windows
   UINT unrollOnVsize : 1;  // autounroll vertical resized windows
   UINT bottom        : 1;  // TRUE=roll to the bottom
   UINT unrollOnClose : 1;  // autounroll when closing window
   UINT               : 0;
   UINT moumsg        : 16; // messaggio mouse x arrotolare finestra
   UINT moukbd        : 16; // tasti associati con messaggio mouse (KC)
   UINT kbdKC         : 16; // KC_ flags
   UINT kbdVK         : 16; // VK_ flags
} WINROLLO, * PWINROLLO;

// maximized windows options ------------------------------------------------
typedef struct {
   UINT on            : 1;  // override default size and position
   UINT exc           : 1;  // indica presenza eccezioni
   UINT nomove        : 1;  // don't move maximized windows
   UINT nosize        : 1;  // don't resize maximized windows
   UINT               : 0;
   INT x              : 16; // coordinata x finestra massimizzata
   INT y              : 16; // coordinata y finestra massimizzata
   UINT cx            : 16; // larghezza finestra massimizzata
   UINT cy            : 16; // altezza finestra massimizzata
} MAXWINO, * PMAXWINO;

// size-move options --------------------------------------------------------
typedef struct {
   UINT exc           : 1;  // indica presenza eccezioni
   UINT fulldrag      : 1;  // full window drag
   UINT noalign       : 1;  // non allinea al byte coordinata x delle finestre
   UINT snap          : 1;  // sposta una finestra sul bordo dello schermo
   UINT framedrg      : 1;  // abilita frame dragging
   UINT Zmove         : 1;  // spinge una finestra in fondo allo Z-order
   UINT Zone          : 1;  // sposta lo Z order di una posizione
//   UINT focusDesk    : 1;  // da il focus al desktop dopo swap Z-order
//   UINT noMouMove    : 1;  // non sposta il mouse sulla focus win dopo Z-order
   UINT               : 0;
   UINT snapMoumsg    : 16; // msg mouse sulla barra del titolo che sposta
                            // la finestra sui bordi dello schermo
   UINT snapMoukbd    : 16; // tasti associati con messaggio mouse (KC)
   UINT frameMoumsg   : 16; // msg mouse che avvia framedrag
   UINT frameMoukbd   : 16; // tasti associati con messaggio mouse (KC)
   UINT ZordMoumsg    : 16; // msg mouse che sposta finestra bottom Zorder
   UINT ZordMoukbd    : 16; // tasti associati con messaggio mouse (KC)
} SIZEPOSITIONO, * PSIZEPOSITIONO;

// text controls options ----------------------------------------------------
typedef struct {
   UINT exc           : 1;  // indica presenza eccezioni
   UINT efsel         : 1;  // selezione automatica contenuto entry field
   UINT on            : 1;  // mouse copy/cut/paste
   UINT ef            : 1;  // mouse copy/cut/paste EF
   UINT mle           : 1;  // mouse copy/cut/paste MLE
   UINT stat          : 1;  // mouse copy/cut/paste static text
   UINT lbox          : 1;  // mouse copy/cut/paste Lbox
   UINT btn           : 1;  // copia testo bottoni
   UINT               : 0;
   UINT selMoumsg     : 16; // msg mouse x selez autom entryfield
   UINT selMoukbd     : 16; // tasti associati
   UINT cpyMoumsg     : 16; // copy: msg mouse
   UINT cpyMoukbd     : 16; // copy: tasti associati
   UINT cutMoumsg     : 16; // cut: msg mouse
   UINT cutMoukbd     : 16; // cut: tasti associati
   UINT insMoumsg     : 16; // insert: msg mouse
   UINT insMoukbd     : 16; // insert: tasti associati
   UINT ovwMoumsg     : 16; // overwrite: msg mouse
   UINT ovwMoukbd     : 16; // overwrite: tasti associati
   // I seguenti valori sono settati dalla DLL alla sua inizializzazione
   // (o dal programma delle preferenze) e dipendono dagli eventi del mouse
   // selezionati per le operazioni di clipboard nelle finestre di testo
   UINT skipcpy       : 16;
   UINT skipcut       : 16;
   UINT skippaste     : 16;
   UINT skipovwrt     : 16;
} TEXTCTLSO, * PTEXTCTLSO;

// VIO windows options ------------------------------------------------------
typedef struct {
   UINT exc           : 1;  // indica presenza eccezioni
   UINT noscrl        : 1;  // elimina scroll bar orizzontale
   UINT ins           : 1;  // rende insert mode il default
   UINT mou           : 1;  // copia con il mouse
   UINT kbd           : 1;  // copia con la tastiera
   UINT               : 0;
   UINT cpyMoumsg     : 16; // messaggio mouse x copiare
   UINT cpyMoukbd     : 16; // tasti associati con messaggio mouse (KC)
   UINT cpyaMoumsg    : 16; // messaggio mouse x copiare tutto testo
   UINT cpyaMoukbd    : 16; // tasti associati con messaggio mouse (KC)
   UINT insMoumsg     : 16; // messaggio mouse x incollare
   UINT insMoukbd     : 16; // tasti associati con messaggio mouse (KC)
   UINT cpyKbdVK      : 16; // virtual key x copiare
   UINT cpyKbdKC      : 16; // tasti associati (KC)
   UINT cpyaKbdVK     : 16; // virtual key x appendere alla clipboard
   UINT cpyaKbdKC     : 16; // tasti associati (KC)
   UINT insKbdVK      : 16; // virtual key x incollare
   UINT insKbdKC      : 16; // tasti associati (KC)
} VIOWINO, * PVIOWINO;

// mouse options ------------------------------------------------------------
typedef struct {
   UINT exc           : 1;  // eccezioni generali opzioni mouse
   // sliding focus options
   UINT focusAuto     : 1;  // opzione autofocus
   UINT focusNoDesk   : 1;  // ignora desktop
   UINT focusNoTop    : 1;  // non cambia lo Z order della finestra attivata
   UINT focusNoNew    : 1;  // ignora nuove finestre
   // mouse pointer hiding
   UINT hide          : 1;  // opzione mouse pointer hiding
   UINT hideNotOnMenu : 1;  // esclude quando su menu
   UINT hideNotOnBtn  : 1;  // esclude quando su bottone
   // middle mouse button options
   UINT btn3dblClk    : 1;  // rimappa bottone 3 del mouse come doppio click
   UINT btn3title     : 1;  // rimappa click tast 3 mouse anche su titlebar
   // mouse move to focus window options
   UINT moveToFocus   : 1;  // mouse pointer move to focus window
   UINT moveToNew     : 1;  // only on window creation
   UINT moveToDlg     : 1;  // only dialog windows
   UINT moveNotBtnles : 1;  // exclude buttonless dialogs
   UINT align         : 2;  // alignment
   UINT moveSteps     : 10; // numero passi movimento mouse
   UINT               : 0;
   // other
   ULONG focusDelay;        // ritardo attivazione nuova finestra (1-999)
   ULONG hideDelay;         // ritardo hiding mouse ptr (1-99)
} MOUSEO, * PMOUSEO;

// Windows 9x keyboards extensions ------------------------------------------
typedef struct {
   UINT left          : 8;  // operazione associata con left win key
   UINT right         : 8;  // operazione associata con right win key
   UINT menu          : 8;  // operazione associata con menu key
   UINT on            : 1;  // acceleratori win95 key attivi
   UINT               : 0;
} WIN95KBDO, * PWIN95KBDO;

// folder windows keyboard accelerators -------------------------------------
typedef struct {
   UINT on            : 1;  // acceleratori folder attivi
   UINT sets          : 1;  // acceleratore apertura propriet… folder
   UINT parent        : 1;  // acceleratore open parent
   UINT find          : 1;  // acceleratore find
   UINT srtname       : 1;  // acceleratore sort by name
   UINT srtext        : 1;  // acceleratore sort by extension
   UINT srttype       : 1;  // acceleratore sort by type
   UINT srtrname      : 1;  // acceleratore sort by real name
   UINT srtsize       : 1;  // acceleratore sort by size
   UINT srtwrite      : 1;  // acceleratore sort by last write date
   UINT srtacc        : 1;  // acceleratore sort by last access date
   UINT srtcreat      : 1;  // acceleratore sort by last create date
   UINT arrange       : 1;  // acceleratore arrange
   UINT refresh       : 1;  // acceleratore refresh
   UINT               : 0;
   UINT setsKC        : 16; // kc flags acceleratore apertura propriet… folder
   UINT setsK         : 16; // key acceleratore apertura propriet… folder
   UINT parentKC      : 16; // kc flags acceleratore open parent
   UINT parentK       : 16; // key acceleratore open parent
   UINT findKC        : 16; // kc flags acceleratore find
   UINT findK         : 16; // key acceleratore find
   UINT srtnameKC     : 16; // kc flags acceleratore sort by name
   UINT srtnameK      : 16; // key acceleratore sort by name
   UINT srtextKC      : 16; // kc flags acceleratore sort by extension
   UINT srtextK       : 16; // key acceleratore sort by extension
   UINT srttypeKC     : 16; // kc flags acceleratore sort by type
   UINT srttypeK      : 16; // key acceleratore sort by type
   UINT srtrnameKC    : 16; // kc flags acceleratore sort by real name
   UINT srtrnameK     : 16; // key acceleratore sort by real name
   UINT srtsizeKC     : 16; // kc flags acceleratore sort by size
   UINT srtsizeK      : 16; // key acceleratore sort by size
   UINT srtwriteKC    : 16; // kc flags acceleratore sort by last write date
   UINT srtwriteK     : 16; // key acceleratore sort by last write date
   UINT srtaccKC      : 16; // kc flags acceleratore sort by last access date
   UINT srtaccK       : 16; // key acceleratore sort by last access date
   UINT srtcreatKC    : 16; // kc flags acceleratore sort by last create date
   UINT srtcreatK     : 16; // key acceleratore sort by last create date
   UINT arrangeKC     : 16; // kc flags acceleratore arrange
   UINT arrangeK      : 16; // key acceleratore arrange
   UINT refreshKC     : 16; // kc flags acceleratore refresh
   UINT refreshK      : 16; // key acceleratore refresh
} FOLDERKBDO, * PFOLDERKBDO;

// all options --------------------------------------------------------------
typedef struct {
   GENERALO gen;            // opzioni generali
   DIALOGO dlg;             // dialog options
   TITLEBARO tb;            // titlebar options
   TBARHIDEO tbh;           // titlebar hiding options
   WINROLLO roll;           // window rolling options
   MAXWINO maxwin;          // maximized window options
   SIZEPOSITIONO sizpos;    // window size and position options
   TEXTCTLSO txt;           // text controls options
   VIOWINO vio;             // VIO windows options
   MOUSEO mou;              // mouse options
   WIN95KBDO winkbd;        // Windows 9x keyboard extension
   FOLDERKBDO wpskbd;       // folder windows keyboard accelerators
} OPTIONS, * POPTIONS;

#endif // #ifndef _STLR_OPTIONS_H_
