//==========================================================================\
// prefutil.c: funzioni utilit… blocco x impostazioni Styler/2
//==========================================================================/

#pragma strings(readonly)

#include "pref.h"
#include "api.h"

#pragma import(GpiSetBitmapOwner,,"PMGPI.DLL",620)
BOOL APIENTRY GpiSetBitmapOwner(HPS hps, HBITMAP hbmp, PID pid);

//==========================================================================\
// Imposta il testo di un bottone come:                                     |
// "pszTerm" "pszData"                                                      |
// se pszData Š NULL lo sostituisce con SZ_DEFAULT                          |
// parametri:                                                               |
// HWND hwnd: handle del dialogo                                            |
// ULONG ulBtn: ID bottone                                                  |
// PSZ pszTerm: termine (es. "Font:", "BMP:")                               |
// PSZ pszData: dati (es. "5.System VIO", "tile.bmp")                       |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID SetCtlCompTxt(HWND hwnd, ULONG ulBtn, PSZ pszTerm, PSZ pszData) {
   CHAR buf[72];
   CHAR term[32];
   if ((ULONG)pszTerm <= 0xffff) {
      GetStr((ULONG)pszTerm, term, 32);
      pszTerm = term;
   } /* endif */
   sprintf(buf, "%.14s %.56s", pszTerm, (*pszData? pszData: SZ_DEFAULT));
   WinSetDlgItemText(hwnd, ulBtn, buf);
}


//===========================================================================
// Get the content of a bitmap file (not exceeding 64 KBs) to buf,
// Return the size of the file, -1 in case of error.
// Parameters --------------------------------------------------------------
// PSZ pszFile : bitmap file name.
// PBYTE buf   : buffer for the file content.
// Return value ------------------------------------------------------------
// ULONG : file size , -1 in case of error.
//===========================================================================

INT ReadBmpFile(PSZ pszFile, PBYTE buf) {
   HFILE hf;
   ULONG ul;
   FILESTATUS3 fs;
   INT rc = -1;
   if (!DosOpen(pszFile, &hf, &ul, 0L, 0L, OPEN_ACTION_OPEN_IF_EXISTS,
                OPEN_ACCESS_READONLY | OPEN_SHARE_DENYWRITE, NULL)) {
      if (!DosQueryFileInfo(hf, FIL_STANDARD, (PVOID)&fs, sizeof(fs))) {
         if (fs.cbFile < 0x10000) {
            if (!DosRead(hf, buf, fs.cbFile, &ul))
               rc = fs.cbFile;
         } /* endif */
      } /* endif */
      DosClose(hf);
   } // end if
   return rc;
}


//==========================================================================\
//  |
//==========================================================================/

ULONG ACCWinDlgBox(HWND hOwnr, PFNWP pfnwp, ULONG ulId, PVOID pb) {
   QMSG qmsg;
   ULONG rc;
   HWND hwnd;

   if (NULLHANDLE == (hwnd = WinLoadDlg(HWND_DESKTOP, hOwnr, pfnwp,
                                        p->hres, ulId, pb)))
      return DID_ERROR;
   WinEnableWindow(p->hMain, FALSE);
   while (WinGetMsg(p->hab, &qmsg, NULLHANDLE, 0, 0)) {
      if (qmsg.msg == WM_DLGDISMISS) {
         rc = (ULONG)qmsg.mp1;
         break;
      } else if (qmsg.msg == WM_MOUSEMOVE &&      // se MOUSEMOVE
          p->is.active &&                       // finestra attiva
          p->hCtlHelp != HWND_DESKTOP && // messaggio errore in corso
          qmsg.hwnd != p->hCtlHelp) {           // nuovo controllo
         p->hCtlHelp = qmsg.hwnd;
         WinPostMsg(p->hObj, WM_BUBBLEHELP, (MPARAM)qmsg.hwnd, MPVOID);
      } // end if
      WinDispatchMsg(p->hab, &qmsg);
   } /* endwhile */
   WinSetFocus(HWND_DESKTOP, p->hMain);
   WinDestroyWindow(hwnd);
   WinEnableWindow(p->hMain, TRUE);
   //WinSetFocus(HWND_DESKTOP, p->hMain);
   return rc;
}


