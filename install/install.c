//==========================================================================\
// install.c: install applicazione multithread basata su finestra di        |
// dialogo                                                                  |
//                                                                          |
// funzioni: -------------------------------------------------------------- |
// int main(void);
// MRESULT EXPENTRY mainDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
// VOID ProcessControl(HWND hwnd, ULONG idCtl, ULONG ulEvent, HWND hctl);
// MRESULT ProcessCommand(HWND hwnd, ULONG ulId);
//==========================================================================/

#pragma strings(readonly)
#include "install.h"



// prototipi funzioni
MRESULT EXPENTRY mainDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID ProcessControl(HWND hwnd, ULONG idCtl, ULONG ulEvent, HWND hctl);
MRESULT ProcessCommand(HWND hwnd, ULONG ulId);

//==========================================================================\
// main: cancellare la riga non utilizzata secondo necessit… o meno di usare|
//       i parametri dell'eseguibile                                        |
// valore restituito:                                                       |
// int: 0 = successo                                                        |
//      1 = errore esecuzione                                               |
//      2 = errore inizializzazione applicazione                            |
//==========================================================================/

int main(void) {
   static CREATEAPP ca;      // struttura dati creazione applicazione
   static WDATA wd;          // struttura dati finestra principale applicazione
   // stringhe errore
   static PSZ aperr[] = {SZ_NULL, SZERRI_ALLOC, SZERRI_OBJWIN, SZERRI_LOADMOD,
                         SZERRI_REGCTLS, SZERRI_CREATWN, SZERRI_LOADPROC};
   // dati applicazione
   memset(&ca, 0, sizeof(CREATEAPP));
   ca.pfinit = instinit;      // nome funzione inizializzazione
   ca.pfnwpobj = instobjproc; // procedura object window
   ca.pszres = SZ_RESMODULE;  // nome modulo risorse o NULL
   // messaggio di errore di inizializzazione
   ca.ppszerr = aperr;
   ca.fl =   APPLS_OBJWIN    // crea thread secondario con object window
           | APPLS_EXTCTLS   // registra controlli PM estesi
           | APPLS_DLGWIN    // la finestra principale Š una dialog window
           ;
   ca.cbad = sizeof(INSTALLDATA); // dimensione struttura dati applicazione
                                   // definita dall'utente
   ca.pw = &wd;
   // dati finestra principale applicazione
   memset(&wd, 0, sizeof(WDATA));
   wd.cb = sizeof(WDATA);
   wd.id = DLG_INSTMAIN;
   wd.pf = mainDlgProc;              // procedura finestra principale
   wd.fcf =       // flag per finestra principale applicazione
              FCF_ICON
            | FCF_FONT
            | FCF_AUTOICON
            ;
   wd.pfinit = instinitUI;         // funzione inizializzazione UI
//   wd.pfend = NULL;          // funzione chiamata al WM_CLOSE per
                             // salvataggio dati

   // inizializza applicazione e interfaccia grafica
   WcreateApp(&ca, 0, NULL);        // versione senza parametri

   WAppInitHelp(&ca, HLP_MAIN, SZ_APPNAME, SZ_HLPFILE);
   // crea loop messaggi ed esegue l'applicazione
   return WrunApp(&ca);
}


//==========================================================================\
// procedura dialogo principale                                             |
// messaggi:                                                                |
// WM_INITDLG                                                               |
// WM_QUERYTRACKINFO                                                        |
// WM_QUERYBORDERSIZE                                                       |
// WM_WINDOWPOSCHANGED                                                      |
// WM_MINMAXFRAME                                                           |
// WM_CONTROL                                                               |
// WM_COMMAND                                                               |
// WM_BUBBLEHELP                                                            |
// WM_CLOSE                                                                 |
//==========================================================================/

MRESULT EXPENTRY mainDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_MINMAXFRAME: {
         // specificare ID controlli da nascondere/mostrare quando la
         // finestra viene minimizzata/ripristinata, il primo elemento
         // rappresenta la quantit… di controlli
         ULONG aul[] = {1, BTN_INSTBACK};
         return WdlgMinMax(hwnd, mp1, aul);
      }  break;
      case WM_CONTROL:
         ProcessControl(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1), (HWND)mp2);
         return (MRESULT)FALSE;
      case WM_COMMAND:
         return ProcessCommand(hwnd, (ULONG)mp1);
   } /* endswitch */
   return WdefDlgProc(hwnd, msg, mp1, mp2);
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

VOID ProcessControl(HWND hwnd, ULONG idCtl, ULONG ulEvent, HWND hctl) {
   PWDATA pw = WinQueryWindowPtr(hwnd, 0);
   PINSTALLDATA pid = (PINSTALLDATA)pw->pad;
   if (pid->fl & INSFL_SKIPCTL) return;
   switch (idCtl) {
      case LBX_INSTDRV:
         if (ulEvent == LN_SELECT) checkPath(hwnd, pid, FALSE);
         break;
      case EF_INSTPATH:
         if (ulEvent == EN_CHANGE) checkPath(hwnd, pid, TRUE);
         break;
      case RDB_INSTINST:
      case RDB_INSTRECR:
      case RDB_INSTUNINST:
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            ULONG aul[] = {INSSEL_INST, INSSEL_RFRSH, INSSEL_UININST};
            pid->fl &= ~(INSSEL_INST | INSSEL_RFRSH | INSSEL_UININST);
            pid->fl |= aul[idCtl - RDB_INSTINST];
         } // end if
         break;
   } /* endswitch */
}


//==========================================================================\
// HWND hwnd: handle finestra principale dialogo                            |
// MPARAM mp1: ID controllo                                                 |
// valore restituito:                                                       |
// MRESULT FALSE                                                            |
//==========================================================================/

MRESULT ProcessCommand(HWND hwnd, ULONG ulId) {
   PWDATA pw = WinQueryWindowPtr(hwnd, 0);
   switch (ulId) {
      case BTN_INSTBACK:
         cmdBack(pw);
         break;
      case BTN_INSTFWD:
         cmdFwd(pw);
         break;
      case BTN_INSTCANC:
         cmdCanc(pw);
         break;
      case BTN_INSTHLP:
         WshowInf(hwnd, ID_INF_FILE, ID_INF_INSTALL);
         break;
   } /* endswitch */
   return (MRESULT)FALSE;
}


//==========================================================================\
// parametri:
// valore restituito:
//==========================================================================/

//==========================================================================\
// parametri:
// valore restituito:
//==========================================================================/

//==========================================================================\
// parametri:
// valore restituito:
//=========================================================================/

