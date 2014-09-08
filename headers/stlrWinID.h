//==========================================================================\
// interfaccia                                                              |
//==========================================================================/

#ifndef WINID_H_
   #define WINID_H_

#define CMAINCTLS            9     // numero controlli dialogo principale

#define MINCY              100   // altezza minima formattazione controlli

// dimensioni standard dialogo pagine notebook
#define CX_PAGEDLG         220
#define CY_PAGEDLG         156


// Help table IDs -----------------------------------------------------------
#define HLP_MAIN          1
#define HLPS_MAIN         10
#define HLPS_BMPSEL       11
#define HLPS_MOUOPT       12
#define HLPS_INSTALL      13
#define HLPS_REGISTER     14

// help topics IDs ----------------------------------------------------------
#define HLPID_INSTALL      5
#define HLPID_MAIN         6
#define HLPID_GENERAL      7
#define HLPID_EXCEPTION    8
#define HLPID_DIALOG       9
#define HLPID_TITLEBAR    10
#define HLPID_BMPSEL      11
#define HLPID_TBARHIDE    12
#define HLPID_WROLL       13
#define HLPID_MAXWIN      14
#define HLPID_MOTION      15
#define HLPID_TEXT        16
#define HLPID_VIO         17
#define HLPID_MOUGEN      18
#define HLPID_MOUOPT      19
#define HLPID_WINKEY      20
#define HLPID_REGISTER    23

// ID comune a tutti i dialoghi ---------------------------------------------
#define GRP                    2

// object window ------------------------------------------------------------
#define ID_OBJWIN           48

// dialogo principale -------------------------------------------------------
#define DLG_MAIN_3         (ID_OBJWIN + 1)
#define DLG_MAIN           (DLG_MAIN_3 + 1)
#define PREF_NOTEBOOK      (DLG_MAIN + 1)
#define STBR_MAIN          (DLG_MAIN + 2)
#define BTN_ISONOFF        (DLG_MAIN + 3)
#define BTN_APPLY          (DLG_MAIN + 4)
#define BTN_UNDO           (DLG_MAIN + 5)
#define BTN_DEFAULT        (DLG_MAIN + 6)
#define BTN_HELP           (DLG_MAIN + 7)
#define WSBMP_LOGO         (DLG_MAIN + 8)
#define TXT_VERSION        (DLG_MAIN + 9)
#define CC_SHADETITLE         "shadedtitlebarsample"

// dialogo copertina --------------------------------------------------------
#define DLG_COVER             (TXT_VERSION + 1)

// dialogo pagina generale
#define DLG_SMWNGEN           (DLG_COVER + 1)
#define CHK_SMWGAPPL          (DLG_SMWNGEN + 1)
#define CHK_SMWGSAVE          (DLG_SMWNGEN + 2)
#define CHK_SMWGLASTPAGE      (DLG_SMWNGEN + 3)
#define BTN_SMWGAPPLYALL      (DLG_SMWNGEN + 4)
#define BTN_SMWGUNDOALL       (DLG_SMWNGEN + 5)
#define BTN_SMWGDEFALL        (DLG_SMWNGEN + 6)

// id dialogo generale eccezioni --------------------------------------------
#define DLG_EXCEPTLIST     (BTN_SMWGDEFALL + 1)
#define TXT_EXCPLIST       (DLG_EXCEPTLIST + 1)
#define LBOX_EXCEPTION     (DLG_EXCEPTLIST + 2)
#define EF_LISTITEM        (DLG_EXCEPTLIST + 3)
#define BTN_EXCEPTFIND     (DLG_EXCEPTLIST + 4)
#define BTN_EXCEPTCADD     (DLG_EXCEPTLIST + 5)
#define BTN_EXCECPTREM     (DLG_EXCEPTLIST + 6)

// dialogo selezione azioni mouse e tastiera (finestra indipendente) --------
#define DLG_MOUKEYBACTSFL  (BTN_EXCECPTREM + 1)
// dialogo selezione azioni mouse e tastiera (pagina notebook) --------------
#define DLG_MOUKEYBACTS    (DLG_MOUKEYBACTSFL + 1)
#define TXT_FEFNCTEVNTS    (DLG_MOUKEYBACTS + 1)
#define LBOX_FEFNCTEVNTS   (DLG_MOUKEYBACTS + 2)
#define SBAR_FEFNCTEVNT    (DLG_MOUKEYBACTS + 3)
#define CHK_FEENABLE       (DLG_MOUKEYBACTS + 4)
#define TXT_FEKEYBTN       (DLG_MOUKEYBACTS + 5)
#define SPIN_FEFKEYBTN     (DLG_MOUKEYBACTS + 6)
#define RDBTN_FECLICK      (DLG_MOUKEYBACTS + 7)
#define RDBTN_FEDBLCLK     (DLG_MOUKEYBACTS + 8)
#define CHK_FECTRL         (DLG_MOUKEYBACTS + 9)
#define CHK_FEALT          (DLG_MOUKEYBACTS + 10)
#define CHK_FESHIFT        (DLG_MOUKEYBACTS + 11)
#define GRP_FEHBAR         (DLG_MOUKEYBACTS + 12)
#define BTN_FEOK           (DLG_MOUKEYBACTS + 13)
#define BTN_FECANCEL       (DLG_MOUKEYBACTS + 14)
#define BTN_FEHELP         (DLG_MOUKEYBACTS + 15)

