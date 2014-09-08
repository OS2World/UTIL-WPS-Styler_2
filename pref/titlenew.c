//==========================================================================
// titlenew.c : procedure of the titlebar settings preview
//==========================================================================

#pragma strings(readonly)

#include "pref.h"
#include "titlenew.h"
#include <images.h>
#include "api.h"

// prototypes ---------------------------------------------------------------
VOID ACCDrawTitlebar(PTITLESAMPLE pts);


//==========================================================================
// procedura dialogo principale impostazione colori e testo barra titolo
// finestra attiva e inattiva
//==========================================================================

MRESULT EXPENTRY ACCTitleWinProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   PTITLESAMPLE pts;
   switch (msg) {
      case WM_CREATE:
         // alloca struttura dati memorizzandone l'indirizzo all'offset 0
         if (!(pts = (PTITLESAMPLE)malloc(sizeof(TITLESAMPLE))))
            return (MRESULT)TRUE;
         memset(pts, 0, sizeof(TITLESAMPLE));
         WinSetWindowPtr(hwnd, 0L, (PVOID)pts);
         // inizializza la struttura
         pts->hwnd = hwnd;
         strcpy(pts->achTxt, ((PCREATESTRUCT)mp2)->pszText);
         pts->cbTxt = strlen(pts->achTxt);
         WinQueryClassInfo(p->hab, WC_TITLEBAR, &pts->ci);
         pts->size.cx = ((PCREATESTRUCT)mp2)->cx;
         pts->size.cy = ((PCREATESTRUCT)mp2)->cy;
         pts->ulCmdId[0] = 0;
         pts->ulCmdId[1] = BTN_SWPCOLSHADE;
         pts->ulCmdId[2] = BTN_SHADEDIRECT;
         pts->ptbo = (pts->i = (p->idxPage == IDPG_TBAR_INACTWIN))?
                     &p->ocur.tb.i: &p->ocur.tb.a;
         break;
      case WM_MOUSEMOVE:
      case WM_BUTTON1DOWN:
      case WM_BUTTON1UP:
      case WM_BUTTON1DBLCLK:
      case WM_BUTTON2DOWN:
      case WM_BUTTON2UP:
      case WM_BUTTON2DBLCLK:
      case WM_BUTTON3DOWN:
      case WM_BUTTON3UP:
      case WM_BUTTON3DBLCLK:
      case WM_CHORD:
         return (MRESULT)FALSE;
      case WM_BUTTON1CLICK:
      case WM_BUTTON2CLICK:
      case WM_BUTTON3CLICK:
         pts = (PTITLESAMPLE)WinQueryWindowPtr(hwnd, 0L);
         switch (pts->ptbo->bkgnd) {
            // sfumatura cicla tra i 4 pulsanti presenti
            case TBARBKGNDSHADE:
               ++pts->ulCmdId[0];
               if (pts->ulCmdId[0] > 2) pts->ulCmdId[0] = 1;
               WinPostMsg(p->pd[p->idxPage].hwnd,
                          WM_COMMAND,
                          (MPARAM)pts->ulCmdId[pts->ulCmdId[0]],
                          MPFROM2SHORT(CMDSRC_OTHER, TRUE));
               break;
            // bitmap apre dialogo selezione bitmap
            case TBARBKGNDBMP:
               WinPostMsg(p->pd[p->idxPage].hwnd,
                          WM_COMMAND, (MPARAM)(BTN_BITMAP),
                          MPFROM2SHORT(CMDSRC_OTHER, TRUE));
               break;
            // tinta unita mostra dialogo selezione colore sfondo
            default:
               WinPostMsg(p->pd[p->idxPage].hwnd,
                          WM_COMMAND, (MPARAM)(BTN_COLUP),
                          MPFROM2SHORT(CMDSRC_OTHER, TRUE));
               break;
         } /* endswitch */
         return (MRESULT)TRUE;
      case WM_WINDOWPOSCHANGED:
         pts = (PTITLESAMPLE)WinQueryWindowPtr(hwnd, 0L);
         if (((PSWP)mp1)->fl & SWP_SIZE) {
            pts->size.cx = ((PSWP)mp1)->cx;
            pts->size.cy = ((PSWP)mp1)->cy;
         } /* endif */
         break;
      case WM_PAINT:
         pts = (PTITLESAMPLE)WinQueryWindowPtr(hwnd, 0L);
         pts->hps = WinBeginPaint(hwnd, NULLHANDLE, NULL);
         ACCDrawTitlebar(pts);
         WinEndPaint(pts->hps);
         return (MRESULT)FALSE;
      case WM_PRESPARAMCHANGED:
         pts = (PTITLESAMPLE)WinQueryWindowPtr(hwnd, 0L);
         if ((LONG)mp1 == PP_FONTNAMESIZE) {
            WinQueryPresParam(hwnd, PP_FONTNAMESIZE, 0, NULL, 64,
                              (PPVOID)&pts->buf, 0);
            // se si Š cambiato font tramite drag&drop aggiorna dati p
            if (strcmp(p->ocur.tb.achFont, pts->buf)) {
               pts->cyfont = 0;
               strcpy(p->ocur.tb.achFont, pts->buf);
               SetCtlCompTxt(p->pd[p->idxPage].hwnd,
                             BTN_FONT, (PSZ)ID_FONT, pts->buf);
               if (p->pd[IDPG_TBAR_INACTWIN - pts->i].hwnd)
                  ResetTBarFont(p->pd[IDPG_TBAR_INACTWIN - pts->i].hwnd, pts->buf);
               // abilita pulsanti Apply e Undo
               EnableToolbarApplyBtn(1);
               EnableToolbarUndoBtn(1);
            } // end if
         } // end if
         break;
      case WM_SETWINDOWPARAMS:
         pts = (PTITLESAMPLE)WinQueryWindowPtr(hwnd, 0L);
         if ((PWNDPARAMS)mp1 && ((PWNDPARAMS)mp1)->fsStatus & WPM_TEXT) {
            strcpy(pts->achTxt, ((PWNDPARAMS)mp1)->pszText);
            pts->cbTxt = strlen(pts->achTxt);
         } // end if
         break;
      case WM_DESTROY:
         pts = (PTITLESAMPLE)WinQueryWindowPtr(hwnd, 0L);
         _stlrFreeHBMP(pts->hbmp);
         free(pts);
         break;
      default:
         pts = (PTITLESAMPLE)WinQueryWindowPtr(hwnd, 0L);
         break;
   } /* endswitch */
   return (MRESULT)(*pts->ci.pfnWindowProc)(hwnd, msg, mp1, mp2);
}


