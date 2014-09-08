//==========================================================================\
// util.c: funzioni costruzione pagine programma installazione
//                                                                          |
// VOID progress(HWND hp, HWND ho, PINSTALLDATA pid, ULONG id);
// VOID checkPath(HWND hwnd, PINSTALLDATA pid, BOOL fl);
// BOOL startProcessing(PINSTALLDATA pid, HWND hprgs);
// BOOL endProcessing(PINSTALLDATA pid, HWND hprgs, BOOL fl);
// BOOL instprfedit(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps,
//                  PSZ app, PSZ key, PSZ val);
// ULONG instinstcsteps(PINSTALLDATA pid);
// ULONG instrfrshcsteps(PINSTALLDATA pid);
// ULONG instuninstcsteps(PINSTALLDATA pid);
// BOOL instcloseprefapp(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
// BOOL instaddprfitems(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
// BOOL instdelobsolete(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
// BOOL instcopyfiles(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
// BOOL instcopytree(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
// BOOL instcreatobjs(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
// BOOL instdelprfitems(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
// BOOL instdeltree(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
// BOOL instdelfiles(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
// BOOL instdelobjs(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
// BOOL instdelinstpath(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps);
//==========================================================================/

#pragma strings(readonly)

#include "install.h"


//==========================================================================\
// usato per mostrare il dialogo di progresso durante l'inizializzazione    |
// della lista dei drive e durante l'elaborazione                           |
// parametri:
// HWND hp: parent window
// HWND ho: owner window
// PINSTALLDATA pid: struttura dati installazione
// ULONG id: id operazione
// valore restituito:
// VOID
//==========================================================================/

VOID progress(HWND hp, HWND ho, PINSTALLDATA pid, ULONG id) {
   PROGRESSDLG prgs;
   HPS hps = WinGetPS(hp);
   pid->flprgrs = id;
   memset((PVOID)&prgs, 0, sizeof(PROGRESSDLG));
   prgs.hNotify = pid->a.hObj;
   prgs.hbmp = GpiLoadBitmap(hps, pid->a.hres, 14, 0, 0);
   if (!(prgs.pszTitle = (PSZ)tsmalloc(256)) ||
       !(prgs.pszPrgrss = (PSZ)tsmalloc(256)) ||
       !(prgs.pszTime = (PSZ)tsmalloc(256)) ||
       !(prgs.pszStop = (PSZ)tsmalloc(256))) goto end;
   WgetStr(id, prgs.pszTitle, 256);
   WgetStr(ID_GENPROGRESS, prgs.pszPrgrss, 256);
   WgetStr(ID_GENTIMEELAPS, prgs.pszTime, 256);
   WgetStr(ID_GENSTOP, prgs.pszStop, 256);
   prgs.fl = PRGSS_CENTER | PRGSS_BITMAP | PRGSS_FONT;
   WprogressBar(hp, ho, &prgs);
end:
   GpiDeleteBitmap(prgs.hbmp);
   tsfree(prgs.pszTitle);
   tsfree(prgs.pszPrgrss);
   tsfree(prgs.pszTime);
   tsfree(prgs.pszStop);
   tsheapmin();
}


//==========================================================================\
// se il path nell'entryfield non menziona il drive, legge drive selezionato|
// e ricostruisce path completo                                             |
// parametri:
// HWND hwnd: handle dialogo
// PINSTALLDATA pid: struttura dati installazione
// BOOL fl: FALSE = controlla path a seguito selezione nuovo drive
//          TRUE = controlla path a seguito editazione entryfield
// valore restituito:
// VOID
//==========================================================================/

