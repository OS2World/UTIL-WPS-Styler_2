//==========================================================================\
// strutil.c  (c) 1998-1999 * Alessandro Cantatore * Team OS/2 Italy        |
// sorgente DLL racchiudente funzioni utilit… x manipolazione di stringhe   |
//  |
// funzioni: -------------------------------------------------------------- |
// LONG APIENTRY linedel(PSZ pszline);                                      |
// PSZ APIENTRY lin1str(PSZ psz, PSZ substr);                               |
// ULONG APIENTRY memdel(PBYTE pdata, ULONG cbdata, PBYTE pdel, ULONG cbdel,
//                       PBYTE psep, ULONG cbsep);
// PBYTE APIENTRY memfind(PBYTE pdata ULONG cbdata, PBYTE pfind, ULONG cbfind,
//                        PBYTE psep, ULONG cbsep);
// PSZ APIENTRY pszlastchr(PSZ psz, CHAR ch);                               |
// BOOL APIENTRY pszwldcmp(char* mask, char* cmp);                          |
// char* APIENTRY pszcpy(char* target, char* source);                       |
// PSZ APIENTRY pszcpycat(PSZ trg, INT cstr, ...);
// ULONG APIENTRY pszncpy(PSZ target, PSZ source, ULONG cch);               |
// int APIENTRY pszicmp(const char* s1, const char* s2);                    |
// int APIENTRY psznicmp(char* s1, const char* s2, unsigned int n);         |
// ULONG APIENTRY strdel(PSZ pszbuf, PSZ pszstr, ULONG cbbuf);              |
// PSZ APIENTRY stristr(PSZ str, PSZ substr, ULONG cb);                     |
// LONG APIENTRY strrepl(PSZ ins, PSZ psznew, PSZ endch);                   |
// ULONG APIENTRY strins(PSZ psz, PSZ substr, PSZ ins, ULONG cb);           |
//                                                                          |
// API: --------------------------------------------------------------------|
//  |
//==========================================================================/

#pragma strings(readonly)

#define INCL_WIN
#define INCL_DOSMISC
#define INCL_DOSNLS
#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_DOSMODULEMGR
#include <os2.h>
#include <stdio.h>
#include <string.h>
#include "commonbs.h"
#include <stdarg.h>


//==========================================================================\
// converte ul (massimo 99) in una stringa concatenandola a psz             |
// parametri:                                                               |
// PSZ psz: stringa in cui scrivere il valore convertito                    |
// ULONG ul: valore numerico da convertire in stringa                       |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID APIENTRY catuch2str(PSZ psz, ULONG ul) {
   PSZ p;
   p = strchr(psz, '\x0');
   if (ul > 10) *p++ = ul / 10;
   *p++ = ul % 10;
   *p = 0;
}


//==========================================================================\
// cancella dalla posizione corrente a fine riga includendo anche i         |
// caratteri di terminazione riga, restituisce il numero dei caratteri      |
// cancellati (in negativo)                                                 |
// parametri: ------------------------------------------------------------- |
// PSZ pszline: indirizzo linea da cancellare                               |
// valore restituito: ----------------------------------------------------- |
// LONG : quantit… caratteri da sottrarre a vecchia lunghezza buffer        |
//==========================================================================/

LONG APIENTRY linedel(PSZ pszline) {
   PSZ pend = strpbrk(pszline, "\r\n\x1a");
   // cerca la fine della linea (l'inizio della riga successiva)
   // se non trova CR-LF o EOF allora punta alla fine del file
   if (!pend) {
      LONG cb = strlen(pszline);
      *pszline = 0;
      return -cb;
   } // end if
   // se pend punta a return cancella eventuale newline successivo
   pend += (memcmp(pend, "\r\n", 2)? 1: 2);
   // il "+ 1" Š allo scopo di copiare il carattere \0 di terminazione
   memmove((PVOID)pszline, (PVOID)pend, strlen(pend) + 1);
   return - (pend - pszline);
}


//==========================================================================\
// restituisce un puntatore alla prima linea che inizia con "substr"        |
// o NULL se non viene trovata alcuna occorrenza                            |
// parametri:                                                               |
// PSZ psz: indirizzo buffer testo                                          |
// PSZ substr: stringa da ricercare                                         |
// valore restituito:                                                       |
// PSZ : indirizzo linea trovata, NULL se nessuna occorrenza                |
//==========================================================================/

