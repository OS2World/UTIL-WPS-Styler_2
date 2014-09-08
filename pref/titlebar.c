//==========================================================================
// titlebar.c: titlebar settings dialog procedures
//==========================================================================

#pragma strings(readonly)

#include "pref.h"
#include "api.h"

// globals ------------------------------------------------------------------


// prototypes ---------------------------------------------------------------

VOID TbarMainOptProcInit(HWND hwnd);
VOID TbarMainOptProcCommand(HWND hwnd, ULONG ulId);
VOID TbarMainOptProcControl(HWND hwnd, MPARAM mp1, PUSERBUTTON pub);
VOID TbarMainOptSetCtls(HWND hwnd);
VOID TbarMainOptSetBkgndDep(HWND hwnd, PTBARAIOPT ptbo);
VOID TbarMainOptSet3DborderDep(HWND hwnd, PTBARAIOPT ptbo);
VOID TbarMainOptSetTextDep(HWND hwnd, PTBARAIOPT ptbo);
VOID TbarMainOptSetDrctDep(HWND hwnd, PTBARAIOPT ptbo);
VOID TbarMainOptEnable(HWND hwnd, BOOL flag);
BOOL TbarMainOptSetColor(HWND hwnd, ULONG ulId, PCLR pclr);
BOOL TbarMainOptSetFont(HWND hwnd);
VOID TbarMainOptFormatSample(HWND hwnd);
VOID TBarMainOptDrawBtn(PUSERBUTTON pub, HBITMAP hbmp,
                        ULONG ulSysBmp, ULONG uloff);
VOID TbarMainOptApply(HWND hwnd);
ULONG applyFrameCtrlBmp(HPS hps, ULONG idx);
VOID TbarMainOptUndo(HWND hwnd);
VOID SetButtonColor(HWND hwnd, PCLR pclr);
VOID SwapColors(HWND hwnd, ULONG id1, PCLR pclr1, ULONG id2, PCLR pclr2);


//==========================================================================
// procedura dialogo principale impostazione colori e testo barra titolo
// finestra attiva e inattiva
// messaggi:
//==========================================================================

MRESULT EXPENTRY TitleBarMainOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         TbarMainOptProcInit(hwnd);
         return (MRESULT)TRUE;
      case WM_WINDOWPOSCHANGED:
         if (((PSWP)mp1)->fl & SWP_SIZE) TbarMainOptFormatSample(hwnd);
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
      case WM_COMMAND:
         TbarMainOptProcCommand(hwnd, (ULONG)mp1);
         break;
      case WM_CONTROL:
         TbarMainOptProcControl(hwnd, mp1, mp2);
         break;
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return (MRESULT)FALSE;
}


//==========================================================================
// inizializzazione dialogo: ridimensiona la barra del titolo, ne aggiorna
//                           il testo e i parametri di presentazione;
//                           aggiorna (se warp 3 i parametri di
//                           presentazione del dialogo;
//                           memorizza nell'array di strutture PAGEDATA
//                           l'handle del dialogo corrente;
//                           riattiva la finestra principale
//==========================================================================
static
VOID TbarMainOptProcInit(HWND hwnd) {
   HWND ahwnd[8];
   ULONG i;
   // setta la frame di esempio come parent preview titlebar e controlli
   WinMultWindowFromIDs(hwnd, ahwnd, FRAMESAMPLE, BTN_TBMNMAX);
   for (i = 1; i < 8; ++i) WinSetParent(ahwnd[i], ahwnd[0], FALSE);
   // ridimensiona e posiziona titlebar e controlli finestra di esempio
   TbarMainOptFormatSample(hwnd);
   // imposta il testo della barra del titolo e inizializza valori default
   SetWinTxt(ahwnd[1], (p->idxPage == IDPG_TBAR_ACTWIN?
                        ID_TBARACTWINSMPL: ID_TBARINACTWINSMPL));
   p->pd[p->idxPage].hwnd = hwnd;       // memorizza hwnd
   p->pd[p->idxPage].def = 1;           // abilita pulsante def
   setparown(hwnd);   // riattiva finestra principale
   TbarMainOptSetCtls(hwnd);
}


