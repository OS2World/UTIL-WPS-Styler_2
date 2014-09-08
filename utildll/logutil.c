//==========================================================================\
// logutil.c
//==========================================================================/

#pragma strings(readonly)

#include "common.h"
#include "progress.h"
#include "pmappapi.h"
#include "logutil.h"
#include "api.h"
#include <stdarg.h>


//==========================================================================\
// Alloca struttura dati per scrivere resoconto azioni contemporaneamente   |
// in un file di log e in una finestra.                                     |
// Se la dimensione specificata per il buffer di log in cbbuf Š inferiore a |
// 16 KB viene arrotondata.                                                 |
// Se un file di log Š gi… presente lo cancella.                            |
// parametri:                                                               |
// HWND hwnd: eventuale finestra in cui scrivere dati                       |
// PSZ pfname: nome file di log                                             |
// ULONG cbbuf: dimensione buffer da allocare                               |
// PSZ pdescr: descrizione della procedura di si esegue il log (per esempio:|
//             "Installazione Styler/2". Se minore di 0xffff viene          |
//             interpretato come id di stringa da ricavare dalle risorse    |
// valore restituito:                                                       |
// PLOGFILE plf: indirizzo struttura dati allocata, NULL in caso di errore  |
//==========================================================================/

PLOGFILE APIENTRY logInit(HWND hwnd, PSZ pfname, ULONG cbbuf, PSZ pdescr) {
   PLOGFILE plf;
   FILESTATUS3 fs;
   if (cbbuf < CBMINBUF) cbbuf = CBMINBUF;
   if (!(plf = (PLOGFILE)malloc(sizeof(LOGFILE) - 4 + cbbuf))) return NULL;
   memset(plf, 0, sizeof(LOGFILE) - 4 + cbbuf);
   plf->cbbuf = cbbuf;
   plf->hwnd = hwnd;
   strcpy(plf->fname, pfname);
   if (ioFExists(pfname, NULL)) {
      if (DosQueryPathInfo(pfname, FIL_STANDARD, &fs, CBFINFO))
         goto error;
      if (fs.attrFile & FLFACCLIMIT) {
         fs.attrFile = 0;
         if (DosSetPathInfo(pfname, FIL_STANDARD, &fs, CBFINFO, DSPI_WRTTHRU))
            goto error;
      } /* endif */
      if (DosDelete(pfname)) goto error;
   } /* endif */
   plf->ulstart = uGetSval(DOS_INFO + QSV_MS_COUNT);
   uDateTime(plf->ach, plf->ach + 9);
   plf->ach[8] = plf->ach[17] = 0;
   plf->pcur = pszcpycat(plf->buf, 6," ------- ", plf->ach, " ------- ",
                         plf->ach + 9, " ------- ", "\n");
   if ((ULONG)pdescr < 0xffff) {
      PSZ psz = plf->pcur;
      plf->pcur += WgetStr((ULONG)pdescr, psz, 256);
      pdescr = psz;
   } else {
      plf->pcur = pszcpy(plf->pcur, pdescr);
   } /* endif */
   if (plf->hwnd) WinSetWindowText(plf->hwnd, pdescr);
   strcpy(plf->pcur++, "\n");
   return plf;
error:
   free(plf);
   return NULL;
}


//==========================================================================\
// Scrive messaggio termine operazione loggata.                             |
// Calcola tempo trascorso e lo scrive.                                     |
// Libera struttura dati file log.                                          |
// parametri:                                                               |
// PLOGFILE plf: struttura dati file log                                    |
// PSZ pendstr: stringa descrizione fine operazione (id stringa o stringa)  |
// PSZ psztime: stringa "Elapsed time: 11 hour, 22 minutes and 33 seconds"  |
// valore restituito:                                                       |
// PLOGFILE plf: indirizzo struttura dati allocata, NULL in caso di errore  |
//==========================================================================/

BOOL APIENTRY logEnd(PLOGFILE plf, PSZ pendstr, PSZ psztime) {
   ULONG ultime = uGetSval(DOS_INFO + QSV_MS_COUNT);
   ULONG sec;
   ULONG min;
   ULONG hour;
   PSZ p, pcur;
   BOOL rc = FALSE;
   // calcola tempo trascorso controllando che nel frattempo il contatore
   // del tempo di boot non sia stato azzerato
   ultime = (ultime < plf->ulstart)? 0xffffffff - plf->ulstart + ultime:
                                     ultime - plf->ulstart;
   ultime /= 1000;
   if (!ultime) ultime = 1; // scrive almeno 1 secondo
   sec = ultime % 60;
   min = (ultime / 60) % 60;
   hour = ultime / 3600;
   // copia stringa descrizione fine operazione loggata
   if (!(pcur = logCat(plf->ach, pendstr, CBLOGBUF)) ||
       !logCat(pcur, psztime, (ULONG)(pcur - plf->ach)) ||
       !(p = strchr(pcur, '1'))) 
      goto end;
   *p = (hour > 10)? hour / 10 + '0': ' ';
   *++p = hour % 10 + '0';
   if (!(p = strchr(pcur, '2'))) goto end;
   *p = (min > 10)? min / 10 + '0': ' ';
   *++p = min % 10 + '0';
   if (!(p = strchr(pcur, '3'))) goto end;
   *p = (sec > 10)? sec / 10 + '0': ' ';
   *++p = sec % 10 + '0';
   if (plf->hwnd) WinSetWindowText(plf->hwnd, plf->ach);
   if ((strlen(plf->ach) + 1) > (plf->cbbuf - (ULONG)(plf->pcur - plf->buf)) &&
       !ioFlushBuf(plf->fname, plf->buf, &plf->pcur))
      goto end;
   plf->pcur = pszcpy(pszcpy(plf->pcur, plf->ach), "\n");
   if (!ioFlushBuf(plf->fname, plf->buf, &plf->pcur)) goto end;
   rc = TRUE;
end:
   free(plf);
   return rc;
}


