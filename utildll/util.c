//==========================================================================\
// util.c  (c) 1998-1999 * Alessandro Cantatore * Team OS/2 Italy           |
// - inizializzazione DLL
// - funzioni allocazione memoria
// - funzioni varie
//  |
// funzioni: -------------------------------------------------------------- |
//  |
// API: --------------------------------------------------------------------|
//  |
//==========================================================================/

#pragma strings(readonly)

#define INCL_WIN
#define INCL_DOSMISC
#define INCL_DOSPROCESS
#define INCL_DOSMODULEMGR
#define INCL_DOSNLS        /* National Language Support values */
#define INCL_DOSDATETIME   /* Date and time values */
#define INCL_SHLERRORS

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commonbs.h"

ULONG ulsem;
//==========================================================================\
// Subsystem Thread Safe malloc                                             |
//==========================================================================/

PVOID APIENTRY tsmalloc(ULONG cb) {
   PVOID pv;
   if (!cb) return NULL;
   while (lockxchng(&ulsem, 1)) DosSleep(1);
   pv = malloc(cb);
   ulsem = 0;
   return pv;
}


//==========================================================================\
// Subsystem Thread Safe free                                               |
//==========================================================================/

VOID APIENTRY tsfree(PVOID pv) {
   if (!pv) return;
   while (lockxchng(&ulsem, 1)) DosSleep(1);
   free(pv);
   ulsem = 0;
}


//==========================================================================\
// Subsystem Thread Safe _heapmin                                           |
//==========================================================================/

INT APIENTRY tsheapmin(VOID) {
   INT rc;
   while (lockxchng(&ulsem, 1)) DosSleep(1);
   rc = _heapmin();
   ulsem = 0;
   return rc;
}


//-------------------------- utility varie ----------------------------------


//==========================================================================\
// ritorna il nome incluso il percorso dell'eseguibile del processo attivo  |
// parametri:                                                               |
// PSZ pszPath: buffer in cui sar… scritto il percorso dell'eseguibile      |
// ULONG cch: dimensione del buffer                                         |
// valore restituito:                                                       |
// BOOL: TRUE = successo, FALSE = errore                                    |
//==========================================================================/

BOOL APIENTRY uExeFullName(PSZ pszPath, ULONG cch) {
   PPIB ppib;
   PTIB ptib;
   if (DosGetInfoBlocks(&ptib, &ppib) ||
       DosQueryModuleName(ppib->pib_hmte, cch, pszPath))
      return FALSE;
   return TRUE;
}


//==========================================================================\
// ricava il nome del modulo che ha avviato il processo corrente e          |
// sostituisce l'estensione EXE con pszExt                                  |
// parametri:                                                               |
// PSZ pszPath: buffer in cui sar… scritto il percorso dell'eseguibile      |
// ULONG cch: dimensione del buffer                                         |
// PSZ pszExt: estensione da dare al file INI                               |
// valore restituito:                                                       |
// BOOL: TRUE = successo, FALSE = errore                                    |
//==========================================================================/

BOOL APIENTRY uEditExeExt(PSZ pszPath, ULONG cch, PSZ pszExt) {
   PSZ psz;
   if (!uExeFullName(pszPath, cch)) return FALSE;
   // se estensione specificata comprende "." iniziale incrementa puntatore
   if (*pszExt == '.') ++pszExt;
   // controlla se Š gi… presente precedente estensione
   psz = strrchr(pszPath, '.');
   strcpy((psz? psz + 1: pszcpy(pszPath, ".")), pszExt);
   return TRUE;
}


//==========================================================================\
// scrive la data e l'ora correnti nel formato appropriato                  |
//==========================================================================/

BOOL APIENTRY uDateTime(PSZ pszdate, PSZ psztime) {
   COUNTRYCODE cc = {0};
   COUNTRYINFO ci = {0};
   DATETIME dt;
   ULONG ul = 0;
   // rileva informazioni relative formato dati secondo nazionalit…
   if (DosQueryCtryInfo(sizeof(COUNTRYINFO), &cc, &ci, &ul))
      return FALSE;
                    
   DosGetDateTime(&dt);
   if (pszdate) {
      pszdate[2] = pszdate[5] = *ci.szDateSeparator;
      switch (ci.fsDateFmt) {
         case(1):                                  /* dd/mm/yy */
            pszdate[0] = '0' + dt.day / 10;
            pszdate[1] = '0' + dt.day % 10;
            pszdate[3] = '0' + dt.month / 10;
            pszdate[4] = '0' + dt.month % 10;
            pszdate[6] = '0' + (dt.year % 100) / 10;
            pszdate[7] = '0' + dt.year % 10;
            break;
         case(2):                                  /* yy/mm/dd */
            pszdate[0] = '0' + (dt.year % 100) / 10;
            pszdate[1] = '0' + dt.year % 10;
            pszdate[3] = '0' + dt.month / 10;
            pszdate[4] = '0' + dt.month % 10;
            pszdate[6] = '0' + dt.day / 10;
            pszdate[7] = '0' + dt.day % 10;
            break;
         default:                                  /* mm/dd/yy */
            pszdate[0] = '0' + dt.month / 10;
            pszdate[1] = '0' + dt.month % 10;
            pszdate[3] = '0' + dt.day / 10;
            pszdate[4] = '0' + dt.day % 10;
            pszdate[6] = '0' + (dt.year % 100) / 10;
            pszdate[7] = '0' + dt.year % 10;
            break;
      } /* endswitch */
   } /* endif */
   if (psztime) {
      psztime[2] = psztime[5] = *ci.szTimeSeparator;
      psztime[0] = '0' + dt.hours / 10;
      psztime[1] = '0' + dt.hours % 10;
      psztime[3] = '0' + dt.minutes / 10;
      psztime[4] = '0' + dt.minutes % 10;
      psztime[6] = '0' + dt.seconds / 10;
      psztime[7] = '0' + dt.seconds % 10;
   } /* endif */
   return TRUE;
}


