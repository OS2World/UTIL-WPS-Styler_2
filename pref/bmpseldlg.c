//===========================================================================
// bmpseldlg.c: dialogo selezione bitmap
//===========================================================================

#pragma strings(readonly)
#include "pref.h"
#include "api.h"

// prototypes ---------------------------------------------------------------
MRESULT EXPENTRY BmpSelDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT BmpSelDlgInit(HWND hwnd, PBMPSELDLG pbsd);
MRESULT BmpSelDlgCheckMinSize(HWND hwnd, MPARAM mp1, MPARAM mp2);
MRESULT BmpSelDlgFormatCtls(HWND hwnd, MPARAM mp1, MPARAM mp2);
VOID BmpSelDlgMinMax(HWND hwnd, MPARAM mp1);
MRESULT BmpSelDlgControl(HWND hwnd, MPARAM mp1, MPARAM mp2);
MRESULT BmpSelDlgCommand(HWND hwnd, ULONG ulId);
MRESULT BmpSelDlgDrawItem(HWND hwnd, POWNERITEM poi);
VOID InitControlCoords(HWND hwnd, PBMPSELDLG pbsd);
MRESULT BmpSelDlgTerminate(HWND hwnd, BOOL flag);
VOID BmpSelDlgSetDisableDep(HWND hwnd, BOOL flag, BOOL def);
VOID BmpSelDlgSetDefaultDep(HWND hwnd, BOOL flag);
VOID BmpSelDlgEdit(HWND hwnd);
VOID BmpSelDlgImport(HWND hwnd);
MRESULT BmpSelDlgEditEnded(HWND hwnd);
VOID getBitmapsPath(PBMPSELDLG pbsd);


//==========================================================================
// - alloca la struttura bitmap selection dialog
// - la inizializza secondo i parametri ricevuti
// - mostra il dialogo di selezione bitmap
// - restituisce l'handle della nuova bitmap o nullhandle se non Š stata
//   selezionata una nuova bitmap, se la bitmap Š stata disabilitata o in
//   caso di errore
// - se la bitmap Š stata disabilitata libera l'handle precedente
// parametri:
// HWND hwnd: handle dialogo
// PBYTE pdata: area memorizzazione dati bitmap
// valore restituito:
// BOOL: TRUE = aggiornamento dati precedenti, FALSE = dati invariati
//==========================================================================

BOOL AccLoadBmpSelDlg(HWND hwnd, INT idx, PULONG pfl) {
   PSZ bmppath[] = {"\\tbkgnds", "\\tbkgnds", "\\sysmenu", "\\close",
                 "\\hide", "\\minimize", "\\restore", "\\maximize",
                 "\\roll", "\\unroll", "\\htbar", "\\unhtbar"};
   BYTE bmpdlgstyle[] = {5, 5, 5, 15, 5, 5, 5, 5, 7, 7, 7, 7};
   PBMPSELDLG pbsd;
   HBITMAP hbmp;
   BOOL rc = FALSE;
   HDIR hdir = HDIR_CREATE;
   FILEFINDBUF3 fbuf;
   ULONG ul = 1;
   // allocazione memoria x struttura dati bitmap selection dialog
   if (!(pbsd = (PBMPSELDLG)malloc(sizeof(BMPSELDLG)))) {
      ShowErr(ID_ERR_BSDALLOC);
      return rc;
   } // end if
   memset(pbsd, 0, sizeof(BMPSELDLG));
   GetStr(idx + ID_BSDTITLE0, p->buf, 256);
   strcpy(pbsd->achTitle, p->buf); // titolo dialogo
   pbsd->ulStyle = bmpdlgstyle[idx]; // controlli da visualizzare
   pbsd->flSet = *pfl;               // checkbox spuntate
   pbsd->cxImage = p->cyTitle * ((idx > BMPITBAR) ? 2 : 5);
   getBitmapsPath(pbsd);
   strcat(pbsd->achPath, "\\bitmaps");
   (idx > 1) ? strcat(pbsd->achPath, (p->cyTitle < 22? "\\vga": "\\xga")): 0;
   strcat(pbsd->achPath, bmppath[idx]);
   strcat(pbsd->achPath, "\\*.bmp");
   if (DosFindFirst(pbsd->achPath, &hdir, FILE_NORMAL, &fbuf,
                    sizeof(FILEFINDBUF3), &ul, FIL_STANDARD) == 3) {
      GetStr(ID_ERR_INVALIDPATH, p->buf, 256);
      strcat(p->buf, pbsd->achPath);
      WinPostMsg(p->hObj, OWM_SHOWERROR, (MPARAM)p->buf, (MPARAM)p);
      free((PVOID)pbsd);
      return rc;
   } /* endif */
   strcpy(pbsd->achBmp, p->abmp[idx].ach);// bmp correntemente attiva
   // se si Š cliccato su OK o doppio click su bitmap
   if (ACCWinDlgBox(hwnd, BmpSelDlgProc, DLG_BMPSEL, pbsd)) {
      if (*pfl != pbsd->flSet) {
         *pfl = pbsd->flSet;
         rc = TRUE;
      } /* endif */
      // se selezionata nuova bitmap o bitmap default cancella handle precedente
      if (pbsd->hbmp || (pbsd->flSet & (BMPSEL_DISABLE | BMPSEL_DEFAULT))) {
         GpiDeleteBitmap(p->abmp[idx].hbmp);
         p->abmp[idx].hbmp = NULLHANDLE;
         if (p->abmp[idx].p != p->abmpu[idx].p) free(p->abmp[idx].p);
         p->abmp[idx].p = NULL;
         p->abmp[idx].cb = 0;
         strcpy(p->abmp[idx].ach, pbsd->achBmp);
         p->abmp[idx].hbmp = pbsd->hbmp;
         if (pbsd->hbmp) {
            p->abmp[idx].cb = ReadBmpFile(pbsd->achPath, pbsd->bmpdata);
            if (p->abmp[idx].cb > 0) {
               if (!(p->abmp[idx].p = malloc(p->abmp[idx].cb))) {
                  ShowErr(ID_ERR_BSDALLOC);
                  free(pbsd);
                  return rc;
               } // end if
               memcpy(p->abmp[idx].p, pbsd->bmpdata, p->abmp[idx].cb);
            } /* endif */
         } /* endif */
         rc = TRUE;
      } /* endif */
   } /* endif */
   free(pbsd);
   return rc;
}


