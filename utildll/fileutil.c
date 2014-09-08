//==========================================================================\
// smwutil.c  (c) 1998 * Alessandro Cantatore * Team OS/2 Italy    |
// sorgente DLL racchiudente funzioni utitlit… comuni al programma di
// installazione e preferenze
//  |
// funzioni: -------------------------------------------------------------- |
//  |
// API: --------------------------------------------------------------------|
//  |
//==========================================================================/

#pragma strings(readonly)

#define INCL_DOSMISC
#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_DOSFILEMGR
#define INCL_DOSMODULEMGR
#include <os2.h>
#include <stdio.h>
#include <string.h>
#include "commonbs.h"


//==========================================================================\
// Sostituisce l'estensione di pszFile con quella contenuta in pszExt       |
// e copia pszFile nel nuovo nome file ottenuto sovrascrivendo l'eventuale  |
// omonimo file preesistente !! se pszExt include punto iniziale, il punto  |
// viene cancellato.                                                        |
// Restituisce TRUE in caso di successo FALSE in caso di errore             |
// se pszExt Š NULL ricava un nome di file unico partendo da 000            |
// parametri:                                                               |
// PSZ pszFile: nome del file da copiare                                    |
// PSZ pszExt : estensione della copia di backup                            |
// valore restituito:                                                       |
// BOOL: TRUE/FALSE (successo/errore)                                       |
//==========================================================================/

BOOL APIENTRY ioBackup(PSZ pszFile, PSZ pszExt) {
   CHAR achNew[260];
   FILESTATUS3 fs;

   strcpy(achNew, pszFile);
   // se estensione == NULL copia in file unico con estensione .XXX (0-999)
   if (!pszExt) {
      ioEditExt(achNew, "???");
      if (!ioUniqFName(achNew)) return FALSE;
   } else {
      ioEditExt(achNew, pszExt);
      memset(&fs, 0, sizeof(FILESTATUS3));
      DosSetPathInfo(achNew, FIL_STANDARD, &fs,
                     sizeof(FILESTATUS3), DSPI_WRTTHRU);
   } /* endif */
   if (DosCopy(pszFile, achNew, DCPY_EXISTING)) return FALSE;
   return TRUE;
}


//==========================================================================\
// incrementa il conteggio dei file e directory                             |
// parametri:                                                               |
// valore restituito:                                                       |
//==========================================================================/

LONG fcfiles(PPROCESSTREEDATA pptd) {
   return 1;
}


//==========================================================================\
// conta il numero di file (rispondenti alla maschera e agli attributi      |
// specificati), contenuti in una directory. La directory base del tree     |
// non Š inclusa nel conteggio.                                             |
// parametri:                                                               |
// PSZ path: Š il path da cui iniziare la ricerca. Se la ricerca non Š      |
//           ricorsiva al path deve essere concatenata la maschera di       |
//           ricerca altrimenti al path deve essere concatenato "*" e la    |
//           maschera di ricerca deve essere specificata a parte            |
// PSZ pmask: se specificata il conteggio Š ricorsivo                       |
// ULONG flattr: gli attributi dei file da conteggiare                      |
// BOOL fl: specifica, nel conteggio ricorsivo, se includere o meno le      |
//          directory. Nel conteggio non ricorsivo, l'inclusione delle      |
//          directory Š determinata dal valore di flattr                    |
// valore restituito:                                                       |
// LONG cf: < 0 = errore, >= 0 = numero di file/directory                   |
//==========================================================================/

LONG APIENTRY ioCountTree(PSZ path, PSZ pmask, ULONG flattr, BOOL fl) {
   PROCESSTREEDATA pt;
   memset(&pt, 0, sizeof(pt));
   strcpy(pt.achPath, path);
   if (pmask) strcpy(pt.achMask, pmask);
   pt.flfind = flattr;
   pt.pffile = fcfiles;
   if (fl) pt.pfdirpre = fcfiles;
   return ioProcessTree(&pt);
}


//==========================================================================\
// cambia l'estensione di pszFile in pszExt. Se pszFile non ha estensione   |
// aggiunge il punto e l'estensione                                         |
// Nota: la memoria cui punta pszFile deve permettere la scrittura di altri |
//       4 byte dell'estensione                                             |
// parametri:                                                               |
// PSZ pszFile: nome del file                                               |
// PSZ pszExt: nome dell'estensione                                         |
// valore restituito:                                                       |
// PSZ == pszFile                                                           |
//==========================================================================/

PSZ APIENTRY ioEditExt(PSZ pszFile, PSZ pszExt) {
   PSZ psz = strrchr(pszFile, '.');
   // se estensione specificata comprende "." iniziale incrementa puntatore
   if (*pszExt == '.') ++pszExt;
   strcpy((psz? psz + 1: pszcpy(pszFile, ".")), pszExt);
   return pszFile;
}


