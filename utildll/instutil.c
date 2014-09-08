//==========================================================================\
// prfutil.c:
//==========================================================================/

#pragma strings(readonly)

#include "common.h"
#include "pmappapi.h"
#include "progress.h"
#include "logutil.h"
#include "instutil.h"
#include "api.h"


//==========================================================================\
// Controlla se l'ID di un oggetto Š registrato in OS2.INI.                 |
// parametri:                                                               |
// PSZ pobj: una serie di stringhe di id di oggetti terminate da 0 e        |
//           concatenate. L'ultima stringa Š una stringa vuota              |
// valore restituito:                                                       |
// BOOL rc: TRUE: almeno uno degli oggetti in pobj Š presente               |
//==========================================================================/

BOOL APIENTRY prfChkObj(PSZ pobj) {
   BOOL rc = FALSE;
   PSZ psz = malloc(0x10000);
   PSZ pcobj, p;
   ULONG cb, cobj;
   if (!psz) return FALSE;
   if (!PrfQueryProfileString(HINI_USER, "PM_Workplace:Location",
                              NULL, NULL, psz, 0x10000))
      goto end;
   for (p = psz; 0 != (cb = strlen(p)); p += cb + 1) {
      for (pcobj = pobj; 0 != (cobj = strlen(pcobj)); pcobj += cobj + 1)
         if (!memcmp(p, pcobj, cobj)) goto found;
   } /* endfor */
   goto end;
found:
   rc = TRUE;
end:
   free(psz);
   return rc;
}


//==========================================================================\
// Cancella una substringa da un valore memorizzato in un file INI.         |
// La ricerca non Š case insensitive                                        |
// parametri:                                                               |
// HINI hini: handle file ini                                               |
// PSZ app: nome applicazione                                               |
// PSZ key: nome chiave                                                     |
// PSZ str: stringa da rimuovere (la stringa pu• contenere '\0')            |
// ULONG cbstr: lunghezza della stringa                                     |
// PSZ sep: stringa di separatori ammessi                                   |
// ULONG cbsep: lunghezza stringa separatori (permette di usare 0)          |
// ULONG fl: flag che indica se il valore nel file INI Š terminato da 0     |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

BOOL APIENTRY prfdelstr(HINI hini, PSZ app, PSZ key, PSZ str,
                        ULONG cbstr, PSZ sep, ULONG cbsep, ULONG fl) {
   ULONG cb;
   PBYTE pdata = getPrfData(hini, app, key, &cb);
   PBYTE p;
   BOOL rc;
   // legge i dati dal file ini
   if (pdata == GPD_ERROR) return PRFINFERROR;
   // se l'applicazione o la key non sono presenti nel file INI
   // o se la substringa non Š inclusa nel valore registrato
   // non c'e' bisogno di cancellare niente e viene restituito TRUE
   if (fl) cb--; // esclude 0 terminazione
   if (!pdata ||
       !(p = memfind(pdata, cb, str, cbstr, sep, cbsep)))
      return TRUE;
   // cancella la stringa ed eventuali separatori in eccesso
   cb = memdel(pdata, cb, p, cbstr, sep, cbsep);
   if (!cb) {          // se nel valore era presente solo str cancella voce
      free(pdata);     // dal file ini
      pdata = NULL;
   } else if (fl) {    // altrimenti se necessario aggiunge 0 terminazione
      pdata[cb++] = 0;
   } /* endif */
   // aggiorna il fine INI
   rc = PrfWriteProfileData(hini, app, key, pdata, cb);
   if (pdata) free(pdata);
   return rc;
}