//==========================================================================\
//  |
//==========================================================================/

VOID SetMouEventTxt(ULONG idstr, ULONG ulMou, ULONG flKey,
                    HWND hwnd, ULONG idctl) {
   GetStr(idstr, p->buf, 256);
   if (flKey & KC_ALT) GetStr(ID_CMPALT, strchr(p->buf, 0), 256);
   if (flKey & KC_CTRL) GetStr(ID_CMPCTRL, strchr(p->buf, 0), 256);
   if (flKey & KC_SHIFT) GetStr(ID_CMPSHIFT, strchr(p->buf, 0), 256);
   GetStr(ID_CMPMOUBTN, strchr(p->buf, 0), 256);
   switch (ulMou) {
      case WM_BUTTON1DOWN:
         strcat(p->buf, SZ_CMP1);
         break;
      case WM_BUTTON1CLICK:
         strcat(p->buf, SZ_CMP1);
         GetStr(ID_CMPCLICK, strchr(p->buf, 0), 256);
         break;
      case WM_BUTTON1DBLCLK:
         strcat(p->buf, SZ_CMP1);
         GetStr(ID_CMPDBLCLK, strchr(p->buf, 0), 256);
         break;
      case WM_BUTTON2DOWN:
         strcat(p->buf, SZ_CMP2);
         break;
      case WM_BUTTON2CLICK:
         strcat(p->buf, SZ_CMP2);
         GetStr(ID_CMPCLICK, strchr(p->buf, 0), 256);
         break;
      case WM_BUTTON2DBLCLK:
         strcat(p->buf, SZ_CMP2);
         GetStr(ID_CMPDBLCLK, strchr(p->buf, 0), 256);
         break;
      case WM_BUTTON3DOWN:
         strcat(p->buf, SZ_CMP3);
         break;
      case WM_BUTTON3CLICK:
         strcat(p->buf, SZ_CMP3);
         GetStr(ID_CMPCLICK, strchr(p->buf, 0), 256);
         break;
      case WM_BUTTON3DBLCLK:
         strcat(p->buf, SZ_CMP3);
         GetStr(ID_CMPDBLCLK, strchr(p->buf, 0), 256);
         break;
      //default:
      //   strcpy(p->buf + strlen(capt), SZ_NULL);
      //   break;
   } /* endswitch */
   WinSetDlgItemText(hwnd, idctl, p->buf);
}


//==========================================================================\
//  |
//==========================================================================/

VOID SetKbdEventTxt(ULONG idstr, ULONG ulVK, ULONG flKey,
                    HWND hwnd, ULONG idctl) {
   GetStr(idstr, p->buf, 256);
   if (flKey & KC_ALT) GetStr(ID_CMPALT, strchr(p->buf, 0), 256);
   if (flKey & KC_CTRL) GetStr(ID_CMPCTRL, strchr(p->buf, 0), 256);
   if (flKey & KC_SHIFT) GetStr(ID_CMPSHIFT, strchr(p->buf, 0), 256);
   switch (ulVK & 0xff) {
      case VK_F1:
         strcat(p->buf, SZ_FKEY);
         strcat(p->buf, SZ_CMP1);
         break;
      case VK_F2:
         strcat(p->buf, SZ_FKEY);
         strcat(p->buf, SZ_CMP2);
         break;
      case VK_F3:
         strcat(p->buf, SZ_FKEY);
         strcat(p->buf, SZ_CMP3);
         break;
      case VK_F4:
         strcat(p->buf, SZ_FKEY);
         strcat(p->buf, SZ_CMP4);
         break;
      case VK_F5:
         strcat(p->buf, SZ_FKEY);
         strcat(p->buf, SZ_CMP5);
         break;
      case VK_F6:
         strcat(p->buf, SZ_FKEY);
         strcat(p->buf, SZ_CMP6);
         break;
      case VK_F7:
         strcat(p->buf, SZ_FKEY);
         strcat(p->buf, SZ_CMP7);
         break;
      case VK_F8:
         strcat(p->buf, SZ_FKEY);
         strcat(p->buf, SZ_CMP8);
         break;
      case VK_F9:
         strcat(p->buf, SZ_FKEY);
         strcat(p->buf, SZ_CMP9);
         break;
      case VK_F10:
         strcat(p->buf, SZ_FKEY);
         strcat(p->buf, SZ_CMP10);
         break;
      case VK_F11:
         strcat(p->buf, SZ_FKEY);
         strcat(p->buf, SZ_CMP11);
         break;
      case VK_F12:
         strcat(p->buf, SZ_FKEY);
         strcat(p->buf, SZ_CMP12);
         break;
      case VK_INSERT:
         GetStr(ID_INSKEY, strchr(p->buf, 0), 256);
         break;
      case VK_DELETE:
         GetStr(ID_DELKEY, strchr(p->buf, 0), 256);
         break;
      case VK_BACKSPACE:
         GetStr(ID_BKSPKEY, strchr(p->buf, 0), 256);
         break;
//      case :
//         strcat(p->buf, );
//         strcat(p->buf, );
//         break;
     // default:
     //    strcpy(p->buf + strlen(capt), SZ_NULL);
     //    break;
   } /* endswitch */
   WinSetDlgItemText(hwnd, idctl, p->buf);
}