//==========================================================================
// procedura dialogo selezione bitmap
//==========================================================================

MRESULT EXPENTRY BmpSelDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         return BmpSelDlgInit(hwnd, (PBMPSELDLG)mp2);
      case WM_ACTIVATE:
         p->is.active = (BOOL)mp1;
         break;
      case WM_QUERYTRACKINFO:
         return BmpSelDlgCheckMinSize(hwnd, mp1, mp2);
      case WM_WINDOWPOSCHANGED:
         return BmpSelDlgFormatCtls(hwnd, mp1, mp2);
        // break;
      case WM_MINMAXFRAME:
         BmpSelDlgMinMax(hwnd, mp1);
         break;
      case WM_CONTROL:
         return BmpSelDlgControl(hwnd, mp1, mp2);
      case WM_COMMAND:
         return BmpSelDlgCommand(hwnd, (ULONG)mp1);
      case WM_APPTERMINATENOTIFY:
         return BmpSelDlgEditEnded(hwnd);
      case WM_MEASUREITEM:
         return (MRESULT)(WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR) + 10);
      case WM_DRAWITEM:
         return BmpSelDlgDrawItem(hwnd, (POWNERITEM)mp2);
      case WM_CLOSE:
         return BmpSelDlgTerminate(hwnd, (BOOL)mp2);
   } /* endswitch */
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
}


//==========================================================================
// - memorizza nelle window words l'indirizzo della struttura dati
// - cambia i parametri di presentazione del dialogo e del titolo
// - setta il testo del titolo
// - se necessario visualizza il pulsante "Find" e le checkbox
//   inizializzando queste ultime
// - rileva dimensioni minime dialogo
// - visualizza il dialogo posizionandolo nella precedente posizione
// - riempie la listbox con i file BMP contenuti nella directory achPath
// parametri:
// HWND hwnd: handle dialogo
// PBMPSELDLG pbsd: struttura dati applicazione
// valore restituito:
// MRESULT FALSE
//==========================================================================