//==========================================================================\
// Inserisce una stringa tra le altre stringhe presenti nei dati memorizzati|
// in un file INI. Se la flag di inserzione non Š PRFINSTART o PRFINEND si  |
// limita a riportare se la stringa Š presente o meno.                      |
// La ricerca non Š case insensitive                                        |
// parametri:                                                               |
// HINI hini: handle file ini                                               |
// PSZ app: nome applicazione                                               |
// PSZ key: nome chiave                                                     |
// PSZ str: stringa da inserire ricercare (la stringa pu• contenere '\0')   |
// ULONG cbstr: lunghezza della stringa                                     |
// ULONG cbfind: lunghezza stringa da ricercare                             |
// PSZ sep: stringa di separatori ammessi                                   |
// ULONG cbsep: lunghezza stringa separatori (permette di usare 0)          |
// ULONG flins: se TRUE inserisce la stringa nella posizione corrispondente |
//         PRFINCHECK     0x0000    verifica solo se stringa Š presente     |
//         PRFINSTART     0x0001    inserisce stringa a inizio              |
//         PRFINEND       0x0002    inserisce stringa alla fine             |
//         PRFINZEROEND   0x0004    i dati devono essere terminati da zero  |
//         PRFINNOTFOUND  0         stringa non trovata                     |
//         PRFINFOUND     1         stringa presente o inserzione riuscita  |
//         PRFINFERROR    -1        errore                                  |
//         PRFINFOVRFLW   -2        inserzione impossibile per overflow     |
//             a flins (1 = inizio, 2 = fine), se FALSE si limita a         |
//             riportare la presenza della stringa.                         |
// valore restituito:                                                       |
// INT rc: 1 = stringa presente/inserita, 0 = stringa assente, -1 = errore  |
//==========================================================================/

INT APIENTRY prfinsstr(HINI hini, PSZ app, PSZ key, PSZ str,
                       ULONG cbstr, PSZ sep, ULONG cbsep, ULONG flins) {
   ULONG cb;
   PBYTE pdata = getPrfData(hini, app, key, &cb);
   ULONG flsep = sep? 1: 0;
   ULONG flend = (flins & PRFINZEROEND)? 1: 0; // flag terminazione con 0
   INT rc = PRFINNOTFOUND;
   // legge i dati dal file ini
   if (pdata == GPD_ERROR) return PRFINFERROR;
   // se app o key sono assenti li aggiunge
   if (!pdata) {
      if (flins & (PRFINSTART | PRFINEND)) {
         pdata = str;
         cb = 0;
         flsep = flend;
         goto insert;
      } /* endif */
      return PRFINNOTFOUND;
   } /* endif */
   // se stringa gi… presente restituisce 1
   if (memfind(pdata, cb - flend, str, cbstr, sep, cbsep)) {
      free(pdata);
      return PRFINFOUND;
   } /* endif */
   // inserisce la stringa:
   if (flins & (PRFINSTART | PRFINEND)) {
      // se l'inserzione della stringa provoca l'overflow del buffer pdata
      // restituisce -1
      if ((cb + cbstr + flsep) > 0x10000) {
         free(pdata);
         return PRFINFOVRFLW;
      } /* endif */
      if (flins & PRFINSTART) {      // inserzione iniziale
         memmove(pdata + cbstr + flsep, pdata, cb);
         memcpy(pdata, str, cbstr);
         if (flsep) *(pdata + cbstr) = *sep;
      } else {                      // inserzione finale
         if (flsep) *(pdata + cb - flend) = *sep;
         memcpy(pdata + cb + flsep - flend, str, cbstr);
         if (flend) *(pdata + cb + flsep + cbstr) = 0;
      } /* endif */
insert:
      if (!PrfWriteProfileData(hini, app, key, pdata, cb + cbstr + flsep))
         rc = PRFINFERROR;
      rc = PRFINFOUND;
   } /* endif */
   if (cb) free(pdata);
   return rc;
}


//==========================================================================\
// i path membri della struttura FILECPY devono terminare con lo slash!!!   |
// il membro fl della struttura FILECPY indica:                             |
// - se sovrascrivere sempre i file                                         |
// - se chiedere conferma per sovrascrivere i file + nuovi                  |
// - se effettuare copia di backup dei file da sostituire                   |
// - se il file Š un modulo che deve essere sbloccato                       |
// - se la copia del file deve essere deferita al riavvio (da implementare) |
// parametri:                                                               |
// PFILECPY pfc: altri dati necessari per copiare file                      |
// PSZ pszfile: nome file da copiare                                        |
// PSZ psznew: se non NULL indica che il nuovo nome del file copiato        |
// valore restituito:                                                       |
// INT rc: -1 = errore critico, 0 = errore non critico, 1 = successo        |
//         -2 = interruzione da finestra progresso                          |
//==========================================================================/

