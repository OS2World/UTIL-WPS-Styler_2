//=========================================================================\
//                                                                         |
//                                winutil.h                                |
//                                                                         |
// collezione di macro per facilitare la programmazione OS/2 PM            |
// (c) 1997 Alessandro Cantatore Team OS/2 Italy                           |
//                                                                         |
// indice:                                                                 |
//                                                                         |
// 001.001 strutture                                                       |
//                                                                         |
// 002.001 macro generiche query                                           |
// 002.002 macro generiche set                                             |
// 002.003 macro generiche varie                                           |
//                                                                         |
// 010.001 button query                                                    |
// 010.002 button set                                                      |
// 010.003 button various                                                  |
//                                                                         |
// 015.001 circular sliders query                                          |
// 015.002 circular slider set                                             |
// 015.003 circular slider various                                         |
//                                                                         |
// 020.001 clipboard query                                                 |
// 020.002 clipboard set                                                   |
// 020.003 clipboard various                                               |
//                                                                         |
// 025.001 combobox query                                                  |
// 025.002 combobox set                                                    |
// 025.003 combobox various                                                |
//                                                                         |
// 030.001 container drag & drop                                           |
// 030.002 container query                                                 |
// 030.003 container set                                                   |
// 030.004 container various                                               |
//                                                                         |
// 050.001 entryfield query                                                |
// 050.002 entryfield set                                                  |
// 050.003 entryfield various                                              |
//                                                                         |
// 070.001 listbox delete                                                  |
// 070.002 listbox insert                                                  |
// 070.003 listbox query                                                   |
// 070.004 listbox set                                                     |
// 070.005 listbox various                                                 |
//                                                                         |
// 075.001 menu query                                                      |
// 075.002 menu set                                                        |
// 075.003 menu various                                                    |
//                                                                         |
// 080.001 MLE query                                                       |
// 080.002 MLE set                                                         |
// 080.003 MLE various                                                     |
//                                                                         |
// 085.001 notebook query                                                  |
// 085.002 notebook set                                                    |
// 085.003 notebook various                                                |
//                                                                         |
// 090.001 scrollbar query                                                 |
// 090.002 scrollbar set                                                   |
// 090.003 scrollbar various                                               |
//                                                                         |
// 095.001 slider query                                                    |
// 095.002 slider set                                                      |
// 095.003 slider various                                                  |
//                                                                         |
// 100.001 spinbutton query                                                |
// 100.002 spinbutton set                                                  |
// 100.003 spinbutton various                                              |
//                                                                         |
// 105.001 static query                                                    |
// 105.002 static set                                                      |
// 105.003 static various                                                  |
//                                                                         |
// 110.001 value set query                                                 |
// 110.002 value set set                                                   |
// 110.003 value set various                                               |
//                                                                         |
//=========================================================================/

#ifndef _WINUTIL_H
   #define _WINUTIL_H


//=========================================================================\
// 001.001 strutture                                                       |
//=========================================================================/

#pragma pack(1)
typedef union {
   LONG l;
   struct {
      UCHAR blu, grn, red, x;
   } rgb;
} CLR, * PCLR;
#pragma pack()

// struttura posizione-dimensioni finestra
typedef struct {
   LONG x;
   LONG y;
   LONG cx;
   LONG cy;
} WRECTL, * PWRECTL;

#pragma pack(2)
typedef struct {
   SHORT x;
   SHORT y;
   SHORT cx;
   SHORT cy;
} WRECTS, * PWRECTS;

typedef struct {
   SHORT cx;
   SHORT cy;
} SSIZE, * PSSIZE;
#pragma pack()


//=========================================================================\
// 002.001               macro generiche query                             |
//=========================================================================/

   // riporta la posizione di un controllo in un dialogo
   #define DlgQueryCtlPos(hwnd, ulId, pswp) \
           (WinQueryWindowPos(WinWindowFromID(hwnd, (ulId)), (pswp)))

   // richiesta dati USHORT controllo
   #define DlgQueryCtlUShort(hwnd, ulId, flag) \
           (WinQueryWindowUShort(WinWindowFromID(hwnd, (ulId)), (flag)))

   // riporta presenza flag WS_VISIBLE
   #define WinIsCtlVisible(hwnd, ulId) \
           (WinIsWindowVisible(WinWindowFromID(hwnd, (ulId))))


//=========================================================================\
// 002.002                 macro generiche set                             |
//=========================================================================/

   // riposiziona un controllo
   #define DlgSetCtlPos(hwnd, ulId, f1, x, y, cx, cy, f2) \
           (WinSetWindowPos(WinWindowFromID(hwnd, (ulId)), (f1), (x), (y), \
                                            (cx), (cy), (f2)))

   // settaggio stili controllo
   #define DlgSetCtlBits(hwnd, ulId, f1, f2, f3) \
           (WinSetWindowBits(WinWindowFromID(hwnd, (ulId)), (f1), (f2), (f3)))

#define WsetWinStyle(hwnd, style) \
   (WinSetWindowBits((hwnd), QWL_STYLE, (style), (style)))
#define WresetWinStyle(hwnd, style) \
   (WinSetWindowBits((hwnd), QWL_STYLE, 0L, (style)))


#define MRTRUE         ((MRESULT)1)
#define MRFALSE        ((MRESULT)0)