MRESULT BmpSelDlgInit(HWND hwnd, PBMPSELDLG pbsd) {
   ULONG ul;
   PSZ psz;
   // memorizza nelle window words l'indirizzo della struttura dati
   WinSetWindowPtr(hwnd, 0L, (PVOID)pbsd);
   // imposta icona
   pbsd->hico = WinLoadPointer(HWND_DESKTOP, p->hres, 2);
   WinSendMsg(hwnd, WM_SETICON, (MPARAM)pbsd->hico, NULL);
   // si aggiunge alla window list
   pbsd->swctl.hwnd = hwnd;
   pbsd->swctl.idProcess = p->pid;
   pbsd->swctl.uchVisibility = SWL_VISIBLE;
   pbsd->swctl.fbJump = SWL_JUMPABLE;
   strncpy(pbsd->swctl.szSwtitle, pbsd->achTitle, MAXNAMEL);
   pbsd->swctl.bProgType = PROG_DEFAULT;
   WinAddSwitchEntry(&pbsd->swctl);
   // cambia i parametri di presentazione del dialogo e del titolo
   WsetDlgFonts(hwnd, 0, NULL);
   // setta il testo del titolo
   WinSetDlgItemText(hwnd, FID_TITLEBAR, pbsd->achTitle);
   // inizializza array ahwnd e aswp
   WinMultWindowFromIDs(hwnd, pbsd->ahwnd, BMPSELDLG_CTL1ST, BMPSELDLG_CTLLAST);
   for (ul = 0; ul < BMPSELDLG_CCTLS; ++ul)
      pbsd->aswp[ul].hwnd = pbsd->ahwnd[ul];
   // inizializza alcuni controlli secondo stile e stato
   WinShowWindow(pbsd->ahwnd[BTN_FINDBMP - BMPSELDLG_CTL1ST],
                 pbsd->ulStyle & BMPSEL_BTNFIND);
   WinShowWindow(pbsd->ahwnd[CHK_BMPDISABLE - BMPSELDLG_CTL1ST],
                 pbsd->ulStyle & BMPSEL_DISABLE);
   WinCheckButton(hwnd, CHK_BMPDISABLE, (pbsd->flSet & BMPSEL_DISABLE? 1: 0));
   WinShowWindow(pbsd->ahwnd[CHK_BMPDEFAULT - BMPSELDLG_CTL1ST],
                 pbsd->ulStyle & BMPSEL_DEFAULT);
   WinCheckButton(hwnd, CHK_BMPDEFAULT, (pbsd->flSet & BMPSEL_DEFAULT? 1: 0));
   // combobox posizione bottone close
   if (pbsd->ulStyle & BMPSEL_LEFTSIDE) {
      GetStr(ID_CLOSEBTNPOS, p->buf, 256);
      // replace tabs with nuls
      for (psz = p->buf; *psz; ++psz) if (*psz == '\t') *psz = 0;
      WfillListBox(hwnd, COMBO_CLOSEPOS, p->buf);
      WinShowWindow(pbsd->ahwnd[COMBO_CLOSEPOS - BMPSELDLG_CTL1ST], TRUE);
      DlgLboxSelectItem(hwnd, COMBO_CLOSEPOS,
                        (pbsd->flSet & BMPSEL_DISABLE? 0:
                          (pbsd->flSet & BMPSEL_LEFTSIDE? 1:
                            (pbsd->flSet & BMPSEL_WIN95? 3: 2))));
   } /* endif */
   BmpSelDlgSetDisableDep(hwnd, !(pbsd->flSet & BMPSEL_DISABLE),
                          !(pbsd->flSet & BMPSEL_DEFAULT));
   // rileva dimensioni minime dialogo
   InitControlCoords(hwnd, pbsd);
   pbsd->szMin.cx = BMPSELDLG_CX;
   pbsd->szMin.cy = BMPSELDLG_CY;
   WinMapDlgPoints(hwnd, (PPOINTL)&pbsd->szMin, 1, TRUE);
   pbsd->szMin.cx += p->ptlBrd.x + p->ptlBrd.x;
   pbsd->szMin.cy += p->cyTitle + p->ptlBrd.y +
                     p->ptlBrd.y;
   // posiziona, dimensiona e visualizza dialogo
   if (!p->wrBmpSel.cx) {  // se non Š presente posiz dimens precedente
      p->wrBmpSel.cx = pbsd->szMin.cx;
      p->wrBmpSel.cy = pbsd->szMin.cy;
      p->wrBmpSel.x = (WinQuerySysValue(HWND_DESKTOP, SV_CXFULLSCREEN) -
                              p->wrBmpSel.cx) / 2;
      p->wrBmpSel.y = (WinQuerySysValue(HWND_DESKTOP, SV_CYFULLSCREEN) -
                              p->wrBmpSel.cy) / 2;
   } // end if
   // riempie la listbox
   p->is.bsdbusy = 1;
   WinPostMsg(p->hObj, OWM_BSDLBOXFILL, (MPARAM)hwnd, (MPARAM)pbsd);
   WinSetWindowPos(hwnd, 0, p->wrBmpSel.x, p->wrBmpSel.y,
                   p->wrBmpSel.cx, p->wrBmpSel.cy,
                   SWP_SIZE | SWP_MOVE | SWP_SHOW);
   return (MRESULT)FALSE;
}


//==========================================================================
// modifica la struttura TRACKINFO per controllare la dimensione minima
// finestra
// parametri:
// HWND, MPARAM, MPARAM
// valore restituito:
// MRESULT mr restituito dalla WinDefDlgProc()
//==========================================================================

MRESULT BmpSelDlgCheckMinSize(HWND hwnd, MPARAM mp1, MPARAM mp2) {
   PBMPSELDLG pbsd = (PBMPSELDLG)WinQueryWindowPtr(hwnd, 0L);
   MRESULT mr = WinDefDlgProc(hwnd, WM_QUERYTRACKINFO, mp1, mp2);
   ((PTRACKINFO)mp2)->ptlMinTrackSize.x = pbsd->szMin.cx;
   ((PTRACKINFO)mp2)->ptlMinTrackSize.y = pbsd->szMin.cy;
   return mr;
}


//==========================================================================
// riposiziona e ridimensiona i controlli secondo le nuove dimensioni della
// finestra
// parametri:
// HWND, MPARAM, MPARAM
// valore restituito:
// VOID MRESULT
//==========================================================================