INT APIENTRY insfilecpy(PFILECPY pfc, PSZ pszfile, PSZ psznew) {
   APIRET rc;
   INT irc;
   // se non Š specificato nome file destinazione usa nome file origine
   if (!psznew) psznew = pszfile;
   // costruisce nome file origine e destinazione:
   strcpy(pfc->achsrc + pfc->cbsrc, pszfile);
   strcpy(pfc->achtrg + pfc->cbtrg, psznew);
   // scrive operazione in corso: copying "filename" to "target"
   logAdd(pfc->plf, 0, 4, (PSZ)pfc->msg, pszfile, (PSZ)pfc->to, pfc->achtrg);
   // se Š richiamato da instreecpy e la flag di abort Š attiva si limita
   // a scrivere il messaggio di errore nel log
   if (pfc->fl & FCPYF_ABORT) goto error;
   // controlla esistenza file con stesso nome nel path di destinazione
   rc = DosQueryPathInfo(pfc->achtrg, FIL_STANDARD, &pfc->fst, CBFINFO);
   if (rc && rc != 2) goto error;       // errore DosQueryPathInfo()
   // copia incondizionata o target file non preesistente
   if (rc || pfc->fl & FCPYF_OVWRT) {
      if (!fcpy(pfc, !rc)) goto error;
   // sovrascrittura con conferma (sempre o con file + nuovi)
   } else if (pfc->fl & (FCPYF_OVWCONF | FCPYF_OVWCONFNEW)) {
      if (DosQueryPathInfo(pfc->achsrc, FIL_STANDARD, &pfc->fss, CBFINFO))
         goto error;
      // se richiesta conferma sovrascrittura file pi— nuovi e il file Š
      // pi— vecchio lo sovrascrive altrimenti lo sovrascrive dietro conferma
      if (pfc->fl & FCPYF_OVWCONFNEW &&
          ioFilOlder(&pfc->fss.fdateLastWrite, &pfc->fss.ftimeLastWrite,
                     &pfc->fst.fdateLastWrite, &pfc->fst.ftimeLastWrite) >= 0) {
         if (!fcpy(pfc, TRUE)) goto error;
      } else {
         if (!insconfirm(pfc)) goto error;
      } /* endif */
   } /* endif */
   irc = 1;
   goto end;
error:
   pfc->achtrg[pfc->cbtrg] = 0;
   irc = (pfc->fl & FCPYF_STOPONERR)? -1: 0;
   logAdd(pfc->plf, irc, 1, (PSZ)pfc->err);
   if (irc) return irc;
end:
   if (pfc->pidx && Wincrease(pfc->pidx, pfc->hprgrs, pfc->csteps)) return -2;
   return irc;
}


//==========================================================================\
// copia un file eventualmente resettando gli attributi del file target se
// esiste gi…. In caso di errore mostra messaggio.
// parametri:                                                               |
// valore restituito:                                                       |
//==========================================================================/

BOOL fcpy(PFILECPY pfc, BOOL fl) {
   APIRET rc;
   // se necessario resetta attributi file da sovrascrivere
   if (fl && (pfc->fss.attrFile & FLFACCLIMIT)) {
      pfc->fss.attrFile = 0;
      rc = DosSetPathInfo(pfc->achtrg, FIL_STANDARD, &pfc->fst,
                          CBFINFO, DSPI_WRTTHRU);
      if (rc == 32) goto replmod;
      if (rc) return FALSE;
   } /* endif */
   rc = DosCopy(pfc->achsrc, pfc->achtrg, DCPY_EXISTING);
replmod:
   if (rc == 32 && (pfc->fl & FCPYF_UNLOCK)) {
      if (DosReplaceModule(pfc->achtrg, pfc->achsrc, NULL)) return FALSE;
   } else if (rc) {
      return FALSE;
   } /* endif */
   return TRUE;
}


//==========================================================================\
// Cancella un file sbloccandolo se necessario. Se il file non esiste       |
// restituisce TRUE (successo)                                              |
// parametri:                                                               |
// PFILEDEL pfd: struttura dati file da cancellare                          |
// PSZ pszfile: nome file da cancellare                                     |
// valore restituito:                                                       |
// INT rc: -1 = errore critico, 0 = errore non critico, 1 = successo        |
//         -2 = interruzione da finestra progresso                          |
//==========================================================================/

