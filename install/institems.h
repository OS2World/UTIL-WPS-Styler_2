// institems.h: liste file da installare, disinstallare, setup strings, ecc...

#ifndef INSTITEMS_H_
   #define INSTITEMS_H_

// numero passi installazione alla fine:


// stringhe usate nella modifica di OS2.INI --------------------------------
// SYSDLLS LOADPER PROCESS
#define SZPRO_SYSDLL       "SYS_DLLS"
#define SZPRO_LOADPP       "LoadPerProcess"
#define SZPRO_VALUE        "SMARTWIN"
#define SZPRO_VALUE1       "STYLER20"
// applicazione "Smart Windows" e relativi items
#define SZPRO_APP          "Smart Windows"
// path installazione
#define SZPRO_PATH         "path"
// aggiunta MPTS.EXE alle eccezioni
#define SZPRO_MPTS         "MPTS.EXE"
#define SZPRO_EXCTBG       "EXC_TBARGEN"
#define SZPRO_EXCTBH       "EXC_TBARHIDE"
#define SZPRO_EXCWRL       "EXC_WINROLL"
// aggiunta opzione startup se file INI non presente
#define SZPRO_APPPROFILE   "SMARTWIN.INI"
#define SZPRO_STARTOPT     "SETSTARTOPTIONS"

// lista file obsoleti da cancellare
#define SZFLST_OBSOLETE   \
{"3DCTLS.DLL", "AFCBSU00.DLL", "AFCPMU00.DLL", "EDITCOL.DLL", "PROGRESS.DLL",\
 "README.1ST", "SMARTWIN.@", "SMARTWIN.TXT", "SMW001.DLL", "SMWUTIL.DLL",\
 "SMW0_001.DLL", "SMW1_001.DLL", "STLRES11.DLL", "STLRES12.DLL", \
 "AFCUTL00.DLL", "STYLER2.TXT", "STYLER2.INF", "AFCUTL01.DLL"}
// lista file da copiare
#define SZFLST_COPY   \
{"AFCUTL05.DLL", "LICENSE.TXT", "INSTALL.EXE", "REGISTER.EXE", "REGISTER.TXT",\
 "SMARTWIN.EXE", "STLRES13.DLL", "STYLER2.HLP", "README.HTM", "HSAW.ICO",  \
 "RUAW.ICO"}

// lista file da cancellare alla disinstallazione
#define SZFLST_DELETE   \
{"AFCUTL05.DLL", "LICENSE.TXT", "INSTALL.EXE", "REGISTER.EXE", "REGISTER.TXT",\
 "SMARTWIN.EXE", "SMARTWIN.INI", "STLRES13.DLL", "STYLER2.HLP", "README.HTM",  \
 "HSAW.ICO", "RUAW.ICO"}

// smartwin.dll:
#define SZSTLRCURDLL      "smartwin.170"
// styler20.dll
#define SZSTLR20DLL       "styler20.170"
// albero da copiare:
#define SZTREENAME        "bitmaps"
// oggetti da creare i titoli vengono ricavati dalla dll delle risorse:
// ID oggetti:
// folder (usati anche nel parametro location)
#define SZ_OBJIDDSKTP     "<WP_DESKTOP>"
#define SZ_OBJIDFLDR      "<SMARTWIN_FOLDER>"
#define SZ_OBJIDSYSSTUP   "<WP_CONFIG>"
// altri oggetti
#define SZ_OBJIDPRPRTY    "<SMARTWIN_PROPERTIES>"
#define SZ_OBJIDPRPRTY1   "<SMARTWIN_PROPERTIES1>"
#define SZ_OBJIDREADME    "<SMARTWIN_README>"
#define SZ_OBJIDLICENSE   "<SMARTWIN_LICENSE>"
#define SZ_OBJIDREGTXT    "<SMARTWIN_REGSTRTXT>"
#define SZ_OBJIDREGPRG    "<SMARTWIN_REGISTRATION>"
#define SZ_OBJIDSETUP     "<SMARTWIN_SETUP>"
#define SZ_OBJIDDOC       "<SMARTWIN_DOCUMENTATION>"
#define SZ_OBJIDHIDEALL   "<SMARTWIN_HIDESHOWALL>"
#define SZ_OBJIDROLLALL   "<SMARTWIN_ROLLUNROLLALL>"

// setup oggetti: al posto di \t viene inserita la directory di installazione
// styler2 folder
#define SZ_FLDRSETUP      "OBJECTID=<SMARTWIN_FOLDER>"
// setup oggetto propriet… creato sia nella directory di installazione che
// in system setup
#define SZ_STLPROPRTY     "CCVIEW=NO;OBJECTID="SZ_OBJIDPRPRTY";"      \
                          "EXENAME=\t\\SMARTWIN.EXE;PROGTYPE=PM"
#define SZ_STLPROPRTY1    "CCVIEW=NO;OBJECTID="SZ_OBJIDPRPRTY1";"     \
                          "EXENAME=\t\\SMARTWIN.EXE;PROGTYPE=PM"