//==========================================================================\
// restituisce il numero di pagina a partire da hwnd                        |
// parametri: --------------------------------------------------------------|
// HWND hwnd: handle dialogo pagina                                         |
// valore restituito: ------------------------------------------------------|
// LONG numero pagina (prima pagina = 0), -1 in caso di errore              |
//==========================================================================/

LONG hwnd2pageIdx(HWND hwnd, PPAGEDATA ppd) {
   ULONG i;
   for (i = 0; i < CPAGES; ++i) {
      if (ppd[i].hwnd == hwnd) return i;
   } // end if
   return -1;
}



//==========================================================================\
// abilita bottone apply quando si verifica cambiamento settaggi            |
// parametri:                                                               |
// PAPPDATA p: indirizzo struttura dati applicazione                      |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID EnableToolbarApplyBtn(BOOL fl) {
   if (p->pd[p->idxPage].apply != fl) {
      p->pd[p->idxPage].apply = fl;
      WinEnableWindow(p->swpCtls[3].hwnd, fl);
   } /* endif */
}


//==========================================================================\
// abilita bottone undo quando si verifica cambiamento settaggi             |
// parametri:                                                               |
// PAPPDATA p: indirizzo struttura dati applicazione                      |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID EnableToolbarUndoBtn(BOOL fl) {
   if (p->pd[p->idxPage].undo != fl) {
      p->pd[p->idxPage].undo = fl;
      WinEnableWindow(p->swpCtls[4].hwnd, fl);
   } /* endif */
}


//==========================================================================\
// aggiorna il font del sample della barra del titolo e i dati mostrati     |
// nel bottone                                                              |
// parametri:                                                               |
// HWND hwnd: handle dialogo pagina preferenze                              |
// PSZ pszFont: stringa nuovo font                                          |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID ResetTBarFont(HWND hwnd, PSZ pszFont) {
   WinSetPresParam(WinWindowFromID(WinWindowFromID(hwnd, FRAMESAMPLE),
                                   TITLEBARSAMPLE),
                   PP_FONTNAMESIZE,
                   strlen(pszFont) + 1,
                   (PVOID)(pszFont));
   GetStr(ID_FONT, p->buf, 256);
   SetCtlCompTxt(hwnd, BTN_FONT, p->buf, pszFont);
}


//==========================================================================\
// mostra una message box prelevando il messaggio dalla stringtable         |
// parametri:                                                               |
// PAPPDATA p:                                                            |
// HWND hwnd   :                                                            |
// ULONG ulid  : id mstringa nella stringtable                              |
// ULONG fl    : flag usata da Wprint                                      |
// valore restituito:                                                       |
// ULONG caratteri letti da WinLoadString()                                 |
//==========================================================================/

ULONG GetStr(ULONG id, PSZ buf, ULONG cch) {
   return WinLoadString(p->hab, p->hres, id, cch, buf);
}

ULONG MsgBox(HWND hwnd, ULONG ulid, ULONG fl) {
   ULONG cch = GetStr(ulid, p->buf, 256);
   if (cch)
      Wprint(hwnd, p->buf, fl);
   else
      WinAlarm(HWND_DESKTOP, WA_ERROR);
   return cch;
}

