@echo off
set debug=
rem mode co80,102

rem if the flag file exists delete it and touch all the sources
if not exist @debug@ goto build
del @debug@ > nul
del *.obj > nul
for %%1 in (*.c) do touch %%1


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

cmd /c lxlite ..\warp4\install.exe
copy ..\warp4\install.exe ..\warp3
copy ..\warp4\install.exe ..\warp4\install
copy ..\warp4\install.exe ..\warp3\install

copy README ..\warp3\README.TXT
copy README ..\warp3\install\README.TXT
copy README ..\warp4\README.TXT
copy README ..\warp4\install\README.TXT

copy hsaw.ico ..\warp3\install
copy ruaw.ico ..\warp3\install
copy hsaw.ico ..\warp4\install
copy ruaw.ico ..\warp4\install

copy styler.@ ..\warp3
copy styler.@ ..\warp3\install
copy styler.@ ..\warp4
copy styler.@ ..\warp4\install

pause