// dialogo selezione bitmap -------------------------------------------------
// dimensioni iniziali dialogo
#define BMPSELDLG_CX       236
#define BMPSELDLG_CY       116
// id --------------------------------
#define DLG_BMPSEL         (BTN_FEHELP + 1)
#define LBOX_BMPSEL        (DLG_BMPSEL + 1)
#define BTN_EDITBMP        (DLG_BMPSEL + 2)
#define BTN_FINDBMP        (DLG_BMPSEL + 3)
#define CHK_BMPDISABLE     (DLG_BMPSEL + 4)
#define CHK_BMPDEFAULT     (DLG_BMPSEL + 5)
#define COMBO_CLOSEPOS     (DLG_BMPSEL + 6)
#define BTN_BMPOK          (DLG_BMPSEL + 7)
#define BTN_BMPCANCEL      (DLG_BMPSEL + 8)
#define BTN_BMPHELP        (DLG_BMPSEL + 9)

// dialogo opzioni Main Windows ---------------------------------------------
#define DLG_MAINWINOPT     (BTN_BMPHELP + 1)
#define CHK_MWUSESMWFONT   (DLG_MAINWINOPT + 1)
#define CHK_MWOVERRDPPS    (DLG_MAINWINOPT + 2)
      // eliminare
#define CHK_MWIGNOREPP     (DLG_MAINWINOPT + 3)
#define CHK_MWDLGONLY      (DLG_MAINWINOPT + 4)
      // ---------
#define BTN_MWFONT         (DLG_MAINWINOPT + 5)
#define BTN_MWINFO         (DLG_MAINWINOPT + 6)
#define BTN_MWQUESTION     (DLG_MAINWINOPT + 7)
#define BTN_MWERROR        (DLG_MAINWINOPT + 8)
#define BTN_MWXCLMATION    (DLG_MAINWINOPT + 9)

// dialogo comune barra titolo attiva e inattiva ----------------------------
#define DLG_TITLEBKGND       (BTN_MWXCLMATION + 1)
#define CHK_ENTBARENH        (DLG_TITLEBKGND + 1)
#define FRAMESAMPLE          (DLG_TITLEBKGND + 2)
#define TITLEBARSAMPLE       (DLG_TITLEBKGND + 3)
#define BTN_TBMNSYS          (DLG_TITLEBKGND + 4)
#define BTN_TBMNCLOSE        (DLG_TITLEBKGND + 5)
#define BTN_TBMNHIDE         (DLG_TITLEBKGND + 6)
#define BTN_TBMNMIN          (DLG_TITLEBKGND + 7)
#define BTN_TBMNRES          (DLG_TITLEBKGND + 8)
#define BTN_TBMNMAX          (DLG_TITLEBKGND + 9)
#define CHK_TBOCNTXTMENU     (DLG_TITLEBKGND + 10)
#define RDBTN_SOLID          (DLG_TITLEBKGND + 11)
#define RDBTN_SHADE          (DLG_TITLEBKGND + 12)
#define RDBTN_BITMAP         (DLG_TITLEBKGND + 13)
#define CHK_TBOOVERRDPPS     (DLG_TITLEBKGND + 14)
#define BTN_SWPCOLSHADE      (DLG_TITLEBKGND + 15)
#define BTN_SHADEDIRECT      (DLG_TITLEBKGND + 16)
#define BTN_COLUP            (DLG_TITLEBKGND + 17)
#define BTN_COLDOWN          (DLG_TITLEBKGND + 18)
#define CHK_STRETCH          (DLG_TITLEBKGND + 19)
#define BTN_BITMAP           (DLG_TITLEBKGND + 20)
#define CHK_SHOWBORDER       (DLG_TITLEBKGND + 21)
#define BTN_SWPCOLBRDR       (DLG_TITLEBKGND + 22)
#define BTN_HILITE           (DLG_TITLEBKGND + 23)
#define BTN_SHADOW           (DLG_TITLEBKGND + 24)
#define CHK_3DEFFECT         (DLG_TITLEBKGND + 25)
#define CHK_TBCENTER         (DLG_TITLEBKGND + 26)
#define BTN_FONT             (DLG_TITLEBKGND + 27)
#define BTN_SWPCOLTXT        (DLG_TITLEBKGND + 28)
#define BTN_FGNDTXT          (DLG_TITLEBKGND + 29)
#define BTN_SHADOWTXT        (DLG_TITLEBKGND + 30)

// dialogo titlebar hiding --------------------------------------------------
#define DLG_HIDETBAR         (BTN_SHADOWTXT + 1)
#define CHK_HTBARENABLE      (DLG_HIDETBAR + 1)
#define CHK_HTBARSYSMENU     (DLG_HIDETBAR + 2)
#define BTN_HTBARMOU         (DLG_HIDETBAR + 3)
#define BTN_HTBARKEYB        (DLG_HIDETBAR + 4)
#define BTN_HTBARHIDEBTN     (DLG_HIDETBAR + 5)
#define WSBMP_HIDE           (DLG_HIDETBAR + 6)
#define BTN_HTBARUNHIDEBTN   (DLG_HIDETBAR + 7)
#define WSBMP_UNHIDE         (DLG_HIDETBAR + 8)
#define CHK_HTREDFLDH        (DLG_HIDETBAR + 9)
#define CHK_HTXCLDLGWIN      (DLG_HIDETBAR + 10)
#define CHK_HTBARINCLMENU    (DLG_HIDETBAR + 11)
#define TXT_HTBARPUPDELAY    (DLG_HIDETBAR + 12)
#define SPIN_HTBARPUPDELAY   (DLG_HIDETBAR + 13)
#define TXT_HTBARHIDEDELAY   (DLG_HIDETBAR + 14)
#define SPIN_HTBARHIDEDELAY  (DLG_HIDETBAR + 15)

