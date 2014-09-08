//==========================================================================
// moukeybdlg.c: dialogo funzioni attivabili da mouse e tastiera
//==========================================================================

#pragma strings(readonly)

#include "pref.h"
#include "api.h"

// prototypes ---------------------------------------------------------------
MRESULT MouKeybDlgInit(HWND hwnd);
MRESULT MouKeybDlgCheckMinSize(HWND hwnd, MPARAM mp1, MPARAM mp2);
MRESULT MouKeybDlgFormatCtls(HWND hwnd, MPARAM mp1, MPARAM mp2);
VOID MouKeybDlgMinMax(HWND hwnd, MPARAM mp1);
MRESULT MouKeybDlgControl(HWND hwnd, MPARAM mp1, MPARAM mp2);
MRESULT MouKeybDlgCommand(HWND hwnd, ULONG ulId);
VOID MouKeybDlgInitCtlsCoords(HWND hwnd, PFEDATA pfed);
VOID MouKeybDlgUpdateStruct(HWND hwnd);
VOID MouKeybDlgFillLbox(HWND hwnd, PFEDATA pfed);
VOID MouKeybDlgEnableCtls(HWND hwnd, BOOL fl);
VOID MouKeybDlgApply(HWND hwnd);
VOID MouKeybDlgUndo(HWND hwnd);

//==========================================================================
// procedura dialogo impostazioni azioni mouse/tastiera
//==========================================================================

MRESULT EXPENTRY MouKeybDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         return MouKeybDlgInit(hwnd);
      case WM_ACTIVATE:
         if (p->flFE & FE_FLOATDLG) p->is.active = (BOOL)mp1;
         break;
      case WM_QUERYTRACKINFO:
         return MouKeybDlgCheckMinSize(hwnd, mp1, mp2);
      case WM_WINDOWPOSCHANGED:
         return MouKeybDlgFormatCtls(hwnd, mp1, mp2);
        // break;
      case WM_MINMAXFRAME:
         MouKeybDlgMinMax(hwnd, mp1);
         break;
      case WM_CONTROL:
         return MouKeybDlgControl(hwnd, mp1, mp2);
      case WM_COMMAND:
         return MouKeybDlgCommand(hwnd, (ULONG)mp1);
      case WM_CLOSE:
         WinDismissDlg(hwnd, FALSE);
         return (MRESULT)FALSE;
      case WM_DESTROY:
         free((PVOID)WinQueryWindowPtr(hwnd, 0L));
         break;
   } /* endswitch */
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
}


//==========================================================================
//
// parametri:
// HWND hwnd: handle dialogo
// PAPPDATA p: struttura dati applicazione
// valore restituito:
// MRESULT FALSE
//==========================================================================

MRESULT MouKeybDlgInit(HWND hwnd) {
   PFEDATA pfed;
   ULONG ul;
   // allocazione struttura dati applicazione
   if (!(pfed = (PFEDATA)malloc(sizeof(FEDATA)))) {
      ShowErr(ID_ERR_BSDALLOC);
      WinPostMsg(hwnd, WM_CLOSE, MPVOID, MPVOID);
      return FALSE;
   } /* endif */
   memset(pfed, 0, sizeof(FEDATA));
   WinSetWindowPtr(hwnd, 0L, (PVOID)pfed);   // assegnazione window words
   // setta indice array e item correntemente selezionato a LIT_NONE
   pfed->iBox = -1;             // indice relativo listbox
   pfed->iSel = -1;             // indice relativo array
   if (p->flFE & FE_FLOATDLG) {
      // imposta icona
      pfed->hico = WsetDlgIcon(hwnd, p->hres, 3);
      // si aggiunge alla window list
      pfed->swctl.hwnd = hwnd;
      pfed->swctl.idProcess = p->pid;
      pfed->swctl.uchVisibility = SWL_VISIBLE;
      pfed->swctl.fbJump = SWL_JUMPABLE;
      // setta il testo del titolo
      SetCtlTxt(hwnd, FID_TITLEBAR, (p->flFE & FE_MOUSECLK?
                                     ID_MOUCTRLDFUNCS: ID_KEYBCTRLDFUNCS));
      strncpy(pfed->swctl.szSwtitle, p->buf, MAXNAMEL);
      pfed->swctl.bProgType = PROG_DEFAULT;
      WinAddSwitchEntry(&pfed->swctl);
      // rileva dimensioni minime dialogo
      pfed->szMin.cx = CX_PAGEDLG;
      pfed->szMin.cy = CY_PAGEDLG;
      WinMapDlgPoints(hwnd, (PPOINTL)&pfed->szMin, 1, TRUE);
      pfed->szMin.cx += p->ptlBrd.x + p->ptlBrd.x;
      pfed->szMin.cy += p->cyTitle + p->ptlBrd.y +
                        p->ptlBrd.y;
   } else {
      p->flFE = (p->idxPage == IDPG_MOUSE_FUNCS? FE_MOUSECLK: 0);
   } /* endif */
   // cambia i parametri di presentazione del dialogo e del titolo
   WsetDlgFonts(hwnd, 0, NULL);
   // inizializza array ahwnd e aswp
   WinMultWindowFromIDs(hwnd, pfed->ahwnd, FE_CTL1ST, FE_CTLLAST);
   for (ul = 0; ul < FE_CCTLS; ++ul) {
      pfed->aswp[ul].hwnd = pfed->ahwnd[ul];
   } // end for
   // inizializza coordinate e stato visibilit… controlli
   MouKeybDlgInitCtlsCoords(hwnd, pfed);
   // riempie la listbox
   MouKeybDlgFillLbox(hwnd, pfed);
   // inizializza alcuni controlli secondo stile e stato
   if (p->flFE & FE_MOUSECLK) {
      SetCtlTxt(hwnd, TXT_FEKEYBTN, ID_MOUBTN);
      WinSendDlgItemMsg(hwnd, SPIN_FEFKEYBTN, SPBM_SETARRAY,
                        (MPARAM)p->moubtns, (MPARAM)CMOUBTNS);
   } else {
      WinSendDlgItemMsg(hwnd, SPIN_FEFKEYBTN, SPBM_SETARRAY,
                        (MPARAM)p->fkeys, (MPARAM)CFKEYS);
   } /* endif */
   // se dialogo indipendente imposta dimensioni dialogo
   if (p->flFE & FE_FLOATDLG) {
      WinSendMsg(pfed->ahwnd[1], LM_SELECTITEM,
                 (MPARAM)pfed->fel[LOUSHORT(p->flFE)].idx, (MPARAM)TRUE);
      // se non Š presente posiz dimens precedente
      if (!p->wrFE.cx)
         WcenterWindow(hwnd);
      else
         WinSetWindowPos(hwnd, 0, p->wrFE.x, p->wrFE.y,
                         p->wrFE.cx, p->wrFE.cy,
                         SWP_SIZE | SWP_MOVE | SWP_SHOW);
   } else {
     p->pd[p->idxPage].hwnd = hwnd;
     setparown(hwnd);
   } // end if
   return (MRESULT)FALSE;
}


//==========================================================================\
// modifica la struttura TRACKINFO per controllare la dimensione minima     |
// finestra                                                                 |
// parametri:                                                               |
// HWND, MPARAM, MPARAM                                                     |
// valore restituito:                                                       |
// MRESULT mr restituito dalla WinDefDlgProc()                              |
//==========================================================================/

MRESULT MouKeybDlgCheckMinSize(HWND hwnd, MPARAM mp1, MPARAM mp2) {
   PFEDATA pfed = (PFEDATA)WinQueryWindowPtr(hwnd, 0L);
   MRESULT mr = WinDefDlgProc(hwnd, WM_QUERYTRACKINFO, mp1, mp2);
   ((PTRACKINFO)mp2)->ptlMinTrackSize.x = pfed->szMin.cx;
   ((PTRACKINFO)mp2)->ptlMinTrackSize.y = pfed->szMin.cy;
   return mr;
}


//==========================================================================\
// riposiziona e ridimensiona i controlli secondo le nuove dimensioni della |
// finestra                                                                 |
// parametri:                                                               |
// HWND, MPARAM, MPARAM                                                     |
// valore restituito:                                                       |
// VOID MRESULT                                                             |
//==========================================================================/

MRESULT MouKeybDlgFormatCtls(HWND hwnd, MPARAM mp1, MPARAM mp2) {
   PFEDATA pfed = (PFEDATA)WinQueryWindowPtr(hwnd, 0L);
   WinDefDlgProc(hwnd, WM_WINDOWPOSCHANGED, mp1, mp2);
   if (!pfed) return (MRESULT)FALSE;
   if ((((PSWP)mp1)->fl & SWP_SIZE) &&      // se ridimensionamento ma
       !(((PSWP)mp1)->fl & SWP_MINIMIZE) &&  // non minimizzazione
       (((PSWP)mp1)->cy > MINCY)) {          // non arrotolamento
      POINTL ptl = {2, 2};
      POINTL ptlBrd = {0, 0};
      if (!pfed) return (MRESULT)FALSE;
      WinMapDlgPoints(hwnd, &ptl, 1, TRUE);
      if (p->flFE & FE_FLOATDLG)
         ptlBrd = p->ptlBrd;
      // larghezza statusbar-listbox
      pfed->aswp[1].cx = pfed->aswp[2].cx = ((PSWP)mp1)->cx - ptlBrd.x -
                                            ptlBrd.x - ptl.x - ptl.x;
      // altezza listbox
      pfed->aswp[1].cy = ((PSWP)mp1)->cy - pfed->aswp[1].y - ptlBrd.y -
                         pfed->aswp[0].cy - ptl.y -
                         (p->flFE & FE_FLOATDLG? p->cyTitle: 0);
      // coordinata y caption listbox
      pfed->aswp[0].y = pfed->aswp[1].y + pfed->aswp[1].cy;
      WinSetMultWindowPos(p->hab, pfed->aswp, FE_CCTLS);
   } // end if
   if ((p->flFE & FE_FLOATDLG) &&
       !(((PSWP)mp1)->fl & (SWP_MAXIMIZE | SWP_RESTORE | SWP_MINIMIZE)) &&
       (((PSWP)mp1)->fl & (SWP_SIZE | SWP_MOVE)) &&
       (((PSWP)mp1)->cy > MINCY)) {
      p->wrFE.x = ((PSWP)mp1)->x;
      p->wrFE.y = ((PSWP)mp1)->y;
      p->wrFE.cx = ((PSWP)mp1)->cx;
      p->wrFE.cy = ((PSWP)mp1)->cy;
   } /* endif */
   return (MRESULT)FALSE;
}


//==========================================================================\
// nasconde controlli x evitare che ricopra l'icona quando il dlg Š ridotto |
// al minimo                                                                |
// parametri:                                                               |
// HWND, MPARAM                                                             |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID MouKeybDlgMinMax(HWND hwnd, MPARAM mp1) {
   PFEDATA pfed = (PFEDATA)WinQueryWindowPtr(hwnd, 0L);
   if (p->flFE & FE_FLOATDLG) {
      ULONG fl = !(((PSWP)mp1)->fl & SWP_MINIMIZE);
      WinShowWindow(pfed->ahwnd[11], fl);
      WinShowWindow(pfed->ahwnd[12], fl);
   } // end if
}


