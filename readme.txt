A short description of the source code structure.

doc           program documentation source (IPF) in various languages
headers       headers shared by the various DLLs and EXEs
images        bitmap resources included in the resource DLL (STLRES13.DLL).
include       headers which were in %INCLUDE% in the original build system
              (should be moved to /headers although that might require some
              other modifications)
install       original install program (should be replaced by warpin)
maindll       main DLLs (smartwin.dll and styler20.dll) source code
nls           national language support for the resource DLL
pref          Styler/2 settings program source code.
resdll        resource dll - source files.
utildll       source code of an old utility functions DLL. This is currently
              used both by the installation and preferences program.
warp3         path for the targets built for warp 3.
warp4         path for the targets built for warp 4/eCS.

Some suggestions:

The installation program should be replaced by warpin.
A REXX script would be necessary since warpin does not support direct
modification of the OS2.INI SYS_DLLS item.

The preferences program should be re-written taking as model the last
eStylerLite source and the content of /utildll should be discarded.