//=========================================================================\
// 002.003                 macro generiche varie                           |
//=========================================================================/

   // definisce un PHELPTABLE da un HELPTABLE id
   #define PHLPTBLFROMID(x)   ((PHELPTABLE)((x) | 0xffff0000))

   // mostra-nasconde un controllo di un dialogo
   #define DlgShowCtl(hwnd, ulId, flag) \
           (WinShowWindow(WinWindowFromID(hwnd, (ulId)), (flag)))
   // per scrivere messaggi errore
   #define PMPrint(hwnd, psz)  WinMessageBox(HWND_DESKTOP, hwnd,\
                               (psz), NULL, 0, MB_OK | MB_ERROR | MB_MOVEABLE)
   #define MOUSEX(mp)          ((SHORT)(ULONG)(mp))
   #define MOUSEY(mp)          ((SHORT)((ULONG)mp >> 16))
   #define forever             for(;;)
   typedef PSZ * PPSZ;

   #define LIMIT(val, min, max) \
                ((val) < (min)? (min): ((val) > (max)? (max): (val)))
#define RNDUP(n, m)   (((ULONG)(n) + (m) - 1) & ~((m) - 1))
#define RNDDWN(n, m)  ((ULONG)(n) & ~((m) - 1))

// mappa da punti di dialogo a pixel cx e cy sono le dimensioni del
// carattere di sistema di default pari a 4 x 8 dialog points
#define MKDLGPTX(cpts, cx)  (((cpts) * (cx)) / 4)
#define MKDLGPTY(cpts, cy)  (((cpts) * (cy)) / 8)

// aumenta le dimensioni di un rettangolo di 2 * dx e 2 * dy
#define WinflateRect(prect, dx, dy)                               \
                           (((PRECTL)(prect))->xLeft -= (dx),     \
                            ((PRECTL)(prect))->yBottom -= (dy),   \
                            ((PRECTL)(prect))->xRight += (dx),    \
                            ((PRECTL)(prect))->yTop += (dy))
#define WsetRect(prect, x0, y0, x1, y1)                           \
                           (((PRECTL)(prect))->xLeft = (x0),      \
                            ((PRECTL)(prect))->yBottom = (y0),    \
                            ((PRECTL)(prect))->xRight = (x1),     \
                            ((PRECTL)(prect))->yTop = (y1))
// nuovi #define
#define WRectSet(prect, x0, y0, x1, y1)                           \
                           (((PRECTL)(prect))->xLeft = (x0),      \
                            ((PRECTL)(prect))->yBottom = (y0),    \
                            ((PRECTL)(prect))->xRight = (x1),     \
                            ((PRECTL)(prect))->yTop = (y1))

#define WRectInflate(prect, dx, dy)                               \
                           (((PRECTL)(prect))->xLeft -= (dx),     \
                            ((PRECTL)(prect))->yBottom -= (dy),   \
                            ((PRECTL)(prect))->xRight += (dx),    \
                            ((PRECTL)(prect))->yTop += (dy))

// copia prs in prd aumentando le dimensioni di dx, dy
#define WRectCpyInflate(prs, prd, dx, dy)                        \
  (((PRECTL)(prd))->xLeft = ((PRECTL)(prs))->xLeft - (dx),       \
   ((PRECTL)(prd))->yBottom = ((PRECTL)(prs))->yBottom - (dy),   \
   ((PRECTL)(prd))->xRight = ((PRECTL)(prs))->xRight + (dx),     \
   ((PRECTL)(prd))->yTop = ((PRECTL)(prs))->yTop + (dy))
// inizializza dati rettangolo da sizel incrementando dimensioni di dx, dy
#define WSz2RectInflate(ps, prd, dx, dy)                         \
  (((PRECTL)(prd))->xLeft = - (dx),                              \
   ((PRECTL)(prd))->yBottom = - (dy),                            \
   ((PRECTL)(prd))->xRight = ((PSIZEL)(ps))->cx + (dx),          \
   ((PRECTL)(prd))->yTop = ((PSIZEL)(ps))->cy + (dy))


#define WRectDeltaSizeMove(prect, dx, dy, dcx, dcy)              \
                           (((PRECTL)(prect))->xLeft += (dx),    \
                            ((PRECTL)(prect))->yBottom += (dy),  \
                            ((PRECTL)(prect))->xRight += (dcx),  \
                            ((PRECTL)(prect))->yTop += (dcy))

// copia prs in prd cambiando le coordinate x,y di dx dy e le dimensioni
// del rettangolo di dcx, dcy
#define WRectCpyDeltaSizeMove(prs, prd, dx, dy, dcx, dcy)              \
   (((PRECTL)(prd))->xLeft = ((PRECTL)(prs))->xLeft + (dx),            \
    ((PRECTL)(prd))->yBottom = ((PRECTL)(prs))->yBottom + (dy),        \
    ((PRECTL)(prd))->xRight = ((PRECTL)(prs))->xRight + (dcx) + (dx),  \
    ((PRECTL)(prd))->yTop = ((PRECTL)(prs))->yTop + (dcy) + (dy))

// inizializza dati rettangolo da sizel cambiando le coordinate x,y di
// dx dy e le dimensioni del rettangolo di dcx, dcy
#define WSz2RectDeltaSizeMove(ps, prd, dx, dy, dcx, dcy)              \
   (((PRECTL)(prd))->xLeft = (dx),                                    \
    ((PRECTL)(prd))->yBottom = (dy),                                  \
    ((PRECTL)(prd))->xRight = ((PSIZEL)(ps))->cx + (dcx) + (dx),      \
    ((PRECTL)(prd))->yTop = ((PSIZEL)(ps))->cy + (dcy) + (dy))