VOID checkPath(HWND hwnd, PINSTALLDATA pid, BOOL fl) {
   CHAR buf[260];
   pid->fl |= INSFL_SKIPCTL;
   // legge testo entryfield
   WinQueryDlgItemText(hwnd, EF_INSTPATH, 260, pid->tpath);
   // legge testo item selezionato nella listbox dei drive
   DlgLboxQueryItemText(hwnd, LBX_INSTDRV,
                        DlgLboxQuerySelectedItem(hwnd, LBX_INSTDRV), buf, 260);
   if (fl) {   // se editazione entryfield
      // abilita/disabilita bottone forward se testo entryfield presente/assente
      WinEnableControl(hwnd, BTN_INSTFWD, *pid->tpath);
      if (!*pid->tpath) goto end;
      // se primi caratteri entryfield rappresentano lettera drive
      if (pid->tpath[1] == ':') {  // controlla che sia un drive valido
         pid->tpath[2] = 0;
         if ((SHORT)WinSendDlgItemMsg(hwnd, LBX_INSTDRV, LM_SEARCHSTRING,
                               MPFROM2SHORT(LSS_PREFIX, LIT_FIRST),
                               MPFROMP(pid->tpath)) < 0) {
            // se il drive non Š valido usa lettera drive selezione listbox
            WinAlarm(HWND_DESKTOP, WA_ERROR);
            *pid->tpath = *buf;
            pid->tpath[2] = '\\';
            goto update;
         } /* endif */
         pid->tpath[2] = '\\';
      } /* endif */
      goto end;
   } /* endif */

   // se il path menzionato nell'entryfield non comprende il drive, lo aggiunge
   if (!strchr(pid->tpath, ':')) {
      // se path entryfield non comincia con '\' aggiunge slash
      if (pid->tpath[0] != '\\') buf[2] = '\\';
      strcpy(buf + 3, pid->tpath);
      strcpy(pid->tpath, buf);
   } else {                        // se drive gi… presente lo aggiorna
      *pid->tpath = *buf;
   } /* endif */
update:
   WinSetDlgItemText(hwnd, EF_INSTPATH, pid->tpath);
end:
   pid->fl &= ~INSFL_SKIPCTL;
}


//==========================================================================\
// Inizializza logging                                                      |
// parametri:                                                               |
// valore restituito:                                                       |
//==========================================================================/

BOOL startProcessing(PINSTALLDATA pid, HWND hprgs) {
   CHAR buf[260];
   // modifica dimensione finestra testo che mostrer… messaggio fine
   // elaborazione
   WRECTL wr;
   HWND htxt = WinWindowFromID(pid->hwnd, TXT_INSTPATH);
   WinQueryWindowRect(htxt, (PRECTL)&wr);
   WinSetWindowPos(htxt, 0, 0, 0, wr.cx, 3 * wr.cy, SWP_SIZE);
   // costruisce nome file log
   if (pid->fl & INSSEL_UININST) {
      strcpy(buf, " :\\");
      *buf = pid->boot;
   } else {
      pszcpycat(buf, 2, pid->tpath, "\\");
   } /* endif */
   strcat(buf, SZ_LOGFILE);
   if (!(pid->plf = logInit(hprgs, buf, 0x10000, (PSZ)pid->flprgrs)))
      return FALSE;
   return TRUE;
}


//==========================================================================\
// termina logging e modifica opportunamente testo mostrato secondo esito   |
// procedura                                                                |
// parametri:                                                               |
// PINSTALLDATA pid: struttura dati installazione                           |
// HWND hprgs: handle progress window                                       |
// BOOL fl: indica se la procedura Š terminata regolarmente (TRUE) o Š      |
//          terminata a causa di un errore (FALSE)                          |
// valore restituito:                                                       |
//==========================================================================/

BOOL endProcessing(PINSTALLDATA pid, HWND hprgs, BOOL fl) {
   BOOL rc;
   ULONG shift = pid->flprgrs - ID_IPRINSTLING;
   ULONG baseid = fl?
                  (pid->cerrors? ID_IENDINSTPRB: ID_IENDINSTOK):
                  ID_IENDINSTSTP;
   CHAR buf[260];
   HWND hmle = WinWindowFromID(pid->hwnd, MLE_INSTMLE);
   strcpy(buf, pid->plf->fname); // memorizza nome file log
   WinPostMsg(hprgs, PRGSM_END, MPVOID, MPVOID);
   pid->plf->hwnd = NULLHANDLE;
   logAdd(pid->plf, 0, 1, "\n\n----------------------------------------\n");
   pid->plf->hwnd = WinWindowFromID(pid->hwnd, TXT_INSTPATH);
   WinSetWindowText(hmle, SZ_NULL);
   if (0 != (rc = logEnd(pid->plf, (PSZ)(baseid + shift), (PSZ)ID_IPRELAPSED)))
      Wfil2mle(hmle, buf);
//   WsetCtlTxt(pid->hwnd, TXT_INSTPATH, baseid + shift);
   WinEnableControl(pid->hwnd, TXT_INSTPATH, TRUE);
   WinEnableControl(pid->hwnd, EF_INSTPATH, TRUE);
   WinEnableControl(pid->hwnd, BTN_INSTFWD, TRUE);
   WinEnableControl(pid->hwnd, BTN_INSTHLP, TRUE);
   return rc;
}