MRESULT BmpSelDlgFormatCtls(HWND hwnd, MPARAM mp1, MPARAM mp2) {
   PBMPSELDLG pbsd = (PBMPSELDLG)WinQueryWindowPtr(hwnd, 0L);
   WinDefDlgProc(hwnd, WM_WINDOWPOSCHANGED, mp1, mp2);
   if ((((PSWP)mp1)->fl & SWP_SIZE) &&      // se ridimensionamento ma
       !(((PSWP)mp1)->fl & SWP_MINIMIZE) &&  // non minimizzazione
       (((PSWP)mp1)->cy > MINCY)) {          // non arrotolamento
      POINTL ptl = {1, 1};
      if (!pbsd) return (MRESULT)FALSE;
      WinMapDlgPoints(hwnd, &ptl, 1, TRUE);
      // riposiziona i controlli ---------------------------------------------
      // listbox --------------
      pbsd->aswp[0].cx = ((PSWP)mp1)->cx - p->ptlBrd.x -
                         p->ptlBrd.x - pbsd->aswp[1].cx - 3 * ptl.x;
      pbsd->aswp[0].cy = ((PSWP)mp1)->cy - p->cyTitle -
                         3 * p->ptlBrd.y - ptl.y;
      // altri controlli ------
      pbsd->aswp[1].x = pbsd->aswp[2].x = pbsd->aswp[3].x = pbsd->aswp[4].x =
      pbsd->aswp[5].x = pbsd->aswp[6].x = pbsd->aswp[7].x = pbsd->aswp[8].x =
      pbsd->aswp[0].cx + pbsd->aswp[0].x + ptl.x;
      WinSetMultWindowPos(p->hab, pbsd->aswp, BMPSELDLG_CCTLS);
   } // end if
   if (!(((PSWP)mp1)->fl & (SWP_MAXIMIZE | SWP_RESTORE | SWP_MINIMIZE)) &&
       (((PSWP)mp1)->fl & (SWP_SIZE | SWP_MOVE)) && (((PSWP)mp1)->cy > MINCY)) {
      p->wrBmpSel.x = ((PSWP)mp1)->x;
      p->wrBmpSel.y = ((PSWP)mp1)->y;
      p->wrBmpSel.cx = ((PSWP)mp1)->cx;
      p->wrBmpSel.cy = ((PSWP)mp1)->cy;
   } /* endif */
   return (MRESULT)FALSE;
}


//==========================================================================\
// nasconde listbox x evitare che ricopra l'icona quando il dlg Š ridotto   |
// al minimo                                                                |
// parametri:                                                               |
// HWND, MPARAM                                                             |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID BmpSelDlgMinMax(HWND hwnd, MPARAM mp1) {
   PBMPSELDLG pbsd = (PBMPSELDLG)WinQueryWindowPtr(hwnd, 0L);
   if (((PSWP)mp1)->fl & SWP_MINIMIZE)
      WinShowWindow(pbsd->ahwnd[0], FALSE);
   else if (WS_MINIMIZED & WinQueryWindowULong(hwnd, QWL_STYLE))
      WinShowWindow(pbsd->ahwnd[0],
                    !WinQueryButtonCheckstate(hwnd, CHK_BMPDISABLE));
}


//==========================================================================\
// WinQueryButtonCheckstate() serve per correggere il mancato rilevamento   |
// di BN_CLICKED e la conseguente mancata disabilitazione della listbox     |
// parametri:                                                               |
// HWND, MPARAM                                                             |
// valore restituito:                                                       |
// MRESULT FALSE                                                            |
//==========================================================================/

MRESULT BmpSelDlgControl(HWND hwnd, MPARAM mp1, MPARAM mp2) {

   if (SHORT1FROMMP(mp1) == LBOX_BMPSEL) {
      if (SHORT2FROMMP(mp1) == LN_ENTER) {
         BmpSelDlgTerminate(hwnd, TRUE);
      } else if (SHORT2FROMMP(mp1) == LN_SELECT &&
                 (DlgLboxQuerySelectedItem(hwnd, LBOX_BMPSEL) != LIT_NONE)) {
         WinCheckButton(hwnd, CHK_BMPDEFAULT, 0);
         BmpSelDlgSetDefaultDep(hwnd, TRUE);
      } // end if
   } else if (SHORT1FROMMP(mp1) == CHK_BMPDISABLE &&
              (SHORT2FROMMP(mp1) == BN_CLICKED ||
               SHORT2FROMMP(mp1) == BN_DBLCLICKED)) {
      ULONG ul = WinQueryButtonCheckstate(hwnd, CHK_BMPDISABLE);
      DlgLboxSelectItem(hwnd, COMBO_CLOSEPOS, ul? 0: 2);
      BmpSelDlgSetDisableDep(hwnd, !ul,
                             !WinQueryButtonCheckstate(hwnd, CHK_BMPDEFAULT));
   } else if (SHORT1FROMMP(mp1) == CHK_BMPDEFAULT &&
              (SHORT2FROMMP(mp1) == BN_CLICKED ||
               SHORT2FROMMP(mp1) == BN_DBLCLICKED)) {
      PBMPSELDLG pbsd = (PBMPSELDLG)WinQueryWindowPtr(hwnd, 0L);
      if (WinQueryButtonCheckstate(hwnd, CHK_BMPDEFAULT)) {
         pbsd->idxSel = DlgLboxQuerySelectedItem(hwnd, LBOX_BMPSEL);
         DlgLboxSelectItem(hwnd, COMBO_CLOSEPOS, 2);
         DlgLboxDeselectItem(hwnd, LBOX_BMPSEL, pbsd->idxSel);
         BmpSelDlgSetDefaultDep(hwnd, 0);
         pbsd->idxSel = -1;
      } else {
         if (pbsd->idxSel < 0) pbsd->idxSel = 0;
         DlgLboxSelectItem(hwnd, LBOX_BMPSEL, pbsd->idxSel);
         BmpSelDlgSetDefaultDep(hwnd, 1);
      } // end if
   // combobox posizione bottone chiusura finestre
   } else if (SHORT1FROMMP(mp1) == COMBO_CLOSEPOS &&
              SHORT2FROMMP(mp1) == CBN_ENTER) {
      // se si seleziona no button simuyla click su checkbox Disable
      if (!DlgLboxQuerySelectedItem(hwnd, COMBO_CLOSEPOS)) {
         WinCheckButton(hwnd, CHK_BMPDISABLE, 1);
         BmpSelDlgSetDisableDep(hwnd, 0,
                             !WinQueryButtonCheckstate(hwnd, CHK_BMPDEFAULT));
      } /* endif */
   } // end if
   return (MRESULT)FALSE;
}