// setup oggetto readme
#define SZ_STLRREADME     "CCVIEW=NO;OBJECTID="SZ_OBJIDREADME";"      \
                          "EXENAME=NETSCAPE.EXE;"                     \
                          "PROGTYPE=PM;PARAMETERS=\t\\README.HTM"
// setup oggetto licenza d'uso
#define SZ_STLRLICENSE    "CCVIEW=NO;OBJECTID="SZ_OBJIDLICENSE";"     \
                          "EXENAME=E.EXE;"                            \
                          "PROGTYPE=PM;PARAMETERS=\t\\LICENSE.TXT"
// setup documentazione registrazione
#define SZ_STLRRGSTRTXT   "CCVIEW=NO;OBJECTID="SZ_OBJIDREGTXT";"      \
                          "EXENAME=E.EXE;"                            \
                          "PROGTYPE=PM;PARAMETERS=\t\\REGISTER.TXT"
// setup oggetto installazione/disinstallazione
#define SZ_STLRINST       "CCVIEW=NO;OBJECTID="SZ_OBJIDSETUP";"       \
                          "EXENAME=\t\\INSTALL.EXE;PROGTYPE=PM"
// crea oggetto registrazione
#define SZ_STLRRGSTRPRG   "CCVIEW=NO;OBJECTID="SZ_OBJIDREGPRG";"      \
                          "EXENAME=\t\\REGISTER.EXE;PROGTYPE=PM"
// crea oggetto documentazione
#define SZ_STLRUSRGUID    "CCVIEW=NO;OBJECTID="SZ_OBJIDDOC";"         \
                          "EXENAME=VIEW.EXE;PARAMETERS=\t\\STYLER2.HLP;"\
                          "PROGTYPE=PM"
// crea oggetto hide/show all windows
#define SZ_STLRHIDEALL    "CCVIEW=NO;OBJECTID="SZ_OBJIDHIDEALL";"      \
                          "ICONFILE=\t\\HSAW.ICO;"                     \
                          "EXENAME=\t\\SMARTWIN.EXE;PROGTYPE=PM;"      \
                          "PARAMETERS=hsaw"
// crea oggetto roll/unroll all windows
#define SZ_STLRROLLALL    "CCVIEW=NO;OBJECTID="SZ_OBJIDROLLALL";"      \
                          "ICONFILE=\t\\RUAW.ICO;"                     \
                          "EXENAME=\t\\SMARTWIN.EXE;PROGTYPE=PM;"      \
                          "PARAMETERS=ruaw"

// array strutture per la creazione degli oggetti
#define AOBJDATA     \
{{"WPFolder", ID_OBJTFOLDER, SZ_FLDRSETUP, SZ_OBJIDDSKTP},     \
 {"WPProgram", ID_OBJTINST_U, SZ_STLRINST, SZ_OBJIDFLDR},      \
 {"WPProgram", ID_OBJTRGSTRPR, SZ_STLRRGSTRPRG, SZ_OBJIDFLDR}, \
 {"WPProgram", ID_OBJTRGSTRNT, SZ_STLRRGSTRTXT, SZ_OBJIDFLDR}, \
 {"WPProgram", ID_OBJTLICENSE, SZ_STLRLICENSE, SZ_OBJIDFLDR},  \
 {"WPProgram", ID_OBJTPRPRTY, SZ_STLPROPRTY, SZ_OBJIDFLDR},    \
 {"WPProgram", ID_OBJTPRPRTY1, SZ_STLPROPRTY1, SZ_OBJIDSYSSTUP},\
 {"WPProgram", ID_OBJTREADME, SZ_STLRREADME, SZ_OBJIDFLDR},    \
 {"WPProgram", ID_OBJTDOC, SZ_STLRUSRGUID, SZ_OBJIDFLDR},      \
 {"WPProgram", ID_WKEYHIDESHOW, SZ_STLRHIDEALL, SZ_OBJIDFLDR}, \
 {"WPProgram", ID_WKEYROLLUNROLL, SZ_STLRROLLALL, SZ_OBJIDFLDR}}

// array id oggetti da distruggere in caso di disinstallazione
#define AOBJIDS    \
 {SZ_OBJIDPRPRTY, SZ_OBJIDPRPRTY1, SZ_OBJIDREADME, SZ_OBJIDLICENSE, \
  SZ_OBJIDREGTXT, SZ_OBJIDREGPRG, SZ_OBJIDSETUP, SZ_OBJIDDOC,       \
  SZ_OBJIDHIDEALL, SZ_OBJIDROLLALL, SZ_OBJIDFLDR}

// calcolo passi installazione/disinstallazione/aggiornamento
#define CCHECKPREFRUN    1     // controlla se blocco preferenze Š aperto
#define CINSTINIITEMS    7     // 2 * SYSDLLS, path, MPTS (3 eccez),
                               // startupoption
#define CINSTSYSLIB      2     // copia smartwin.dll e styler20.dll in \os2\dll
#define CUINISTINIITEMS  3     // 2 * SYSDLLS, rimuove applicazione SmartWindows

#endif