//==========================================================================\
// aggiunge/cancella una key da OS2.INI                                     |
// parametri:                                                               |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

BOOL instprfedit(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps,
                 PSZ app, PSZ key, PSZ val) {
   if (val) {
      logAdd(pid->plf, 0, 7,
             ID_INSTADDINI0, app, "/", key, "/", val, ID_INSTADDINI1);
   } else {
      logAdd(pid->plf, 0, 5, ID_ILOGREM, app, "/", key, ID_INSTDELINI1);
   } /* endif */
   if (!PrfWriteProfileString(HINI_USER, app, key, val)) {
      LOGERR;
      return FALSE;
   } /* endif */
   return !Wincrease(pidx, hprgs, csteps);
}


//==========================================================================\
// calcola il numero di operazioni necessarie per l'installazione           |
// parametri:                                                               |
// valore restituito:                                                       |
//==========================================================================/

ULONG instinstcsteps(PINSTALLDATA pid) {
   LONG csteps;
   PSZ aobs[] = SZFLST_OBSOLETE;
   PSZ acpy[] = SZFLST_COPY;
   PSZ aobj[] = AOBJIDS;
   pszcpycat(pid->spath + pid->cbspath, 2, SZTREENAME, "\\*");
   if (!(csteps = ioCountTree(pid->spath, "*", ALLFILES, TRUE)))
      return 0;
   csteps += CCHECKPREFRUN +        // controlla blocco preferenze
             CINSTINIITEMS +        // modifiche al file ini
             CAITEMS(aobs, PSZ) +   // cancellazione file obsoleti
             CAITEMS(acpy, PSZ) +   // nuovi file da copiare
             CINSTSYSLIB +          // copia SMARTWIN.DLL e STYLER20.DLL
             CAITEMS(aobj, PSZ);    // oggetti da creare
   pid->spath[pid->cbspath] = 0;
   return csteps;
}


//==========================================================================\
// calcola il numero di operazioni necessarie per il refresh degli oggetti  |
// parametri:                                                               |
// valore restituito:                                                       |
//==========================================================================/

ULONG instrfrshcsteps(PINSTALLDATA pid) {
   ULONG csteps;
   PSZ aobj[] = AOBJIDS;
   csteps = CAITEMS(aobj, PSZ);    // oggetti da creare
   return csteps;
}


//==========================================================================\
// calcola il numero di operazioni necessarie per la disinstallazione       |
// parametri:                                                               |
// valore restituito:                                                       |
//==========================================================================/

ULONG instuninstcsteps(PINSTALLDATA pid, HWND hprgs) {
   CHAR buf[256];
   LONG csteps = 0;
   PSZ adel[] = SZFLST_DELETE;
   PSZ aobj[] = AOBJIDS;
   // se esiste albero bitmap chiede conferma per rimuoverlo
   pszcpycat(pid->tpath + pid->cbtpath, 2, "\\", SZTREENAME);
   if (ioFExists(pid->tpath, NULL)) {
      WgetStr(ID_IQRYREMBMPS, buf, 256);
      if (WinSendMsg(hprgs, PRGSM_MSG, (MPARAM)buf, (MPARAM)PMPRNT_QUERY)) {
         pid->fl |= INSSEL_REMBMPS;
         strcat(pid->tpath, "\\*");
         // somma al contenuto del tree anche il path di installazione
         // che non viene rimosso nel caso si risponde no
         if (!(csteps = ioCountTree(pid->tpath, "*", ALLFILES, TRUE) + 1))
            goto end;
      } /* endif */
   } /* endif */
   csteps += CCHECKPREFRUN +        // controlla blocco preferenze
             CUINISTINIITEMS +      // modifiche al file ini
             CAITEMS(adel, PSZ) +   // cancellazione file
             CINSTSYSLIB;           // cancella SMARTWIN.DLL e STYLER20.DLL
   // oggetti da distruggere
   if (pid->fl & INSFL_DESKOBJS) csteps += CAITEMS(aobj, PSZ);
end:
   pid->tpath[pid->cbtpath] = 0;
   return csteps;
}


//==========================================================================\
// Se smartwin.dll risulta caricata in memoria controlla se il programma    |
// delle preferenze Š aperto ed eventualmente lo chiude                     |
// parametri:                                                               |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = continua elaborazione/interrompi elaborazione      |
//==========================================================================/

