//==========================================================================\
// process.c: inizializzazione/terminazione dati relativi ai singoli        |
//            processi                                                      |
// PSMWVER stlrVersion(void);
// BOOL SmartWinIsExpired(VOID);
// PHWND stlrPrefWindow(VOID);
// POPTIONS stlrOptions(VOID);
// PGLOBAL SmartWinGetGlobalDataAddress(VOID);
// HWND stlrObjectWindow(VOID);
//==========================================================================/

#pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "stlrMain.h"
#include <images.h>

// shared bitmap API import -------------------------------------------------

#pragma import(GpiSetBitmapOwner,,"PMGPI.DLL",620)
BOOL APIENTRY GpiSetBitmapOwner(HPS hps, HBITMAP hbmp, PID pid);


//===========================================================================
// Return the program version.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// PSMWVER : structure holding the version data.
//===========================================================================

PSMWVER stlrVersion(VOID) {
   return &g.ver;
}


//==========================================================================\
//    restituisce l'indirizzo dei dati globali x debugging                  |
//==========================================================================/

PGLOBAL stlrGlobals(VOID) {
   return &g;
}


//===========================================================================
// Return the address of the structure holding the currently active options.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// POPTIONS : structure holding the current options.
//===========================================================================

POPTIONS stlrOptions(VOID) {
   return &o;
}


//==========================================================================\
// restituisce l'handle della object window che gestisce i timer            |
//==========================================================================/

HWND stlrObjectWindow(VOID) {
   return g.timerThrd.hwnd;
}


//===========================================================================
// This avoids that multiple prefences windows be opened.
// If no window is opened NULLHANDLE is returned, if a window is opened
// the window handle is returned.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// HWND : handle of the currently opened preferences window or NULLHANDLE
//        if no preferences window is opened.
//===========================================================================

PHWND stlrPrefWindow(VOID) {
   return &g.hwnd.pref;
}


//===========================================================================
// Get a bitmap handle from the content of a bitmap file optionally
// returning the bitmap size.
// Any OS/2 bitmap is valid (either 1.x and 2.x formats).
// If the bitmap file contains a bitmaps array only the first bitmap is
// taken into account.
// Parameters --------------------------------------------------------------
// HPS hps      : presentation space handle.
// PBYTE pbmp   : bitmap data (content of bitmap file).
// PSIZEL psize : (output) if not NULL the bitmap size is stored here.
// Return value ------------------------------------------------------------
// HBITMAP : handle of the bitmap or NULLHANDLE in case of error.
//===========================================================================

HBITMAP stlrHBmpFromBmpData(HPS hps, PBYTE pbmp, PSIZEL psize) {
   HBITMAP hbmp = NULLHANDLE;
   PBITMAPFILEHEADER2 pbfh;
   // if is a bitmap array points to the first bitmap file header
   pbfh = (*((PUSHORT)(pbmp)) == BFT_BITMAPARRAY)?
          &((PBITMAPARRAYFILEHEADER2)(pbmp))->bfh2:
          (PBITMAPFILEHEADER2)(pbmp);
   // if this is a true bitmap get the bitmap handle
   if (pbfh->usType == BFT_BMAP) {
      hbmp = GpiCreateBitmap(hps, &pbfh->bmp2, CBM_INIT, pbmp + pbfh->offBits,
                             (PBITMAPINFO2)&pbfh->bmp2);
      if (psize) {
         // format 1.x
         if (((PBITMAPINFO2)&pbfh->bmp2)->cbFix == 12) {
            psize->cx = ((PBITMAPINFO)&pbfh->bmp2)->cx;
            psize->cy = ((PBITMAPINFO)&pbfh->bmp2)->cy;
         // format 2.x
         } else {
            psize->cx = ((PBITMAPINFO2)&pbfh->bmp2)->cx;
            psize->cy = ((PBITMAPINFO2)&pbfh->bmp2)->cy;
         } /* endif */
      } /* endif */
   } /* endif */
   return hbmp;
}


