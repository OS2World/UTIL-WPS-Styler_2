//==========================================================================
// general.c: pagina impostazioni generali
//==========================================================================

#pragma strings(readonly)

#include "pref.h"
#include "api.h"

// prototypes ---------------------------------------------------------------
VOID GeneralInit(HWND hwnd, POPTIONS po);
VOID GeneralControl(HWND hwnd, ULONG idCtl, ULONG ulEvent);
VOID GeneralCommand(HWND hwnd, ULONG idCtl);
VOID GeneralUndo(HWND hwnd);


//==========================================================================
// procedura dialogo opzioni titlebar hiding
//==========================================================================

MRESULT EXPENTRY GeneralOptProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         p->pd[p->idxPage].hwnd = hwnd;
         GeneralInit(hwnd, &p->ocur);
         // riattiva finestra principale
         setparown(hwnd);
         break;
      case WM_CONTROL:
         GeneralControl(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1));
         break;
      case WM_COMMAND:
         GeneralCommand(hwnd, SHORT1FROMMP(mp1));
         break;
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return (MRESULT)FALSE;
}


//==========================================================================
// procedura utilizzata sia per l'inizializzazione del dialogo che per
// l'UNDO
// parametri:
// HWND hwnd: handle dialogo
// POPTIONS po: indirizzo struttura opzioni globali (ocur=init,
//                                                       ou=undo)
// valore restituito:
// VOID
//==========================================================================

VOID GeneralInit(HWND hwnd, POPTIONS po) {
   WinCheckButton(hwnd, CHK_SMWGAPPL, po->gen.applyoe);
   WinCheckButton(hwnd, CHK_SMWGSAVE, po->gen.saveoe);
   WinCheckButton(hwnd, CHK_SMWGLASTPAGE, po->gen.lastpage >= 0);
}


//==========================================================================
// processa il messaggio WM_CONTROL
// - se cambiato stato checkbox Enable abilita/disabilita altri controlli
// parametri:
// HWND hwnd: handle dialogo
// ULONG idCtl: identit… controllo
// ULONG ulEvent: identit… evento
// valore restituito:
// VOID
//==========================================================================

VOID GeneralControl(HWND hwnd, ULONG idCtl, ULONG ulEvent) {
   switch (idCtl) {
      case CHK_SMWGAPPL:   // check box
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->gen.applyoe = p->ocur.gen.applyoe =
                           WinQueryButtonCheckstate(hwnd, CHK_SMWGAPPL);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_SMWGSAVE:   // check box
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->gen.saveoe = p->ocur.gen.saveoe =
                           WinQueryButtonCheckstate(hwnd, CHK_SMWGSAVE);
            EnableToolbarUndoBtn(1);
         } // end if
         break;
      case CHK_SMWGLASTPAGE:   // check box
         if (ulEvent == BN_CLICKED || ulEvent == BN_DBLCLICKED) {
            p->po->gen.lastpage = p->ocur.gen.lastpage =
                           WinQueryButtonCheckstate(hwnd, CHK_SMWGLASTPAGE)?
                           1: -1;
            EnableToolbarUndoBtn(1);
         } // end if
         break;
   } /* endswitch */
}


//==========================================================================
// processa il messaggio WM_COMMAND visualizzando dialoghi settaggi azioni
// mouse, tastiera o di selezione bitmap bottoni bsrrs titolo
// parametri:
// HWND hwnd: handle dialogo
// ULONG idCtl: identit… controllo
// valore restituito:
// VOID
//==========================================================================

