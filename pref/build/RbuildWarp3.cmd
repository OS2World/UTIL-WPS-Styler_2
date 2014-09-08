@echo off
echo build Styler/2 Warp 3 version - no debug
set debug=
mode co80,102

rem if the flag file doesn't exist create it and touch all the sources
if not exist @debug@ if exist @warp3@ goto build

if exist @debug@ del @debug@ > nul
if not exist @warp3@ echo . > @warp3@
for %%1 in (..\*.c) do touch %%1

goto build

:tryagain
echo.
echo Prememere un tasto qualsiasi per rieseguire NMAKE
echo Premerere Ctrl-C per terminare.
pause > nul

:build
cls
nmake -f warp3.mak -nologo
if errorlevel 1 goto tryagain
copy ..\..\warp3\smartwin.exe ..\..\warp3\install\smartwin.exe
cmd /c lxlite ..\..\warp3\install\smartwin.exe
pause