//==========================================================================\
// restituisce il valore corrispondente ad un settaggio di sistema          |
// parametri:                                                               |
// LONG lID: id valore richiesto                                            |
// valore restituito:                                                       |
// LONG: valore restituito (-1 in caso di errore)                           |
//==========================================================================/

LONG APIENTRY uGetSval(LONG lID) {
   LONG l;
   if (lID > DOS_INFO) {
      lID -= DOS_INFO;
      if (DosQuerySysInfo(lID, lID, &l, 4)) return -1;
   } else {
      l = WinQuerySysValue(HWND_DESKTOP, lID);
   } /* endif */
   return l;
}


//==========================================================================\
// restituisce il crc di cb byte puntati da pb                              |
// parametri:                                                               |
// PBYTE pb: indirizzo buffer contenente dati di cui calcolare CRC          |
// ULONG cb: dimensione dati di cui calcolare CRC                           |
// valore restituito:                                                       |
// ULONG: CRC / 0 in caso di errore                                         |
//==========================================================================/

ULONG APIENTRY ucrc(PBYTE pb, ULONG cb) {
   PULONG ptab;
   ULONG CRC;
   int i, n;
   if (!(ptab = (PULONG)tsmalloc(256 * sizeof(ULONG)))) return 0;
   for (i = 0; i < 256; i++) {
      CRC = i;
      for (n = 1; n < 9; n++) {
         if (CRC & 1) CRC = (CRC >> 1) ^ 0xedb88320;
         else CRC = CRC >> 1;
      } /* endfor */
      ptab[i] = CRC;
   } /* endfor */
   CRC=0xffffffffL;
   for (i = 0; i < cb; ++i, ++pb) {
      CRC = ptab[(CRC ^ *pb) & 0xFF] ^ ((CRC >> 8) & 0x00ffffffL);
   } /* endfor */
   tsfree(ptab);
   return CRC ^ 0xffffffffL;
}


//==========================================================================\
// trasforma il codice in una sequenza di caratteri alfanumerici + ':' e ';'|
// parametri:                                                               |
// PBYTE pin: indirizzo dati da convertire                                  |
// PBYTE pout: indirizzo buffer dati convertiti                             |
// ULONG cb: dimensione dati da convertire                                  |
// ULONG cxline: dimensione formattazione linea output                      |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/
 
VOID APIENTRY upb2an(PBYTE pin, PBYTE pout, ULONG cb, ULONG cxline) {
   INT i, j;
   PSZ pcur = pout;
   *pcur = 0;
   j = 0;
   while (cb) {
      for (i = 2; i < 8; i += 2) {
         *pcur = (*pcur | (*pin >> i)) + '0';
         if (++j == cxline) {*++pcur = '\n'; j = 0;}
         *++pcur = 0x3f & (*pin++ << (6 - i));
         if (!--cb) {*pcur += '0'; goto end;}
      } /* endfor */
      *pcur += '0';
      if (++j == cxline) {*++pcur = '\n'; j = 0;}
      ++pcur;
   } /* endwhile */
end:
   for (pcur = pout; *pcur; ++pcur) {
      if (*pcur < '<') continue;
      if (*pcur < 'V') *pcur += 5;
      else *pcur += 11;
   } /* endfor */
}


//==========================================================================\
// decodifica una stringa codificata con upb2an                             |
// il buffer pout deve avere una dimensione almeno di 3/4 la dimensione di  |
// pin                                                                      |
// parametri:                                                               |
// PBYTE pin: indirizzo dati da decodificare                                |
// PBYTE pout: indirizzo buffer in cui scrivere dati decodificati           |
//==========================================================================/

VOID APIENTRY uan2pb(PBYTE pin, PBYTE pout) {
   INT i;
   PSZ pcur;
   *pout = 0;
   for (pcur = pin; *pcur; ++pcur) {
      if (*pcur < '<') continue;
      if (*pcur >= 'a') *pcur -= 11;
      else *pcur -= 5;
   } /* endfor */
   while (*pin) {
      for (i = 2; i < 8; i += 2) {
         *pout = (*pin - '0') << i;
         if (!*++pin) return;
         while (*pin == '\r' || *pin == '\n') ++pin;
         *pout++ |= (*pin - '0') >> (6 - i);
      } /* endfor */
      ++pin;
      while (*pin == '\r' || *pin == '\n') ++pin;
   } /* endwhile */
}



