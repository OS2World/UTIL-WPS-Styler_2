// images.h: definizione formato pattern, file pattern (*.ptn), costanti,
//           macro per interagire con bitmap e altre immagini

#ifndef IMAGES_H_
   #define IMAGES_H_

// dati pattern
typedef struct {
   POINTL shift;      // scostamento asse x-y disegno parte scura pattern
   BOOL zord;         // disegna prima secondo pattern
   UCHAR data[8];     // dati bitmap pattern (non allineati a ULONG)
} PATTERN, * PPATTERN;

// dati per disegnare pattern sullo schermo
typedef struct {
   POINTL shift;      // scostamento asse x-y disegno parte scura pattern
   BOOL zord;         // disegna prima secondo pattern
   HBITMAP hbmp;      // handle bitmap dati pattern
} DRAWPATTERN, * PDRAWPATTERN;
typedef PDRAWPATTERN HPATTERN;
typedef HPATTERN * PHPATTERN;

// identit… risorsa

// file pattern
typedef struct {
   CHAR signature[8];
   PATTERN ptrn;
} PATTERNFILE, * PPATTERNFILE;
// dimensione file pattern:
#define CBPTRNFILE  (sizeof(PATTERNFILE))
// identificazione file pattern
#define SZ_PATTERNSIGNATURE    "Pattern"

// costante usata per ricavare bitmap handle da pattern
#define PTRNBMPHDR  {0x10, 0x8, 0x8, 0x10001, 0xffffff, 0x0}

// restituisce un handle da un puntatore ai dati di una bitmap              |

#define WloadBmpData(hps, pbmp) \
 (HBITMAP)(GpiCreateBitmap((hps),\
           (PBITMAPINFOHEADER2)&((PBITMAPFILEHEADER2)(pbmp))->bmp2,\
           CBM_INIT, ((PBYTE)(pbmp)) + ((PBITMAPFILEHEADER2)(pbmp))->offBits,\
           (PBITMAPINFO2)&((PBITMAPFILEHEADER2)(pbmp))->bmp2))
// restituisce un handle ad una bitmap dai dati di una bitmap senza file
// header e con BITAMPINFOHEADER ridotto
#define WloadBmpBits(hps, pb) \
 (HBITMAP)(GpiCreateBitmap((hps),\
              (PBITMAPINFOHEADER2)(pb),\
              CBM_INIT, \
              ((PBYTE)(pb)) + 16 + (1 << (((PBITMAPINFO2)(pb))->cPlanes * \
                                          ((PBITMAPINFO2)(pb))->cBitCount)) *\
                                          sizeof(RGB2),\
              (PBITMAPINFO2)(pb)))

// macro per estrazione valore componente RGB da colore espresso come long
#define L2RED(l)  (((l) & 0x00ff0000) >> 16)
#define L2GRN(l)  (((l) & 0x0000ff00) >> 8)
#define L2BLU(l)  ((l) & 0x000000ff)

// macro cancella bitmap handle se non NULLHANDLE
#define FreeBmpHandle(x)       {if (x) {GpiDeleteBitmap(x); (x) = 0;}}
#define ResetBmpHandle(x, y)   {if (x) {GpiDeleteBitmap(x); (x) = (y);}}

#endif