INT APIENTRY insfiledel(PFILEDEL pfd, PSZ pszfile) {
   APIRET rc;
   INT irc;
   // costruisce nome completo file da cancellare
   strcpy(pfd->path + pfd->cbpath, pszfile);
   // messaggio di log
   logAdd(pfd->plf, 0, 2, (PSZ)pfd->msg, pfd->path);
   // controlla esistenza file da cancellare
   rc = DosQueryPathInfo(pfd->path, FIL_STANDARD, &pfd->fs, CBFINFO);
   // se file not found o path not found restituisce TRUE
   if (!rc) {
      // se file readonly, sytem o hidden resetta attributi
      if (pfd->fs.attrFile & FLFACCLIMIT) {
         pfd->fs.attrFile = 0;
         rc = DosSetPathInfo(pfd->path, FIL_STANDARD, &pfd->fs,
                             CBFINFO, DSPI_WRTTHRU);
         if (rc == 32) goto unlock;
         if (rc) goto error;
      } /* endif */
      rc = DosDelete(pfd->path);
      if (rc == 32) goto unlock;
      if (rc) goto error;
   } else if (rc != 2 && rc != 3) {
      goto error;
   } /* endif */
   irc = 1;
   goto end;
unlock:
   if (!DosReplaceModule(pfd->path, NULL, NULL) && !DosDelete(pfd->path))
   irc = 1;
   goto end;
error:
   irc = pfd->stop? -1: 0;
   logAdd(pfd->plf, irc, 1, (PSZ)pfd->err);
   if (irc) return irc;
end:
   if (pfd->pidx && Wincrease(pfd->pidx, pfd->hprgrs, pfd->csteps)) return -2;
   return irc;
}


//==========================================================================\
// copia un albero di file creando la directory di partenza se non esiste   |
// parametri:                                                               |
// PTREECPY ptc: struttura dati tree da copiare                             |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore critico o interruzione             |
//==========================================================================/

BOOL APIENTRY instreecpy(PTREECPY ptc) {
   PROCESSTREEDATA pt;
   ULONG cb = strlen(ptc->pdir);
   BOOL rc;
   memset(&pt, 0, sizeof(pt));
   // aggiunge nome directory base albero al path sorgente e target
   strcpy(pszcpy(ptc->fc.achsrc + ptc->fc.cbsrc, ptc->pdir), "\\");
   strcpy(pszcpy(ptc->fc.achtrg + ptc->fc.cbtrg, ptc->pdir), "\\");
   // log
   logAdd(ptc->fc.plf, 0, 2, (PSZ)ptc->msg, (PSZ)ptc->fc.achtrg);
   // se necessario crea directory target
   if (!ioMakePath(ptc->fc.achtrg)) {
      if (ptc->fc.fl & FCPYF_STOPONERR) goto error;
      // essendo fallita la creazione della base dell'albero nella elaborazione
      // successiva si limita a riportare errore
      ptc->fc.fl |= FCPYF_ABORT;
      ptc->cerr++;
   } /* endif */
   if (ptc->fc.pidx && Wincrease(ptc->fc.pidx, ptc->fc.hprgrs, ptc->fc.csteps))
      return FALSE;
   // aggiorna lunghezze path
   ptc->fc.cbsrc += cb + 1;
   ptc->fc.cbtrg += cb + 1;
   // inizializza struttura PROCESSTREEDATA
   strcpy(pt.achPath, ptc->fc.achsrc);
   strcpy(pt.achPath + ptc->fc.cbsrc, "*");
   if (ptc->pmask) strcpy(pt.achMask, ptc->pmask);
   pt.flfind = ptc->fl;
   pt.pdata = ptc;
   pt.pffile = tcpy;
   pt.pfdirpre = tcdirpre;
   pt.pfdirpost = tcdirpost;
   // copia l'albero (l'eventuale errore Š mostrato durante l'elaborazione
   if (ioProcessTree(&pt) < 0) return FALSE;      // di ioProcessTree()
   // ripristina path originali sorgente e target
   ptc->fc.achsrc[(ptc->fc.cbsrc -= cb + 1)] = 0;
   ptc->fc.achtrg[(ptc->fc.cbtrg -= cb + 1)] = 0;
end:
   return TRUE;
error:
   logAdd(ptc->fc.plf, (ptc->fc.fl & FCPYF_STOPONERR), 1, (PSZ)ptc->err);
   return FALSE;
}


