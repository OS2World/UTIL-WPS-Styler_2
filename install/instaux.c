//==========================================================================\
// instaux.c:funzioni ausiliarie                                            |
//                                                                          |
// BOOL instinit(int argc, char** argv, PAPPDATA pad, PWDATA pw);
// INT instinitUI(PWDATA pw);
// VOID cmdBack(PWDATA pw);
// VOID cmdFwd(PWDATA pw);
// VOID cmdCanc(PWDATA pw);
//==========================================================================/

#pragma strings(readonly)

#include "install.h"


//==========================================================================\
// funzione inizializzazione applicazione                                   |
// se si verifica un errore mostra un messaggio opportuno tramite Wprint(), |
// setta al valore opportuno pca->pad->rc e restituisce FALSE               |
// parametri:                                                               |
// int argc: numero argomenti avviamento programma                          |
// char** argv: lista argomenti programma                                   |
// PAPPDATA pad: da castare alla struttura dati definita per l'applicazione |
// PWDATA pw: struttura dati finestra principale                            |
// valore restituito:                                                       |
// BOOL rc: TRUE/FALSE = successo/errore                                    |
//==========================================================================/

BOOL instinit(int argc, char** argv, PAPPDATA pad, PWDATA pw) {
   PINSTALLDATA pid = (PINSTALLDATA)pad;
   PSZ psz;
   // controlla presenza flag file installazione
   uExeFullName(pid->spath, 260);
   strcpy((psz = ioFNameFromPath(pid->spath)), INSTFLAGFILE);
   pid->cbspath = psz - pid->spath;
   if (ioFExists(pid->spath, NULL)) pid->fl = INSFL_INST;
   *psz = 0; // termina path sorgente installazione dopo slash
   // ottiene lettera drive di boot
   pid->boot = *pid->tpath = 'A' + uGetSval(QSV_BOOT_DRIVE + DOS_INFO) - 1;
   // controlla presenza SMARTWIN.DLL
   strcpy(pid->tpath + 1, SZ_RUNDLLPATH);
   if (ioFExists(pid->tpath, NULL)) {
      HMODULE hmod;
      pid->fl |= INSFL_UNINST | INSFL_RUNDLL;
      // controlla se la DLL Š caricata
      if (!DosLoadModule(NULL, 0, pid->tpath, &hmod)) {
         pid->fl |= INSFL_RFRSH | INSFL_LOADEDDLL;
         if (DosQueryProcAddr(hmod, OFN_OPENPREF, NULL, (PFN*)&pid->pstlrfn)) {
            WinitErr(6);
            return FALSE;
         } /* endif */
      } /* endif */
   } /* endif */
   // controlla presenza voce Smart Windows in os2.ini e path preced installaz
   if (PrfQueryProfileString(HINI_USER, SZ_USER_APP, SZ_USER_KEYPATH,
                             NULL, pid->tpath, 260)) {
      pid->fl |= INSFL_UNINST | INSFL_USERAPP;
   } else {  // se voce path non presente usa path default
      strcpy(pid->tpath + 1, SZ_DEFINSTPATH);
   } /* endif */
   pid->cbtpath = strlen(pid->tpath);
   // controlla presenza oggetti in os2.ini
   if (prfChkObj(SZ_DESKOBJLIST)) pid->fl |= INSFL_UNINST | INSFL_DESKOBJS;
   // flagga possibilit… aggiornamento oggetti
   if ((pid->fl & INSFL_LOADEDDLL) && (pid->fl & INSFL_USERAPP)) 
      pid->fl |= INSFL_RFRSH;
   return TRUE;
}


//==========================================================================\
// funzione inizializzazione interfaccia utente                             |
// centra il logo nella finestra                                            |
//==========================================================================/

INT instinitUI(PWDATA pw) {
   HWND hlogo = WinWindowFromID(pw->hwnd, STLRLOGO);
   WRECTL wr, wrlogo;
   SWP swp;
   MKP(pw->pad)->fl |= INSFL_SKIPCTL;
   MKP(pw->pad)->hwnd = pw->hwnd;
   if (!WinQueryWindowRect(pw->hwnd, (PRECTL)&wr) ||
       !WinQueryWindowRect(hlogo, (PRECTL)&wrlogo) ||
       !WinQueryWindowPos(WinWindowFromID(pw->hwnd, BTN_INSTBACK), &swp))
      return -1;
   WinSetWindowPos(hlogo, 0, (wr.cx - wrlogo.cx) / 2,
                   (wr.cy - (swp.y + swp.cy) - wrlogo.cy -
                   uGetSval(SV_CYTITLEBAR) - uGetSval(SV_CYBORDER)) /
                   2 + swp.y + swp.cy,
                   0, 0, SWP_MOVE);
   // preseleziona radiobutton azione default
   if (MKP(pw->pad)->fl & INSFL_INST) {
      WinCheckButton(pw->hwnd, RDB_INSTINST, 1);
      MKP(pw->pad)->fl |= INSSEL_INST;
   } else if (MKP(pw->pad)->fl & INSFL_RFRSH) {
      WinCheckButton(pw->hwnd, RDB_INSTRECR, 1);
      MKP(pw->pad)->fl |= INSSEL_RFRSH;
   } else {
      WinCheckButton(pw->hwnd, RDB_INSTUNINST, 1);
      MKP(pw->pad)->fl |= INSSEL_UININST;
   } /* endif */
   progress(HWND_DESKTOP, pw->hwnd, MKP(pw->pad), ID_IPRDRVINFO);
   // disabilita radiobutton non utilizzabili
   WinSetDlgItemText(pw->hwnd, EF_INSTPATH, MKP(pw->pad)->tpath);
   WinEnableControl(pw->hwnd, RDB_INSTINST, MKP(pw->pad)->fl & INSFL_INST);
   WinEnableControl(pw->hwnd, RDB_INSTRECR, MKP(pw->pad)->fl & INSFL_RFRSH);
   WinEnableControl(pw->hwnd, RDB_INSTUNINST, MKP(pw->pad)->fl & INSFL_UNINST);
   MKP(pw->pad)->fl &= ~INSFL_SKIPCTL;
   return 0;
}


