//===========================================================================
// main.c : main module.
// - application initialization
// - UI initialization
// - main window event handling.
// - termination.
//===========================================================================

#pragma strings(readonly)

#include "pref.h"
#include "titlenew.h"
#include "api.h"


// globals ------------------------------------------------------------------

PAPPDATA p;
PFN_PREFHWND stlrFnPrefHwnd;
PFN_OPTIONS stlrFnOptions;
PFN_OPENPROFILE stlrFnOpenProfile;
PFN_OBJECTHWND stlrFnObjectHwnd;
PFN_VERSION stlrFnVersion;
PFN_HBMPFROMDATA stlrHbmpFromData;
PFN_HBMPFROMMODULE stlrHbmpFromModule;
PFN_BMPDATA stlrBmpDataFromPrf;
PFN_HBMPOFSHADE stlrHbmpOfShade;


// prototypes ---------------------------------------------------------------
MRESULT EXPENTRY MainDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT CheckWindowSizingProc(HWND hwnd, MPARAM mp1, MPARAM mp2);
MRESULT GetPrefBorderSize(HWND hwnd, MPARAM mp1, MPARAM mp2);
MRESULT FormatControls(HWND hwnd, MPARAM mp1, MPARAM mp2);
VOID MinMaxProc(HWND hwnd, MPARAM mp1);
VOID ProcessControl(HWND hwnd, MPARAM mp1, MPARAM mp2);
MRESULT ProcessCommand(HWND hwnd, ULONG ulId);
VOID ProcessMainClose(HWND hwnd);
BOOL InitPrefAppl(VOID);
BOOL InitDlgProc(VOID);
VOID MainSetDisable(VOID);
BOOL EndPref(BOOL flag, BOOL rc);
VOID InitBitmapData(VOID);
MRESULT EXPENTRY ACCFrameSampleProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
BOOL getStyler2Procedures(VOID);
BOOL CheckInstance(HWND hwnd);
POPTIONS getCurOptions(VOID);
BOOL Superclass(HAB hab, PSZ class, PSZ newClass, PFNWP pfn, BOOL public);
VOID InitMouKbdStrings(VOID);
VOID processStlrPrefArgs(PSZ pszArg);

// definitions --------------------------------------------------------------


//===========================================================================
// main
//===========================================================================

int main(INT argc, PSZ* argv) {
   POPTIONS po;
   QMSG qmsg;

   if (argc > 1) {
      processStlrPrefArgs(argv[1]);
      return 0;
   } /* endif */
   // alloca la memoria per la struttura dati dell'applicazione
   if (!(p = malloc(sizeof(APPDATA)))) {
      Wprint(HWND_DESKTOP, SZERR_MAINALL, PMPRNT_WININIT);
      return TRUE;
   } // end if

   // inizializzazione applicazione -----------------------------------------
   if (InitPrefAppl()) return 1;

   // creazione dialogo -----------------------------------------------------
   if (InitDlgProc()) return 1;

   // loop messaggi
   forever {
      if(WinGetMsg(p->hab, &qmsg, NULLHANDLE, 0, 0)) {    // loop standard
         if (qmsg.msg == WM_MOUSEMOVE &&      // se MOUSEMOVE
             p->is.active &&                // finestra attiva
             p->hCtlHelp != HWND_DESKTOP && // messaggio errore in corso
             qmsg.hwnd != p->hCtlHelp) {    // nuovo controllo
            p->hCtlHelp = qmsg.hwnd;
            WinPostMsg(p->hObj, WM_BUBBLEHELP, (MPARAM)qmsg.hwnd, MPVOID);
         } else if (qmsg.msg == WM_HELP) {
            qmsg.hwnd = p->hMain;
            qmsg.msg = WM_COMMAND;
            qmsg.mp1 = (MPARAM)BTN_HELP;
         } // end if
         WinDispatchMsg(p->hab, &qmsg);
      } else {                   // WM_QUIT
         if(qmsg.hwnd == NULLHANDLE)                       // SHUTDOWN
            WinPostMsg(p->hMain, WM_CLOSE, MPFROMLONG(TRUE), NULL);
         else if(qmsg.hwnd == HWNDFROMMP(qmsg.mp2))        // TASKLIST
            WinPostMsg(qmsg.hwnd, WM_CLOSE, 0L, 0L);
         else                            // chiusura regolare: termina
            break;
      } // end if
   } // end forever

   return (int)EndPref(TRUE, FALSE);
}


