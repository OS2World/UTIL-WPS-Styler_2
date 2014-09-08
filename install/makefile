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

# Debug version    ---------------------------------------------------------
!ifdef DEBUG

CFLAGS = /I..\headers /I..\pref /Q /Ss /Ti /Rn /G5 /C
LFLAGS = /DE /E:2 /PM:PM /PACKD /A:4 /OPTF /NOL /M /L

!else
# RELEASE version  ---------------------------------------------------------

CFLAGS = /I..\headers /I..\pref /Q /Ss /O /Oc /Ol /Rn /G5 /C
LFLAGS = /E:2 /PM:PM /PACKD /A:4 /OPTF /NOL /M /L

!endif           # ---------------------------------------------------------

RCFLAGS = -r -n -i ..\headers -i ..\pref
RBFLAGS = -x2 -n

.rc.res:
  $(RC) $(RCFLAGS) $<

all: ..\warp4\install.exe

!ifdef DEBUG     # ---------------------------------------------------------

.C.obj:
   $(CC) $(CFLAGS) $<

..\warp4\install.exe: \
   util.obj \
   instpages.obj \
   instaux.obj \
   object.obj \
   iniconv.obj \
   install.obj \
   install.res \
   {$(LIB)}afcutl05.lib \
   install.def
   $(CL) @<<
      $(LFLAGS)
      /O:..\warp4\install.exe
      afcutl05.lib
      install.def
      util.obj
      instpages.obj
      instaux.obj
      object.obj
      iniconv.obj
      install.obj
<<
   $(RB) $(RBFLAGS) install.res ..\warp4\install.exe

util.obj:      util.c      *.h ..\headers\*.h

instpages.obj: instpages.c *.h ..\headers\*.h

instaux.obj:   instaux.c   *.h ..\headers\*.h

object.obj:    object.c    *.h ..\headers\*.h

iniconv.obj:   iniconv.c   *.h ..\headers\*.h

install.obj:   install.c   *.h ..\headers\*.h

!else            # ---------------------------------------------------------

install.obj: \
   util.c \
   install.c \
   instaux.c \
   instpages.c \
   object.c \
   iniconv.c \
   *.h \
   ..\headers\stlrDefs.h \
   ..\headers\stlrWinId.h
   $(CC) $(CFLAGS) install.c util.c instaux.c instpages.c object.c iniconv.c

..\warp4\install.exe: install.obj install.res {$(LIB)}afcutl05.lib install.def
   $(CL) @<<
      $(LFLAGS)
      /O:..\warp4\install.exe
      afcutl05.lib
      install.def
      install.obj
<<
   $(RB) $(RBFLAGS) install.res ..\warp4\install.exe

!endif           # ---------------------------------------------------------

install.res: install.rc install.ico *.h ..\headers\stlrWinId.h