ULONG SetCtlTxt(HWND hwnd, ULONG idctl, ULONG idstr) {
   ULONG cch = GetStr(idstr, p->buf, 256);
   WinSetDlgItemText(hwnd, idctl, p->buf);
   return cch;
}

ULONG SetWinTxt(HWND hwnd, ULONG idstr) {
   ULONG cch = GetStr(idstr, p->buf, 256);
   WinSetWindowText(hwnd, p->buf);
   return cch;
}

ULONG ShowErr(ULONG iderr) {
   ULONG cch = GetStr(iderr, p->buf, 256);
   WinPostMsg(p->hObj, OWM_SHOWERROR, (MPARAM)p->cntxt, MPVOID);
   return cch;
}


//==========================================================================\
// scrive in hwnd la versione del programma                                 |
// parametri:                                                               |
// HWND hwnd   :                                                            |
// valore restituito:                                                       |
// VOID                                                                     |
//==========================================================================/

VOID BuildVersionString(HWND hwnd) {
   PSZ pszVer = (PSZ)malloc(256);
   PSMWVER psmwv = stlrFnVersion();
   if (pszVer) {
      sprintf(pszVer, "Styler/2 V. %d.%d.%d.%d",
              psmwv->Mver, psmwv->mver, psmwv->rver, psmwv->bver);
      WinSetWindowText(hwnd, pszVer);
      free((PVOID)pszVer);
   } // end if
}


//==========================================================================\
// avvia la'applicazione pszAppl con il parametro pszParm                   |
// restituisce FALSE se fallisce, TRUE in caso di successo                  |
//==========================================================================/

HAPP WstartApp(HWND hwnd, PSZ pszAppl, PSZ pszParm, PSZ pszPath, ULONG fl) {
   PROGDETAILS prgdet;
   memset(&prgdet, 0, sizeof(PROGDETAILS));
   prgdet.Length = sizeof(PROGDETAILS);
   prgdet.progt.progc = PROG_DEFAULT;
   prgdet.progt.fbVisible = SHE_VISIBLE;
   prgdet.pszTitle = NULL;
   prgdet.pszExecutable = pszAppl;
   prgdet.pszParameters = pszParm;
   prgdet.pszStartupDir = pszPath;
   prgdet.pszIcon = NULL;
   prgdet.pszEnvironment = NULL;
   prgdet.swpInitial.hwndInsertBehind = HWND_TOP;
   prgdet.swpInitial.fl = SWP_SHOW | SWP_ACTIVATE | SWP_ZORDER;
   return WinStartApp(hwnd, &prgdet, NULL, NULL, fl);
}


//==========================================================================\
// apre o porta in primo piano una finestra di un file inf passandogli come
// parametro la stringa opportuna
//==========================================================================/
//
//BOOL WshowInf(HWND hwnd, ULONG idinf, ULONG idarg) {
//   CHAR buf[256];
//   ULONG cb = 256;
//   return (0 != (cb = GetStr(idinf, buf, cb)) &&
//          GetStr(idarg, buf + cb, 256 - cb) &&
//          (BOOL)WstartApp(hwnd, "VIEW.EXE", buf, NULL, SAF_INSTALLEDCMDLINE));
//}
//

//==========================================================================\
// aggiorna font tramite finestra dialogo font                              |
// parametri:                                                               |
// ULONG istrs: ULONG costituito da id stringa titolo + id stringa sample   |
//              shiftata di 16 posizioni a sinistra                         |
// PSZ pszfont: in/out: font corrente da modificare                         |
// valore restituito:                                                       |
// ULONG lunghezza stringa font incluso zero terminazione                   |
//==========================================================================/

ULONG setFont(HWND hwnd, ULONG istrs, PSZ pszfont) {
   char font [128];
   char title[72];
   char sample[72];
   ULONG ul = TRUE;
   if (!*pszfont) {
   #ifdef STLR_WARP3
      strcpy(pszfont, "8.Helv");
   #else
      strcpy(pszfont, "9.WarpSans");
   #endif
      _setPPFont(hwnd, pszfont);
   } /* endif */
   GetStr(istrs & 0xffff, title, 72);
   GetStr(istrs >> 16, sample, 72);
   strcpy(font, pszfont);
   // se si Š selezionato un nuovo font
   if (WfontDlg(hwnd, title, sample, font, 128, &ul) && ul) {
      strcpy(pszfont, font);
      ul = strlen(pszfont) + 1;
      _setPPFont(hwnd, pszfont);
      return ul;
   } else {
      return 0;
   } /* endif */
}


