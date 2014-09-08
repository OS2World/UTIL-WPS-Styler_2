//==========================================================================\
// notebook.c: funzioni utilit… per il controllo notebook                   |
// aggiungere api per inserire e cancellare pagine che modifichino anche    |
// la struttura ppd                                                         |
// BOOL APIENTRY WnbkFill(PNBK pnbk, PNBINSPG pnbi, BOOL fl);               |
// PPGDAT APIENTRY WnbkInitPg(PNBK pnbk, PBOOKPAGEINFO pbpi, PNBINSPG pnbi, |
//                            PPGDAT pprev, PULONG pcxM, PULONG pcxm);      |
// PPGDAT APIENTRY WnbkInsertPg(PNBK pnbk, PBOOKPAGEINFO pbpi, PNBINSPG pnbi,
//                              PPGDAT pprev, ULONG flins);                 |
// VOID APIENTRY WnbkDelPg(PNBK pnbk, ULONG id, ULONG fl);                  |
// ULONG APIENTRY WnbkPgn(PNBK pnbk, ULONG id, ULONG fl);                   |
// BOOL APIENTRY WnbkMinSize(PWDATA pw, ULONG cxm, ULONG cym, BOOL fl);     |
// PSZ APIENTRY WpageXofY(ULONG idstr, ULONG ulPg, ULONG cPgs, PSZ psz);    |
//==========================================================================/

#pragma strings(readonly)

#include "common.h"
#include "pmappapi.h"
#include "notebook.h"
#include "api.h"


//==========================================================================\
// riempie sequenzialmente di pagine un controllo notebook:                 |
// parametri:                                                               |
// PNBK pnbk: indirizzo struttura dati notebook                             |
// PNBINSPG pnbi: indirizzo array strutture NBINSPG                         |
// PPGLIST ppl: indirizzo struttura lista pagine                            |
// ULONG ci: numero di pagine (elementi array) da inserire                  |
// BOOL fl: indica necessit… calcolare e settare dimensioni tab             |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE : successo/errore                                    |
//==========================================================================/

BOOL APIENTRY WnbkFill(PNBK pnbk, PNBINSPG pnbi, BOOL fl) {
   POINTL ptl = {4, 8};     // usato per la conversione DlgUnits2pixel
   PBOOKPAGEINFO pbpi = NULL;
   BOOL rc = FALSE;
   ULONG i = 0;             // indice inserzione pagina
   ULONG cxM, cxm;          // larghezza minortab e majortab
   PPGDAT ppd;

   if (!(pbpi = (PBOOKPAGEINFO)malloc(sizeof(BOOKPAGEINFO)))) goto end;
   memset(pbpi, 0, sizeof(BOOKPAGEINFO));
   if (!(pbpi->pszStatusLine = (PSZ)malloc(256)) ||
       !(pbpi->pszMajorTab = (PSZ)malloc(256)) ||
       !(pbpi->pszMinorTab = (PSZ)malloc(256))) goto end;
   // dati comuni
   pbpi->cb = sizeof(BOOKPAGEINFO);
   pbpi->hmodPageDlg = pnbk->hres;

   // inserisce tutte le pagine definite nell'array
   for (ppd = (PPGDAT)NULL, i = 0; i < pnbk->cpgs; ++i) {
      if (!(ppd = WnbkInitPg(pnbk, pbpi, &pnbi[i], ppd, &cxM, &cxm)))
         goto end;
   } /* endfor */

   // se necessario setta dimensioni tab
   if (fl) {
      WinMapDlgPoints(pnbk->hwnd, &ptl, 1, TRUE);
      WinSendMsg(pnbk->hwnd, BKM_SETDIMENSIONS,
                 MPFROM2SHORT((USHORT)(ptl.x * cxM),
                 (USHORT)(ptl.y + 8)), MPFROMLONG(BKA_MAJORTAB));
      WinSendMsg(pnbk->hwnd, BKM_SETDIMENSIONS,
                 MPFROM2SHORT((USHORT)(ptl.x * cxm),
                 (USHORT)(ptl.y + 6)), MPFROMLONG(BKA_MINORTAB));
   } // end if
   rc = TRUE;

   // liberazione risorse e controllo errori
end:
   if (pbpi) {
      free(pbpi->pszStatusLine);
      free(pbpi->pszMajorTab);
      free(pbpi->pszMinorTab);
      free(pbpi);
   } /* endif */
   if (!rc) WnbkDelPg(pnbk, 0, 0);
   return rc;
}