BOOL instcloseprefapp(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps) {
   PHWND phpref;
   logAdd(pid->plf, 0, 1, ID_INSTCHKPREF);
   if (pid->fl & INSFL_LOADEDDLL) {
      phpref = pid->pstlrfn();
      while (*phpref &&
             *phpref == (HWND)WinSendMsg(*phpref, WM_QUERYISTANCE,
                                         (MPARAM)*phpref, (MPARAM)*phpref)) {
         WinPostMsg(*phpref, WM_CLOSE, MPVOID, MPVOID);
         DosSleep(500);
      } /* endwhile */
   } /* endif */
   return !Wincrease(pidx, hprgs, csteps);
}


//==========================================================================\
// modifica OS2.INI:                                                        |
// SYSDLLS/LoadPerProcess/SMARTWIN                                          |
// path installazione                                                       |
// aggiunge MPTS.EXE in eccezioni titlebar, titlebarhiding, winrolling      |
// controlla opzioni avviamento se manca smartwin.ini                       |
// parametri:                                                               |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

BOOL instaddprfitems(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps) {
   CHAR buf[260];
   INT i;
   ULONG cb;
   // aggiunge STYLER20.DLL a sysdlls/loadperprocess
   logAdd(pid->plf, 0, 1, ID_INSTSYSDLLS2);
   if (prfinsstr(HINI_USER, SZPRO_SYSDLL, SZPRO_LOADPP, SZPRO_VALUE1,
                 strlen(SZPRO_VALUE1), " ", 1, PRFINSTART | PRFINZEROEND) < 1) {
      LOGERR;
      return FALSE;
   } /* endif */
   if (Wincrease(pidx, hprgs, csteps)) return FALSE;
   // aggiunge SMARTWIN.DLL a sysdlls/loadperprocess
   logAdd(pid->plf, 0, 1, ID_INSTSYSDLLS);
   if (prfinsstr(HINI_USER, SZPRO_SYSDLL, SZPRO_LOADPP, SZPRO_VALUE,
                 strlen(SZPRO_VALUE), " ", 1, PRFINEND | PRFINZEROEND) < 1) {
      LOGERR;
      return FALSE;
   } /* endif */
   if (Wincrease(pidx, hprgs, csteps)) return FALSE;
   // aggiunge path installazione
   if (!instprfedit(pid, pidx, hprgs, csteps,
                    SZPRO_APP, SZPRO_PATH, pid->tpath)) return FALSE;
   // se trova eseguibile MPTS.EXE nel path lo aggiunge alle eccezioni
   // della titlebar, titlebarhiding, windowrolling
   if (!DosSearchPath(SEARCH_ENVIRONMENT, "PATH", SZPRO_MPTS, buf, 260)) {
      PSZ apsz[] = {SZPRO_EXCTBG, SZPRO_EXCTBH, SZPRO_EXCWRL};
      cb = strlen(buf);
      buf[++cb] = 0;
      for (i = 0; i < 3; ++i) {
         logAdd(pid->plf, 0, 7, ID_INSTADDINI0, SZPRO_APP, "/",
                apsz[i], "/", buf, ID_INSTADDINI1);
         if (prfinsstr(HINI_USER, SZPRO_APP, apsz[i], buf, cb,
                       NULL, 0, PRFINSTART | PRFINZEROEND) < 0) {
            LOGNCERR;
            pid->cerrors++;
         } /* endif */
         if (Wincrease(pidx, hprgs, csteps)) return FALSE;
      } /* endfor */
   } else {
      // incrementa 3 volte il progresso
      for (i = 0; i < 3; ++i)
         if (Wincrease(pidx, hprgs, csteps)) return FALSE;
   } /* endif */
   // aggiunge startupoptions
   // controlla presenza precedente smartwin.ini
   logAdd(pid->plf, 0, 1, ID_ICHKSMWINI);
   pszcpycat(buf, 3, pid->tpath, "\\", "smartwin.ini");
   if (!ioFExists(buf, NULL)) {
      ULONG ul;
      WgetStr(ID_INSQRYSTRTUP, buf, 256);
      ul = (ULONG)WinSendMsg(hprgs, PRGSM_MSG, (MPARAM)buf, (MPARAM)PMPRNT_QUERY);
      if (!PrfWriteProfileData(HINI_USER, SZPRO_APP, SZPRO_STARTOPT,
                               (PVOID)&ul, sizeof(ULONG))) {
         LOGNCERR;
         pid->cerrors++;
      } /* endif */
   } /* endif */
   pid->tpath[pid->cbtpath] = 0;
   return !Wincrease(pidx, hprgs, csteps);
}