//==========================================================================\
// processa WM_COMMAND                                                      |
// parametri:                                                               |
// HWND hwnd: handle del dialogo                                            |
// ULONG ulId: id bottone                                                   |
// valore restituito:                                                       |
// MRESULT FALSE                                                            |
//==========================================================================/

MRESULT BmpSelDlgCommand(HWND hwnd, ULONG ulId) {
   switch (ulId) {
      case BTN_EDITBMP:
         BmpSelDlgEdit(hwnd);
         break;
      case BTN_FINDBMP:
         BmpSelDlgImport(hwnd);
         break;
      case BTN_BMPOK:
         BmpSelDlgTerminate(hwnd, TRUE);
         break;
      case BTN_BMPCANCEL:
         BmpSelDlgTerminate(hwnd, FALSE);
         break;
      case BTN_BMPHELP:
         WshowInf(hwnd, ID_INF_FILE, ID_INF_BITMAP);
         break;
   } /* endswitch */
   return (MRESULT)FALSE;
}


//==========================================================================\
// disegna l'item della listbox                                             |
// parametri:                                                               |
// HWND hwnd: handle finestra dialogo                                       |
// POWNERITEM poi: indirizzo struttura OWNERITEM                            |
// valore restituito:                                                       |
// MRESULT (ULONG) altezza item listbox                                     |
//==========================================================================/

