//==========================================================================
// exceptions.c: dialogo impostazione eccezioni
//==========================================================================

#pragma strings(readonly)

#include "pref.h"
#include "api.h"


// prototypes ---------------------------------------------------------------
VOID ExceptionsProcInit(HWND hwnd);
VOID ExceptionsProcCommand(HWND hwnd, ULONG ulId);
VOID ExceptionsProcControl(HWND hwnd, ULONG ulId, ULONG ulEvent);
VOID ExceptionFillLbox(HWND hwnd, PSZ pszKey);
VOID ExceptionError(HWND hwnd);
PSZ ExceptionsGetKeyName(ULONG ulId);
VOID ExceptionsUpdateProfile(HWND hwnd);
VOID ExceptionsSetFlag(ULONG idx, ULONG l);


//==========================================================================
// procedura dialogo principale impostazione eccezioni
// finestra attiva e inattiva
//==========================================================================

MRESULT EXPENTRY ExceptionsProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         ExceptionsProcInit(hwnd);
         return (MRESULT)TRUE;
      case WM_COMMAND:
         ExceptionsProcCommand(hwnd, (ULONG)mp1);
         break;
      case WM_CONTROL:
         ExceptionsProcControl(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1));
         break;
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return (MRESULT)FALSE;
}


//==========================================================================
// - memorizza dati applicazione e handle finestra
// - ricava dall'ID della pagina la key da leggere in OS2.INI
// - riempie la listbox
//==========================================================================

VOID ExceptionsProcInit(HWND hwnd) {
   PSZ pszKey;
   // memorizza l'indirizzo della struttura dati dell'applicazione
   p->pd[p->idxPage].hwnd = hwnd;       // memorizza hwnd
   pszKey = ExceptionsGetKeyName(p->idxPage);
   // riempie listbox
   ExceptionFillLbox(hwnd, pszKey);
   setparown(hwnd);   // riattiva finestra principale
}


//==========================================================================
// aggiunge/rimuove item dalla lista
//==========================================================================

VOID ExceptionsProcCommand(HWND hwnd, ULONG ulId) {
   switch (ulId) {
      case BTN_EXCEPTFIND: {
         FILEDLG fdlg;
         CHAR drv[12];
         ULONG ul;
         DosQuerySysInfo(QSV_BOOT_DRIVE, QSV_BOOT_DRIVE,
                         (PVOID)&ul, sizeof(ULONG));
         strcpy(drv, "X:\\*.exe");
         drv[0] = 'A' + ul - 1;
         memset((PVOID)&fdlg, 0, sizeof(FILEDLG));
         fdlg.cbSize = sizeof(FILEDLG);
         fdlg.fl = FDS_CENTER | FDS_OPEN_DIALOG;
         fdlg.pszTitle = (PSZ)malloc(256);
         GetStr(ID_ADDTOEXCPTLIST, fdlg.pszTitle, 256);
         fdlg.pszOKButton =(PSZ)malloc(256);
         GetStr(ID_ADD, fdlg.pszOKButton, 256);
         strcpy(fdlg.szFullFile, drv);
         if (WinFileDlg(HWND_DESKTOP, hwnd, &fdlg) &&
             fdlg.lReturn == DID_OK &&
             LIT_NONE ==             // item non presente
             (LONG)WinSendDlgItemMsg(hwnd, LBOX_EXCEPTION, LM_SEARCHSTRING,
                                    MPFROM2SHORT(0, LIT_FIRST),
                                    (MPARAM)fdlg.szFullFile)) {
            WinUpper(p->hab, 0, 0, fdlg.szFullFile);
            DlgLboxInsertItem(hwnd, LBOX_EXCEPTION, LIT_SORTASCENDING,
                              fdlg.szFullFile);
            ExceptionsUpdateProfile(hwnd);
         } /* endif */
         free((PVOID)fdlg.pszTitle);
         free((PVOID)fdlg.pszOKButton);
         break;
      }
      case BTN_EXCEPTCADD:             // bottone Add
         {
         PSZ pszend = p->buf;
         pszend += WinQueryDlgItemText(hwnd, EF_LISTITEM, 260, p->buf);
         if (LIT_NONE ==             // item non presente
             (LONG)WinSendDlgItemMsg(hwnd, LBOX_EXCEPTION, LM_SEARCHSTRING,
                             MPFROM2SHORT(0, LIT_FIRST), (MPARAM)p->buf)) {
            if (!ioFExists(p->buf, NULL)) {
               strcat(p->buf, ":\n");
               GetStr(ID_EXCFILENOTFOUND, p->buf + strlen(p->buf), 256);
               if (!Wprint(hwnd, p->buf, PMPRNT_QUERY))
                  break;
            } // end if
            WinUpper(p->hab, 0, 0, p->buf);
            *pszend = 0;
            DlgLboxInsertItem(hwnd, LBOX_EXCEPTION, LIT_SORTASCENDING, p->buf);
            ExceptionsUpdateProfile(hwnd);
         } // end if
         WinSetDlgItemText(hwnd, EF_LISTITEM, SZ_NULL);
         break;
         }
      case BTN_EXCECPTREM: {            // bottone Remove
         int i;
         while ((i = DlgLboxQuerySelectedItem(hwnd, LBOX_EXCEPTION)) !=
                 LIT_NONE)
            DlgLboxDeleteItem(hwnd, LBOX_EXCEPTION, i);
         ExceptionsUpdateProfile(hwnd);
         WinEnableControl(hwnd, BTN_EXCECPTREM, FALSE);
         break;
         } // end case BTN_EXCECPTREM
   } /* endswitch */
}