//==========================================================================\
// parametri:                                                               |
// HWND, MPARAM                                                             |
// valore restituito:                                                       |
// MRESULT FALSE                                                            |
//==========================================================================/

MRESULT MouKeybDlgControl(HWND hwnd, MPARAM mp1, MPARAM mp2) {
   PFEDATA pfed = (PFEDATA)WinQueryWindowPtr(hwnd, 0L);
   switch (SHORT1FROMMP(mp1)) {
      case LBOX_FEFNCTEVNTS: // listbox: setta controlli secondo item selez
         if (SHORT2FROMMP(mp1) == LN_SELECT) {
            ULONG i, ul = 0;
            pfed->iBox = (SHORT)WinSendMsg((HWND)mp2, LM_QUERYSELECTION,
                                           (MPARAM)LIT_FIRST, MPVOID);
            i = (ULONG)WinSendMsg((HWND)mp2, LM_QUERYITEMHANDLE,
                                   (MPARAM)pfed->iBox, MPVOID);
            // trova dati corrispondenti all'item selezionato
            for (i = 0; pfed->fel[i].idf; ++i)
               if (pfed->iBox == pfed->fel[i].idx) break;
            // se dati validi setta controlli
            if (pfed->fel[i].idf) {
               SetWinTxt(pfed->ahwnd[2], pfed->fel[i].idTrg);
               pfed->iSel = i;
               WinEnableControl(hwnd, BTN_FEOK, TRUE);            // btn OK
               WinEnableControl(hwnd, CHK_FEENABLE, pfed->fel[i].ctl); // chkEnable
               WinSendDlgItemMsg(hwnd, CHK_FEENABLE, BM_SETCHECK,
                                 (MPARAM)pfed->fel[i].fl, MPVOID);
               MouKeybDlgEnableCtls(hwnd, pfed->fel[i].fl);
               if (p->flFE & FE_MOUSECLK) {
                  // setta valore in spinbutton
                  switch (pfed->fel[i].mouVK) {
                     case WM_BUTTON3DOWN:
                     case WM_BUTTON3CLICK:
                     case WM_BUTTON3DBLCLK:
                        ul = 2;
                        break;
                     case WM_BUTTON2DOWN:
                     case WM_BUTTON2CLICK:
                     case WM_BUTTON2DBLCLK:
                        ul = 1;
                        break;
                     default:
                        break;
                  } /* endswitch */
                  WinSendDlgItemMsg(hwnd, SPIN_FEFKEYBTN,
                                    SPBM_SETCURRENTVALUE,
                                    (MPARAM)ul, MPVOID);
                  // radiobuttons click/doubleclick
                  if (pfed->fel[i].mouVK == WM_BUTTON1CLICK ||
                      pfed->fel[i].mouVK == WM_BUTTON2CLICK ||
                      pfed->fel[i].mouVK == WM_BUTTON3CLICK) {
                     WinSendDlgItemMsg(hwnd, RDBTN_FECLICK,
                                       BM_SETCHECK, (MPARAM)1, MPVOID);
                  } else if (pfed->fel[i].mouVK == WM_BUTTON1DBLCLK ||
                             pfed->fel[i].mouVK == WM_BUTTON2DBLCLK ||
                             pfed->fel[i].mouVK == WM_BUTTON3DBLCLK) {
                     WinSendDlgItemMsg(hwnd, RDBTN_FEDBLCLK,
                                       BM_SETCHECK, (MPARAM)1, MPVOID);
                  } /* endif */
                  DlgShowCtl(hwnd, RDBTN_FECLICK, pfed->fel[i].click);
                  DlgShowCtl(hwnd, RDBTN_FEDBLCLK, pfed->fel[i].click);
               } else {
                  ULONG avk[] = VKEYLIST;
                  for (ul = 0;
                       (ul < CVKEYLIST) && (avk[ul] != pfed->fel[i].mouVK);
                       ++ul)
                     ;
                  if (ul == CVKEYLIST) ul = 0;
                  WinSendDlgItemMsg(hwnd, SPIN_FEFKEYBTN, SPBM_SETCURRENTVALUE,
                                    (MPARAM)ul, MPVOID);
               } // end if
               WinSendDlgItemMsg(hwnd, CHK_FECTRL, BM_SETCHECK,
                          (MPARAM)(pfed->fel[i].KC & KC_CTRL? 1: 0), MPVOID);
               WinSendDlgItemMsg(hwnd, CHK_FEALT, BM_SETCHECK,
                          (MPARAM)(pfed->fel[i].KC & KC_ALT? 1: 0), MPVOID);
               WinSendDlgItemMsg(hwnd, CHK_FESHIFT, BM_SETCHECK,
                          (MPARAM)(pfed->fel[i].KC & KC_SHIFT? 1: 0), MPVOID);
            } /* endif */
         } /* endif */
         break;
      case CHK_FEENABLE:            // checkbox enable
         if ((SHORT2FROMMP(mp1) == BN_CLICKED ||
             SHORT2FROMMP(mp1) == BN_DBLCLICKED)) {
            pfed->fel[pfed->iSel].fl =
                                 WinQueryButtonCheckstate(hwnd, CHK_FEENABLE);
            MouKeybDlgEnableCtls(hwnd, pfed->fel[pfed->iSel].fl);
            if (!(p->flFE & FE_FLOATDLG)) {
               EnableToolbarApplyBtn(1);
               EnableToolbarUndoBtn(1);
            } else {
               if (p->flFE & FE_MOUSECLK) {
                  p->pd[IDPG_MOUSE_FUNCS].undo = 1;
               } else {
                  p->pd[IDPG_KEYB_FUNCS].undo = 1;
               } // end if
            } // end if
         } // end if
         break;
      case SPIN_FEFKEYBTN:          // spinbutton fine spin o radiobtn click
      case RDBTN_FECLICK:
      case RDBTN_FEDBLCLK:
         if (SHORT2FROMMP(mp1) == SPBN_ENDSPIN ||
             SHORT2FROMMP(mp1) == BN_CLICKED ||
             SHORT2FROMMP(mp1) == BN_DBLCLICKED) {
            ULONG iSpin = 0;
            WinSendDlgItemMsg(hwnd, SPIN_FEFKEYBTN,
                              SPBM_QUERYVALUE, (MPARAM)&iSpin, MPVOID);
            // se dialogo settaggi azioni mouse
            if (p->flFE & FE_MOUSECLK) {
               switch (iSpin) {
                  case 0:
                     pfed->fel[pfed->iSel].mouVK =
                              pfed->fel[pfed->iSel].click?
                               (WinQueryButtonCheckstate(hwnd, RDBTN_FECLICK)?
                                  WM_BUTTON1CLICK: WM_BUTTON1DBLCLK):
                               WM_BUTTON1DOWN;
                     break;
                  case 1:
                     pfed->fel[pfed->iSel].mouVK =
                              pfed->fel[pfed->iSel].click?
                               (WinQueryButtonCheckstate(hwnd, RDBTN_FECLICK)?
                                  WM_BUTTON2CLICK: WM_BUTTON2DBLCLK):
                               WM_BUTTON2DOWN;
                     break;
                  default:
                     pfed->fel[pfed->iSel].mouVK =
                              pfed->fel[pfed->iSel].click?
                               (WinQueryButtonCheckstate(hwnd, RDBTN_FECLICK)?
                                  WM_BUTTON3CLICK: WM_BUTTON3DBLCLK):
                               WM_BUTTON3DOWN;
                     break;
               } /* endswitch */
            } else {   // se dialogo settaggi azioni tastiera
               ULONG avk[] = VKEYLIST;
               pfed->fel[pfed->iSel].mouVK = avk[iSpin];
            } /* endif */
            if (!(p->flFE & FE_FLOATDLG)) {
               EnableToolbarApplyBtn(1);
               EnableToolbarUndoBtn(1);
            } else {
               if (p->flFE & FE_MOUSECLK) {
                  p->pd[IDPG_MOUSE_FUNCS].undo = 1;
               } else {
                  p->pd[IDPG_KEYB_FUNCS].undo = 1;
               } // end if
            }
         } /* endif */
         break;
      case CHK_FECTRL:
         if ((SHORT2FROMMP(mp1) == BN_CLICKED ||
             SHORT2FROMMP(mp1) == BN_DBLCLICKED)) {
            if (WinQueryButtonCheckstate(hwnd, CHK_FECTRL)) {
               pfed->fel[pfed->iSel].KC |= KC_CTRL;
            } else {
               pfed->fel[pfed->iSel].KC &= ~KC_CTRL;
            } /* endif */
            if (!(p->flFE & FE_FLOATDLG)) {
               EnableToolbarApplyBtn(1);
               EnableToolbarUndoBtn(1);
            } else {
               if (p->flFE & FE_MOUSECLK) {
                  p->pd[IDPG_MOUSE_FUNCS].undo = 1;
               } else {
                  p->pd[IDPG_KEYB_FUNCS].undo = 1;
               } // end if
            }
         } // end if
         break;
      case CHK_FEALT:
         if ((SHORT2FROMMP(mp1) == BN_CLICKED ||
             SHORT2FROMMP(mp1) == BN_DBLCLICKED)) {
            if (WinQueryButtonCheckstate(hwnd, CHK_FEALT)) {
               pfed->fel[pfed->iSel].KC |= KC_ALT;
            } else {
               pfed->fel[pfed->iSel].KC &= ~KC_ALT;
            } /* endif */
            if (!(p->flFE & FE_FLOATDLG)) {
               EnableToolbarApplyBtn(1);
               EnableToolbarUndoBtn(1);
            } else {
               if (p->flFE & FE_MOUSECLK) {
                  p->pd[IDPG_MOUSE_FUNCS].undo = 1;
               } else {
                  p->pd[IDPG_KEYB_FUNCS].undo = 1;
               } // end if
            }
         } // end if
         break;
      case CHK_FESHIFT:
         if ((SHORT2FROMMP(mp1) == BN_CLICKED ||
             SHORT2FROMMP(mp1) == BN_DBLCLICKED)) {
            if (WinQueryButtonCheckstate(hwnd, CHK_FESHIFT)) {
               pfed->fel[pfed->iSel].KC |= KC_SHIFT;
            } else {
               pfed->fel[pfed->iSel].KC &= ~KC_SHIFT;
            } /* endif */
            if (!(p->flFE & FE_FLOATDLG)) {
               EnableToolbarApplyBtn(1);
               EnableToolbarUndoBtn(1);
            } else {
               if (p->flFE & FE_MOUSECLK) {
                  p->pd[IDPG_MOUSE_FUNCS].undo = 1;
               } else {
                  p->pd[IDPG_KEYB_FUNCS].undo = 1;
               } // end if
            }
         } // end if
         break;
   } /* endswitch */
   return (MRESULT)FALSE;
}


//==========================================================================
// processa WM_COMMAND
// parametri:
// HWND hwnd: handle del dialogo
// ULONG ulId: id bottone
// valore restituito:
// MRESULT FALSE
//==========================================================================

