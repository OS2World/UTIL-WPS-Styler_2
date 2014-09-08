// shddraw.h : strutture dati usate per disegnare sfumature titlebar

#ifndef STLR_SHDDRAW_H_
   #define STLR_SHDDRAW_H_

#define SHADEV_CX  256             // larghezza bitmap sfumatura verticale
#define SHADEV_CY   64             // altezza bitmap sfumatura verticale

#define SHADEH_CX 2048             // larghezza bitmap sfumatura orizzontale
#define SHADEH_CY    2             // altezza bitmap sfumatura orizzontale

// minimal BITMAPINFO2 structure --------------------------------------------
typedef struct {
   ULONG  cbFix;            /* Length of fixed portion of structure   */
   ULONG  cx;               /* Bit-map width in pels                  */
   ULONG  cy;               /* Bit-map height in pels                 */
   USHORT cPlanes;          /* Number of bit planes                   */
   USHORT cBitCount;        /* Number of bits per pel within a plane  */
} BMPINFOM, * PBMPINFOM;

typedef struct {
   BMPINFOM bi;      // minimal bitmp header informations needed to get
                     // a bitmap handle
   BYTE pb[4];       // inizio memorizzazione dati bitmap
} SHDCREATE, * PSHDCREATE;


// quantit… di byte da allocare per la creazione della bitmap
#define SHADEV_CB           (SHADEV_CX * SHADEV_CY * 3 + sizeof(BMPINFOM))
#define SHADEH_CB           (SHADEH_CX * SHADEH_CY * 3 + sizeof(BMPINFOM))
#define CB_SHADEALLOC       SHADEV_CB

// inizializzazione header bitmap shade
#define _shadeInitBmpHdr(_pbi_, _cx_, _cy_)  \
   (_pbi_)->cbFix = 16,                      \
   (_pbi_)->cx = (_cx_),                     \
   (_pbi_)->cy = (_cy_),                     \
   (_pbi_)->cPlanes = 1,                     \
   (_pbi_)->cBitCount = 24

// maximum color value
#define SHADECLRMAX       ((0xff) << 22)
// questa macro controlla che il valore shiftato del colore sia nei limiti
#define _shadeChkClrLimit(_l_) \
   if ((_l_) < 0)  (_l_) = 0;  else if ((_l_) > SHADECLRMAX) \
      (_l_) = SHADECLRMAX

// delta singoli colori
#define _shadeDeltaR(l1, l2, c) (((L2RED(l1) - L2RED(l2)) << 22) / (c))
#define _shadeDeltaG(l1, l2, c) (((L2GRN(l1) - L2GRN(l2)) << 22) / (c))
#define _shadeDeltaB(l1, l2, c) (((L2BLU(l1) - L2BLU(l2)) << 22) / (c))

// init RGB deltas
#define _shadeInitDeltas(_dr_, _dg_, _db_, _l1_, _l2_, _c_) \
   (_dr_) = _shadeDeltaR((_l1_), (_l2_), (_c_)), \
   (_dg_) = _shadeDeltaG((_l1_), (_l2_), (_c_)), \
   (_db_) = _shadeDeltaB((_l1_), (_l2_), (_c_))

// init RGB start values (shifted left by 22)
#define _shadeInitRGB(_lr_, _lg_, _lb_, _clr_)   \
   (_lr_) = (L2RED(_clr_)) << 22;                \
   (_lg_) = (L2GRN(_clr_)) << 22;                \
   (_lb_) = (L2BLU(_clr_)) << 22

// increase the RGB value in the bitmap
#define _shadeHIncr(_p_, _r_, _g_, _b_, _dr_, _dg_, _db_) \
   *(_p_)++ = (UCHAR)((_b_) >> 22), (_b_) += (_db_); _shadeChkClrLimit((_b_)); \
   *(_p_)++ = (UCHAR)((_g_) >> 22), (_g_) += (_dg_); _shadeChkClrLimit((_g_)); \
   *(_p_)++ = (UCHAR)((_r_) >> 22), (_r_) += (_dr_); _shadeChkClrLimit((_r_))

// increase the RGB value in the bitmap with dithering
#define _shadeHDitherIncr(_p_, _d_, _i_, _r_, _g_, _b_, _dr_, _dg_, _db_) \
   (_d_) = ((_i_) & 1) ? 0x800000: -0x800000;                             \
   *(_p_)++ = (UCHAR)((_b_) >> 22), (_b_) += (_db_) + (_d_);              \
   _shadeChkClrLimit((_b_));                                              \
   *(_p_)++ = (UCHAR)((_g_) >> 22), (_g_) += (_dg_) + (_d_);              \
   _shadeChkClrLimit((_g_));                                              \
   *(_p_)++ = (UCHAR)((_r_) >> 22), (_r_) += (_dr_) + (_d_);              \
   _shadeChkClrLimit((_r_))

#define _shadeVSetPixel(_p_, _r_, _g_, _b_) \
   *(_p_)-- = (UCHAR)((_r_) >> 22),         \
   *(_p_)-- = (UCHAR)((_g_) >> 22),         \
   *(_p_)-- = (UCHAR)((_b_) >> 22)

#define _shadeVIncr(_r_, _g_, _b_, _dr_, _dg_, _db_) \
   (_b_) += (_db_); _shadeChkClrLimit((_b_));        \
   (_g_) += (_dg_); _shadeChkClrLimit((_g_));        \
   (_r_) += (_dr_); _shadeChkClrLimit((_r_))


#endif