//==========================================================================
// mostra il dialogo di editazione colore quando si clicca su uno dei
// bottoni del colore, mostra il dialogo apertura file quando si clicca sul
// bottone delle BMP, mostra il dialogo di selezione font quando si clicca
// sul pulsante Font.
// parametri:
// HWND hwnd: handle finestra principale dialogo
// MPARAM mp: ID controllo + codice notifica
// valore restituito:
// VOID
//==========================================================================
static
VOID TbarMainOptProcCommand(HWND hwnd, ULONG ulId) {
   PTBARAIOPT ptbo = p->idxPage == IDPG_TBAR_ACTWIN? &p->ocur.tb.a:
                                                     &p->ocur.tb.i;
   ULONG fl = ulId;
   INT idx;
   switch (ulId) {
      // bottoni toolbar ---------------------------------------------------
      case BTN_APPLY:
         TbarMainOptApply(hwnd);
         break;
      case BTN_UNDO:
         TbarMainOptUndo(hwnd);
         break;
      case BTN_DEFAULT:
         TbarMainOptDefault(hwnd);
         break;
      // bottoni settaggio barra titolo ------------------------------------
      case BTN_SHADEDIRECT:             // bottone direzione sfumatura
         // la sequenza Š vert - horizz. - vert.3 - horizz.3
         if (ptbo->shade) ptbo->shd3 = !ptbo->shd3;
         ptbo->shade = !ptbo->shade;
         TbarMainOptSetDrctDep(hwnd, ptbo);
         break;
      case BTN_SWPCOLSHADE:             // scambio colori sfumatura
         SwapColors(hwnd, BTN_COLUP, &ptbo->clrLeftTop,
                    BTN_COLDOWN, &ptbo->clrRghtBtm);
         break;
      case BTN_COLUP:                   // colore sinistra-sopra
         if (!TbarMainOptSetColor(hwnd, BTN_COLUP, &ptbo->clrLeftTop))
            fl = 0;
         break;
      case BTN_COLDOWN:                 // colore destra-sotto
         if (!TbarMainOptSetColor(hwnd, BTN_COLDOWN, &ptbo->clrRghtBtm))
            fl = 0;
         break;
      case BTN_SWPCOLBRDR:              // scambio colori bordo
         SwapColors(hwnd, BTN_HILITE, &ptbo->clrHiBkgnd,
                    BTN_SHADOW, &ptbo->clrShBkgnd);
         break;
      case BTN_HILITE:                  // colore bordo illuminato
         if (!TbarMainOptSetColor(hwnd, BTN_HILITE, &ptbo->clrHiBkgnd))
            fl = 0;
         break;
      case BTN_SHADOW:                  // colore bordo scuro
         if (!TbarMainOptSetColor(hwnd, BTN_SHADOW, &ptbo->clrShBkgnd))
            fl = 0;
         break;
      case BTN_SWPCOLTXT:               // scambia i colori del testo
         SwapColors(hwnd, BTN_FGNDTXT, &ptbo->clrFgTxt,
                    BTN_SHADOWTXT, &ptbo->clrBgTxt);
         break;
      case BTN_FGNDTXT:                 // colore primo piano testo
         if (!TbarMainOptSetColor(hwnd, BTN_FGNDTXT, &ptbo->clrFgTxt))
            fl = 0;
         break;
      case BTN_SHADOWTXT:               // colore testo sfondo
         if (!TbarMainOptSetColor(hwnd, BTN_SHADOWTXT, &ptbo->clrBgTxt))
            fl = 0;
         break;
      case BTN_BITMAP:                  // selezione bitmap sfondo
         idx = p->idxPage == IDPG_TBAR_ACTWIN? BMPATBAR : BMPITBAR;
         fl = p->abmp[idx].p ? 0 : BMPSEL_DEFAULT;
         if (AccLoadBmpSelDlg(hwnd, idx, &fl)) {
            if (p->abmp[idx].hbmp)
               bitmapSizeFromHbmp(p->abmp[idx].hbmp, &ptbo->size);
            WinSetDlgItemText(hwnd, BTN_BITMAP,
                              ioFNameFromPath(p->abmp[idx].ach));
            fl = BTN_BITMAP;
         } else {
            fl = 0;     // non aggiorna dati
         } // end if
         break;
      case BTN_FONT:                    // selezione font
         if (!TbarMainOptSetFont(hwnd)) fl = 0;
         break;
      // bottoni settaggio bitmap controlli frame --------------------------
      case BTN_TBMNSYS:
         idx = BMPSMENU;
         fl = p->abmp[idx].p ? 0: BMPSEL_DEFAULT;
         fl = (AccLoadBmpSelDlg(hwnd, idx, &fl)) ? BTN_SWPCOLSHADE : BTN_TBMNSYS;
         break;
      case BTN_TBMNCLOSE:
         idx = BMPCLOSE;
         switch (p->ocur.tb.close) {
            case 1: fl = BMPSEL_LEFTSIDE; break;
            case 2: fl = 0;               break;
            case 3: fl = BMPSEL_WIN95;    break;
            default: fl = BMPSEL_DISABLE; break;
         } /* endswitch */
         fl |= p->abmp[idx].p ? 0 : BMPSEL_DEFAULT;
         if (AccLoadBmpSelDlg(hwnd, idx, &fl)) {
            if (fl & BMPSEL_DISABLE) {
               p->ocur.tb.close = 0;
            } else if (fl & BMPSEL_LEFTSIDE) {
               p->ocur.tb.close = 1;
            } else if (fl & BMPSEL_WIN95) {
               p->ocur.tb.close = 3;
            } else {
               p->ocur.tb.close = 2;
            } /* endif */
            fl = BTN_SWPCOLSHADE;    // per abilitare Apply-Undo
            TbarMainOptFormatSample(hwnd);
         } else {
            fl = BTN_TBMNCLOSE;
         } // end if
         //TbarMainOptFormatSample(hwnd);
         break;
      case BTN_TBMNHIDE:
         idx = BMPHIDE;
         fl = p->abmp[idx].p ? 0 : BMPSEL_DEFAULT;
         fl = (AccLoadBmpSelDlg(hwnd, idx, &fl)) ?
                                       BTN_SWPCOLSHADE : BTN_TBMNHIDE;
         break;
      case BTN_TBMNMIN:
         idx = BMPMIN;
         fl = p->abmp[idx].p ? 0 : BMPSEL_DEFAULT;
         fl = (AccLoadBmpSelDlg(hwnd, idx, &fl)) ?
                                       BTN_SWPCOLSHADE : BTN_TBMNMIN;
         break;
      case BTN_TBMNRES:
         idx = BMPREST;
         fl = p->abmp[idx].p ? 0 : BMPSEL_DEFAULT;
         fl = (AccLoadBmpSelDlg(hwnd, idx, &fl)) ?
                                       BTN_SWPCOLSHADE : BTN_TBMNRES;
         break;
      case BTN_TBMNMAX:
         idx = BMPMAX;
         fl = p->abmp[idx].p ? 0 : BMPSEL_DEFAULT;
         fl = (AccLoadBmpSelDlg(hwnd, idx, &fl)) ?
                                       BTN_SWPCOLSHADE : BTN_TBMNMAX;
         break;
   } /* endswitch */
   // se Š cambiato qualche settaggio abilita Apply e Undo e aggiorna
   // sample barra titolo
   if (fl >= BTN_SWPCOLSHADE && fl <= BTN_SHADOWTXT) {
      EnableToolbarApplyBtn(1);
      EnableToolbarUndoBtn(1);
      repaintWindow(WinWindowFromID(WinWindowFromID(hwnd, FRAMESAMPLE),
                         ((ulId > TITLEBARSAMPLE) && (ulId <= BTN_TBMNMAX))?
                         ulId: TITLEBARSAMPLE));
   } /* endif */
}


