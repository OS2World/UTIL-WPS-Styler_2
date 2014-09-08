//===========================================================================
// stlrDefs.h : definitions of constants and data types used by the DLLs and
//              the preferences program
//
// --2003  - Alessandro Felice Cantatore
//===========================================================================


#ifndef _STLR_DEFS_H_
   #define _STLR_DEFS_H_


BOOL APIENTRY GpiSetBitmapOwner(HPS hps, HBITMAP hbmp, PID pid);

// resource module ----------------------------------------------------------
// nome modulo contenente dialoghi e stringhe
#define SZ_RESMODULE     "STLRES13"     // SMW001.DLL (risorse)
// logo bitmap
#define SZLOGONAME       "STLRES13\t1"
#define SZ_APPNAME       "Styler/2"
#define SZ_HLPFILE       "STYLER2.HLP"

// program version -----------------------------------------------------------
typedef struct {
   UINT Mver   : 8;     // version number principale
   UINT mver   : 8;     // version number secondario
   UINT rver   : 8;     // version number release
   UINT bver   : 8;     // version number build
} SMWVER, *PSMWVER;

// resources DLL - string IDs ------------------------------------------------

#define ISSMN_SYSMNTBHIDE       0x6000
#define ISSMN_SYSMNRLUNRL       0x6001
#define ISSMN_SYSMNALT          0x6002
#define ISSMN_SYSMNCTL          0x6003
#define ISSMN_SYSMNSHFT         0x6004
#define ISSMN_SYSMNINS          0x6005
#define ISSMN_SYSMNBKSPC        0x6006
#define ISSMN_SYSMNDEL          0x6007
#define ISSMN_SYSMNF1           0x6008
#define ISSMN_SYSMNF2           0x6009
#define ISSMN_SYSMNF3           0x6010
#define ISSMN_SYSMNF4           0x6011
#define ISSMN_SYSMNF5           0x6012
#define ISSMN_SYSMNF6           0x6013
#define ISSMN_SYSMNF7           0x6014
#define ISSMN_SYSMNF8           0x6015
#define ISSMN_SYSMNF9           0x6016
#define ISSMN_SYSMNF10          0x6017
#define ISSMN_SYSMNF11          0x6018
#define ISSMN_SYSMNF12          0x6019


// type of titlebar background -----------------------------------------------
#define TBARBKGNDDEF      0        // background titlebar = default
#define TBARBKGNDSHADE    1        // background titlebar = shade
#define TBARBKGNDBMP      2        // background titlebar = bitmap


// DLL resources - Bitmaps IDs -----------------------------------------------
#define IDBMP_TBA         12
#define IDBMP_TBI         13
#define IDBMP_NULL18     200
#define IDBMP_NULL22     201
#define IDBMP_CLOSE18    202
#define IDBMP_CLOSE22    203
#define IDBMP_HT18       204
#define IDBMP_HT22       205
#define IDBMP_UHT18      206
#define IDBMP_UHT22      207
#define IDBMP_RL18       208
#define IDBMP_RL22       209
#define IDBMP_URL18      210
#define IDBMP_URL22      211


// bitmap handle array indexes -----------------------------------------------
#define IBMP_SYSMENU       0
#define IBMP_CLOSE         1
#define IBMP_HIDE          2
#define IBMP_MIN           3
#define IBMP_MAX           4
#define IBMP_REST          5
#define IBMP_TBHIDE        6
#define IBMP_TBUNHIDE      7
#define IBMP_TBROLL        8
#define IBMP_TBUNROLL      9
#define IBMP_TBCORNER     10
// totale finestre comuni
#define CCMNBMPS          11


// modified-titlebar-setting flags -------------------------------------------

// dati barra titolo
#define MODIF_TBAR       0x0001     // dati barra titolo modificati
#define MODIF_FONT       0x0002     // font modificato
#define MODIF_ACTWIN     0x0004     // finestra attiva
#define MODIF_INACTWIN   0x0008     // finestra inattiva
#define MODIF_ALLTBAR    0x07ff     // poi ripristinare valore di sotto
//#define MODIF_ALLTBAR    0x000f     // usato x abilitazione/disabilitazione

// dati bottoni comuni
#define MODIF_SYSMN      0x0010     // btn system menu modificato
#define MODIF_CLOSEBMP   0x0020     // btn close modificata bmp
#define MODIF_CLOSEPOS   0x0040     // btn close modificata posizione
#define MODIF_HIDE       0x0080     // btn hide modificato
#define MODIF_MIN        0x0100     // btn min modificato
#define MODIF_REST       0x0200     // btn restore modificato
#define MODIF_MAX        0x0400     // btn maximize modificato