//===========================================================================
// procedura dialogo principale
// messaggi:
// WM_INITDLG
// WM_QUERYTRACKINFO
// WM_QUERYBORDERSIZE
// WM_WINDOWPOSCHANGED
// WM_MINMAXFRAME
// WM_CONTROL
// WM_COMMAND
// WM_BUBBLEHELP
// WM_CLOSE
// WM_STYLER2
//===========================================================================
static
MRESULT EXPENTRY MainDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   if (p->is.mainwinOK) {
      switch (msg) {
         case WM_ACTIVATE:
            p->is.active = (BOOL)mp1;
            break;
         case WM_QUERYTRACKINFO:
            return CheckWindowSizingProc(hwnd, mp1, mp2);
         case WM_QUERYISTANCE:
            return (MRESULT)(((HWND)mp1 == hwnd && (HWND)mp2 == hwnd)? hwnd: 0);
         case WM_SETBORDERSIZE:
            return GetPrefBorderSize(hwnd, mp1, mp2);
         case WM_WINDOWPOSCHANGED:
            return FormatControls(hwnd, mp1, mp2);
         case WM_MINMAXFRAME:
            MinMaxProc(hwnd, mp1);
            break;
         case WM_CONTROL:
            ProcessControl(hwnd, mp1, mp2);
            return (MRESULT)FALSE;
         case WM_COMMAND:
            return ProcessCommand(hwnd, (ULONG)mp1);
         case WM_CLOSE:
            ProcessMainClose(hwnd);
            WinPostMsg(hwnd, WM_QUIT, 0, 0);
            return (MRESULT)FALSE;
         case WM_STYLER2:
            if ((ULONG)mp1 == STLR_SETFRAMEFONT)
               return (MRESULT)TRUE;
            break;
      } /* endswitch */
   } // end if
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
}


//==========================================================================
// modifica la struttura TRACKINFO per controllare la dimensione minima
// finestra
// parametri:
// HWND, MPARAM, MPARAM
// valore restituito:
// MRESULT mr restituito dalla WinDefDlgProc()
//==========================================================================
static
MRESULT CheckWindowSizingProc(HWND hwnd, MPARAM mp1, MPARAM mp2) {
   MRESULT mr = WinDefDlgProc(hwnd, WM_QUERYTRACKINFO, mp1, mp2);
   ((PTRACKINFO)mp2)->ptlMinTrackSize.x = p->rcl.xRight;
   ((PTRACKINFO)mp2)->ptlMinTrackSize.y = p->rcl.yTop;
   return mr;
}


//==========================================================================
// memorizza spessore bordi finestra e altezza barra titolo
// parametri:
// HWND, MPARAM
// valore restituito:
// MRESULT restituito dalla WinDefDlgProc()
//==========================================================================
static
MRESULT GetPrefBorderSize(HWND hwnd, MPARAM mp1, MPARAM mp2) {
   p->ptlBrd.x = (LONG)mp1;
   p->ptlBrd.y = (LONG)mp2;
   return WinDefDlgProc(hwnd, WM_SETBORDERSIZE, mp1, mp2);
}


//==========================================================================
// riposiziona e ridimensiona i controlli secondo le nuove dimensioni della
// finestra
// parametri:
// HWND, MPARAM, MPARAM
// valore restituito:
// MRESULT cCtl restituito dalla WinDefDlgProc()
//==========================================================================
static
MRESULT FormatControls(HWND hwnd, MPARAM mp1, MPARAM mp2) {
   if ((((PSWP)mp1)->fl & SWP_SIZE) &&       // se ridimensionamento ma
       !(((PSWP)mp1)->fl & SWP_MINIMIZE) &&  // non minimizzazione
       (((PSWP)mp1)->cy > MINCY)) {          // e altezza < minimo cy
      // riposiziona i controlli ---------------------------------------------
      // alcuni valori gi… inizializzati in InitDlgProc()
      // valori comuni
      p->swpCtls[0].x = p->swpCtls[1].x = p->swpCtls[8].x = p->ptlBrd.x;
      p->swpCtls[1].y = p->swpCtls[2].y = p->swpCtls[3].y =
      p->swpCtls[4].y = p->swpCtls[5].y = p->swpCtls[6].y = p->ptlBrd.y;
      // notebook preferenze
      p->swpCtls[0].cx = ((PSWP)mp1)->cx - p->ptlBrd.x - p->ptlBrd.x;
      p->swpCtls[0].y = p->ptlBrd.y + 36;
      p->swpCtls[0].cy = ((PSWP)mp1)->cy - p->ptlBrd.y - p->ptlBrd.y -
                           p->cyTitle - p->swpCtls[0].y;
      // status bar
      p->swpCtls[1].cx = ((PSWP)mp1)->cx - p->ptlBrd.x - p->ptlBrd.x - 136;
      // bottoni
      p->swpCtls[2].x = p->swpCtls[1].x + p->swpCtls[1].cx + 2;
      p->swpCtls[3].x = p->swpCtls[2].x + p->swpCtls[2].cx + 1;
      p->swpCtls[4].x = p->swpCtls[3].x + p->swpCtls[3].cx + 1;
      p->swpCtls[5].x = p->swpCtls[4].x + p->swpCtls[4].cx + 1;
      p->swpCtls[6].x = p->swpCtls[5].x + p->swpCtls[5].cx + 1;
      // testo versione
      p->swpCtls[8].x = p->swpCtls[0].x + (p->swpCtls[0].cx -
                          p->szlLogo.cx) / 2;
      p->swpCtls[8].y = 44;
      p->swpCtls[8].cx = p->szlLogo.cx;
      p->swpCtls[8].cy = p->cyTitle;
      // logo
      p->swpCtls[7].x = p->swpCtls[8].x;
      p->swpCtls[7].y = (p->swpCtls[0].y + p->swpCtls[0].cy -
                           p->swpCtls[8].y - p->cyTitle -
                           p->szlLogo.cy ) / 2 +
                           p->swpCtls[8].y + p->cyTitle;
      WinSetMultWindowPos(p->hab, p->swpCtls, CMAINCTLS);
   } // end if
   // memorizza posizione/dimensione correnti
   if (!(((PSWP)mp1)->fl & (SWP_MAXIMIZE | SWP_RESTORE | SWP_MINIMIZE)) &&
       (((PSWP)mp1)->fl & (SWP_SIZE | SWP_MOVE)) && (((PSWP)mp1)->cy > MINCY)) {
      p->wrMain.x = ((PSWP)mp1)->x;
      p->wrMain.y = ((PSWP)mp1)->y;
      p->wrMain.cx = ((PSWP)mp1)->cx;
      p->wrMain.cy = ((PSWP)mp1)->cy;
   } /* endif */
   return WinDefDlgProc(hwnd, WM_WINDOWPOSCHANGED, mp1, mp2);
}


