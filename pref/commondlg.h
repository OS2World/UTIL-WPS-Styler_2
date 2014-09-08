// commondlg.h

#ifndef COMMONDLG_H_
   #define COMMONDLG_H_

// programma editazione bitmap
#define SZEXE_ICONEDIT            "iconedit.exe"

// controlli da riposizionare tramite WinSetMultWindowPos()
#define BMPSELDLG_CTL1ST         LBOX_BMPSEL
#define BMPSELDLG_CTLLAST        BTN_BMPHELP
#define BMPSELDLG_CCTLS          BMPSELDLG_CTLLAST - BMPSELDLG_CTL1ST + 1

// flag stile dialogo in (indicano di visualizzare checkbox corrispondente)
// out (indicano che la corrispondente checkbox Š stata spuntata)
#define BMPSEL_BTNFIND       1      // aggiunge bottone "Find"
#define BMPSEL_DISABLE       2      // aggiunge checkbox "Disable"
#define BMPSEL_DEFAULT       4      // aggiunge checkbox "Default"
#define BMPSEL_LEFTSIDE      8      // aggiunge checkbox "Left side"
// ~BMPSEL_LEFTSIDE                  btn close posiz default
// BMPSEL_LEFTSIDE                   btn close sinistra
// BMPSEL_WIN95                      btn close posiz win95
#define BMPSEL_WIN95        0x10    // posizione bottone chiusura win95

//extern PSZ bmppath[];
//extern BYTE bmpdlgstyle[];
//extern PSZ bmpseldlgtitle[];
#define MAXBMFILESIZE 0x10000

typedef struct {
   SWCNTRL swctl;                   // usata x aggiungere nome task
   CHAR achTitle[128];              // titolo dialogo
   ULONG ulStyle;                   // in (stile dialogo)
   ULONG flSet;                     // in/out (stato checkbox)
   HPOINTER hico;                   // handle bitmap dialogo
   CHAR achPath[260];               // in path ricerca bmp (path\*.bmp)
   CHAR achBmp[256];                // in/out nome bitmap corrente/selezionata
   LONG idxCurr;                    // indice bitmap correntemente attiva
   LONG idxSel;                     // indice ultima bmp selezionata
   HBITMAP hbmp;                    // handle bitmap selezionata
   BYTE bmpdata[MAXBMFILESIZE];     // buffer per dati bitmap
   ULONG cxImage;                   // width of the image displayed in the list
//   SIZEL szBmp;                     // in/out dimensione bmp
//   INT cbFile;                      // size of the bitmap file
//   ULONG ccdata;                    // massima dimensione ammessa x la bitmap
   SIZEL szMin;                     // minima dimensione finestra
   HWND ahwnd[BMPSELDLG_CCTLS];     // array handle controlli
   SWP aswp[BMPSELDLG_CCTLS];       // array SWP controlli
   PROGDETAILS prg;                 // struttura avviamento iconedit.exe
   FILEDLG fdlg;                    // struttura x file dlg importazione bmp
   HAPP happ;                       // handle occorrenza iconedit.exe
} BMPSELDLG, * PBMPSELDLG;


// controlli da riposizionare tramite WinSetMultWindowPos()
#define FE_CTL1ST         TXT_FEFNCTEVNTS
#define FE_CTLLAST        SBAR_FEFNCTEVNT
#define FE_CCTLS          FE_CTLLAST - FE_CTL1ST + 1

// define per dialogo funzioni impostabili con mouse/tastiera
// sommate tramite | alle flag FEIDX_* per determinare item da selezionare
#define FE_NBPAGE       0x00000      // default = pagina notebook
#define FE_FLOATDLG     0x10000      // dialogo in finestra indipendente
#define FE_KEYB         0x00000      // default = dialogo tastiera
#define FE_MOUSECLK     0x20000      // dialogo funzioni mouse
#define FE_KBDINSDEL    0x40000      // dialogo tastiera (delete/Insert)
#define FE_KBDBID       0x80000      // dialogo tastiera (bkspc/delete/Insert)

typedef struct {
   UINT idf   : 16;       // descrizione funzione
   UINT idTrg : 16;       // target azione
   INT idx    : 16;       // indice inserzione item
   UINT fl    :  1;       // flag abilitazione (TRUE)/disabilitazione (FALSE)
   UINT ctl   :  1;       // flag TRUE = controllo Enable abilitato
   UINT click :  1;       // TRUE radio buttons click/doubleclick visibili
   UINT       :  0;
   UINT KC    : 16;       // KC flags
   UINT mouVK : 16;       // mousemsg/virtual keys
} FELIST, * PFELIST;

