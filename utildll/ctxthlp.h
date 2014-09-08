// ctxthlp.h

BOOL APIENTRY WsetChlp(HWND hwnd, ULONG idctl, ULONG idstr);
BOOL APIENTRY WsetChlpMulti(HWND hwnd, ULONG idstart, ULONG idend, ULONG idstr);
BOOL APIENTRY WsetChlpSpin(HWND hwnd, ULONG idctl, ULONG idstr);
BOOL APIENTRY WsetChlpCombo(HWND hwnd, ULONG idctl, ULONG idstr);
VOID APIENTRY WsetChlpMorebox(HWND hwnd, ULONG idctl,
                             ULONG idcpt, ULONG idswp, ULONG idfnd);
VOID APIENTRY WsetChlpClbox(HWND hwnd, ULONG idctl, ULONG idcpt, ULONG idlst,
                     ULONG identry, ULONG idadd, ULONG idrem, ULONG idfnd);
VOID APIENTRY WsetChlpElbox(HWND hwnd, ULONG idctl, ULONG idstr);


// controllare se ID in clbox.h, elbox.h e more.h cambiano
#define FID_CAPTION                  0xa000
#define FID_LIST                     0xa001
#define FID_ENTRY                    0xa002
#define FID_ADD                      0xa003
#define FID_REMOVE                   0xa004
#define FID_FIND                     0xa005
#define FID_SWAP                     0xa001