// dialogo window roll ------------------------------------------------------
#define DLG_ROLLWIN          (SPIN_HTBARHIDEDELAY + 1)
#define CHK_RLWINENABLE      (DLG_ROLLWIN + 1)
#define CHK_RLWINSYSMENU     (DLG_ROLLWIN + 2)
#define BTN_RLWINMOU         (DLG_ROLLWIN + 3)
#define BTN_RLWINKEYB        (DLG_ROLLWIN + 4)
#define BTN_RLWINROLLBTN     (DLG_ROLLWIN + 5)
#define WSBMP_ROLL           (DLG_ROLLWIN + 6)
#define BTN_RLWINUNROLLBTN   (DLG_ROLLWIN + 7)
#define WSBMP_UNROLL         (DLG_ROLLWIN + 8)
#define CHK_XCLHTBARWIN      (DLG_ROLLWIN + 9)
#define CHK_AUNRLGETFOCUS    (DLG_ROLLWIN + 10)
#define CHK_ARLLOSEFOCUS     (DLG_ROLLWIN + 11)
#define CHK_ALLOWVRSZ        (DLG_ROLLWIN + 12)
#define CHK_AUNRLVRSZWIN     (DLG_ROLLWIN + 13)
#define CHK_RLTOBTOTTOM      (DLG_ROLLWIN + 14)
#define CHK_RLSTATESTORE     (DLG_ROLLWIN + 15)
#define CHK_AUNRLCLOSEWIN    (DLG_ROLLWIN + 16)

// dialogo maximized windows ------------------------------------------------
#define DLG_MAXWIN           (CHK_AUNRLCLOSEWIN + 1)
#define CHK_MXOVERSZPOS      (DLG_MAXWIN + 1)
#define TXT_MXX              (DLG_MAXWIN + 2)
#define SPIN_MXX             (DLG_MAXWIN + 3)
#define TXT_MXY              (DLG_MAXWIN + 4)
#define SPIN_MXY             (DLG_MAXWIN + 5)
#define TXT_MXWIDHT          (DLG_MAXWIN + 6)
#define SPIN_MXWIDHT         (DLG_MAXWIN + 7)
#define TXT_MXHEIGHT         (DLG_MAXWIN + 8)
#define SPIN_MXHEIGHT        (DLG_MAXWIN + 9)
#define CHK_MXNOMOVE         (DLG_MAXWIN + 10)
#define CHK_MXNOSIZE         (DLG_MAXWIN + 11)

// dialogo size/move --------------------------------------------------------
#define DLG_SIZEMOVE         (CHK_MXNOSIZE + 1)
#define CHK_FULLWINDRAG      (DLG_SIZEMOVE + 1)
#define CHK_SMOOTHHMOVE      (DLG_SIZEMOVE + 2)
#define CHK_BKGNDSZMOVE      (DLG_SIZEMOVE + 3)
#define BTN_BKGNDSZMOVE      (DLG_SIZEMOVE + 4)
#define CHK_FRAMEDRAG        (DLG_SIZEMOVE + 5)
#define BTN_FRAMEDRAG        (DLG_SIZEMOVE + 6)
#define CHK_PUSHBOTTOM       (DLG_SIZEMOVE + 7)
#define CHK_SZMVZONE         (DLG_SIZEMOVE + 8)
#define BTN_PUSHBOTTOM       (DLG_SIZEMOVE + 9)

// dialogo controlli testo --------------------------------------------------
#define DLG_TXTCTLGEN        (BTN_PUSHBOTTOM + 1)
#define CHK_TCGEFAUTOSEL     (DLG_TXTCTLGEN + 1)
#define BTN_TCGEFAUTOSEL     (DLG_TXTCTLGEN + 2)
#define CHK_TCGMOUCPYPST     (DLG_TXTCTLGEN + 3)
#define CHK_TCGMCPYEF        (DLG_TXTCTLGEN + 4)
#define CHK_TCGMCPYMLE       (DLG_TXTCTLGEN + 5)
#define CHK_TCGMCPYSTXT      (DLG_TXTCTLGEN + 6)
#define CHK_TCGMCPYLBOX      (DLG_TXTCTLGEN + 7)
#define CHK_TCGMCPYBTN       (DLG_TXTCTLGEN + 8)
#define BTN_TCGCPYBY         (DLG_TXTCTLGEN + 9)
#define BTN_TCGCUTBY         (DLG_TXTCTLGEN + 10)
#define BTN_TCGPSTINSBY      (DLG_TXTCTLGEN + 11)
#define BTN_TCGOVRWRTBY      (DLG_TXTCTLGEN + 12)

// pagina VIO windows -------------------------------------------------------
#define DLG_VIOWIN           (BTN_TCGOVRWRTBY + 1)
#define CHK_VIONOHSCRL       (DLG_VIOWIN + 1)
#define CHK_VIOINS           (DLG_VIOWIN + 2)
#define CHK_VIOMOUCPY        (DLG_VIOWIN + 3)
#define BTN_VIOCPYMOU        (DLG_VIOWIN + 4)
#define BTN_VIOAPNDMOU       (DLG_VIOWIN + 5)
#define BTN_VIOPASTEMOU      (DLG_VIOWIN + 6)
#define CHK_VIOKBDCPY        (DLG_VIOWIN + 7)
#define BTN_VIOCPYKBD        (DLG_VIOWIN + 8)
#define BTN_VIOAPNDKBD       (DLG_VIOWIN + 9)
#define BTN_VIOPASTEKBD      (DLG_VIOWIN + 10)

