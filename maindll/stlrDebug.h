#ifndef _DEBUG_H_
   #define _DEBUG_H_

#ifdef DEBUGGING
   VOID _dbgInit(VOID);
   VOID _dbgEnd(VOID);
   VOID _dbgCreate1(HWND hwnd, PFRAMECDATA pfd, PCREATESTRUCT pc);
   VOID _dbgCreate2(PSTLRFRAME pwd);
   VOID _dbgCreate3(HWND hwnd, PCREATESTRUCT pc);
   VOID _dbgPrintf(PSZ psz, ...);

   #define dbgInit()                 _dbgInit()
   #define dbgEnd()                  _dbgEnd()
   #define dbgPrintf1(a)             _dbgPrintf(a)
   #define dbgPrintf2(a,b)           _dbgPrintf(a,b)
   #define dbgPrintf3(a,b,c)         _dbgPrintf(a,b,c)
   #define dbgPrintf4(a,b,c,d)       _dbgPrintf(a,b,c,d)
   #define dbgPrintf5(a,b,c,d,e)     _dbgPrintf(a,b,c,d,e)
   #define dbgPrintf6(a,b,c,d,e,f)   _dbgPrintf(a,b,c,d,e,f)
   #define dbgPrintf7(a,b,c,d,e,f,g) _dbgPrintf(a,b,c,d,e,f,g)
   #define dbgCreate1(h, pfcd, pc)   _dbgCreate1(h, pfcd, pc)
   #define dbgCreate2(pwd)           _dbgCreate2(pwd)
   #define dbgCreate3(h, pc)         _dbgCreate3(h, pc)
#else
   #undef DEBUGMEM
   #define dbgInit()                  ((void)0)
   #define dbgEnd()                   ((void)0)
   #define dbgPrintf1(a)              ((void)0)
   #define dbgPrintf2(a,b)            ((void)0)
   #define dbgPrintf3(a,b,c)          ((void)0)
   #define dbgPrintf4(a,b,c,d)        ((void)0)
   #define dbgPrintf5(a,b,c,d,e)      ((void)0)
   #define dbgPrintf6(a,b,c,d,e,f)    ((void)0)
   #define dbgPrintf7(a,b,c,d,e,f,g)  ((void)0)
   #define dbgCreate1(h, pfcd, pc)    ((void)0)
   #define dbgCreate2(pwd)            ((void)0)
   #define dbgCreate3(h, pc)          ((void)0)
#endif

#endif