VOID GeneralCommand(HWND hwnd, ULONG idCtl) {
   ULONG i;
   switch (idCtl) {
      // bottoni toolbar ---------------------------------------------------
      case BTN_UNDO:
         GeneralUndo(hwnd);
         break;
      case BTN_DEFAULT:
         GeneralDefault(hwnd);
         break;
      case BTN_SMWGAPPLYALL:
         GeneralApplyAll();
         break;
      case BTN_SMWGUNDOALL:
         for (i = 1; i < CPAGES; ++i) {
            if (p->pd[i].undo && p->pd[i].hwnd) {
               p->flFE = 0;
               p->idxPage = i;
               if (i == IDPG_MOUSE_FUNCS) p->flFE = FE_MOUSECLK;
               WinSendMsg(p->pd[i].hwnd, WM_COMMAND, (MPARAM)(BTN_UNDO),
                          MPFROM2SHORT(CMDSRC_OTHER, FALSE));
            } // end if
         } /* endfor */
         p->idxPage = IDPG_GENERAL;
         p->flFE = 0;
         break;
      case BTN_SMWGDEFALL: {
         PROGRESSDLG prgs;
         HPS hps = WinGetPS(hwnd);
         memset((PVOID)&prgs, 0, sizeof(PROGRESSDLG));
         prgs.hNotify = p->hObj;
         prgs.hbmp = GpiLoadBitmap(hps, p->hres, 14, 0, 0);
         if (!(prgs.pszTitle = (PSZ)malloc(256)) ||
             !(prgs.pszPrgrss = (PSZ)malloc(256)) ||
             !(prgs.pszTime = (PSZ)malloc(256)) ||
             !(prgs.pszStop = (PSZ)malloc(256))) goto end;
         GetStr(ID_DEFALLING, prgs.pszTitle, 256);
         GetStr(ID_GENPROGRESS, prgs.pszPrgrss, 256);
         GetStr(ID_GENTIMEELAPS, prgs.pszTime, 256);
         GetStr(ID_GENSTOP, prgs.pszStop, 256);
         prgs.fl = PRGSS_CENTER | PRGSS_BITMAP | PRGSS_FONT;
         WprogressBar(HWND_DESKTOP, hwnd, &prgs);
         p->idxPage = IDPG_GENERAL;
      end:
         GpiDeleteBitmap(prgs.hbmp);
         free(prgs.pszTitle);
         free(prgs.pszPrgrss);
         free(prgs.pszTime);
         free(prgs.pszStop);
         _heapmin();
         break;
      } // end case BTN_SMWGDEFALL
   } /* endswitch */
}


// undo

VOID GeneralUndo(HWND hwnd) {
   p->pd[p->idxPage].undo = 0;     // disabilita undo
   WinEnableWindow(p->swpCtls[4].hwnd, FALSE);
   // applyonexit
   p->po->gen.applyoe = p->ocur.gen.applyoe = p->ou.gen.applyoe;
   // saveonexit
   p->po->gen.saveoe = p->ocur.gen.saveoe = p->ou.gen.saveoe;
   // savelastpage
   p->po->gen.lastpage = p->ocur.gen.lastpage = p->ou.gen.lastpage;
   // aggiorna controlli dialogo
   GeneralInit(hwnd, &p->ocur);
}


// default

VOID GeneralDefault(HWND hwnd) {
   EnableToolbarUndoBtn(1);      // abilita undo
   // applyonexit
   p->po->gen.applyoe = p->ocur.gen.applyoe = 1;
   // saveonexit
   p->po->gen.saveoe = p->ocur.gen.saveoe = 1;
   // savelastpage
   p->po->gen.lastpage = p->ocur.gen.lastpage >= 0? p->ocur.gen.lastpage: 0;
   // aggiorna controlli dialogo
   if (hwnd) GeneralInit(hwnd, &p->ocur);
}

// applica settaggi correnti di tutte le pagine la cui flag apply sia attiva

VOID GeneralApplyAll(VOID) {
   LONG idxPageTtmp = p->idxPage;
   LONG flFEtmp = p->flFE;
   if (p->pd[IDPG_TBAR_ACTWIN].apply) {
      p->idxPage = IDPG_TBAR_ACTWIN;
      TbarMainOptApply(NULLHANDLE);
   } // end if
   if (p->pd[IDPG_TBAR_INACTWIN].apply) {
      p->idxPage = IDPG_TBAR_INACTWIN;
      TbarMainOptApply(NULLHANDLE);
   } // end if
   if (p->pd[IDPG_MOUSE_FUNCS].apply && p->pd[IDPG_MOUSE_FUNCS].hwnd) {
      p->idxPage = IDPG_MOUSE_FUNCS;
      p->flFE = FE_MOUSECLK;
      WinSendMsg(p->pd[IDPG_MOUSE_FUNCS].hwnd, WM_COMMAND,
                 (MPARAM)(BTN_APPLY), MPFROM2SHORT(CMDSRC_OTHER, FALSE));
   } // end if
   if (p->pd[IDPG_KEYB_FUNCS].apply && p->pd[IDPG_KEYB_FUNCS].hwnd) {
      p->idxPage = IDPG_KEYB_FUNCS;
      p->flFE = FE_KEYB;
      WinSendMsg(p->pd[IDPG_KEYB_FUNCS].hwnd, WM_COMMAND,
                 (MPARAM)(BTN_APPLY), MPFROM2SHORT(CMDSRC_OTHER, FALSE));
   } // end if
   p->idxPage = idxPageTtmp;
   p->flFE = flFEtmp;
}