//==========================================================================\
// inserisce sequenzialmente una pagina controllando la massima lunghezza   |
// delle stringhe usate nelle tab primarie e secondarie                     |
// parametri:                                                               |
// PNBK pnbk: indirizzo struttura dati notebook                             |
// PBOOKPAGEINFO pbpi: indirizzo struttura dati notebook                    |
// PNBINSPG pnbi: indirizzo struttura dati inserzione pagina                |
// PPGDAT pprev: punta alla struttura pagina precedente                     |
// PULONG pcxM: usato per calcolo larghezza major tabs                      |
// PULONG pcxm: usato per calcolo larghezza minor tabs                      |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

PPGDAT APIENTRY WnbkInitPg(PNBK pnbk, PBOOKPAGEINFO pbpi, PNBINSPG pnbi,
                           PPGDAT pprev, PULONG pcxM, PULONG pcxm) {
   PPGDAT ppd;
   pnbi->fs = pnbi->pgn == 1? BKA_MAJOR | BKA_MINOR: BKA_MINOR;
   if (!(ppd = WnbkInsertPg(pnbk, pbpi, pnbi, pprev, NBINS_PGXOFY)))
      return NULL;

   // calcola massima lunghezza stringhe major tab e minor tab
   if (pbpi->cbMajorTab > *pcxM) *pcxM = pbpi->cbMajorTab;
   if (pbpi->cbMinorTab > *pcxm) *pcxm = pbpi->cbMinorTab;
   return ppd;
}


//==========================================================================\
// Alloca una struttura pagedata, la inizializza ed inserisce una pagina    |
// nel notebook. In caso di errore libera la struttura e restituisce NULL   |
// La pagina viene sempre inserita dopo la pagina la cui struttura ppgdata  |
// viene passata come parametro                                             |
// parametri:                                                               |
// PNBK pnbk: indirizzo struttura dati notebook                             |
// PBOOKPAGEINFO pbpi: indirizzo struttura dati notebook                    |
// PNBINSPG pnbi: indirizzo struttura dati inserzione pagina                |
// PPGDAT pprev: punta alla struttura pagina precedente                     |
// ULONG ulpos: la posizione Š relativa a questa pagina (a meno che         |
//              pnbi->fsbka non sia BKA_LAST o BKA_FIRST)                   |
// ULONG flins: flag inserzione: BKA_LAST BKA_FIRST BKA_NEXT BKA_PREV       |
//              pu• anche essere NBIS_PGXOFY che indica di inserire la      |
//              stringa "page x of y" ricavandola da pnbi->idst, pnbi->pgn  |
//              e pnbi->cpg.                                                |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

