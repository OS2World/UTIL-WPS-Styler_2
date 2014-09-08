//==========================================================================\
// instpages.c: funzioni costruzione pagine programma installazione         |
//                                                                          |
// VOID instPgSel(PWDATA pw, PINSTALLDATA pid, BOOL fl);
// VOID instPgLic(PWDATA pw, PINSTALLDATA pid, BOOL fl);
// VOID instPgPath(PWDATA pw, PINSTALLDATA pid, BOOL fl);
// VOID instPgConf(PWDATA pw, PINSTALLDATA pid, BOOL fl);
// VOID instPgStart(PWDATA pw, PINSTALLDATA pid);
// VOID instPgLog(PWDATA pw, PINSTALLDATA pid);
//==========================================================================/

#pragma strings(readonly)

#include "install.h"


//==========================================================================\
// mostra pagina logo                                                       |
//==========================================================================/


//==========================================================================\
// mostra/nasconde pagina selezione azione                                  |
//==========================================================================/

VOID instPgSel(PWDATA pw, PINSTALLDATA pid, BOOL fl) {
   DlgShowCtl(pw->hwnd, GRP_ISNTSEL, fl);
   DlgShowCtl(pw->hwnd, RDB_INSTINST, fl);
   DlgShowCtl(pw->hwnd, RDB_INSTRECR, fl);
   DlgShowCtl(pw->hwnd, RDB_INSTUNINST, fl);
}


//==========================================================================\
// mostra nasconde pagina licenza                                           |
//==========================================================================/

VOID instPgLic(PWDATA pw, PINSTALLDATA pid, BOOL fl) {
   if (fl) {
      HWND hmle = WinWindowFromID(pw->hwnd, MLE_INSTMLE);
      WinSetWindowText(hmle, SZ_NULL);
      if (!Wfil2mle(hmle, SZ_INSTLICTXT)) WsetWinTxt(hmle, ID_INSTERR_NOLIC);
      WsetCtlTxt(pw->hwnd, TXT_INSTDRV, ID_INST_LICENSE);
   } /* endif */
   DlgShowCtl(pw->hwnd, MLE_INSTMLE, fl);
   DlgShowCtl(pw->hwnd, TXT_INSTDRV, fl);
   WinEnableControl(pw->hwnd, BTN_INSTFWD, TRUE);
}


//==========================================================================\
// mostra nasconde pagina selezione path installazione                      |
//==========================================================================/

VOID instPgPath(PWDATA pw, PINSTALLDATA pid, BOOL fl) {
   if (fl) {
      WsetCtlTxt(pw->hwnd, TXT_INSTDRV, ID_INST_DRIVE);
      WsetCtlTxt(pw->hwnd, TXT_INSTPATH, ID_INST_INSTPATH);
      WinSendDlgItemMsg(pw->hwnd, EF_INSTPATH, EM_SETREADONLY, MPVOID, MPVOID);
   } /* endif */
   DlgShowCtl(pw->hwnd, TXT_INSTDRV, fl);
   DlgShowCtl(pw->hwnd, LBX_INSTDRV, fl);
   DlgShowCtl(pw->hwnd, TXT_INSTPATH, fl);
   DlgShowCtl(pw->hwnd, EF_INSTPATH, fl);
}


//==========================================================================\
// mostra nasconde pagina conferma elaborazione                             |
//==========================================================================/

VOID instPgConf(PWDATA pw, PINSTALLDATA pid, BOOL fl) {
   if (fl) { // se viene mostrata la pagina
      if (pid->fl & INSSEL_INST) {
         WinSendDlgItemMsg(pw->hwnd, EF_INSTPATH, EM_SETREADONLY,
                           (MPARAM)TRUE, MPVOID);
         WsetCtlTxt(pw->hwnd, TXT_INSTPATH, ID_INST_INSTIN);
      } else if (pid->fl & INSSEL_RFRSH) {
         WsetCtlTxt(pw->hwnd, TXT_INSTPATH, ID_INST_RFRSHIN);
      } else {
         WsetCtlTxt(pw->hwnd, TXT_INSTPATH, ID_INST_UNINSTIN);
      } /* endif */
   } /* endif */
   DlgShowCtl(pw->hwnd, TXT_INSTPATH, fl);
   DlgShowCtl(pw->hwnd, EF_INSTPATH, fl && (pid->fl & INSSEL_INST));
   // se Š una procedura di installazione controlla esistenza path
   if (fl && (pid->fl & INSSEL_INST)) {
      pid->cbtpath = strlen(pid->tpath);
      // se path installazione prescelto esiste prosegue
      if (ioFExists(pid->tpath, NULL)) return;
      // chiede conferma se creare il path:
      if (WmsgBox(pw->hwnd, ID_IQRYMKPATH, PMPRNT_QUERY)) {
         // se creazione nuovo path riuscita prosegua
         if (ioMakePath(pid->tpath)) return;
         // altrimenti mostra msg errore
         WmsgBox(pw->hwnd, ID_IERRMKPATH, 0);
      } /* endif */
      // disabilita Forward e scrive di cliccare su back
      WsetCtlTxt(pw->hwnd, TXT_INSTPATH, ID_INSTREPATH);
      WinEnableControl(pw->hwnd, BTN_INSTFWD, FALSE);
   } /* endif */
}


//==========================================================================\
// da inizio al processo di elaborazione:
// disabilita bottoni e richiama progress() con il parametro opportuno
//==========================================================================/

VOID instPgStart(PWDATA pw, PINSTALLDATA pid) {
   WinEnableControl(pw->hwnd, TXT_INSTPATH, FALSE);
   WinEnableControl(pw->hwnd, EF_INSTPATH, FALSE);
   WinEnableControl(pw->hwnd, BTN_INSTBACK, FALSE);
   WinEnableControl(pw->hwnd, BTN_INSTFWD, FALSE);
   WinEnableControl(pw->hwnd, BTN_INSTCANC, FALSE);
   WinEnableControl(pw->hwnd, BTN_INSTHLP, FALSE);
   progress(pw->hwnd, pw->hwnd, pid,
            ((pid->fl & INSSEL_INST)? ID_IPRINSTLING:
              ((pid->fl & INSSEL_RFRSH)? ID_IPRRFRSHING: ID_IPRDELETING)));
}


//==========================================================================\
// mostra pagina logfile                                                    |
//==========================================================================/

VOID instPgLog(PWDATA pw, PINSTALLDATA pid) {
   WsetCtlTxt(pw->hwnd, BTN_INSTFWD, ID_INST_EXIT);
   WsetCtlTxt(pw->hwnd, TXT_INSTDRV, ID_INST_LOGFILE);
   //WinSetDlgItemText(pw->hwnd, MLE_INSTMLE, SZ_NULL);
   // aggiungere codice lettura file log
   DlgShowCtl(pw->hwnd, TXT_INSTDRV, TRUE);
   DlgShowCtl(pw->hwnd, MLE_INSTMLE, TRUE);
}


//==========================================================================\
//==========================================================================/