// dialogo mouse options ----------------------------------------------------
#define DLG_MOUSEOPT         (BTN_VIOPASTEKBD + 1)
#define CHK_MAUTFOCUS        (DLG_MOUSEOPT + 1)
#define TXT_MAUTFOCUS        (DLG_MOUSEOPT + 2)
#define SPIN_MAUTFOCUS       (DLG_MOUSEOPT + 3)
#define CHK_MAFIGNDESK       (DLG_MOUSEOPT + 4)
#define CHK_MAFNOTOP         (DLG_MOUSEOPT + 5)
#define CHK_MAFIGNNEW        (DLG_MOUSEOPT + 6)
#define CHK_MPTRHIDE         (DLG_MOUSEOPT + 7)
#define TXT_MPTRHIDE         (DLG_MOUSEOPT + 8)
#define SPIN_MPTRHIDE        (DLG_MOUSEOPT + 9)
#define CHK_MPHXCLBTN        (DLG_MOUSEOPT + 10)
#define CHK_MPHXCLMENU       (DLG_MOUSEOPT + 11)
#define CHK_MBTN3MAP         (DLG_MOUSEOPT + 12)
#define CHK_MBTN3TBAR        (DLG_MOUSEOPT + 13)
#define CHK_MMOVETOFOCUS     (DLG_MOUSEOPT + 14)
#define TXT_MMTFSTEPS        (DLG_MOUSEOPT + 15)
#define SPIN_MMTFSTEPS       (DLG_MOUSEOPT + 16)
#define CHK_MMTFWINCREATE    (DLG_MOUSEOPT + 17)
#define CHK_MMTFWINDLG       (DLG_MOUSEOPT + 18)
#define CHK_MMTFXCLDLGNOBTN  (DLG_MOUSEOPT + 19)
// dialogo window keys
#define DLG_WINKEY           (CHK_MMTFXCLDLGNOBTN + 1)
#define COMBO_WKLEFT         (DLG_WINKEY + 1)
#define COMBO_WKRIGHT        (DLG_WINKEY + 2)
#define COMBO_WKMENU         (DLG_WINKEY + 3)

// ultimo ID controllo con context help
#define I_LASTCONTEXTHELP    COMBO_WKMENU

// dialoghi esterni: --------------------------------------------------------
// dialogo installazione
#define DLG_INSTMAIN              0x6000
#define STLRLOGO                     100
#define MLE_INSTMLE                  101
#define GRP_ISNTSEL                  102
#define RDB_INSTINST                 103
#define RDB_INSTRECR                 104
#define RDB_INSTUNINST               105
#define TXT_INSTDRV                  106
#define LBX_INSTDRV                  107
#define TXT_INSTPATH                 108
#define EF_INSTPATH                  109
#define BTN_INSTBACK                 110
#define BTN_INSTFWD                  111
#define BTN_INSTCANC                 112
#define BTN_INSTHLP                  113

// dialogo registrazione
#define DLG_REGMAIN               0x6001
#define TXT_REGCAPT                 100
#define MLE_REGMAIN                 101
#define TXT_REGNAME                 102
#define EF_REGNAME                  103
#define TXT_REGADDRESS              104
#define EF_REGADDRESS               105
#define TXT_REGCITY                 106
#define EF_REGCITY                  107
#define TXT_REGZIP                  108
#define EF_REGZIP                   109
#define TXT_REGPROV                 110
#define EF_REGPROV                  111
#define TXT_REGNATION               112
#define EF_REGNATION                113
#define TXT_REGEMAIL                114
#define EF_REGEMAIL                 115
#define TXT_REGPWD                  116
#define EF_REGPWD                   117
#define SPIN_REGML                  118
#define BTN_REG0                    119
#define BTN_REG1                    120
#define BTN_REG2                    121
#define BTN_REGHLP                  122
#define SBAR_REGMAIN                2

// ID Stringhe --------------------------------------------------------------

// messaggi di errore di inizializzazione
#define ID_ERR_LOADMOD         (I_LASTCONTEXTHELP + 1)
#define ID_ERR_REGCTLS         (I_LASTCONTEXTHELP + 2)
#define ID_ERR_LOADDLG         (I_LASTCONTEXTHELP + 3)
#define ID_ERR_OPENPROFILE     (I_LASTCONTEXTHELP + 4)
#define ID_ERR_FILLNOTEBOOK    (I_LASTCONTEXTHELP + 5)

// tab notebook preferenze
#define ID_PAGEXOFY           (ID_ERR_FILLNOTEBOOK + 1)
#define ID_SMARTWINDOWS       (ID_ERR_FILLNOTEBOOK + 2)
#define ID_PRODINFO           (ID_ERR_FILLNOTEBOOK + 3)
#define ID_GENERAL            (ID_ERR_FILLNOTEBOOK + 4)
#define ID_EXCEPTIONS         (ID_ERR_FILLNOTEBOOK + 5)
#define ID_MAINWIN            (ID_ERR_FILLNOTEBOOK + 6)
#define ID_GENOPTS            (ID_ERR_FILLNOTEBOOK + 7)
#define ID_TITLEBAR           (ID_ERR_FILLNOTEBOOK + 8)
#define ID_TBARACTIVEWIN      (ID_ERR_FILLNOTEBOOK + 9)
#define ID_TBARINACTWIN       (ID_ERR_FILLNOTEBOOK + 10)
#define ID_TBARHIDING         (ID_ERR_FILLNOTEBOOK + 11)
#define ID_WINDOWROLLING      (ID_ERR_FILLNOTEBOOK + 12)
#define ID_MAXWIN             (ID_ERR_FILLNOTEBOOK + 13)
#define ID_SIZEMOVE           (ID_ERR_FILLNOTEBOOK + 14)
#define ID_TXTCTLS            (ID_ERR_FILLNOTEBOOK + 15)
#define ID_VIOWIN             (ID_ERR_FILLNOTEBOOK + 16)
#define ID_MOUSE              (ID_ERR_FILLNOTEBOOK + 17)
#define ID_MAPPINGS           (ID_ERR_FILLNOTEBOOK + 18)
#define ID_KEYBOARD           (ID_ERR_FILLNOTEBOOK + 19)
#define ID_WINDOWSKEYS        (ID_ERR_FILLNOTEBOOK + 20)