// resetta parent e owner delle pagine del notebook

VOID setparown(HWND hwnd) {
   WinSetParent(hwnd, p->hBook, FALSE);
   WinSetOwner(hwnd, p->hBook);
}


//===========================================================================
// Close smartwin.ini and set p->hini to NULLHANLE.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID stlrCloseProfile(VOID) {
   PrfCloseProfile(p->hini);
   p->hini = NULLHANDLE;
}


//===========================================================================
// Get the size of a bitmap from the bitmap handle.
// Parameters --------------------------------------------------------------
// HBITMAP hbmp : (input) bitmap handle.
// PSIZEL psize : (output) bitmap size.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID bitmapSizeFromHbmp(HBITMAP hbmp, PSIZEL psize) {
   BITMAPINFOHEADER bih;
   if (GpiQueryBitmapParameters(hbmp, &bih)) {
      psize->cx = bih.cx;
      psize->cy = bih.cy;
   } else {
      psize->cx = psize->cy = 18;
   } /* endif */
}


//===========================================================================
// Store the name and data of the bitmap 'idx' in SMARTWIN.INI.
// Parameters --------------------------------------------------------------
// ULONG idx : bitmap idx.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID prfStoreBitmapData(ULONG idx) {
   PSZ akey1[] = ASZPRO_BMPFILES;
   PSZ akey2[] = ASZPRO_BMPDATA;
   if (p->hini &&
       (!PrfWriteProfileString(p->hini, SZPRO_OPTIONS, akey1[idx],
                              p->abmp[idx].ach) ||
        !PrfWriteProfileData(p->hini, SZPRO_OPTIONS, akey2[idx],
                             p->abmp[idx].p, p->abmp[idx].cb)))
      ShowErr(ID_ERR_UPDBMPNAME);
}