//==========================================================================\
// funzione chiamata da instreecpy per copiare i file                       |
// parametri:                                                               |
// valore restituito:                                                       |
//==========================================================================/

LONG tcpy(PPROCESSTREEDATA pptd) {
   INT ir;
   if (!(ir = insfilecpy(MKPCF(pptd->pdata), pptd->fBuf.achName, NULL)))
      MKPCT(pptd->pdata)->cerr++;
   return ir;
}


//==========================================================================\
// funzione usata da instreecpy() per creare directory e modificare         |
// membri struttura FILECPY                                                 |
// parametri:                                                               |
// valore restituito:                                                       |
//==========================================================================/

LONG tcdirpre(PPROCESSTREEDATA pptd) {
   INT irc = 0;
   // appende al nome del path target quello della dir corrente
   strcpy(MKPCF(pptd->pdata)->achtrg + MKPCF(pptd->pdata)->cbtrg,
          pptd->fBuf.achName);
   // log
   logAdd(MKPCF(pptd->pdata)->plf, 0, 2, (PSZ)MKPCT(pptd->pdata)->msg,
          MKPCF(pptd->pdata)->achtrg);
   // se flag abort attiva si limita a scrivere il messaggio di errore nel log
   if (MKPCF(pptd->pdata)->fl & FCPYF_ABORT) goto error;
   // se non riesce a creare path setta memorizza flag attuale, setta flag
   // abort e procede
   if (!ioMakePath(MKPCF(pptd->pdata)->achtrg)) {
      if (MKPCF(pptd->pdata)->fl & FCPYF_STOPONERR) goto error;
      MKPCF(pptd->pdata)->fl |= FCPYF_ABORT;
      irc = -1;
      MKPCT(pptd->pdata)->cerr++;
   } /* endif */
   // appende al nome del path sorgente quello della dir corrente
   strcpy(MKPCF(pptd->pdata)->achsrc + MKPCF(pptd->pdata)->cbsrc,
          pptd->fBuf.achName);
   // aggiunge ad entrambi lo slash finale e aggiorna lunghezze path
   strcpy(MKPCF(pptd->pdata)->achsrc +
          (MKPCF(pptd->pdata)->cbsrc += pptd->fBuf.cchName), "\\");
   MKPCF(pptd->pdata)->cbsrc++;
   strcpy(MKPCF(pptd->pdata)->achtrg +
          (MKPCF(pptd->pdata)->cbtrg += pptd->fBuf.cchName), "\\");
   MKPCF(pptd->pdata)->cbtrg++;
   // spedisce msg incremento:
   if (MKPCF(pptd->pdata)->pidx &&
       Wincrease(MKPCF(pptd->pdata)->pidx,
                 MKPCF(pptd->pdata)->hprgrs, MKPCF(pptd->pdata)->csteps))
      return -1;
   // se errore precedente ristora flag FILECPY e va a error
   if (irc) {
      MKPCF(pptd->pdata)->fl &= ~FCPYF_ABORT;
      goto error;
   } /* endif */
   return 1;
error:
   irc = (MKPCF(pptd->pdata)->fl & FCPYF_STOPONERR)? -1: 0;
   logAdd(MKPCF(pptd->pdata)->plf, irc, 1, (PSZ)MKPCT(pptd->pdata)->err);
   return irc;
}


//==========================================================================\
// funzione usata da instreecpy() per ripristinare path alla terminazione   |
// dell'elaborazione di un sottoalbero                                      |
// parametri:                                                               |
// valore restituito:                                                       |
//==========================================================================/

