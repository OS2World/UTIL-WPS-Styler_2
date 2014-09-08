@echo off
set debug=1
mode co80,102

rem if the flag file doesn't exist create it and touch all the sources
if exist @debug@ if not exist @warp3@ goto build
if not exist @debug@ echo . > @debug@
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
copy ..\..\warp4\smartwin.dll ..\..\warp4\install\smartwin.170
copy ..\..\warp4\styler20.dll ..\..\warp4\install\styler20.170
pause

