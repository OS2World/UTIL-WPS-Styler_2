//==========================================================================\
// pmappaux.c  (c) 1998-1999 * Alessandro Cantatore * Team OS/2 Italy       |
// API creazione applicazioni e finestre parti di applicazioni:
// funzioni di utilit… non esportate
//  |
// funzioni: -------------------------------------------------------------- |
// BOOL WloadAppDlg(PCREATEAPP pca);
// VOID _System objThrdProc(ULONG ul);
// BOOL startObjThrd(PCREATEAPP pca);
// VOID WfreeWres(PWDATA pw);
//  |
// API: --------------------------------------------------------------------|
//  |
//==========================================================================/

#pragma strings(readonly)

#include "common.h"
#include "pmappapi.h"
#include "api.h"


//==========================================================================\
// loop messaggi standard finestra principale applicazione:                 |
// parametri:                                                               |
// PQMSG pqmsg: coda messaggi applicazione                                  |
// BOOL fl: indica se processare il messaggio WM_QUIT in modo che           |
//          l'applicazione possa salvare i dati prima di terminare          |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID Wapploop(PQMSG pqmsg, ULONG fl) {
   forever {
      if(WinGetMsg(pa->pad->hab, pqmsg, NULLHANDLE, 0, 0)) { // loop standard
         // se deve processare help contestuale controlla WM_MOUSEMOVE
         if ((fl & APPLS_CONTXTHLP) &&
             pqmsg->msg == WM_MOUSEMOVE &&        // se MOUSEMOVE
             pa->pw->is.active &&                 // finestra attiva
             pa->pad->hctlhlp != HWND_DESKTOP &&  // messaggio errore in corso
             pqmsg->hwnd != pa->pad->hctlhlp) {   // nuovo controllo
            pa->pad->hctlhlp = pqmsg->hwnd;
            WinPostMsg(pa->pad->hObj, WM_BUBBLEHELP,
                       (MPARAM)pqmsg->hwnd, (MPARAM)pa->pw->hsbar);
         } // end if
         WinDispatchMsg(pa->pad->hab, pqmsg);
      } else if (fl & MSGLOOP_CHKQUIT) {   // se deve processare WM_QUIT
         if(pqmsg->hwnd == NULLHANDLE)                       // SHUTDOWN
            WinPostMsg(pa->pw->hwnd, WM_CLOSE, MPFROMLONG(TRUE), NULL);
         else if(pqmsg->hwnd == HWNDFROMMP(pqmsg->mp2))        // TASKLIST
            WinPostMsg(pqmsg->hwnd, WM_CLOSE, 0L, 0L);
         else                            // chiusura regolare: termina
            break;
      } else {
         break;
      } // end if
   } // end forever
}


//==========================================================================\
// crea un dialogo come finestra principale dell'applicazione               |
// parametri:                                                               |
// PCREATEAPP pca: struttura dati creazione applicazione                    |
// valore restituito:                                                       |
// BOOL rc: TRUE = successo, FALSE = errore;                                |
//==========================================================================/

BOOL WloadAppDlg(PCREATEAPP pca) {
   // crea dialogo
   if (!WinLoadDlg(HWND_DESKTOP, NULLHANDLE, pca->pw->pf,
                   pca->pad->hres, pca->pw->id, pca->pw))
      return !WinitErr(APPLERR_CREATEWIN);
   return TRUE;
}


//==========================================================================\
// procedura thread secondario                                              |
// parametri:                                                               |
// durante l'elaborazione dei messaggi che accedono a risorse allocate nel  |
// thread primario, come per esempio la struttura dati APPDATA, la object   |
// window richiede l'accesso ad un semaforo mutex, se il semaforo non Š
// disponibile, questo significa che il thread principale Š in fase di
// chiusura allora la object window posta a se stessa un WM_QUIT e al thread
// principale che sar… bloccato in attesa della chiusura del thread
// secondario un EventSemaphore
// La object window, a parte la creazione e la distruzione, ricevono solo
// messaggi espressamente postati dal thread principale, per cui la
// disponibilit… del semaforo dovrebbe essere controllata ad ogni messaggio!|
// valore restituito:                                                       |
//==========================================================================/

