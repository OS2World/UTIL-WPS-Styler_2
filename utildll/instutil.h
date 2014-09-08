//==========================================================================\
// instutil.h: funzioni di utilit… per programma di installazione
//==========================================================================/

#ifndef INSTUTIL_H_
   #define INSTUTIL_H_

APIRET APIENTRY DosReplaceModule(PSZ pszOld, PSZ pszNew, PSZ pszBkup);

// struttura copiatura file

#pragma pack(2)
typedef struct {
   FILESTATUS3 fss;      // dati file origine
   FILESTATUS3 fst;      // dati file destinazione
   CHAR achsrc[260];     // path sorgente file da copiare
   CHAR achtrg[260];     // path destinazione in cui copiare
                         // entrambi i path devono includere slash finale
   HWND hprgrs;          // handle progress window
   USHORT fl;            // modalit… copia
   USHORT cbsrc;         // lunghezza nome path sorgente
   USHORT cbtrg;         // lunghezza nome path destinazione
   USHORT msg;           // msg da scrivere nel file di log
   USHORT to;            // eventuale id messaggio "to:"
   USHORT confirm;       // msg conferma sovrascrittura
   USHORT err;           // msg di errore da mostrare in caso di fallimento
   USHORT csteps;        // numero passi totali procedura
   PULONG pidx;          // indice incremento se non Š NULL viene spedito
                         // messaggio incremento alla progress window
   PLOGFILE plf;
} FILECPY, * PFILECPY;

typedef struct {
   FILECPY fc;           // modalit… usata per copiare i singoli file
   PSZ pdir;             // directory contenente l'albero da copiare
   PSZ pmask;            // maschera file da copiare
   ULONG fl;             // attributi file da copiare
   ULONG cerr;           // numero errori non critici
   USHORT msg;           // messaggio creazione directory;
   USHORT err;           // messaggio errore creazione directory
} TREECPY, *PTREECPY;

#pragma pack()



// flag di settaggio
#define FCPYF_OVWRT       0x0001       // sovrascrive tutti i file
#define FCPYF_OVWCONF     0x0002       // sovrascrive chiedendo conferma
#define FCPYF_OVWCONFNEW  0x0004       // chiede conferma per sovrascrivere
                                       // i file + nuovi
#define FCPYF_BACKUP      0x0008       // crea copia backup file sovrascritti
#define FCPYF_UNLOCK      0x0010       // unlocka i moduli bloccati
#define FCPYF_DEFER       0x0020       // postpone copia al riavvio del sistema
#define FCPYF_ERRNC       0x0040       // in caso di errore mostra msg
                                       // ma non restituisce FALSE
#define FCPYF_STOPONERR   0x0040       // considera tutti gli errori critici
                                       // e in caso di errore mostra messaggio
                                       // errore, restituisce -1 e ferma
                                       // l'elaborazione.
                                       // Altrimenti scrive il messaggio di
                                       // errore solo nel log, incrementa
                                       // l'eventuale controllo di progresso
                                       // e restituisce 0 (errore non critico)
// flag usata internamente da treecpy
#define FCPYF_ABORT       0x8000       // usata da instreecpy() quando si Š
                                       // verificato un errore nella
                                       // creazione di una directory in modo
                                       // che il programma non tenti di copiare
                                       // file nella directory mancante, ma si
                                       // limiti a riportare il messaggio di
                                       // errore (questo serve solo quando non
                                       // c'Š FCPYF_STOPONERR

// macro usate da copytree

// ottiene PFILECPY da PVOID
#define MKPCF(x) ((PFILECPY)(x))
#define MKPCT(x) ((PTREECPY)(x))

// struttura cancellazione file

#pragma pack(2)
typedef struct {
   FILESTATUS3 fs;       // dati file da cancellare
   CHAR path[260];       // path file da cancellare
   HWND hprgrs;          // handle progress window
   USHORT cbpath;        // lunghezza nome path sorgente
   USHORT msg;           // msg da scrivere nel file di log
   USHORT err;           // msg di errore da mostrare in caso di fallimento
   USHORT stop;          // se TRUE considera l'errore critico, cioŠ in caso
                         // di errore mostra err e restituisce -1,
                         // altrimenti, se definiti membri progress window,
                         // spedisce messaggio di incremento e restituisce 0
   USHORT csteps;        // numero passi totali procedura
   PULONG pidx;          // indice incremento se non Š NULL viene spedito
                         // messaggio incremento alla progress window
   PLOGFILE plf;
} FILEDEL, * PFILEDEL;