//==========================================================================
// nasconde status bar e notebook x evitare che ricoprano l'icona della
// applicazione quando la finestra Š ridotta al minimo
// parametri:
// HWND, MPARAM, MPARAM
// valore restituito:
// VOID
//==========================================================================
static
VOID MinMaxProc(HWND hwnd, MPARAM mp1) {
   BOOL fl;
   if (((PSWP)mp1)->fl & SWP_MINIMIZE) {
      WinShowWindow(p->swpCtls[0].hwnd, FALSE);
      WinShowWindow(p->swpCtls[1].hwnd, FALSE);
      WinShowWindow(p->swpCtls[7].hwnd, FALSE);
   } else if (WS_MINIMIZED & WinQueryWindowULong(hwnd, QWL_STYLE)) {
      WinShowWindow(p->swpCtls[0].hwnd, TRUE);
      WinShowWindow(p->swpCtls[1].hwnd, TRUE);
      WinShowWindow(p->swpCtls[7].hwnd, p->ocur.gen.disabled);
   } /* endif */
}


//==========================================================================
// determina quale pagina del notebook Š attiva, assegnandone il valore
// corrispondente a p->ulCurrPage. Se la pagina richiede il bottone apply
// lo abilita, altrimenti lo disabilita
// parametri:
// HWND hwnd: handle finestra principale dialogo
// MPARAM mp1: ID controllo + codice notifica
// MPARAM mp2: dati relativi all'evento
// valore restituito:
// VOID
//==========================================================================
static
VOID ProcessControl(HWND hwnd, MPARAM mp1, MPARAM mp2) {
   if (SHORT1FROMMP(mp1) == PREF_NOTEBOOK &&      // se Š stata selezionata
       SHORT2FROMMP(mp1) == BKN_PAGESELECTED) {   // una nuova pagina
      ULONG id = (ULONG)WinSendDlgItemMsg(hwnd, PREF_NOTEBOOK,
                           BKM_QUERYPAGEDATA,
                           MPFROMLONG(((PPAGESELECTNOTIFY)mp2)->ulPageIdNew),
                           MPVOID);
      // se pagina corrente = azioni mouse o tastiera legge valori correnti
      // in p->ocur
      if (p->idxPage == IDPG_MOUSE_FUNCS ||
          p->idxPage == IDPG_KEYB_FUNCS &&
          p->pd[p->idxPage].hwnd)
         MouKeybDlgUpdateStruct(p->pd[p->idxPage].hwnd);
      p->idxPage = id;
      p->pd[p->idxPage].def = (id == IDPG_COVER ||
                                   id == IDPG_TBARG_EXC ||
                                   id == IDPG_TBARH_EXC ||
                                   id == IDPG_WROLL_EXC ||
                                   id == IDPG_MAXWIN_EXC ||
                                   id == IDPG_SZMOVE_EXC ||
                                   id == IDPG_TXTG_EXC ||
                                   id == IDPG_VIO_EXC ||
                                   id == IDPG_MOUSEG_EXC)? 0: 1;
      // abilita/disabilita pulsanti toolbar secondo la pagina
      WinEnableWindow(p->swpCtls[3].hwnd, p->pd[p->idxPage].apply);
      WinEnableWindow(p->swpCtls[4].hwnd, p->pd[p->idxPage].undo);
      WinEnableWindow(p->swpCtls[5].hwnd, p->pd[p->idxPage].def);
      // usato x l'inizializzazione del dialogo azioni mouse/tastiera
      p->flFE = id == IDPG_MOUSE_FUNCS? FE_MOUSECLK: 0;
      // se nuova pagina = azioni mouse o tastiera reinizializza lista
      // azioni e eventi collegati
      if (p->idxPage == IDPG_MOUSE_FUNCS ||
          p->idxPage == IDPG_KEYB_FUNCS)
         MouKbdUpdateFElist(p->pd[p->idxPage].hwnd);
   } // end if
}