// shifta le coordinate di un rettangolo di dx, dy
#define WRectShift(prect, dx, dy)                    \
                           (((PRECTL)(prect))->xLeft += (dx),     \
                            ((PRECTL)(prect))->yBottom += (dy),   \
                            ((PRECTL)(prect))->xRight += (dx),   \
                            ((PRECTL)(prect))->yTop += (dy))
                            
// copia prs in prd shiftando le coordinate di prs di dx, dy
#define WRectCpyShift(prs, prd, dx, dy)                          \
  (((PRECTL)(prd))->xLeft = ((PRECTL)(prs))->xLeft + (dx),       \
   ((PRECTL)(prd))->yBottom = ((PRECTL)(prs))->yBottom + (dy),   \
   ((PRECTL)(prd))->xRight = ((PRECTL)(prs))->xRight + (dx),     \
   ((PRECTL)(prd))->yTop = ((PRECTL)(prs))->yTop + (dy))

#define WPointSet(ppt, px, py)  \
  (((PPOINTL)(ppt))->x = (px),  \
   ((PPOINTL)(ppt))->y = (py))

#define WsetFont(hwnd, font)                                      \
   (WinSetPresParam((hwnd), PP_FONTNAMESIZE, strlen(font) + 1, font))

#define WqryParent(hwnd)     WinQueryWindow((hwnd), QW_PARENT)


//==========================================================================\
// estensioni a WinQuerySysValue/WinSetSysValue                             |
//==========================================================================/

#define SV_FULLWINDOWDRAG          99      /* FullWindowDrag            */ 
#define SV_ALTTABSWITCHWIN        100      /* Alt-Tab Switching windows */ 
#define SV_VIOMOUSEACTIONS        101      /* VIO mouse actions         */ 
#define SV_ASYNCFOCUSCHANGE       102      /* Asynchronus focus change  */ 
#define SV_FOCUSCHANGESENS        103      /* Focus Change sensivity    */ 


//==============================[LISTBOX.H]=================================\
// Macro per interagire con le listbox definite come parte di una dialog    |
// window                                                                   |
//==========================================================================/

// 070.001 ----------------- listbox delete --------------------------------

// DlgLboxDeleteAll(HWND hwndDlg, ULONG LboxID)
// cancella tutti gli item nella list box "LboxID"
// probabilmente restituisce 0 in caso di successo

#define DlgLboxDeleteAll(hwndDlg, LboxID)\
        ((LONG)WinSendDlgItemMsg(hwndDlg, (LboxID), LM_DELETEALL, NULL, NULL))

#define wLboxDelAll(hwnd) \
        ((LONG)WinSendMsg((hwnd), LM_DELETEALL, MPVOID, MPVOID))

// DlgLboxDeleteItem(HWND hwndDlg, ULONG LboxID, LONG index)
// cancella l'item "index" dalla listbox "LboxID" appartenente al dialog
// "hwndDlg"
// restituisce il numero di item restanti

#define DlgLboxDeleteItem(hwndDlg, LboxID, index)\
        ((SHORT)WinSendDlgItemMsg(hwndDlg, (LboxID), LM_DELETEITEM,\
                                 MPFROMLONG(index), (MPARAM)NULL))

#define wLboxDelItem(hwnd, idx)\
((SHORT)WinSendMsg((hwnd), LM_DELETEITEM, (MPARAM)idx, MPVOID))

// cancella l'item selezionato

#define DlgLboxDeleteSelectedItem(hwnd, ulId)\
((LONG)WinSendDlgItemMsg(hwnd, (ulId), LM_DELETEITEM,\
                 MPFROMLONG((LONG)WinSendDlgItemMsg(hwnd, (ulId),\
                                               LM_QUERYSELECTION,\ 
                                               MPFROMLONG(LIT_FIRST), NULL)),\
                (MPARAM)NULL))

#define wLboxDelSelItem(hwnd)                                           \
((SHORT)WinSendMsg((hwnd), LM_DELETEITEM,                               \
                   (MPARAM)WinSendMsg((hwnd), LM_QUERYSELECTION,        \
                                   (MPARAM)LIT_FIRST, MPVOID), MPVOID))           

// 070.002 ----------------- listbox insert --------------------------------

// DlgLboxInsertItem(HWND hwndDlg, ULONG LboxID, LONG index, PSZ pszItem)
// inserisce nella listbox LboxID la stringa pszItem alla posizione index
// restituisce l'indice della stringa inserita

#define DlgLboxInsertItem(hwndDlg, LboxID, index, pszItem)\
        ((SHORT)WinSendDlgItemMsg(hwndDlg, (LboxID), LM_INSERTITEM,\ 
                                 MPFROMLONG(index), MPFROMP(pszItem)))

#define wLboxInsItem(hwnd, idx, pszItem) \
((SHORT)WinSendMsg((hwnd), LM_INSERTITEM, (MPARAM)(idx), (MPARAM)(pszItem)))

// DlgLboxInsertItemAndHandle(HWND hDlg, ULONG LboxID, LONG index, PSZ pszItem,
//                         ULONG ulHnd);
// inserisce un item nella listbox settandone contemporaneamente l'handle

#define DlgLboxInsertItemAndHandle(hDlg, LboxID, index, pszItem, ulHnd)\
((BOOL)WinSendDlgItemMsg(hDlg, (LboxID), LM_SETITEMHANDLE,\
                             ((MPARAM)WinSendDlgItemMsg(hDlg, (LboxID),\
                                                        LM_INSERTITEM,\ 
                                                        MPFROMLONG(index),\
                                                        MPFROMP (pszItem))),\
                                                        (MPARAM)ulHnd))