//==========================================================================\
// cancella file precedenti installazioni non pi— utilizzati
// parametri:
// valore restituito:
// BOOL rc: TRUE/FALSE = successo/interruzione
//==========================================================================/

BOOL instdelobsolete(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps) {
   FILEDEL fd;
   PSZ apszfile[] = SZFLST_OBSOLETE;
   INT i, irc;
   memset(&fd, 0, sizeof(FILEDEL));
   pszcpycat(fd.path, 2, pid->tpath, "\\");
   fd.hprgrs = hprgs;
   fd.cbpath = pid->cbtpath + 1;
   fd.msg = ID_ILOGREM;
   fd.err = ID_INSTPROCERR;
   fd.csteps = csteps;
   fd.pidx = pidx;
   fd.plf = pid->plf;
   for (i = 0; i < CAITEMS(apszfile, PSZ); ++i) {
      if ((irc = insfiledel(&fd, apszfile[i])) < 0) return FALSE;
      if (!irc) pid->cerrors++;
   } /* endfor */
   return TRUE;
}


//==========================================================================\
// Copia file testo, documentazione, dll, ed eseguibili nella directory di  |
// installazione. Copia smartwin.dll in \os2\dll                            |
// parametri:                                                               |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore o interruzione                     |
//==========================================================================/

BOOL instcopyfiles(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps) {
   FILECPY fc;
   PSZ apszfile[] = SZFLST_COPY;
   INT i, irc;
   memset(&fc, 0, sizeof(FILECPY));
   // pid->spath ha gi… lo slash finale!!!
   strcpy(fc.achsrc, pid->spath);
   pszcpycat(fc.achtrg, 2, pid->tpath, "\\");
   fc.hprgrs = hprgs;
   fc.cbsrc = pid->cbspath;
   fc.cbtrg = pid->cbtpath + 1;
   fc.fl = FCPYF_OVWRT | FCPYF_UNLOCK | FCPYF_STOPONERR;
   fc.msg = ID_ILOGCPY;
   fc.to =  ID_ILOGCPYTO;
   fc.err = ID_INSTPROCERR;
   fc.csteps = csteps;
   fc.pidx = pidx;
   fc.plf = pid->plf;
   // gli errori sono critici quindi non considera il contatore degli errori
   for (i = 0; i < CAITEMS(apszfile, PSZ); ++i)
      if (insfilecpy(&fc, apszfile[i], NULL) < 0) return FALSE;
   // copia smartwin.160 come smartwin.dll
   *fc.achtrg = pid->boot;
   strcpy(fc.achtrg + 1, SZ_RUNDLLPATH);
   fc.cbtrg = strlen(fc.achtrg);
   if (insfilecpy(&fc, SZSTLRCURDLL, "") < 0) return FALSE;
   // copia styler20.160 come styler20.dll
   *fc.achtrg = pid->boot;
   strcpy(fc.achtrg + 1, SZ_RUNDLL1PATH);
   fc.cbtrg = strlen(fc.achtrg);
   if (insfilecpy(&fc, SZSTLR20DLL, "") < 0) return FALSE;
   return TRUE;
}


//==========================================================================\
// Copia l'albero delle directory contenente le bitmap                      |
// parametri:                                                               |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

BOOL instcopytree(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps) {
   TREECPY tc;
   PSZ psz;
   memset(&tc, 0, sizeof(TREECPY));
   strcpy(tc.fc.achsrc, pid->spath);
   pszcpycat(tc.fc.achtrg, 2, pid->tpath, "\\");
   tc.fc.hprgrs = hprgs;
   tc.fc.cbsrc = pid->cbspath;
   tc.fc.cbtrg = pid->cbtpath + 1;
   tc.fc.fl = DosScanEnv("STLROVERWRITE", &psz)? FCPYF_OVWCONFNEW: 0;
   tc.fc.msg = ID_ILOGCPY;
   tc.fc.to =  ID_ILOGCPYTO;
   tc.fc.confirm = ID_IOVWRTNEW;
   tc.fc.err = ID_INSTPROCERR;
   tc.fc.csteps = csteps;
   tc.fc.pidx = pidx;
   tc.fc.plf = pid->plf;
   tc.pdir = SZTREENAME;
   tc.pmask = "*";
   tc.fl = ALLFILES;
   tc.msg = ID_ILOGMKPATH;
   tc.err = ID_INSTPROCERR;
   return instreecpy(&tc);
}