PSZ APIENTRY lin1str(PSZ psz, PSZ substr) {
   ULONG cb = strlen(substr);
   while (*psz) {
      if (!psznicmp(psz, substr, cb)) return psz;       // compara
      if (!(psz = strchr(psz, '\n'))) return NULL; // avanza riga seguente
      psz++;
   } /* endwhile */
   return NULL;
}


//==========================================================================\
// Cancella cb byte da pdata eliminando eventuali caratteri di separazione  |
// in eccesso. Se la stringa dei separatori non Š specificata viene solo    |
// cancellato pdel.                                                         |
// parametri:                                                               |
// PBYTE pdata: dati da cui cancellare i dati pdel                          |
// ULONG cbdata: dimensione dati                                            |
// PBYTE pdel: indirizzo stringa da cancellare                              |
// ULONG cbdel: dimensione stringa da cancellare                            |
// PBYTE psep: array di separatori validi. NULL se i separatori devono      |
//             essere tralasciati                                           |
// ULONG cbsep: lunghezza array separatori                                  |
// valore restituito:                                                       |
// nuova lunghezza dati                                                     |
//==========================================================================/

ULONG APIENTRY memdel(PBYTE pdata, ULONG cbdata, PBYTE pdel, ULONG cbdel,
                      PBYTE psep, ULONG cbsep) {
   PBYTE pstart, pend, penddata;
   // se pdel non Š gi… il puntatore alla stringa da cancellare lo cerca e
   // se non lo trova restituisce cbdata inalterato
   if ((pdel < pdata) || (pdel > (penddata = pdata + cbdata))) {
      if (!(pdel = memfind(pdata, cbdata, pdel, cbdel, psep, cbsep)))
         return cbdata;
   } /* endif */
   // cancella separatori precedenti pdel
   if (pstart > pdata) {
      for (pstart = pdel - 1;
           (pstart > pdata) && memchr(psep, *pstart, cbsep);
           pstart--) ;
   } /* endif */
   if (pstart > pdata) ++pstart; // punta al primo separatore
   // cancella separatori successivi a pdel
   for (pend = pdel + cbdel;
        pend < penddata && memchr(psep, *pend, cbsep);
        pend++) ;
   // se la porzione da cancellare Š alla fine si limita a riportare la
   // nuova dimensione
   if (pend == penddata) goto end;
   // se pdel non Š all'inizio lascia un separatore
   if (pstart != pdata) ++pstart;
   memmove(pstart, pend, (ULONG)(penddata - pend));
end:
   return cbdata - (ULONG)(pend - pstart);
}


//==========================================================================\
// Trova la prima ricorrenza della sequenza di byte pfind di lunghezza      |
// cbfind nei dati pdata di lunghezza cbdata.                               |
// Se Š specificato psep controlla che la sequenza di byte sia in posizione |
// iniziale, finale, o separata da uno dei caratteri presenti nell'array    |
// psep di cbsep elementi.                                                  |
// parametri:                                                               |
// parametri:                                                               |
// PBYTE pdata: dati in cui ricercare pfind                                 |
// ULONG cbdata: dimensione dati                                            |
// PBYTE pfind: indirizzo stringa da ricercare                              |
// ULONG cbfind: dimensione stringa da ricercare                            |
// PBYTE psep: array di separatori validi. NULL se i separatori devono      |
//             essere tralasciati                                           |
// ULONG cbsep: lunghezza array separatori                                  |
// valore restituito                                                        |
// puntatore a pfind                                                        |
//==========================================================================/