//==========================================================================
// - se ci sono dati nell'entryfield abilita pulsante Add
// - se c'Š un item selezionato abilita pulsante Remove
// parametri:
//==========================================================================

VOID ExceptionsProcControl(HWND hwnd, ULONG ulId, ULONG ulEvent) {
   if (ulId == EF_LISTITEM && ulEvent == EN_CHANGE) {
      WinEnableControl(hwnd, BTN_EXCEPTCADD,
                       WinQueryDlgItemTextLength(hwnd, EF_LISTITEM));
   } else if (ulId == LBOX_EXCEPTION && ulEvent == LN_SELECT) {
      WinEnableControl(hwnd, BTN_EXCECPTREM,
                (DlgLboxQuerySelectedItem(hwnd, LBOX_EXCEPTION) != LIT_NONE));
   } // end if
}


//==========================================================================
// - ricava lunghezza dati key se questa > 0 alloca memoria x leggerli
// - riempie la listbox
// - libera la memoria
// - se pszKey = NULL mostra messaggio errore e disabilita i controlli
// parametri:
// HWND hwnd: handle dialogo
// PSZ pszKey: chiave nel file OS2.INI da cui leggere lista eccezioni
//==========================================================================

VOID ExceptionFillLbox(HWND hwnd, PSZ pszKey) {
   ULONG ul;
   PSZ pszExc = NULL;            // indirizzo lista eccezioni
   PSZ pszCur;                   // cursore nella lista di eccezioni
   if (pszKey == NULL) {         // errore nel ricavare nome entry OS2.INI
      ExceptionError(hwnd);
   } else if (!PrfQueryProfileSize(HINI_USERPROFILE, OS2INIAPPL, pszKey, &ul)) {
      // si Š verificato errore, non dipendente dal fatto che non c'Š alcuna
      // eccezione in lista, disabilita controlli, altrimenti ritorna
      if (LOUSHORT(WinGetLastError(WinQueryAnchorBlock(hwnd))) !=
                                                     PMERR_NOT_IN_IDX) {
         ExceptionError(hwnd);
      } /* endif */
   } else if (!ul) {     // se non ci sono dati
      return;
   } else if (!(pszExc = (PSZ)malloc(ul)) ||
              !PrfQueryProfileData(HINI_USERPROFILE, OS2INIAPPL, pszKey,
                            pszExc, &ul)) { // errore alloc o lettura INI
      ExceptionError(hwnd);
      free((PVOID)pszExc);
   } else { // se tutto a posto riempie lista
      for (pszCur = pszExc; *pszCur; pszCur+= strlen(pszCur) + 1)
         DlgLboxInsertItem(hwnd, LBOX_EXCEPTION, LIT_SORTASCENDING, pszCur);
      free((PVOID)pszExc);
   } // end if
}


//==========================================================================
// - scrive errore lettura dati nella listbox e disabilita i controlli
//   del dialogo
// parametri:
// HWND hwnd: handle dialogo
//==========================================================================

