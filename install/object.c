//==========================================================================\
// object.c : procedura object window
//
// MRESULT EXPENTRY instobjproc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
// VOID objprogress(HWND hwnd, HWND hprgs);
// BOOL fillDrvList(PINSTALLDATA pid, HWND hprgs);
// BOOL install(PINSTALLDATA pid, HWND hprgs);
// BOOL refresh(PINSTALLDATA pid, HWND hprgs);
// BOOL uninstall(PINSTALLDATA pid, HWND hprgs);
//==========================================================================/

#pragma strings(readonly)

#include "install.h"

// prototipi funzioni

VOID objprogress(HWND hwnd, HWND hprgs);
BOOL fillDrvList(PINSTALLDATA pid, HWND hprgs);
BOOL install(PINSTALLDATA pid, HWND hprgs);
BOOL refresh(PINSTALLDATA pid, HWND hprgs);
BOOL uninstall(PINSTALLDATA pid, HWND hprgs);


//==========================================================================\
// procedura object window
// parametri:
// valore restituito:
//==========================================================================/

MRESULT EXPENTRY instobjproc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_PRGRSREADY:
         objprogress(hwnd, (HWND)mp1);
         break;
      default:
         return WdefObjWinProc(hwnd, msg, mp1, mp2);
   } // end switch
   return (MRESULT)FALSE;
}


//==========================================================================\
// esegue procedura appropriata secondo valore flag pid->flprgrs
// parametri:
// valore restituito:
//==========================================================================/

VOID objprogress(HWND hwnd, HWND hprgs) {
   PINSTALLDATA pid = WinQueryWindowPtr(hwnd, 0);
   if (!pid) return;
   switch (pid->flprgrs) {
      case ID_IPRDRVINFO: // riempie lista drive
         fillDrvList(pid, hprgs);
         break;
      case ID_IPRINSTLING: // esegue installazione
         install(pid, hprgs);
         break;
      case ID_IPRRFRSHING: // ricrea gli oggetti
         refresh(pid, hprgs);
         break;
      case ID_IPRDELETING: // disinstalla
         uninstall(pid, hprgs);
         break;
   } /* endswitch */
}


//==========================================================================\
// parametri:
// valore restituito:
//==========================================================================/

BOOL fillDrvList(PINSTALLDATA pid, HWND hprgs) {
   char dev[4];
   FSALLOCATE fsa;
   BYTE buf[32];
   CHAR achdd[32];
   ULONG ulDriveNum = 0;
   ULONG ulDriveMap = 0;
   ULONG ul, idx, tot;
   int i, j, k;
   PFSQBUFFER2 pfsqb = (PFSQBUFFER2)buf;

   strcpy(dev, " :");
   DosError(0);
   DosQueryCurrentDisk(&ulDriveNum, &ulDriveMap);
   // conta drive presenti
   for (i = 2, tot = 0; i < 26; ++i)
      tot += (ulDriveMap & 1L << i)? 1: 0;
   // per ogni drive presente raccogli informazioni
   for (i = 2, j = 0, k = 0, idx = 0; i < 26; i++) {// ripete x tutti i drive
//      DosSleep(1000);
      if (ulDriveMap & 1L << i) {            // se trova partizione
         if (Wincrease(&idx, hprgs, tot)) return FALSE;
         ul = 32;
         dev[0] = (CHAR)(i + 'A');
         if (DosQueryFSInfo(i + 1, FSIL_ALLOC, &fsa, sizeof(FSALLOCATE)))
            continue;        // se drive non disponibile passa a successivo
         DosQueryFSAttach((PSZ)dev, 1, FSAIL_QUERYNAME,
                          (PFSQBUFFER2)&buf, &ul);
         if (pfsqb->iType != 3 ||         // se non drive locale
             (strcmp(pfsqb->szName + 3, "FAT") &&    // o nŠ FAT
              strcmp(pfsqb->szName + 3, "HPFS") &&
              strcmp(pfsqb->szName + 3, "JFS")))    // e nŠ HPFS
            continue;                     // passa al successivo
         pid->cMbDrv[i] = (ULONG)((float)fsa.cbSector *
                                  (float)fsa.cSectorUnit *
                                  (float)fsa.cUnitAvail)
                                  / 1048576;
         sprintf(achdd, "%s % 15d MB", dev, pid->cMbDrv[i]);
         DlgLboxInsertItem(pid->hwnd, LBX_INSTDRV, LIT_END, achdd);
         // seleziona l'item corrispondente al drive menzionato in pad->path
         if (dev[0] == *pid->tpath) k = j;
         j++;
      } // end if
   } // end for
   Wincrease(&idx, hprgs, tot);
   DosError(1);
   DlgLboxSelectItem(pid->hwnd, LBX_INSTDRV, k);
   WinPostMsg(hprgs, PRGSM_END, MPVOID, MPVOID);
   return TRUE;
}


