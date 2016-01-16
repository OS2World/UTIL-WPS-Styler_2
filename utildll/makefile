# makefile
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

.c.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Ss /Q /O /Rn /Ge- /G5 /Gu /C %s

.\afcutl05.dll: \
    .\lock.obj \
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
     .\lock.obj
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
#    rc.exe -x2 .\common.res afcutl05.dll

.\common.res: \
    .\common.rc \
    {.;$(INCLUDE)}progress.dlg \
    {.;$(INCLUDE)}progress.h \
    {.;$(INCLUDE)}editcol.dlg \
    {.;$(INCLUDE)}editcol.h \
    {.;$(INCLUDE)}common.h \
    {.;$(INCLUDE)}commonbs.h

.\strutil.obj: \
    .\strutil.c \
    {.;$(INCLUDE);}commonbs.h

.\statusbar.obj: \
    .\statusbar.C \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h

.\progress.obj: \
    .\progress.c \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}progress.h

.\pmapputil.obj: \
    .\pmapputil.c \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}pmappapi.h \
    {.;$(INCLUDE);}pmapi.h

.\logutil.obj: \
    .\logutil.c \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}pmappapi.h \
    {.;$(INCLUDE);}logutil.h \
    {.;$(INCLUDE);}pmapi.h

.\instutil.obj: \
    .\instutil.c \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}pmappapi.h \
    {.;$(INCLUDE);}instutil.h \
    {.;$(INCLUDE);}pmapi.h

.\ctxthllp.obj: \
    .\ctxthlp.c \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}pmappapi.h \
    {.;$(INCLUDE);}clbox.h \
    {.;$(INCLUDE);}elbox.h \
    {.;$(INCLUDE);}more.h \
    {.;$(INCLUDE);}ctxthlp.h \
    {.;$(INCLUDE);}pmapi.h

.\pmappaux.obj: \
    .\pmappaux.c \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}pmappapi.h \
    {.;$(INCLUDE);}pmapi.h

.\pmappapi.obj: \
    .\pmappapi.c \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}pmappapi.h \
    {.;$(INCLUDE);}pmapi.h

.\pmapi.obj: \
    .\pmapi.C \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}pmapi.h

.\notebook.obj: \
    .\notebook.c \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}pmappapi.h \
    {.;$(INCLUDE);}notebook.h \
    {.;$(INCLUDE);}pmapi.h

.\moreaux.obj: \
    .\moreaux.c \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}more.h

.\more.obj: \
    .\more.c \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}more.h

.\image.obj: \
    .\image.C \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}image.h

.\groupbox.obj: \
    .\groupbox.C \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h

.\fontdlg.obj: \
    .\fontdlg.c \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h

.\fileutil.obj: \
    .\fileutil.c \
    {.;$(INCLUDE);}commonbs.h

.\elbxlbox.obj: \
    .\elbxlbox.c \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}elbox.h

.\elbx.obj: \
    .\elbx.c \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}elbox.h

.\editcol.obj: \
    .\editcol.c \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}editcol.h

.\common.obj: \
    .\common.C \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h

.\clbxsub.obj: \
    .\clbxsub.c \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}clbox.h

.\clbxaux.obj: \
    .\clbxaux.c \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}clbox.h

.\clbx.obj: \
    .\clbx.c \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}api.h \
    {.;$(INCLUDE);}clbox.h

.\bar.obj: \
    .\bar.C \
    {.;$(INCLUDE);}commonbs.h \
    {.;$(INCLUDE);}common.h \
    {.;$(INCLUDE);}bar.h \
    {.;$(INCLUDE);}api.h

.\api.obj: \
    .\api.c

.\util.obj: \
    .\util.c \
    {.;$(INCLUDE);}commonbs.h