//==========================================================================
// HWND hwnd: handle finestra principale dialogo
// MPARAM mp1: ID controllo
// valore restituito:
// MRESULT FALSE
//==========================================================================
static
MRESULT ProcessCommand(HWND hwnd, ULONG ulId) {
   switch (ulId) {
      case BTN_ISONOFF:
         p->ocur.gen.disabled = !p->ocur.gen.disabled;
         MainSetDisable();
         WinPostMsg(p->hTmrObj, SMWM_DISABLE,
                    MPFROMLONG(p->ocur.gen.disabled), MPFROMHWND(p->hTmrObj));
         break;
      case BTN_APPLY:
         WinSendMsg(p->pd[p->idxPage].hwnd, WM_COMMAND,
                    (MPARAM)(BTN_APPLY), MPFROM2SHORT(CMDSRC_OTHER, FALSE));
         break;
      case BTN_UNDO:
         WinSendMsg(p->pd[p->idxPage].hwnd, WM_COMMAND,
                    (MPARAM)(BTN_UNDO), MPFROM2SHORT(CMDSRC_OTHER, FALSE));
         break;
      case BTN_DEFAULT:
         WinSendMsg(p->pd[p->idxPage].hwnd, WM_COMMAND,
                    (MPARAM)(BTN_DEFAULT), MPFROM2SHORT(CMDSRC_OTHER, FALSE));
         break;
      case BTN_HELP: {
         BYTE ab[] = {HLPID_MAIN, HLPID_GENERAL, HLPID_EXCEPTION,
                      HLPID_DIALOG, HLPID_EXCEPTION, HLPID_TITLEBAR,
                      HLPID_TITLEBAR, HLPID_EXCEPTION, HLPID_TBARHIDE,
                      HLPID_EXCEPTION, HLPID_WROLL, HLPID_EXCEPTION,
                      HLPID_MAXWIN, HLPID_EXCEPTION, HLPID_MOTION,
                      HLPID_EXCEPTION, HLPID_TEXT, HLPID_EXCEPTION,
                      HLPID_VIO, HLPID_EXCEPTION, HLPID_MOUGEN,
                      HLPID_EXCEPTION, HLPID_MOUOPT, HLPID_MOUOPT,
                      HLPID_WINKEY};
         WinSendMsg(p->hHelp, HM_DISPLAY_HELP,
                    (MPARAM)ab[p->idxPage], (MPARAM)HM_RESOURCEID);
      }  break;
   } /* endswitch */
   return (MRESULT)FALSE;
}


//==========================================================================
// - salva posiz/dimens delle varie finestre
// - se applyonexit applica settaggi pagine con flag apply = TRUE
// - se saveonexit scrive settaggi correnti nel file SMARTWIN.INI
//==========================================================================
static
VOID ProcessMainClose(HWND hwnd) {
   p->hini = openProfile(TRUE);
   // salva posizione/dimensioni finestra principale
   PrfWriteProfileData(p->hini, SZPRO_SIZEPOS, SZPRO_PREF,
                       (PVOID)&p->wrMain, sizeof(RECTL));
   // salva posizione/dimensioni dialogo funzioni mouse/tastiera
   PrfWriteProfileData(p->hini, SZPRO_SIZEPOS, SZPRO_MOUKBD,
                       (PVOID)&p->wrFE, sizeof(RECTL));
   // salva posizione/dimensioni dialogo selezione bitmap
   PrfWriteProfileData(p->hini, SZPRO_SIZEPOS, SZPRO_BMPSEL,
                       (PVOID)&p->wrBmpSel, sizeof(RECTL));
   // se opzione savelastpage memorizza indice pagina corrente
   if (p->ocur.gen.lastpage >= 0)
      p->po->gen.lastpage = p->ocur.gen.lastpage = p->idxPage;
   // applica settaggi correnti dove necessario
   if (p->ocur.gen.applyoe) GeneralApplyAll();
   // salva settaggi correnti
   if (p->ocur.gen.saveoe) {
      // evita salvataggio dati non applicati
      if (!p->ocur.gen.applyoe) {
         INT i;
         for (i = 1; i < CPAGES; ++i) {
            if (p->pd[i].apply && p->pd[i].undo && p->pd[i].hwnd) {
               p->flFE = 0;
               p->idxPage = i;
               if (i == IDPG_MOUSE_FUNCS) p->flFE = FE_MOUSECLK;
               WinSendMsg(p->pd[i].hwnd, WM_COMMAND, (MPARAM)(BTN_UNDO),
                          MPFROM2SHORT(CMDSRC_OTHER, FALSE));
            } // end if
         } /* endfor */
      } /* endif */
      PrfWriteProfileData(p->hini, SZPRO_OPTIONS, SZPRO_GLOBAL,
                          (PVOID)&p->ocur, sizeof(OPTIONS));
   } /* endif */
   stlrCloseProfile();
}