PPGDAT APIENTRY WnbkInsertPg(PNBK pnbk, PBOOKPAGEINFO pbpi, PNBINSPG pnbi,
                             PPGDAT pprev, ULONG flins) {
   MPARAM prevpg = (MPARAM)(pprev? (pprev == NBINS_ISFIRST? 0: pprev->pgid): 0);
   MPARAM bkfl;
   PPGDAT ppd;
   // alloca dati pagina
   if (!(ppd = (PPGDAT)malloc(sizeof(PGDAT)))) return NULL;
   memset(ppd, 0, sizeof(PGDAT));
   ppd->cb = sizeof(PGDAT);
   // ricava stile pagina da ordine pagina nella tab
   bkfl = MPFROM2SHORT((BKA_COMMON | pnbi->fs),
            (pprev? (pprev == NBINS_ISFIRST? BKA_FIRST: BKA_NEXT): BKA_LAST));
   // memorizza indirizzo dati inserzione pagina
   pbpi->ulPageData = (ULONG)ppd;
   pbpi->pPageDlgCreateParams = ppd;
   // memorizza attributi pagina
   ppd->fs = pnbi->fs;
   // memorizza flag definite dall'utente
   ppd->fl = pnbi->fl;
   // memorizza indirizzo dati definiti dall'utente
   ppd->pv = pnbi->pv;
   // ricava attributi pagina
   pbpi->fl = (pnbi->pgn == 1)? BFA_COMMON | BFA_MAJORTABTEXT: BFA_COMMON;
   // memorizza procedura dialogo
   pbpi->pfnPageDlgProc = (PFN)pnbi->pf;
   // memorizza id dialogo
   pbpi->idPageDlg = pnbi->iddlg;
   // memorizza statusline
   if (pnbi->idst) {
      if (flins & NBINS_PGXOFY) {
         WpageXofY(pnbi->idst, pnbi->pgn, pnbi->cpg, pbpi->pszStatusLine);
      } else {
         WgetStr(pnbi->idst, pbpi->pszStatusLine, 256);
      } /* endif */
   } /* endif */
   // memorizza stringa major tab
   if (pnbi->idmaj) WgetStr(pnbi->idmaj, pbpi->pszMajorTab, 256);
   // memorizza stringa minor tab
   if (pnbi->idmin) WgetStr(pnbi->idmin, pbpi->pszMinorTab, 256);
   // inserisce pagina
   if (!(ppd->pgid = (ULONG)WinSendMsg(pnbk->hwnd, BKM_INSERTPAGE, prevpg, bkfl)))
      goto error;
   // inizializza lunghezza stringhe)
   pbpi->cbStatusLine = strlen(pbpi->pszStatusLine);
   pbpi->cbMajorTab = strlen(pbpi->pszMajorTab);
   pbpi->cbMinorTab = strlen(pbpi->pszMinorTab);
   if (!(BOOL)WinSendMsg(pnbk->hwnd, BKM_SETPAGEINFO,
                         (MPARAM)ppd->pgid, (MPARAM)pbpi))
      goto error;
   // inserisce pagina nella lista:
   // se Š l'unica pagina ne memorizza l'indirizzo nella struttura notebook
   if (!pprev) {
      // i puntatori alla precedente e prossima struttura rimangono NULL
      pnbk->ppd = ppd;
   // se Š la prima pagina nel notebook aggiorna la linked list delle pagine
   } else if (pprev == NBINS_ISFIRST) {
      // il puntatore alla struttura precedente rimane NULL
      pnbk->ppd->pprv = ppd;
      ppd->pnxt = pnbk->ppd;
      pnbk->ppd = ppd;
   // se Š qualsiasi altra pagina
   } else {
      pprev->pnxt->pprv = ppd;
      ppd->pnxt = pprev->pnxt;
      pprev->pnxt = ppd;
      ppd->pprv = pprev;
   } /* endif */
   // incrementa numero pagine
   pnbk->cpgs++;
   // se richiesto memorizza indirizzo allocato
   if (pnbi->pppg) *pnbi->pppg = ppd;
   return ppd;
error:
   free(ppd);
   return NULL;
}


//==========================================================================\
// cancella una o + pagine dal notebook, prima cancella le pagine poi       |
// libera la memoria allocata per i dati della pagina                       |
// parametri:                                                               |
// PNBK pnbk: struttura dati notebook                                       |
// ULONG id: id pagina                                                      |
// ULONG fl: 0 libera tutta la memoria allocata senza spedire msg           |
//           BKA_SINGLE = cancella solo una pagina                          |
//           BKA_TAB = se la pagina corrente Š major tab cancella tutte     |
//                     pagine successive fino a prossima major tab          |
//           BKA_ALL = cancella tutte le pagine                             |
// valore restituito:                                                       |
// BOOL risultato del messaggio                                             |
//==========================================================================/

BOOL APIENTRY WnbkDelPg(PNBK pnbk, ULONG id, ULONG fl) {
   INT i;
   PPGDAT ppd, pnext;
   if (fl & BKA_PAGEORDER) id = WnbkPgn(pnbk, id, TRUE);
   fl &= 0x7;
   if (fl & !WinSendMsg(pnbk->hwnd, BKM_DELETEPAGE, (MPARAM)id, (MPARAM)fl))
      return FALSE;
   // cancella tutte le pagine
   if (!fl || (fl & BKA_ALL)) {
      for (ppd = pnbk->ppd, i = 0; i < pnbk->cpgs; ++i) {
         pnext = ppd->pnxt;
         free(ppd);
         ppd = pnext;
      } /* endfor */
   // cancella solo le pagine incluse nella tab
   } else if (fl & BKA_TAB) {
      // trova struttura PGDAT corrispondente alla pagina corrente
      ppd = (PPGDAT)WnbkPgn(pnbk, id, NBKQ_PGDAT);
      // inizializza fl secondo flag pagina corrente
      fl = (ppd->fs & BKA_MAJOR)? BKA_MAJOR: BKA_MINOR;
      do {
         pnext = ppd->pnxt;
         free(ppd);
         ppd = pnext;
      } while (!(ppd->fs & fl)); /* enddo */
   // cancellazione pagina singola
   } else {
      free((PPGDAT)WnbkPgn(pnbk, id, NBKQ_PGDAT));
   } /* endif */
   return TRUE;
}


