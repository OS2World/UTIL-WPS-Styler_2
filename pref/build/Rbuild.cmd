@echo off
set debug=
mode co80,102

rem if the flag file exists delete it and touch all the sources
if not exist @debug@ if not exist @warp3@ goto build
rem del *obj > nul

if exist @debug@ del @debug@ > nul
if exist @warp3@ del @warp3@ > nul
for %%1 in (..\*.c) do touch %%1

goto build

:tryagain
echo.
echo Prememere un tasto qualsiasi per rieseguire NMAKE
echo Premerere Ctrl-C per terminare.
pause > nul

:build
cls
nmake -nologo
if errorlevel 1 goto tryagain
copy ..\..\warp4\smartwin.exe ..\..\warp4\install\smartwin.exe
cmd /c lxlite ..\..\warp4\install\smartwin.exe
pause