MRESULT BmpSelDlgDrawItem(HWND hwnd, POWNERITEM poi) {
   PBMPSELDLG pbsd = (PBMPSELDLG)WinQueryWindowPtr(hwnd, 0L);
   LONG clr1, clr2, clr3, clr4, clr5;
   POINTL aptl[3];
   CHAR buf[256];
   SIZEL szl = {0, 0};
   WRECTL ir = {poi->rclItem.xLeft, poi->rclItem.yBottom,
                poi->rclItem.xRight - 1, poi->rclItem.yTop - 1};
   GpiCreateLogColorTable(poi->hps, 0, LCOLF_RGB, 0, 0, NULL);
  // if (poi->hItem) {
      if (poi->fsState) {
         clr1 = 0;        // BLACK
         clr2 = 0x808080; // DARKGRAY
         clr3 = 0xffffff; // BIANCO
         clr4 = 0x808080; // DARKGRAY
         clr5 = 0xffffff; // BIANCO

      } else {
         clr1 = 0x808080; // DARKGRAY
         clr2 = 0xffffff; // BIANCO
         clr3 = 0x808080; // DARKGRAY
         clr4 = 0;        // BLACK
         clr5 = 0;        // BLACK
      } /* endif */
      // sfondo --------------------------------
      GpiSetColor(poi->hps, 0xcccccc);
      GpiMove(poi->hps, (PPOINTL)&ir);
      GpiBox(poi->hps, DRO_FILL, ((PPOINTL)&ir) + 1, 0L, 0L);
      // sfondo bottone premuto ----------------
      if (poi->fsState) {
         aptl[0].x = ir.x + 1;
         aptl[0].y = ir.y + 1;
         aptl[1].x = ir.cx - 1;
         aptl[1].y = ir.cy - 1;
         GpiSetColor(poi->hps, 0xa5a5a5);
         GpiMove(poi->hps, aptl);
         GpiBox(poi->hps, DRO_FILL, aptl + 1, 0L, 0L);
      } /* endif */
      // bordo esterno superiore-sinistro
      GpiSetColor(poi->hps, clr1);
      aptl[0].x = aptl[1].x = ir.x + 1;
      aptl[0].y = ir.y + 1;
      aptl[1].y = aptl[2].y = ir.cy - 1;
      aptl[2].x = ir.cx - 2;
      GpiMove(poi->hps, aptl);
      GpiPolyLine(poi->hps, 2, aptl + 1);
      // bordo interno superiore-sinistro
      GpiSetColor(poi->hps, clr2);
      aptl[0].x = aptl[1].x = ir.x + 2;
      aptl[0].y = ir.y + 3;
      aptl[1].y = aptl[2].y = ir.cy - 2;
      aptl[2].x = ir.cx - 3;
      GpiMove(poi->hps, aptl);
      GpiPolyLine(poi->hps, 2, aptl + 1);
      aptl[0].x = aptl[1].x = ir.x + 3;
      aptl[0].y = ir.y + 4;
      aptl[1].y = aptl[2].y = ir.cy - 3;
      aptl[2].x = ir.cx - 4;
      GpiMove(poi->hps, aptl);
      GpiPolyLine(poi->hps, 2, aptl + 1);
      // bordo interno inferiore-destro
      GpiSetColor(poi->hps, clr3);
      aptl[0].x = ir.x + 3;
      aptl[0].y = aptl[1].y = ir.y + 2;
      aptl[1].x = aptl[2].x = ir.cx - 2;
      aptl[2].y = ir.cy - 3;
      GpiMove(poi->hps, aptl);
      GpiPolyLine(poi->hps, 2, aptl + 1);
      aptl[0].x = ir.x + 4;
      aptl[0].y = aptl[1].y = ir.y + 3;
      aptl[1].x = aptl[2].x = ir.cx - 3;
      aptl[2].y = ir.cy - 4;
      GpiMove(poi->hps, aptl);
      GpiPolyLine(poi->hps, 2, aptl + 1);
      // bordo esterno inferiore-destro
      GpiSetColor(poi->hps, clr4);
      aptl[0].x = ir.x + 2;
      aptl[0].y = aptl[1].y = ir.y + 1;
      aptl[1].x = aptl[2].x = ir.cx - 1;
      aptl[2].y = ir.cy - 1;
      GpiMove(poi->hps, aptl);
      GpiPolyLine(poi->hps, 2, aptl + 1);
      // Testo ---------------------------------
      WinQueryLboxItemText(poi->hwnd, poi->idItem, buf, 256);
      aptl[1].y = ir.y + 4;
      aptl[2].x = ir.cx - 4; aptl[2].y = ir.cy - 4;
      if (poi->fsState) {
         GpiSetColor(poi->hps, 0);
         aptl[0].x = aptl[1].x = pbsd->cxImage + 17;
         aptl[0].y = ir.y + 3 + p->cyTitle / 3;
         GpiCharStringPosAt(poi->hps, aptl, (PRECTL)(aptl + 1), CHS_CLIP,
                            strlen(buf), buf, NULL);
      } // end if
      GpiSetColor(poi->hps, clr5);
      aptl[0].x = aptl[1].x = pbsd->cxImage + 16;
      aptl[0].y = ir.y + 4 + p->cyTitle / 3;
      aptl[1].y = ir.y + 4;
      aptl[2].x = ir.cx - 4; aptl[2].y = ir.cy - 4;
      GpiCharStringPosAt(poi->hps, aptl, (PRECTL)(aptl + 1), CHS_CLIP,
                         strlen(buf), buf, NULL);
   // bitmap --------------------------------
   aptl[0].x = ir.x + 5; aptl[0].y = ir.y + 5;
   aptl[1].x = aptl[1].y = 0;
   aptl[2].x = pbsd->cxImage; aptl[2].y = p->cyTitle;
   WinDrawBitmap(poi->hps, poi->hItem, (PRECTL)(aptl + 1),
                 (PPOINTL)aptl, 0L, 0L, DBM_NORMAL);
   poi->fsState = poi->fsStateOld = 0;
   return (MRESULT) TRUE;
}


//==========================================================================
// Inizializza i valori dell'array delle strutture SWP relative ai controlli
// del dialogo
// parametri:
// HWND hwnd: handle dialogo
// PSWP pswp: array strutture SWP controlli
// valore restituito:
// VOID
//==========================================================================

VOID InitControlCoords(HWND hwnd, PBMPSELDLG pbsd) {
   POINTL ptl[2] = {{1, 1}, {76, 13}};
   WinMapDlgPoints(hwnd, ptl, 2, TRUE);
   // listbox --------------
   pbsd->aswp[0].x = ptl[0].x + p->ptlBrd.x;
   pbsd->aswp[0].y = ptl[0].y + p->ptlBrd.y;
   pbsd->aswp[0].fl = SWP_SIZE | SWP_MOVE;
   // bottone Help ---------
   pbsd->aswp[8].y = pbsd->aswp[0].y;
   pbsd->aswp[8].cx = ptl[1].x; pbsd->aswp[8].cy = ptl[1].y;
   pbsd->aswp[8].fl = SWP_SIZE | SWP_MOVE;
   // bottone Cancel -------
   pbsd->aswp[7].y = pbsd->aswp[8].y + pbsd->aswp[8].cy + ptl[0].y;
   pbsd->aswp[7].cx = ptl[1].x; pbsd->aswp[7].cy = ptl[1].y;
   pbsd->aswp[7].fl = SWP_SIZE | SWP_MOVE;
   // bottone OK -----------
   pbsd->aswp[6].y = pbsd->aswp[7].y + pbsd->aswp[7].cy + ptl[0].y;
   pbsd->aswp[6].cx = ptl[1].x; pbsd->aswp[6].cy = ptl[1].y;
   pbsd->aswp[6].fl = SWP_SIZE | SWP_MOVE;
   // combobox closebtn position
   pbsd->aswp[5].y = ptl[1].y;
   pbsd->aswp[5].cx = ptl[1].x;
   pbsd->aswp[5].cy = 4 * ptl[1].y - 7 * ptl[0].y;
   pbsd->aswp[5].fl = SWP_SIZE | SWP_MOVE;
   // bottone Default ------
   pbsd->aswp[4].y = pbsd->aswp[5].y + pbsd->aswp[5].cy + 2 * ptl[0].y;
   pbsd->aswp[4].cx = ptl[1].x; pbsd->aswp[4].cy = ptl[1].y - 2 * ptl[0].y;
   pbsd->aswp[4].fl = SWP_SIZE | SWP_MOVE;
   // bottone Disable ------
   pbsd->aswp[3].y = pbsd->aswp[4].y + pbsd->aswp[4].cy + ptl[0].y;
   pbsd->aswp[3].cx = ptl[1].x; pbsd->aswp[3].cy = pbsd->aswp[4].cy;
   pbsd->aswp[3].fl = SWP_SIZE | SWP_MOVE;
   // bottone Find ---------
   pbsd->aswp[2].y = pbsd->aswp[3].y + pbsd->aswp[3].cy + 3 * ptl[0].y;
   pbsd->aswp[2].cx = ptl[1].x; pbsd->aswp[2].cy = ptl[1].y;
   pbsd->aswp[2].fl = SWP_SIZE | SWP_MOVE;
   // bottone edit ---------
   pbsd->aswp[1].y =  pbsd->aswp[2].y + pbsd->aswp[2].cy + ptl[0].y;
   pbsd->aswp[1].cx = ptl[1].x; pbsd->aswp[1].cy = ptl[1].y;
   pbsd->aswp[1].fl = SWP_SIZE | SWP_MOVE;
}


