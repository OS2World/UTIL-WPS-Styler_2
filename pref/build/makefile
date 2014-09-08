# To add new extensions to the inference rules uncomment and redefine this:
#.SUFFIXES:
#
#.SUFFIXES: \
#    .C .obj .rc .res

# compiler, linker, resource compiler, resource binder MACRO

CC = icc.exe
CL = ilink.exe
RC = rc.exe
RB = rc.exe

# compiler and linker flags

# Debug version
!ifdef DEBUG

CFLAGS = /I..\..\headers /Ss /Q /Ti /Rn /G5 /C
LFLAGS = /DE /E:2 /PM:PM /PACKD /A:4 /OPTF /NOL /M /L

!else
# RELEASE version

CFLAGS = /I..\..\headers /Ss /Q /O /Oc /Ol /Rn  /G5 /C
LFLAGS = /E:2 /PM:PM /PACKD /A:4 /OPTF /NOL /M /L

!endif

RCFLAGS = -r -n -i ..\..\headers -i ..
RBFLAGS = -x2 -n

all: ..\..\warp4\smartwin.exe

{..}.c.obj:
   $(CC) $(CFLAGS) $<

# {..}.rc.res:
#   $(RC) $(RCFLAGS) $<

..\..\warp4\smartwin.exe: \
   wroll.obj \
   _api.obj \
   bmpseldlg.obj \
   cover.obj \
   exceptions.obj \
   general.obj \
   hidetitle.obj \
   dlgwin.obj \
   maxwin.obj \
   moukeybdlg.obj \
   mouseopt.obj \
   object.obj \
   main.obj \
   util.obj \
   sizemove.obj \
   titlebar.obj \
   titlenew.obj \
   txtctlgen.obj \
   viowin.obj \
   winkey.obj \
   initnbk.obj \
   SMARTWIN.res \
   {$(LIB)}afcutl05.lib \
   smartwin.def
   $(CL) @<<
      $(LFLAGS)
      /O:..\..\warp4\smartwin.exe
      afcutl05.lib
      smartwin.def
      wroll.obj
      _api.obj
      bmpseldlg.obj
      cover.obj
      exceptions.obj
      general.obj
      hidetitle.obj
      dlgwin.obj
      maxwin.obj
      moukeybdlg.obj
      mouseopt.obj
      object.obj
      main.obj
      util.obj
      sizemove.obj
      titlebar.obj
      titlenew.obj
      txtctlgen.obj
      viowin.obj
      winkey.obj
      initnbk.obj
<<
    $(RB) $(RBFLAGS) SMARTWIN.res ..\..\warp4\smartwin.exe

SMARTWIN.res: \
    ..\SMARTWIN.RC \
    ..\..\headers\stlrDefs.h \
    ..\..\headers\stlrWinID.h \
    ..\..\images\*.ico \
    ..\..\images\*.bmp
    $(RC) $(RCFLAGS) ..\smartwin.rc smartwin.res

wroll.obj: \
    ..\wroll.c \
    ..\*.h \
    ..\..\headers\*.h

winkey.obj: \
    ..\winkey.c \
    ..\*.h \
    ..\..\headers\*.h

viowin.obj: \
    ..\viowin.c \
    ..\*.h \
    ..\..\headers\*.h

txtctlgen.obj: \
    ..\txtctlgen.c \
    ..\*.h \
    ..\..\headers\*.h

titlenew.obj: \
    ..\titlenew.c \
    ..\*.h \
    ..\..\headers\*.h

titlebar.obj: \
    ..\titlebar.c \
    ..\*.h \
    ..\..\headers\*.h

sizemove.obj: \
    ..\sizemove.c \
    ..\*.h \
    ..\..\headers\*.h

util.obj: \
    ..\util.c \
    ..\*.h \
    ..\..\headers\*.h

main.obj: \
    ..\main.c \
    ..\*.h \
    ..\..\headers\*.h

object.obj: \
    ..\object.c \
    ..\*.h \
    ..\..\headers\*.h

mouseopt.obj: \
    ..\mouseopt.c \
    ..\*.h \
    ..\..\headers\*.h

moukeybdlg.obj: \
    ..\moukeybdlg.c \
    ..\*.h \
    ..\..\headers\*.h

maxwin.obj: \
    ..\maxwin.c \
    ..\*.h \
    ..\..\headers\*.h

dlgwin.obj: \
    ..\dlgwin.c \
    ..\*.h \
    ..\..\headers\*.h

hidetitle.obj: \
    ..\hidetitle.c \
    ..\*.h \
    ..\..\headers\*.h

general.obj: \
    ..\general.c \
    ..\*.h \
    ..\..\headers\*.h

exceptions.obj: \
    ..\exceptions.c \
    ..\*.h \
    ..\..\headers\*.h

cover.obj: \
    ..\cover.c \
    ..\*.h \
    ..\..\headers\*.h

bmpseldlg.obj: \
    ..\bmpseldlg.c \
    ..\*.h \
    ..\..\headers\*.h

initnbk.obj: \
   ..\initnbk.c \
    ..\*.h \
    ..\..\headers\*.h

_api.obj: \
    ..\_api.c