//==========================================================================\
// Concatena un numero variabile di stringhe nel file di log.               |
// Se gli indirizzi delle stringhe sono minori di 0xffff, le stringhe       |
// vengono interpretate come id di stringhe da ricavare dalle risorse.      |
// setta il testo del controllo hwnd alla stringa composta                  |
// aggiunge un carattere di nuova linea e appende la stringa al buffer per  |
// la scrittura del file di Log incrementandone il puntatore al cursore     |
// parametri:                                                               |
// PLOGFILE plf: struttura descrizione file log e finestra log              |
// ULONG fl: se TRUE viene considerato un messaggio di errore.              |
//           In questo caso si assume che plf->ach contenga la descrizione  |
//           dell'operazione fallita, per cui a plf->ach viene concatenato  |
//           "\nFailed" ed il buffer completo viene usato per mostrare      |
//           il messaggio di errore tramite la finestra di progresso.       |
//           Nel buffer del file invece viene scritto solo "Failed\n".      |
// INT carg: numero stringhe (o id stringhe) presenti come argomenti        |
// ... : numero variabile di char* o id stringhe ricavabili da dll risorse  |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

BOOL APIENTRY logAdd(PLOGFILE plf, ULONG fl, INT carg, ...) {
   va_list ap;
   PSZ p, parg, ptab;
   INT i;
   BOOL rc = FALSE;
   ULONG cbbuf = CBLOGBUF;
   // controlla validit… plf
   if (!plf) return TRUE;
   p = plf->ach;
   // se errore inserisce all'inizio del buffer "Error\t" e appende alla
   // fine dello stesso "Error"
   if (fl) {
      ULONG cb = strlen(plf->ach);
      cbbuf /= 2;                   // dimezza dimensione disponibilit… buffer
      if (cb > cbbuf) return FALSE; // overflow
      // copia prima parte del messaggio alla fine di se stesso
      memmove(plf->ach + cbbuf, plf->ach, cbbuf);
   } /* endif */
   va_start(ap, carg);
   // concatena stringhe nel buffer temporaneo controllando l'overflow
   // se errore ci sar… una sola stringa: Errore!
   while (carg--) {
      if (!(p = logCat(p, (parg = va_arg(ap, PSZ)),
                       cbbuf - (ULONG)(p - plf->ach))))
         goto end;
   } /* endwhile */
   // se specificato handle finestra setta testo dal buffer temporaneo
   if (plf->hwnd) {
      if (fl) {
         ptab = p;     // memorizza posizione in cui verr… concatenata tab
         // concatena "\t"
         if (!( p = logCat(p, "\t", cbbuf - (ULONG)(p - plf->ach)))) goto end;
         // concatena porzione precedente messaggio
         memmove(p, plf->ach + cbbuf, cbbuf);
         p = memchr(p, 0, cbbuf);
         // concatena \n e stringa di errore
         if (!(p = logCat(p, "\n", cbbuf * 2 - (ULONG)(p - plf->ach))))
            goto end;
         if (!(p = logCat(p, parg, cbbuf * 2 - (ULONG)(p - plf->ach))))
            goto end;
         WinSendMsg(plf->hwnd, PRGSM_MSG, (MPARAM)plf->ach, (MPARAM)PMPRNT_ERROR);
         // nel buffer del file scrive solo la stringa di errore
         *ptab = 0;
         p = ptab;
      } else {
         WinSetWindowText(plf->hwnd, plf->ach);
      } /* endif */
   } /* endif */
   // controlla che la stringa possa entrare nel buffer di scrittura file
   if ((ULONG)(p - plf->ach + 1) >
       (plf->cbbuf - (ULONG)(plf->pcur - plf->buf)) &&
       !ioFlushBuf(plf->fname, plf->buf, &plf->pcur))
      goto end;
   // appende contenuto buffer temporaneo al buffer aggiungendo '\n'
   plf->pcur = pszcpy(pszcpy(plf->pcur, plf->ach), "\n");
end:
   va_end(ap);
   return rc;
}


//==========================================================================\
// copia pstr in pbuf controllando se Š una vera stringa o Š l'id di una    |
// risorsa e controllando l'overflow del buffer.                            |
// se la stringa Š NULL restituisce l'indirizzo del buffer di partenza      |
// se la lunghezza della stringa da copiare supera la dimensione disponibile|
// nel buffer restituisce NULL                                              |
// parametri:                                                               |
// PSZ pbuf: indirizzo buffer                                               |
// PSZ pstr: stringa da copiare                                             |
// ULONG cb: spazio disponibile nel buffer                                  |
// valore restituito:                                                       |
// PSZ fine buffer per successiva concatenazione                            |
//==========================================================================/

PSZ APIENTRY logCat(PSZ pbuf, PSZ pstr, ULONG cb) {
   ULONG cbstr;
   if (!pstr) return pbuf;
   // se id stringa la ricava dalle risorse dell'applicazione
   if ((ULONG)pstr < 0xffff)
      return pbuf + WgetStr((ULONG)pstr, pbuf, cb);
   // altrimenti controlla lunghezza stringa e la copia
   if ((cbstr = strlen(pstr)) > cb) return NULL;
   return strcpy(pbuf, pstr) + cbstr;
}
