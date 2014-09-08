//===========================================================================
// initnbk.c : preferences notebook initialization.
// --2003  - Alessandro Felice Cantatore
//===========================================================================

#pragma strings(readonly)

#include "pref.h"
#include "api.h"

// definitions --------------------------------------------------------------


// prototypes ---------------------------------------------------------------
VOID InsertPage(PBOOKPAGEINFO pbpi, ULONG fl);
PSZ makePageXofY(PSZ buf, ULONG ulPg, ULONG cPgs);


// globals ------------------------------------------------------------------


//==========================================================================
// Inizializza client WC_NOTEBOOK inserendo pagine di impostazioni
// TRUE = successo, FALSE = errore
//==========================================================================

BOOL FillNotebook(VOID) {
   POINTL ptl = {4, 8};     // usato per la conversione DlgUnits2pixel
   PBOOKPAGEINFO pbpi;
   if (!(pbpi = (PBOOKPAGEINFO)malloc(sizeof(BOOKPAGEINFO)))) return FALSE;
   memset(pbpi, 0, sizeof(BOOKPAGEINFO));
   if (!(pbpi->pszMajorTab = (PSZ)malloc(256)) ||
       !(pbpi->pszMinorTab = (PSZ)malloc(256))) goto end;
   p->hBook = WinWindowFromID(p->hMain, PREF_NOTEBOOK);
   // dati comuni
   pbpi->cb = sizeof(BOOKPAGEINFO);
   pbpi->pPageDlgCreateParams = p;
   pbpi->bLoadDlg = 0;
   pbpi->hwndPage = 0;
   pbpi->hmodPageDlg = p->hres;
   pbpi->pdlgtPage = NULL;
   pbpi->pBidiInfo = NULL;
   // copertina
   pbpi->fl = BFA_COMMON | BFA_MAJORTABTEXT | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_COVER;
   pbpi->pfnPageDlgProc = (PFN)CoverPageProc;
   pbpi->idPageDlg = DLG_COVER;
   pbpi->pszStatusLine = makePageXofY(p->buf, 1, 3);
   GetStr(ID_SMARTWINDOWS, pbpi->pszMajorTab, 256);
   GetStr(ID_PRODINFO, pbpi->pszMinorTab, 256);
   p->pd[IDPG_COVER].defFunc = NULL;
   InsertPage(pbpi, BKA_COMMON | BKA_MAJOR);
   // copertina opzioni generali
   pbpi->fl = BFA_COMMON | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_GENERAL;
   pbpi->pfnPageDlgProc = (PFN)GeneralOptProc;
   pbpi->idPageDlg = DLG_SMWNGEN;
   pbpi->pszStatusLine = makePageXofY(p->buf, 2, 3);
   GetStr(ID_GENERAL, pbpi->pszMinorTab, 256);
   p->pd[IDPG_GENERAL].defFunc = GeneralDefault;
   InsertPage(pbpi, BKA_COMMON | BKA_MINOR);
   // copertina eccezioni generali
   pbpi->fl = BFA_COMMON | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_GEN_EXC;
   pbpi->pfnPageDlgProc = (PFN)ExceptionsProc;;
   pbpi->idPageDlg = DLG_EXCEPTLIST;
   pbpi->pszStatusLine = makePageXofY(p->buf, 3, 3);
   GetStr(ID_EXCEPTIONS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_GEN_EXC].defFunc = NULL;
   InsertPage(pbpi, BKA_COMMON | BKA_MINOR);
   // pagina Main windows
   pbpi->fl = BFA_COMMON | BFA_MAJORTABTEXT | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_MAINWIN;
   pbpi->pfnPageDlgProc = (PFN)MainWinOptProc;
   pbpi->idPageDlg = DLG_MAINWINOPT;
   pbpi->pszStatusLine = makePageXofY(p->buf, 1, 2);
   GetStr(ID_MAINWIN, pbpi->pszMajorTab, 256);
   GetStr(ID_GENOPTS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_MAINWIN].defFunc = MainWinDefault;
   InsertPage(pbpi, BKA_COMMON | BKA_MAJOR | BKA_MINOR);
   // pagina eccezioni Main windows
   pbpi->fl = BFA_COMMON | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_MAINWIN_EXC;
   pbpi->pfnPageDlgProc = (PFN)ExceptionsProc;
   pbpi->idPageDlg = DLG_EXCEPTLIST;
   pbpi->pszStatusLine = makePageXofY(p->buf, 2, 2);
   GetStr(ID_EXCEPTIONS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_MAINWIN].defFunc = NULL;
   InsertPage(pbpi, BKA_COMMON | BKA_MINOR);
   // pagina titlebar - finestra attiva
   pbpi->fl = BFA_COMMON | BFA_MAJORTABTEXT | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_TBAR_ACTWIN;
   pbpi->pfnPageDlgProc = (PFN)TitleBarMainOptProc;
   pbpi->idPageDlg = DLG_TITLEBKGND;
   pbpi->pszStatusLine = makePageXofY(p->buf, 1, 5);
   GetStr(ID_TITLEBAR, pbpi->pszMajorTab, 256);
   GetStr(ID_TBARACTIVEWIN, pbpi->pszMinorTab, 256);
   p->pd[IDPG_TBAR_ACTWIN].defFunc = TbarMainOptDefault;
   InsertPage(pbpi, BKA_COMMON | BKA_MAJOR | BKA_MINOR);
   // pagina titlebar - finestra inattiva
   pbpi->fl = BFA_COMMON | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_TBAR_INACTWIN;
   pbpi->pfnPageDlgProc = (PFN)TitleBarMainOptProc;
   pbpi->idPageDlg = DLG_TITLEBKGND;
   pbpi->pszStatusLine = makePageXofY(p->buf, 2, 5);
   GetStr(ID_TBARINACTWIN, pbpi->pszMinorTab, 256);
   p->pd[IDPG_TBAR_INACTWIN].defFunc = TbarMainOptDefault;
   InsertPage(pbpi, BKA_COMMON | BKA_MINOR);
   // pagina eccezioni opzioni estetiche titlebar
   pbpi->fl = BFA_COMMON | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_TBARG_EXC;
   pbpi->pfnPageDlgProc = (PFN)ExceptionsProc;
   pbpi->idPageDlg = DLG_EXCEPTLIST;
   pbpi->pszStatusLine = makePageXofY(p->buf, 3, 5);
   GetStr(ID_EXCEPTIONS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_TBARG_EXC].defFunc = NULL;
   InsertPage(pbpi, BKA_COMMON | BKA_MINOR);
   // pagina titlebar - titlebar hiding
   pbpi->fl = BFA_COMMON | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_TBAR_HIDING;
   pbpi->pfnPageDlgProc = (PFN)TitleBarHideOptProc;
   pbpi->idPageDlg = DLG_HIDETBAR;
   pbpi->pszStatusLine = makePageXofY(p->buf, 4, 5);
   GetStr(ID_TBARHIDING, pbpi->pszMinorTab, 256);
   p->pd[IDPG_TBAR_HIDING].defFunc = TitleBarHideDefault;
   InsertPage(pbpi, BKA_COMMON | BKA_MINOR);
   // pagina eccezioni titlebar hiding
   pbpi->fl = BFA_COMMON | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_TBARH_EXC;
   pbpi->pfnPageDlgProc = (PFN)ExceptionsProc;
   pbpi->idPageDlg = DLG_EXCEPTLIST;
   pbpi->pszStatusLine = makePageXofY(p->buf, 5, 5);
   GetStr(ID_EXCEPTIONS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_TBARH_EXC].defFunc = NULL;
   InsertPage(pbpi, BKA_COMMON | BKA_MINOR);
   // pagina window rolling
   pbpi->fl = BFA_COMMON | BFA_MAJORTABTEXT | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_ROLLWIN;
   pbpi->pfnPageDlgProc = (PFN)WinRollOptProc;
   pbpi->idPageDlg = DLG_ROLLWIN;
   pbpi->pszStatusLine = makePageXofY(p->buf, 1, 2);
   GetStr(ID_WINDOWROLLING, pbpi->pszMajorTab, 256);
   GetStr(ID_GENOPTS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_ROLLWIN].defFunc = WinRollDefault;
   InsertPage(pbpi, BKA_COMMON | BKA_MAJOR | BKA_MINOR);
   // pagina eccezioni window rolling
   pbpi->fl = BFA_COMMON | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_WROLL_EXC;
   pbpi->pfnPageDlgProc = (PFN)ExceptionsProc;
   pbpi->idPageDlg = DLG_EXCEPTLIST;
   pbpi->pszStatusLine = makePageXofY(p->buf, 2, 2);
   GetStr(ID_EXCEPTIONS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_WROLL_EXC].defFunc = NULL;
   InsertPage(pbpi, BKA_COMMON | BKA_MINOR);
   // pagina finestre massimizzate
   pbpi->fl = BFA_COMMON | BFA_MAJORTABTEXT | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_MAXWIN;
   pbpi->pfnPageDlgProc = (PFN)MaxWinOptProc;
   pbpi->idPageDlg = DLG_MAXWIN;
   pbpi->pszStatusLine = makePageXofY(p->buf, 1, 2);
   GetStr(ID_MAXWIN, pbpi->pszMajorTab, 256);
   GetStr(ID_GENOPTS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_MAXWIN].defFunc = MaxWinDefault;
   InsertPage(pbpi, BKA_COMMON | BKA_MAJOR | BKA_MINOR);
   // pagina eccezioni finestre massimizzate
   pbpi->fl = BFA_COMMON | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_MAXWIN_EXC;
   pbpi->pfnPageDlgProc = (PFN)ExceptionsProc;
   pbpi->idPageDlg = DLG_EXCEPTLIST;
   pbpi->pszStatusLine = makePageXofY(p->buf, 2, 2);
   GetStr(ID_EXCEPTIONS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_MAXWIN_EXC].defFunc = NULL;
   InsertPage(pbpi, BKA_COMMON | BKA_MINOR);
   // pagina movimento-dimensionamento finestre
   pbpi->fl = BFA_COMMON | BFA_MAJORTABTEXT | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_SIZEMOVE;
   pbpi->pfnPageDlgProc = (PFN)SizeMoveOptProc;
   pbpi->idPageDlg = DLG_SIZEMOVE;
   pbpi->pszStatusLine = makePageXofY(p->buf, 1, 2);
   GetStr(ID_SIZEMOVE, pbpi->pszMajorTab, 256);
   GetStr(ID_GENOPTS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_SIZEMOVE].defFunc = SizeMoveDefault;
   InsertPage(pbpi, BKA_COMMON | BKA_MAJOR | BKA_MINOR);
   // pagina eccezioni movimento-dimensionamento finestre
   pbpi->fl = BFA_COMMON | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_SZMOVE_EXC;
   pbpi->pfnPageDlgProc = (PFN)ExceptionsProc;
   pbpi->idPageDlg = DLG_EXCEPTLIST;
   pbpi->pszStatusLine = makePageXofY(p->buf, 2, 2);
   GetStr(ID_EXCEPTIONS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_SZMOVE_EXC].defFunc = NULL;
   InsertPage(pbpi, BKA_COMMON | BKA_MINOR);
   // pagina controlli testo - generale
   pbpi->fl = BFA_COMMON | BFA_MAJORTABTEXT | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_TXTCTLGEN;
   pbpi->pfnPageDlgProc = (PFN)TxtCtlGenOptProc;
   pbpi->idPageDlg = DLG_TXTCTLGEN;
   pbpi->pszStatusLine = makePageXofY(p->buf, 1, 2);
   GetStr(ID_TXTCTLS, pbpi->pszMajorTab, 256);
   GetStr(ID_GENOPTS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_TXTCTLGEN].defFunc = TxtCtlGenDefault;
   InsertPage(pbpi, BKA_COMMON | BKA_MAJOR | BKA_MINOR);
   // pagina eccezioni controlli testo - generale
   pbpi->fl = BFA_COMMON | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_TXTG_EXC;
   pbpi->pfnPageDlgProc = (PFN)ExceptionsProc;
   pbpi->idPageDlg = DLG_EXCEPTLIST;
   pbpi->pszStatusLine = makePageXofY(p->buf, 2, 2);
   GetStr(ID_EXCEPTIONS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_TXTG_EXC].defFunc = NULL;
   InsertPage(pbpi, BKA_COMMON | BKA_MINOR);
   // pagina finestre VIO
   pbpi->fl = BFA_COMMON | BFA_MAJORTABTEXT | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_VIOWIN;
   pbpi->pfnPageDlgProc = (PFN)VIOwinOptProc;
   pbpi->idPageDlg = DLG_VIOWIN;
   pbpi->pszStatusLine = makePageXofY(p->buf, 1, 2);
   GetStr(ID_VIOWIN, pbpi->pszMajorTab, 256);
   GetStr(ID_GENOPTS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_VIOWIN].defFunc = VIOwinDefault;
   InsertPage(pbpi, BKA_COMMON | BKA_MAJOR | BKA_MINOR);
   // pagina eccezioni finestre VIO
   pbpi->fl = BFA_COMMON | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_VIO_EXC;
   pbpi->pfnPageDlgProc = (PFN)ExceptionsProc;
   pbpi->idPageDlg = DLG_EXCEPTLIST;
   pbpi->pszStatusLine = makePageXofY(p->buf, 2, 2);
   GetStr(ID_EXCEPTIONS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_VIO_EXC].defFunc = NULL;
   InsertPage(pbpi, BKA_COMMON | BKA_MINOR);
   // pagina mouse - autofocus, pointer hiding, button 3 click, move pointer
   pbpi->fl = BFA_COMMON | BFA_MAJORTABTEXT | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_MOUSE_GENOPTS;
   pbpi->pfnPageDlgProc = (PFN)MouseOptProc;
   pbpi->idPageDlg = DLG_MOUSEOPT;
   pbpi->pszStatusLine = makePageXofY(p->buf, 1, 3);
   GetStr(ID_MOUSE, pbpi->pszMajorTab, 256);
   GetStr(ID_GENOPTS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_MOUSE_GENOPTS].defFunc = MouseOptDefault;
   InsertPage(pbpi, BKA_COMMON | BKA_MAJOR | BKA_MINOR);
   // pagina eccezioni opzioni mouse
   pbpi->fl = BFA_COMMON | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_MOUSEG_EXC;
   pbpi->pfnPageDlgProc = (PFN)ExceptionsProc;
   pbpi->idPageDlg = DLG_EXCEPTLIST;
   pbpi->pszStatusLine = makePageXofY(p->buf, 2, 3);
   GetStr(ID_EXCEPTIONS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_MOUSEG_EXC].defFunc = NULL;
   InsertPage(pbpi, BKA_COMMON | BKA_MINOR);
   // pagina mouse - mouse actions
   pbpi->fl = BFA_COMMON | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_MOUSE_FUNCS;
   pbpi->pfnPageDlgProc = (PFN)MouKeybDlgProc;
   pbpi->idPageDlg = DLG_MOUKEYBACTS;
   pbpi->pszStatusLine = makePageXofY(p->buf, 3, 3);
   GetStr(ID_MAPPINGS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_MOUSE_FUNCS].defFunc = MouKeybDlgDefault;
   InsertPage(pbpi, BKA_COMMON | BKA_MINOR);
   // pagina keyboard - keyboard actions
   pbpi->fl = BFA_COMMON | BFA_MAJORTABTEXT | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_KEYB_FUNCS;
   pbpi->pfnPageDlgProc = (PFN)MouKeybDlgProc;
   pbpi->idPageDlg = DLG_MOUKEYBACTS;
   pbpi->pszStatusLine = makePageXofY(p->buf, 1, 2);
   GetStr(ID_KEYBOARD, pbpi->pszMajorTab, 256);
   GetStr(ID_MAPPINGS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_KEYB_FUNCS].defFunc = MouKeybDlgDefault;
   InsertPage(pbpi, BKA_COMMON | BKA_MAJOR | BKA_MINOR);
   // pagina Windows keys
   pbpi->fl = BFA_COMMON | BFA_MINORTABTEXT;
   pbpi->ulPageData = IDPG_KEYB_WKEY;
   pbpi->pfnPageDlgProc = (PFN)WinKeyDlgProc;
   pbpi->idPageDlg = DLG_WINKEY;
   pbpi->pszStatusLine = makePageXofY(p->buf, 2, 2);
   GetStr(ID_WINDOWSKEYS, pbpi->pszMinorTab, 256);
   p->pd[IDPG_KEYB_WKEY].defFunc = WinKeyDefault;
   InsertPage(pbpi, BKA_COMMON | BKA_MINOR);
   #ifdef STLR_WARP3
      WinMapDlgPoints(p->hBook, &ptl, 1, TRUE);
      WinSendMsg(p->hBook, BKM_SETDIMENSIONS,
                 MPFROM2SHORT((USHORT)(ptl.x * p->cxM),
                 (USHORT)(ptl.y + 8)),
                 MPFROMLONG(BKA_MAJORTAB));
      WinSendMsg(p->hBook, BKM_SETDIMENSIONS,
                 MPFROM2SHORT((USHORT)(ptl.x * p->cxm),
                 (USHORT)(ptl.y + 6)),
                 MPFROMLONG(BKA_MINORTAB));
   #endif
end:
   if (pbpi) {
      free((PVOID)pbpi->pszMajorTab);
      free((PVOID)pbpi->pszMinorTab);
      free((PVOID)pbpi);
   } /* endif */
   return TRUE;
}