//==========================================================================
// Inizializzazione applicazione
// parametri:
// PAPPDATA p: indirizzo struttura dati applicazione
// valore restituito:
// BOOL rc: TRUE = errore, FALSE = successo
//==========================================================================
static
BOOL InitPrefAppl(VOID) {
   ULONG ul;              // usi generici
   HPS hps;
   CHAR buf[256];

   // avvia thread secondario
   if (StartObjectThread()) {
      Wprint(HWND_DESKTOP, SZERR_OBJTHREAD, PMPRNT_WININIT);
      return EndPref(FALSE, TRUE);
   } // end if
   // inizializza PM
   p->hab = WinInitialize(0);
   p->hmq = WinCreateMsgQueue(p->hab, 0);

   // controlla versione S.O.
   DosQuerySysInfo(QSV_VERSION_MINOR, QSV_VERSION_MINOR, &ul, sizeof(ULONG));
   p->is.osver = (BYTE)ul;
   // controlla presenza mouse 3 bottoni
   p->is.mou3btns = (WinQuerySysValue(HWND_DESKTOP, SV_CMOUSEBUTTONS) == 3);

   // carica modulo da cui ricava risorse dialoghi, bitmap e stringhe
   if (DosLoadModule(NULL, 0, SZ_RESMODULE, &p->hres)) {
      Wprint(HWND_DESKTOP, SZERR_LOADRES, PMPRNT_ERROR);
      return EndPref(FALSE, TRUE);
   } /* endif */
   // import some SMARTWIN.DLL procedures
   if (!getStyler2Procedures()) return FALSE;
   // registra classi controlli 3D
   if (!InitControls(p->hab) ||
       !Superclass(p->hab, WC_TITLEBAR, CC_SHADETITLE, ACCTitleWinProc, TRUE) ||
       !Superclass(p->hab, WC_FRAME, CC_FRAMESAMPLE, ACCFrameSampleProc, TRUE)) {
      MsgBox(HWND_DESKTOP, ID_ERR_REGCTLS, PMPRNT_ERROR);
      return EndPref(FALSE, TRUE);
   } // end if
   // altezza barra titolo, bordo finestra
   p->cyTitle = uGetSval(SV_CYTITLEBAR);
   p->ptlBrd.x = uGetSval(SV_CXSIZEBORDER);
   p->ptlBrd.y = uGetSval(SV_CYSIZEBORDER);
   return FALSE;
}