#define wLboxInsItemHnd(hwnd, idx, pszItem, ulHnd)\
((BOOL)WinSendMsg((hwnd), LM_SETITEMHANDLE,\
                  ((MPARAM)WinSendMsg((hwnd), LM_INSERTITEM, (MPARAM)(idx),\ 
                                      (MPARAM)(pszItem))),\
                  (MPARAM)ulHnd))

// inserisce l'item dopo l'item su cui Š correntemente il cursore
// restituendo la posizione di inserimento
#define DlgLboxInsertItemAtCurrPos(hwnd, ulId, pszItem)\
((LONG)WinSendDlgItemMsg(hwnd, (ulId), LM_INSERTITEM,\
                MPFROMLONG((LONG)WinSendDlgItemMsg(hwnd, (ulId),\
                                           LM_QUERYSELECTION,\ 
                                           MPFROMLONG(LIT_CURSOR), NULL) + 1),\
                MPFROMP(pszItem)))

// inserisce item multipli
#define DlgLboxInsertMultiItems(hwnd, id, plbi, ppsz) \
((LONG)WinSendDlgItemMsg(hwnd, (id), LM_INSERTMULTITEMS, \
                         (MPARAM)(plbi), (MPARAM)(ppsz)))


// 070.003 ----------------- listbox query ---------------------------------

// restituisce l'indice dell'item selezionato successivo al primo
//DlgLboxQueryNextSelItem(HWND hwndDlg,      handle della dialog window
//                        ULONG LboxID,      ID della listbox
//                        USHORT prevItem)   indice dell'Item preced selezion

#define DlgLboxQueryNextSelItem(hwndDlg, LboxID, prevItem)\
((SHORT)WinSendDlgItemMsg(hwndDlg, (LboxID), LM_QUERYSELECTION,\ 
                                  MPFROMLONG((LONG)(prevItem)), NULL))

#define wLboxQuerySelItemNext(hwnd, prevItem)\
((SHORT)WinSendMsg((hwnd), LM_QUERYSELECTION, (MPARAM)(prevItem), MPVOID))

// restituisce il testo corrispondente all'indice i dell'item selezionato
#define DlgLboxQueryNextSelItemText(hwndDlg, LboxID, prevItem, pszBuff, cchMax)\
((LONG)WinSendDlgItemMsg(hwndDlg, (LboxID),\
                         LM_QUERYITEMTEXT,\
                         MPFROM2SHORT((WinSendDlgItemMsg(hwndDlg, (LboxID),\
                                                   LM_QUERYSELECTION,\
                                                   MPFROMLONG((LONG)(prevItem)),\
                                                   NULL)),\
                                      (cchMax)),\
                         MPFROMP(pszBuff)))

// resituisce la quantit… di item presenti nella listbox

#define DlgLboxQueryItemCount(hDlg, ID)\
((SHORT)WinSendDlgItemMsg(hDlg, (ID),  LM_QUERYITEMCOUNT, MPVOID, MPVOID))

#define wLboxQueryItemCount(hwnd) \
((SHORT)WinSendMsg(hwnd, LM_QUERYITEMCOUNT, MPVOID, MPVOID))


// DlgLboxQueryItemHandle(HWND hDlg, ULONG LboxID, SHORT index
#define DlgLboxQueryItemHandle(hDlg, LboxID, index)\
((ULONG)WinSendDlgItemMsg(hDlg, (LboxID),\
                          LM_QUERYITEMHANDLE,\
                          MPFROMSHORT(index), MPFROMLONG(0L)))

#define wLboxQueryItemHnd(hwnd, idx)\
((ULONG)WinSendMsg((hwnd), LM_QUERYITEMHANDLE, (MPARAM)(idx), MPVOID))

// richiede il testo presente ad un dato indice, restituisce la lunghezza del
// testo relativo all'indice
// DlgQueryLboxItemText(HWND hwndDlg,  handle della dialog window
//                      ULONG LboxID,  ID della listbox
//                      SHORT index,   indice dell'item richiesto
//                      PSZ pszBuff,   buffer che riceve il risultato
//                      SHORT cchMax)  dimensione del buffer

#define DlgLboxQueryItemText(hwndDlg, LboxID, index, pszBuff, cchMax)\  
        ((SHORT)WinSendDlgItemMsg(hwndDlg, (LboxID),\
                                 LM_QUERYITEMTEXT,\
                                 MPFROM2SHORT((index), (cchMax)),\
                                 MPFROMP(pszBuff)))

#define wLboxQueryItemText(hwnd, idx, psz, cb)\  
        ((SHORT)WinSendMsg(hwnd, LM_QUERYITEMTEXT,\
                          MPFROM2SHORT((idx), (cb)), (MPARAM)(psz)))

// restituisce la lunghezza del testo relativo all'item presente in index
#define DlgLboxQueryItemTextLength(hwndDlg, LboxID, index)\
        ((SHORT)WinSendDlgItemMsg(hwndDlg, (LboxID),\
                                 LM_QUERYITEMTEXTLENGTH,\
                                 MPFROMLONG(index), NULL))
// restituisce la lunghezza del testo relativo all'item presente in index
#define WLboxQueryItemTextLength(hwnd, index)\
        ((SHORT)WinSendMsg(hwnd, LM_QUERYITEMTEXTLENGTH,\
                           (MPARAM)index, MPVOID))

// restituisce l'indice del primo item selezionato
//DlgQueryLboxSelectedItem(HWND hwndDlg,      handle della dialog window
//                         ULONG LboxID)      ID della listbox

