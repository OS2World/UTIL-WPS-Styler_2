// funzioni utilit… e strutture per controllo notebook

#ifndef NOTEBOOK_H_
   #define NOTEBOOK_H_

// flag comunemente usate nei notebook stile warp 4
#define BFA_COMMON         BFA_PAGEDATA | BFA_PAGEFROMDLGRES | \
                           BFA_STATUSLINE | BFA_MINORTABTEXT
#define BKA_COMMON         BKA_AUTOPAGESIZE | BKS_TABTEXTCENTER | \
                           BKA_STATUSTEXTON | BKA_MINOR

#pragma pack(2)
// struttura minimale dati pagina
typedef struct _PGDAT{
   USHORT cb;    // dimensione struttura
   USHORT fs;    // tipo di tab (0 o BKA_MAJOR o BKA_MINOR
   struct _PGDAT * pprv; // indirizzo struttura pagina precedente
   struct _PGDAT * pnxt; // indirizzo struttura pagina successiva
   HWND hwnd;    // handle dialogo pagina
   ULONG pgid;   // id assegnata dal controllo all'inserimento della pagina
   ULONG fl;     // flag definibili dall'utente
   PVOID pv;     // indirizzo dati definibili dall'utente
} PGDAT, * PPGDAT;

// struttura lista pagine a dimensione variabile. ppd viene riallocato ogni
// volta che si inserisce o si nasconde una pagina
typedef struct {
   HWND hwnd;      // handle controllo notebook
   ULONG cpgs;     // totale pagine inserite
   HMODULE hres;   // handle modulo risorse dialoghi pagina notebook
   PPGDAT ppd;     // indirizzo prima struttura dati pagina
} NBK, * PNBK;

// struttura dati inserzione pagine notebook (notebook insertion page)

typedef struct {
   USHORT pgn;   // numero pagina all'interno della tab
   USHORT cpg;   // numero pagine sotto tab corrente
   USHORT idst;  // ID stringa status line
                 // "pagina X di Y" usando i membri isdt, pgn e cpg per
                 // creare il testo opportuno, altrimenti indica inserzione
                 // BKA_LAST BKA_FIRST BKA_NEXT BKA_PREV
   USHORT idmaj; // ID stringa major tab
   USHORT idmin; // ID stringa minor tab
   USHORT fs;    // flag stile pagina 0, BKA_MAJOR, BKA_MINOR
   PFNWP pf;     // indirizzo procedura dialogo pagina
   ULONG iddlg;  // id dialogo da caricare
   ULONG fl;     // flag definite da utente da memorizzare nei dati pagina
   PVOID pv;     // indirizzo dati definibili dall'utente (opzioni pagina)
   PPGDAT* pppg; // eventuale ulteriore indirizzo in cui memorizzare dati pagina
} NBINSPG, * PNBINSPG;
#pragma pack()


#define NBINS_ISFIRST      ((PPGDAT)0xffffffff)    // prima pagina notebook
#define NBINS_PGXOFY       0x8000
// la seguente flag Š usata da WnbkDelPg() per indicare che l'id della pagina
// deve essere interpretato come ordine di inserzione della pagina nel notebook
#define BKA_PAGEORDER      0x10000L
// valore restituito da alcune funzioni in caso di errore
#define NBK_ERROR          0xffffffff
// flag usate da WnbkPgn() per restituire id pagina, posizione inserzione
// pagina o indirizzo struttura dati associati con la pagina
#define NBKQ_PGORD       0x0000 // restituisce posizione inserzione pagina
#define NBKQ_PGID        0x0001 // restituisce id pagina
#define NBKQ_PGDAT       0x0002 // restituisce indirizzo struttura pagina

// prototipi funzioni
BOOL APIENTRY WnbkFill(PNBK pnbk, PNBINSPG pnbi, BOOL fl);
PPGDAT APIENTRY WnbkInitPg(PNBK pnbk, PBOOKPAGEINFO pbpi, PNBINSPG pnbi,
                           PPGDAT pprev, PULONG pcxM, PULONG pcxm);
PPGDAT APIENTRY WnbkInsertPg(PNBK pnbk, PBOOKPAGEINFO pbpi, PNBINSPG pnbi,
                             PPGDAT pprev, ULONG flins);
BOOL APIENTRY WnbkDelPg(PNBK pnbk, ULONG id, ULONG fl);
ULONG APIENTRY WnbkPgn(PNBK pnbk, ULONG id, ULONG fl);
BOOL APIENTRY WnbkMinSize(PWDATA pw, ULONG cxm, ULONG cym, BOOL fl);
PSZ APIENTRY WpageXofY(ULONG idstr, ULONG ulPg, ULONG cPgs, PSZ psz);

#endif