//==========================================================================\
// alloca la memoria necessaria e vi scrive il contenuto del file           |
// nell'ULONG puntato da pcb scrive la dimensione del file letto            |
// restituendo un puntatore all'area di memoria allocata o NULL in caso     |
// di errore                                                                |
// parametri:                                                               |
// PSZ pszFile: nome del file da leggere                                    |
// PULONG pcb : (in) alloca per la lettura (e successiva editazione) una    |
//              quantit… di caratteri pari alla dimensione del file per     |
//              *pcb. (out) quantit… caratteri letti                        |
//              se pcb == 0 si limita a restituire la dimensione del file   |
//              senza leggerlo                                              |
// valore restituito:                                                       |
// PSZ : indirizzo area memoria in cui Š stato scritto il contenuto del file|
//==========================================================================/

PSZ APIENTRY ioF2psz(PSZ pszFile, PULONG pcb) {
   FILESTATUS3 fs;
   HFILE hf;
   ULONG ul;
   PSZ psz;

   if (DosOpen(pszFile,  &hf, &ul, 0, FILE_NORMAL,
               OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
               OPEN_FLAGS_SEQUENTIAL | OPEN_SHARE_DENYWRITE |
               OPEN_ACCESS_READONLY, NULL))
      goto error0;
   if (DosQueryFileInfo(hf, FIL_STANDARD, &fs, sizeof(fs))) goto error1;
   // alloca la dimensione del file oppure moltiplica questa per *pcb
   ul = (pcb && *pcb)? *pcb: 1;
   if (!pcb || (pcb && *pcb)) {                // legge il file
      if (!(psz = (PSZ)tsmalloc(ul * fs.cbFile + 1))) goto error1;
      if (DosRead(hf, psz, fs.cbFile, &ul)) goto error2;
   } /* endif */
   
   if (DosClose(hf)) goto error2;

   if (pcb) *pcb = fs.cbFile;
   *(psz + fs.cbFile) = 0;     // termina file
   return psz;

   error2: tsfree(psz);
   error1: DosClose(hf);
   error0: return NULL;
}


//==========================================================================\
// riporta il nome del file inclusa l'estensione isolandolo dal percorso    |
// cercando l'ultima slash nel nome del file completo                       |
// parametri:                                                               |
// PSZ psz: nome file eventualmente comprendente il path                    |
// valore restituito:                                                       |
// PSZ pszSlash: nome file inclusa estensione                               |
//==========================================================================/

//#define ioFNameFromPath(psz)            pszlastchr(psz, '\\')


//==========================================================================\
// controlla l'esistenza di un file:                                        |
// parametri:                                                               |
// PSZ pszFile: nome del file                                               |
// PULONG pul: indirizzo variabile in cui scrivere rc DosFindFirst()        |
// valore restituito:                                                       |
// BOOL = TRUE se il file esiste, FALSE se non esiste                       |
//==========================================================================/

BOOL APIENTRY ioFExists(PSZ pszFile, PULONG pul) {
   HDIR hdir = HDIR_CREATE;
   FILEFINDBUF3 fbuf;
   APIRET rc;
   ULONG ul = 1;
   rc = DosFindFirst(pszFile, &hdir, ALLFILES, &fbuf,
                     sizeof(FILEFINDBUF3), &ul, FIL_STANDARD);
   DosFindClose(hdir);
   if (pul) *pul = rc;
   return rc == 0;
}


//==========================================================================\
// compara le date e ore di due file                                        |
// parametri:                                                               |
// PFDATE pfds: data file 1                                                 |
// PFTIME pfts: ora file 1                                                  |
// PFDATE pfdt: data file 2                                                 |
// PFTIME pftt: ora file 2                                                  |
// valore restituito:                                                       |
// INT: 1 file1 + vecchio di file 2 / -1 viceversa / 0 file ==              |
//==========================================================================/

INT APIENTRY ioFilOlder(PFDATE pfds, PFTIME pfts, PFDATE pfdt, PFTIME pftt) {
   return (pfds->year > pfdt->year? 1:
            (pfds->year < pfdt->year? -1:
              (pfds->month > pfdt->month? 1:
                (pfds->month < pfdt->month? -1:
                  (pfds->day > pfdt->day? 1:
                    (pfds->day < pfdt->day? -1:
                      (pfts->hours > pftt->hours? 1:
                        (pfts->hours < pftt->hours? -1:
                          (pfts->minutes > pftt->minutes? 1:
                            (pfts->minutes < pftt->minutes? -1:
                              (pfts->twosecs > pftt->twosecs? 1:
                                (pfts->twosecs < pftt->twosecs? -1: 0)
          ) ) ) ) ) ) ) ) ) ) );
}