// messaggi errore vari
#define ID_ERR_ALLOC          (ID_WINDOWSKEYS + 1)

// varie stringhe finestra principale
#define ID_SMWDISABLED        (ID_ERR_ALLOC + 1)
#define ID_SMWPREFDSBLD       (ID_ERR_ALLOC + 2)
#define ID_SMARTWINPREF       (ID_ERR_ALLOC + 3)

// Main windows ----------------------------------------------------------
#define ID_DLGFONT            (ID_SMARTWINPREF + 1)
#define ID_FONTSAMPLE         (ID_SMARTWINPREF + 2)
#define ID_TITLEREG           (ID_SMARTWINPREF + 3)
#define ID_EXPIRED            (ID_SMARTWINPREF + 4)


// General page ----------------------------------------------------------
#define ID_DEFALLING              (ID_EXPIRED + 1)
#define ID_GENPROGRESS            (ID_EXPIRED + 2)
#define ID_GENTIMEELAPS           (ID_EXPIRED + 3)
#define ID_GENSTOP                (ID_EXPIRED + 4)


// Titlebar page ---------------------------------------------------------
#define ID_TBARACTWINSMPL         (ID_GENSTOP + 1)
#define ID_TBARINACTWINSMPL       (ID_GENSTOP + 2)
#define ID_TBARFONT               (ID_GENSTOP + 3)
#define ID_B_KGND                 (ID_GENSTOP + 4)
#define ID_TOP                    (ID_GENSTOP + 5)
#define ID_LEFT                   (ID_GENSTOP + 6)
#define ID_FGNDTXT                (ID_GENSTOP + 7)
#define ID_HORZ                   (ID_GENSTOP + 8)
#define ID_BOTTOM                 (ID_GENSTOP + 9)
#define ID_VERTICAL               (ID_GENSTOP + 10)
#define ID_HORZ3                  (ID_GENSTOP + 11)
#define ID_VERTICAL3              (ID_GENSTOP + 12)
#define ID_OUTER                  (ID_GENSTOP + 13)
#define ID_INNER                  (ID_GENSTOP + 14)
#define ID_RIGHT                  (ID_GENSTOP + 15)
#define ID_FONT                   (ID_GENSTOP + 16)
#define ID_ERR_UPDBMPNAME         (ID_GENSTOP + 17)
// dialogo selezione colori:
#define ID_CLRTITLE               (ID_GENSTOP + 18)
#define ID_CLROK                  (ID_GENSTOP + 19)
#define ID_CLRUNDO                (ID_GENSTOP + 20)
#define ID_CLRCANC                (ID_GENSTOP + 21)
#define ID_CLRRED                 (ID_GENSTOP + 22)
#define ID_CLRGRN                 (ID_GENSTOP + 23)
#define ID_CLRBLU                 (ID_GENSTOP + 24)

// Titlebar hiding -------------------------------------------------------

// opzioni size/move -----------------------------------------------------
#define ID_SZMVSZMVBY             (ID_CLRBLU + 1)
#define ID_SZMVDRAGBY             (ID_CLRBLU + 2)
#define ID_SZMVPUSHBY             (ID_CLRBLU + 3)

// opzioni controlli testo -----------------------------------------------
#define ID_TCGEFAUTOSELBY         (ID_SZMVPUSHBY + 1)
#define ID_TCGCPYBY               (ID_SZMVPUSHBY + 2)
#define ID_TCGCUTBY               (ID_SZMVPUSHBY + 3)
#define ID_TCGPSTINSBY            (ID_SZMVPUSHBY + 4)
#define ID_TCGOVRWRTBY            (ID_SZMVPUSHBY + 5)

// VIO windows -----------------------------------------------------------
#define ID_VIOCPYBY               (ID_TCGOVRWRTBY + 1)
#define ID_VIOCPYALLBY            (ID_TCGOVRWRTBY + 2)
#define ID_VIOPASTEBY             (ID_TCGOVRWRTBY + 3)

// mouse/keyboard actions/event dialog -----------------------------------
#define ID_KEYBCTRLDFUNCS         (ID_VIOPASTEBY + 1)
#define ID_MOUCTRLDFUNCS          (ID_VIOPASTEBY + 2)
#define ID_MOUBTN                 (ID_VIOPASTEBY + 3)

