//===========================================================================
// iniconv.c : convert pre-1.7 format to 1.7 format
//===========================================================================

#pragma strings(readonly)

#include "install.h"


// definitions --------------------------------------------------------------
typedef struct {
   OPTIONSOLD oold;
   OPTIONS onew;
   CHAR path[CCHMAXPATH];
   CHAR inifile[CCHMAXPATH];
   HINI hini;
   ULONG ul;
   LONG version;
   BOOL newINI;
} STLRINICONV, * PSTLRINICONV;

// prototypes ---------------------------------------------------------------



// globals ------------------------------------------------------------------



//===========================================================================
//
// Parameters --------------------------------------------------------------
// HWND hwnd : window handle
// VOID
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
// VOID
//===========================================================================

VOID storeBitmapData(HINI hini, PSZ pszKey, PBYTE pData, ULONG cbData) {
   if (!PrfWriteProfileData(hini, SZPRO_OPTIONS, pszKey, pData, cbData)) {
      printf("Failed to store in SMARTWIN.INI bitmap data (%s).\n"
             "The default bitmap will be used.\n");
   } /* endif */
}


//===========================================================================
//===========================================================================

VOID updateIni(VOID) {
   PSTLRINICONV p;
   if (!(p = tsmalloc(sizeof(STLRINICONV)))) return;
   memset(p, 0, sizeof(STLRINICONV));
   // get the smartwin.ini p->path
   if (!PrfQueryProfileString(HINI_USER, OS2INIAPPL,
                              "path", NULL, p->path, sizeof(p->path)))
      goto exit1;
   // build the fpull name of SMARTWIN.INI
   sprintf(p->inifile, "%s\\SMARTWIN.INI", p->path);
   // check if the file exists
   p->newINI = !ioFExists(p->inifile, NULL);
   if (!(p->hini = PrfOpenProfile(NULLHANDLE, p->inifile)))
      goto exit1;
   // check the p->version of SMARTWIN.INI data
   p->version = PrfQueryProfileInt(p->hini, SZPRO_OPTIONS, SZPRO_VERSION,
                                (p->newINI ? 170 : 0));
   if (!p->version) {
      p->version = 170;
      PrfWriteProfileData(p->hini, SZPRO_OPTIONS, SZPRO_VERSION,
                          &p->version, sizeof(int));
      PrfQueryProfileSize(p->hini, SZPRO_OPTIONS, SZPRO_GLOBAL, &p->ul);
      p->version = (p->ul == sizeof(p->onew)) ? 170 : 0;
   } /* endif */
   if (p->version >= 170) goto exit0;
   // build the name of the backup file
   strcat(p->path, "\\SMARTWIN.OLD");
   // backup smartwin.ini
   DosCopy(p->inifile, p->path, DCPY_EXISTING);
   // get current options
   p->ul = sizeof(p->oold);
   if (!PrfQueryProfileData(p->hini, SZPRO_OPTIONS, SZPRO_GLOBAL, &p->oold, &p->ul))
      goto exit0;
   // convert old options to new options format
   memset(&p->onew, 0, sizeof(p->onew));
   // general options -----------------------------------------
   p->onew.gen.disabled = p->oold.disabled;
   p->onew.gen.applyoe = p->oold.applyonexit;
   p->onew.gen.saveoe = p->oold.saveonexit;
   p->onew.gen.exc = p->oold.genexc;
   p->onew.gen.loaded = p->oold.loaded;
   p->onew.gen.lastpage = p->oold.lastpage;
   // dialog window options
   p->onew.dlg.cbFont = p->oold.MWcbFont;
   p->onew.dlg.font = p->oold.MWfont;
   p->onew.dlg.ovrPP = p->oold.MWovrPP;
   p->onew.dlg.exc = p->oold.MWexc;
   strcpy(p->onew.dlg.achFont, p->oold.MWFont);
   // titlebar options ----------------------------------------
   // active titlebar -----------------------------
   p->onew.tb.a.bkgnd = p->oold.atb.bkgnd;
   p->onew.tb.a.border = p->oold.atb.border;
   p->onew.tb.a.shade = p->oold.atb.shade;
   p->onew.tb.a._3DTxt = p->oold.atb._3DTxt;
   p->onew.tb.a.shd3 = p->oold.atb.shd3;
   p->onew.tb.a.strch = p->oold.atb.strch;
   p->onew.tb.a.center = p->oold.atb.center;
   p->onew.tb.a.clrLeftTop = p->oold.atb.clrLeftTop;
   p->onew.tb.a.clrRghtBtm = p->oold.atb.clrRghtBtm;
   p->onew.tb.a.clrHiBkgnd = p->oold.atb.clrHiBkgnd;
   p->onew.tb.a.clrShBkgnd = p->oold.atb.clrShBkgnd;
   p->onew.tb.a.clrFgTxt = p->oold.atb.clrFgTxt;
   p->onew.tb.a.clrBgTxt = p->oold.atb.clrBgTxt;
   p->onew.tb.a.size = p->oold.atb.bmpsize;
   // inactive titlebar ---------------------------
   p->onew.tb.i.bkgnd = p->oold.itb.bkgnd;
   p->onew.tb.i.border = p->oold.itb.border;
   p->onew.tb.i.shade = p->oold.itb.shade;
   p->onew.tb.i._3DTxt = p->oold.itb._3DTxt;
   p->onew.tb.i.shd3 = p->oold.itb.shd3;
   p->onew.tb.i.strch = p->oold.itb.strch;
   p->onew.tb.i.center = p->oold.itb.center;
   p->onew.tb.i.clrLeftTop = p->oold.itb.clrLeftTop;
   p->onew.tb.i.clrRghtBtm = p->oold.itb.clrRghtBtm;
   p->onew.tb.i.clrHiBkgnd = p->oold.itb.clrHiBkgnd;
   p->onew.tb.i.clrShBkgnd = p->oold.itb.clrShBkgnd;
   p->onew.tb.i.clrFgTxt = p->oold.itb.clrFgTxt;
   p->onew.tb.i.clrBgTxt = p->oold.itb.clrBgTxt;
   p->onew.tb.i.size = p->oold.itb.bmpsize;
   // titlebar general options --------------------
   strcpy(p->onew.tb.achFont, p->oold.tbb.achFont);
   p->onew.tb.close = p->oold.tbb.close;
   p->onew.tb.on = p->oold.atb.enabled;
   p->onew.tb.htbar = p->oold.tbb.htbar;
   p->onew.tb.roll = p->oold.tbb.roll;
   p->onew.tb.ovrPP = p->oold.tbb.ovrPP;
   p->onew.tb.menu = p->oold.tbb.menu;
   p->onew.tb.exc = p->oold.tbb.exc;
   // titlebar hiding options ---------------------------------
   p->onew.tbh.on = p->oold.HTon;
   p->onew.tbh.exc = p->oold.HTexc;
   p->onew.tbh.sysMn = p->oold.HTsysMn;
   p->onew.tbh.mou = p->oold.HTmou;
   p->onew.tbh.kbd = p->oold.HTkeyb;
   p->onew.tbh.noDlg = p->oold.HTxclDlg;
   p->onew.tbh.inclMn = p->oold.HTinclMn;
   p->onew.tbh.redfld = p->oold.HTredfld;
   p->onew.tbh.unhideDelay = p->oold.HTPopUpDl;
   p->onew.tbh.hideDelay = p->oold.HTHideDl;
   p->onew.tbh.moumsg = p->oold.HTmoumsg;
   p->onew.tbh.moukbd = p->oold.HTmoukey;
   p->onew.tbh.kbdKC = p->oold.HTkeybKC;
   p->onew.tbh.kbdVK = p->oold.HTkeybVK;
   // window rolling options ----------------------------------
   p->onew.roll.on = p->oold.RLon;
   p->onew.roll.exc = p->oold.RLexc;
   p->onew.roll.sysMn = p->oold.RLsysMn;
   p->onew.roll.mou = p->oold.RLmou;
   p->onew.roll.kbd = p->oold.RLkeyb;
   p->onew.roll.unrollOnFocus = p->oold.RLaunrl;
   p->onew.roll.rollOnUnfocus = p->oold.RLarl;
   p->onew.roll.letVsize = p->oold.RLvrsz;
   p->onew.roll.unrollOnVsize = p->oold.RLvrszaunrl;
   p->onew.roll.bottom = p->oold.RLbtm;
   p->onew.roll.unrollOnClose = p->oold.RLaunrlcl;
   p->onew.roll.moumsg = p->oold.RLmoumsg;
   p->onew.roll.moukbd = p->oold.RLmoukey;
   p->onew.roll.kbdKC = p->oold.RLkeybKC;
   p->onew.roll.kbdVK = p->oold.RLkeybVK;
   // maximized windows options -------------------------------
   p->onew.maxwin.on = p->oold.MXOverOn;
   p->onew.maxwin.exc = p->oold.MXexc;
   p->onew.maxwin.nomove = p->oold.MXnomove;
   p->onew.maxwin.nosize = p->oold.MXnosize;
   p->onew.maxwin.x = p->oold.MXx;
   p->onew.maxwin.y = p->oold.MXy;
   p->onew.maxwin.cx = p->oold.MXcx;
   p->onew.maxwin.cy = p->oold.MXcy;
   // window siziong-positioning ------------------------------
   p->onew.sizpos.exc = p->oold.SZMVexc;
   p->onew.sizpos.fulldrag = p->oold.SZMVfulldrag;
   p->onew.sizpos.noalign = p->oold.SZMVnoalign;
   p->onew.sizpos.snap = p->oold.SZMVsnap;
   p->onew.sizpos.framedrg = p->oold.SZMVframedrg;
   p->onew.sizpos.Zmove = p->oold.SZMVZorder;
   p->onew.sizpos.Zone = p->oold.SZMVzone;
   p->onew.sizpos.snapMoumsg = p->oold.SZMVSmoumsg;
   p->onew.sizpos.snapMoukbd = p->oold.SZMVSmoukey;
   p->onew.sizpos.frameMoumsg = p->oold.SZMVFmoumsg;
   p->onew.sizpos.frameMoukbd = p->oold.SZMVFmoukey;
   p->onew.sizpos.ZordMoumsg = p->oold.SZMVZmoumsg;
   p->onew.sizpos.ZordMoukbd = p->oold.SZMVZmoukey;
   // text controls options -----------------------------------
   p->onew.txt.exc = p->oold.TxGexc;
   p->onew.txt.efsel = p->oold.TxGefsel;
   p->onew.txt.on = p->oold.TxGMcpy;
   p->onew.txt.ef = p->oold.TxGMcpyEF;
   p->onew.txt.mle = p->oold.TxGMcpyMLE;
   p->onew.txt.stat = p->oold.TxGMcpyStxt;
   p->onew.txt.lbox = p->oold.TxGMcpyLbox;
   p->onew.txt.btn = p->oold.TxGMcpyBtn;
   p->onew.txt.selMoumsg = p->oold.TxGefselMby;
   p->onew.txt.selMoukbd = p->oold.TxGefselKby;
   p->onew.txt.cpyMoumsg = p->oold.TxGMcpyMby;
   p->onew.txt.cpyMoukbd = p->oold.TxGMcpyKby;
   p->onew.txt.cutMoumsg = p->oold.TxGMcutMby;
   p->onew.txt.cutMoukbd = p->oold.TxGMcutKby;
   p->onew.txt.insMoumsg = p->oold.TxGMinsMby;
   p->onew.txt.insMoukbd = p->oold.TxGMinsKby;
   p->onew.txt.ovwMoumsg = p->oold.TxGMovwMby;
   p->onew.txt.ovwMoukbd = p->oold.TxGMovwKby;
   // VIO windows options -------------------------------------
   p->onew.vio.exc = p->oold.VIOexc;
   p->onew.vio.noscrl = p->oold.VIOnoHscrl;
   p->onew.vio.ins = p->oold.VIOins;
   p->onew.vio.mou = p->oold.VIOmcpy;
   p->onew.vio.kbd = p->oold.VIOkcpy;
   p->onew.vio.cpyMoumsg = p->oold.VIOmCmsg;
   p->onew.vio.cpyMoukbd = p->oold.VIOmCkey;
   p->onew.vio.cpyaMoumsg = p->oold.VIOmAmsg;
   p->onew.vio.cpyaMoukbd = p->oold.VIOmAkey;
   p->onew.vio.insMoumsg = p->oold.VIOmPmsg;
   p->onew.vio.insMoukbd = p->oold.VIOmPkey;
   p->onew.vio.cpyKbdVK = p->oold.VIOkCVK;
   p->onew.vio.cpyKbdKC = p->oold.VIOkCKC;
   p->onew.vio.cpyaKbdVK = p->oold.VIOkAVK;
   p->onew.vio.cpyaKbdKC = p->oold.VIOkAKC;
   p->onew.vio.insKbdVK = p->oold.VIOkPVK;
   p->onew.vio.insKbdKC = p->oold.VIOkPKC;
   // mouse options -------------------------------------------
   p->onew.mou.exc = p->oold.mouexc;
   // sliding focus options -----------------------
   p->onew.mou.focusAuto = p->oold.autoFocus;
   p->onew.mou.focusNoDesk = p->oold.afNoDesktop;
   p->onew.mou.focusNoTop = p->oold.afNoSwapZ;
   p->onew.mou.focusNoNew = p->oold.afNotIfNew;
   // mouse pointer hiding ------------------------
   p->onew.mou.hide = p->oold.ptrHide;
   p->onew.mou.hideNotOnMenu = p->oold.xclMenu;
   p->onew.mou.hideNotOnBtn = p->oold.xclBtn;
   // middle mouse button options -----------------
   p->onew.mou.btn3dblClk = p->oold.btn3;
   p->onew.mou.btn3title = p->oold.btn3title;
   // mouse move to focus window options ----------
   p->onew.mou.moveToFocus = p->oold.MPMFWon;
   p->onew.mou.moveToNew = p->oold.MPMFWcreate;
   p->onew.mou.moveToDlg = p->oold.MPMFWdlg;
   p->onew.mou.moveNotBtnles = p->oold.MPMFWxclnobtn;
   p->onew.mou.moveSteps = p->oold.MPMFsteps;
   // other ---------------------------------------
   p->onew.mou.focusDelay = p->oold.dlActivate;
   p->onew.mou.hideDelay = p->oold.dlPtrHide;
   // Windows 9x keyboards extensions -------------------------
   p->onew.winkbd.left = p->oold.W95lw;
   p->onew.winkbd.right = p->oold.W95rw;
   p->onew.winkbd.menu = p->oold.W95menu;
   p->onew.winkbd.on = p->oold.W95on;
   // folder windows keyboard accelerators --------------------
   p->onew.wpskbd.on           = p->oold.FLDon       ;
   p->onew.wpskbd.sets         = p->oold.FLDprprt    ;
   p->onew.wpskbd.parent       = p->oold.FLDoppar    ;
   p->onew.wpskbd.find         = p->oold.FLDfind     ;
   p->onew.wpskbd.srtname      = p->oold.FLDsname    ;
   p->onew.wpskbd.srtext       = p->oold.FLDsext     ;
   p->onew.wpskbd.srttype      = p->oold.FLDstype    ;
   p->onew.wpskbd.srtrname     = p->oold.FLDsrname   ;
   p->onew.wpskbd.srtsize      = p->oold.FLDssize    ;
   p->onew.wpskbd.srtwrite     = p->oold.FLDswrite   ;
   p->onew.wpskbd.srtacc       = p->oold.FLDsacc     ;
   p->onew.wpskbd.srtcreat     = p->oold.FLDscreat   ;
   p->onew.wpskbd.arrange      = p->oold.FLDarrange  ;
   p->onew.wpskbd.refresh      = p->oold.FLDrfrsh    ;
   p->onew.wpskbd.setsKC       = p->oold.FLDprprtkc  ;
   p->onew.wpskbd.setsK        = p->oold.FLDprprtkk  ;
   p->onew.wpskbd.parentKC     = p->oold.FLDopparkc  ;
   p->onew.wpskbd.parentK      = p->oold.FLDopparkk  ;
   p->onew.wpskbd.findKC       = p->oold.FLDfindkc   ;
   p->onew.wpskbd.findK        = p->oold.FLDfindkk   ;
   p->onew.wpskbd.srtnameKC    = p->oold.FLDsnamekc  ;
   p->onew.wpskbd.srtnameK     = p->oold.FLDsnamekk  ;
   p->onew.wpskbd.srtextKC     = p->oold.FLDsextkc   ;
   p->onew.wpskbd.srtextK      = p->oold.FLDsextkk   ;
   p->onew.wpskbd.srttypeKC    = p->oold.FLDstypekc  ;
   p->onew.wpskbd.srttypeK     = p->oold.FLDstypekk  ;
   p->onew.wpskbd.srtrnameKC   = p->oold.FLDsrnamekc ;
   p->onew.wpskbd.srtrnameK    = p->oold.FLDsrnamekk ;
   p->onew.wpskbd.srtsizeKC    = p->oold.FLDssizekc  ;
   p->onew.wpskbd.srtsizeK     = p->oold.FLDssizekk  ;
   p->onew.wpskbd.srtwriteKC   = p->oold.FLDswritekc ;
   p->onew.wpskbd.srtwriteK    = p->oold.FLDswritekk ;
   p->onew.wpskbd.srtaccKC     = p->oold.FLDsacckc   ;
   p->onew.wpskbd.srtaccK      = p->oold.FLDsacckk   ;
   p->onew.wpskbd.srtcreatKC   = p->oold.FLDscreatkc ;
   p->onew.wpskbd.srtcreatK    = p->oold.FLDscreatkk ;
   p->onew.wpskbd.arrangeKC    = p->oold.FLDarrangekc;
   p->onew.wpskbd.arrangeK     = p->oold.FLDarrangekk;
   p->onew.wpskbd.refreshKC    = p->oold.FLDrfrshkc  ;
   p->onew.wpskbd.refreshK     = p->oold.FLDrfrshkk  ;
   // write the new options
   p->ul = sizeof(p->onew);
   PrfWriteProfileData(p->hini, SZPRO_OPTIONS, SZPRO_GLOBAL, &p->onew, p->ul);
   // bitmaps
   storeBitmapData(p->hini, SZPRO_BMPATBARDATA, p->oold.atb.bmp, TBMPSIZE);
   storeBitmapData(p->hini, SZPRO_BMPITBARDATA, p->oold.itb.bmp, TBMPSIZE);
   storeBitmapData(p->hini, SZPRO_BMPSMENUDATA, p->oold.tbb.bmpSm, TBTNBMPSZ);
   storeBitmapData(p->hini, SZPRO_BMPCLOSEDATA, p->oold.tbb.bmpCl, TBTNBMPSZ);
   storeBitmapData(p->hini, SZPRO_BMPHIDEDATA, p->oold.tbb.bmpHd, TBTNBMPSZ);
   storeBitmapData(p->hini, SZPRO_BMPMINDATA, p->oold.tbb.bmpMn, TBTNBMPSZ);
   storeBitmapData(p->hini, SZPRO_BMPRESTDATA, p->oold.tbb.bmpRs, TBTNBMPSZ);
   storeBitmapData(p->hini, SZPRO_BMPMAXDATA, p->oold.tbb.bmpMx, TBTNBMPSZ);
   storeBitmapData(p->hini, SZPRO_BMPROLLDATA, p->oold.tbb.bmpRl, TBTNBMPSZ);
   storeBitmapData(p->hini, SZPRO_BMPUNROLLDATA, p->oold.tbb.bmpUrl, TBTNBMPSZ);
   storeBitmapData(p->hini, SZPRO_BMPHTBARDATA, p->oold.tbb.bmpHtb, TBTNBMPSZ);
   storeBitmapData(p->hini, SZPRO_BMPUNHTBARDATA, p->oold.tbb.bmpUhtb, TBTNBMPSZ);

exit0 :
   PrfCloseProfile(p->hini);
exit1 :
   free(p);
}
