#ifndef STLR_ERROR_H_
   #define STLR_ERROR_H_

// Smart Windows error messages
enum {IDERR_NULL,
      // errori in stlrCheckReg()
      IDERR_CHKREGALLOC,    // errore DosAllocMem()
      IDERR_CHKREGREAD,     // errore ReadEA()
      IDERR_CHKREGREAD1,    // secondo errore ReadEA()
      IDERR_CHKREGWRT1,     // errore PrfWriteProfileData()
      IDERR_CHKREGWRT2,     // errore WriteEA()
      IDERR_CHKREGCLOCK,    // l'ora di sistema Š stata modificata
      IDERR_CHKREGEXPIRED,  // errore scadenza periodo prova programma
      IDERR_CHKREGEAPATCH,  // EA registrazione non validi
      IDERR_INIREAD,        // errore lettura smartwin.ini
      IDERR_MENUCLASS,      // errore rilevazione dati classe menu
      IDERR_BMPMINMAX,      // errore rilevazione handle default minmax e sysmenu
      IDERR_FRAMECLASS,     // errore rilevazione dati classe frame
      IDERR_NEWFRAME,       // errore registrazione nuova classe frame
      IDERR_TITLEBARCLASS,  // errore rilevazione dati classe titlebar
      IDERR_NEWTITLEBAR,    // errore registrazione nuova classe titlebar
      IDERR_ENTRYFCLASS,    // errore rilevazione dati classe entryfield
      IDERR_NEWENTRYF,      // errore registrazione nuova classe entryfield
      IDERR_BTNCLASS,       // errore rilevazione dati classe bottone
      IDERR_NEWBTN,         // errore registrazione nuova classe bottone
      IDERR_NEWMENU,        // errore registrazione nuova classe menu
      IDERR_BUTTONCLASS,
      IDERR_NEWBUTTON,
      IDERR_LBOXCLASS,
      IDERR_NEWLBOX,
      IDERR_STATICCLASS,
      IDERR_NEWSTATIC,
      IDERR_MLECLASS,
      IDERR_NEWMLE,
      IDERR_CREATESERVERTHRD, // errore creazione server thread
      IDERR_CREATESERVEVSEM,  // errore creazione semaforo evento server thread
      IDERR_CREATETIMERMGR, // errore creazione thread timer manager
      IDERR_CREATEINPUTHOOK,// errore creazione input hook
      IDERR_CREATESNMSGHOOK,// errore creazione send message hook
      IDERR_INITPROCHEAP,   // errore inizializzazione heap processo
      IDERR_ALLOCPROCDATA,  // OBSOLETO errore allocazione memoria dati processo
//      IDERR_CRETMTXSEM,     // OBSOLETO!!!errore creazione semaforo mutex
      IDERR_INFOBLOCKS,     // errore richiesta infoblock
      IDERR_GETRESMOD,
      IDERR_QUERYPRFSIZE,
      IDERR_ALLOCGETPRF,
      IDERR_QUERYPRF,
      IDERR_ALLOCGETMODNAME,
      IDERR_GETMODNAME,
      IDERR_REGEXITFUNC,    // errore registrazione funzione cleanup
      IDERR_CHKEXCALLOC,    // errore allocazione in CheckExceptions
      IDERR_CHKEXCREAD,     // errore lettura dati eccezioni
      IDERR_REGINVALIDKEY,  // chiave di registrazione non valida
      IDERR_COMBOCLASS,
      IDERR_NEWCOMBO,
      C_IDERRORS};
// "Failed to read smartwin.ini"

// Failed to retrieve WC_MENU class informations
// "Failed to retrieve the bitmap handles of the default window controls"

// Failed to retrieve WC_FRAME class informations.
// Failed to register the new WC_FRAME procedure.
// Failed to retrieve WC_TITLEBAR class informations.
// Failed to register the new WC_TITLEBAR procedure.
// Failed to retrieve WC_ENTRYFIELD class informations.
// Failed to register the new WC_ENTRYFIELD procedure.
// Failed to retrieve WC_MENU class informations.
// Failed to register the new WC_MENU procedure.
// Failed to retrieve WC_BUTTON class informations.
// Failed to register the new WC_BUTTON procedure.
// Failed to retrieve WC_LISTBOX class informations.
// Failed to register the new WC_LISTBOX procedure.
// Failed to retrieve WC_STATIC class informations.
// Failed to register the new WC_STATIC procedure.

// Failed to start the timer management thread.
// Failed to start the input queue hook.

// Failed to allocate storage for storing the process data
// Failed to retrieve the process information block.
// Failed to retrieve size of initialization file data
// Failed allocation for retrieving initialization file data
// Failed to retrieve data from initialization file
// to allocate storage for retrieving the module name
// to retrieve the module name

// to register clean-up function.


#endif