//==========================================================================
// quando si clicca su un radiobutton o una checkbox imposta opportunamente
// i controlli dipendenti
// parametri:
// HWND hwnd: handle finestra principale dialogo
// MPARAM mp: ID controllo + codice notifica
// valore restituito:
//==========================================================================
static
VOID TbarMainOptProcControl(HWND hwnd, MPARAM mp1, PUSERBUTTON pub) {
   if (SHORT2FROMMP(mp1) == BN_CLICKED ||
       SHORT2FROMMP(mp1) == BN_DBLCLICKED) {
      PTBARAIOPT ptbo;
      HWND hother;
      HWND htbar = WinWindowFromID(WinWindowFromID(hwnd, FRAMESAMPLE),
                                   TITLEBARSAMPLE);
      ptbo = p->idxPage == IDPG_TBAR_ACTWIN?
                             &p->ocur.tb.a: &p->ocur.tb.i;
      switch (SHORT1FROMMP(mp1)) {
         case CHK_ENTBARENH:           // abilitazione opzioni
            p->ocur.tb.on = WinQueryButtonCheckstate(hwnd, CHK_ENTBARENH);
            TbarMainOptEnable(p->pd[IDPG_TBAR_ACTWIN].hwnd,
                              p->ocur.tb.on);
            TbarMainOptEnable(p->pd[IDPG_TBAR_INACTWIN].hwnd,
                              p->ocur.tb.on);
            break;
         // il context menu non ha bisogno del bottone apply!!!
         case CHK_TBOCNTXTMENU:
            p->ocur.tb.menu = p->po->tb.menu =
                              WinQueryButtonCheckstate(hwnd, CHK_TBOCNTXTMENU);
            hother = p->idxPage == IDPG_TBAR_ACTWIN?
                     p->pd[IDPG_TBAR_INACTWIN].hwnd:
                     p->pd[IDPG_TBAR_ACTWIN].hwnd;
            if (hother)
               WinCheckButton(hother, CHK_TBOCNTXTMENU, p->po->tb.menu);
            break;
         case RDBTN_SOLID:             // sfondo tinta unita
         case RDBTN_SHADE:             // sfondo sfumato
         case RDBTN_BITMAP:            // sfondo bitmap
            ptbo->bkgnd = SHORT1FROMMP(mp1) - RDBTN_SOLID;
            TbarMainOptSetBkgndDep(hwnd, ptbo);
            break;
         case CHK_TBOOVERRDPPS:
            p->ocur.tb.ovrPP = WinQueryButtonCheckstate(hwnd, CHK_TBOOVERRDPPS);
            hother = p->idxPage == IDPG_TBAR_ACTWIN?
                     p->pd[IDPG_TBAR_INACTWIN].hwnd:
                     p->pd[IDPG_TBAR_ACTWIN].hwnd;
            if (hother)
               WinCheckButton(hother, CHK_TBOOVERRDPPS, p->ocur.tb.ovrPP);
            break;
         case CHK_STRETCH:
            ptbo->strch = WinQueryButtonCheckstate(hwnd, CHK_STRETCH);
            break;
         case CHK_SHOWBORDER:
            ptbo->border = WinQueryButtonCheckstate(hwnd, CHK_SHOWBORDER);
            TbarMainOptSet3DborderDep(hwnd, ptbo);
            break;
         case CHK_TBCENTER:
            ptbo->center = WinQueryButtonCheckstate(hwnd, CHK_TBCENTER);
            break;
         case CHK_3DEFFECT:
            ptbo->_3DTxt = WinQueryButtonCheckstate(hwnd, CHK_3DEFFECT);
            TbarMainOptSetTextDep(hwnd, ptbo);
            break;
         default:
            return;
      } // end switch
      EnableToolbarApplyBtn(1);
      EnableToolbarUndoBtn(1);
      if (WinIsWindowShowing(hwnd)) repaintWindow(htbar);
   } else if (SHORT2FROMMP(mp1) == BN_PAINT) {
      switch (SHORT1FROMMP(mp1)) {
         case BTN_TBMNSYS:
            TBarMainOptDrawBtn(pub, p->abmp[BMPSMENU].hbmp,
                               (pub->fsState & BDS_HILITED?
                               SBMP_SYSMENUDEP: SBMP_SYSMENU), p->cyTitle);
            break;
         case BTN_TBMNCLOSE:
         #ifdef STLR_WARP3
            TBarMainOptDrawBtn(pub, p->abmp[BMPCLOSE].hbmp,
                               (p->ocur.tb.close?
                                  (p->cyTitle < 22?
                                     IDBMP_CLOSE18: IDBMP_CLOSE22)
                               :
                                  (p->cyTitle < 22?
                                     IDBMP_NULL18: IDBMP_NULL22)),
                               p->cyTitle);
         #else
            TBarMainOptDrawBtn(pub, p->abmp[BMPCLOSE].hbmp,
                               (p->ocur.tb.close?
                                  (pub->fsState & BDS_HILITED?
                                     SBMP_CLOSEDEP: SBMP_CLOSE)
                               :
                                  (p->cyTitle < 22?
                                     IDBMP_NULL18: IDBMP_NULL22)),
                               p->cyTitle);
         #endif
            break;
         case BTN_TBMNHIDE:
            TBarMainOptDrawBtn(pub, p->abmp[BMPHIDE].hbmp, (pub->fsState & BDS_HILITED?
                               SBMP_HIDEDEP: SBMP_HIDE), p->cyTitle);
            break;
         case BTN_TBMNMIN:
            TBarMainOptDrawBtn(pub, p->abmp[BMPMIN].hbmp, (pub->fsState & BDS_HILITED?
                               SBMP_MINBUTTONDEP: SBMP_MINBUTTON), p->cyTitle);
            break;
         case BTN_TBMNRES:
            TBarMainOptDrawBtn(pub, p->abmp[BMPREST].hbmp, (pub->fsState & BDS_HILITED?
                               SBMP_RESTOREBUTTONDEP: SBMP_RESTOREBUTTON), p->cyTitle);
            break;
         case BTN_TBMNMAX:
            TBarMainOptDrawBtn(pub, p->abmp[BMPMAX].hbmp, (pub->fsState & BDS_HILITED?
                               SBMP_MAXBUTTONDEP: SBMP_MAXBUTTON), p->cyTitle);
            break;
      } /* endswitch */
   } // end if
}