//===========================================================================
// Draw the titlebar sample.
// Parameters --------------------------------------------------------------
// PTITLESAMPLE pts : titlebar sample data.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================
static
VOID ACCDrawTitlebar(PTITLESAMPLE pts) {
   POINTL apt[3];
   INT i;
   PSHDCREATE psc;
   GpiCreateLogColorTable(pts->hps, 0, LCOLF_RGB, 0, 0, NULL);
   // background ----------------------------------------------------------
   switch (pts->ptbo->bkgnd) {
      // shade --------------------------------------------------------
      case TBARBKGNDSHADE:
         if (NULL != (psc = malloc(CB_SHADEALLOC))) {
            pts->hbmp = stlrHbmpOfShade(pts->hps, psc, pts->ptbo);
            if (pts->ptbo->shade) {         // sfumatura orizzontale
               WsetRect(apt, 0, 0, pts->size.cx, pts->size.cy);
               WinDrawBitmap(pts->hps, pts->hbmp, NULL, apt, 0, 0, DBM_STRETCH);
            } else {                        // sfumatura verticale
               WsetRect(apt, 0, 0, pts->size.cx, SHADEH_CY);
               for (i = 0; i < pts->size.cy; i += 2) {
                  WinDrawBitmap(pts->hps, pts->hbmp, NULL, apt, 0, 0, DBM_STRETCH);
                  ((PRECTL)apt)->yBottom += SHADEH_CY;
                  ((PRECTL)apt)->yTop += SHADEH_CY;
               } /* endfor */
            } /* endif */
            _stlrFreeHBMP(pts->hbmp);
            free(psc);
         } /* endif */
         break;
      // bitmap -------------------------------------------------------
      case TBARBKGNDBMP:
         // se hbmp = NULLHANDLE carica bmp default dalle risorse dell'eseguibile
         if (!p->abmp[pts->i].hbmp) {
            p->abmp[pts->i].hbmp = stlrHbmpFromModule(pts->hps, p->hres,
                                                 12 + pts->i, &pts->ptbo->size);
         } // end if
         WsetRect(&apt[1], 0, 0, pts->size.cx, pts->size.cy);
         if (pts->ptbo->strch) {
            WinDrawBitmap(pts->hps, p->abmp[pts->i].hbmp,
                          NULL, &apt[1], 0, 0, DBM_STRETCH);
         } else {
            apt[0].x = apt[0].y = 0;
            while (apt[0].x <= pts->size.cx) {
               WinDrawBitmap(pts->hps, p->abmp[pts->i].hbmp,
                             (PRECTL)&apt[1], apt, 0, 0, 0);
               apt[0].x += pts->ptbo->size.cx;
               apt[2].x = pts->size.cx - apt[0].x;
            } /* endwhile */
         } /* endif */
         break;
      // solid color --------------------------------------------------
      default:
         apt[0].x = pts->size.cx - 1;
         apt[0].y = pts->size.cy - 1;
         GpiSetColor(pts->hps, pts->ptbo->clrLeftTop.l);
         GpiBox(pts->hps, DRO_OUTLINEFILL, apt, 0L, 0L);
         break;
   } /* endswitch */
   // text ----------------------------------------------------------------
   if (!pts->cyfont) {
      GpiQueryTextBox(pts->hps, pts->cbTxt, pts->achTxt, 3, apt);
      pts->yfont = - apt[1].y;
      pts->cyfont = apt[0].y - apt[1].y;
      pts->cxfont = apt[2].x - apt[0].x;
   } /* endif */
   // se il testo Š pi— largo della finestra viene comunque allineato a sin.
   i = (pts->cxfont < pts->size.cx) ? 8: 0;
   apt[0].x = (pts->ptbo->center && i)?
              (pts->size.cx - pts->cxfont) / 2 + 1: i + 3;
   apt[0].y = (pts->size.cy - pts->cyfont) / 2 + pts->yfont;
   apt[1].x = 0;
   apt[1].y = 0;
   apt[2].x = pts->size.cx - 1;
   apt[2].y = pts->size.cy - 1;
   if (pts->ptbo->_3DTxt) {                         // disegna ombra testo
      GpiSetColor(pts->hps, pts->ptbo->clrBgTxt.l);
      GpiCharStringPosAt(pts->hps, apt, (PRECTL)(apt + 1), CHS_CLIP,
                         pts->cbTxt, pts->achTxt, NULL);
   } /* endif */
   apt[0].x--;
   apt[0].y++;
   GpiSetColor(pts->hps, pts->ptbo->clrFgTxt.l);
   GpiCharStringPosAt(pts->hps, apt, (PRECTL)(apt + 1), CHS_CLIP,
                      pts->cbTxt, pts->achTxt, NULL);
   // border --------------------------------------------------------------
   if (pts->ptbo->border) {
      GpiMove(pts->hps, &apt[1]);
      apt[1].y = apt[2].y;
      GpiSetColor(pts->hps, pts->ptbo->clrHiBkgnd.l);
      GpiPolyLine(pts->hps, 2, &apt[1]);
      apt[1].x = apt[2].x;
      apt[1].y = 0;
      apt[2].x = 1;
      apt[2].y = 0;
      GpiSetColor(pts->hps, pts->ptbo->clrShBkgnd.l);
      GpiPolyLine(pts->hps, 2, &apt[1]);
   } /* endif */
}