// strings to build the text of the "mouse action" buttons
#define ID_CMPMOUBTN             (ID_MOUBTN + 1)
#define ID_DELKEY                (ID_MOUBTN + 2)
#define ID_INSKEY                (ID_MOUBTN + 3)
#define ID_BKSPKEY               (ID_MOUBTN + 4)
#define ID_CMPCLICK              (ID_MOUBTN + 5)
#define ID_CMPDBLCLK             (ID_MOUBTN + 6)
#define ID_CMPCTRL               (ID_MOUBTN + 7)
#define ID_CMPALT                (ID_MOUBTN + 8)
#define ID_CMPSHIFT              (ID_MOUBTN + 9)
// stringhe x lista e target azione
#define ID_ROLL                  (ID_CMPSHIFT + 1)
#define ID_PERFTBAR              (ID_CMPSHIFT + 2)
#define ID_HTBAR                 (ID_CMPSHIFT + 3)
#define ID_VIOCOPY               (ID_CMPSHIFT + 4)
#define ID_PERFVIO               (ID_CMPSHIFT + 5)
#define ID_VIOAPND               (ID_CMPSHIFT + 6)
#define ID_VIOPASTE              (ID_CMPSHIFT + 7)
#define ID_BKSZMOVE              (ID_CMPSHIFT + 8)
#define ID_PERFANYWIN            (ID_CMPSHIFT + 9)
#define ID_EFSELALLTXT           (ID_CMPSHIFT + 10)
#define ID_PERFEF                (ID_CMPSHIFT + 11)
#define ID_TXTCTLSCPY            (ID_CMPSHIFT + 12)
#define ID_PERFTXTCTLS           (ID_CMPSHIFT + 13)
#define ID_TXTCTLSCUT            (ID_CMPSHIFT + 14)
#define ID_TXTCTLSPASTE          (ID_CMPSHIFT + 15)
#define ID_TXTCTLSOVRWRT         (ID_CMPSHIFT + 16)
#define ID_FRAMEDRAG             (ID_CMPSHIFT + 17)
#define ID_PERFWINBRD            (ID_CMPSHIFT + 18)
#define ID_PUTBEHIND             (ID_CMPSHIFT + 19)
#define ID_PERFFOCWIN            (ID_CMPSHIFT + 20)
#define ID_FLDOPPRPRT            (ID_CMPSHIFT + 21)
#define ID_FLDOPPAR              (ID_CMPSHIFT + 22)
#define ID_FLDFIND               (ID_CMPSHIFT + 23)
#define ID_FLDSRTNAME            (ID_CMPSHIFT + 24)
#define ID_FLDSRTEXT             (ID_CMPSHIFT + 25)
#define ID_FLDSRTTYPE            (ID_CMPSHIFT + 26)
#define ID_FLDRSRTRNAME          (ID_CMPSHIFT + 27)
#define ID_FLDRSRTSIZE           (ID_CMPSHIFT + 28)
#define ID_FLDSRTWRT             (ID_CMPSHIFT + 29)
#define ID_FLDSRTACC             (ID_CMPSHIFT + 30)
#define ID_FLDSRTCREAT           (ID_CMPSHIFT + 31)
#define ID_FLDARRANGE            (ID_CMPSHIFT + 32)
#define ID_FLDREFRESH            (ID_CMPSHIFT + 33)
#define ID_PERFONFLDR            (ID_CMPSHIFT + 34)

// dialogo eccezioni
#define ID_ADDTOEXCPTLIST        (ID_PERFONFLDR + 1)
#define ID_ADD                   (ID_PERFONFLDR + 2)
#define ID_EXCFILENOTFOUND       (ID_PERFONFLDR + 3)
#define ID_ERR_EXCGETLIST        (ID_PERFONFLDR + 4)
#define ID_ERR_EXCUPDATEPRO      (ID_PERFONFLDR + 5)

// bitmap selection dialog -----------------------------------------------
#define ID_EDIT                  (ID_ERR_EXCUPDATEPRO + 1)
#define ID_IMPORT___             (ID_ERR_EXCUPDATEPRO + 2)
#define ID_DISABLE               (ID_ERR_EXCUPDATEPRO + 3)
#define ID__DEFAULT              (ID_ERR_EXCUPDATEPRO + 4)
#define ID_CLOSEBTNPOS           (ID_ERR_EXCUPDATEPRO + 5)
#define ID_OK                    (ID_ERR_EXCUPDATEPRO + 6)
#define ID_CANCEL                (ID_ERR_EXCUPDATEPRO + 7)
#define ID_HELP                  (ID_ERR_EXCUPDATEPRO + 8)
#define ID_EDIT_                 (ID_ERR_EXCUPDATEPRO + 9)
// dialog titles
#define ID_BSDTITLE0            (ID_EDIT_ + 1)
#define ID_BSDTITLE1            (ID_EDIT_ + 2)
#define ID_BSDTITLE2            (ID_EDIT_ + 3)
#define ID_BSDTITLE3            (ID_EDIT_ + 4)
#define ID_BSDTITLE4            (ID_EDIT_ + 5)
#define ID_BSDTITLE5            (ID_EDIT_ + 6)
#define ID_BSDTITLE6            (ID_EDIT_ + 7)
#define ID_BSDTITLE7            (ID_EDIT_ + 8)
#define ID_BSDTITLE8            (ID_EDIT_ + 9)
#define ID_BSDTITLE9            (ID_EDIT_ + 10)
#define ID_BSDTITLE10           (ID_EDIT_ + 11)
#define ID_BSDTITLE11           (ID_EDIT_ + 12)
// warning/error messages
#define ID_ERR_BSDALLOC         (ID_BSDTITLE11 + 1)
#define ID_ASKOVERWRITE         (ID_BSDTITLE11 + 2)
#define ID_ASKCONFIRM           (ID_BSDTITLE11 + 3)
#define ID_ERR_STARTICONEDIT    (ID_BSDTITLE11 + 4)
#define ID_ERR_COPYBMP          (ID_BSDTITLE11 + 5)
#define ID_ERR_INVALIDPATH      (ID_BSDTITLE11 + 6)
// import bitmap dialog
#define ID_IMPORTBMP            (ID_ERR_INVALIDPATH + 1)
#define ID_IMPORTOK             (ID_ERR_INVALIDPATH + 2)