PBYTE APIENTRY memfind(PBYTE pdata, ULONG cbdata, PBYTE pfind, ULONG cbfind,
                       PBYTE psep, ULONG cbsep) {
   PBYTE p;
   // se la dimensione dei dati in cui effettuare la ricerca Š inferiore a
   // quella dei dati da cercare restituisce NULL
   if (cbdata < cbfind) return NULL;
   for (p = pdata; (p - pdata) < cbdata; ++p) {
      // trova corrispondenza primo carattere dati
      if (!(p = memchr(p, *pfind, cbdata - (p - pdata)))) return NULL;
      // se il resto dei dati corrisponde e la stringa Š all'inizio o Š
      // preceduta da un separatore valido e Š alla fine o Š seguita da
      // un separatore valido o se non Š stata specificata una lista di
      // separatori interrompe il ciclo
      if (!memcmp(p, pfind, cbfind) &&
          (!psep ||
           (p == pdata || memchr(psep, *(p - 1), cbsep)) &&
           ((p + cbfind == pdata + cbdata) ||
             memchr(psep, *(p + cbfind), cbsep))))
         break;
   } /* endfor */
   if (p == pdata + cbdata) return NULL;
   return p;
}


//==========================================================================\
// trova nella stringa l'indirizzo dell'ultima occorrenza del carattere ch  |
// + 1 (il carattere successivo a ch)                                       |
// parametri:                                                               |
// PSZ psz: stringa                                                         |
// CHAR ch: carattere da ricercare                                          |
// valore restituito:                                                       |
// PSZ pszCh: carattere successivo all'ultima occorrenza di ch in psz       |
//            se ch non Š presente ritorna psz                              |
//==========================================================================/

PSZ APIENTRY pszlastchr(PSZ psz, CHAR ch) {
   PSZ pszLst = strrchr(psz, ch);
   return (pszLst? ++pszLst: psz);
}


//==========================================================================\
// compara due stringhe di cui la prima contiene caratteri jolly            |
// parametri:                                                               |
// char* mask: maschera contenente wildchar                                 |
// char* cmp: stringa da comparare con "mask"                               |
// valore restituito:                                                       |
// BOOL: TRUE se cmp risponde al criterio specificato in mask               |
//==========================================================================/

BOOL APIENTRY pszwldcmp(char* mask, char* cmp) {
   char buf[260];
   DosEditName(1, cmp, mask, buf, 260);
   return WinCompareStrings(0, 0, 0, cmp, buf, 0) == WCS_EQ;
}


//==========================================================================\
// copia source su target, ritorna un puntatore alla fine di target         |
//==========================================================================/

char* APIENTRY pszcpy(char* target, char* source) {
   return strchr(strcpy(target, source), 0);
}


//==========================================================================\
// Copia cstr stringhe concatenandole in trg.                               |
// Restituisce un puntatore alla fine della stringa                         |
//==========================================================================/

PSZ APIENTRY pszcpycat(PSZ trg, INT cstr, ...) {
   va_list ap;
   PSZ p;
   va_start(ap, cstr);
   for (p = trg; cstr--; p = pszcpy(p, va_arg(ap, PSZ)));
   va_end(ap);
   return p;
}


//==========================================================================\
// copia n caratteri di source su target terminando con un carattere NULL   |
// target in n Š compreso lo 0 di teminazione                               |
// restituisce il numero di caratteri copiati (lunghezza target: lo 0       |
// di terminazione non Š incluso                                            |
// esempio:                                                                 |
// pszncpy(buf, "0123456789", 5);                                           |
// copia in buf: "0123" e restituisce 4                                     |
//==========================================================================/

ULONG APIENTRY pszncpy(PSZ target, PSZ source, ULONG cch) {
   strncpy(target, source, cch);
   *(target + cch) = 0;
   return strlen(target);
}


//==========================================================================\
// compara due stringhe in modo case insensitivo                            |
//==========================================================================/

int APIENTRY pszicmp(const char* s1, const char* s2) {
   int ai[4] = {-2, 0, -1, +1};
   ULONG ul = WinCompareStrings(0, 0, 0, (PSZ)s1, (PSZ)s2, 0);
   return ai[ul];
}


//==========================================================================\
// compara due stringhe in modo case insensitivo per n caratteri            |
//==========================================================================/

int APIENTRY psznicmp(const char* s1, const char* s2, unsigned int n) {
   PSZ ss1, ss2;
   ULONG ul = 0;
   COUNTRYCODE cc = {0, 0};
   int ret;
   if (!(ss1 = tsmalloc(2 * (n + 1)))) return -256;
   ss2 = ss1 + n + 1;
   memcpy(ss1, s1, n);
   memcpy(ss2, s2, n);
   *(ss1 + n) = *(ss2 + n) = 0;
   if (DosMapCase(n, &cc, ss1) || DosMapCase(n, &cc, ss2)) return -256;
   ret = memcmp(ss1, ss2, n);
   tsfree(ss1);
   return ret;
}