MRESULT MouKeybDlgCommand(HWND hwnd, ULONG ulId) {
   switch (ulId) {
      // bottoni toolbar ---------------------------------------------------
      case BTN_APPLY:
         MouKeybDlgApply(hwnd);
         break;
      case BTN_UNDO:
         MouKeybDlgUndo(hwnd);
         break;
      case BTN_DEFAULT:
         MouKeybDlgDefault(hwnd);
         break;
      // bottoni dialogo indipendente
      case BTN_FEOK:
         MouKeybDlgApply(hwnd);
         WinDismissDlg(hwnd, TRUE);
         break;
      case BTN_FECANCEL:
         WinDismissDlg(hwnd, FALSE);
         break;
      case BTN_FEHELP:
         WshowInf(hwnd, ID_INF_FILE, ID_INF_MOUKEY);
         break;
   } /* endswitch */
   return (MRESULT)FALSE;
}


//==========================================================================\
// Inizializza i valori dell'array delle strutture SWP relative ai controlli|
// del dialogo                                                              |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// PSWP pswp: array strutture SWP controlli                                 |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID MouKeybDlgInitCtlsCoords(HWND hwnd, PFEDATA pfed) {
   POINTL ptl[6] = {{0, 0},        // bordo
                    {2, 2},        // distanze
                    {216, 10},     // dimensioni statusbar
                    {2, 48},       // coordinate listbox pagina notebook
                    {2, 68},       // coordinate listbox dialogo indipendente
                    {0, 8}};       // altezza static text
   WinMapDlgPoints(hwnd, ptl, 6, TRUE);
   if (p->flFE & FE_FLOATDLG) {
      ptl[0] = p->ptlBrd;
      pfed->aswp[1].y = ptl[4].y + ptl[0].y;
   } else {
      pfed->aswp[1].y = ptl[3].y;
   } // end if
   // caption listbox
   pfed->aswp[0].fl = SWP_MOVE;
   pfed->aswp[0].x = ptl[0].x + ptl[1].x;
   pfed->aswp[0].cy = ptl[5].y;
   // listbox
   pfed->aswp[1].fl = SWP_SIZE;
   // statusbar
   pfed->aswp[2].fl = SWP_SIZE;
   pfed->aswp[2].cy = ptl[2].y;
}


//==========================================================================\
// parametri:                                                               |
// HWND hwnd: handle del dialogo                                            |
// valore restituito:                                                       |
// MRESULT FALSE                                                            |
//==========================================================================/

VOID MouKeybDlgUpdateStruct(HWND hwnd) {
   PFEDATA pfed = (PFEDATA)WinQueryWindowPtr(hwnd, 0L);
   if (p->flFE & FE_MOUSECLK) { // dialogo mouse
      // roll windows
      p->ocur.roll.mou = pfed->fel[FEIDX_ROLL].fl;
      p->ocur.roll.moukbd = pfed->fel[FEIDX_ROLL].KC;
      p->ocur.roll.moumsg = pfed->fel[FEIDX_ROLL].mouVK;
      // hide titlebar
      p->ocur.tbh.mou = pfed->fel[FEIDX_HTBAR].fl;
      p->ocur.tbh.moukbd = pfed->fel[FEIDX_HTBAR].KC;
      p->ocur.tbh.moumsg = pfed->fel[FEIDX_HTBAR].mouVK;
      // VIO window copy/append/paste
      p->ocur.vio.cpyMoukbd = pfed->fel[FEIDX_VIOWINCOPY].KC;
      p->ocur.vio.cpyMoumsg = pfed->fel[FEIDX_VIOWINCOPY].mouVK;
      p->ocur.vio.cpyaMoukbd = pfed->fel[FEIDX_VIOWINAPND].KC;
      p->ocur.vio.cpyaMoumsg = pfed->fel[FEIDX_VIOWINAPND].mouVK;
      p->ocur.vio.insMoukbd = pfed->fel[FEIDX_VIOWINPASTE].KC;
      p->ocur.vio.insMoumsg = pfed->fel[FEIDX_VIOWINPASTE].mouVK;
      // snap to screen edge
      p->ocur.sizpos.snapMoukbd = pfed->fel[FEIDX_BKSZMOVE].KC;
      p->ocur.sizpos.snapMoumsg = pfed->fel[FEIDX_BKSZMOVE].mouVK;
      // entryfield content selection
      p->ocur.txt.selMoukbd = pfed->fel[FEIDX_EFAUTOSEL].KC;
      p->ocur.txt.selMoumsg = pfed->fel[FEIDX_EFAUTOSEL].mouVK;
      // text controls copy/append/cut/insert/overwrite
      p->ocur.txt.cpyMoukbd = pfed->fel[FEIDX_TXTCTLCOPY].KC;
      p->ocur.txt.cpyMoumsg = pfed->fel[FEIDX_TXTCTLCOPY].mouVK;
      p->ocur.txt.cutMoukbd = pfed->fel[FEIDX_TXTCTLCUT].KC;
      p->ocur.txt.cutMoumsg = pfed->fel[FEIDX_TXTCTLCUT].mouVK;
      p->ocur.txt.insMoukbd = pfed->fel[FEIDX_TXTCTLINS].KC;
      p->ocur.txt.insMoumsg = pfed->fel[FEIDX_TXTCTLINS].mouVK;
      p->ocur.txt.ovwMoukbd = pfed->fel[FEIDX_TXTCTLOVRWRITE].KC;
      p->ocur.txt.ovwMoumsg = pfed->fel[FEIDX_TXTCTLOVRWRITE].mouVK;
      // frame drag
      p->ocur.sizpos.frameMoukbd = pfed->fel[FEIDX_FRAMEDRAG].KC;
      p->ocur.sizpos.frameMoumsg = pfed->fel[FEIDX_FRAMEDRAG].mouVK;
      // sposta in fondo allo Z-order
      p->ocur.sizpos.ZordMoukbd = pfed->fel[FEIDX_PUTBEHIND].KC;
      p->ocur.sizpos.ZordMoumsg = pfed->fel[FEIDX_PUTBEHIND].mouVK;
   } else {        // ---------------- dialogo tastiera -------------------
      // roll windows
      p->ocur.roll.kbd = pfed->fel[FEIDX_ROLL].fl;
      p->ocur.roll.kbdKC = pfed->fel[FEIDX_ROLL].KC;
      p->ocur.roll.kbdVK = pfed->fel[FEIDX_ROLL].mouVK;
      // hide titlebar
      p->ocur.tbh.kbd = pfed->fel[FEIDX_HTBAR].fl;
      p->ocur.tbh.kbdKC = pfed->fel[FEIDX_HTBAR].KC;
      p->ocur.tbh.kbdVK = pfed->fel[FEIDX_HTBAR].mouVK;
      // VIO window copy/append/paste
      p->ocur.vio.cpyKbdKC = pfed->fel[FEIDX_VIOWINCOPY].KC;
      p->ocur.vio.cpyKbdVK = pfed->fel[FEIDX_VIOWINCOPY].mouVK;
      p->ocur.vio.cpyaKbdKC = pfed->fel[FEIDX_VIOWINAPND].KC;
      p->ocur.vio.cpyaKbdVK = pfed->fel[FEIDX_VIOWINAPND].mouVK;
      p->ocur.vio.insKbdKC = pfed->fel[FEIDX_VIOWINPASTE].KC;
      p->ocur.vio.insKbdVK = pfed->fel[FEIDX_VIOWINPASTE].mouVK;
      // accelleratori folder
      p->ocur.wpskbd.sets = pfed->fel[FEIDX_FLDRPRPRTS].fl;
      p->ocur.wpskbd.setsKC = pfed->fel[FEIDX_FLDRPRPRTS].KC;
      p->ocur.wpskbd.setsK = pfed->fel[FEIDX_FLDRPRPRTS].mouVK;
      p->ocur.wpskbd.parent = pfed->fel[FEIDX_FLDROPPAR].fl;
      p->ocur.wpskbd.parentKC = pfed->fel[FEIDX_FLDROPPAR].KC;
      p->ocur.wpskbd.parentK = pfed->fel[FEIDX_FLDROPPAR].mouVK;
      p->ocur.wpskbd.find = pfed->fel[FEIDX_FLDRFIND].fl;
      p->ocur.wpskbd.findKC = pfed->fel[FEIDX_FLDRFIND].KC;
      p->ocur.wpskbd.findK = pfed->fel[FEIDX_FLDRFIND].mouVK;
      p->ocur.wpskbd.srtname = pfed->fel[FEIDX_FLDRSRTNAME].fl;
      p->ocur.wpskbd.srtnameKC = pfed->fel[FEIDX_FLDRSRTNAME].KC;
      p->ocur.wpskbd.srtnameK = pfed->fel[FEIDX_FLDRSRTNAME].mouVK;
      p->ocur.wpskbd.srtext = pfed->fel[FEIDX_FLDRSRTEXT].fl;
      p->ocur.wpskbd.srtextKC = pfed->fel[FEIDX_FLDRSRTEXT].KC;
      p->ocur.wpskbd.srtextK = pfed->fel[FEIDX_FLDRSRTEXT].mouVK;
      p->ocur.wpskbd.srttype = pfed->fel[FEIDX_FLDRSRTTYPE].fl;
      p->ocur.wpskbd.srttypeKC = pfed->fel[FEIDX_FLDRSRTTYPE].KC;
      p->ocur.wpskbd.srttypeK = pfed->fel[FEIDX_FLDRSRTTYPE].mouVK;
      p->ocur.wpskbd.srtrname = pfed->fel[FEIDX_FLDRSRTRNAME].fl;
      p->ocur.wpskbd.srtrnameKC = pfed->fel[FEIDX_FLDRSRTRNAME].KC;
      p->ocur.wpskbd.srtrnameK = pfed->fel[FEIDX_FLDRSRTRNAME].mouVK;
      p->ocur.wpskbd.srtsize = pfed->fel[FEIDX_FLDRSRTSIZE].fl;
      p->ocur.wpskbd.srtsizeKC = pfed->fel[FEIDX_FLDRSRTSIZE].KC;
      p->ocur.wpskbd.srtsizeK = pfed->fel[FEIDX_FLDRSRTSIZE].mouVK;
      p->ocur.wpskbd.srtwrite = pfed->fel[FEIDX_FLDRSRTWRITE].fl;
      p->ocur.wpskbd.srtwriteKC = pfed->fel[FEIDX_FLDRSRTWRITE].KC;
      p->ocur.wpskbd.srtwriteK = pfed->fel[FEIDX_FLDRSRTWRITE].mouVK;
      p->ocur.wpskbd.srtacc = pfed->fel[FEIDX_FLDRSRTACCES].fl;
      p->ocur.wpskbd.srtaccKC = pfed->fel[FEIDX_FLDRSRTACCES].KC;
      p->ocur.wpskbd.srtaccK = pfed->fel[FEIDX_FLDRSRTACCES].mouVK;
      p->ocur.wpskbd.srtcreat = pfed->fel[FEIDX_FLDRSRTCREAT].fl;
      p->ocur.wpskbd.srtcreatKC = pfed->fel[FEIDX_FLDRSRTCREAT].KC;
      p->ocur.wpskbd.srtcreatK = pfed->fel[FEIDX_FLDRSRTCREAT].mouVK;
      p->ocur.wpskbd.arrange = pfed->fel[FEIDX_FLDRARRANGE].fl;
      p->ocur.wpskbd.arrangeKC = pfed->fel[FEIDX_FLDRARRANGE].KC;
      p->ocur.wpskbd.arrangeK = pfed->fel[FEIDX_FLDRARRANGE].mouVK;
      p->ocur.wpskbd.refresh = pfed->fel[FEIDX_FLDRRFRSH].fl;
      p->ocur.wpskbd.refreshKC = pfed->fel[FEIDX_FLDRRFRSH].KC;
      p->ocur.wpskbd.refreshK = pfed->fel[FEIDX_FLDRRFRSH].mouVK;
      p->ocur.wpskbd.on = p->ocur.wpskbd.sets || p->ocur.wpskbd.parent ||
                         p->ocur.wpskbd.find || p->ocur.wpskbd.srtname ||
                         p->ocur.wpskbd.srtext || p->ocur.wpskbd.srttype ||
                         p->ocur.wpskbd.srtrname || p->ocur.wpskbd.srtsize ||
                         p->ocur.wpskbd.srtwrite || p->ocur.wpskbd.srtacc ||
                         p->ocur.wpskbd.srtcreat || p->ocur.wpskbd.arrange ||
                         p->ocur.wpskbd.refresh;
   } // end if
}


