// afcutb00.h

#ifndef AFCUTB00_H_
   #define AFCUTB00_H_

#define DOS_INFO   0x100

#define ALLFILES                    FILE_ARCHIVED | FILE_DIRECTORY |\
                                    FILE_SYSTEM | FILE_HIDDEN |\
                                    FILE_READONLY 
#define CBFINFO           (sizeof(FILESTATUS3))
#define FLFACCLIMIT       (FILE_READONLY | FILE_SYSTEM | FILE_HIDDEN)

// calcola numero item di un array
#define CAITEMS(array, type) (sizeof(array) / sizeof(type))


// struttura usata da ProcessTree
typedef struct _PROCESSTREEDATA {
   FILEFINDBUF3 fBuf;       // dati file trovato
   CHAR achPath[260];       // file specifiche file da ricercare
   CHAR achMask[80];        // maschera di ricerca
   ULONG flfind;            // flag ricerca
   PVOID pdata;             // user data
   LONG (* pffile)(struct _PROCESSTREEDATA * pptd);
   LONG (* pfdirpre)(struct _PROCESSTREEDATA * pptd);
   LONG (* pfdirpost)(struct _PROCESSTREEDATA * pptd);
} PROCESSTREEDATA, * PPROCESSTREEDATA;


// various:
PVOID APIENTRY getPrfData(HINI hini, PSZ pszApp, PSZ pszKey, PULONG pul);
VOID APIENTRY lockincr(PULONG pul);
VOID APIENTRY lockdecr(PULONG pul);
ULONG APIENTRY lockxchng(PULONG pul, ULONG val);
PVOID APIENTRY tsmalloc(ULONG cb);
VOID APIENTRY tsfree(PVOID pv);
INT APIENTRY tsheapmin(VOID);
BOOL APIENTRY uExeFullName(PSZ pszPath, ULONG cch);
BOOL APIENTRY uEditExeExt(PSZ pszPath, ULONG cch, PSZ pszExt);
BOOL APIENTRY uDateTime(PSZ pszdate, PSZ psztime);
LONG APIENTRY uGetSval(LONG lID);
ULONG APIENTRY ucrc(PBYTE pb, ULONG cb);
VOID APIENTRY upb2an(PBYTE pin, PBYTE pout, ULONG cb, ULONG cxline);
VOID APIENTRY uan2pb(PBYTE pin, PBYTE pout);

// string utilities
VOID APIENTRY catuch2str(PSZ psz, ULONG ul);
LONG APIENTRY linedel(PSZ pszline);
PSZ APIENTRY lin1str(PSZ psz, PSZ substr);
ULONG APIENTRY memdel(PBYTE pdata, ULONG cbdata, PBYTE pdel, ULONG cbdel,
                      PBYTE psep, ULONG cbsep);
PBYTE APIENTRY memfind(PBYTE pdata, ULONG cbdata, PBYTE pfind, ULONG cbfind,
                       PBYTE psep, ULONG cbsep);
PSZ APIENTRY pszlastchr(PSZ psz, CHAR ch);
BOOL APIENTRY pszwldcmp(char* mask, char* cmp);
char* APIENTRY pszcpy(char* target, char* source);
PSZ APIENTRY pszcpycat(PSZ trg, INT cstr, ...);
ULONG APIENTRY pszncpy(PSZ target, PSZ source, ULONG cch);
int APIENTRY pszicmp(const char* s1, const char* s2);
int APIENTRY psznicmp(const char* s1, const char* s2, unsigned int n);
ULONG APIENTRY strdel(PSZ pszbuf, PSZ pszstr, ULONG cbbuf);
PSZ APIENTRY stristr(PSZ str, PSZ substr, ULONG cb);
LONG APIENTRY strrepl(PSZ ins, PSZ psznew, PSZ endch);
ULONG APIENTRY strins(PSZ psz, PSZ substr, PSZ ins, ULONG cb);

// io utilities
#define ioFNameFromPath(psz)            pszlastchr((psz), '\\')
BOOL APIENTRY ioBackup(PSZ pszFile, PSZ pszExt);
LONG APIENTRY ioCountTree(PSZ path, PSZ pmask, ULONG flattr, BOOL fl);
PSZ APIENTRY ioEditExt(PSZ pszFile, PSZ pszExt);
PSZ APIENTRY ioF2psz(PSZ pszFile, PULONG pcb);
BOOL APIENTRY ioFExists(PSZ pszFile, PULONG pul);
INT APIENTRY ioFilOlder(PFDATE pfds, PFTIME pfts, PFDATE pfdt, PFTIME pftt);
BOOL APIENTRY ioFlushBuf(PSZ pszFile, PSZ pbuf, PSZ* ppend);
BOOL APIENTRY ioMakePath(PSZ pszPath);
LONG APIENTRY ioProcessTree(PPROCESSTREEDATA pptd);
ULONG APIENTRY ioPsz2f(PSZ pszFile, PSZ pdata, ULONG cb);
PSZ APIENTRY ioUniqFName(PSZ pszFile);