// struttura usata x dialogo settaggi mouse/tastiera (funzioni/eventi)
typedef struct {
   HPOINTER hico;                   // handle icona dialogo
   SWCNTRL swctl;                   // usata x aggiungere nome task
   SIZEL szMin;                     // minima dimensione finestra
   ULONG cCtls;                     // numero controlli
   HWND ahwnd[FE_CCTLS];            // array handle controlli
   SWP aswp[FE_CCTLS];              // array SWP controlli
   FELIST fel[20];                  // array di strutture FELIST
   ULONG svMouCM;                   // valore x undo SysvalueMouseContextMenu
   ULONG svMouOT;                   // valore x undo SysvalueMouseObjectTitle
   ULONG svKbdCM;                   // valore x undo SysvalueKbdContextMenu
   ULONG svKbdOT;                   // valore x undo SysvalueKbdObjectTitle
   LONG iBox;                       // indice selezione listbox
   LONG iSel;                       // indice FELIST item corrent selez
} FEDATA, * PFEDATA;

// lista virtualkeys + codice scansione << 8
#define VKEYLIST  {0x3b20, 0x3c21, 0x3d22, 0x3e23, 0x3f24, 0x4025, 0x4126,\
                   0x4227, 0x4328, 0x4429, 0x572a, 0x582b, 0x0e05, 0x691b,\
                   0x681a}
#define CVKEYLIST 15

// indici x array strutture FELIST
enum {FEIDX_ROLL, FEIDX_HTBAR, FEIDX_VIOWINCOPY, FEIDX_VIOWINAPND,
      FEIDX_VIOWINPASTE, FEIDX_KEYBASELAST,
      FEIDX_BKSZMOVE = FEIDX_KEYBASELAST, FEIDX_EFAUTOSEL, FEIDX_TXTCTLCOPY,
      FEIDX_TXTCTLCUT, FEIDX_TXTCTLINS, FEIDX_TXTCTLOVRWRITE,
      FEIDX_FRAMEDRAG, FEIDX_PUTBEHIND, FEIDX_MOULAST};
// accelleratori folder
enum {FEIDX_FLDRPRPRTS = FEIDX_KEYBASELAST, FEIDX_FLDROPPAR, FEIDX_FLDRFIND,
      FEIDX_FLDRSRTNAME, FEIDX_FLDRSRTEXT, FEIDX_FLDRSRTTYPE,
      FEIDX_FLDRSRTRNAME, FEIDX_FLDRSRTSIZE, FEIDX_FLDRSRTWRITE,
      FEIDX_FLDRSRTACCES, FEIDX_FLDRSRTCREAT, FEIDX_FLDRARRANGE,
      FEIDX_FLDRRFRSH, FEIDX_KEYLAST};


//#define FEIDX_ROLL               0    // arrotolamento finestre
//#define FEIDX_HTBAR              1    // rimozione barra titolo
//#define FEIDX_VIOWINCOPY         2    // copia testo VIO WINDOWS
//#define FEIDX_VIOWINAPND         3    // appende testo VIO in clipboard
//#define FEIDX_VIOWINPASTE        4    // incolla testo nelle VIO windows
//#define FEIDX_BKSZMOVE           5    // background size-move
//#define FEIDX_EFAUTOSEL          6    // autoselezione contenuto entry field
//#define FEIDX_TXTCTLCOPY         7    // copy text controls
//#define FEIDX_TXTCTLAPND         8    // append text controls
//#define FEIDX_TXTCTLCUT          9    // cut text controls
//#define FEIDX_TXTCTLINS         10    // insert text controls
//#define FEIDX_TXTCTLOVRWRITE    11    // overwrite textcontrols
//#define FEIDX_FRAMEDRAG         12    // trascina finestre dal bordo
//#define FEIDX_PUTBEHIND         13    // trascina finestra dietro tutte altre
//#define FEIDX_MOULAST           14    // numero items x mouse
//#define FEIDX_KEYLAST            5    // numero items x tastiera

// indice negli array dei tasti primari per voci superiori ai tasti funzione
#define FEKEYIDX_FKEYEND        12    // limite superiore tasti funzione
#define FEKEYIDX_BKSPC          12    // backspace
#define FEKEYIDX_DEL            13    // delete
#define FEKEYIDX_INS            14    // insert

// define x funzione MouKeybDlgTerminate()
#define FEDID_CANCEL    0          // premuto tasto cancel
#define FEDID_OK        1          // premuto tasto OK
#define FEDID_STORE     2          // cambio pagina

// funzioni bmpseldlg.c ---------------
BOOL AccLoadBmpSelDlg(HWND hwnd, INT idx, PULONG pfl);

// funzioni dialogo azioni mouse/tastiera
MRESULT EXPENTRY MouKeybDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID MouKbdUpdateFElist(HWND hwnd);
VOID MouKeybDlgUpdateStruct(HWND hwnd);
VOID MouKeybDlgDefault(HWND hwnd);

#endif