//==========================================================================\
// Crea la cartella Styler/2 sulla scrivania e i vari oggetti in essa       |
// contenuti. Una copia del programma di preferenze Š inclusa anche nella   |
// cartella di impostazioni del sistema                                     |
// parametri:                                                               |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

BOOL instcreatobjs(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps) {
   WPSOBJ awo[] = AOBJDATA;
   PCREATOBJ pco = tsmalloc(sizeof(CREATOBJ));
   INT i;
   logAdd(pid->plf, 0, 1, (PSZ)ID_IMKOBJS);
   if (!pco) {
      pid->cerrors++;
      return TRUE;
   } /* endif */
   memset(pco, 0, sizeof(CREATOBJ));
   pco->plf = pid->plf;
   pco->path = pid->tpath;
   pco->msg = ID_IMKOBJ;
   pco->err = ID_INSTPROCERR;
   pco->pidx = pidx;
   pco->hprgrs = hprgs;
   pco->csteps = csteps;
   for (i = 0; i < CAITEMS(awo, WPSOBJ); ++i) {
      pco->po = &awo[i];
      if (!insmakeobj(pco)) {
         tsfree(pco);
         return FALSE;
      } /* endif */
   } /* endfor */
   pid->cerrors += pco->cerr;
   tsfree(pco);
   return TRUE;
}


//==========================================================================\
// cancella le voci aggiunte al file INI dall'installazione                 |
// parametri:                                                               |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

BOOL instdelprfitems(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps) {
   CHAR buf[260];
   INT i;
   // rimuove SMARTWIN.DLL da sysdlls/loadperprocess
   logAdd(pid->plf, 0, 1, ID_IDELSYSDLLS);
   if (!prfdelstr(HINI_USER, SZPRO_SYSDLL, SZPRO_LOADPP, SZPRO_VALUE,
                  strlen(SZPRO_VALUE), " ", 1, PRFINZEROEND)) {
      LOGNCERR;
      pid->cerrors++;
   } /* endif */
   if (Wincrease(pidx, hprgs, csteps)) return FALSE;
   // rimuove STYLER20.DLL da sysdlls/loadperprocess
   logAdd(pid->plf, 0, 1, ID_IDELSYSDLLS2);
   if (!prfdelstr(HINI_USER, SZPRO_SYSDLL, SZPRO_LOADPP, SZPRO_VALUE1,
                  strlen(SZPRO_VALUE1), " ", 1, PRFINZEROEND)) {
      LOGNCERR;
      pid->cerrors++;
   } /* endif */
   if (Wincrease(pidx, hprgs, csteps)) return FALSE;
   // rimuove completamente l'applicazione Smart Windows da OS2.INI
   if (!PrfWriteProfileData(HINI_USER, SZPRO_APP, NULL, NULL, 0)) {
      LOGNCERR;
      pid->cerrors++;
   } /* endif */
   return !Wincrease(pidx, hprgs, csteps);
}


//==========================================================================\
// Cancella albero directory bitmaps                                        |
// parametri:                                                               |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

BOOL instdeltree(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps) {
   TREEDEL td;
   memset(&td, 0, sizeof(TREEDEL));
   pszcpycat(td.fd.path, 2, pid->tpath, "\\");
   td.fd.hprgrs = hprgs;
   td.fd.cbpath = pid->cbtpath + 1;
   td.fd.msg = ID_ILOGREM;
   td.fd.err = ID_INSTPROCERR;
   td.fd.csteps = csteps;
   td.fd.pidx = pidx;
   td.fd.plf = pid->plf;
   td.pdir = SZTREENAME;
   td.pmask = "*";
   td.fl = ALLFILES;
   td.msg = ID_ILOGREM;
   td.err = ID_INSTPROCERR;
   return instreedel(&td);
}


//==========================================================================\
// cancella file contenuti nella directory di installazione e smartwin.dll  |
// parametri:                                                               |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