//==========================================================================
// termina il dialogo di selezione bitmap restituendo l'handle della bitmap
// selezionata o NULLHANDLE se la bitmap selezionata Š = alla precedente,
// se si Š premuto Cancel, se si Š verificato un errore o se si Š
// disabilitato il bottone corrispondente e liberando i bitmap handle
// parametri:
// HWND hwnd: handle del dialogo
// BOOL flag: TRUE = doppio click o OK, FALSE = cancel o chiusura
// valore restituito:
// MRESULT FALSE
//==========================================================================

MRESULT BmpSelDlgTerminate(HWND hwnd, BOOL flag) {
   PBMPSELDLG pbsd = (PBMPSELDLG)WinQueryWindowPtr(hwnd, 0L);
   HBITMAP hbmp;
   ULONG citems;
   // se l'altro thread Š occupato setta la flag di chiusura, sospende
   // esecuzione, riposta a se stesso WM_CLOSE, e ritorna
   if (p->is.bsdbusy) {
      p->is.bsdend = 1;
      DosSleep(1);
      WinPostMsg(hwnd, WM_CLOSE, MPVOID, (MPARAM)flag);
      return (MRESULT)FALSE;
   } /* endif */
   citems = WinQueryLboxCount(pbsd->ahwnd[0]);
   pbsd->idxSel = WinQueryLboxSelectedItem(pbsd->ahwnd[0]);
   pbsd->flSet = 0;
   if (pbsd->happ) WinTerminateApp(pbsd->happ);
   if (flag) {    // controlla stato checkbox e selezione nuova bmp
      if (WinQueryButtonCheckstate(hwnd, CHK_BMPDISABLE))
         pbsd->flSet |= BMPSEL_DISABLE;
      if (WinQueryButtonCheckstate(hwnd, CHK_BMPDEFAULT))
         pbsd->flSet |= BMPSEL_DEFAULT;
      if (!(pbsd->flSet & (BMPSEL_DISABLE | BMPSEL_DEFAULT))) {
         INT i = DlgLboxQuerySelectedItem(hwnd, COMBO_CLOSEPOS);
         if (i == 1)
            pbsd->flSet |= BMPSEL_LEFTSIDE;
         else if (i == 3)
            pbsd->flSet |= BMPSEL_WIN95;
      } /* endif */
      if (pbsd->idxSel == pbsd->idxCurr ||
          pbsd->flSet & (BMPSEL_DISABLE | BMPSEL_DEFAULT))
         pbsd->idxSel = -1;
   } else {       // cancella solo tutti handle bmp e ritorna 0
      pbsd->idxSel = -1;
   } /* endif */
   while (citems--) {
      hbmp = (HBITMAP)WinSendMsg(pbsd->ahwnd[0], LM_QUERYITEMHANDLE,
                                 (MPARAM)citems, MPVOID);
      if (citems == pbsd->idxSel) pbsd->hbmp = hbmp;
      else GpiDeleteBitmap(hbmp);
   } /* endwhile */
   if (pbsd->idxSel >= 0) { // se Š stata selezionata una nuova bmp
      // ne memorizza nome, dati e dimensione nella struttura
      WinQueryLboxItemText(pbsd->ahwnd[0], pbsd->idxSel, pbsd->achBmp, 256);
      strcpy(ioFNameFromPath(pbsd->achPath), pbsd->achBmp);
   } else {
      *((PLONG)pbsd->bmpdata) = 0;
      pbsd->achBmp[0] = 0;
   } /* endif */
   p->is.bsdend = 0;
   WinPostMsg(hwnd, WM_DLGDISMISS, (MPARAM)flag, MPVOID);
   WinDestroyPointer(pbsd->hico);
   return (MRESULT)FALSE;
}