//==========================================================================\
// cancella una substringa (pszstr) contenuta in una stringa (pszbuf)       |
// restituisce la nuova lunghezza di pszbuf (cbbuf Š la lunghezza originaria)
//==========================================================================/

ULONG APIENTRY strdel(PSZ pszbuf, PSZ pszstr, ULONG cbbuf) {
   PSZ pcur;
   ULONG ul = strlen(pszstr);
   // se non trova la stringa restituisce dimensione immutata
   if (!(pcur = stristr(pszbuf, pszstr, ul))) return cbbuf;
   memmove((PVOID)pcur, (PVOID)(pcur + ul), pszbuf + cbbuf + 1 - pcur - ul);
   return cbbuf - ul;
}


//==========================================================================\
// trova la substringa substr in str in modo case insensitivo               |
// rtestituisce il puntatore alla substringa in str o NULL se la            |
// substringa non Š presente                                                |
//==========================================================================/


PSZ APIENTRY stristr(PSZ str, PSZ substr, ULONG cb) {
   while (*str) {
      if (!psznicmp(str, substr, cb)) return str;
      ++str;
   } /* endwhile */
   return NULL;
}


//==========================================================================\
// sostituisce la substringa "psznew" in "psz" alla precedente stringa      |
// delimitata da endch restituisce differenza lunghezza tra nuova stringa   |
// e vecchia stringa                                                        |
// parametri: ------------------------------------------------------------- |
// PSZ ins: punto inserzione nuova stringa (inizio vecchia stringa)         |
// PSZ psznew: nuova stringa da inserire. Se NULL la vecchia stringa viene  |
//             semplicemente cancellata                                     |
// PSZ endch: caratteri che delimitano il riconoscimento della vecchia      |
//            stringa (da ins alla prima occorrenza di un carattere incluso |
//            in endch                                                      |
// valore restituito: ----------------------------------------------------- |
// LONG : differenza lunghezza tra vecchia stringa e nuova                  |
//==========================================================================/

LONG APIENTRY strrepl(PSZ ins, PSZ psznew, PSZ endch) {
   PSZ pend = strpbrk(ins, endch);
   ULONG cbnew = (psznew? strlen(psznew): 0);
   // se carattere endch non presente considera \0 come delimitatore
   if (!pend) {                         // vecchia stringa
      if (cbnew) strcpy(ins, psznew);
      else *(ins + 1) = 0;
      return cbnew - strlen(ins);
   } else {
      if ((pend - ins) != cbnew)        // se le stringhe non sono lunghe =
      // sposta a fine inserzione nuova stringa quanto segue vecchia stringa
         memmove((PVOID)(ins + cbnew), (PVOID)pend, strlen(pend) + 1);
      if (cbnew) memmove((PVOID)ins, (PVOID)psznew, cbnew);
   } // end if
   return (ULONG)(cbnew - (pend - ins));
}


//==========================================================================\
// inserisce una stringa in un altra restituendo la nuova lunghezza della   |
// stringa                                                                  |
// parametri:                                                               |
// PSZ psz   : stringa in cui si deve effettuare inserzione                 |
// PSZ substr: substringa da inserire                                       |
// PSZ ins   : indirizzo punto di inserzione                                |
// ULONG cb  : dimensione corrente di psz                                   |
// valore restituito:                                                       |
// ULONG: nuova lunghezza di psz                                            |
//==========================================================================/

ULONG APIENTRY strins(PSZ psz, PSZ substr, PSZ ins, ULONG cb) {
   ULONG cbss = strlen(substr);
   memmove((PVOID)(ins + cbss), (PVOID)ins, psz + cb - ins);
   memmove((PVOID)ins, (PVOID)substr, cbss);
   return cb + cbss;
}


//==========================================================================\
//  |
// parametri:                                                               |
// HWND hwnd: handle dialogo
// PAPPDATA pad: indirizzo struttura dati applicazione
//  |
// valore restituito:                                                       |
// VOID:
// BOOL:
// ULONG:
//==========================================================================/