//===========================================================================
// Delete the previous global bitmap and if 'bNew' is TRUE allocate a new
// global bitmap.
// Parameters --------------------------------------------------------------
// HPS hps     : presentation space handle.
// INT idx     : bitmap index.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID resetGlobalBitmap(HPS hps, INT idx) {
#ifndef DEMOVERSION
   PHBITMAP phbmp;
   BOOL bNew = TRUE;
   HBITMAP hbmpNew;
   PSIZEL psize = NULL;
   ULONG id;
   HMODULE hmod = NULLHANDLE;
   PSHDCREATE psc = NULL;
   PTBARAIOPT ptb;
   switch (idx) {
      case BMPATBAR:
         phbmp = &p->po->tb.a.hbmp;
         bNew = p->po->tb.a.bkgnd;
         psize = &p->po->tb.a.size;
         id = IDBMP_TBA;
         hmod = p->hres;
         if (p->po->tb.a.bkgnd == TBARBKGNDSHADE)
            psc = malloc(CB_SHADEALLOC);
         ptb = &p->po->tb.a;
         break;
      case BMPITBAR:
         phbmp = &p->po->tb.i.hbmp;
         bNew = p->po->tb.i.bkgnd;
         psize = &p->po->tb.i.size;
         id = IDBMP_TBI;
         hmod = p->hres;
         if (p->po->tb.i.bkgnd == TBARBKGNDSHADE)
            psc = malloc(CB_SHADEALLOC);
         ptb = &p->po->tb.i;
         break;
      case BMPSMENU:
         phbmp = &p->po->tb.bmp.sysmenu;
         id = SBMP_SYSMENU;
         break;
      case BMPCLOSE:
         phbmp = &p->po->tb.bmp.close;
         bNew = p->po->tb.close;
      #ifdef STLR_WARP3
         hmod = p->hres;
         id = IDBMP_CLOSE18 + (p->cyTitle > 18);
      #else
         id = SBMP_CLOSE;
      #endif
         break;
      case BMPHIDE:
         phbmp = &p->po->tb.bmp.hide;
         id = SBMP_HIDE;
         break;
      case BMPMIN:
         phbmp = &p->po->tb.bmp.min;
         id = SBMP_MINBUTTON;
         break;
      case BMPREST:
         phbmp = &p->po->tb.bmp.rest;
         id = SBMP_RESTOREBUTTON;
         break;
      case BMPMAX:
         phbmp = &p->po->tb.bmp.max;
         id = SBMP_MAXBUTTON;
         break;
      case BMPROLL:
         phbmp = &p->po->tb.bmp.roll;
         bNew = p->po->tb.roll;
         id = IDBMP_RL18 + (p->cyTitle > 18);
         hmod = p->hres;
         break;
      case BMPUNROLL:
         phbmp = &p->po->tb.bmp.unroll;
         bNew = p->po->tb.roll;
         id = IDBMP_URL18 + (p->cyTitle > 18);
         hmod = p->hres;
         break;
      case BMPHTBAR:
         phbmp = &p->po->tb.bmp.htbar;
         bNew = p->po->tb.htbar;
         id = IDBMP_HT18 + (p->cyTitle > 18);
         hmod = p->hres;
         break;
      case BMPUNHTBAR:
         phbmp = &p->po->tb.bmp.unhtbar;
         bNew = p->po->tb.htbar;
         id = IDBMP_UHT18 + (p->cyTitle > 18);
         hmod = p->hres;
         break;
   } /* endswitch */
   // delete old bitmap handle
   if (*phbmp) {
      GpiSetBitmapOwner(hps, *phbmp, p->pid);
      GpiDeleteBitmap(*phbmp);
      *phbmp = NULLHANDLE;
   } /* endif */
   // create new global bitmap handle
   if (p->po->tb.on && bNew) {
      // shade
      if (psc) {
         hbmpNew = stlrHbmpOfShade(hps, psc, ptb);
         free(psc);
      // custom bitmap
      } else if (p->abmp[idx].p) {
         hbmpNew = stlrHbmpFromData(hps, p->abmp[idx].p, psize);
      // default (resource) bitmap
      } else if (hmod) {
         hbmpNew = stlrHbmpFromModule(hps, p->hres, id, psize);
      // default (system) bitmap
      } else {
         hbmpNew = WinGetSysBitmap(HWND_DESKTOP, id);
      } /* endif */
      if (hbmpNew) {
         *phbmp = hbmpNew;
         GpiSetBitmapOwner(hps, hbmpNew, 0);
      } /* endif */
   } /* endif */
#endif
}


//===========================================================================
// The user clicked on the button to set the image used by a non standard
// frame control (window roll/unroll - titlebar hide/unhide.
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// ULONG id  : id of the bitmap beeing changed.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID featureBtnSetCmd(HWND hwnd, ULONG id) {
   ULONG fl;
   HPS hps;
   ULONG flModif;
   USHORT afl1[] = {MODIF_WROLL, MODIF_WUNROLL, MODIF_TBHIDE, MODIF_TBUNHIDE};
   USHORT afl2[] = {MODIF_WUNROLL, MODIF_WROLL, MODIF_TBUNHIDE, MODIF_TBHIDE};
   UCHAR aid2[] = {BMPUNROLL, BMPROLL, BMPUNHTBAR, BMPHTBAR};

   ULONG id2 = aid2[id - BMPROLL];
   fl = (id > BMPUNROLL) ?
            ((p->ocur.tb.htbar ? 0 : BMPSEL_DISABLE) |
               (p->abmp[id].hbmp ? 0 : BMPSEL_DEFAULT)) :
            ((p->ocur.tb.roll ? 0 : BMPSEL_DISABLE) |
               (p->abmp[id].hbmp ? 0 : BMPSEL_DEFAULT));
   if (AccLoadBmpSelDlg(hwnd, id, &fl)) {
      flModif = afl1[id - BMPROLL];
      if (id > BMPUNROLL) {
         p->ocur.tb.htbar = !(fl & BMPSEL_DISABLE);
         if (p->po->tb.htbar != p->ocur.tb.htbar) {
            p->po->tb.htbar = p->ocur.tb.htbar;
            flModif |= afl2[id - BMPROLL];
         } /* endif */
      } else {
         p->ocur.tb.roll = !(fl & BMPSEL_DISABLE);
         if (p->po->tb.roll != p->ocur.tb.roll) {
            p->po->tb.roll = p->ocur.tb.roll;
            flModif |= afl2[id - BMPROLL];
         } /* endif */
      } /* endif */
      // open SMARTWIN.INI
      p->hini = openProfile(TRUE);
      // update SMARTWIN.INI
      prfStoreBitmapData(id);
      hps = WinGetPS(hwnd);
      featureBtnSetBmp(hwnd, hps, id);
      if (!p->abmp[id].hbmp) {
         freeBmpRes(id2, TRUE);
         prfStoreBitmapData(id2);
         featureBtnSetBmp(hwnd, hps, id2);
         resetGlobalBitmap(hps, id2);
      } else if (!p->abmp[id2].hbmp) {
         featureBtnSetBmp(hwnd, hps, id2);
         resetGlobalBitmap(hps, id2);
      } // end if
      resetGlobalBitmap(hps, id);
      WinReleasePS(hps);
      stlrCloseProfile();
      EnableToolbarUndoBtn(1);
      if (flModif)
         _stlrMsgPostAll(STLR_REDRAWTITLE, flModif);
   } // end if
}



