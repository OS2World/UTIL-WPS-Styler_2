// titlenew.h
#ifndef TITLENEW_H_
   #define TITLENEW_H_

// ULONG ulCmdId[5];   Š usato per simulare comandi inviati ciclicamente
// dai tasti swapcolors, horizontal/vertical, left/top e bottom/right
// quando lo stile della barra del titolo Š shade
#pragma pack(1)
 typedef struct {
   HAB hab;
   HWND hwnd;                 // titlebar handle
   HBITMAP hbmp;              // handle bitmap usata per la sfumatura
   BOOL i;                    // 0 finestra attiva, 1 finestra inattiva
   CHAR achTxt[128];          // testo barra titolo
   UCHAR cbTxt;               // lunghezza testo
   UCHAR yfont;               // fm.lMaxDescender
   UCHAR cyfont;              // fm.lMaxBaselineExt
   UCHAR cxfont;              // larghezza intera stringa
   CHAR buf[64];
   CLASSINFO ci;              // dati classe originale
   WRECTL size;               // dimensioni titlebar
   HPS hps;                   // PS handle
   ULONG ulCmdId[3];          // vedi nota precedente
   PTBARAIOPT ptbo;           // indirizzo opzioni titlebar attiva/inattiva
} TITLESAMPLE, * PTITLESAMPLE;
#pragma pack()

MRESULT EXPENTRY ACCTitleWinProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID ACCDrawTitlebar(PTITLESAMPLE pts);
VOID ACCDrawTitlebarShadeBkgnd(PTITLESAMPLE pts, PSIZEL psiz);
VOID ACCDrawTitlebarBmpBkgnd(PTITLESAMPLE pts);
VOID ACCDrawTitlebarSolidBkgnd(PTITLESAMPLE pts);
VOID ACCDrawTitlebarText(PTITLESAMPLE pts, BOOL flag);
VOID ACCDrawTitlebarEdge(PTITLESAMPLE pts);


#endif