LONG tcdirpost(PPROCESSTREEDATA pptd) {
   PSZ pend;
   // cancella slash finale da path sorgente
   MKPCF(pptd->pdata)->achsrc[--MKPCF(pptd->pdata)->cbsrc] = 0;
   // accorcia a precedente slash
   pend = ioFNameFromPath(MKPCF(pptd->pdata)->achsrc);
   *pend = 0;
   // ricalcola lunghezza stringa
   MKPCF(pptd->pdata)->cbsrc = (ULONG)(pend - MKPCF(pptd->pdata)->achsrc);
   // cancella slash finale da path target
   MKPCF(pptd->pdata)->achtrg[--MKPCF(pptd->pdata)->cbtrg] = 0;
   // accorcia a precedente slash
   pend = ioFNameFromPath(MKPCF(pptd->pdata)->achtrg);
   *pend = 0;
   // ricalcola lunghezza stringa
   MKPCF(pptd->pdata)->cbtrg = (ULONG)(pend - MKPCF(pptd->pdata)->achtrg);
   return 0;
}



//==========================================================================\
// rimuove un intero albero di file e directory                             |
// parametri:                                                               |
// PTREEDEL ptd: struttura dati albero da cancellare                        |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
// se la flag di errore non critico Š attiva FALSE sta per errore non critico
//==========================================================================/

BOOL APIENTRY instreedel(PTREEDEL ptd) {
   PROCESSTREEDATA pt;
   ULONG cb = strlen(ptd->pdir);
   memset(&pt, 0, sizeof(pt));
   // aggiunge nome directory base albero al path
   strcpy(pszcpy(ptd->fd.path + ptd->fd.cbpath, ptd->pdir), "\\");
   // aggiorna lunghezza path
   ptd->fd.cbpath += cb + 1;
   // inizializza struttura PROCESSTREEDATA
   strcpy(pt.achPath, ptd->fd.path);
   strcpy(pt.achPath + ptd->fd.cbpath, "*");
   if (ptd->pmask) strcpy(pt.achMask, ptd->pmask);
   pt.flfind = ptd->fl;
   pt.pdata = ptd;
   pt.pffile = tddel;
   pt.pfdirpre = tddirpre;
   pt.pfdirpost = tddirpost;
   // cancella l'albero
   if ((ioProcessTree(&pt) < 0) && ptd->fd.stop)
      return FALSE;
   // cancella slash finale
   pt.achPath[ptd->fd.cbpath - 1] = 0;
   // LOG
   logAdd(ptd->fd.plf, 0, 2, (PSZ)ptd->msg, pt.achPath);
   // rimuove directory contenente il tree
   if (DosDeleteDir(pt.achPath)) {
      if (ptd->fd.stop) return FALSE;
      ptd->cerr++;
      cb = 0; // usato come flag per indicare che deve scrivere msg errore
   } /* endif */
   if (ptd->fd.pidx && Wincrease(ptd->fd.pidx, ptd->fd.hprgrs, ptd->fd.csteps))
      return FALSE;
   if (!cb) goto error;
   return TRUE;
error:
   logAdd(ptd->fd.plf, ptd->fd.stop, 1, (PSZ)ptd->err);
   return FALSE;
}


//==========================================================================|
// cancella file e spedisce msg incremento dialogo progresso                |
// parametri:                                                               |
// valore restituito:                                                       |
//==========================================================================/

LONG tddel(PPROCESSTREEDATA pptd) {
   INT ir;
   // se errore non critico incrementa contatore errori
   if (!(ir = insfiledel(MKPDF(pptd->pdata), pptd->fBuf.achName)))
      MKPDT(pptd->pdata)->cerr++;
   return ir;
}


//==========================================================================\
// aggiunge nome directory al membro path della struttura FILEDEL           |
// parametri:                                                               |
// valore restituito:                                                       |
//==========================================================================/

LONG tddirpre(PPROCESSTREEDATA pptd) {
   // appende al nome del path quello della dir corrente seguito da slash
   strcpy(pszcpy(MKPDF(pptd->pdata)->path + MKPDF(pptd->pdata)->cbpath,
                 pptd->fBuf.achName), "\\");
   // aggiorna lunghezza path
   MKPDF(pptd->pdata)->cbpath += pptd->fBuf.cchName + 1;
   return 0;
}


//==========================================================================\
// rimuove directory e ne cancella il nome dal membro path                  |
// parametri:                                                               |
// valore restituito:                                                       |
//==========================================================================/