//==========================================================================
// inizializza/reinizializza hanlde bitmap usate e stato controlli
// parametri:
// HWND hwnd: handle del dialogo
// PTBARAIOPT ptbo: indirizzo struttura dati opzioni barra titolo
// valore restituito:
// VOID
//==========================================================================
static
VOID TbarMainOptSetCtls(HWND hwnd) {
   PTBARAIOPT ptbo;            // indirizzo struttura opzioni barra titolo
   HPS hps;
   HWND hOther;              // handle dialogo altra pagina settaggi titlebar
   PSTLRBMP psbmp;
   if (p->idxPage == IDPG_TBAR_ACTWIN) {
      psbmp = &p->abmp[BMPATBAR];
      ptbo = &p->ocur.tb.a;
      hOther = p->pd[IDPG_TBAR_INACTWIN].hwnd;
   } else {
      psbmp = &p->abmp[BMPITBAR];
      ptbo = &p->ocur.tb.i;
      hOther = p->pd[IDPG_TBAR_ACTWIN].hwnd;
   } // end if
   // ottiene handle bitmap usati x lo sfondo titlebar e x controlli finestra
   // se opzione bitmap e dati bitmap presenti visualizza la bitmap
   hps = WinGetPS(hwnd);
   if (psbmp->p)
      psbmp->hbmp = stlrHbmpFromData(hps, psbmp->p, &ptbo->size);
   WinSetDlgItemText(hwnd, BTN_BITMAP, psbmp->ach);
   // ottiene handle bitmap controlli
   if (p->abmp[BMPSMENU].p)
      p->abmp[BMPSMENU].hbmp = stlrHbmpFromData(hps, p->abmp[BMPSMENU].p, NULL);
   if (p->abmp[BMPCLOSE].p)
      p->abmp[BMPCLOSE].hbmp = stlrHbmpFromData(hps, p->abmp[BMPCLOSE].p, NULL);
   if (p->abmp[BMPHIDE].p)
      p->abmp[BMPHIDE].hbmp = stlrHbmpFromData(hps, p->abmp[BMPHIDE].p, NULL);
   if (p->abmp[BMPMIN].p)
      p->abmp[BMPMIN].hbmp = stlrHbmpFromData(hps, p->abmp[BMPMIN].p, NULL);
   if (p->abmp[BMPREST].p)
      p->abmp[BMPREST].hbmp = stlrHbmpFromData(hps, p->abmp[BMPREST].p, NULL);
   if (p->abmp[BMPMAX].p)
      p->abmp[BMPMAX].hbmp = stlrHbmpFromData(hps, p->abmp[BMPMAX].p, NULL);
   WinReleasePS(hps);
   // imposta il font della barra del titolo:
   if (!*p->ocur.tb.achFont)
   #ifdef STLR_WARP3
      PrfQueryProfileString(HINI_USERPROFILE, "PM_SystemFonts",
                            "WindowTitles", "10.System Proportional.Bold",
                            (PVOID)p->ocur.tb.achFont, CB_FONTNAME);
   #else
      PrfQueryProfileString(HINI_USERPROFILE, "PM_SystemFonts",
                            "WindowTitles", "9.WarpSans.Bold",
                            (PVOID)p->ocur.tb.achFont, CB_FONTNAME);
   #endif
   ResetTBarFont(hwnd, p->ocur.tb.achFont);
   if (hOther) ResetTBarFont(hOther, p->ocur.tb.achFont);
   // inizializza stato altri controlli
   WinCheckButton(hwnd, CHK_TBOCNTXTMENU, p->ocur.tb.menu);
   WinCheckButton(hwnd, RDBTN_SOLID + ptbo->bkgnd, TRUE);
   TbarMainOptSetBkgndDep(hwnd, ptbo);
   WinCheckButton(hwnd, CHK_TBOOVERRDPPS, p->ocur.tb.ovrPP);
   WinCheckButton(hwnd, CHK_STRETCH, ptbo->strch);
   WinCheckButton(hwnd, CHK_SHOWBORDER, ptbo->border);
   TbarMainOptSet3DborderDep(hwnd, ptbo);
   TbarMainOptSetDrctDep(hwnd, ptbo);
   WinCheckButton(hwnd, CHK_TBCENTER, ptbo->center);
   WinCheckButton(hwnd, CHK_3DEFFECT, ptbo->_3DTxt);
   TbarMainOptSetTextDep(hwnd, ptbo);
   SetButtonColor(WinWindowFromID(hwnd, BTN_COLUP), &ptbo->clrLeftTop);
   SetButtonColor(WinWindowFromID(hwnd, BTN_COLDOWN), &ptbo->clrRghtBtm);
   SetButtonColor(WinWindowFromID(hwnd, BTN_HILITE), &ptbo->clrHiBkgnd);
   SetButtonColor(WinWindowFromID(hwnd, BTN_SHADOW),&ptbo->clrShBkgnd);
   SetButtonColor(WinWindowFromID(hwnd, BTN_FGNDTXT), &ptbo->clrFgTxt);
   SetButtonColor(WinWindowFromID(hwnd, BTN_SHADOWTXT), &ptbo->clrBgTxt);
   WinCheckButton(hwnd, CHK_ENTBARENH, p->ocur.tb.on);
   TbarMainOptEnable(p->pd[IDPG_TBAR_ACTWIN].hwnd, p->ocur.tb.on);
   TbarMainOptEnable(p->pd[IDPG_TBAR_INACTWIN].hwnd, p->ocur.tb.on);
}


