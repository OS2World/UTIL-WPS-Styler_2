//==========================================================================\
// ctxthlp.c: Contextual controls help functions                            |
// BOOL APIENTRY WsetChlp(HWND hwnd, ULONG idctl, ULONG idstr);             |
// BOOL APIENTRY WsetChlpMulti(HWND hwnd, ULONG idstart,                    |
//                             ULONG idend, ULONG idstr);                   |
// BOOL APIENTRY WsetChlpSpin(HWND hwnd, ULONG idctl, ULONG idstr);         |
// BOOL APIENTRY WsetChlpCombo(HWND hwnd, ULONG idctl, ULONG idstr);        |
// VOID APIENTRY WsetChlpMorebox(HWND hwnd, ULONG idctl,                    |
//                               ULONG idcpt, ULONG idswp, ULONG idfnd);    |
// VOID APIENTRY WsetChlpClbox(HWND hwnd, ULONG idctl, ULONG idcpt,         |
//       ULONG idlst, ULONG identry, ULONG idadd, ULONG idrem, ULONG idfnd);|
// VOID APIENTRY WsetChlpElbox(HWND hwnd, ULONG idctl, ULONG idstr);        |
//==========================================================================/

#pragma strings(readonly)

#include "common.h"
#include "pmappapi.h"
#include "ctxthlp.h"
#include "api.h"


//==========================================================================\
// setta nelle window words del controllo id l'id della stringa di help     |
// contestuale                                                              |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idctl: id controllo                                                |
// ULONG idstr: id stringa                                                  |
// valore restituito:                                                       |
// BOOL:    TRUE  = errore                                                  |
//          FALSE = successo                                                |
//==========================================================================/

BOOL APIENTRY WsetChlp(HWND hwnd, ULONG idctl, ULONG idstr) {
   return wsetulong(WinWindowFromID(hwnd, idctl), idstr);
}


//==========================================================================\
// setta nelle window words dei controlli da istart a iend l'id delle       |
// corrispondenti stringhe di help contestuale                              |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idstart: id primo controllo                                        |
// ULONG idend: id ultimo controllo                                         |
// ULONG idstr: id stringa                                                  |
// valore restituito:                                                       |
// BOOL:    TRUE  = errore                                                  |
//          FALSE = successo                                                |
//==========================================================================/

BOOL APIENTRY WsetChlpMulti(HWND hwnd, ULONG idstart,
                            ULONG idend, ULONG idstr) {
   while (idstart <= idend) {
      if (!wsetulong(WinWindowFromID(hwnd, idstart++), idstr++))
         return FALSE;
   } // end while
   return TRUE;
}


//==========================================================================\
// setta nelle window words dei controlli componenti lo spinbutton          |
// l'id della corrispondente stringa di help contestuale                    |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idctl: id controllo                                                |
// ULONG idstr: id stringa                                                  |
// valore restituito:                                                       |
// BOOL:    TRUE  = errore                                                  |
//          FALSE = successo                                                |
//==========================================================================/

BOOL APIENTRY WsetChlpSpin(HWND hwnd, ULONG idctl, ULONG idstr) {
   HWND hctl = WinWindowFromID(hwnd, idctl);
   return (wsetulong(WinWindowFromID(hctl, idctl), idstr) &&
           wsetulong(WinWindowFromID(hctl, 0), idstr));
}


//==========================================================================\
// setta nelle window words dei controlli componenti una combobox           |
// l'id della corrispondente stringa di help contestuale                    |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idctl: id controllo                                                |
// ULONG idstr: id stringa                                                  |
// valore restituito:                                                       |
// BOOL:    TRUE  = errore                                                  |
//          FALSE = successo                                                |
//==========================================================================/

BOOL APIENTRY WsetChlpCombo(HWND hwnd, ULONG idctl, ULONG idstr) {
   HWND hctl = WinWindowFromID(hwnd, idctl);
   return (wsetulong(WinWindowFromID(hctl, CBID_EDIT), idstr) &&
           wsetulong(hctl, idstr));
}