//===========================================================================
// Update the control showing the image currently used for window rolling
// or titlebar hiding frame control buttons.
// Parameters --------------------------------------------------------------
// HWND hwnd   : window handle
// HPS hps     : presentation space handle.
// ULONG idBmp : bitmap id.
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID featureBtnSetBmp(HWND hwnd, HPS hps, ULONG idBmp) {
   ULONG aidCtrl[] = {WSBMP_ROLL, WSBMP_UNROLL, WSBMP_HIDE, WSBMP_UNHIDE};
   ULONG aresid[] = {IDBMP_RL18, IDBMP_URL18, IDBMP_HT18, IDBMP_UHT18};
   HBITMAP hbmp, hDel;
   BOOL option;
   ULONG i = idBmp - BMPROLL;
   option = (idBmp > BMPUNROLL) ? p->ocur.tb.htbar : p->ocur.tb.roll;
   // in caso di inizializzazione, undo, default o selezione bmp default
   // o disabilitazione bottone chiusura
   if (!p->abmp[idBmp].hbmp) {
      // se presenti dati bitmap ne ricava l'handle
      if (p->abmp[idBmp].p) {
         hbmp = stlrHbmpFromData(hps, p->abmp[idBmp].p, NULL);
      // se non sono presenti i dati e il bottone hide/unhide non Š disabilitato
      // carica dalle risorse una bitmap dipendente dal tipo di bottone
      // (hide/unhide) dalla versione del S.O. e dalla risoluzione
      } else if (option) {
         hbmp = GpiLoadBitmap(hps, p->hres, aresid[i] + (p->cyTitle > 18), 0, 0);
      // se il bottone Š disabilitato carica bitmap nulla
      } else {
         hbmp = GpiLoadBitmap(hps, NULLHANDLE, (p->cyTitle < 22? 151: 150), 0, 0);
      } // end if
   } else {
      hbmp = p->abmp[idBmp].hbmp;
   } // end if
   hwnd = WinWindowFromID(hwnd, aidCtrl[i]);
   hDel = (HBITMAP)WinSendMsg(hwnd, SM_QUERYHANDLE, MPVOID, MPVOID);
   if (hbmp != hDel) {
      GpiDeleteBitmap(hDel);
      WinSendMsg(hwnd, SM_SETHANDLE, (MPARAM)hbmp, MPVOID);
   } /* endif */
   repaintWindow(hwnd);
}



//===========================================================================
// Apre SMARTWIN.INI.
// Parameters --------------------------------------------------------------
// BOOL bHandleError : TRUE/FALSE (handle error / just return NULLHANDLE)
// Return value ------------------------------------------------------------
// HINI : smartwin.ini handle.
//===========================================================================

HINI openProfile(BOOL bHandleError) {
   HINI hini;
#ifdef DEMOVERSION
   PSZ pszEnd;
   // apre file smartwin.ini
   uExeFullName(p->buf, 260);
   pszEnd = ioFNameFromPath(p->buf);
   strcpy(pszEnd, "SMARTWIN.INI");
   if (!(hini = PrfOpenProfile(p->hab, p->buf))) {
      if (bHandleError) ShowErr(ID_ERR_OPENPROFILE);
   } // end if
#else
   // apre file inizializzazione
   if (!(hini = stlrFnOpenProfile())) {
      if (bHandleError) ShowErr(ID_ERR_OPENPROFILE);
   } // end if
#endif
   return hini;
}