//==========================================================================
// inizializza controlli dipendenti dal settaggio del background
// parametri:
// HWND hwnd: handle del dialogo
// PTBARAIOPT ptbo: indirizzo struttura dati opzioni barra titolo
// valore restituito:
// VOID
//==========================================================================
static
VOID TbarMainOptSetBkgndDep(HWND hwnd, PTBARAIOPT ptbo) {
   BOOL solid = FALSE, shade = FALSE, bitmap = FALSE;
   switch (ptbo->bkgnd) {
      case 0: solid = TRUE; break;
      case 1: shade = TRUE; break;
      default: bitmap = TRUE; break;
   } // end switch
   DlgShowCtl(hwnd, BTN_COLUP, !bitmap);
   DlgShowCtl(hwnd, BTN_COLDOWN, shade);
   DlgShowCtl(hwnd, BTN_SWPCOLSHADE, shade);
   DlgShowCtl(hwnd, BTN_SHADEDIRECT, shade);
   DlgShowCtl(hwnd, BTN_BITMAP, bitmap);
   DlgShowCtl(hwnd, CHK_STRETCH, bitmap);
   SetCtlTxt(hwnd, BTN_COLUP,
             (solid? ID_B_KGND: (ptbo->shd3? ID_OUTER:
                                            (ptbo->shade? ID_TOP: ID_LEFT))));
}


//==========================================================================
// inizializza controlli dipendenti dal settaggio dello stile 3D
// parametri:
// HWND hwnd: handle del dialogo
// PTBARAIOPT ptbo: indirizzo struttura dati opzioni barra titolo
// valore restituito:
// VOID
//==========================================================================
static
VOID TbarMainOptSet3DborderDep(HWND hwnd, PTBARAIOPT ptbo) {
   DlgShowCtl(hwnd, BTN_SWPCOLBRDR, ptbo->border);
   DlgShowCtl(hwnd, BTN_HILITE, ptbo->border);
   DlgShowCtl(hwnd, BTN_SHADOW, ptbo->border);
}


//==========================================================================
// mostra/nasconde controlli dipendenti dal settaggio del testo
// parametri:
// HWND hwnd: handle del dialogo
// PTBARAIOPT ptbo: indirizzo struttura dati opzioni barra titolo
// valore restituito:
// VOID
//==========================================================================
static
VOID TbarMainOptSetTextDep(HWND hwnd, PTBARAIOPT ptbo) {
   SetCtlTxt(hwnd, BTN_FGNDTXT, (ptbo->_3DTxt? ID_TOP: ID_FGNDTXT));
   DlgShowCtl(hwnd, BTN_SWPCOLTXT, ptbo->_3DTxt);
   DlgShowCtl(hwnd, BTN_SHADOWTXT, ptbo->_3DTxt);
}


//==========================================================================
// inizializza controlli dipendenti dal settaggio della direzione della
// sfumatura
// parametri:
// HWND hwnd: handle del dialogo
// PTBARAIOPT ptbo: indirizzo struttura dati opzioni barra titolo
// valore restituito:
// VOID
//==========================================================================
static
VOID TbarMainOptSetDrctDep(HWND hwnd, PTBARAIOPT ptbo) {
   ULONG Drc, Up, Dwn;
   char buf[64];
   if (ptbo->shd3) {
      Drc = ptbo->shade? ID_HORZ: ID_VERTICAL3;
      Up = ID_OUTER;
      Dwn = ID_INNER;
   } else if (ptbo->shade) {
      Drc = ID_HORZ3;
      Up = ID_TOP;
      Dwn = ID_BOTTOM;
   } else {
      Drc = ID_VERTICAL;
      Up = ID_LEFT;
      Dwn = ID_RIGHT;
   } /* endif */
   SetCtlTxt(hwnd, BTN_SHADEDIRECT, Drc);
   SetCtlTxt(hwnd, BTN_COLUP, Up);
   SetCtlTxt(hwnd, BTN_COLDOWN, Dwn);
}


//==========================================================================
// disabilita/abilita i controlli secondo il settaggio di CHK_ENTBARENH
//==========================================================================
static
VOID TbarMainOptEnable(HWND hwnd, BOOL flag) {
   ULONG i;
   if (hwnd) {
      WinCheckButton(hwnd, CHK_ENTBARENH, flag);
      WinShowWindow(WinWindowFromID(hwnd, FRAMESAMPLE), flag);
      for (i = CHK_TBOCNTXTMENU; i <= BTN_SHADOWTXT; ++i)
         WinEnableControl(hwnd, i, flag);
   } /* endif */
}


//==========================================================================
// richiama dialogo selezione colori
// restituisce TRUE se il colore Š stato cambiato altrimenti FALSE
//==========================================================================
static
BOOL TbarMainOptSetColor(HWND hwnd, ULONG ulId, PCLR pclr) {
   PSETCOLOR psc;
   BOOL rc = FALSE;
   if (!(psc = malloc(sizeof(SETCOLOR)))) return FALSE;
   memset(psc, 0, sizeof(SETCOLOR));
   GetStr(ID_CLRTITLE, psc->achtitle, 80);
   GetStr(ID_CLROK, psc->achok, 32);
   GetStr(ID_CLRUNDO, psc->achundo, 32);
   GetStr(ID_CLRCANC, psc->achcanc, 32);
   GetStr(ID_CLRRED, psc->achred, 24);
   GetStr(ID_CLRGRN, psc->achgrn, 24);
   GetStr(ID_CLRBLU, psc->achblu, 24);
   psc->clrw.clr = pclr->l;
   psc->clrw.pszTitle = psc->achtitle;
   psc->clrw.pszOK = psc->achok;
   psc->clrw.pszUndo = psc->achundo;
   psc->clrw.pszCancel = psc->achcanc;
   psc->clrw.pszRed = psc->achred;
   psc->clrw.pszGreen = psc->achgrn;
   psc->clrw.pszBlue = psc->achblu;
   psc->clrw.fl = CWDS_CENTER;
   if ((psc->lclr = WeditColorDlg(HWND_DESKTOP, hwnd, (PCLRWHEEL)psc)) !=
        CLR_ERROR) {
      pclr->l = psc->lclr;
      SetButtonColor(WinWindowFromID(hwnd, ulId), pclr);
      rc = TRUE;
   } /* endif */
   free(psc);
   return rc;
}