//==========================================================================\
// porta in primo piano il pannello precedente                              |
// parametri:                                                               |
// PWDATA pw                                                                |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID cmdBack(PWDATA pw) {
   PINSTALLDATA pid = (PINSTALLDATA)pw->pad;
   switch (pid->idpage) {
      case INSTPG_SELECT: // >> pagina logo
         WinEnableControl(pw->hwnd, BTN_INSTBACK, FALSE);
         instPgSel(pw, pid, FALSE);
         DlgShowCtl(pw->hwnd, STLRLOGO, TRUE);
         break;
      case INSTPG_LIC:    // >> pagina selezione azione
         instPgLic(pw, pid, FALSE);
         instPgSel(pw, pid, TRUE);
         break;
      case INSTPG_PATH:   // >> pagina licenza
         instPgPath(pw, pid, FALSE);
         instPgLic(pw, pid, TRUE);
         break;
      case INSTPG_START:  // >> pagina selezione path o selezione azione
         instPgConf(pw, pid, FALSE);
         if (pid->fl & INSSEL_INST) {
            instPgPath(pw, pid, TRUE);
            break;
         } /* endif */
         instPgSel(pw, pid, TRUE);
         pid->idpage = INSTPG_SELECT;
         return;
   } /* endswitch */
   pid->idpage--;
}


//==========================================================================\
// porta in primo piano il pannello successivo                              |
// parametri:                                                               |
// PWDATA pw                                                                |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID cmdFwd(PWDATA pw) {
   PINSTALLDATA pid = (PINSTALLDATA)pw->pad;
   switch (pid->idpage) {
      case INSTPG_LOGO:   // >> pagina selezione azione
         WinEnableControl(pw->hwnd, BTN_INSTBACK, TRUE);
         DlgShowCtl(pw->hwnd, STLRLOGO, FALSE);
         instPgSel(pw, pid, TRUE);
         break;
      case INSTPG_SELECT: // >> pagina lettura licenza o conferma elaborazione
         instPgSel(pw, pid, FALSE);
         if (pid->fl & INSSEL_INST) {
            instPgLic(pw, pid, TRUE);
            break;
         } /* endif */
         instPgConf(pw, pid, TRUE);
         pid->idpage = INSTPG_START;
         return;
      case INSTPG_LIC:    // >> pagina selezione path
         instPgLic(pw, pid, FALSE);
         instPgPath(pw, pid, TRUE);
         break;
      case INSTPG_PATH:   // pagina conferma inizio elaborazione
         instPgPath(pw, pid, FALSE);
         instPgConf(pw, pid, TRUE);
         break;
      case INSTPG_START:  // pagina inizio elaborazione (dlg progresso)
         instPgStart(pw, pid);
         break;
      case INSTPG_PRGRS:  // pagina file log
         WinEnableControl(pw->hwnd, BTN_INSTCANC, FALSE);
         instPgConf(pw, pid, FALSE);
         instPgLog(pw, pid);
         DlgShowCtl(pw->hwnd, MLE_INSTMLE, TRUE);
         break;
      case INSTPG_LOG:    // bottone exit: termina applicazione
         WdefDlgProc(pw->hwnd, WM_CLOSE, MPVOID, MPVOID);
         break;
   } /* endswitch */
   pid->idpage++;
}


//==========================================================================\
// termina il programma                                                     |
// parametri:                                                               |
// PWDATA pw                                                                |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID cmdCanc(PWDATA pw) {
   WdefDlgProc(pw->hwnd, WM_CLOSE, MPVOID, MPVOID);
}


//==========================================================================\
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idCtl: id controllo                                                |
// ULONG ulEvent: id evento                                                 |
// HWND hctl: tipicamente handle controllo, ma pu• essere un altro dato     |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/
//==========================================================================\
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idCtl: id controllo                                                |
// ULONG ulEvent: id evento                                                 |
// HWND hctl: tipicamente handle controllo, ma pu• essere un altro dato     |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/
//==========================================================================\
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idCtl: id controllo                                                |
// ULONG ulEvent: id evento                                                 |
// HWND hctl: tipicamente handle controllo, ma pu• essere un altro dato     |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/
//==========================================================================\
// funzione inizializzazione interfaccia utente                             |
// centra il logo nella finestra                                            |
//==========================================================================/