//==========================================================================\
// svuota il contenuto di un buffer sequenzialmente in un file.             |
// parametri:                                                               |
// PSZ pszFile: nome file in cui svuotare il buffer                         |
// PSZ pbuf: indirizzo buffer da copiare nel file                           |
// PSZ *ppend: indirizzo indirizzo ultimo carattere scritto nel buffer      |
// valore restituito:                                                       |
// BOOL: (TRUE/FALSE) successo/errore                                       |
//==========================================================================/

BOOL APIENTRY ioFlushBuf(PSZ pszFile, PSZ pbuf, PSZ* ppend) {
   ULONG ul;
   HFILE hf;
   APIRET rc;
   if (0!= (rc =DosOpen(pszFile, &hf, &ul, 0, 0,
               OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
               OPEN_FLAGS_SEQUENTIAL | OPEN_SHARE_DENYREADWRITE |
               OPEN_ACCESS_WRITEONLY, NULL)))
      return FALSE;
   if (DosSetFilePtr(hf, 0, FILE_END, &ul)) goto error;
   if (DosWrite(hf, pbuf, *ppend - pbuf, &ul)) goto error;
   DosClose(hf);
   *ppend = pbuf;
   return TRUE;
error:
   DosClose(hf);
   return FALSE;
}


//==========================================================================\
// crea tutte le directory necessarie per costruire il path pszPath         |
// parametri:                                                               |
// PSZ pszPath: path da creare se non esiste                                |
// valore restituito:                                                       |
// BOOL: TRUE = successo, FALSE = errore                                    |
//==========================================================================/

BOOL APIENTRY ioMakePath(PSZ pszPath) {
   ULONG ul;
   PSZ pszSlash = pszPath + 3;
   if (pszPath[ul = strlen(pszPath) - 1] != '\\') strcat(pszPath, "\\");
   while (*pszSlash++) {
      if (*pszSlash == '\\') {
         *pszSlash = 0;
         if (!ioFExists(pszPath, NULL) && DosCreateDir(pszPath, NULL))
            return FALSE;
         if (*(pszSlash + 1)) *pszSlash = '\\';
      } // end if
   } // end while
   // se necessario ripristina slash finale
   if (!pszPath[ul]) strcat(pszPath, "\\");
   return TRUE;
}


//==========================================================================\
// Processa (opzionalmente in modo ricorsivo) il contenuto di una directory.|
// La funzione usa la struttura PROCESSTREEDATA:                            |
// typedef struct _PROCESSTREEDATA {                                        |
//    FILEFINDBUF3 fBuf;       // dati file trovato                         |
//    CHAR achPath[260];       // file specifiche file da ricercare         |
//    CHAR achMask[80];        // maschera di ricerca                       |
//    ULONG flfind;            // attributi file da ricercare               |
//    PVOID pdata;             // user data                                 |
//    LONG (* pffile)(struct _PROCESSTREEDATA * pptd);    // funzione       |
//    LONG (* pfdirpre)(struct _PROCESSTREEDATA * pptd);  // funzione       |
//    LONG (* pfdirpost)(struct _PROCESSTREEDATA * pptd); // funzione       |
// } PROCESSTREEDATA, * PPROCESSTREEDATA;                                   |
// Quando la funzione viene chiamata in modo ricorsivo Š necessario         |
// specificare la maschera di ricerca in achMask, mentre achPath deve       |
// contenere il path completo di partenza seguito da "\\*".                 |
// In tal caso, per ogni directory trovata, viene modificato achPath        |
// aggiungendo il nome della directory seguito da "\\*", viene eseguita     |
// la funzione pfdirpre() (se non NULL), viene richiamata ricorsivamente    |
// ioProcessTree() e infine viene richiamata la funzione pfdirpost()        |
// (se non NULL), mentre per ogni file trovato, se questo soddisfa la       |
// maschera di ricerca achMask, viene richiamata la funzione pffile.        |
// Nel modo non ricorsivo per ogni file e directory soddisfacente la        |
// maschera achMask viene richiamato pffile()                               |
// Tutte le funzioni (pffile, pfdirpre, pfdirpost) devono restituire        |
// il numero di file processati (>= 0) oppure -1 in caso di errore.         |
// valore restituito:                                                       |
// LONG: >= 0 indica numero file trovati, < 0 indica errore                 |
//==========================================================================/