//==========================================================================
// setta il font della barra del titolo
// restituisce TRUE se Š stato selezionato un nuovo font, altrimenti FALSE
//==========================================================================
static
BOOL TbarMainOptSetFont(HWND hwnd) {
   ULONG ul;
   HWND hsmpl = WinWindowFromID(WinWindowFromID(hwnd, FRAMESAMPLE),
                                TITLEBARSAMPLE);
   if (0 != (ul = setFont(hsmpl, ID_TBARFONT + (ID_FONTSAMPLE << 16),
                          p->ocur.tb.achFont))) {
      if (p->pd[IDPG_TBAR_ACTWIN].hwnd)
         ResetTBarFont(p->pd[IDPG_TBAR_ACTWIN].hwnd,
                       p->ocur.tb.achFont);
      if (p->pd[IDPG_TBAR_INACTWIN].hwnd)
         ResetTBarFont(p->pd[IDPG_TBAR_INACTWIN].hwnd,
                       p->ocur.tb.achFont);
      return TRUE;
   } // end if
   return FALSE;
}


//==========================================================================
// disegna i bottoni dei controlli della finestra sul dialogo,
// riposizionando i bottoni stessi e la barra del titolo secondo le opzioni
// scelte
// parametri:
// HWND hwnd: handle del dialogo
// PAPPDATA p: indirizzo struttura dati applicazione
// valore restituito:
// VOID
//==========================================================================
static
VOID TbarMainOptFormatSample(HWND hwnd) {
   WRECTL wrc;               // coordinate frame esempio + dimensioni dlg
   HWND hFrame = WinWindowFromID(hwnd, FRAMESAMPLE);
   HWND ahwnd[7];            // array handle controlli finestra
   SWP aswp[7];              // array SWP titlebar e controlli finestra
   ULONG i;
   WinQueryWindowRect(hwnd, (PRECTL)&wrc);            // dimensioni dialogo
   wrc.x = 2, wrc.y = 124;                            // coordinate frame di
   WinMapDlgPoints(hwnd, (PPOINTL)&wrc, 1, TRUE);     // esempio
   WinMultWindowFromIDs(hFrame, ahwnd, TITLEBARSAMPLE, BTN_TBMNMAX);
   // dati indipendenti dalla posizione del bottone di chiusura
   for (i = 0; i < 7; ++i) {
       aswp[i].hwnd = ahwnd[i];
       aswp[i].fl = SWP_SIZE | SWP_MOVE;
       aswp[i].y = 4;
       aswp[i].cy = p->cyTitle;
   } // end for
   aswp[0].cx = wrc.cx - wrc.x - wrc.x - 8 - 6 * p->cyTitle;  // titlebar
   aswp[0].cy = p->cyTitle;
   aswp[1].x = 4;                                               // system menu
   for (i = 1; i < 7; ++i) aswp[i].cx = p->cyTitle;
   // coordinate dipendenti da posizione bottone chiusura
   if (p->ocur.tb.close == 1) {      // se bottone chiusura a sinistra
      aswp[2].x = aswp[1].x + p->cyTitle;
      aswp[0].x = aswp[2].x + p->cyTitle;
      aswp[3].x = aswp[0].x + aswp[0].cx;
   } else if (p->ocur.tb.close == 3) { // posiz. win95
      aswp[0].x = aswp[1].x + p->cyTitle;
      aswp[3].x = aswp[0].x + aswp[0].cx;
      aswp[2].x = aswp[3].x + p->cyTitle * 4;
   } else {                                // default
      aswp[0].x = aswp[1].x + p->cyTitle;
      aswp[2].x = aswp[0].x + aswp[0].cx;
      aswp[3].x = aswp[2].x + p->cyTitle;
   } /* endif */
   aswp[4].x = aswp[3].x + p->cyTitle;
   aswp[5].x = aswp[4].x + p->cyTitle;
   aswp[6].x = aswp[5].x + p->cyTitle;
   WinSetWindowPos(hFrame, NULLHANDLE, wrc.x, wrc.y, wrc.cx - wrc.x - wrc.x,
                   p->cyTitle + 8, SWP_SIZE | SWP_MOVE);
   WinSetMultWindowPos(p->hab, aswp, 7);
}


//==========================================================================
// disegna bitmap controlli aggiuntivi finestra su bottone. Se non Š stata
// specificata alcuna bitmap dall'utente usa la corrispondente bitmap di
// default
// parametri:
// PUSERBUTTON pub: indirizzo struttura USERBUTTON
// HBITMAP hbmp: handle bitmap da disegnare sul bottone
// ULONG ulSysBmp: indice bitmap sistema
// ULONG uloff: dimensione bottone ed eventale offset bitmap bottone premuto
// HWND hwnd: handle del dialogo
// PTBARAIOPT ptbo: indirizzo struttura dati opzioni barra titolo
// valore restituito:
// VOID
//==========================================================================
static
VOID TBarMainOptDrawBtn(PUSERBUTTON pub, HBITMAP hbmp,
                        ULONG ulSysBmp, ULONG uloff) {
   WRECTL wr = {0, 0, uloff, uloff};
   BOOL custom;
   POINTL ptl = {0, 0};
   if (0 == (custom = hbmp)) {
      if (ulSysBmp >= 200)
         hbmp = GpiLoadBitmap(pub->hps, p->hres, ulSysBmp, 0, 0);
      else
         hbmp = WinGetSysBitmap(HWND_DESKTOP, ulSysBmp);
   } // end if
   if ((pub->fsState & BDS_HILITED) &&
       (custom || (!custom && ulSysBmp >= 200)))
      ++ptl.x, --ptl.y;
//      wr.x = uloff, wr.cx = uloff + uloff;
   WinDrawBitmap(pub->hps, hbmp, (PRECTL)&wr, &ptl, 0, 0, DBM_NORMAL);
   if (!custom) GpiDeleteBitmap(hbmp);
}