//==========================================================================\
// parametri:                                                               |
// valore restituito:                                                       |
//==========================================================================/

VOID MouKeybDlgFillLbox(HWND hwnd, PFEDATA pfed) {
   ULONG ul;
   if (p->flFE & FE_MOUSECLK) { // dialogo mouse
      // roll windows
      pfed->fel[FEIDX_ROLL].idf = ID_ROLL;
      pfed->fel[FEIDX_ROLL].idTrg = ID_PERFTBAR;
      pfed->fel[FEIDX_ROLL].fl = p->ocur.roll.mou &&
                                 p->ocur.roll.on;
      pfed->fel[FEIDX_ROLL].ctl = p->ocur.roll.on;
      pfed->fel[FEIDX_ROLL].click = 1;
      pfed->fel[FEIDX_ROLL].KC = p->ocur.roll.moukbd;
      pfed->fel[FEIDX_ROLL].mouVK = p->ocur.roll.moumsg?
                                    p->ocur.roll.moumsg:
                                    WM_BUTTON1CLICK;
      // hide titlebar
      pfed->fel[FEIDX_HTBAR].idf = ID_HTBAR;
      pfed->fel[FEIDX_HTBAR].idTrg = ID_PERFTBAR;
      pfed->fel[FEIDX_HTBAR].fl = p->ocur.tbh.mou &&
                                  p->ocur.tbh.on;
      pfed->fel[FEIDX_HTBAR].ctl = p->ocur.tbh.on;
      pfed->fel[FEIDX_HTBAR].click = 1;
      pfed->fel[FEIDX_HTBAR].KC = p->ocur.tbh.moukbd;
      pfed->fel[FEIDX_HTBAR].mouVK = p->ocur.tbh.moumsg?
                                     p->ocur.tbh.moumsg:
                                     WM_BUTTON1CLICK;
      // VIO copy
      pfed->fel[FEIDX_VIOWINCOPY].idf = ID_VIOCOPY;
      pfed->fel[FEIDX_VIOWINCOPY].idTrg = ID_PERFVIO;
      pfed->fel[FEIDX_VIOWINCOPY].fl = p->ocur.vio.mou;
      pfed->fel[FEIDX_VIOWINCOPY].KC = p->ocur.vio.cpyMoukbd;
      pfed->fel[FEIDX_VIOWINCOPY].mouVK = p->ocur.vio.cpyMoumsg?
                                          p->ocur.vio.cpyMoumsg:
                                          WM_BUTTON1DOWN;
      // VIO append - trasformato in copy all
      pfed->fel[FEIDX_VIOWINAPND].idf = ID_VIOAPND;
      pfed->fel[FEIDX_VIOWINAPND].idTrg = ID_PERFVIO;
      pfed->fel[FEIDX_VIOWINAPND].fl = p->ocur.vio.mou;
      pfed->fel[FEIDX_VIOWINAPND].click = 1;
      pfed->fel[FEIDX_VIOWINAPND].KC = p->ocur.vio.cpyaMoukbd;
      pfed->fel[FEIDX_VIOWINAPND].mouVK = p->ocur.vio.cpyaMoumsg?
                                          p->ocur.vio.cpyaMoumsg:
                                          WM_BUTTON1DOWN;
      // VIO paste
      pfed->fel[FEIDX_VIOWINPASTE].idf = ID_VIOPASTE;
      pfed->fel[FEIDX_VIOWINPASTE].idTrg = ID_PERFVIO;
      pfed->fel[FEIDX_VIOWINPASTE].fl = p->ocur.vio.mou;
      pfed->fel[FEIDX_VIOWINPASTE].click = 1;
      pfed->fel[FEIDX_VIOWINPASTE].KC = p->ocur.vio.insMoukbd;
      pfed->fel[FEIDX_VIOWINPASTE].mouVK = p->ocur.vio.insMoumsg?
                                           p->ocur.vio.insMoumsg:
                                           WM_BUTTON1CLICK;
      // snap to screen edge
      pfed->fel[FEIDX_BKSZMOVE].idf = ID_BKSZMOVE;
      pfed->fel[FEIDX_BKSZMOVE].idTrg = ID_PERFTBAR;
      pfed->fel[FEIDX_BKSZMOVE].fl = p->ocur.sizpos.snap;
      pfed->fel[FEIDX_BKSZMOVE].click = 1;
      pfed->fel[FEIDX_BKSZMOVE].KC = p->ocur.sizpos.snapMoukbd;
      pfed->fel[FEIDX_BKSZMOVE].mouVK = p->ocur.sizpos.snapMoumsg?
                                        p->ocur.sizpos.snapMoumsg:
                                        WM_BUTTON1DOWN;
      // entry field autoselect
      pfed->fel[FEIDX_EFAUTOSEL].idf = ID_EFSELALLTXT;
      pfed->fel[FEIDX_EFAUTOSEL].idTrg = ID_PERFEF;
      pfed->fel[FEIDX_EFAUTOSEL].fl = p->ocur.txt.efsel;
      pfed->fel[FEIDX_EFAUTOSEL].click = 1;
      pfed->fel[FEIDX_EFAUTOSEL].KC = p->ocur.txt.selMoukbd;
      pfed->fel[FEIDX_EFAUTOSEL].mouVK = p->ocur.txt.selMoumsg?
                                         p->ocur.txt.selMoumsg:
                                         WM_BUTTON1CLICK;
      // text control copy
      pfed->fel[FEIDX_TXTCTLCOPY].idf = ID_TXTCTLSCPY;
      pfed->fel[FEIDX_TXTCTLCOPY].idTrg = ID_PERFTXTCTLS;
      pfed->fel[FEIDX_TXTCTLCOPY].fl = p->ocur.txt.on;
      pfed->fel[FEIDX_TXTCTLCOPY].click = 1;
      pfed->fel[FEIDX_TXTCTLCOPY].KC = p->ocur.txt.cpyMoukbd;
      pfed->fel[FEIDX_TXTCTLCOPY].mouVK = p->ocur.txt.cpyMoumsg?
                                          p->ocur.txt.cpyMoumsg:
                                          WM_BUTTON1CLICK;
      // text control cut
      pfed->fel[FEIDX_TXTCTLCUT].idf = ID_TXTCTLSCUT;
      pfed->fel[FEIDX_TXTCTLCUT].idTrg = ID_PERFTXTCTLS;
      pfed->fel[FEIDX_TXTCTLCUT].fl = p->ocur.txt.on;
      pfed->fel[FEIDX_TXTCTLCUT].click = 1;
      pfed->fel[FEIDX_TXTCTLCUT].KC = p->ocur.txt.cutMoukbd;
      pfed->fel[FEIDX_TXTCTLCUT].mouVK = p->ocur.txt.cutMoumsg?
                                         p->ocur.txt.cutMoumsg:
                                         WM_BUTTON1CLICK;
      // text control insert
      pfed->fel[FEIDX_TXTCTLINS].idf = ID_TXTCTLSPASTE;
      pfed->fel[FEIDX_TXTCTLINS].idTrg = ID_PERFTXTCTLS;
      pfed->fel[FEIDX_TXTCTLINS].fl = p->ocur.txt.on;
      pfed->fel[FEIDX_TXTCTLINS].click = 1;
      pfed->fel[FEIDX_TXTCTLINS].KC = p->ocur.txt.insMoukbd;
      pfed->fel[FEIDX_TXTCTLINS].mouVK = p->ocur.txt.insMoumsg?
                                         p->ocur.txt.insMoumsg:
                                         WM_BUTTON1CLICK;
      // text control overwrite
      pfed->fel[FEIDX_TXTCTLOVRWRITE].idf = ID_TXTCTLSOVRWRT;
      pfed->fel[FEIDX_TXTCTLOVRWRITE].idTrg = ID_PERFTXTCTLS;
      pfed->fel[FEIDX_TXTCTLOVRWRITE].fl = p->ocur.txt.on;
      pfed->fel[FEIDX_TXTCTLOVRWRITE].click = 1;
      pfed->fel[FEIDX_TXTCTLOVRWRITE].KC = p->ocur.txt.ovwMoukbd;
      pfed->fel[FEIDX_TXTCTLOVRWRITE].mouVK = p->ocur.txt.ovwMoumsg?
                                              p->ocur.txt.ovwMoumsg:
                                              WM_BUTTON1CLICK;
      // frame drag
      pfed->fel[FEIDX_FRAMEDRAG].idf = ID_FRAMEDRAG;
      pfed->fel[FEIDX_FRAMEDRAG].idTrg = ID_PERFWINBRD;
      pfed->fel[FEIDX_FRAMEDRAG].fl = p->ocur.sizpos.framedrg;
      pfed->fel[FEIDX_FRAMEDRAG].KC = p->ocur.sizpos.frameMoukbd;
      pfed->fel[FEIDX_FRAMEDRAG].mouVK = p->ocur.sizpos.frameMoumsg?
                                         p->ocur.sizpos.frameMoumsg:
                                         WM_BUTTON1CLICK;
      // sposta in fondo allo Z-order
      pfed->fel[FEIDX_PUTBEHIND].idf = ID_PUTBEHIND;
      pfed->fel[FEIDX_PUTBEHIND].idTrg = ID_PERFTBAR;
      pfed->fel[FEIDX_PUTBEHIND].fl = p->ocur.sizpos.Zmove;
      pfed->fel[FEIDX_PUTBEHIND].click = 1;
      pfed->fel[FEIDX_PUTBEHIND].KC = p->ocur.sizpos.ZordMoukbd;
      pfed->fel[FEIDX_PUTBEHIND].mouVK = p->ocur.sizpos.ZordMoumsg?
                                         p->ocur.sizpos.ZordMoumsg:
                                         WM_BUTTON1CLICK;
   } else {                             // dialogo keyboard
      // roll windows
      pfed->fel[FEIDX_ROLL].idf = ID_ROLL;
      pfed->fel[FEIDX_ROLL].idTrg = ID_PERFFOCWIN;
      pfed->fel[FEIDX_ROLL].fl = p->ocur.roll.kbd &&
                                 p->ocur.roll.on;
      pfed->fel[FEIDX_ROLL].ctl = p->ocur.roll.on;
      pfed->fel[FEIDX_ROLL].KC = p->ocur.roll.kbdKC;
      pfed->fel[FEIDX_ROLL].mouVK = p->ocur.roll.kbdVK?
                                    p->ocur.roll.kbdVK:
                                    VK_F1;
      // hide titlebar
      pfed->fel[FEIDX_HTBAR].idf = ID_HTBAR;
      pfed->fel[FEIDX_HTBAR].idTrg = ID_PERFFOCWIN;
      pfed->fel[FEIDX_HTBAR].fl = p->ocur.tbh.kbd &&
                                  p->ocur.tbh.on;
      pfed->fel[FEIDX_HTBAR].ctl = p->ocur.tbh.on;
      pfed->fel[FEIDX_HTBAR].KC = p->ocur.tbh.kbdKC;
      pfed->fel[FEIDX_HTBAR].mouVK = p->ocur.tbh.kbdVK?
                                     p->ocur.tbh.kbdVK:
                                     VK_F1;
      // VIO copy
      pfed->fel[FEIDX_VIOWINCOPY].idf = ID_VIOCOPY;
      pfed->fel[FEIDX_VIOWINCOPY].idTrg = ID_PERFVIO;
      pfed->fel[FEIDX_VIOWINCOPY].fl = p->ocur.vio.kbd;
      pfed->fel[FEIDX_VIOWINCOPY].KC =p->ocur.vio.cpyKbdKC;
      pfed->fel[FEIDX_VIOWINCOPY].mouVK = p->ocur.vio.cpyKbdVK?
                                          p->ocur.vio.cpyKbdVK:
                                          0x3b20;
      // VIO append
      pfed->fel[FEIDX_VIOWINAPND].idf = ID_VIOAPND;
      pfed->fel[FEIDX_VIOWINAPND].idTrg = ID_PERFVIO;
      pfed->fel[FEIDX_VIOWINAPND].fl = p->ocur.vio.kbd;
      pfed->fel[FEIDX_VIOWINAPND].KC = p->ocur.vio.cpyaKbdKC;
      pfed->fel[FEIDX_VIOWINAPND].mouVK = p->ocur.vio.cpyaKbdVK?
                                          p->ocur.vio.cpyaKbdVK:
                                          0x3b20;
      // VIO paste
      pfed->fel[FEIDX_VIOWINPASTE].idf = ID_VIOPASTE;
      pfed->fel[FEIDX_VIOWINPASTE].idTrg = ID_PERFVIO;
      pfed->fel[FEIDX_VIOWINPASTE].fl = p->ocur.vio.kbd;
      pfed->fel[FEIDX_VIOWINPASTE].KC = p->ocur.vio.insKbdKC;
      pfed->fel[FEIDX_VIOWINPASTE].mouVK = p->ocur.vio.insKbdVK?
                                           p->ocur.vio.insKbdVK:
                                           0x3b20;
      // acceleratore folder open properties
      pfed->fel[FEIDX_FLDRPRPRTS].idf = ID_FLDOPPRPRT;
      pfed->fel[FEIDX_FLDRPRPRTS].idTrg = ID_PERFONFLDR;
      pfed->fel[FEIDX_FLDRPRPRTS].fl = p->ocur.wpskbd.sets;
      pfed->fel[FEIDX_FLDRPRPRTS].ctl = 1;
      pfed->fel[FEIDX_FLDRPRPRTS].KC = p->ocur.wpskbd.setsKC;
      pfed->fel[FEIDX_FLDRPRPRTS].mouVK = p->ocur.wpskbd.setsK?
                                          p->ocur.wpskbd.setsK:
                                          VK_F1;
      // acceleratore folder open parent
      pfed->fel[FEIDX_FLDROPPAR].idf = ID_FLDOPPAR;
      pfed->fel[FEIDX_FLDROPPAR].idTrg = ID_PERFONFLDR;
      pfed->fel[FEIDX_FLDROPPAR].fl = p->ocur.wpskbd.parent;
      pfed->fel[FEIDX_FLDROPPAR].ctl = 1;
      pfed->fel[FEIDX_FLDROPPAR].KC = p->ocur.wpskbd.parentKC;
      pfed->fel[FEIDX_FLDROPPAR].mouVK = p->ocur.wpskbd.parentK?
                                         p->ocur.wpskbd.parentK:
                                         VK_F1;
      // acceleratore folder find
      pfed->fel[FEIDX_FLDRFIND].idf = ID_FLDFIND;
      pfed->fel[FEIDX_FLDRFIND].idTrg = ID_PERFONFLDR;
      pfed->fel[FEIDX_FLDRFIND].fl = p->ocur.wpskbd.find;
      pfed->fel[FEIDX_FLDRFIND].ctl = 1;
      pfed->fel[FEIDX_FLDRFIND].KC = p->ocur.wpskbd.findKC;
      pfed->fel[FEIDX_FLDRFIND].mouVK = p->ocur.wpskbd.findK?
                                        p->ocur.wpskbd.findK:
                                        VK_F1;
      // acceleratore folder sort by name
      pfed->fel[FEIDX_FLDRSRTNAME].idf = ID_FLDSRTNAME;
      pfed->fel[FEIDX_FLDRSRTNAME].idTrg = ID_PERFONFLDR;
      pfed->fel[FEIDX_FLDRSRTNAME].fl = p->ocur.wpskbd.srtname;
      pfed->fel[FEIDX_FLDRSRTNAME].ctl = 1;
      pfed->fel[FEIDX_FLDRSRTNAME].KC = p->ocur.wpskbd.srtnameKC;
      pfed->fel[FEIDX_FLDRSRTNAME].mouVK = p->ocur.wpskbd.srtnameK?
                                           p->ocur.wpskbd.srtnameK:
                                           VK_F1;
      // acceleratore folder sort by extension
      pfed->fel[FEIDX_FLDRSRTEXT].idf = ID_FLDSRTEXT;
      pfed->fel[FEIDX_FLDRSRTEXT].idTrg = ID_PERFONFLDR;
      pfed->fel[FEIDX_FLDRSRTEXT].fl = p->ocur.wpskbd.srtext;
      pfed->fel[FEIDX_FLDRSRTEXT].ctl = 1;
      pfed->fel[FEIDX_FLDRSRTEXT].KC = p->ocur.wpskbd.srtextKC;
      pfed->fel[FEIDX_FLDRSRTEXT].mouVK = p->ocur.wpskbd.srtextK?
                                          p->ocur.wpskbd.srtextK:
                                          VK_F1;
      // acceleratore folder sort by type
      pfed->fel[FEIDX_FLDRSRTTYPE].idf = ID_FLDSRTTYPE;
      pfed->fel[FEIDX_FLDRSRTTYPE].idTrg = ID_PERFONFLDR;
      pfed->fel[FEIDX_FLDRSRTTYPE].fl = p->ocur.wpskbd.srttype;
      pfed->fel[FEIDX_FLDRSRTTYPE].ctl = 1;
      pfed->fel[FEIDX_FLDRSRTTYPE].KC = p->ocur.wpskbd.srttypeKC;
      pfed->fel[FEIDX_FLDRSRTTYPE].mouVK = p->ocur.wpskbd.srttypeK?
                                           p->ocur.wpskbd.srttypeK:
                                           VK_F1;
      // acceleratore folder sort by real name
      pfed->fel[FEIDX_FLDRSRTRNAME].idf = ID_FLDRSRTRNAME;
      pfed->fel[FEIDX_FLDRSRTRNAME].idTrg = ID_PERFONFLDR;
      pfed->fel[FEIDX_FLDRSRTRNAME].fl = p->ocur.wpskbd.srtrname;
      pfed->fel[FEIDX_FLDRSRTRNAME].ctl = 1;
      pfed->fel[FEIDX_FLDRSRTRNAME].KC = p->ocur.wpskbd.srtrnameKC;
      pfed->fel[FEIDX_FLDRSRTRNAME].mouVK = p->ocur.wpskbd.srtrnameK?
                                            p->ocur.wpskbd.srtrnameK:
                                            VK_F1;
      // acceleratore folder sort by size
      pfed->fel[FEIDX_FLDRSRTSIZE].idf = ID_FLDRSRTSIZE;
      pfed->fel[FEIDX_FLDRSRTSIZE].idTrg = ID_PERFONFLDR;
      pfed->fel[FEIDX_FLDRSRTSIZE].fl = p->ocur.wpskbd.srtsize;
      pfed->fel[FEIDX_FLDRSRTSIZE].ctl = 1;
      pfed->fel[FEIDX_FLDRSRTSIZE].KC = p->ocur.wpskbd.srtsizeKC;
      pfed->fel[FEIDX_FLDRSRTSIZE].mouVK = p->ocur.wpskbd.srtsizeK?
                                           p->ocur.wpskbd.srtsizeK:
                                           VK_F1;
      // acceleratore folder sort by last write date
      pfed->fel[FEIDX_FLDRSRTWRITE].idf = ID_FLDSRTWRT;
      pfed->fel[FEIDX_FLDRSRTWRITE].idTrg = ID_PERFONFLDR;
      pfed->fel[FEIDX_FLDRSRTWRITE].fl = p->ocur.wpskbd.srtwrite;
      pfed->fel[FEIDX_FLDRSRTWRITE].ctl = 1;
      pfed->fel[FEIDX_FLDRSRTWRITE].KC = p->ocur.wpskbd.srtwriteKC;
      pfed->fel[FEIDX_FLDRSRTWRITE].mouVK = p->ocur.wpskbd.srtwriteK?
                                            p->ocur.wpskbd.srtwriteK:
                                            VK_F1;
      // acceleratore folder sort by last access date
      pfed->fel[FEIDX_FLDRSRTACCES].idf = ID_FLDSRTACC;
      pfed->fel[FEIDX_FLDRSRTACCES].idTrg = ID_PERFONFLDR;
      pfed->fel[FEIDX_FLDRSRTACCES].fl = p->ocur.wpskbd.srtacc;
      pfed->fel[FEIDX_FLDRSRTACCES].ctl = 1;
      pfed->fel[FEIDX_FLDRSRTACCES].KC = p->ocur.wpskbd.srtcreatKC;
      pfed->fel[FEIDX_FLDRSRTACCES].mouVK = p->ocur.wpskbd.srtcreatK?
                                            p->ocur.wpskbd.srtcreatK:
                                            VK_F1;
      // acceleratore folder srot by last creation date
      pfed->fel[FEIDX_FLDRSRTCREAT].idf = ID_FLDSRTCREAT;
      pfed->fel[FEIDX_FLDRSRTCREAT].idTrg = ID_PERFONFLDR;
      pfed->fel[FEIDX_FLDRSRTCREAT].fl = p->ocur.wpskbd.srtcreat;
      pfed->fel[FEIDX_FLDRSRTCREAT].ctl = 1;
      pfed->fel[FEIDX_FLDRSRTCREAT].KC = p->ocur.wpskbd.srtcreatKC;
      pfed->fel[FEIDX_FLDRSRTCREAT].mouVK = p->ocur.wpskbd.srtcreatK?
                                            p->ocur.wpskbd.srtcreatK:
                                            VK_F1;
      // acceleratore folder arrange
      pfed->fel[FEIDX_FLDRARRANGE].idf = ID_FLDARRANGE;
      pfed->fel[FEIDX_FLDRARRANGE].idTrg = ID_PERFONFLDR;
      pfed->fel[FEIDX_FLDRARRANGE].fl = p->ocur.wpskbd.arrange;
      pfed->fel[FEIDX_FLDRARRANGE].ctl = 1;
      pfed->fel[FEIDX_FLDRARRANGE].KC = p->ocur.wpskbd.arrangeKC;
      pfed->fel[FEIDX_FLDRARRANGE].mouVK = p->ocur.wpskbd.arrangeK?
                                           p->ocur.wpskbd.arrangeK:
                                           VK_F1;
      // acceleratore folder refresh
      pfed->fel[FEIDX_FLDRRFRSH].idf = ID_FLDREFRESH;
      pfed->fel[FEIDX_FLDRRFRSH].idTrg = ID_PERFONFLDR;
      pfed->fel[FEIDX_FLDRRFRSH].fl = p->ocur.wpskbd.refresh;
      pfed->fel[FEIDX_FLDRRFRSH].ctl = 1;
      pfed->fel[FEIDX_FLDRRFRSH].KC = p->ocur.wpskbd.refreshKC;
      pfed->fel[FEIDX_FLDRRFRSH].mouVK = p->ocur.wpskbd.refreshK?
                                         p->ocur.wpskbd.refreshK:
                                         VK_F1;
//      // acceleratore folder
//      pfed->fel[].idf = ;
//      pfed->fel[].idTrg = ;
//      pfed->fel[].fl = p->ocur.;
//      pfed->fel[].ctl = 0;
//      pfed->fel[].click = 0;
//      pfed->fel[].KC = p->ocur.;
//      pfed->fel[].mouVK = p->ocur.? p->ocur.?
//                                       p->ocur.:
//                                           VK_F1;
      pfed->fel[FEIDX_KEYLAST].idf = 0;
   } /* endif */
   for (ul = 0; pfed->fel[ul].idf; ul++) {
      GetStr(pfed->fel[ul].idf, p->buf, 256);
      pfed->fel[ul].idx = (SHORT)WinSendMsg(pfed->ahwnd[1], LM_INSERTITEM,
                                            (MPARAM)LIT_SORTASCENDING,
                                            (MPARAM)p->buf);
      WinSendMsg(pfed->ahwnd[1], LM_SETITEMHANDLE,
                 (MPARAM)pfed->fel[ul].idx, (MPARAM)ul);
   } // end for
   // reinizializza ai corretti valori l'elemento idx delle strutture FELIST
   ul = (LONG)WinSendMsg(pfed->ahwnd[1], LM_QUERYITEMCOUNT, MPVOID, MPVOID);
   while (ul--) {
      pfed->fel[(ULONG)WinSendMsg(pfed->ahwnd[1], LM_QUERYITEMHANDLE,
                                  (MPARAM)ul, MPVOID)].idx = ul;
   } /* endwhile */
}


