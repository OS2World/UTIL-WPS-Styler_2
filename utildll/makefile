# makefile
# Creato dal MakeMake del WorkFrame/2 IBM alle 16:27:10 del 19 Novembre 1999
#
# Le azioni incluse in questo file Make sono:
#  Compile::Resource Compiler
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind

.SUFFIXES: .c .obj .rc .res 

.all: \
    .\afcutl05.dll

.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|dpfF.RES

{F:\dev\prj\UTIL_DLL\DLL2}.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|dpfF.RES

.c.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Ss /Q /O /Rn /Ge- /G5 /Gu /C %s

{F:\dev\prj\UTIL_DLL\DLL2}.c.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Ss /Q /O /Rn /Ge- /G5 /Gu /C %s

.\afcutl05.dll: \
    F:\dev\prj\UTIL_DLL\DLL2\lock.obj \
    .\util.obj \
    .\api.obj \
    .\bar.obj \
    .\clbx.obj \
    .\clbxaux.obj \
    .\clbxsub.obj \
    .\common.obj \
    .\editcol.obj \
    .\elbx.obj \
    .\elbxlbox.obj \
    .\fileutil.obj \
    .\fontdlg.obj \
    .\groupbox.obj \
    .\image.obj \
    .\instutil.obj \
    .\ctxthlp.obj \
    .\logutil.obj \
    .\more.obj \
    .\moreaux.obj \
    .\notebook.obj \
    .\pmapi.obj \
    .\pmappapi.obj \
    .\pmappaux.obj \
    .\pmapputil.obj \
    .\progress.obj \
    .\statusbar.obj \
    .\strutil.obj \
    .\common.res \
    {$(LIB)}DLL.def
    @echo " Link::Linker "
    @echo " Bind::Resource Bind "
    icc.exe @<<
     /B" /exepack:2 /pmtype:pm /packd /align:4 /optfunc /nologo"
     /Feafcutl05.dll 
     DLL.def
     F:\dev\prj\UTIL_DLL\DLL2\lock.obj
     .\util.obj
     .\api.obj
     .\bar.obj
     .\clbx.obj
     .\clbxaux.obj
     .\clbxsub.obj
     .\common.obj
     .\editcol.obj
     .\elbx.obj
     .\elbxlbox.obj
     .\fileutil.obj
     .\fontdlg.obj
     .\groupbox.obj
     .\image.obj
     .\instutil.obj
     .\ctxthlp.obj
     .\logutil.obj
     .\more.obj
     .\moreaux.obj
     .\notebook.obj
     .\pmapi.obj
     .\pmappapi.obj
     .\pmappaux.obj
     .\pmapputil.obj
     .\progress.obj
     .\statusbar.obj
     .\strutil.obj
<<
    rc.exe -x2 .\common.res afcutl05.dll

.\common.res: \
    F:\dev\prj\UTIL_DLL\DLL2\common.rc \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE)}progress.dlg \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE)}progress.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE)}editcol.dlg \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE)}editcol.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE)}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE)}commonbs.h

.\strutil.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\strutil.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h

.\statusbar.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\statusbar.C \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h

.\progress.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\progress.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}progress.h

.\pmapputil.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\pmapputil.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}pmappapi.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}pmapi.h

.\logutil.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\logutil.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}pmappapi.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}logutil.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}pmapi.h

.\instutil.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\instutil.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}pmappapi.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}instutil.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}pmapi.h

.\ctxthllp.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\ctxthlp.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}pmappapi.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}clbox.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}elbox.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}more.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}ctxthlp.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}pmapi.h

.\pmappaux.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\pmappaux.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}pmappapi.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}pmapi.h

.\pmappapi.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\pmappapi.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}pmappapi.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}pmapi.h

.\pmapi.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\pmapi.C \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}pmapi.h

.\notebook.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\notebook.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}pmappapi.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}notebook.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}pmapi.h

.\moreaux.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\moreaux.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}more.h

.\more.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\more.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}more.h

.\image.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\image.C \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}image.h

.\groupbox.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\groupbox.C \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h

.\fontdlg.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\fontdlg.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h

.\fileutil.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\fileutil.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h

.\elbxlbox.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\elbxlbox.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}elbox.h

.\elbx.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\elbx.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}elbox.h

.\editcol.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\editcol.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}editcol.h

.\common.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\common.C \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h

.\clbxsub.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\clbxsub.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}clbox.h

.\clbxaux.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\clbxaux.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}clbox.h

.\clbx.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\clbx.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}clbox.h

.\bar.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\bar.C \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}common.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}bar.h \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}api.h

.\api.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\api.c

.\util.obj: \
    F:\dev\prj\UTIL_DLL\DLL2\util.c \
    {F:\dev\prj\UTIL_DLL\DLL2;$(INCLUDE);}commonbs.h