// id stringhe emulazione help tramite file inf
#define ID_INF_FILE             (ID_IMPORTOK + 1)
#define ID_INF_LOGO             (ID_IMPORTOK + 2)
#define ID_INF_GEN              (ID_IMPORTOK + 3)
#define ID_INF_EXCPTN           (ID_IMPORTOK + 4)
#define ID_INF_DLG              (ID_IMPORTOK + 5)
#define ID_INF_TBAR             (ID_IMPORTOK + 6)
#define ID_INF_TBHIDE           (ID_IMPORTOK + 7)
#define ID_INF_ROLLING          (ID_IMPORTOK + 8)
#define ID_INF_MAXIMIZ          (ID_IMPORTOK + 9)
#define ID_INF_MOVING           (ID_IMPORTOK + 10)
#define ID_INF_TEXT             (ID_IMPORTOK + 11)
#define ID_INF_VIO              (ID_IMPORTOK + 12)
#define ID_INF_MOUSE            (ID_IMPORTOK + 13)
#define ID_INF_MOUKEY           (ID_IMPORTOK + 14)
#define ID_INF_BITMAP           (ID_IMPORTOK + 15)
#define ID_INF_INSTALL          (ID_IMPORTOK + 16)
#define ID_INF_REGISTER         (ID_IMPORTOK + 17)
#define ID_INF_WINKEY           (ID_IMPORTOK + 18)

#define ID_WKEYFIRST             ID_INF_WINKEY
// ID stringhe pagina Window keys
#define ID_WKEYNOACT            (ID_WKEYFIRST + 1)
#define ID_WKEYHIDESHOW         (ID_WKEYFIRST + 2)
#define ID_WKEYROLLUNROLL       (ID_WKEYFIRST + 3)
#define ID_WKEYWINLIST          (ID_WKEYFIRST + 4)
#define ID_WKEYCONTEXT          (ID_WKEYFIRST + 5)
#define ID_WKEYWCLOCK           (ID_WKEYFIRST + 6)
#define ID_WKEYDRIVEFLDR        (ID_WKEYFIRST + 7)
#define ID_WKEYSSETUPFLDR       (ID_WKEYFIRST + 8)
#define ID_WKEYOS2CMD           (ID_WKEYFIRST + 9)
#define ID_WKEYWCDESK           (ID_WKEYFIRST + 10)
#define ID_WKEYWCWLIST          (ID_WKEYFIRST + 11)
#define ID_WKEYWCPLIST          (ID_WKEYFIRST + 12)
#define ID_WKEYWCTRAYLIST       (ID_WKEYFIRST + 13)
#define ID_WKEYWCNEXTTRAY       (ID_WKEYFIRST + 14)
#define ID_WKEYWCINFO           (ID_WKEYFIRST + 15)

// id stringhe usate da dialogo installazione
#define ID_INST_INSTIN          (0x3000 + 1)
#define ID_INST_INSTPATH        (0x3000 + 2)
#define ID_INST_EXIT            (0x3000 + 3)
#define ID_INST_LICENSE         (0x3000 + 4)
#define ID_INST_LOGFILE         (0x3000 + 5)
#define ID_INST_DRIVE           (0x3000 + 6)
#define ID_INST_RFRSHIN         (0x3000 + 7)
#define ID_INST_UNINSTIN        (0x3000 + 8)
// errori
#define ID_INSTERR_NOLIC        (0x3000 + 9)
#define ID_IENERDINST           (0x3000 + 10)
#define ID_IENERDRFRSH          (0x3000 + 11)
#define ID_IENERDUNINST         (0x3000 + 12)
#define ID_INSTERCPY            (0x3000 + 13)
#define ID_INSTERMKDIR          (0x3000 + 14)
#define ID_INSTERDEL            (0x3000 + 15)

// id titolo dialogo progresso inizializzazione lista drive
#define ID_IPRDRVINFO           (0x3000 + 16)
#define ID_IPRINSTLING          (0x3000 + 17)
#define ID_IPRRFRSHING          (0x3000 + 18)
#define ID_IPRDELETING          (0x3000 + 19)
// other messages
#define ID_IQRYMKPATH           (0x3000 + 20)
#define ID_INSTREPATH           (0x3000 + 21)
#define ID_IERRMKPATH           (0x3000 + 22)
#define ID_IENDINSTOK           (0x3000 + 23)
#define ID_IENDRFRSHOK          (0x3000 + 24)
#define ID_IENDUNINSTOK         (0x3000 + 25)
#define ID_IENDINSTSTP          (0x3000 + 26)
#define ID_IENDRFRSHSTP         (0x3000 + 27)
#define ID_IENDUNINSTSTP        (0x3000 + 28)
#define ID_IENDINSTPRB          (0x3000 + 29)
#define ID_IENDRFRSHPRB         (0x3000 + 30)
#define ID_IENDUNINSTPRB        (0x3000 + 31)
#define ID_INSTPROCERR          (0x3000 + 32)
#define ID_INSTCHKPREF          (0x3000 + 33)
#define ID_INSTSYSDLLS          (0x3000 + 34)
#define ID_INSTSYSDLLS2         (0x3000 + 35)
#define ID_INSTADDINI0          (0x3000 + 36)
#define ID_INSTADDINI1          (0x3000 + 37)
#define ID_INSTDELINI1          (0x3000 + 38)
#define ID_ICHKSMWINI           (0x3000 + 39)
#define ID_IMKOBJS              (0x3000 + 40)
#define ID_IMKOBJ               (0x3000 + 41)
#define ID_IDELSYSDLLS          (0x3000 + 42)
#define ID_IDELSYSDLLS2         (0x3000 + 43)
#define ID_INSQRYSTRTUP         (0x3000 + 44)
#define ID_IQRYREMBMPS          (0x3000 + 45)
#define ID_INSDSTROBJ           (0x3000 + 46)
#define ID_IQRYREMIPTH          (0x3000 + 47)
#define ID_IQRYREMIPTH1         (0x3000 + 48)
#define ID_IPRELAPSED           (0x3000 + 49)
#define ID_ILOGCPY              (0x3000 + 50)
#define ID_ILOGCPYTO            (0x3000 + 51)
#define ID_IOVWRTNEW            (0x3000 + 52)
#define ID_IOVWRTCONF           (0x3000 + 53)
#define ID_ILOGREM              (0x3000 + 54)
#define ID_ILOGMKPATH           (0x3000 + 55)