typedef struct {
   FILEDEL fd;           // dati cancellazione file
   PSZ pdir;             // directory contenente l'albero da cancellare
   PSZ pmask;            // maschera file da cancellare
   ULONG fl;             // attributi file da cancellare
   ULONG cerr;           // numero errori non critici
   USHORT msg;           // messaggio rimozione directory;
   USHORT err;           // messaggio errore rimozione directory
} TREEDEL, *PTREEDEL;

#pragma pack()

// ottiene PFILEDEL da PVOID
#define MKPDF(x) ((PFILEDEL)(x))
#define MKPDT(x) ((PTREEDEL)(x))

// flag usate da prfinsstr e prfdelstr
#define PRFINCHECK     0x0000   // verifica se stringa Š presente
#define PRFINSTART     0x0001   // inserisce stringa a inizio
#define PRFINEND       0x0002   // inserisce stringa alla fine
#define PRFINZEROEND   0x0004   // i dati devono essere terminati da zero
#define PRFINNOTFOUND  0        // stringa non trovata
#define PRFINFOUND     1        // stringa presente o inserzione riuscita
#define PRFINFERROR    -1       // errore
#define PRFINFOVRFLW   -2       // inserzione impossibile per overflow

// strutture per la creazione di oggetti
// dati setup oggetto
typedef struct {
   PSZ pclass;          // classe oggetto
   ULONG ititle;        // id stringa titolo oggetto
   PSZ psetup;          // stringa di setup
   PSZ ploc;            // location
} WPSOBJ, * PWPSOBJ;

// dati per la creazione inclusi messaggi log e progresso
typedef struct {
   PWPSOBJ po;          // indirizzo dati creazione oggetto
   PLOGFILE plf;        // indirizzo dati file di log
   PSZ path;            // path installazione eventualmente da inserire nella
                        // stringa di setup al posto di \t
   CHAR title[256];     // buffer per titolo oggetto
   CHAR setup[1024];    // buffer per setup oggetto
   USHORT msg;          // msg da scrivere nel log per descrivere operazione
   USHORT err;          // msg da scrivere nel log in caso di errore
   BOOL stop;           // se TRUE considera gli errori critici e in caso di
                        // errore restituisce FALSE altrimenti incrementa
                        // il contatore degli errori e restituisce TRUE
   USHORT cerr;         // contatore errori
   PULONG pidx;         // contatore da incrementare per finestra progresso
   HWND hprgrs;         // handle finestra progresso
   ULONG csteps;        // passi elaborazione totali
} CREATOBJ, * PCREATOBJ;


// prototipi funzioni

BOOL APIENTRY prfChkObj(PSZ pobj);
BOOL APIENTRY prfdelstr(HINI hini, PSZ app, PSZ key, PSZ str,
                        ULONG cbstr, PSZ sep, ULONG cbsep, ULONG fl);
INT APIENTRY prfinsstr(HINI hini, PSZ app, PSZ key, PSZ str,
                       ULONG cbstr, PSZ sep, ULONG cbsep, ULONG flins);
INT APIENTRY insfilecpy(PFILECPY pfc, PSZ pszfile, PSZ psznew);
BOOL fcpy(PFILECPY pfc, BOOL fl);
INT APIENTRY insfiledel(PFILEDEL pfd, PSZ pszfile);
BOOL APIENTRY instreecpy(PTREECPY ptc);
LONG tcpy(PPROCESSTREEDATA pptd);
LONG tcdirpre(PPROCESSTREEDATA pptd);
LONG tcdirpost(PPROCESSTREEDATA pptd);
BOOL APIENTRY instreedel(PTREEDEL ptd);
LONG tddel(PPROCESSTREEDATA pptd);
LONG tddirpre(PPROCESSTREEDATA pptd);
LONG tddirpost(PPROCESSTREEDATA pptd);
BOOL insconfirm(PFILECPY pfc);
BOOL APIENTRY insmakeobj(PCREATOBJ pco);

#endif