//===========================================================================
// Free the resources allocated for a bitmap.
// Parameters --------------------------------------------------------------
// ULONG idx    : bitmap index.
// BOOL primary : TRUE/FALSE (abmp/abmpu array)
// Return value ------------------------------------------------------------
// VOID
//===========================================================================

VOID freeBmpRes(ULONG idx, BOOL primary) {
   PSTLRBMP psbmp;
   psbmp = primary ? &p->abmp[idx] : &p->abmpu[idx];
   GpiDeleteBitmap(psbmp->hbmp);
   psbmp->hbmp = NULLHANDLE;
   if (primary && (psbmp->p != p->abmpu[idx].p))
      free(p->abmp[idx].p);
   psbmp->p = NULL;
   psbmp->cb = 0;
   *psbmp->ach = 0;
}


//===========================================================================
// Undo the setting for hide/unhide titlebar or roll/unroll window frame
// control images.
// Parameters --------------------------------------------------------------
// HWND hwnd   : window handle.
// HPS hps     : presentation space handle.
// ULONG idBmp : bitmap ID (index in the abmp[] array)
// ULONG flag  : flag to be returned if the bitmap must be undone.
// Return value ------------------------------------------------------------
// ULONG : modification flag.
//===========================================================================

ULONG undoFrameCtrlBmp(HWND hwnd, HPS hps, ULONG idBmp, ULONG flag) {
   if (strcmp(p->abmp[idBmp].ach, p->abmpu[idBmp].ach)) {
      freeBmpRes(idBmp, TRUE);
      p->abmp[idBmp].p = p->abmpu[idBmp].p;
      p->abmp[idBmp].cb = p->abmpu[idBmp].cb;
      if (flag) prfStoreBitmapData(idBmp);
      if (hps) resetGlobalBitmap(hps, idBmp);
      return flag ? flag : TRUE;
   } /* endif */
   return 0;
}


//===========================================================================
// Set to the default the setting for hide/unhide titlebar or roll/unroll
// window frame control images.
// Parameters --------------------------------------------------------------
// HWND hwnd   : window handle.
// HPS hps     : presentation space handle.
// ULONG idBmp : bitmap ID (index in the abmp[] array)
// ULONG flag  : flag to be returned if the bitmap must be undone.
// Return value ------------------------------------------------------------
// ULONG : modification flag.
//===========================================================================

ULONG defaultFrameCtrlBmp(HWND hwnd, HPS hps, ULONG idBmp, ULONG flag) {
   if (p->abmp[idBmp].p) {
      freeBmpRes(idBmp, TRUE);
      if (flag) prfStoreBitmapData(idBmp);
      if (hps) resetGlobalBitmap(hps, idBmp);
      return flag ? flag : TRUE;
   } /* endif */
   return 0;
}


VOID initHelp(VOID) {
   HELPINIT hi;
   memset(&hi, 0, sizeof(HELPINIT));
   hi.cb = sizeof(HELPINIT);
   hi.phtHelpTable = PHLPTBLFROMID(HLP_MAIN);
   hi.hmodHelpTableModule = p->hres;
   hi.pszHelpWindowTitle = SZ_APPNAME;
   hi.fShowPanelId = CMIC_HIDE_PANEL_ID;
   hi.pszHelpLibraryName = SZ_HLPFILE;
   if (!(p->hHelp = WinCreateHelpInstance(p->hab, &hi))) return;
   if (hi.ulReturnCode) {
      WinDestroyHelpInstance(p->hHelp);
      p->hHelp = NULLHANDLE;
   } else {
      WinAssociateHelpInstance(p->hHelp, p->hMain);
   } /* endif */
}

// termina help

VOID endHelp(VOID) {
   if (p->hHelp) {
      WinAssociateHelpInstance(NULLHANDLE, p->hMain);
      WinDestroyHelpInstance(p->hHelp);
   } /* endif */
}