//==========================================================================
// parametri:
// HWND hwnd: handle del dialogo
// BOOL flag: TRUE = doppio click o OK, FALSE = cancel o chiusura
// valore restituito:
// MRESULT FALSE
//==========================================================================

VOID MouKeybDlgEnableCtls(HWND hwnd, BOOL fl) {
   WinEnableControl(hwnd, TXT_FEKEYBTN, fl);    // spin txt
   WinEnableControl(hwnd, SPIN_FEFKEYBTN, fl);  // spin
   WinEnableControl(hwnd, RDBTN_FECLICK, fl);   // radiobtns
   WinEnableControl(hwnd, RDBTN_FEDBLCLK, fl);
   WinEnableControl(hwnd, CHK_FECTRL, fl);      // Ctrl
   WinEnableControl(hwnd, CHK_FEALT, fl);       // Alt
   WinEnableControl(hwnd, CHK_FESHIFT, fl);     // Shift
}


//==========================================================================
// aggiorna la lista delle azioni eseguibili tramite mouse/tastiera ed
// eventi associati
// parametri:
// HWND hwnd: handle dialogo
// valore restituito:
// VOID
//==========================================================================

VOID MouKbdUpdateFElist(HWND hwnd) {
   if (hwnd) {
      PFEDATA pfed = (PFEDATA)WinQueryWindowPtr(hwnd, 0L);
      DlgLboxDeleteAll(hwnd, LBOX_FEFNCTEVNTS);
      MouKeybDlgFillLbox(hwnd, pfed);
      DlgLboxSelectItem(hwnd, LBOX_FEFNCTEVNTS, pfed->iBox);
   } // end if
}