//==========================================================================
// procedura creazione e inizializzazione dialogo
// setta l'icona del dialogo, legge le preferenze correnti, legge precedente
// posizione/dimensioni finestra dal file INI, inserisce le pagine nel
// notebook, mostra la finestra
// parametri:
// PAPPDATA p : indirizzo struttura dati applicazione
// valore restituito:
// BOOL : TRUE = errore , FALSE = SUCCESSO
//==========================================================================
static
BOOL InitDlgProc(VOID) {
   HWND hCtls[CMAINCTLS];
   ULONG i;
   WRECTL wr;

   // crea dialogo
#ifdef STLR_WARP3
   if (NULLHANDLE == (p->hMain = WinLoadDlg(HWND_DESKTOP, NULLHANDLE,
                                            (PFNWP)MainDlgProc, p->hres,
                                            DLG_MAIN_3, NULL))) {
#else
   if (NULLHANDLE == (p->hMain = WinLoadDlg(HWND_DESKTOP, NULLHANDLE,
                                            (PFNWP)MainDlgProc, p->hres,
                                            DLG_MAIN, NULL))) {
#endif
      MsgBox(HWND_DESKTOP, ID_ERR_LOADDLG, PMPRNT_ERROR);
      sprintf(p->buf, "last error: %08x",
              WinGetLastError(p->hab));
      Wprint(HWND_DESKTOP, p->buf, PMPRNT_ERROR);
      return EndPref(FALSE, TRUE);
   } // end if

   // se Š gi… aperta un'altra occorrenza del programma, termina
   if (CheckInstance(p->hMain)) return EndPref(FALSE, TRUE);
   // ID processo
   WinQueryWindowProcess(p->hMain, &p->pid, NULL);

   // setta icona dialogo
   WsetDlgIcon(p->hMain, 0, 1);
   // ottiene l'handle della statusbar
   p->hSbar = WinWindowFromID(p->hMain, STBR_MAIN);
   // ottiene indirizzo opzioni correnti e copia opzioni in APPDATA
   if (NULL == (p->po = getCurOptions())) return FALSE;
   p->ou = p->ocur = *p->po;
   // inserisce le pagine
   if (!FillNotebook()) {
      MsgBox(HWND_DESKTOP, ID_ERR_FILLNOTEBOOK, PMPRNT_ERROR);
      return EndPref(TRUE, TRUE);
   } // end if
   // inizializza alcuni valori dell'array di SWP dei controlli
   if (WinMultWindowFromIDs(p->hMain, hCtls, PREF_NOTEBOOK, TXT_VERSION)
       != CMAINCTLS)
      return EndPref(TRUE, TRUE);
   for (i = 0; i <= BTN_HELP - PREF_NOTEBOOK; ++i) {
      p->swpCtls[i].hwnd = hCtls[i];
      p->swpCtls[i].fl = SWP_SIZE | SWP_MOVE;
      p->swpCtls[i].cx = 26;
      p->swpCtls[i].cy = 32;
   } // end for
   p->swpCtls[7].hwnd = hCtls[7];
   p->swpCtls[8].hwnd = hCtls[8];
   p->swpCtls[7].fl = SWP_MOVE;
   p->swpCtls[8].fl = SWP_SIZE | SWP_MOVE;
   WinQueryWindowRect(hCtls[7], (PRECTL)&wr);
   p->szlLogo.cx = wr.cx; p->szlLogo.cy = wr.cy;
   // costruisce stringa versione programma
   BuildVersionString(p->swpCtls[8].hwnd);
   // apre file inizializzazione
   if (!(p->hini = openProfile(FALSE))) {
      MsgBox(HWND_DESKTOP, ID_ERR_OPENPROFILE, PMPRNT_ERROR);
      return EndPref(TRUE, TRUE);
   } // end if
   InitBitmapData();
   InitMouKbdStrings();
   p->is.mainwinOK = 1;
   p->is.active = 1;
   initHelp();
   // ridimensiona e mostra il notebook delle impostazioni sullo schermo
   ShowNotebook();
   stlrCloseProfile();
   if (p->ocur.gen.lastpage >= 0)   // eventualmente riapre pagina precedente
      WinSendMsg(hCtls[0], BKM_TURNTOPAGE,
                 (MPARAM)p->pd[p->ocur.gen.lastpage].ulPg, MPVOID);
   MainSetDisable();
   return FALSE;
}


//==========================================================================
// valore restituito:
// VOID
//==========================================================================
static
VOID MainSetDisable(VOID) {
   HPS hps = WinGetPS(p->hMain);
   WsetButtonBmp(p->hMain, BTN_ISONOFF, GpiLoadBitmap(hps, NULLHANDLE,
                                     (p->ocur.gen.disabled? 101: 100),0, 0));
   WinReleasePS(hps);
   WinShowWindow(p->swpCtls[0].hwnd, !p->ocur.gen.disabled);
   WinShowWindow(p->swpCtls[7].hwnd, p->ocur.gen.disabled);
   WinShowWindow(p->swpCtls[8].hwnd, p->ocur.gen.disabled);
   WinEnableWindow(p->swpCtls[3].hwnd, !p->ocur.gen.disabled &&
                                          p->pd[p->idxPage].apply);
   WinEnableWindow(p->swpCtls[4].hwnd, !p->ocur.gen.disabled &&
                                          p->pd[p->idxPage].undo);
   WinEnableWindow(p->swpCtls[5].hwnd, !p->ocur.gen.disabled &&
                                          p->pd[p->idxPage].def);
   if (p->ocur.gen.disabled) {
      ShowErr(ID_SMWDISABLED);
      SetWinTxt(p->hMain, ID_SMWPREFDSBLD);
   } else {
      SetWinTxt(p->hMain, ID_TITLEREG);
      WinPostMsg(p->hObj, WM_TIMER, (MPARAM)TID_CONTEXTHELP, MPVOID);
   } /* endif */
}


//==========================================================================
// liberazione risorse e terminazione (flag = TRUE -> CheckInstance(0L))
// parametri:
// PAPPDATA p: indirizzo struttura dati applicazione
// BOOL flag: se TRUE setta a NULLHANDLE l'handle della finestra delle
//            impostazioni memorizzato nell'area condivisa di SmartWin
// BOOL rc: valore da restituire
// valore restituito:
// BOOL = a parametro BOOL rc
//==========================================================================
static
BOOL EndPref(BOOL flag, BOOL rc) {
   INT i;
   p->is.closing = 1;
   // free the bitmap data resources
   for (i = 0; i < TOTBMP; ++i) {
      if (p->abmp[i].p != p->abmpu[i].p)
         free(p->abmpu[i].p);
      free(p->abmp[i].p);
      GpiDeleteBitmap(p->abmp[i].hbmp);
   } /* endfor */
#ifdef DEMOVERSION
   if (p->po) free(p->po);
#else
   if (flag) CheckInstance(NULLHANDLE);
#endif
   endHelp();
   if (p->hMain) WinDestroyWindow(p->hMain);
   if (p->hmq) WinDestroyMsgQueue(p->hmq);
   WinTerminate(p->hab);
   if (p->hres) DosFreeModule(p->hres);
   if (p->hmod) DosFreeModule(p->hmod);
   free(p);
   return rc;
}

//===========================================================================
// Initialize the arrays of STLRBMP structures.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID InitBitmapData(VOID) {
   PSZ apszFiles[] = ASZPRO_BMPFILES;
   PSZ apszData[] = ASZPRO_BMPDATA;
   int i;
   ULONG cb;
   for (i = 0; i < TOTBMP; ++i) {
      // get the name of the bitmap file
      cb = PrfQueryProfileString(p->hini, SZPRO_OPTIONS, apszFiles[i], "",
                                 p->abmp[i].ach, CCHMAXPATHCOMP);
      // copy the name to the UNDO array
      memcpy(p->abmpu[i].ach, p->abmp[i].ach, cb);
      // get the bitmap data (in case of error just set the pointer to NULL)
      if (PrfQueryProfileSize(p->hini, SZPRO_OPTIONS, apszData[i], &cb) &&
          cb &&
          (NULL != (p->abmp[i].p = malloc(cb)))) {
         if (!PrfQueryProfileData(p->hini, SZPRO_OPTIONS, apszData[i],
                                   p->abmp[i].p, &cb)) {
            free(p->abmp[i].p);
            p->abmp[i].p = NULL;
         } else {
            p->abmpu[i].cb = p->abmp[i].cb = cb;
            p->abmpu[i].p = p->abmp[i].p;
         } /* endif */
      } /* endif */
   } /* endfor */
}