//==========================================================================
// Se viene spuntata la checkbox disable vengono disabilitati alcuni altri
// controlli
// HWND hwnd: handle del dialogo
// BOOL flag: TRUE abilita, FALSE disabilita
// valore restituito:
// VOID
//==========================================================================

VOID BmpSelDlgSetDisableDep(HWND hwnd, BOOL flag, BOOL def) {
   DlgShowCtl(hwnd, LBOX_BMPSEL, flag);
   WinEnableControl(hwnd, CHK_BMPDEFAULT, flag);
   BmpSelDlgSetDefaultDep(hwnd, flag & def);
}


//==========================================================================
// Se viene spuntata la checkbox default vengono disabilitati alcuni altri
// controlli
// HWND hwnd: handle del dialogo
// BOOL flag: TRUE abilita, FALSE disabilita
// valore restituito:
// VOID
//==========================================================================

VOID BmpSelDlgSetDefaultDep(HWND hwnd, BOOL flag) {
   WinEnableControl(hwnd, BTN_EDITBMP, flag);
   WinEnableControl(hwnd, BTN_FINDBMP, flag);
   WinEnableControl(hwnd, COMBO_CLOSEPOS, flag);
}


//==========================================================================
// Avvia iconedit.exe con la bitmap correntemente selezionata
// parametri:
// HWND hwnd: handle del dialogo
// valore restituito:
// VOID
//==========================================================================

VOID BmpSelDlgEdit(HWND hwnd) {
   PBMPSELDLG pbsd = (PBMPSELDLG)WinQueryWindowPtr(hwnd, 0L);
   CHAR buf[260];
   PSZ psz;
   WinEnableControl(hwnd, BTN_EDITBMP, FALSE);
   pbsd->idxSel = WinQueryLboxSelectedItem(pbsd->ahwnd[0]);
   WinQueryLboxItemText(pbsd->ahwnd[0], pbsd->idxSel, pbsd->achBmp, 256);
   strcpy(ioFNameFromPath(pbsd->achPath), pbsd->achBmp);
   strcpy(buf, pbsd->achPath);
   buf[ioFNameFromPath(buf) - buf - 1] = 0;
   if (!(pbsd->happ = WstartApp(hwnd, SZEXE_ICONEDIT, pbsd->achPath, buf,
                                SAF_INSTALLEDCMDLINE | SAF_STARTCHILDAPP))) {
      MsgBox(HWND_DESKTOP, ID_ERR_STARTICONEDIT, PMPRNT_ERROR);
      WinEnableControl(hwnd, BTN_EDITBMP, TRUE);    // pulsante edit
   } /* endif */
}


//==========================================================================
// Copia un file bmp nel path corrente
// parametri:
// HWND hwnd: handle del dialogo
// valore restituito:
// VOID
//==========================================================================

VOID BmpSelDlgImport(HWND hwnd) {
   PBMPSELDLG pbsd = (PBMPSELDLG)WinQueryWindowPtr(hwnd, 0L);
   p->is.bsdbusy = 1;
   WinPostMsg(p->hObj, OWM_BSDIMPORT, (MPARAM)hwnd, (MPARAM)pbsd);
}


//==========================================================================
// Riabilita il pulsante di editazione e aggiorna la lista delle bitmap
// parametri:
// HWND hwnd: handle del dialogo
// valore restituito:
// MRESULT
//==========================================================================

MRESULT BmpSelDlgEditEnded(HWND hwnd) {
   PBMPSELDLG pbsd = (PBMPSELDLG)WinQueryWindowPtr(hwnd, 0L);
   WinEnableControl(hwnd, BTN_EDITBMP, TRUE);
   pbsd->happ = 0;
   DlgLboxDeleteAll(hwnd, LBOX_BMPSEL);
   strcpy(ioFNameFromPath(pbsd->achPath), "*.bmp");
   p->is.bsdbusy = 1;
   WinPostMsg(p->hObj, OWM_BSDLBOXFILL, (MPARAM)hwnd, (MPARAM)pbsd);
   //BmpSelDlgFillBmpList(hwnd, pbsd);
   // WinInvalidateRect(pbsd->ahwnd[0], NULL, FALSE);
   pbsd->idxCurr = -1;
   return (MRESULT)FALSE;
}


//===========================================================================
// Get the path of the 'bitmaps' directory.
// Parameters --------------------------------------------------------------
// PBMPSELDLG pbsd : bitmap selection dialog data.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID getBitmapsPath(PBMPSELDLG pbsd) {
#ifdef DEMOVERSION
   PSZ pszEnd;
   uExeFullName(pbsd->achPath, CCHMAXPATH);
   pszEnd = ioFNameFromPath(pbsd->achPath);
   *(--pszEnd) = 0;
#else
   // costruisce path ricerca bitmap
   PrfQueryProfileString(HINI_USER, "Smart Windows", "path",
                         NULL, pbsd->achPath, CCHMAXPATH);  // path base bmp
#endif
}