// applica nuovi settaggi

VOID MouKeybDlgApply(HWND hwnd) {
   PFEDATA pfed = (PFEDATA)WinQueryWindowPtr(hwnd, 0L);
   p->pd[p->idxPage].apply = 0;
   WinEnableWindow(p->swpCtls[3].hwnd, FALSE);
   if (p->flFE & FE_MOUSECLK) { // dialogo mouse
      // roll windows
      p->po->roll.mou = p->ocur.roll.mou = pfed->fel[FEIDX_ROLL].fl;
      p->po->roll.moukbd = p->ocur.roll.moukbd = pfed->fel[FEIDX_ROLL].KC;
      p->po->roll.moumsg = p->ocur.roll.moumsg = pfed->fel[FEIDX_ROLL].mouVK;
      // hide titlebar
      p->po->tbh.mou = p->ocur.tbh.mou = pfed->fel[FEIDX_HTBAR].fl;
      p->po->tbh.moukbd = p->ocur.tbh.moukbd = pfed->fel[FEIDX_HTBAR].KC;
      p->po->tbh.moumsg = p->ocur.tbh.moumsg = pfed->fel[FEIDX_HTBAR].mouVK;
      // VIO window copy/append/paste
      p->po->vio.cpyMoukbd = p->ocur.vio.cpyMoukbd =
                                         pfed->fel[FEIDX_VIOWINCOPY].KC;
      p->po->vio.cpyMoumsg = p->ocur.vio.cpyMoumsg =
                                         pfed->fel[FEIDX_VIOWINCOPY].mouVK;
      p->po->vio.cpyaMoukbd = p->ocur.vio.cpyaMoukbd =
                                         pfed->fel[FEIDX_VIOWINAPND].KC;
      p->po->vio.cpyaMoumsg = p->ocur.vio.cpyaMoumsg =
                                         pfed->fel[FEIDX_VIOWINAPND].mouVK;
      p->po->vio.insMoukbd = p->ocur.vio.insMoukbd =
                                         pfed->fel[FEIDX_VIOWINPASTE].KC;
      p->po->vio.insMoumsg = p->ocur.vio.insMoumsg =
                                         pfed->fel[FEIDX_VIOWINPASTE].mouVK;
      // snap to screen edge
      p->po->sizpos.snapMoukbd = p->ocur.sizpos.snapMoukbd =
                                         pfed->fel[FEIDX_BKSZMOVE].KC;
      p->po->sizpos.snapMoumsg = p->ocur.sizpos.snapMoumsg =
                                         pfed->fel[FEIDX_BKSZMOVE].mouVK;
      // entryfield content selection
      p->po->txt.selMoukbd = p->ocur.txt.selMoukbd =
                                         pfed->fel[FEIDX_EFAUTOSEL].KC;
      p->po->txt.selMoumsg = p->ocur.txt.selMoumsg =
                                         pfed->fel[FEIDX_EFAUTOSEL].mouVK;
      // text controls copy/append/cut/insert/overwrite
      p->po->txt.cpyMoukbd = p->ocur.txt.cpyMoukbd =
                                         pfed->fel[FEIDX_TXTCTLCOPY].KC;
      p->po->txt.cpyMoumsg = p->ocur.txt.cpyMoumsg =
                                         pfed->fel[FEIDX_TXTCTLCOPY].mouVK;
      p->po->txt.cutMoukbd = p->ocur.txt.cutMoukbd =
                                         pfed->fel[FEIDX_TXTCTLCUT].KC;
      p->po->txt.cutMoumsg = p->ocur.txt.cutMoumsg =
                                         pfed->fel[FEIDX_TXTCTLCUT].mouVK;
      p->po->txt.insMoukbd = p->ocur.txt.insMoukbd =
                                         pfed->fel[FEIDX_TXTCTLINS].KC;
      p->po->txt.insMoumsg = p->ocur.txt.insMoumsg =
                                         pfed->fel[FEIDX_TXTCTLINS].mouVK;
      p->po->txt.ovwMoukbd = p->ocur.txt.ovwMoukbd =
                                         pfed->fel[FEIDX_TXTCTLOVRWRITE].KC;
      p->po->txt.ovwMoumsg = p->ocur.txt.ovwMoumsg =
                                         pfed->fel[FEIDX_TXTCTLOVRWRITE].mouVK;
      // frame drag
      p->po->sizpos.frameMoukbd = p->ocur.sizpos.frameMoukbd =
                                         pfed->fel[FEIDX_FRAMEDRAG].KC;
      p->po->sizpos.frameMoumsg = p->ocur.sizpos.frameMoumsg =
                                         pfed->fel[FEIDX_FRAMEDRAG].mouVK;
      // sposta in fondo allo Z-order
      p->po->sizpos.ZordMoukbd = p->ocur.sizpos.ZordMoukbd =
                                         pfed->fel[FEIDX_PUTBEHIND].KC;
      p->po->sizpos.ZordMoumsg = p->ocur.sizpos.ZordMoumsg =
                                         pfed->fel[FEIDX_PUTBEHIND].mouVK;
   } else {        // ---------------- dialogo tastiera -------------------
      // roll windows
      p->po->roll.kbd = p->ocur.roll.kbd = pfed->fel[FEIDX_ROLL].fl;
      p->po->roll.kbdKC = p->ocur.roll.kbdKC = pfed->fel[FEIDX_ROLL].KC;
      p->po->roll.kbdVK = p->ocur.roll.kbdVK = pfed->fel[FEIDX_ROLL].mouVK;
      // hide titlebar
      p->po->tbh.kbd = p->ocur.tbh.kbd = pfed->fel[FEIDX_HTBAR].fl;
      p->po->tbh.kbdKC = p->ocur.tbh.kbdKC = pfed->fel[FEIDX_HTBAR].KC;
      p->po->tbh.kbdVK = p->ocur.tbh.kbdVK = pfed->fel[FEIDX_HTBAR].mouVK;
      // VIO window copy/append/paste
      p->po->vio.cpyKbdKC = p->ocur.vio.cpyKbdKC = pfed->fel[FEIDX_VIOWINCOPY].KC;
      p->po->vio.cpyKbdVK = p->ocur.vio.cpyKbdVK = pfed->fel[FEIDX_VIOWINCOPY].mouVK;
      p->po->vio.cpyaKbdKC = p->ocur.vio.cpyaKbdKC = pfed->fel[FEIDX_VIOWINAPND].KC;
      p->po->vio.cpyaKbdVK = p->ocur.vio.cpyaKbdVK = pfed->fel[FEIDX_VIOWINAPND].mouVK;
      p->po->vio.insKbdKC = p->ocur.vio.insKbdKC = pfed->fel[FEIDX_VIOWINPASTE].KC;
      p->po->vio.insKbdVK = p->ocur.vio.insKbdVK = pfed->fel[FEIDX_VIOWINPASTE].mouVK;
      // folder
      p->po->wpskbd.sets = p->ocur.wpskbd.sets = pfed->fel[FEIDX_FLDRPRPRTS].fl;
      p->po->wpskbd.setsKC = p->ocur.wpskbd.setsKC = pfed->fel[FEIDX_FLDRPRPRTS].KC;
      p->po->wpskbd.setsK = p->ocur.wpskbd.setsK = pfed->fel[FEIDX_FLDRPRPRTS].mouVK;
      p->po->wpskbd.parent = p->ocur.wpskbd.parent = pfed->fel[FEIDX_FLDROPPAR].fl;
      p->po->wpskbd.parentKC = p->ocur.wpskbd.parentKC = pfed->fel[FEIDX_FLDROPPAR].KC;
      p->po->wpskbd.parentK = p->ocur.wpskbd.parentK = pfed->fel[FEIDX_FLDROPPAR].mouVK;
      p->po->wpskbd.find = p->ocur.wpskbd.find = pfed->fel[FEIDX_FLDRFIND].fl;
      p->po->wpskbd.findKC = p->ocur.wpskbd.findKC = pfed->fel[FEIDX_FLDRFIND].KC;
      p->po->wpskbd.findK = p->ocur.wpskbd.findK = pfed->fel[FEIDX_FLDRFIND].mouVK;
      p->po->wpskbd.srtname = p->ocur.wpskbd.srtname = pfed->fel[FEIDX_FLDRSRTNAME].fl;
      p->po->wpskbd.srtnameKC = p->ocur.wpskbd.srtnameKC = pfed->fel[FEIDX_FLDRSRTNAME].KC;
      p->po->wpskbd.srtnameK = p->ocur.wpskbd.srtnameK = pfed->fel[FEIDX_FLDRSRTNAME].mouVK;
      p->po->wpskbd.srtext = p->ocur.wpskbd.srtext = pfed->fel[FEIDX_FLDRSRTEXT].fl;
      p->po->wpskbd.srtextKC = p->ocur.wpskbd.srtextKC = pfed->fel[FEIDX_FLDRSRTEXT].KC;
      p->po->wpskbd.srtextK = p->ocur.wpskbd.srtextK = pfed->fel[FEIDX_FLDRSRTEXT].mouVK;
      p->po->wpskbd.srttype = p->ocur.wpskbd.srttype = pfed->fel[FEIDX_FLDRSRTTYPE].fl;
      p->po->wpskbd.srttypeKC = p->ocur.wpskbd.srttypeKC = pfed->fel[FEIDX_FLDRSRTTYPE].KC;
      p->po->wpskbd.srttypeK = p->ocur.wpskbd.srttypeK = pfed->fel[FEIDX_FLDRSRTTYPE].mouVK;
      p->po->wpskbd.srtrname = p->ocur.wpskbd.srtrname = pfed->fel[FEIDX_FLDRSRTRNAME].fl;
      p->po->wpskbd.srtrnameKC = p->ocur.wpskbd.srtrnameKC = pfed->fel[FEIDX_FLDRSRTRNAME].KC;
      p->po->wpskbd.srtrnameK = p->ocur.wpskbd.srtrnameK = pfed->fel[FEIDX_FLDRSRTRNAME].mouVK;
      p->po->wpskbd.srtsize = p->ocur.wpskbd.srtsize = pfed->fel[FEIDX_FLDRSRTSIZE].fl;
      p->po->wpskbd.srtsizeKC = p->ocur.wpskbd.srtsizeKC = pfed->fel[FEIDX_FLDRSRTSIZE].KC;
      p->po->wpskbd.srtsizeK = p->ocur.wpskbd.srtsizeK = pfed->fel[FEIDX_FLDRSRTSIZE].mouVK;
      p->po->wpskbd.srtwrite = p->ocur.wpskbd.srtwrite = pfed->fel[FEIDX_FLDRSRTWRITE].fl;
      p->po->wpskbd.srtwriteKC = p->ocur.wpskbd.srtwriteKC = pfed->fel[FEIDX_FLDRSRTWRITE].KC;
      p->po->wpskbd.srtwriteK = p->ocur.wpskbd.srtwriteK = pfed->fel[FEIDX_FLDRSRTWRITE].mouVK;
      p->po->wpskbd.srtacc = p->ocur.wpskbd.srtacc = pfed->fel[FEIDX_FLDRSRTACCES].fl;
      p->po->wpskbd.srtaccKC = p->ocur.wpskbd.srtaccKC = pfed->fel[FEIDX_FLDRSRTACCES].KC;
      p->po->wpskbd.srtaccK = p->ocur.wpskbd.srtaccK = pfed->fel[FEIDX_FLDRSRTACCES].mouVK;
      p->po->wpskbd.srtcreat = p->ocur.wpskbd.srtcreat = pfed->fel[FEIDX_FLDRSRTCREAT].fl;
      p->po->wpskbd.srtcreatKC = p->ocur.wpskbd.srtcreatKC = pfed->fel[FEIDX_FLDRSRTCREAT].KC;
      p->po->wpskbd.srtcreatK = p->ocur.wpskbd.srtcreatK = pfed->fel[FEIDX_FLDRSRTCREAT].mouVK;
      p->po->wpskbd.arrange = p->ocur.wpskbd.arrange = pfed->fel[FEIDX_FLDRARRANGE].fl;
      p->po->wpskbd.arrangeKC = p->ocur.wpskbd.arrangeKC = pfed->fel[FEIDX_FLDRARRANGE].KC;
      p->po->wpskbd.arrangeK = p->ocur.wpskbd.arrangeK = pfed->fel[FEIDX_FLDRARRANGE].mouVK;
      p->po->wpskbd.refresh = p->ocur.wpskbd.refresh = pfed->fel[FEIDX_FLDRRFRSH].fl;
      p->po->wpskbd.refreshKC = p->ocur.wpskbd.refreshKC = pfed->fel[FEIDX_FLDRRFRSH].KC;
      p->po->wpskbd.refreshK = p->ocur.wpskbd.refreshK = pfed->fel[FEIDX_FLDRRFRSH].mouVK;
      p->ocur.wpskbd.on = p->ocur.wpskbd.sets || p->ocur.wpskbd.parent ||
                        p->ocur.wpskbd.find || p->ocur.wpskbd.srtname ||
                        p->ocur.wpskbd.srtext || p->ocur.wpskbd.srttype ||
                        p->ocur.wpskbd.srtrname || p->ocur.wpskbd.srtsize ||
                        p->ocur.wpskbd.srtwrite || p->ocur.wpskbd.srtacc ||
                        p->ocur.wpskbd.srtcreat || p->ocur.wpskbd.arrange ||
                        p->ocur.wpskbd.refresh;
      // trasmette msg per subclassing/unsubclassing folder
      if (p->ocur.wpskbd.on != p->po->wpskbd.on) {
         p->po->wpskbd.on = p->ocur.wpskbd.on;
         _stlrMsgPostAll(STLR_SUBCLASSFLDR, 0);
      } /* endif */
   } // end if
}


