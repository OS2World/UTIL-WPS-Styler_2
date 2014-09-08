@echo off
set debug=1
mode co80,102

for %%1 in (..\*.c) do touch %%1

goto build

:tryagain
echo.
echo Prememere un tasto qualsiasi per rieseguire NMAKE
echo Premerere Ctrl-C per terminare.
pause > nul

:build
cls
nmake -f debug.mak -nologo
if errorlevel 1 goto tryagain
copy ..\..\warp4\smartwin.dll ..\..\warp4\install\smartwin.170
copy ..\..\warp4\styler20.dll ..\..\warp4\install\styler20.170
pause