//==========================================================================\
// setta nelle window words dei controlli componenti una morebox            |
// l'id della corrispondente stringa di help contestuale                    |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idctl: id controllo                                                |
// ULONG idcpt: id help caption o check box facente parte del controllo     |
// ULONG idswp: id help bottone swap                                        |
// ULONG idfnd: id help bottone find                                        |
// valore restituito:                                                       |
// BOOL:    TRUE  = errore                                                  |
//          FALSE = successo                                                |
//==========================================================================/

VOID APIENTRY WsetChlpMorebox(HWND hwnd, ULONG idctl,
                             ULONG idcpt, ULONG idswp, ULONG idfnd) {
   HWND hctl = WinWindowFromID(hwnd, idctl);
   HWND hcpt = WinWindowFromID(hctl, FID_CAPTION);
   HWND hswap = WinWindowFromID(hctl, FID_SWAP);
   HWND hfnd = WinWindowFromID(hctl, FID_FIND);
   if (idcpt) wsetulong((hcpt? hcpt: hctl), idcpt);
   if (hswap && idswp) wsetulong(hswap, idswp);
   if (idfnd) wsetulong(hfnd, idfnd);
}


//==========================================================================\
// setta nelle window words dei controlli componenti una compound listbox   |
// l'id della corrispondente stringa di help contestuale                    |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idctl: id controllo                                                |
// ULONG idcpt: id help caption o check box facente parte del controllo     |
// ULONG idlst: id help listbox                                             |
// ULONG identry: id help entryfield                                        |
// ULONG idadd: id help bottone add                                         |
// ULONG idrem: id help bottone remove                                      |
// ULONG idfnd: id help bottone find                                        |
// valore restituito:                                                       |
// BOOL:    TRUE  = errore                                                  |
//          FALSE = successo                                                |
//==========================================================================/

VOID APIENTRY WsetChlpClbox(HWND hwnd, ULONG idctl, ULONG idcpt, ULONG idlst,
                     ULONG identry, ULONG idadd, ULONG idrem, ULONG idfnd) {
   HWND hctl = WinWindowFromID(hwnd, idctl);
   HWND hcpt = WinWindowFromID(hctl, FID_CAPTION);
   HWND hlst = WinWindowFromID(hctl, FID_LIST);
   HWND hentry = WinWindowFromID(hctl, FID_ENTRY);
   HWND hadd = WinWindowFromID(hctl, FID_ADD);
   HWND hrem = WinWindowFromID(hctl, FID_REMOVE); 
   HWND hfnd = WinWindowFromID(hctl, FID_FIND);
   if (idcpt) wsetulong((hcpt? hcpt: hctl), idcpt);
   if (idlst) wsetulong(hlst, idlst);
   if (identry) {         // se  una combobox richiama apposita funzione per
      CHAR buf[8];        // settare l'help contestuale
      WinQueryClassName(hentry, 8, buf);
      wsetulong(hentry, identry);
      if (!memcmp(buf, "#2\x0", 3))
         wsetulong(WinWindowFromID(hentry, CBID_EDIT), identry);
   } /* endif */
   if (idadd) wsetulong(hadd, idadd); 
   if (idrem) wsetulong(hrem, idrem); 
   if (idfnd) wsetulong(hfnd, idfnd); 
}


//==========================================================================\
// setta nelle window words dei controlli componenti una enhanced listbox   |
// l'id della corrispondente stringa di help contestuale                    |
// parametri:                                                               |
// HWND hwnd: handle dialogo                                                |
// ULONG idctl: id controllo                                                |
// ULONG idstr: id stringa                                                  |
// valore restituito:                                                       |
// BOOL:    TRUE  = errore                                                  |
//          FALSE = successo                                                |
//==========================================================================/

VOID APIENTRY WsetChlpElbox(HWND hwnd, ULONG idctl, ULONG idstr) {
   HWND hctl = WinWindowFromID(hwnd, idctl);
   HWND hlst = WinWindowFromID(hctl, FID_LIST);
   if (hctl) wsetulong(hctl, idstr);
   if (hlst) wsetulong(hlst, idstr);
}