//===========================================================================
// Get a bitmap handle and optionally the bitmap size from a bitmap
// contained in a resources DLL.
// Parameters --------------------------------------------------------------
// HPS hps      : presentation space handle.
// HMODULE hmod : module handle.
// ULONG id     : bitmap resource id.
// PSIZEL psize : (output) if not NULL the bitmap size is stored here.
// Return value ------------------------------------------------------------
// HBITMAP : handle of the bitmap or NULLHANDLE in case of error.
//===========================================================================

HBITMAP stlrHBmpFromModule(HPS hps, HMODULE hmod, ULONG id, PSIZEL psize) {
   BITMAPINFOHEADER bih;
   HBITMAP hbmp;
   bih.cbFix = sizeof(BITMAPINFOHEADER);
   if (NULLHANDLE != (hbmp = GpiLoadBitmap(hps, hmod, id, 0, 0))) {
      if (psize) {
         if (GpiQueryBitmapParameters(hbmp, &bih)) {
            psize->cx = bih.cx;
            psize->cy = bih.cy;
         } else {
            psize->cx = psize->cy = 18;
         } /* endif */
      } /* endif */
   } /* endif */
   return hbmp;
}


//===========================================================================
// Get the bitmap data from the smartwin.ini file.
// Parameters --------------------------------------------------------------
// HINI hini   : smartwin.ini handle.
// PSZ pszKey  : bitmap to get.
// PBYTE pdata : buffer where the bitmap data must be returned.
//               It must be at least 0x10000 bytes.
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================

BOOL stlrBmpData(HINI hini, PSZ pszKey, PBYTE pdata) {
   ULONG ul = 0x10000;
   return PrfQueryProfileData(hini, SZPRO_OPTIONS, pszKey, pdata, &ul);
}


//===========================================================================
// Create a bitmap in memory setting its content according to the current
// shade settings.
// For horizontal shades a bitmap of 96 pixel * cyTitleBar is created,
// then, to draw the titlebar background, the bitmap is tiled horizontally.
// For vertical shades a bitmap of 1920 pixel * 2 is created, then, to
// draw the titlebar backgroun, the bitmap is stretched horizontally and
// tiled vertically.
// Parameters --------------------------------------------------------------
// HPS hps         : presentation space handle.
// PSHDCREATE psc  : buffer for the shade creation data.
// PTBARAIOPT ptbo : current titlebar options.
// Return value ------------------------------------------------------------
// HBITMAP : handle of the bitmap or NULLHANDLE in case of error.
//===========================================================================