VOID ExceptionError(HWND hwnd) {
   ShowErr(ID_ERR_EXCGETLIST);
   WinEnableControl(hwnd, TXT_EXCPLIST, FALSE);
   WinEnableControl(hwnd, LBOX_EXCEPTION, FALSE);
   WinEnableControl(hwnd, EF_LISTITEM, FALSE);
   WinEnableControl(hwnd, BTN_EXCEPTCADD, FALSE);
   WinEnableControl(hwnd, BTN_EXCECPTREM, FALSE);
}


//==========================================================================
// parametri:
// HWND hwnd: handle dialogo
//==========================================================================

PSZ ExceptionsGetKeyName(ULONG ulId) {
   switch (ulId) {
      case IDPG_GEN_EXC: return SZEXC_GENERAL;
      case IDPG_MAINWIN_EXC: return SZEXC_MAINWIN;
      case IDPG_TBARG_EXC: return SZEXC_TBARGEN;
      case IDPG_TBARH_EXC: return SZEXC_TBARHIDE;
      case IDPG_WROLL_EXC: return SZEXC_WINROLL;
      case IDPG_MAXWIN_EXC: return SZEXC_MAXWIN;
      case IDPG_SZMOVE_EXC: return SZEXC_SZMOVE;
      case IDPG_TXTG_EXC: return SZEXC_TXTCTL;
      case IDPG_VIO_EXC: return SZEXC_VIOWIN;
      case IDPG_MOUSEG_EXC: return SZEXC_MOUSE;
      default: return SZ_NULL;
   } /* endswitch */
}


//==========================================================================
// parametri:
// HWND hwnd: handle dialogo
//==========================================================================

VOID ExceptionsUpdateProfile(HWND hwnd) {
   PSZ pszData = NULL;
   PSZ pszCur, pszEnd;
   LONG l, idx; // usato prima x numero item listbox poi x dimensione dati
   if ((l = DlgLboxQueryItemCount(hwnd, LBOX_EXCEPTION) - 1) >= 0) {
      // alloca memoria x leggere lista eccezioni
      if (!(pszData = (PSZ)malloc(0x10000))) {
         ShowErr(ID_ERR_EXCUPDATEPRO);
         return;
      } /* endif */
      // legge la listbox e scrive nel buffer i dati separandoli da \0
      pszCur = pszData;
      pszEnd = pszData + 0x10000 - 1;
      do {
         pszCur += DlgLboxQueryItemText(hwnd, LBOX_EXCEPTION, l, pszCur,
                                        ((pszEnd - pszCur) < 260?
                                         pszEnd - pszCur: 260)) + 1;
      } while (l--); /* enddo */
      *(pszCur++) = 0;
      l = pszCur - pszData;
   } else {
      l = 0;
   } /* endif */

   if ((idx = hwnd2pageIdx(hwnd, p->pd)) < 0 ||
       !(pszCur = ExceptionsGetKeyName(idx)) ||
       !PrfWriteProfileData(HINI_USERPROFILE, OS2INIAPPL, pszCur,
                            pszData, l)) {
      ShowErr(ID_ERR_EXCUPDATEPRO);
   } else {
      ExceptionsSetFlag(idx, l);
   } /* endif */
   free((PVOID)pszData);
}

// setta flag presenza eccezioni

VOID ExceptionsSetFlag(ULONG idx, ULONG l) {
   l = l > 0? 1: 0;
#ifdef DEMOVERSION
   return;
#endif
   switch (idx) {
      case IDPG_GEN_EXC: p->ocur.gen.exc = p->po->gen.exc = l; break;
      case IDPG_MAINWIN_EXC: p->ocur.dlg.exc = p->po->dlg.exc = l; break;
      case IDPG_TBARG_EXC: p->ocur.tb.exc = p->po->tb.exc = l; break;
      case IDPG_TBARH_EXC: p->ocur.tbh.exc = p->po->tbh.exc = l; break;
      case IDPG_WROLL_EXC: p->ocur.roll.exc = p->po->roll.exc = l; break;
      case IDPG_MAXWIN_EXC: p->ocur.maxwin.exc = p->po->maxwin.exc = l; break;
      case IDPG_SZMOVE_EXC: p->ocur.sizpos.exc = p->po->sizpos.exc = l; break;
      case IDPG_TXTG_EXC: p->ocur.txt.exc = p->po->txt.exc = l; break;
      case IDPG_VIO_EXC: p->ocur.vio.exc = p->po->vio.exc = l; break;
      case IDPG_MOUSEG_EXC: p->ocur.mou.exc = p->po->mou.exc = l; break;
      default: break;
   } /* endswitch */
}