#define DlgLboxQuerySelectedItem(hwndDlg, LboxID)\
        ((SHORT)WinSendDlgItemMsg(hwndDlg, (LboxID), LM_QUERYSELECTION,\ 
                                  MPFROMLONG(LIT_FIRST), NULL))

#define wLboxQuerySelItem(hwnd)\
((SHORT)WinSendMsg(hwnd, LM_QUERYSELECTION, (MPARAM)LIT_FIRST, MPVOID))

// restituisce il testo dell'item selezionato

#define DlgLboxQuerySelItemTxt(hwndDlg, LboxID, pszBuff, cchMax)\
((LONG)WinSendDlgItemMsg(hwndDlg, (LboxID),\
                         LM_QUERYITEMTEXT,\
                         MPFROM2SHORT((WinSendDlgItemMsg(hwndDlg, (LboxID),\
                                                   LM_QUERYSELECTION,\
                                                   (MPARAM)LIT_FIRST,\
                                                   NULL)),\
                                      (cchMax)),\
                         MPFROMP(pszBuff)))
                         
#define wLboxQuerySelItemTxt(hwnd, psz, cch)\
((SHORT)WinSendMsg((hwnd), LM_QUERYITEMTEXT,\
                   MPFROM2SHORT(WinSendMsg((hwnd),LM_QUERYSELECTION,    \
                                           (MPARAM)LIT_FIRST, MPVOID),  \
                                (cch)), (MPARAM)psz))
                                
// riporta l'handle dell'item selezionato
#define wLboxQuerySelItemHnd(hwnd) \
((ULONG)WinSendMsg((hwnd), LM_QUERYITEMHANDLE,\
                   (MPARAM)WinSendMsg((hwnd), LM_QUERYSELECTION, \
                                      (MPARAM)LIT_FIRST, MPVOID), MPVOID))


// 070.004 ----------------- listbox set  ----------------------------------

#define DlgLboxSetItemHandle(hwnd, ulId, idx, ulHnd)\
((BOOL)WinSendDlgItemMsg((hwnd), (ulId), LM_SETITEMHANDLE,\
                         (MPARAM)(idx), (MPARAM)(ulHnd)))

#define wLboxSetItemHnd(hwnd, idx, ulHnd)\
((BOOL)WinSendMsg((hwnd), LM_SETITEMHANDLE, (MPARAM)(idx), (MPARAM)(ulHnd)))

// imposta l'handle dell'item selezionato
#define dLboxSetSelItemHnd(hwnd, id, ulHnd)\
((BOOL)WinSendDlgItemMsg((hwnd), (id), LM_SETITEMHANDLE,\
                         (MPARAM)(WinSendDlgItemMsg((hwnd), (id), \
                                                LM_QUERYSELECTION,\
                                                (MPARAM)LIT_FIRST, MPVOID)), \
                         (MPARAM)(ulHnd)))

#define wLboxSetSelItemHnd(hwnd, ulHnd)\
((BOOL)WinSendMsg((hwnd), LM_SETITEMHANDLE,\
                  (MPARAM)(WinSendMsg((hwnd), LM_QUERYSELECTION,\
                                      (MPARAM)LIT_FIRST, MPVOID)), \
                         (MPARAM)(ulHnd)))

// imposta l'eltezza di un item di una listbox owner draw
// DlgLboxSetItemHeight(HWND hwnd,            handle dialog
//                      ULONG LboxID,         ID listbox
//                      ULONG cy);            altezza item

#define DlgLboxSetItemHeight(hDlg, LboxID, cy)\
((BOOL)WinSendDlgItemMsg(hDlg, (LboxID), LM_SETITEMHEIGHT,\
                         MPFROMLONG(cy), MPFROMLONG(0L)))

// imposta il testo relativo ad un dato index
// DlgSetLboxItemText(HWND hwndDlg,      handle della dialog window
//                    ULONG LboxID,      ID della listbox
//                    LONG index,        indice dell'item richiesto
//                    PSZ pszItemText)   buffer contente il testo da agg

#define DlgLboxSetItemText(hwndDlg, LboxID, index, pszItemText)\
        ((BOOL)WinSendDlgItemMsg(hwndDlg, (LboxID), LM_SETITEMTEXT,\ 
                                 MPFROMLONG(index), MPFROMP (pszItemText)))
                                 
#define wLboxSetItemText(hwnd, idx, psz)\
((BOOL)WinSendMsg((hwnd), LM_SETITEMTEXT, (MPARAM)(idx), (MPARAM)(psz)))

#define wLboxSetTopIdx(hwnd, idx) \
        ((BOOL)WinSendMsg((hwnd), LM_SETTOPINDEX, (MPARAM)(idx), MPVOID))
                                 

// 070.005 ----------------- listbox various  ------------------------------

// deseleziona un item preventivamente selezionato
// DlgDeselectLboxItem(HWND hwndDlg,      handle della dialog window
//                 ULONG LboxID)          ID della list box
// restituisce TRUE in caso di successo, FALSE in caso di errore

#define DlgLboxDeselect(hwndDlg, LboxID)\
        ((BOOL)WinSendDlgItemMsg(hwndDlg, (LboxID), LM_SELECTITEM,\
                      MPFROMLONG((ULONG)(WinSendDlgItemMsg(hwndDlg, (LboxID),\
                                                 LM_QUERYSELECTION,\
                                                 MPFROMLONG(LIT_FIRST),\
                                                 NULL))),\
                      MPFROMLONG((ULONG)FALSE)))
