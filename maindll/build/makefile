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

APIINC = ..\..\include

# compiler and linker flags

# Debug version
!ifdef DEBUG

# CFLAGS = /I..\..\headers /Ss /Q /O /Oc /Rn /Ge- /G5 /C /DDEBUGGING=2
CFLAGS = /I..\..\headers /I$(APIINC) /Ss /Q /Ti /Rn /Ge- /G5 /C /DDEBUGGING=2
# LFLAGS = /E:2 /PM:PM /PACKD /A:4 /OPTF /NOL /M /L
LFLAGS = /DE /E:2 /PM:PM /PACKD /A:4 /OPTF /NOL /M /L

!else
# RELEASE version

CFLAGS = /I..\..\headers /I$(APIINC) /Ss /Q /O /Oc /Rn /Ge- /G5 /C
LFLAGS = /E:2 /PM:PM /PACKD /A:4 /OPTF /NOL /M /L

!endif

# RCFLAGS = -r -n -i ..\..\headers
# RBFLAGS = -x2 -n
#
# .rc.res:
#   $(RC) $(RCFLAGS) $<

.PATH.C = ..
.PATH.H = ..\..\HEADERS

{..}.c.obj:
   $(CC) $(CFLAGS) $<

all: ..\..\warp4\smartwin.dll smartwin.lib ..\..\warp4\styler20.dll

# SMARTWIN.DLL
..\..\warp4\smartwin.dll: \
   main.obj \
   utils.obj \
   frame.obj \
   debug.obj \
   combobox.obj \
   entry.obj \
   export.obj \
   hooks.obj \
   menu.obj \
   process.obj \
   titlebar.obj \
   workthreads.obj \
   button.obj \
   listbox.obj \
   static.obj \
   SMARTWIN.DEF
   $(CL) @<<
      $(LFLAGS)
      /O:..\..\warp4\smartwin.dll
      SMARTWIN.DEF
      main.obj
      utils.obj
      frame.obj
      debug.obj
      combobox.obj
      entry.obj
      export.obj
      hooks.obj
      menu.obj
      process.obj
      titlebar.obj
      workthreads.obj
      button.obj
      listbox.obj
      static.obj
<<

main.obj: \
    ..\main.c \
    ..\*.h \
    ..\..\headers\*.h

utils.obj: \
    ..\utils.c \
    ..\*.h \
    ..\..\headers\*.h

workthreads.obj: \
    ..\workthreads.c \
    ..\*.h \
    ..\..\headers\*.h

titlebar.obj: \
    ..\titlebar.c \
    ..\*.h \
    ..\..\headers\*.h

process.obj: \
    ..\process.c \
    ..\*.h \
    ..\..\headers\*.h

menu.obj: \
    ..\menu.c \
    ..\*.h \
    ..\..\headers\*.h

hooks.obj: \
    ..\hooks.c \
    ..\*.h \
    ..\..\headers\*.h

export.obj: \
    ..\export.c \
    ..\*.h \
    ..\..\headers\*.h

combobox.obj: \
    ..\combobox.c \
    ..\*.h \
    ..\..\headers\*.h

entry.obj: \
    ..\entry.c \
    ..\*.h \
    ..\..\headers\*.h

debug.obj: \
    ..\debug.c \
    ..\*.h \
    ..\..\headers\*.h

frame.obj: \
    ..\frame.c \
    ..\*.h \
    ..\..\headers\*.h

button.obj: \
    ..\button.c \
    ..\*.h \
    ..\..\headers\*.h

listbox.obj: \
    ..\listbox.c \
    ..\*.h \
    ..\..\headers\*.h

static.obj: \
    ..\static.c \
    ..\*.h \
    ..\..\headers\*.h

# SMARTWIN.LIB
smartwin.lib: smartwin.def
   IMPLIB /NOL smartwin.lib smartwin.def


# STYLER20.DLL
..\..\warp4\styler20.dll: \
   main2.obj \
   mle.obj \
   smartwin.lib \
   styler20.DEF
   $(CL) @<<
      $(LFLAGS)
      /O:..\..\warp4\styler20.dll
      styler20.def
      smartwin.lib
      main2.obj
      mle.obj
<<

main2.obj: \
   ..\main2.c \
   ..\*.h \
   ..\..\headers\*.h

mle.obj: \
   ..\mle.c \
   ..\*.h \
   ..\..\headers\*.h