LONG tddirpost(PPROCESSTREEDATA pptd) {
   INT irc = 1;
   PSZ pend;
   // cancella slash finale da path
   MKPDF(pptd->pdata)->path[--MKPDF(pptd->pdata)->cbpath] = 0;
   // log
   logAdd(MKPDF(pptd->pdata)->plf, 0, 2, (PSZ)MKPDT(pptd->pdata)->msg,
          MKPDF(pptd->pdata)->path);
   if (DosDeleteDir(MKPDF(pptd->pdata)->path)) {
      if (MKPDF(pptd->pdata)->stop) goto error;
      MKPDT(pptd->pdata)->cerr++;
      irc = 0;
   } /* endif */
   // accorcia a precedente slash
   pend = ioFNameFromPath(MKPDF(pptd->pdata)->path);
   *pend = 0;
   // ricalcola lunghezza stringa
   MKPDF(pptd->pdata)->cbpath = (ULONG)(pend - MKPDF(pptd->pdata)->path);
   // spedisce msg incremento:
   if (MKPDF(pptd->pdata)->pidx &&
       Wincrease(MKPDF(pptd->pdata)->pidx,
                 MKPDF(pptd->pdata)->hprgrs, MKPDF(pptd->pdata)->csteps))
      return -1;
   if (irc) return 1;
error:
   irc = (MKPDF(pptd->pdata)->stop)? -1 : 0;
   logAdd(MKPDF(pptd->pdata)->plf, irc, 1, (PSZ)MKPDT(pptd->pdata)->err);
   return irc;
}


//==========================================================================\
// costruisce la richiesta di conferma di sovrascrittura del file inserendo
// il nome del file tra il titolo della finestra di popup e il testo della
// stessa
// parametri:                                                               |
// PFILECPY pfc: struttura dati copia file
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = yes e copiato con successo o no overwrite / errore
//==========================================================================/

BOOL insconfirm(PFILECPY pfc) {
   ULONG ul;
   PSZ psz;
   if (!(ul = WgetStr(pfc->confirm, pfc->plf->ach + 3840, 256)) ||
       !(psz = strchr(pfc->plf->ach + 3840, '\t'))) {
      // in caso di errore lettura stringa continua comunque
      *pfc->plf->ach = 0;
   } else {
      // concatena titolo, nomefile, richiesta conferma sovrascrittura
      *psz = 0;
      pszcpycat(pfc->plf->ach, 4, pfc->plf->ach + 3840,
                "\t", pfc->achtrg, ++psz);
   } /* endif */
   if (WinSendMsg(pfc->plf->hwnd, PRGSM_MSG,
                  (MPARAM)pfc->plf->ach, (MPARAM)PMPRNT_QUERY)) {
      if (!fcpy(pfc, TRUE)) return FALSE;
   } /* endif */
   return TRUE;
}


//==========================================================================\
// Crea un oggetto scrivendo il resoconto dell'operazione nell'eventuale    |
// file di log ed incrementando l'eventuale finestra di progresso.          |
// In caso di errore se il membro stop Š TRUE restituisce FALSE altrimenti  |
// incrementa il contatore degli errori e restituisce TRUE                  |
// parametri:                                                               |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/interruzione o errore critico             |
//==========================================================================/

BOOL APIENTRY insmakeobj(PCREATOBJ pco) {
   PSZ path;
   ULONG cb;
   // ricava titolo oggetto
   WgetStr(pco->po->ititle, pco->title, 256);
   // scrive nel log l'operazione in corso
   logAdd(pco->plf, 0, 5, (PSZ)pco->msg, (PSZ)pco->title,
          " (", pco->po->pclass, ")");
   // compone stringa di setup
   strcpy(pco->setup, pco->po->psetup);
   // se presente tab, inserisce al suo posto path di installazione
   cb = strlen(pco->path);
   for (path = pco->setup; (NULL != (path = strchr(path, '\t')));) {
      memmove(path + cb, path + 1, strlen(path));
      memcpy(path, pco->path, cb);
   } /* endif */
   if (!WinCreateObject(pco->po->pclass, pco->title, pco->setup,
                        pco->po->ploc, CO_REPLACEIFEXISTS)) {
      logAdd(pco->plf, 0, 1, pco->err); // errore non critico
      if (pco->stop) return FALSE;
      pco->cerr++;
   } /* endif */
   if (pco->pidx && Wincrease(pco->pidx, pco->hprgrs, pco->csteps))
      return FALSE;
   return TRUE;
}
