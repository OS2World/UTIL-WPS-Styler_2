//===========================================================================
// stlrOldOptions.h : data structures used to handle Styler/2 options
//                    in versions older than 1.7
//===========================================================================


#ifndef _STLR_OLDOPTIONS_H_
   #define _STLR_OLDOPTIONS_H_

#define CB_FONTNAMEOLD    128 // size of the buffer reserved for the font name
#define TBMPSIZE       8192        // massima dimensione bmp titlebar
#define TBTNBMPSZ       408        // dimensioni bmp bottone titlebar
#define TBMPDEFCX       141        // larghezza bmp default barra titolo

// titlebar options (1 for active and 1 for inactive windows) ---------------
// titlebar background: 0 = tinta unita, 1 = sfumatura, 2 = bitmap
// border: 0 = piatto, 1 = in rilievo
// direzione sfumatura: 0 = orizzontale, 1 = verticale
// testo 3D: 0 = piatto, 1 = 3D

typedef struct {
   // opzioni
   UINT enabled    : 1;     // indica se opzioni attive
   UINT bkgnd      : 2;     // titlebar background
   UINT border     : 1;     // titlebar presenza bordo
   UINT shade      : 1;     // titlebar direzione sfumatura (1 = orizzontale)
   UINT _3DTxt     : 1;     // titlebar testo 3D
   UINT shd3       : 1;     // usa pseudo shade 3 colori (1 = 3 colori)
   UINT strch      : 1;     //* adatta la bitmap alla dimensione della titlebar
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
   BYTE bmp[TBMPSIZE];      // dati bmp bkgnd titlebar
   SIZEL bmpsize;           // dimensioni bmp background
} TBAROPTOLD, * PTBAROPTOLD;


// titlebar bitmaps and other options ---------------------------------------
// se il primo byte dei dati della bitmap Š 0 indica bitmap default

typedef struct {
   BYTE bmpSm[TBTNBMPSZ];   // dati bitmap system menu
   BYTE bmpCl[TBTNBMPSZ];   // bitmap bottone chiusura
   BYTE bmpHd[TBTNBMPSZ];   // bitmap bottone per nascondere
   BYTE bmpMn[TBTNBMPSZ];   // bitmap bottone minimizzazione
   BYTE bmpRs[TBTNBMPSZ];   // bitmap bottone ripristino
   BYTE bmpMx[TBTNBMPSZ];   // bitmap bottone massimizzazione
   BYTE bmpHtb[TBTNBMPSZ];  // bitmap bott. titlebar hide
   BYTE bmpUhtb[TBTNBMPSZ]; // bitmap bott. titlebar unhide
   BYTE bmpRl[TBTNBMPSZ];   // bitmap bott. arrotolamento finestre
   BYTE bmpUrl[TBTNBMPSZ];  // bitmap bott. srotolamento finestre
   CHAR achFont[CB_FONTNAMEOLD];       // font default barra titolo
   UINT close      : 2;     // 0 = bottone chiusura assente,
                            // 1 = bottone chiusura item del systemmenu,
                            // 2 = bottone chiusura posizione default,
                            // 3 = bottone chiusura posizione win95
   UINT htbar      : 1;     // TRUE aggiunge bottone titlebar hiding
   UINT roll       : 1;     // TRUE aggiunge bottone arrotolamento finestre
   UINT ovrPP      : 1;     // override presentation parameters
   UINT menu       : 1;     // system menu come menu contestuale titlebar
   UINT exc        : 1;     // indica presenza eccezioni
   UINT            : 0;
} TBARBUTTONSOLD, * PTBARBUTTONSOLD;     /* tbb, ptbb */


