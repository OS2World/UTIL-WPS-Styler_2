//==========================================================================\
// logutil.h : funzioni di utilit… per il logging                           |
//==========================================================================/

#ifndef LOGUTIL_H_
   #define LOGUTIL_H_

#define CBLOGBUF 0x1000
#define CBMINBUF 0x4000

typedef struct {
   ULONG cbbuf;         // dimensione buffer
   HWND hwnd;           // finestra in cui eventualmente scrivere messaggi
   PSZ pcur;            // puntatore scrittura nel buffer
   CHAR fname[260];     // nome del file in cui scrivere il log
   ULONG ulstart;       // ms da boot time in cui inizia installazione
   CHAR ach[CBLOGBUF];  // buffer temporaneo di 4 KB
   CHAR buf[4];         // inizio buffer scrittura file log
} LOGFILE, * PLOGFILE;

PLOGFILE APIENTRY logInit(HWND hwnd, PSZ pfname, ULONG cbbuf, PSZ pdescr);
BOOL APIENTRY logEnd(PLOGFILE plf, PSZ pendstr, PSZ psztime);
BOOL APIENTRY logAdd(PLOGFILE plf, ULONG fl, INT carg, ...);
PSZ APIENTRY logCat(PSZ pbuf, PSZ pstr, ULONG cb);

#endif