BOOL instdelfiles(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps) {
   FILEDEL fd;
   PSZ apszfile[] = SZFLST_DELETE;
   INT i, irc;
   memset(&fd, 0, sizeof(FILEDEL));
   pszcpycat(fd.path, 2, pid->tpath, "\\");
   fd.hprgrs = hprgs;
   fd.cbpath = pid->cbtpath + 1;
   fd.msg = ID_ILOGREM;
   fd.err = ID_INSTPROCERR;
   fd.csteps = csteps;
   fd.pidx = pidx;
   fd.plf = pid->plf;
   for (i = 0; i < CAITEMS(apszfile, PSZ); ++i) {
      if ((irc = insfiledel(&fd, apszfile[i])) < 0) return FALSE;
      if (!irc) pid->cerrors++;
   } /* endfor */
   // cancella smartwin.dll da \OS2\DLL
   *fd.path = pid->boot;
   strcpy(fd.path + 1, SZ_RUNDLLPATH);
   apszfile[0] = ioFNameFromPath(fd.path);
   fd.cbpath = (ULONG)(apszfile[0] - fd.path);
   if ((irc = insfiledel(&fd, apszfile[0])) < 0) return FALSE;
   if (!irc) pid->cerrors++;
   // cancella styler20.dll da \OS2\DLL
   *fd.path = pid->boot;
   strcpy(fd.path + 1, SZ_RUNDLL1PATH);
   apszfile[0] = ioFNameFromPath(fd.path);
   fd.cbpath = (ULONG)(apszfile[0] - fd.path);
   if ((irc = insfiledel(&fd, apszfile[0])) < 0) return FALSE;
   if (!irc) pid->cerrors++;
   return TRUE;
}


//==========================================================================\
// Distrugge gli oggetti creati dall'installazione del programma            |
// parametri:                                                               |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

BOOL instdelobjs(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps) {
   PSZ aobj[] = AOBJIDS;
   INT i;
   HOBJECT hobj;
   if (!(pid->fl & INSFL_DESKOBJS)) return TRUE;
   for (i = 0; i < CAITEMS(aobj, PSZ); ++i) {
      logAdd(pid->plf, 0, 2, (PSZ)ID_INSDSTROBJ, aobj[i]);
      if (0 != (hobj = WinQueryObject(aobj[i]))) {
         if (!WinDestroyObject(hobj)) {
            LOGNCERR;
            pid->cerrors++;
         } /* endif */
      } /* endif */
      if (Wincrease(pidx, hprgs, csteps)) return FALSE;
   } /* endfor */
   return TRUE;
}


//==========================================================================\
// Se in precedenza sono state cancellate le bitmap chiede se deve          |
// cancellare l'intero albero di installazione                              |
// parametri:                                                               |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

BOOL instdelinstpath(PINSTALLDATA pid, PULONG pidx, HWND hprgs, ULONG csteps) {
   if (pid->fl & INSSEL_REMBMPS) {
      Wcpycat(pid->plf->ach, 3, (PSZ)ID_IQRYREMIPTH,
              pid->tpath, (PSZ)ID_IQRYREMIPTH1);
      // Se viene confermata cancellazione path di installazione
      if (WinSendMsg(hprgs, PRGSM_MSG,
                     (MPARAM)pid->plf->ach, (MPARAM)PMPRNT_QUERY)) {
         TREEDEL td;
         PSZ pszdir;
         // si sposta nella parent directory
         if (DosSetCurrentDir("..")) {
            pid->cerrors++;
            return TRUE;
         } /* endif */
         // cancella albero
         memset(&td, 0, sizeof(TREEDEL));
         strcpy(td.fd.path, pid->tpath);
         pszdir = ioFNameFromPath(td.fd.path);
         td.fd.hprgrs = hprgs;
         td.fd.cbpath = (ULONG)(pszdir - td.fd.path);
         td.fd.msg = ID_ILOGREM;
         td.fd.err = ID_INSTPROCERR;
         td.fd.plf = pid->plf;
         td.pdir = pszdir;
         td.pmask = "*";
         td.msg = ID_ILOGREM;
         td.err = ID_INSTPROCERR;
         instreedel(&td);
         if (td.cerr) pid->cerrors++;
      } /* endif */
      if (Wincrease(pidx, hprgs, csteps)) return FALSE;
   } /* endif */
   return TRUE;
}


//==========================================================================\
// parametri:
// valore restituito:
//==========================================================================/

//==========================================================================\
// parametri:
// valore restituito:
//==========================================================================/