//==========================================================================
// applica i settaggi correnti copiando la struttura TBARAIOPT, e i dati
// relativi ai bottoni della barra del titolo
// i settaggi correnti (p->ocur) vengono comparati preventivamente con
// quelli correntemente attivi e la flag flApply viene settata di
// conseguenza allo scopo di aggiornare solo i settaggi differenti
// parametri:
// HWND hwnd: handle del dialogo
// PAPPDATA p: indirizzo struttura dati applicazione
// valore restituito:
// VOID
//==========================================================================

VOID TbarMainOptApply(HWND hwnd) {
   PTBARAIOPT ptbosrc;             // nuovi dati struttura TBARAIOPT
   PTBARAIOPT ptbodest;            // indirizzo in cui scrivere nuovi dati
   ULONG flApply;
   INT idx;
   HPS hps;
   BOOL mustCloseProfile = FALSE;
   p->pd[p->idxPage].apply = 0;
   if (hwnd) WinEnableWindow(p->swpCtls[3].hwnd, FALSE);
   // controlla pagina e se sono cambiati settaggi titlebar
   if (p->idxPage == IDPG_TBAR_ACTWIN) {
      ptbosrc = &p->ocur.tb.a;
      ptbodest = &p->po->tb.a;
      flApply = MODIF_ACTWIN;
      idx = BMPATBAR;
   } else {
      ptbosrc = &p->ocur.tb.i;
      ptbodest = &p->po->tb.i;
      flApply = MODIF_INACTWIN;
      idx = BMPITBAR;
   } /* endif */
   // aggiorna settaggi barre titolo (enabled = x finestra attiva e inattiva
   if (p->po->tb.on != p->ocur.tb.on) {
      flApply |= MODIF_ALLTBAR;
      p->po->tb.on = p->ocur.tb.on;
   } /* endif */
   if ((p->po->tb.ovrPP != p->ocur.tb.ovrPP) ||
       memcmp(ptbodest, ptbosrc, CBTBAIOPTCMP)) {
      flApply |= MODIF_TBAR;
      p->po->tb.ovrPP = p->ocur.tb.ovrPP;
      memcpy(ptbodest, ptbosrc, CBTBAIOPTCMP);
   } // end if
   if (strcmp(p->po->tb.achFont, p->ocur.tb.achFont)){
      flApply |= MODIF_FONT;
      strcpy(p->po->tb.achFont, p->ocur.tb.achFont);
   } // end if
   if (p->po->tb.close != p->ocur.tb.close) {
      flApply |= MODIF_CLOSEPOS;
      p->po->tb.close = p->ocur.tb.close;
   } // end if
   if (!p->hini) {
      p->hini = openProfile(TRUE);
      mustCloseProfile = TRUE;
   } /* endif */
   hps = WinGetScreenPS(HWND_DESKTOP);
   flApply |= applyFrameCtrlBmp(hps, idx);
   for (idx = BMPSMENU; idx <= BMPMAX; ++idx) {
      flApply |= applyFrameCtrlBmp(hps, idx);
   } /* endfor */
   WinReleasePS(hps);
   if (mustCloseProfile) stlrCloseProfile();
   // aggiorna i controlli di tutte le frame windows
   if (flApply)
      _stlrMsgPostAll(STLR_REDRAWTITLE, flApply);
}



//===========================================================================
// Store the new bitmap data in SMARTWIN.INI.
// Update the global bitmap.
// Return the modification flag corresponding to idx.
// Parameters --------------------------------------------------------------
// HPS hps   : presentation space handle.
// ULONG idx : bitmap idx.
// Return value ------------------------------------------------------------
// ULONG : modification flag.
//===========================================================================
static
ULONG applyFrameCtrlBmp(HPS hps, ULONG idx) {
   USHORT aFlags[] = AMODIFFLAGS;
   prfStoreBitmapData(idx);
   resetGlobalBitmap(hps, idx);
   return aFlags[idx];
}


//==========================================================================
// copia precedenti settaggi relativi alla barra del titolo da p->ou a
// p->ocur, disabilita undo e abilita apply
// parametri:
// HWND hwnd: handle del dialogo
// PAPPDATA p: indirizzo struttura dati applicazione
// valore restituito:
// VOID
//==========================================================================
static
VOID TbarMainOptUndo(HWND hwnd) {
   PTBARAIOPT ptbosrc;             // nuovi dati struttura TBARAIOPT
   PTBARAIOPT ptbodest;            // indirizzo in cui scrivere nuovi dati
   INT idx;
   EnableToolbarApplyBtn(1);      // abilita apply
   p->pd[p->idxPage].undo = 0;     // disabilita undo
   WinEnableWindow(p->swpCtls[4].hwnd, FALSE);
   // controlla pagina e se sono cambiati settaggi titlebar
   if (p->idxPage == IDPG_TBAR_ACTWIN) {
      ptbosrc = &p->ou.tb.a;
      ptbodest = &p->ocur.tb.a;
      idx = BMPATBAR;
   } else {
      ptbosrc = &p->ou.tb.i;
      ptbodest = &p->ocur.tb.i;
      idx = BMPITBAR;
   } /* endif */
// aggiorna settaggi barre titolo (enabled = x finestra attiva e inattiva
   memcpy(ptbodest, ptbosrc, CBTBAIOPTCMP);
   p->ocur.tb.on = p->ou.tb.on;
   p->ocur.tb.close = p->ou.tb.close;
   p->ocur.tb.ovrPP = p->ou.tb.ovrPP;
   p->ocur.tb.menu = p->ou.tb.menu;
   strcpy(p->ocur.tb.achFont, p->ou.tb.achFont);
   undoFrameCtrlBmp(hwnd, NULLHANDLE, idx, 0);
   // aggiorna settaggi relativi ai controlli finestra
   for (idx = BMPSMENU; idx <= BMPMAX; ++idx) {
      undoFrameCtrlBmp(hwnd, NULLHANDLE, idx, 0);
   } /* endfor */
   // resetta stato controlli
   TbarMainOptSetCtls(hwnd);
   repaintWindowR(WinWindowFromID(hwnd, FRAMESAMPLE));
}