// deseleziona tutti gli item
#define DlgLboxDeselectAll(hwndDlg, LboxID)\
       ((BOOL)WinSendDlgItemMsg(hwnd, (ID), LM_SELECTITEM,\
                                (MPARAM)LIT_NONE, MPVOID))

#define DlgLboxDeselectItem(hwnd, ID, idx)\
       ((BOOL)WinSendDlgItemMsg(hwnd, (ID), LM_SELECTITEM,\
                                MPFROMLONG(idx), MPFROMLONG(FALSE)))

#define DlgLboxIsItemSelected(hwnd, ID, idx) \
   ((BOOL)((idx) == (SHORT)WinSendDlgItemMsg((hwnd), (ID), LM_QUERYSELECTION,\
                                  MPFROMSHORT((idx) -1), MPFROMLONG(0L))))

// cerca la stringa psz nella listbox ID iniziando dal primo item
// option pu• essere:
// LSS_CASESENSITIVE
// LSS_PREFIX        controlla i primi caratteri di un item
// LSS_SUBSTRING     controlla che un item contenga i caratteri specificati
//                   in psz

#define DlgLboxSearchString(hwnd, ID, psz, option) \
   ((SHORT)WinSendDlgItemMsg(hwnd, (ID), LM_SEARCHSTRING,\
                             MPFROM2SHORT((option), LIT_FIRST),\
                             (MPARAM)(psz)))

#define wLboxSearchStr(hwnd, option, psz) \
 ((SHORT)WinSendMsg(hwnd, LM_SEARCHSTRING,\
                    MPFROM2SHORT((option), LIT_FIRST), (MPARAM)(psz)))

// cerca la stringa psz nella listbox ID iniziando dall'item "start"
// option pu• essere:
// LSS_CASESENSITIVE
// LSS_PREFIX        controlla i primi caratteri di un item
// LSS_SUBSTRING     controlla che un item contenga i caratteri specificati
//                   in psz

#define DlgLboxSearchStringFrom(hwnd, ID, start, psz, option) \
   ((SHORT)WinSendDlgItemMsg(hwnd, (ID), LM_SEARCHSTRING,\
                             MPFROM2SHORT((option), (start)),\
                             (MPARAM)(psz)))

#define wLboxSearchStrFrom(hwnd, option, start, psz) \
   ((SHORT)WinSendMsg(hwnd, LM_SEARCHSTRING,\
                      MPFROM2SHORT((option), (start)), (MPARAM)(psz)))


// seleziona un item
#define DlgLboxSelectItem(hDlg, LboxID, item)\
       ((BOOL)WinSendDlgItemMsg(hDlg, (LboxID), LM_SELECTITEM,\
                                MPFROMLONG(item), MPFROMLONG(TRUE)))

#define wLboxSelItem(hwnd, item)\
((BOOL)WinSendMsg((hwnd), LM_SELECTITEM, (MPARAM)(item), (MPARAM)(TRUE)))


//=========================================================================\
//                                  MLE                                    |
//=========================================================================/

//--------inizializzazione del buffer di import/export di testo------------|
//                                                                         |
// (BOOL)DlgMleInit(HWND hwnd,     Handle della dialog window              |
//                  ULONG ulId,      ID del controllo definito nel file RC |
//                  CHAR *szBuf,      Buffer di I/E testo                  |
//                  USHORT cbBuf);    dimensione del buffer                |

#define DlgMleInit(hwnd, ulId, szBuf, cbBuf)\
        (WinSendDlgItemMsg(hwnd, (ulId), MLM_SETIMPORTEXPORT,\
                          MPFROMP(szBuf), MPFROMSHORT((USHORT)(cbBuf))))

//-------------------------- importazione di testo ----------------------- |
//                                                                         |
// (ULONG)DlgMleSetTxt(HWND hwnd,   Handle della dialog window             |
//                     ULONG ulId,    ID del controllo definito nel file RC|
//                     LONG *lOffset,  offset del punto di inserzione      |
//                     ULONG *cbWrote);quantit… byte da scrivere           |
// valore restituito numero di byte importati nella MLE                    |

#define DlgMleSetTxt(hwnd, ulId, plOffset, pcbWrote)\
((ULONG)WinSendDlgItemMsg(hwnd, (ulId), MLM_IMPORT,\
                          MPFROMP(plOffset), MPFROMP(pcbWrote)))


#define DlgMleSetTextLimit(hwnd, ulId, lLimit)\
((ULONG)WinSendDlgItemMsg(hwnd, (ulId), MLM_SETTEXTLIMIT,\
                          MPFROMLONG(lLimit), MPFROMLONG(0L)))


//---------------------------esportazione di testo-------------------------|
// (ULONG)DlgMleGetTxt(HWND hwnd,   Handle della dialog window             |
//                     ULONG ulId,   ID del controllo definito nel file RC |
//                     LONG *lOffset,  offset del punto di inserzione      |
//                     ULONG *cbWrote);quantit… byte da scrivere           |

#define DlgMleGetTxt(hwnd, ulId, plOffset, pcbWrote)\
 (ULONG)WinSendDlgItemMsg(hwnd, (ulId), MLM_EXPORT,\
                          MPFROMP(plOffset), MPFROMP(pcbWrote))

//-----------------------esportazione di tutto il testo--------------------|
//                                                                         |
// (ULONG)DlgMleGetAllTxt(HWND hwnd, Handle della dialog window            |
//                        ULONG ulId, ID del controllo definito nel file RC|
//                        LONG *lOffset,  offset del punto di inserzione   |
//                        ULONG *cbWrote);quantit… byte da scrivere        |
// valore restituito numero di byte importati dalla MLE                    |