//==========================================================================
// procedura frame subclassata usata per il preview
//==========================================================================
static
MRESULT EXPENTRY ACCFrameSampleProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   PFNWP pfnwpFrame;
   switch (msg) {
      case WM_CREATE: {
         CLASSINFO ci;
         WinQueryClassInfo(WinQueryAnchorBlock(hwnd), WC_FRAME, &ci);
         WinSetWindowPtr(hwnd, 0L, (PVOID)ci.pfnWindowProc);
         } break;
      case WM_BUTTON1DOWN:
      case WM_BUTTON2DOWN:
      case WM_MOUSEMOVE:
         return (MRESULT)TRUE;
      default:
         break;
   } /* endswitch */
   pfnwpFrame = (PFNWP)WinQueryWindowPtr(hwnd, 0L);
   return (MRESULT) (*pfnwpFrame)(hwnd, msg, mp1, mp2);
}


//===========================================================================
// Get the address of procedures imported from SMARTWIN.DLL.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
BOOL getStyler2Procedures(VOID) {
#ifndef DEMOVERSION
   // controlla se smartwin.dll Š caricata in memoria e cerca di ottenere
   // l'indirizzo delle procedure che servono
   if (DosLoadModule(NULL, 0, SZ_MODULENAME, &p->hmod) ||
       getStlrProc(OFN_VERSION, &stlrFnVersion) ||
       getStlrProc(OFN_OPTIONS, &stlrFnOptions) ||
       getStlrProc(OFN_OBJECTHWND, &stlrFnObjectHwnd) ||
       getStlrProc(OFN_PREFHWND, &stlrFnPrefHwnd) ||
       getStlrProc(OFN_OPENPROFILE, &stlrFnOpenProfile) ||
       getStlrProc(OFN_HBMPFROMDATA, &stlrHbmpFromData) ||
       getStlrProc(OFN_HBMPFROMMODULE, &stlrHbmpFromModule) ||
       getStlrProc(OFN_BMPDATA, &stlrBmpDataFromPrf) ||
       getStlrProc(OFN_HBMPOFSHADE, &stlrHbmpOfShade)) {
      MsgBox(HWND_DESKTOP, ID_ERR_LOADMOD, PMPRNT_ERROR);
      return EndPref(FALSE, TRUE);
   } /* endif */
   p->hTmrObj = stlrFnObjectHwnd();
#endif
   return TRUE;
}