//==========================================================================\
// restituisce il numero della pagina corrispondente all'ID specificato     |
// (fl == FALSE) o l'id della pagina inserita alla posizione specificata    |
// (fl == TRUE).                                                            |
// PNBK pnbk: struttura dati notebook                                       |
// ULONG id: numero o id pagina                                             |
// ULONG fl: NBKQ_PGORD restituisce posizione inserzione pagina             |
//           NBKQ_PGID restituisce id pagina                                |
//           NBKQ_PGDAT restituisce indirizzo dati associati con pagina     |
// ULONG id pagina o posizione inserzione pagina secondo flag specificata   |
//       restituisce 0xffffffff in caso di errore                           |
//==========================================================================/

ULONG APIENTRY WnbkPgn(PNBK pnbk, ULONG id, ULONG fl) {
   int i;
   PPGDAT ppd;
   // conversione da numero pagina in id
   if (fl & NBKQ_PGID) {
      // sono state inserite meno pagine rispetto al numero di pagine richiesto
      if (id >= pnbk->cpgs) return NBK_ERROR;
      for (ppd = pnbk->ppd, i = 0; i < id; ++i, ppd = ppd->pnxt);
      if (!ppd) return NBK_ERROR;
      return (fl & NBKQ_PGDAT)? (ULONG)ppd: ppd->pgid;
   // conversione da id pagina in posizione inserzione pagina
   } else {
      for (ppd = pnbk->ppd, i = 0; i < pnbk->cpgs; ++i, ppd = ppd->pnxt)
         if (ppd->pgid == id) return (fl & NBKQ_PGDAT)? (ULONG)ppd: i;
      return NBK_ERROR;
   } /* endif */
}


//==========================================================================\
// calcola la dimensione minima di un notebook in base a quella tipica di   |
// un dialogo che costituisce la sua pagina.                                |
// parametri:                                                               |
// PWDATA pw: struttura dati pagina                                         |
// ULONG cxm: larghezza minima interna pagina                               |
// ULONG cym: altezza minima interna pagina                                 |
// BOOL fl: TRUE/FALSE = espressione dimensioni in pixel/dialog units       |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

BOOL APIENTRY WnbkMinSize(PWDATA pw, ULONG cxm, ULONG cym, BOOL fl) {
   ULONG ul = sizeof(RECTL);
   WRECTL wr = {0, 0, cxm, cym};
   if (fl && !WinMapDlgPoints(pw->hwnd, ((PPOINTL)&wr) + 1, 1, TRUE))
      return FALSE;
   if (!WinSendMsg(pw->hwnd, BKM_CALCPAGERECT, (MPARAM)&wr, MPVOID))
      return FALSE;
   // dimensioni correnti bordo finestra
   pw->sbrd.cx = uGetSval(SV_CXSIZEBORDER);
   pw->sbrd.cy = uGetSval(SV_CYSIZEBORDER);
   pw->smin.cx = wr.cx - wr.x + 2 * pw->sbrd.cx;
   pw->smin.cy = wr.cy - wr.y + 2 * pw->sbrd.cy;
   return TRUE;
}


//==========================================================================\
// trova nella stringa "pagina 1 di 2" i numeri 1 e 2 e li sostituisce      |
// con ulPg e cPgs.                                                         |
// parametri:                                                               |
// ULONG idstr: id stringa "pagina 1 di 2"                                  |
// ULONG ulPg: indice pagina corrente                                       |
// ULONG cPgs: totale pagine                                                |
// PSZ psz: buffer in cui si copia la stringa (256 caratteri)               |
//==========================================================================/

PSZ APIENTRY WpageXofY(ULONG idstr, ULONG ulPg, ULONG cPgs, PSZ psz) {
   CHAR buf[256];
   PSZ p1;
   PSZ p2;
   PSZ p;
   // recupera stringa "pagina 1 di 2"
   WgetStr(idstr, buf, 256);
   // trova posizione caratteri da sostituire 1 e 2
   p1 = strchr(buf, '1');
   p2 = strchr(buf, '2');
   // divide la stringa in 3 parti
   *p1 = *p2 = 0;
   strcpy(psz, buf);       // copia porzione precedente '1'
   catuch2str(psz, ulPg);  // converte ulPg in stringa
   strcat(psz, ++p1);      // copia porzione stringa compresa tra 1 e 2
   catuch2str(psz, cPgs);  // converte cPgs in stringa
   strcat(psz, ++p2);      // copia ultima porzione stringa
   return psz;
}