//==========================================================================\
// inizializza file log, installa i vari file, crea gli oggetti sul desktop |
// parametri:                                                               |
// valore restituito:                                                       |
//==========================================================================/

BOOL install(PINSTALLDATA pid, HWND hprgs) {
   BOOL rc = FALSE;
   ULONG csteps;
   ULONG i = 0;
   // inizializza file log
   startProcessing(pid, hprgs);
   // calcola numero passi da eseguire
   if (!(csteps = instinstcsteps(pid))) goto end;
   // se programma preferenze aperto lo chiude
   if (!instcloseprefapp(pid, &i, hprgs, csteps)) goto end;
   // modifica os2.ini
   if (!instaddprfitems(pid, &i, hprgs, csteps)) goto end;
   // cancella file obsoleti
   if (!instdelobsolete(pid, &i, hprgs, csteps)) goto end;
   // copia file e smartwin.dll
   if (!instcopyfiles(pid, &i, hprgs, csteps)) goto end;
   // copia albero bitmap
   if (!instcopytree(pid, &i, hprgs, csteps)) goto end;
   // crea oggetti
   if (!instcreatobjs(pid, &i, hprgs, csteps)) goto end;
   // if needed update the ini file
   updateIni();
   rc = TRUE;
end:
   // scrive file log, mostra messaggio termine e riabilita controlli
   endProcessing(pid, hprgs, rc);
   return rc;
}


//==========================================================================\
// Ricrea gli oggetti sul desktop                                           |
// parametri:                                                               |
// valore restituito:                                                       |
//==========================================================================/

BOOL refresh(PINSTALLDATA pid, HWND hprgs) {
   BOOL rc = FALSE;
   ULONG csteps;
   ULONG i = 0;
   // inizializza file log
   startProcessing(pid, hprgs);
   // calcola numero passi da eseguire
   csteps = instrfrshcsteps(pid);
   // crea oggetti
   if (!instcreatobjs(pid, &i, hprgs, csteps)) goto end;
   rc = TRUE;
end:
   // scrive file log, mostra messaggio termine e riabilita controlli
   endProcessing(pid, hprgs, rc);
   return rc;
}


//==========================================================================\
// Disinstalla il programma                                                 |
// parametri:                                                               |
// valore restituito:                                                       |
//==========================================================================/

BOOL uninstall(PINSTALLDATA pid, HWND hprgs) {
   BOOL rc = FALSE;
   ULONG csteps;
   ULONG i = 0;
   // inizializza file log
   startProcessing(pid, hprgs);
   // calcola numero passi da eseguire
   if (!(csteps = instuninstcsteps(pid, hprgs))) goto end;
   // se programma preferenze aperto lo chiude
   if (!instcloseprefapp(pid, &i, hprgs, csteps)) goto end;
   // modifica os2.ini
   if (!instdelprfitems(pid, &i, hprgs, csteps)) goto end;
   // cancella albero bitmap
   if ((pid->fl & INSSEL_REMBMPS) && !instdeltree(pid, &i, hprgs, csteps))
      goto end;
   // cancella file, smartwin.dll e styler20.dll
   if (!instdelfiles(pid, &i, hprgs, csteps)) goto end;
   // distrugge oggetti
   if (!instdelobjs(pid, &i, hprgs, csteps)) goto end;
   // distrugge directory installazione e altri eventuali file (chiedendo
   // conferma)
   if (!instdelinstpath(pid, &i, hprgs, csteps)) goto end;
   rc = TRUE;
end:
   // scrive file log, mostra messaggio termine e riabilita controlli
   endProcessing(pid, hprgs, rc);
   return rc;
}