VOID _System objThrdProc(ULONG ul) {
   PCREATEAPP pca = (PCREATEAPP)ul;
   QMSG qmsg;
   HMQ hmqObj;
   BOOL fl = TRUE;
   hmqObj = WinCreateMsgQueue((pca->pad->habObj = WinInitialize(0)), 0);
   // solo il thread principale riceve WM_QUIT dovuti allo shutdown
   WinCancelShutdown(pca->pad->hmq, TRUE);
   // registra classe object window
   if (!WinRegisterClass(pca->pad->habObj, "objWinClass",
                         (PFNWP)pca->pfnwpobj, 0L, 4L) ||
       !(pca->pad->hObj = WinCreateWindow(HWND_OBJECT, "objWinClass", NULL, 0,
                   0, 0, 0, 0, NULLHANDLE, HWND_BOTTOM, 100, pca->pad, NULL)))
      goto endobj;
   // posta primo semaforo per indicare al thread principale che il thread
   // della object window Š pronto
   DosPostEventSem(pca->pad->hevObj);
   // message loop
   while (WinGetMsg(pca->pad->habObj, &qmsg, NULLHANDLE, 0, 0) &&
          !lockxchng(&pca->pad->ulsem, 1)) {
      pca->pad->ulsem = 0;
      WinDispatchMsg(pca->pad->habObj, &qmsg);
   } /* endif */
endobj:
   if (pca->pad->hObj) WinDestroyWindow(pca->pad->hObj);
   if (hmqObj) WinDestroyMsgQueue(hmqObj);
   if (pca->pad->habObj) WinTerminate(pca->pad->habObj);
   // se non Š saltato qui per un errore nella registrazione della classe o
   // della creazione della object window, posta semaforo per indicare al
   // thread primario che la chiusura del thread secondario Š stata completata
   DosPostEventSem(pca->pad->hevObj);
}


//==========================================================================\
// Inizia il thread secondario in cui verr… creata la object window         |
// parametri:                                                               |
// PAPPDATA pad : struttura dati applicazione                               |
// valore restituito:                                                       |
// BOOL:    TRUE  = errore                                                  |
//          FALSE = successo                                                |
//==========================================================================/

BOOL startObjThrd(PCREATEAPP pca) {
   APIRET rc;
   TID tid;
   if (DosCreateEventSem((PSZ)NULL, &pca->pad->hevObj, 0L, FALSE))
      return FALSE;
   rc = DosCreateThread(&tid, (PFNTHREAD)objThrdProc, (ULONG)pca,
                        CREATE_READY | STACK_SPARSE, THREADSTACKSZ);
   rc = !WinWaitEventSem(pca->pad->hevObj, SEM_INDEFINITE_WAIT);
   return(BOOL)rc;
}


//==========================================================================\
// libera le risorse standard di una finestra                               |
//==========================================================================/

VOID WfreeWres(PWDATA pw) {
   HWND hHelp;
   if (pw->hico) WinDestroyPointer(pw->hico);
   if (pw->hmenu) WinDestroyWindow(pw->hmenu);
   if (pw->hacc) WinDestroyAccelTable(pw->hacc);
   if (NULLHANDLE != (hHelp = WinQueryHelpInstance(pw->hwnd))) {
      WinAssociateHelpInstance(NULLHANDLE, pw->hwnd);
      WinDestroyHelpInstance(hHelp);
   } /* endif */
   if (pw->hwnd) WinDestroyWindow(pw->hwnd);
}


//==========================================================================\
//==========================================================================/

//==========================================================================\
//==========================================================================/

//==========================================================================\
//==========================================================================/