// titoli oggetti
#define ID_OBJTFOLDER           (0x3000 + 56)
#define ID_OBJTPRPRTY           (0x3000 + 57)
#define ID_OBJTPRPRTY1          (0x3000 + 58)
#define ID_OBJTREADME           (0x3000 + 59)
#define ID_OBJTLICENSE          (0x3000 + 60)
#define ID_OBJTRGSTRNT          (0x3000 + 61)
#define ID_OBJTINST_U           (0x3000 + 62)
#define ID_OBJTRGSTRPR          (0x3000 + 63)
#define ID_OBJTDOC              (0x3000 + 64)

// stringhe usate nel dialogo di registrazione ---------------------------
// captions
#define ID_REGMLNONE            (0x4000 + 1)
#define ID_REGMLSTLR            (0x4000 + 2)
#define ID_REGMLALL             (0x4000 + 3)
#define ID_REGCLIENTDATA        (0x4000 + 4)
#define ID_REGCLIENTCODE        (0x4000 + 5)
#define ID_REGREGCODE           (0x4000 + 6)
#define ID_REGINPUTCSTMR        (0x4000 + 7)
#define ID_REGSENDCODE          (0x4000 + 8)
#define ID_REGINPUTREGCODE      (0x4000 + 9)
#define ID_REGEDIT              (0x4000 + 10)
#define ID_REGCPYCLPBRD         (0x4000 + 11)
#define ID_REGOK                (0x4000 + 12)
// file dialog
#define ID_REGSAVECSTMRFIL      (0x4000 + 13)
// errori:
#define IDERR_REGFAILALLOC      (0x4000 + 14)
#define IDERR_REGLOADREGFILE    (0x4000 + 15)
#define IDERR_REGREADCSTMDAT    (0x4000 + 16)
#define IDERR_REGFAILOPPRF      (0x4000 + 17)
#define IDERR_REGSAVECSTMRCD    (0x4000 + 18)
#define IDERR_REGCONVCSTDATA    (0x4000 + 19)
#define IDERR_REGREADREGCODE    (0x4000 + 20)
#define IDERR_REGINVKEY         (0x4000 + 21)
// help contestuale
#define IDHLP_REGMLEREADME      (0x4000 + 22)
#define IDHLP_REGINNAME         (0x4000 + 23)
#define IDHLP_REGINADDRESS      (0x4000 + 24)
#define IDHLP_REGINCITY         (0x4000 + 25)
#define IDHLP_REGINZIP          (0x4000 + 26)
#define IDHLP_REGINPROV         (0x4000 + 27)
#define IDHLP_REGINNATION       (0x4000 + 28)
#define IDHLP_REGINEMAIL        (0x4000 + 29)
#define IDHLP_REGINPIVA         (0x4000 + 30)
#define IDHLP_REGBTNEDIT        (0x4000 + 31)
#define IDHLP_REGMLECSTMRCODE   (0x4000 + 32)
#define IDHLP_REGMLEREGCODE     (0x4000 + 33)
#define IDHLP_REGREGREG         (0x4000 + 34)
#define IDHLP_REGSBAR           (0x4000 + 35)

// altri messaggi
#define IDMSG_REGCANCNEWREG     (0x4000 + 36)
#define IDMSG_REGREGOK          (0x4000 + 37)
#define IDMSG_REGREGCODEPRES    (0x4000 + 38)

// stringhe che potrebbero essere pi— lunghe di 256 caratteri
// gli ID non devono entrare in conflitto con le stringhe definite
// nella stringtable
// context help
#define IDHLP_REGML             0x5000
#define IDHLP_REGCSTMRDATA      0x5001
#define IDHLP_REGCSTMRCODE      0x5002
#define IDHLP_REGREGCODE        0x5003
#define IDHLP_REGBTNCOPY        0x5004
// altri messaggi
#define IDMSG_REGEDCSTDATA0     0x5005
#define IDMSG_REGEDCSTDATA1     0x5006
#define IDMSG_REGCSTMRCODESVD   0x5007
#define IDMSG_REGNEWCSTCODE0    0x5008
#define IDMSG_REGNEWCSTCODE1    0x5009
#define IDMSG_REGNEWREGCODE     0x5010

// id stringhe usate come testo menuitem da smartwin.dll

#endif