// UNDO:

VOID MouKeybDlgUndo(HWND hwnd) {
   PFEDATA pfed = (PFEDATA)WinQueryWindowPtr(hwnd, 0L);
   EnableToolbarApplyBtn(1);      // abilita apply
   p->pd[p->idxPage].undo = 0;     // disabilita undo
   WinEnableWindow(p->swpCtls[4].hwnd, FALSE);
   if (p->flFE & FE_MOUSECLK) { // dialogo mouse
      // roll windows
      p->ocur.roll.mou = p->ou.roll.mou;
      p->ocur.roll.moukbd = p->ou.roll.moukbd;
      p->ocur.roll.moumsg = p->ou.roll.moumsg;
      // hide titlebar
      p->ocur.tbh.mou = p->ou.tbh.mou;
      p->ocur.tbh.moukbd = p->ou.tbh.moukbd;
      p->ocur.tbh.moumsg = p->ou.tbh.moumsg;
      // VIO window copy/append/paste
      p->ocur.vio.cpyMoukbd = p->ou.vio.cpyMoukbd;
      p->ocur.vio.cpyMoumsg = p->ou.vio.cpyMoumsg;
      p->ocur.vio.cpyaMoukbd = p->ou.vio.cpyaMoukbd;
      p->ocur.vio.cpyaMoumsg = p->ou.vio.cpyaMoumsg;
      p->ocur.vio.insMoukbd = p->ou.vio.insMoukbd;
      p->ocur.vio.insMoumsg = p->ou.vio.insMoumsg;
      // background size/move
      p->ocur.sizpos.snapMoukbd = p->ou.sizpos.snapMoukbd;
      p->ocur.sizpos.snapMoumsg = p->ou.sizpos.snapMoumsg;
      // entryfield content selection
      p->ocur.txt.selMoukbd = p->ou.txt.selMoukbd;
      p->ocur.txt.selMoumsg = p->ou.txt.selMoumsg;
      // text controls copy/append/cut/insert/overwrite
      p->ocur.txt.cpyMoukbd = p->ou.txt.cpyMoukbd;
      p->ocur.txt.cpyMoumsg = p->ou.txt.cpyMoumsg;
      p->ocur.txt.cutMoukbd = p->ou.txt.cutMoukbd;
      p->ocur.txt.cutMoumsg = p->ou.txt.cutMoumsg;
      p->ocur.txt.insMoukbd = p->ou.txt.insMoukbd;
      p->ocur.txt.insMoumsg = p->ou.txt.insMoumsg;
      p->ocur.txt.ovwMoukbd = p->ou.txt.ovwMoukbd;
      p->ocur.txt.ovwMoumsg = p->ou.txt.ovwMoumsg;
      // frame drag
      p->ocur.sizpos.frameMoukbd = p->ou.sizpos.frameMoukbd;
      p->ocur.sizpos.frameMoumsg = p->ou.sizpos.frameMoumsg;
      // sposta in fondo allo Z-order
      p->ocur.sizpos.ZordMoukbd = p->ou.sizpos.ZordMoukbd;
      p->ocur.sizpos.ZordMoumsg = p->ou.sizpos.ZordMoumsg;
   } else {        // ---------------- dialogo tastiera -------------------
      // roll windows
      p->ocur.roll.kbd = p->ou.roll.kbd;
      p->ocur.roll.kbdKC = p->ou.roll.kbdKC;
      p->ocur.roll.kbdVK = p->ou.roll.kbdVK;
      // hide titlebar
      p->ocur.tbh.kbd = p->ou.tbh.kbd;
      p->ocur.tbh.kbdKC = p->ou.tbh.kbdKC;
      p->ocur.tbh.kbdVK = p->ou.tbh.kbdVK;
      // VIO window copy/append/paste
      p->ocur.vio.cpyKbdKC = p->ou.vio.cpyKbdKC;
      p->ocur.vio.cpyKbdVK = p->ou.vio.cpyKbdVK;
      p->ocur.vio.cpyaKbdKC = p->ou.vio.cpyaKbdKC;
      p->ocur.vio.cpyaKbdVK = p->ou.vio.cpyaKbdVK;
      p->ocur.vio.insKbdKC = p->ou.vio.insKbdKC;
      p->ocur.vio.insKbdVK = p->ou.vio.insKbdVK;
      // accelleratori folder
      p->ocur.wpskbd.sets = p->ou.wpskbd.sets;
      p->ocur.wpskbd.setsKC = p->ou.wpskbd.setsKC;
      p->ocur.wpskbd.setsK = p->ou.wpskbd.setsK;
      p->ocur.wpskbd.parent = p->ou.wpskbd.parent;
      p->ocur.wpskbd.parentKC = p->ou.wpskbd.parentKC;
      p->ocur.wpskbd.parentK = p->ou.wpskbd.parentK;
      p->ocur.wpskbd.find = p->ou.wpskbd.find;
      p->ocur.wpskbd.findKC = p->ou.wpskbd.findKC;
      p->ocur.wpskbd.findK = p->ou.wpskbd.findK;
      p->ocur.wpskbd.srtname = p->ou.wpskbd.srtname;
      p->ocur.wpskbd.srtnameKC = p->ou.wpskbd.srtnameKC;
      p->ocur.wpskbd.srtnameK = p->ou.wpskbd.srtnameK;
      p->ocur.wpskbd.srtext = p->ou.wpskbd.srtext;
      p->ocur.wpskbd.srtextKC = p->ou.wpskbd.srtextKC;
      p->ocur.wpskbd.srtextK = p->ou.wpskbd.srtextK;
      p->ocur.wpskbd.srttype = p->ou.wpskbd.srttype;
      p->ocur.wpskbd.srttypeKC = p->ou.wpskbd.srttypeKC;
      p->ocur.wpskbd.srttypeK = p->ou.wpskbd.srttypeK;
      p->ocur.wpskbd.srtrname = p->ou.wpskbd.srtrname;
      p->ocur.wpskbd.srtrnameKC = p->ou.wpskbd.srtrnameKC;
      p->ocur.wpskbd.srtrnameK = p->ou.wpskbd.srtrnameK;
      p->ocur.wpskbd.srtsize = p->ou.wpskbd.srtsize;
      p->ocur.wpskbd.srtsizeKC = p->ou.wpskbd.srtsizeKC;
      p->ocur.wpskbd.srtsizeK = p->ou.wpskbd.srtsizeK;
      p->ocur.wpskbd.srtwrite = p->ou.wpskbd.srtwrite;
      p->ocur.wpskbd.srtwriteKC = p->ou.wpskbd.srtwriteKC;
      p->ocur.wpskbd.srtwriteK = p->ou.wpskbd.srtwriteK;
      p->ocur.wpskbd.srtacc = p->ou.wpskbd.srtacc;
      p->ocur.wpskbd.srtaccKC = p->ou.wpskbd.srtaccKC;
      p->ocur.wpskbd.srtaccK = p->ou.wpskbd.srtaccK;
      p->ocur.wpskbd.srtcreat = p->ou.wpskbd.srtcreat;
      p->ocur.wpskbd.srtcreatKC = p->ou.wpskbd.srtcreatKC;
      p->ocur.wpskbd.srtcreatK = p->ou.wpskbd.srtcreatK;
      p->ocur.wpskbd.arrange = p->ou.wpskbd.arrange;
      p->ocur.wpskbd.arrangeKC = p->ou.wpskbd.arrangeKC;
      p->ocur.wpskbd.arrangeK = p->ou.wpskbd.arrangeK;
      p->ocur.wpskbd.refresh = p->ou.wpskbd.refresh;
      p->ocur.wpskbd.refreshKC = p->ou.wpskbd.refreshKC;
      p->ocur.wpskbd.refreshK = p->ou.wpskbd.refreshK;
      p->ocur.wpskbd.on = p->ou.wpskbd.on;
   } // end if
   MouKbdUpdateFElist(hwnd);
}