//==========================================================================
// applica i settaggi di default, abilta apply e undo
// parametri:
// HWND hwnd: handle del dialogo
// PAPPDATA p: indirizzo struttura dati applicazione
// valore restituito:
// VOID
//==========================================================================

VOID TbarMainOptDefault(HWND hwnd) {
   INT idx;
   EnableToolbarApplyBtn(1);      // abilita apply
   EnableToolbarUndoBtn(1);       // abilita undo
   // aggiorna settaggi barre titolo (enbled = x finestra attiva e inattiva
   if (p->idxPage == IDPG_TBAR_ACTWIN) {
      p->ocur.tb.a.bkgnd = TBARBKGNDBMP;
      p->ocur.tb.a.border = 1;
      p->ocur.tb.a.shade = 1;
      p->ocur.tb.a.center = 0;
      p->ocur.tb.a._3DTxt = 1;
      p->ocur.tb.a.clrLeftTop.l = RGB_DARKBLUE;
      p->ocur.tb.a.clrRghtBtm.l = RGB_GRAY;
      p->ocur.tb.a.clrHiBkgnd.l = RGB_BLACK;
      p->ocur.tb.a.clrShBkgnd.l = RGB_WHITE;
      p->ocur.tb.a.clrFgTxt.l = RGB_WHITE;
      p->ocur.tb.a.clrBgTxt.l = RGB_BLACK;
      idx = BMPATBAR;
   } else {
      p->ocur.tb.i.bkgnd = TBARBKGNDBMP;
      p->ocur.tb.i.border = 1;
      p->ocur.tb.i.shade = 1;
      p->ocur.tb.i.center = 0;
      p->ocur.tb.i._3DTxt = 1;
      p->ocur.tb.i.clrLeftTop.l = RGB_GRAY;
      p->ocur.tb.i.clrRghtBtm.l = RGB_DARKGRAY;
      p->ocur.tb.i.clrHiBkgnd.l = RGB_WHITE;
      p->ocur.tb.i.clrShBkgnd.l = RGB_BLACK;
      p->ocur.tb.i.clrFgTxt.l = RGB_DARKGRAY;
      p->ocur.tb.i.clrBgTxt.l = RGB_WHITE;
      idx = BMPITBAR;
   } /* endif */
   // aggiorna settaggi relativi ai controlli finestra
   p->ocur.tb.on = 1;
   p->ocur.tb.close = 2;
   p->ocur.tb.ovrPP = 1;
   p->ocur.tb.menu = 1;
   // settaggi system menu
#ifdef STLR_WARP3
   strcpy(p->ocur.tb.achFont, "10.System Proportional.Bold");
#else
   strcpy(p->ocur.tb.achFont, "9.WarpSans Bold");
#endif
   defaultFrameCtrlBmp(hwnd, NULLHANDLE, idx, 0);
   // aggiorna settaggi relativi ai controlli finestra
   for (idx = BMPSMENU; idx <= BMPMAX; ++idx) {
      defaultFrameCtrlBmp(hwnd, NULLHANDLE, idx, 0);
   } /* endfor */
   // resetta stato controlli
   if (hwnd) {
      TbarMainOptSetCtls(hwnd);
      repaintWindowR(WinWindowFromID(hwnd, FRAMESAMPLE));
   } /* endif */
}


//==========================================================================
// cambia colore sfondo bottone e setta il colore del testo bianco o nero
// secondo la luminosit… del colore di sfondo
// parametri:
// HWND hwnd: handle del bottone
// PCLR pclr: indirizzo della struttura colori
// valore restituito:
// VOID
//==========================================================================
static
VOID SetButtonColor(HWND hwnd, PCLR pclr) {
   LONG color = ((pclr->rgb.red + pclr->rgb.grn + pclr->rgb.blu) / 3) > 127?
                CLR_BLACK: CLR_WHITE;
   WinSetPresParam(hwnd, PP_BACKGROUNDCOLOR, sizeof(CLR), (PVOID)pclr);
   WinSetPresParam(hwnd, PP_DISABLEDBACKGROUNDCOLOR, sizeof(CLR), (PVOID)pclr);
   WinSetPresParam(hwnd, PP_FOREGROUNDCOLORINDEX, sizeof(LONG), (PVOID)&color);
}


//==========================================================================
// scambia i colori tra due bottoni
// parametri:
// HWND hwnd: handle del dialogo
// ULONG id1: id bottone 1
// PCLR pclr1: indirizzo struttura dati colore1
// ULONG id2: id bottone 2
// PCLR pclr2: indirizzo struttura dati colore2
// valore restituito:
// VOID
//=========================================================================
static
VOID SwapColors(HWND hwnd, ULONG id1, PCLR pclr1, ULONG id2, PCLR pclr2) {
   LONG lSwap = pclr1->l;
   pclr1->l = pclr2->l;
   pclr2->l = lSwap;
   SetButtonColor(WinWindowFromID(hwnd, id1), pclr1);
   SetButtonColor(WinWindowFromID(hwnd, id2), pclr2);
}