HBITMAP stlrShadeHBmp(HPS hps, PSHDCREATE psc, PTBARAIOPT ptb) {
   INT i;            // indice per inizializzazione tramire loop
   LONG dither,      // usato per calcolare il dithering quando necessario
        cbline,      // bytes needed for a line
        lsteps,      // numero variazioni di colore tra colore partenza e medio
        rsteps,      // numero variazioni di colore tra colore medio e finale
        lr,          // valore temporaneo colore rosso
        lg,          // valore temporaneo colore verde
        lb,          // valore temporaneo colore blu
        dr,          // variazione colore rosso
        dg,          // variazione colore verde
        db;          // variazione colore blu
   PBYTE p;          // indirizzo per inizializzazione singoli byte
   // vertical shade --------------------------------------------------------
   if (ptb->shade) {
      memset(psc, 0, SHADEV_CB);
      lsteps = ptb->shd3? SHADEV_CY / 2: SHADEV_CY;
      rsteps = SHADEV_CY - lsteps;
      // init the bitmap header
      _shadeInitBmpHdr(&psc->bi, SHADEV_CX, SHADEV_CY);
      // calculate color deltas (color values are shifted left by 22)
      _shadeInitDeltas(dr, dg, db, ptb->clrRghtBtm.l, ptb->clrLeftTop.l, lsteps);
      // shift initial color values by 22
      _shadeInitRGB(lr, lg, lb, ptb->clrLeftTop.l);
      // parte dall'ultimo byte della bitmap
      p = psc->pb + SHADEV_CX * 3 * SHADEV_CY - 1;
      while (lsteps--) {
         for (i = SHADEV_CX; i; --i) {
            _shadeVSetPixel(p, lr, lg, lb);
         } /* endfor */
         _shadeVIncr(lr, lg, lb, dr, dg, db);
      } /* endwhile */
      if (rsteps) {
         _shadeInitDeltas(dr, dg, db, ptb->clrLeftTop.l, ptb->clrRghtBtm.l, rsteps);
         while (rsteps--) {
            for (i = SHADEV_CX; i; --i) {
               _shadeVSetPixel(p, lr, lg, lb);
            } /* endfor */
            _shadeVIncr(lr, lg, lb, dr, dg, db);
         } /* endwhile */
      } /* endif */
   // horizontal shade ------------------------------------------------------
   } else {
      memset(psc, 0, SHADEH_CB);
      lsteps = ptb->shd3? SHADEH_CX / 2: SHADEH_CX;
      rsteps = SHADEH_CX - lsteps;
      // init the bitmap header
      _shadeInitBmpHdr(&psc->bi, SHADEH_CX, SHADEH_CY);
      // calculate color deltas (color values are shifted left by 22)
      _shadeInitDeltas(dr, dg, db, ptb->clrRghtBtm.l, ptb->clrLeftTop.l, lsteps);
      // shift initial color values by 22
      _shadeInitRGB(lr, lg, lb, ptb->clrLeftTop.l);
      for (p = psc->pb; lsteps; --lsteps) {
         _shadeHDitherIncr(p, dither, lsteps, lr, lg, lb, dr, dg, db);
      } /* endfor */
      if (rsteps) {           // esegue solo quando shade a 3 colori
         _shadeInitDeltas(dr, dg, db, ptb->clrLeftTop.l, ptb->clrRghtBtm.l, rsteps);
         for (; rsteps; --rsteps) {
            _shadeHDitherIncr(p, dither, rsteps, lr, lg, lb, dr, dg, db);
         } /* endfor */
      } /* endif */
      // dithering raddoppia riga di pixel sfalsandola di 1 posizione
      cbline = SHADEH_CX * 3;
      p = psc->pb + cbline; // inizio seconda riga gradazioni colore
      // copia prima riga nella seconda sfalsandola di 1 pixel a destra
      memcpy((PVOID)(p + 3), psc->pb, cbline - 3);
      // colora primo pixel seconda riga come secondo pixel prima riga
      *p++ = *(psc->pb + 3), *p++ = *(psc->pb + 4), *p = *(psc->pb + 5);
   } /* endif */
   return GpiCreateBitmap(hps, (PBITMAPINFOHEADER2)&psc->bi,
                          CBM_INIT, psc->pb, (PBITMAPINFO2)&psc->bi);
}


//===========================================================================
// Look for bitmap data in the SMARTWIN.INI file.
// If no data is found get the default bitmap.
// Get the bitmap handle and makes it global.
// Parameters --------------------------------------------------------------
// HINI hini    : smartwin.ini handle.
// PSZ key      : key used to store the bitmap in smartwin.ini.
// PBYTE p      : buffer for getting the bitmap data.
// HPS hps      : presentation space handle.
// PSIZEL psize : (output) bitmap size (NULL if not needed).
// HMODULE hmod : resource DLL module handle.
// ULONG id     : id of bitmap resource in the resource DLL.
//                If this is 0 then no bitmap resource is loaded.
// Return value ------------------------------------------------------------
// HBITMAP : bitmap handle. NULLHANDLE in case of error or for the default
//           bitmaps.
//===========================================================================

HBITMAP stlrHBmp(HINI hini, PSZ key, PBYTE p, HPS hps,
                 PSIZEL psize, HMODULE hmod, ULONG id) {
   HBITMAP hbmp = NULLHANDLE;
   // get the bitmap handle from smartwin.ini
   if (stlrBmpData(hini, key, p))
      hbmp = stlrHBmpFromBmpData(hps, p, psize);
   // get the bitmap handle from the resources DLL
   if (!hbmp && id)
      hbmp = stlrHBmpFromModule(hps, hmod, id, psize);
   // if a bitmap handle has been allocated make it global
   if (hbmp)  GpiSetBitmapOwner(hps, hbmp, 0);
   return hbmp;
}