#define DlgMleGetAllTxt(hwnd, ulId, plOffset, pcbWrote)\
 *pcbWrote = WinSendDlgItemMsg(hwnd, (ulId), MLM_QUERYFORMATTEXTLENGTH,\
                          MPFROMLONG(0L), MPFROMLONG(-1L));\
 (ULONG)WinSendDlgItemMsg(hwnd, (ulId), MLM_EXPORT,\
                          MPFROMP(plOffset), MPFROMP(pcbWrote))

//-------------------------cancellazione di testo-----------------------------|
//                                                                            |
// (ULONG)DlgMleDelete(HWND hwnd,   Handle della dialog window             |
//                     ULONG ulId,    ID del controllo definito nel file RC  |
//                     IPT iptBegin,   punto di partenza del testo da cancell |
//                     ULONG ulDel)    numero di byte da cancellare           |
// valore restituito numero di byte cancellati                                |

#define DlgMleDelete(hwnd, ulId, iptBegin, ulDel)\
 (ULONG)WinSendDlgItemMsg(hwnd, (ulId), MLM_DELETE,\
                          MPFROMLONG(iptBegin), MPFROMLONG(ulDel))


//-------------------------cancella tutto il testo----------------------------|
//                                                                            |
// (ULONG)DlgMleDeleteAll(HWND hwnd,   Handle della dialog window          |
//                    ULONG ulId,    ID del controllo definito nel file RC   |
//                                                                            |

#define DlgMleDeleteAll(hwnd, ulId)\
 (ULONG)WinSendDlgItemMsg(hwnd, (ulId), MLM_DELETE,\
                          MPFROMLONG(0L),\
                          MPFROMLONG(WinSendDlgItemMsg(hwnd, (ulId),\
                                                   MLM_QUERYFORMATTEXTLENGTH,\
                                                   MPFROMLONG(0L),\
                                                   MPFROMLONG(-1L))))


//-------------------------chiede se il testo Š cambiato----------------------|
//                                                                            |
// (BOOL)DlgMleQueryChanged(HWND hwnd,   Handle della dialog window        |
//                      ULONG ulId)    ID del controllo definito nel file RC |
// valore restituito: TRUE se il testo Š cambiato, altrimenti FALSE           |

#define DlgMleQueryChanged(hwnd, ulId)\
 ((BOOL)WinSendDlgItemMsg(hwnd, (ulId), MLM_QUERYCHANGED,\
                          MPFROMLONG(0L), MPFROMLONG(0L)))
                             
//-------------------------resetta la flag "changed"--------------------------|
// (BOOL)DlgMleResetChanged(HWND hwnd,   Handle della dialog window        |
//                      ULONG ulId)    ID del controllo definito nel file RC |

#define DlgMleResetChanged(hwnd, ulId)\
 ((BOOL)WinSendDlgItemMsg(hwnd, (ulId), MLM_SETCHANGED,\
                          MPFROMLONG((ULONG)FALSE), MPFROMLONG(0L)))


//-------------chiede la lunghezza del testo da un punto di partenza----------|
// (per SBCS)                                                                 |
// (ULONG)DlgMleQueryTxtLenFrom(HWND hwnd,   Handle della dialog window    |
//                          ULONG ulId, ID del controllo definito nel file RC|
//                          IPT iptStart)   punto di partenza                 |
// restituisce la lunghezza del testo da iptStart alla fine                   |

#define DlgMleQueryTxtLenFrom(hwnd, ulId, iptStart)\
((ULONG)WinSendDlgItemMsg(hwnd, (ulId), MLM_QUERYFORMATTEXTLENGTH,\
                          MPFROMLONG(iptStart), MPFROMLONG(-1L)))


//------------------chiede la lunghezza di tutto il testo---------------------|
//                                                                            |
// (ULONG)DlgMleQueryTxtLength(HWND hwnd,   Handle della dialog window     |
//                          ULONG ulId) ID del controllo definito nel file RC|
// restituisce la lunghezza di tutto il testo contenuto nella MLE             |

#define DlgMleQueryTxtLength(hwnd, ulId)\
((ULONG)WinSendDlgItemMsg(hwnd, (ulId), MLM_QUERYFORMATTEXTLENGTH,\
                          MPFROMLONG(0L), MPFROMLONG(-1L)))


//------------ abilita disabilita modalit… editazione MLE ------------------\
//                                                                          \
// parametri:                                                               \
// HWND hDlg   = handle della finestra di dialogo                           \
// ULONG ulId = id MLE                                                     \
// BOOL flag   = TRUE (readonly), FALSE (editazione)                        \
// risultato:                                                               \
// BOOL rc     = TRUE (successo), FALSE (errore)                            \

#define DlgMleSetReadOnly(hDlg, ulId, flag)\
((BOOL)WinSendDlgItemMsg(hDlg, (ulId), MLM_SETREADONLY,\
                         MPFROMSHORT((USHORT)(flag)), MPFROMLONG(0L)))


// ------------ riporta offset testo selezionato

#define DlgMleQuerySelection(hDlg, ulId)\
((ULONG)WinSendDlgItemMsg(hDlg, (ulId), MLM_QUERYSEL,\
                          MPFROMSHORT(MLFQS_MINMAXSEL), MPFROMLONG(0L)))

// -----------------------------------------------------
// ------------ operazioni clipboard -------------------
// -----------------------------------------------------