// all options --------------------------------------------------------------
typedef struct {
   TBAROPTOLD atb;          // opzioni titlebar finestra attiva
   TBAROPTOLD itb;          // opzioni titlebar finestra inattiva
   TBARBUTTONSOLD tbb;      // opzioni bottoni controllo finestra
   // opzioni generali
   UINT disabled      : 1;  // disabilita tutte funzioni smart windows
   UINT applyonexit   : 1;  // applica nuovi settaggi alla chiusura preferenze
   UINT saveonexit    : 1;  // salva nuovi settaggi alla chiusura preferenze
   UINT genexc        : 1;  // indica presenza eccezioni generali
   UINT loaded        : 1;  // indica che la dll Š stata caricata
                            // utilizzato dalla seconda dll
   UINT               : 0;
   int lastpage;            // indice ultima pagina selezionata (-1 = disabil)

   // main windows options ------------------------------------------------
   UINT MWfont        : 1;  // usa il font specificato x tutte le finestre
   UINT MWovrPP       : 1;  // override presentation parameters
   UINT MWonlyDlgs    : 1;  // usa il font solo x i dialoghi
   UINT MWcbFont      : 8;  // lunghezza stringa FontNameSize + 1
   UINT MWignorePP    : 1;  // ignore presentation parameters
   UINT MWexc         : 1;  // indica presenza eccezioni main windows
   UINT               : 0;
   CHAR MWFont[CB_FONTNAMEOLD];  // font usato nei dialoghi e nelle frame windows

   // titlebar hiding options ---------------------------------------------
   UINT HTon          : 1;  // modo titlehiding abilitato/disabilitato
   UINT HTexc         : 1;  // indica presenza eccezioni
   UINT HTsysMn       : 1;  // aggiunge voce hide titlebar al menu di sistema
   UINT HTmou         : 1;  // attivazione tramite mouse abilitata
   UINT HTkeyb        : 1;  // attivazione tramite keyboard abilitata
   UINT HTxclRld      : 1;  // obsoleto!!!!
   UINT HTxclDlg      : 1;  // esclude il titlebar hiding x le dialog windows
   UINT HTinclMn      : 1;  // nasconde anche il menu
   UINT HTredfld      : 1;  // riduce l'altezza dei folder
   UINT               : 0;
   ULONG HTPopUpDl;         // ritardo apparizione titlebar nascosto (0-2000)
   ULONG HTHideDl;          // ritardo hiding titlebar (0-2000)
   UINT HTmoumsg      : 16; // messaggio mouse x nascondere barratitolo
   UINT HTmoukey      : 16; // tasti associati con messaggio mouse (KC)
   UINT HTkeybKC      : 16; // KC_ flags
   UINT HTkeybVK      : 16; // VK_ flags

   // window roll ---------------------------------------------------------
   UINT RLon          : 1;  // window rooling abilitato
   //UINT wrap          : 1;  // modo wrapping abilitato/disabilitato
   UINT RLexc         : 1;  // indica presenza eccezioni
   UINT RLsysMn       : 1;  // aggiunge voce roll window al menu di sistema
   UINT RLmou         : 1;  // attivazione tramite mouse abilitata
   UINT RLkeyb        : 1;  // attivazione tramite keyboard abilitata
   UINT RLxclHT       : 1;  // esclude le finestre con barra titolo nascosta
   UINT RLaunrl       : 1;  // autounroll when getting focus
   UINT RLarl         : 1;  // autoroll when losing focus
   UINT RLvrsz        : 1;  // allow vertical resizing of rolled windows
   UINT RLvrszaunrl   : 1;  // autounroll vertical resized windows
   UINT RLbtm         : 1;  // TRUE=roll to the bottom
   UINT RLstore       : 1;  // store rolled state
   UINT RLaunrlcl     : 1;  // autounroll when closing window
   UINT               : 0;
   UINT RLmoumsg      : 16; // messaggio mouse x arotolare finestra
   UINT RLmoukey      : 16; // tasti associati con messaggio mouse (KC)
   UINT RLkeybKC      : 16; // KC_ flags
   UINT RLkeybVK      : 16; // VK_ flags

   // maximized windows options --------------------------------------------
   UINT MXOverOn        : 1;  // override default size and position
   UINT MXexc           : 1;  // indica presenza eccezioni
   UINT MXTileRld       : 1;  // autotile rolled windows
   UINT MXRlLF          : 1;  // always roll when losing focus
   UINT MXnomove        : 1;  // don't move maximized windows
   UINT MXnosize        : 1;  // don't resize maximized windows
   UINT                 : 0;
   INT MXx              : 16; // coordinata x finestra massimizzata
   INT MXy              : 16; // coordinata y finestra massimizzata
   UINT MXcx            : 16; // larghezza finestra massimizzata
   UINT MXcy            : 16; // altezza finestra massimizzata

   // size-move options ----------------------------------------------------
   UINT SZMVexc       : 1;  // indica presenza eccezioni
   UINT SZMVfulldrag  : 1;  // full window drag
   UINT SZMVnoalign   : 1;  // non allinea al byte coordinata x delle finestre
   UINT SZMVnull      : 1;  // obsoleto
   UINT SZMVsnap      : 1;  // sposta una finestra sul bordo dello schermo
   UINT SZMVframedrg  : 1;  // abilita frame dragging
   UINT SZMVZorder    : 1;  // spinge una finestra in fondo allo Z-order
   UINT SZMVzone      : 1;  // sposta lo Z order di una posizione
   UINT SZMVtodesk    : 1;  // da il focus al desktop dopo swap Z-order
   UINT SZMVnomoumove : 1;  // non sposta il mouse sulla focus win dopo Z-order
   UINT               : 0;
   UINT SZMVSmoumsg   : 16; // msg mouse sulla barra del titolo che sposta
                            // la finestra sui bordi dello schermo
   UINT SZMVSmoukey   : 16; // tasti associati con messaggio mouse (KC)
   UINT SZMVFmoumsg   : 16; // msg mouse che avvia framedrag
   UINT SZMVFmoukey   : 16; // tasti associati con messaggio mouse (KC)
   UINT SZMVZmoumsg   : 16; // msg mouse che sposta finestra bottom Zorder
   UINT SZMVZmoukey   : 16; // tasti associati con messaggio mouse (KC)

   // controlli testo
   UINT TxGexc        : 1;  // indica presenza eccezioni
   UINT TxGefsel      : 1;  // selezione automatica contenuto entry field
   //UINT EFTextSel     : 1;  // selezione automatica contenuto entry field
   UINT TxGMcpy       : 1;  // mouse copy/cut/paste
   UINT TxGMcpyEF     : 1;  // mouse copy/cut/paste EF
   UINT TxGMcpyMLE    : 1;  // mouse copy/cut/paste MLE
   UINT TxGMcpyStxt   : 1;  // mouse copy/cut/paste static text
   UINT TxGMcpyLbox   : 1;  // mouse copy/cut/paste Lbox
   UINT TxGMcpyBtn    : 1;  // copia testo bottoni
   UINT               : 0;
   UINT TxGefselMby   : 16; // msg mouse x selez autom entryfield
   UINT TxGefselKby   : 16; // tasti associati
   UINT TxGMcpyMby    : 16; // copy: msg mouse
   UINT TxGMcpyKby    : 16; // copy: tasti associati
   UINT TxGMapndMby   : 16; // append: msg mouse
   UINT TxGMapndKby   : 16; // append: tasti associati
   UINT TxGMcutMby    : 16; // cut: msg mouse
   UINT TxGMcutKby    : 16; // cut: tasti associati
   UINT TxGMinsMby    : 16; // insert: msg mouse
   UINT TxGMinsKby    : 16; // insert: tasti associati
   UINT TxGMovwMby    : 16; // overwrite: msg mouse
   UINT TxGMovwKby    : 16; // overwrite: tasti associati

   // finestre VIO
   UINT VIOexc        : 1;  // indica presenza eccezioni
   UINT VIOnoHscrl    : 1;  // elimina scroll bar orizzontale
   UINT VIOins        : 1;  // rende insert mode il default
   UINT VIOmcpy       : 1;  // copia con il mouse
   UINT VIOkcpy       : 1;  // copia con la tastiera
   UINT               : 0;
   UINT VIOmCmsg      : 16; // messaggio mouse x copiare
   UINT VIOmCkey      : 16; // tasti associati con messaggio mouse (KC)
   UINT VIOmAmsg      : 16; // messaggio mouse x copiare tutto testo
   UINT VIOmAkey      : 16; // tasti associati con messaggio mouse (KC)
   UINT VIOmPmsg      : 16; // messaggio mouse x incollare
   UINT VIOmPkey      : 16; // tasti associati con messaggio mouse (KC)
   UINT VIOkCVK       : 16; // virtual key x copiare
   UINT VIOkCKC       : 16; // tasti associati (KC)
   UINT VIOkAVK       : 16; // virtual key x appendere alla clipboard
   UINT VIOkAKC       : 16; // tasti associati (KC)
   UINT VIOkPVK       : 16; // virtual key x incollare
   UINT VIOkPKC       : 16; // tasti associati (KC)

   UINT mouexc        : 1;  // eccezioni generali opzioni mouse
   // sliding focus options
   UINT autoFocus     : 1;  // opzione autofocus
   UINT afNoDesktop   : 1;  // ignora desktop
   UINT afNoSwapZ     : 1;  // non cambia lo Z order della finestra attivata
   UINT afNotIfNew    : 1;  // ignora nuove finestre
   // <mouse pointer hiding> e <move to focus window> options
   UINT ptrHide       : 1;  // opzione mouse pointer hiding
   UINT xclMenu       : 1;  // esclude quando su menu
   UINT xclBtn        : 1;  // esclude quando su bottone
   UINT btn3          : 1;  // rimappa bottone 3 del mouse come doppio click
   UINT MPMFWon       : 1;  // mouse pointer move to focus window
   UINT MPMFWcreate   : 1;  // only on window creation
   UINT MPMFWdlg      : 1;  // only dialog windows
   UINT MPMFWxclnobtn : 1;  // exclude buttonless dialogs
   UINT btn3title     : 1;  // rimappa click tast 3 mouse anche su titlebar
   UINT MPMFsteps     : 10; // numero passi movimento mouse
   UINT               : 0;
   ULONG dlActivate;        // ritardo attivazione nuova finestra (1-999)
   ULONG dlPtrHide;         // ritardo hiding mouse ptr (1-99)

   // menu selection options
   UINT menuautosel   : 1;  // selezione automatica menu
   UINT menuinclbar   : 1;  // include action bar
   UINT dlMenuSelect  : 11; // ritardo attivazione action bar menu (max 2 sec)
   UINT               : 0;
   // opzioni accelleratori win95 key
   UINT W95lw         : 8;  // operazione associata con left win key
   UINT W95rw         : 8;  // operazione associata con right win key
   UINT W95menu       : 8;  // operazione associata con menu key
   UINT W95on         : 1;  // acceleratori win95 key attivi
   UINT               : 0;
   // opzioni acceleratori folder
   UINT FLDon         : 1;  // acceleratori folder attivi
   UINT FLDprprt      : 1;  // acceleratore apertura propriet… folder
   UINT FLDoppar      : 1;  // acceleratore open parent
   UINT FLDfind       : 1;  // acceleratore find
   UINT FLDsname      : 1;  // acceleratore sort by name
   UINT FLDsext       : 1;  // acceleratore sort by extension
   UINT FLDstype      : 1;  // acceleratore sort by type
   UINT FLDsrname     : 1;  // acceleratore sort by real name
   UINT FLDssize      : 1;  // acceleratore sort by size
   UINT FLDswrite     : 1;  // acceleratore sort by last write date
   UINT FLDsacc       : 1;  // acceleratore sort by last access date
   UINT FLDscreat     : 1;  // acceleratore sort by last create date
   UINT FLDarrange    : 1;  // acceleratore arrange
   UINT FLDrfrsh      : 1;  // acceleratore refresh
   UINT               : 0;
   UINT FLDprprtkc    : 16; // kc flags acceleratore apertura propriet… folder
   UINT FLDprprtkk    : 16; // key acceleratore apertura propriet… folder
   UINT FLDopparkc    : 16; // kc flags acceleratore open parent
   UINT FLDopparkk    : 16; // key acceleratore open parent
   UINT FLDfindkc     : 16; // kc flags acceleratore find
   UINT FLDfindkk     : 16; // key acceleratore find
   UINT FLDsnamekc    : 16; // kc flags acceleratore sort by name
   UINT FLDsnamekk    : 16; // key acceleratore sort by name
   UINT FLDsextkc     : 16; // kc flags acceleratore sort by extension
   UINT FLDsextkk     : 16; // key acceleratore sort by extension
   UINT FLDstypekc    : 16; // kc flags acceleratore sort by type
   UINT FLDstypekk    : 16; // key acceleratore sort by type
   UINT FLDsrnamekc   : 16; // kc flags acceleratore sort by real name
   UINT FLDsrnamekk   : 16; // key acceleratore sort by real name
   UINT FLDssizekc    : 16; // kc flags acceleratore sort by size
   UINT FLDssizekk    : 16; // key acceleratore sort by size
   UINT FLDswritekc   : 16; // kc flags acceleratore sort by last write date
   UINT FLDswritekk   : 16; // key acceleratore sort by last write date
   UINT FLDsacckc     : 16; // kc flags acceleratore sort by last access date
   UINT FLDsacckk     : 16; // key acceleratore sort by last access date
   UINT FLDscreatkc   : 16; // kc flags acceleratore sort by last create date
   UINT FLDscreatkk   : 16; // key acceleratore sort by last create date
   UINT FLDarrangekc  : 16; // kc flags acceleratore arrange
   UINT FLDarrangekk  : 16; // key acceleratore arrange
   UINT FLDrfrshkc    : 16; // kc flags acceleratore refresh
   UINT FLDrfrshkk    : 16; // key acceleratore refresh
} OPTIONSOLD, * POPTIONSOLD;

#endif // #ifndef _STLR_OLDOPTIONS_H_