//==========================================================================
// inserisce una pagina
//==========================================================================
static
VOID InsertPage(PBOOKPAGEINFO pbpi, ULONG fl) {
   BOOL rc;
   p->pd[pbpi->ulPageData].ulPg = (ULONG)WinSendMsg(p->hBook,
                                                 BKM_INSERTPAGE, MPVOID,
                                                 MPFROM2SHORT(fl, BKA_LAST));
   pbpi->cbStatusLine = strlen(pbpi->pszStatusLine);
   pbpi->cbMajorTab = strlen(pbpi->pszMajorTab);
   pbpi->cbMinorTab = strlen(pbpi->pszMinorTab);
   if (pbpi->cbMajorTab > p->cxM) p->cxM = pbpi->cbMajorTab;
   if (pbpi->cbMinorTab > p->cxm) p->cxm = pbpi->cbMinorTab;
   rc = (BOOL)WinSendMsg(p->hBook, BKM_SETPAGEINFO,
                         MPFROMLONG(p->pd[pbpi->ulPageData].ulPg),
                         (MPARAM)pbpi);
}


//==========================================================================
//  |
//==========================================================================

VOID ShowNotebook(VOID) {
   ULONG ul = sizeof(RECTL);
   // ricava dimensioni minime notebook
   p->rcl.xRight = CX_PAGEDLG; p->rcl.yTop = CY_PAGEDLG;
   WinMapDlgPoints(p->hBook, ((PPOINTL)&p->rcl) + 1, 1, TRUE);
   p->rcl.xLeft = 0;
   p->rcl.yBottom = 0;
   WinSendMsg(p->hBook, BKM_CALCPAGERECT,
              MPFROMP((PVOID)&p->rcl), MPVOID);
   p->rcl.xRight = p->rcl.xRight - p->rcl.xLeft +
                     2 * WinQuerySysValue(HWND_DESKTOP, SV_CXSIZEBORDER);
   p->rcl.yTop = p->rcl.yTop - p->rcl.yBottom + p->cyTitle + 28 +
                   2 * WinQuerySysValue(HWND_DESKTOP, SV_CYSIZEBORDER);
   // ricava posizione/dimensioni memorizzate in smartwin.ini
   PrfQueryProfileData(p->hini, SZPRO_SIZEPOS, SZPRO_MOUKBD,
                       (PVOID)&p->wrFE, &ul);
   ul = sizeof(RECTL);
   PrfQueryProfileData(p->hini, SZPRO_SIZEPOS, SZPRO_BMPSEL,
                       (PVOID)&p->wrBmpSel, &ul);
   ul = sizeof(RECTL);
   if (!PrfQueryProfileData(p->hini, SZPRO_SIZEPOS, SZPRO_PREF,
                            (PVOID)&p->wrMain, &ul)) {
      p->rcl.xLeft = (WinQuerySysValue(HWND_DESKTOP, SV_CXFULLSCREEN) -
                        p->rcl.xRight) / 2;
      p->rcl.yBottom = (WinQuerySysValue(HWND_DESKTOP, SV_CYFULLSCREEN) -
                         p->rcl.yTop) / 2;
      memcpy((void*)&p->wrMain, (void*)&p->rcl, sizeof(RECTL));
   } // end if
   WinSetWindowPos(p->hMain, 0, p->wrMain.x, p->wrMain.y,
                   p->wrMain.cx, p->wrMain.cy,
                   SWP_SIZE | SWP_MOVE | SWP_SHOW);
}


// costruisce la stringa "pagina ulPg di cPgs"

PSZ makePageXofY(PSZ buf, ULONG ulPg, ULONG cPgs) {
   PSZ psz = p->buf;
   PSZ p1 = NULL;
   PSZ p2 = NULL;
   GetStr(ID_PAGEXOFY, psz, 256);
   while (*psz) {
      if       (*psz == '1') p1 = psz;
      else if  (*psz == '2') p2 = psz;
      psz++;
   } /* endwhile */
   if (p1) *p1 = ulPg + '0';
   if (p2) *p2 = cPgs + '0';
   return p->buf;
}