#define MODIF_TBHIDE     0x0800     // bmp bottone titlebar hiding
#define MODIF_TBUNHIDE   0x1000     // bmp bottone titlebar unhide
#define MODIF_TBHIDEUNHIDE         (MODIF_TBHIDE | MODIF_TBUNHIDE)

#define MODIF_WROLL      0x2000     // bmp bottone window rolling
#define MODIF_WUNROLL    0x4000     // bmp bottone window unroll
#define MODIF_WROLLUNROLL          (MODIF_WROLL | MODIF_WUNROLL)

#define MODIF_CMNBTNS    0xfff0     // abilitazione bottoni comuni

#define MODIF_ALLCMNBMP  0xffff     // bottoni e titlebar

#define AMODIFFLAGS \
   {MODIF_ACTWIN, MODIF_INACTWIN, MODIF_SYSMN, \
    MODIF_CLOSEBMP, MODIF_HIDE, MODIF_MIN, \
    MODIF_REST, MODIF_MAX}

// standard frame windows controls flags -------------------------------------
#define FCF_BASIC  \
(FCF_TITLEBAR | FCF_SYSMENU | FCF_HIDEBUTTON | FCF_MINMAX | FCF_CLOSEBUTTON)
// flag possibili indicanti presenza controlli frame alla sinistra della
// titlebar
#define FCF_LEFTCTLS \
(FCF_HIDEBUTTON | FCF_MINMAX | FCF_CLOSEBUTTON)
// flag frame che definiscono presenza di un titlebar button
#define FCF_TBARBTNS \
(FCF_SYSMENU | FCF_HIDEBUTTON | FCF_MINMAX | FCF_CLOSEBUTTON)


// timers IDs ----------------------------------------------------------------
#define TID_EXPIRATION      0x0200    // timer scadenza periodo prova
#define TID_MOUSEPTR        0x0201
#define TID_AUTOFOCUS       0x0202
#define TID_HIDETITLE       0x0203
#define TID_UNHIDETITLE     0x0204
#define TID_MENUACTIVATE    0x0205
#define TID_MOVEMOUPTR      0x0206    // spostamento mouse ptr su hwndfocus

#define DELAYMOVEMOUPTR     0        // mSecondi


// ID of the controls under the mouse pointer --------------------------------
enum {OCTLNORM, OCTLMENU, OCTLBTN, OCTLEF, OCTLTBAR, OCTLMLE};


// private PM messages -------------------------------------------------------
// #define SMWM_EXPIRED      (WM_USER + 10)
#define SMWM_DISABLE      (WM_USER + 11)
// #define SMWM_CHECKREG     (WM_USER + 12)
// #define SMWM_QRYREG       (WM_USER + 13) // mp1 = handle blocco pref
// #define SMWM_REGISTERED   (WM_USER + 13)
#define SMWM_RESETBITMAPS (WM_USER + 14)

// message processed by WC_FRAME windows -------------------------------------
#define WM_STYLER2        WM_QUERYFRAMEINFO   // global usage message


// Styler/2 PM messages ------------------------------------------------------

// flag ID msg Styler2:
#define CMDSRC_SMW         0xafc00000   // mp1 flag
#define CMDSRC_SMWMIN      0xafc00200   // mp1 flag min
#define CMDSRC_SMWMAX      0xafc00300   // mp1 flag max
// i seguenti ID sono usati come mp1 di WM_STYLER2 :
//#define STLR_TOGGLETITLEHIDE (0xafc00200)
//#define STLR_TOGGLEROLL      (0xafc00201)
#define STLR_ZORDACTIV       ((CMDSRC_SMW | TID_AUTOFOCUS))    // 0x0202
#define STLR_HIDETITLE       ((CMDSRC_SMW | TID_HIDETITLE))    // 0x0203
#define STLR_UNHIDETITLE     ((CMDSRC_SMW | TID_UNHIDETITLE))  // 0x0204
#define STLR_MENUACTIVATE    ((CMDSRC_SMW | TID_MENUACTIVATE)) // 0x0205
#define STLR_MOVEMOUPTR      ((CMDSRC_SMW | TID_MOVEMOUPTR))   // 0x0206
#define STLR_AUTOROLL        (0xafc00210)
#define STLR_AUTOUNROLL      (0xafc00211)
#define STLR_REDRAWTITLE     (0xafc00212) // forza il repaint della titlebar
#define STLR_EDITSYSMENU     (0xafc00213)
#define STLR_SETFRAMEFONT    (0xafc00214) // resetta font usato dalle frame
#define STLR_WROLLHIDECTLS   (0xafc00215) // nasconde controlli in caso rolling
#define STLR_AUTOUNRLONCLS   (0xafc00216) // srotola finestra alla chiusura
#define STLR_SUBCLASSFLDR    (0xafc00217) // subclassa i folder
#define STLR_HIDEALL         (0xafc00218) // nasconde/mostra tutte le finestre
#define STLR_ROLLALL         (0xafc00219) // arrotola/srotola tutte le finestre
#define STLR_PROCWKL         (0xafc00220) // processa wkey left
#define STLR_PROCWKR         (0xafc00221) // processa wkey right
#define STLR_PROCWKMENU      (0xafc00222) // processa la wkey menu
#define STLR_WARPCENTERCMD   (0xafc00223) // mostra la lista dei processi tramite
                                          // il warpcenter