// ----------------- CUT -----------------
#define DlgMleCut(hDlg, ulId)\
((ULONG)WinSendDlgItemMsg(hDlg, (ulId), MLM_CUT, (MPARAM)0L, (MPARAM)0L))

// ----------------- COPY -----------------
#define DlgMleCopy(hDlg, ulId)\
((ULONG)WinSendDlgItemMsg(hDlg, (ulId), MLM_COPY, (MPARAM)0L, (MPARAM)0L))
// ----------------- PASTE -----------------
#define DlgMlePaste(hDlg, ulId)\
((ULONG)WinSendDlgItemMsg(hDlg, (ulId), MLM_PASTE, (MPARAM)0L, (MPARAM)0L))
// ----------------- CLEAR -----------------
#define DlgMleClear(hDlg, ulId)\
((ULONG)WinSendDlgItemMsg(hDlg, (ulId), MLM_CLEAR, (MPARAM)0L, (MPARAM)0L))
// ----------------- UNDO ------------------
#define DlgMleUndo(hDlg, ulId)\
((USHORT)WinSendDlgItemMsg(hDlg, (ulId), MLM_UNDO, (MPARAM)0L, (MPARAM)0L))
#define DlgMleQueryUndo(hDlg, ulId)\
((USHORT)WinSendDlgItemMsg(hDlg, (ulId), MLM_QUERYUNDO, (MPARAM)0L, (MPARAM)0L))


//=========================================================================\
//                              spinbuttons                                |
//=========================================================================/

// 100.001 ----------------------- spinbutton QUERY -----------------------

// DlgSpinQueryIdx(HWND hwnd, ULONG ulId, PULONG pulIdx)
// riporta l'indice del valore dell'array correntemente selezionato
#define DlgSpinQueryIdx(hwnd, ulId, pulIdx)\
((BOOL)WinSendDlgItemMsg(hwnd, (ulId), SPBM_QUERYVALUE,\ 
                         MPFROMP(pulIdx),\
                         MPFROM2SHORT((0), (SPBQ_UPDATEIFVALID))))


// DlgSpinQueryLimits(HWND hwnd, ULONG ID, PULONG pulUpper, PULONG pulLower)
// richiede il limite superiore e inferiore dello spin button, restituisce
// TRUE in caso di successo, FALSE in caso di fallimento
#define DlgSpinQueryLimits(hwnd, ulId, pulUpper, pulLower)\
((BOOL)WinSendDlgItemMsg(hwnd, (ulId), SPBM_QUERYLIMITS,\ 
                         MPFROMP(pulUpper), MPFROMP (pulLower)))


// DlgSpinQueryLong(HWND hwnd, ULONG ulId, PULONG pulValue)
// riporta il valore selezionato come ULONG
#define DlgSpinQueryLong(hwnd, ulId, pulValue)\
((BOOL)WinSendDlgItemMsg(hwnd, (ulId), SPBM_QUERYVALUE,\ 
                         MPFROMP(pulValue),\
                         MPFROM2SHORT((0), (SPBQ_UPDATEIFVALID))))


// DlgSpinQueryLongAU(HWND hwnd, ULONG ulId, PULONG pulValue)
// riporta il valore selezionato come ULONG ALWAYSUPDATE
#define DlgSpinQueryLongAU(hwnd, ulId, pulValue)\
((BOOL)WinSendDlgItemMsg(hwnd, (ulId), SPBM_QUERYVALUE,\ 
                         MPFROMP(pulValue),\
                         MPFROM2SHORT(0, SPBQ_ALWAYSUPDATE)))


// DlgSpinQueryString(HWND hwnd, ULONG ulId, PSZ pszBuf, USHORT cbBuf)
// riporta la stringa corrente nello spin button
#define DlgSpinQueryString(hwnd, ulId, pszBuf, cbBuf)\
((BOOL)WinSendDlgItemMsg(hwnd, (ulId), SPBM_QUERYVALUE,\ 
                         MPFROMP(pszBuf),\
                         MPFROM2SHORT((cbBuf), (SPBQ_UPDATEIFVALID))))


// ----------------------------- SET ---------------------------------------

// DlgSpinSetArray(HWND hwnd, ULONG ulId, PSZ pszArr, USHORT cbItems)

// DlgSpinSetLimits(HWND hwnd, ULONG ulId, LONG lUpper, LONG lLower)
// setta i limiti validi per lo spin button
#define DlgSpinSetLimits(hwnd, ulId, lUpper, lLower)\
((BOOL)WinSendDlgItemMsg(hwnd, (ulId), SPBM_SETLIMITS,\
                        MPFROMLONG(lUpper), MPFROMLONG(lLower)))


// DlgSpinSetTextLimit(HWND hwnd, ULONG ulId, USHORT limit)
// il massimo valore per limit Š 256
#define DlgSpinSetTextLimit(hwnd, ulId, usLimit)\
((BOOL)WinSendDlgItemMsg(hwnd, (ulId), SPBM_SETTEXTLIMIT,\
                         MPFROMSHORT(usLimit), MPFROMLONG(0L)))


// DlgSpinSetArray(HWND hwnd, ULONG ulId, PSZ pszArr, USHORT cbItems)

// DlgSpinSetValue(HWND hwnd, ULONG ulId, LONG lValue)
#define DlgSpinSetValue(hwnd, ulId, lValue)\
((BOOL)WinSendDlgItemMsg(hwnd, (ulId), SPBM_SETCURRENTVALUE,\
                         MPFROMLONG(lValue), MPFROMLONG(0L)))


#endif