/* API description:

 PVOID APIENTRY getPrfData(HINI hini, PSZ pszApp, PSZ pszKey);
 richiede dimensioni dati memorizzati nel file ini, allocando la memoria
 necessaria per scriverli e restituisce l'indirizzo della memoria,
 0xffffffff in caso di errore o NULL in caso di mancanza dati
 Vengono sempre allocati 0x10000 byte in modo che i dati possano essere
 editati
 parametri:
 HINI hini, PSZ pszApp, PSZ pszKey
 valore restituito:
 PVOID: 0xffffffff = errore, NULL = no dati, altro = indirizzo memoria

 PVOID APIENTRY tsmalloc(ULONG cb):
 Subsystem Thread Safe malloc

 VOID APIENTRY tsfree(PVOID pv):
 Subsystem Thread Safe free

 INT APIENTRY tsheapmin(VOID):
 Subsystem Thread Safe _heapmin

 BOOL APIENTRY uExeFullName(PSZ pszPath, ULONG cch):
 ritorna il nome incluso il percorso dell'eseguibile del processo attivo
 parametri:
 PSZ pszPath: buffer in cui sar… scritto il percorso dell'eseguibile
 ULONG cch: dimensione del buffer
 valore restituito:
 BOOL: TRUE = successo, FALSE = errore

 BOOL APIENTRY uEditExeExt(PSZ pszPath, ULONG cch, PSZ pszExt):
 ricava il nome del modulo che ha avviato il processo corrente e
 sostituisce l'estensione EXE con pszExt.
 parametri:
 PSZ pszPath: buffer in cui sar… scritto il percorso dell'eseguibile
 ULONG cch: dimensione del buffer
 PSZ pszExt: estensione da dare al file INI. Il punto pu• essere specificato
             o omesso.
 valore restituito:
 BOOL: TRUE = successo, FALSE = errore

 BOOL APIENTRY uDateTime(PSZ pszdate, PSZ psztime):
 scrive la data e l'ora correnti nel formato appropriato

 LONG uGetSval(LONG lID):
 restituisce il valore corrispondente ad un settaggio di sistema
 se lID Š > di DOS_INFO (0x100) restituisce il valore della flag di
 DosQuerySysInfo() pari a lID - 0x100, altrimenti funziona come
 WinQuerySysValue()
 parametri:
 LONG lID: id valore richiesto
 valore restituito:
 LONG: valore restituito (-1 in caso di errore)

 ULONG APIENTRY ucrc(PBYTE pb, ULONG cb):
 restituisce il crc di cb byte puntati da pb
 parametri:
 PBYTE pb: indirizzo buffer contenente dati di cui calcolare CRC
 ULONG cb: dimensione dati di cui calcolare CRC
 valore restituito:
 ULONG: CRC / 0 in caso di errore

 VOID APIENTRY catuch2str(PSZ psz, ULONG ul):
 converte ul (massimo 99) in una stringa concatenandola a psz
 parametri:
 PSZ psz: stringa in cui scrivere il valore convertito
 ULONG ul: valore numerico da convertire in stringa
 valore restituito:
 VOID

 VOID APIENTRY upb2an(PBYTE pin, PBYTE pout, ULONG cb, ULONG cxline):
 trasforma il codice in una sequenza di caratteri alfanumerici + ':' e ';'
 parametri:
 PBYTE pin: indirizzo dati da convertire
 PBYTE pout: indirizzo buffer dati convertiti
 ULONG cb: dimensione dati da convertire
 ULONG cxline: dimensione formattazione linea output
 valore restituito:
 VOID

 VOID APIENTRY uan2pb(PBYTE pin, PBYTE pout):
 decodifica una stringa codificata con upb2an
 il buffer pout deve avere una dimensione almeno di 3/4 la dimensione di
 pin
 parametri:
 PBYTE pin: indirizzo dati da decodificare
 PBYTE pout: indirizzo buffer in cui scrivere dati decodificati

 LONG APIENTRY linedel(PSZ pszline):
 cancella dalla posizione corrente a fine riga includendo anche i
 caratteri di terminazione riga, restituisce il numero dei caratteri
 cancellati (in negativo)
 parametri:
 PSZ pszline: indirizzo linea da cancellare
 valore restituito:
 LONG : quantit… caratteri da sottrarre a vecchia lunghezza buffer

 PSZ APIENTRY lin1str(PSZ psz, PSZ substr):
 restituisce un puntatore alla prima linea che inizia con "substr"
 o NULL se non viene trovata alcuna occorrenza
 (versione internazionale)
 parametri:
 PSZ psz: indirizzo buffer testo
 PSZ substr: stringa da ricercare
 valore restituito:
 PSZ : indirizzo linea trovata, NULL se nessuna occorrenza

 ULONG APIENTRY memdel(PBYTE pdata, ULONG cbdata, PBYTE pdel, ULONG cbdel,
                       PBYTE psep, ULONG cbsep)
 Cancella cb byte da pdata eliminando eventuali caratteri di separazione
 in eccesso. Se la stringa dei separatori non Š specificata viene solo
 cancellato pdel.
 parametri:
 PBYTE pdata: dati da cui cancellare i dati pdel
 ULONG cbdata: dimensione dati
 PBYTE pdel: indirizzo stringa da cancellare
 ULONG cbdel: dimensione stringa da cancellare
 PBYTE psep: array di separatori validi. NULL se i separatori devono
             essere tralasciati
 ULONG cbsep: lunghezza array separatori
 valore restituito:
 nuova lunghezza dati

 PBYTE APIENTRY memfind(PBYTE pdata ULONG cbdata, PBYTE pfind, ULONG cbfind,
                        PBYTE psep, ULONG cbsep)
 Trova la prima ricorrenza della sequenza di byte pfind di lunghezza
 cbfind nei dati pdata di lunghezza cbdata.
 Se Š specificato psep controlla che la sequenza di byte sia in posizione
 iniziale, finale, o separata da uno dei caratteri presenti nell'array
 psep di cbsep elementi.
 parametri:
 parametri:
 PBYTE pdata: dati in cui ricercare pfind
 ULONG cbdata: dimensione dati
 PBYTE pfind: indirizzo stringa da ricercare
 ULONG cbfind: dimensione stringa da ricercare
 PBYTE psep: array di separatori validi. NULL se i separatori devono
             essere tralasciati
 ULONG cbsep: lunghezza array separatori
 valore restituito
 puntatore a pfind

 PSZ APIENTRY pszlastchr(PSZ psz, CHAR ch):
 trova nella stringa l'indirizzo dell'ultima occorrenza del carattere ch
 + 1 (il carattere successivo a ch)
 parametri:
 PSZ psz: stringa
 CHAR ch: carattere da ricercare
 valore restituito:
 PSZ pszCh: carattere successivo all'ultima occorrenza di ch in psz
            se ch non Š presente ritorna psz

 BOOL APIENTRY pszwldcmp(char* mask, char* cmp):
 compara due stringhe di cui la prima contiene caratteri jolly
 parametri:
 char* mask: maschera contenente wildchar
 char* cmp: stringa da comparare con "mask"
 valore restituito:
 BOOL: TRUE se cmp risponde al criterio specificato in mask

 char* APIENTRY pszcpy(char* target, char* source)
 copia source su target, ritorna un puntatore alla fine di target

 PSZ APIENTRY pszcpycat(PSZ trg, PSZ src, INT cstr, ...)
 Copia cstr stringhe concatenandole in trg.
 Restituisce un puntatore alla fine della stringa

 ULONG APIENTRY pszncpy(PSZ target, PSZ source, ULONG cch)
 copia n caratteri di source su target terminando con un carattere NULL
 target in n Š compreso lo 0 di teminazione
 restituisce il numero di caratteri copiati (lunghezza target: lo 0
 di terminazione non Š incluso
 esempio:
 pszncpy(buf, "0123456789", 5);
 copia in buf: "0123" e restituisce 4

 int APIENTRY pszicmp(const char* s1, const char* s2)
 compara due stringhe in modo case insensitivo

 int APIENTRY psznicmp(const char* s1, const char* s2, unsigned int n)
 compara due stringhe in modo case insensitivo per n caratteri

 ULONG APIENTRY strdel(PSZ pszbuf, PSZ pszstr, ULONG cbbuf)
 cancella una substringa (pszstr) contenuta in una stringa (pszbuf)
 restituisce la nuova lunghezza di pszbuf (cbbuf Š la lunghezza originaria)

 PSZ APIENTRY stristr(PSZ str, PSZ substr, ULONG cb)
 trova la substringa substr in str in modo case insensitivo
 rtestituisce il puntatore alla substringa in str o NULL se la
 substringa non Š presente

 LONG APIENTRY strrepl(PSZ ins, PSZ psznew, PSZ endch)
 sostituisce la substringa "psznew" in "psz" alla precedente stringa
 delimitata da endch restituisce differenza lunghezza tra nuova stringa
 e vecchia stringa
 parametri:
 PSZ ins: punto inserzione nuova stringa (inizio vecchia stringa)
 PSZ psznew: nuova stringa da inserire. Se NULL la vecchia stringa viene
             semplicemente cancellata
 PSZ endch: caratteri che delimitano il riconoscimento della vecchia
            stringa (da ins alla prima occorrenza di un carattere incluso
            in endch
 valore restituito:
 LONG : differenza lunghezza tra vecchia stringa e nuova

 ULONG APIENTRY strins(PSZ psz, PSZ substr, PSZ ins, ULONG cb)
 inserisce una stringa in un altra restituendo la nuova lunghezza della
 stringa
 parametri:
 PSZ psz   : stringa in cui si deve effettuare inserzione
 PSZ substr: substringa da inserire
 PSZ ins   : indirizzo punto di inserzione
 ULONG cb  : dimensione corrente di psz
 valore restituito:
 ULONG: nuova lunghezza di psz

 BOOL APIENTRY ioBackup(PSZ pszFile, PSZ pszExt)
 Sostituisce l'estensione di pszFile con quella contenuta in pszExt
 e copia pszFile nel nuovo nome file ottenuto sovrascrivendo l'eventuale
 omonimo file preesistente !! se pszExt include punto iniziale, il punto
 viene cancellato.
 Restituisce TRUE in caso di successo FALSE in caso di errore
 se pszExt Š NULL ricava un nome di file unico partendo da 000
 parametri:
 PSZ pszFile: nome del file da copiare
 PSZ pszExt : estensione della copia di backup
 valore restituito:
 BOOL: TRUE/FALSE (successo/errore)

 LONG APIENTRY ioCountTree(PSZ path, PSZ pmask, ULONG flattr, BOOL fl)
 conta il numero di file (rispondenti alla maschera e agli attributi
 specificati), contenuti in una directory. La directory base del tree
 non Š inclusa nel conteggio.
 parametri:
 PSZ path: Š il path da cui iniziare la ricerca. Se la ricerca non Š
           ricorsiva al path deve essere concatenata la maschera di
           ricerca altrimenti al path deve essere concatenato "*" e la
           maschera di ricerca deve essere specificata a parte
 PSZ pmask: se specificata il conteggio Š ricorsivo
 ULONG flattr: gli attributi dei file da conteggiare
 BOOL fl: specifica, nel conteggio ricorsivo, se includere o meno le
          directory. Nel conteggio non ricorsivo, l'inclusione delle
          directory Š determinata dal valore di flattr
 valore restituito:
 LONG cf: < 0 = errore, >= 0 = numero di file/directory

 PSZ APIENTRY ioEditExt(PSZ pszFile, PSZ pszExt)
 cambia l'estensione di pszFile in pszExt. Se pszFile non ha estensione
 aggiunge il punto e l'estensione
 Nota: la memoria cui punta pszFile deve permettere la scrittura di altri
       4 byte dell'estensione
 parametri:
 PSZ pszFile: nome del file
 PSZ pszExt: nome dell'estensione
 valore restituito:
 PSZ == pszFile

 PSZ APIENTRY ioF2psz(PSZ pszFile, PULONG pcb)
 alloca la memoria necessaria e vi scrive il contenuto del file
 nell'ULONG puntato da pcb scrive la dimensione del file letto
 restituendo un puntatore all'area di memoria allocata o NULL in caso
 di errore. La stringa di contenuto del file Š sempre terminata da uno 0.
 parametri:
 PSZ pszFile: nome del file da leggere
 PULONG pcb : (in) alloca per la lettura (e successiva editazione) una
              quantit… di caratteri pari alla dimensione del file per
              *pcb. (out) quantit… caratteri letti
              se *pcb == 0 si limita a restituire la dimensione del file
              senza leggerlo
              se pcb == NULL si limita a leggere il file
 valore restituito:
 PSZ : indirizzo area memoria in cui Š stato scritto il contenuto del file

 #define ioFNameFromPath(psz)            pszlastchr(psz, '\\')
 riporta il nome del file inclusa l'estensione isolandolo dal percorso
 cercando l'ultima slash nel nome del file completo
 parametri:
 PSZ psz: nome file eventualmente comprendente il path
 valore restituito:
 PSZ pszSlash: nome file inclusa estensione

 BOOL APIENTRY ioFExists(PSZ pszFile, PULONG pul)
 controlla l'esistenza di un file:
 parametri:
 PSZ pszFile: nome del file
 PULONG pul: indirizzo variabile in cui scrivere rc DosFindFirst()
 valore restituito:
 BOOL = TRUE se il file esiste, FALSE se non esiste

 BOOL APIENTRY ioFilOlder(PFDATE pfds, PFTIME pfts, PFDATE pfdt, PFTIME pftt)
 compara le date e ore di due file
 parametri:
 PFDATE pfds: data file 1
 PFTIME pfts: ora file 1
 PFDATE pfdt: data file 2
 PFTIME pftt: ora file 2
 valore restituito:
 BOOL: (TRUE/FALSE) file1 + vecchio di file 2 / viceversa

 BOOL APIENTRY ioFlushBuf(PSZ pszFile, PSZ pbuf, PSZ* ppend)
 svuota il contenuto di un buffer sequenzialmente in un file.
 parametri:
 PSZ pszFile: nome file in cui svuotare il buffer
 PSZ pbuf: indirizzo buffer da copiare nel file
 PSZ *ppend: indirizzo indirizzo ultimo carattere scritto nel buffer
 valore restituito:
 BOOL: (TRUE/FALSE) successo/errore

 BOOL APIENTRY ioMakePath(PSZ pszPath)
 crea tutte le directory necessarie per costruire il path pszPath
 parametri:
 PSZ pszPath: path da creare se non esiste
 valore restituito:
 BOOL: TRUE = successo, FALSE = errore

 LONG ioProcessTree(PPROCESSTREEDATA pptd)
 Processa (opzionalmente in modo ricorsivo) il contenuto di una directory.
 La funzione usa la struttura PROCESSTREEDATA:
 typedef struct _PROCESSTREEDATA {
    FILEFINDBUF3 fBuf;       // dati file trovato
    CHAR achPath[260];       // file specifiche file da ricercare
    CHAR achMask[80];        // maschera di ricerca
    ULONG flfind;            // attributi file da ricercare
    PVOID pdata;             // user data
    LONG (* pffile)(struct _PROCESSTREEDATA * pptd);    // funzione
    LONG (* pfdirpre)(struct _PROCESSTREEDATA * pptd);  // funzione
    LONG (* pfdirpost)(struct _PROCESSTREEDATA * pptd); // funzione
 } PROCESSTREEDATA, * PPROCESSTREEDATA;
 Quando la funzione viene chiamata in modo ricorsivo Š necessario
 specificare la maschera di ricerca in achMask, mentre achPath deve
 contenere il path completo di partenza seguito da "\\*".
 In tal caso, per ogni directory trovata, viene modificato achPath
 aggiungendo il nome della directory seguito da "\\*", viene eseguita
 la funzione pfdirpre() (se non NULL), viene richiamata ricorsivamente
 ioProcessTree() e infine viene richiamata la funzione pfdirpost()
 (se non NULL), mentre per ogni file trovato, se questo soddisfa la
 maschera di ricerca achMask, viene richiamata la funzione pffile.
 Nel modo non ricorsivo per ogni file e directory soddisfacente la
 maschera achMask viene richiamato pffile()
 Tutte le funzioni (pffile, pfdirpre, pfdirpost) devono restituire
 il numero di file processati (>= 0) oppure -1 in caso di errore.
 valore restituito:
 LONG: >= 0 indica numero file trovati, < 0 indica errore

 ULONG APIENTRY ioPsz2f(PSZ pszFile, PSZ pdata, ULONG cb)
 scrive cb Byte dall'area di memoria puntata da pdata nel file pszFile
 sovrascrive il file se gi… esiste
 PSZ pszFile: nome file in cui si vuole scrivere pdata
 PSZ pdata  : indirizzo dati che si vogliono scrivere nel file
 ULONG cb   : quantit… dati da pdata da scrivere in pszFile
              se cb = 0 ricava la lunghezza di pdata tramite strlen
              ovviamente questo funziona correttamente solo se pdata Š
              una stringa terminata da 0
 valore restituito:
 ULONG : il numero dei byte scritti o 0 in caso di errore

 PSZ APIENTRY ioUniqFName(PSZ pszFile)
 crea nome file temporaneo sostituendo al carattere ? trovato nella
 stringa pszFile le cifre da 0 a 9 finch‚ non si ottiene nome file unico
 restituisce il nome del file trovato
 parametri:
 PSZ pszFile: nome del file contenente la maschera di sostituzione
 valore restituito:
 PSZ nome file unico
*/

#endif //  AFCUTB00_H_