#define STLR_INITWIN32       (0xafc00224) // complete initialization of win32
                                          // (odin) windows
#define STLR_INITCLIENTCLASS (0xafc00248)
#define STLR_DISABLED        (0xafc00249)
#define STLR_EXPIRED         (0xafc00250)

// subflags (mp2)
#define SW2_SFDEFAULT       0x0000
#define SW2_SFMOVETOBOTTOM  0x0001
#define SW2_SFMOVETOTOP     0x0002

// Styler/2 system command PM messages ---------------------------------------
// SYSTEM COMMAND speciali
#define SC_TOGGLETITLEHIDE 0x9230
#define SC_TOGGLEROLL      0x9231

// flag syscommand nelle VIO windows
#define SC_VIOMARK    0x9c      // mark nelle VIO windows
#define SC_VIOCOPY    0x9d      // copy nelle VIO windows
#define SC_VIOCPYALL  0x9e      // copy all nelle VIO windows
#define SC_VIOPASTE   0x9f      // paste nelle VIO windows


// mouse button state -------------------------------------------------------
#define DOWN1          0x01
#define DOWN2          0x02
#define DOWN3          0x04
#define MENUOPENED     0x08          // menu aperto
#define MOVEMOUPTR     0x10          // spostamento mouseptr su focus window


// windows key actions ------------------------------------------------------

enum {IWKEY_NONE,               // Do not perform any action
      IWKEY_HIDEWIN,            // Hide/show all windows
      IWKEY_ROLLWIN,            // Roll/unroll all windows
      IWKEY_WINLIST,            // Show window list
      IWKEY_CTXTMENU,           // Show context menu
      IWKEY_LOCKUP,             // Lockup the desktop
      IWKEY_FLDDRIVE,           // Open drive folder
      IWKEY_FLDSYSSETUP,        // Open system setup folder
      IWKEY_OS2CMD,             // Open OS/2 command window
      IWKEY_WCDESKMENU,         // Show WarpCenter desktop menu
      IWKEY_WCWINLIST,          // Show WarpCenter window list
      IWKEY_WCPROCLIST,         // Show WarpCenter process list
      IWKEY_WCTRAYLIST,         // Show WarpCenter tray list
      IWKEY_WCNEXTTRAY,         // Switch to the next WarpCenter tray
      IWKEY_WCINFOMENU};        // Show WarpCenter information menu

// additional RGB constants -------------------------------------------------

#define RGB_DARKGRAY            0x00808080
#define RGB_GRAY                0x00cccccc
#define RGB_DARKBLUE            0x00000080

// --------------------------------------------------------------------------
// macro cancella bitmap handle se non NULLHANDLE
#define _stlrFreeHBMP(x)    if (x) GpiDeleteBitmap(x), x = 0


// delete a global bitmap ---------------------------------------------------

#define _stlrFreeGlobalBitmap(_hps_, _hbmp_, _hab_)         \
   if (_hbmp_)                                              \
      GpiSetBitmapOwner((_hps_), (_hbmp_), (_hab_) >> 16),  \
      GpiDeleteBitmap(_hbmp_),                              \
      _hbmp_ = NULLHANDLE


// titlebar sample - window class -------------------------------------------
#define CC_FRAMESAMPLE   "ACCFrameSample"  // controlli superclassati

// --------------------------------------------------------------------------

// flag allocazione memoria shared per accesso clipboard:
#define FLCLIPMEM      \
        (PAG_COMMIT | PAG_READ | PAG_WRITE | OBJ_TILE | OBJ_GIVEABLE)

// undocumented OS2 flag used to prevent menu dismission when a new window
// is started

#ifndef WS_TOPMOST
   #define WS_TOPMOST  0x00200000
#endif

#endif // #ifndef _STLR_DEFS_H_