LONG ioProcessTree(PPROCESSTREEDATA pptd) {
   HDIR hDir = HDIR_CREATE;
   ULONG ul = 1;
   APIRET rc = 0;
   LONG cfile = 0;
   LONG res = 0;

   // inizia scansione
   rc = DosFindFirst(pptd->achPath, &hDir, pptd->flfind, &pptd->fBuf,
                     sizeof(FILEFINDBUF3), &ul, FIL_STANDARD);
   if (rc) return -1;
   do {
      if (*pptd->achMask) {           // se la ricerca Š ricorsiva
         if (pptd->fBuf.attrFile & FILE_DIRECTORY) { // se directory
            // se non Š directory corrente "." o parent ".."
            if (!(pptd->fBuf.achName[0] == '.' && (!pptd->fBuf.achName[1] ||
                 (pptd->fBuf.achName[1] == '.' && !pptd->fBuf.achName[2])))) {
               // appende nome directory a path ricerca e richiama se stessa
               PSZ pszend = ioFNameFromPath(pptd->achPath);
               if (pptd->pfdirpre) {
                  if ((res = pptd->pfdirpre(pptd)) >= 0) cfile += res;
                  else goto error;
               } /* endif */
               strcpy(pszcpy(pszend, pptd->fBuf.achName), "\\*");
               if ((res = ioProcessTree(pptd)) >= 0) cfile += res;
               else goto error;
               if (pptd->pfdirpost) {
                  if ((res = pptd->pfdirpost(pptd)) >= 0) cfile += res;
                  else goto error;
               } /* endif */
               // ripristina percorso originario
               *pszend = '*'; *++pszend = '\0';
            } /* endif */
         // altrimenti se Š un file rispondente alle specifiche chiama pfunc
         } else if (pszwldcmp(pptd->achMask, pptd->fBuf.achName)) {
            if (pptd->pffile) {
               if ((res = pptd->pffile(pptd)) >= 0) cfile += res;
               else goto error;
            } /* endif */
         } /* endif */
      } else {                       // ricerca non ricorsiva
         if (pptd->pffile) {
            if ((res = pptd->pffile(pptd)) >= 0) cfile += res;
            else goto error;
         } /* endif */
      } /* endif */
   } while (!DosFindNext (hDir, &pptd->fBuf, sizeof(FILEFINDBUF3), &ul));
   goto end;
error:
   cfile = -1;
end:
   DosFindClose(hDir);
   return cfile;
}


//==========================================================================\
// scrive cb Byte dall'area di memoria puntata da pdata nel file pszFile    |
// sovrascrive il file se gi… esiste                                        |
// PSZ pszFile: nome file in cui si vuole scrivere pdata                    |
// PSZ pdata  : indirizzo dati che si vogliono scrivere nel file            |
// ULONG cb   : quantit… dati da pdata da scrivere in pszFile               |
//              se cb = 0 ricava la lunghezza di pdata tramite strlen       |
//              ovviamente questo funziona correttamente solo se pdata Š    |
//              una stringa terminata da 0                                  |
// valore restituito:                                                       |
// ULONG : il numero dei byte scritti o 0 in caso di errore                 |
//==========================================================================/

ULONG APIENTRY ioPsz2f(PSZ pszFile, PSZ pdata, ULONG cb) {
   FILESTATUS3 fs;
   HFILE hf;
   ULONG ul;
   if (!cb) cb = strlen(pdata);
   if (DosOpen(pszFile, &hf, &ul, 0, FILE_NORMAL,
               OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_REPLACE_IF_EXISTS,
               OPEN_FLAGS_SEQUENTIAL | OPEN_SHARE_DENYWRITE |
               OPEN_ACCESS_WRITEONLY, NULL))
      goto error0;
   // crea nuovo config.sys
   if (DosWrite(hf, (PVOID)pdata, cb, &ul)) goto error1;
   if (DosClose(hf)) goto error1;

   return ul;

   error1: DosClose(hf);
   error0: return 0;
}


//==========================================================================\
// crea nome file temporaneo sostituendo al carattere ? trovato nella       |
// stringa pszFile le cifre da 0 a 9 finch‚ non si ottiene nome file unico  |
// restituisce il nome del file trovato                                     |
// parametri:                                                               |
// PSZ pszFile: nome del file contenente la maschera di sostituzione        |
// valore restituito:                                                       |
// PSZ nome file unico                                                      |
//==========================================================================/

PSZ APIENTRY ioUniqFName(PSZ pszFile) {
   CHAR mask[260];
   PSZ pctl;
   ULONG cb = strlen(pszFile);
   INT i;

   strcpy(mask, pszFile);
   // se manca il carattere mask restituisce errore
   if (!(pctl = strchr(pszFile, '?'))) return NULL;
   // comincia sostituendo 0 a tutti i '?' incontrati
   for (i = 0; i < cb; ++i) if (pszFile[i] == '?') pszFile[i] = '0';
   while (ioFExists(pszFile, NULL)) {
      for (i = cb; i--;) {
         if (mask[i] == '?') {
            if (pszFile[i] == '9') {
               if (i == pctl - pszFile) return NULL;       // overflow
               pszFile[i] = '0';
            } else {
               ++pszFile[i];
               break;
            } /* endif */
         } /* endif */
      } /* endfor */
   } /* endwhile */
   return pszFile;
}