// DEFAULT:

VOID MouKeybDlgDefault(HWND hwnd) {
   EnableToolbarApplyBtn(1);      // abilita apply
   EnableToolbarUndoBtn(1);       // abilita undo
   if (p->flFE & FE_MOUSECLK) { // dialogo mouse
      // roll windows
      p->ocur.roll.mou = 1;
      p->ocur.roll.moukbd = 0;
      p->ocur.roll.moumsg = WM_BUTTON2DBLCLK;
      // hide titlebar
      p->ocur.tbh.mou = 1;
      p->ocur.tbh.moukbd = 0;
      p->ocur.tbh.moumsg = WM_BUTTON1DBLCLK;
      // VIO window copy/append/paste
      p->ocur.vio.cpyMoukbd = KC_CTRL;
      p->ocur.vio.cpyMoumsg = WM_BUTTON1DOWN;
      p->ocur.vio.cpyaMoukbd = KC_CTRL | KC_SHIFT;
      p->ocur.vio.cpyaMoumsg = WM_BUTTON1CLICK;
      p->ocur.vio.insMoukbd = KC_SHIFT;
      p->ocur.vio.insMoumsg = WM_BUTTON1CLICK;
      // snap to screen edge
      p->ocur.sizpos.snapMoukbd = KC_SHIFT;
      p->ocur.sizpos.snapMoumsg = WM_BUTTON1CLICK;
      // entryfield content selection
      p->ocur.txt.selMoukbd = 0;
      p->ocur.txt.selMoumsg = WM_BUTTON1DBLCLK;
      // text controls copy/append/cut/insert/overwrite
      p->ocur.txt.cpyMoukbd = KC_CTRL;
      p->ocur.txt.cpyMoumsg = WM_BUTTON1DOWN;
      p->ocur.txt.ovwMoukbd = KC_CTRL | KC_SHIFT;
      p->ocur.txt.ovwMoumsg = WM_BUTTON1DOWN;
      p->ocur.txt.cutMoukbd = KC_ALT;
      p->ocur.txt.cutMoumsg = WM_BUTTON1DOWN;
      p->ocur.txt.insMoukbd = KC_SHIFT;
      p->ocur.txt.insMoumsg = WM_BUTTON2DOWN;
      p->ocur.txt.ovwMoukbd = KC_CTRL | KC_SHIFT;
      p->ocur.txt.ovwMoumsg = WM_BUTTON2DOWN;
      // frame drag
      p->ocur.sizpos.frameMoukbd = 0;
      p->ocur.sizpos.frameMoumsg = WM_BUTTON2DOWN;
      // sposta in fondo allo Z-order
      p->ocur.sizpos.ZordMoukbd = KC_SHIFT;
      p->ocur.sizpos.ZordMoumsg = WM_BUTTON1CLICK;
   } else {        // ---------------- dialogo tastiera -------------------
      // roll windows
      p->ocur.roll.kbd = 0;
      p->ocur.roll.kbdKC = KC_ALT;
      p->ocur.roll.kbdVK = VK_F1 | 0x3b00;
      // hide titlebar
      p->ocur.tbh.kbd = 0;
      p->ocur.tbh.kbdKC = KC_ALT;
      p->ocur.tbh.kbdVK = VK_F2 | 0x3c00;
      // VIO window copy/append/paste
      p->ocur.vio.cpyKbdKC = KC_CTRL;
      p->ocur.vio.cpyKbdVK = VK_INSERT | 0x6800;
      p->ocur.vio.cpyaKbdKC = KC_CTRL | KC_SHIFT;
      p->ocur.vio.cpyaKbdVK = VK_INSERT | 0x6800;
      p->ocur.vio.insKbdKC = KC_SHIFT;
      p->ocur.vio.insKbdVK = VK_INSERT | 0x6800;
      // acceleratori folder
      p->ocur.wpskbd.sets = 0;
      p->ocur.wpskbd.setsKC = KC_CTRL;
      p->ocur.wpskbd.setsK = VK_F12 | 0x5800;
      p->ocur.wpskbd.parent = 0;
      p->ocur.wpskbd.parentKC = 0;
      p->ocur.wpskbd.parentK = VK_BACKSPACE | 0x0e00;
      p->ocur.wpskbd.find = 0;
      p->ocur.wpskbd.findKC = KC_CTRL;
      p->ocur.wpskbd.findK = VK_F1 | 0x3b00;
      p->ocur.wpskbd.srtname = 0;
      p->ocur.wpskbd.srtnameKC = 0;
      p->ocur.wpskbd.srtnameK = VK_F2 | 0x3c00;
      p->ocur.wpskbd.srtext = 0;
      p->ocur.wpskbd.srtextKC = 0;
      p->ocur.wpskbd.srtextK = VK_F3 | 0x3d00;
      p->ocur.wpskbd.srttype = 0;
      p->ocur.wpskbd.srttypeKC = KC_CTRL;
      p->ocur.wpskbd.srttypeK = VK_F2 | 0x3c00;
      p->ocur.wpskbd.srtrname = 0;
      p->ocur.wpskbd.srtrnameKC = KC_CTRL;
      p->ocur.wpskbd.srtrnameK = VK_F3 | 0x3d00;
      p->ocur.wpskbd.srtsize = 0;
      p->ocur.wpskbd.srtsizeKC = KC_CTRL;
      p->ocur.wpskbd.srtsizeK = VK_F4 | 0x3e00;
      p->ocur.wpskbd.srtwrite = 0;
      p->ocur.wpskbd.srtwriteKC = 0;
      p->ocur.wpskbd.srtwriteK = VK_F4 | 0x3e00;
      p->ocur.wpskbd.srtacc = 0;
      p->ocur.wpskbd.srtaccKC = KC_CTRL;
      p->ocur.wpskbd.srtaccK = VK_F5 | 0x3f00;
      p->ocur.wpskbd.srtcreat = 0;
      p->ocur.wpskbd.srtcreatKC = KC_CTRL;
      p->ocur.wpskbd.srtcreatK = VK_F6 | 0x4000;
      p->ocur.wpskbd.arrange = 0;
      p->ocur.wpskbd.arrangeKC = 0;
      p->ocur.wpskbd.arrangeK = VK_F6 | 0x4000;
      p->ocur.wpskbd.refresh = 0;
      p->ocur.wpskbd.refreshKC = 0;
      p->ocur.wpskbd.refreshK = VK_F5 | 0x3f00;
      p->ocur.wpskbd.on = 0;
   } // end if
   if (hwnd) MouKbdUpdateFElist(hwnd);
}