//===========================================================================
// se una finestra del programma Š gi… aperta trasferisce il focus su essa
// piuttosto che aprire una nuova occorrenza
// se chiamato durante inizializzazione e restituisce TRUE termina
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================
static
BOOL CheckInstance(HWND hwnd) {
#ifndef DEMOVERSION
   PHWND phPrev = stlrFnPrefHwnd();
   if (*phPrev &&
       *phPrev == (HWND)WinSendMsg(*phPrev, WM_QUERYISTANCE,
                                   (MPARAM)*phPrev, (MPARAM)*phPrev)) {
      WinSetWindowPos(*phPrev, HWND_TOP, 0, 0, 0, 0, SWP_ZORDER |
                      SWP_SHOW | SWP_FOCUSACTIVATE | SWP_RESTORE);
      return TRUE;
   }
   *phPrev = hwnd;
   return FALSE;
#else
   return TRUE;
#endif
}


//===========================================================================
// Get the current options.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// POPTIONS : address of current options.
//===========================================================================
static
POPTIONS getCurOptions(VOID) {
#ifdef DEMOVERSION
   POPTIONS pOpt;
   pOpt = (POPTIONS)malloc(sizeof(OPTIONS));
   if (!(p->hini = openProfile(FALSE))) {
      MsgBox(HWND_DESKTOP, ID_ERR_OPENPROFILE, PMPRNT_ERROR);
      free(pOpt);
      pOpt = NULL;
      EndPref(TRUE, TRUE);
   } else {
      ul = sizeof(OPTIONS);
      PrfQueryProfileData(p->hini, SZPRO_OPTIONS, SZPRO_GLOBAL,
                          (PVOID)p->po, &ul);
      stlrCloseProfile(p->hini);
   } // end if
   return pOpt;
#else
   return stlrFnOptions();
#endif
}


//===========================================================================
// Make a new window class from an already existing window class.
// Parameters --------------------------------------------------------------
// HAB hab      : anchor block handle.
// PSZ class    : parent class.
// PSZ newClass : name of the new class.
// PFNWP pfn    : new class procedure.
// BOOL public  : TRUE/FALSE (class has the CS_PUBLIC style flag).
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================
static
BOOL Superclass(HAB hab, PSZ class, PSZ newClass, PFNWP pfn, BOOL public) {
   CLASSINFO ci;
   if (!WinQueryClassInfo(hab, class, &ci) ||
       !WinRegisterClass(hab, newClass, pfn,
                         (public? ci.flClassStyle & ~CS_PUBLIC: ci.flClassStyle),
                         ci.cbWindowData))
      return FALSE;
   return TRUE;
}


VOID InitMouKbdStrings(VOID) {
   GetStr(ID_BKSPKEY, p->bksp, 32);
   GetStr(ID_DELKEY, p->del, 32);
   GetStr(ID_INSKEY, p->ins, 32);
   // tastiera
   p->fkeys[0] = "F1";
   p->fkeys[1] = "F2";
   p->fkeys[2] = "F3";
   p->fkeys[3] = "F4";
   p->fkeys[4] = "F5";
   p->fkeys[5] = "F6";
   p->fkeys[6] = "F7";
   p->fkeys[7] = "F8";
   p->fkeys[8] = "F9";
   p->fkeys[9] = "F10";
   p->fkeys[10] = "F11";
   p->fkeys[11] = "F12";
   p->fkeys[12] = p->bksp;
   p->fkeys[13] = p->del;
   p->fkeys[14] = p->ins;
   // mouse
   p->moubtns[0] = "1";
   p->moubtns[1] = "2";
   p->moubtns[2] = "3";
}


//===========================================================================
// Manages parameters passed to smartwin.exe.
// Currently only 2 parameters are allowed:
// "hsaw" -> hide/show all windows
// "ruaw" -> roll/unroll all windows
// Parameters --------------------------------------------------------------
// PSZ pszArg : smartwin.exe first startup argument
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID processStlrPrefArgs(PSZ pszArg) {
   HMODULE hmod;
   if (!DosLoadModule(NULL, 0, SZ_MODULENAME, &hmod)) {
      if (!DosQueryProcAddr(hmod, OFN_OBJECTHWND, NULL, (PFN*)&stlrFnObjectHwnd)) {
         if (!memcmp(pszArg, "hsaw", 5)) {
            WinPostMsg(stlrFnObjectHwnd(),
                       WM_STYLER2, (MPARAM)STLR_HIDEALL, MPVOID);
         } else if (!memcmp(pszArg, "ruaw", 5)) {
            WinPostMsg(stlrFnObjectHwnd(),
                       WM_STYLER2, (MPARAM)STLR_ROLLALL, MPVOID);
         } /* endif */
      } /* endif */
      DosFreeModule(hmod);
   } /* endif */
}
