/**/
arg language
lp = language
"@ECHO OFF"
select
   when language = "EN" then do
      path = "EN"
      country = 001
      codepage = 850
      language = ENU
      end /* do */
   when language = "NL" then do
      path = "NL"
      country = 031
      codepage = 437
      language = NLD
      end /* do */
   when language = "FR" then do
      path = "FR"
      country = 001
      codepage = 850
      language = ENU
      end /* do */
   when language = "DE" then do
      path = "DE"
      country = 049
      codepage = 850
      language = DEU
      end /* do */
   when language = "IT" then do
      path = "IT"
      country = 039
      codepage = 850
      language = ITA
      end /* do */
   when language = "RU" then do
      path = "RU"
      country = 007
      codepage = 866
      language = RUS
      end /* do */
   when language = "ES" then do
      path = "ES"
      country = 034
      codepage = 850
      language = ESP
      end /* do */
   when language = "SV" then do
      path = "SV"
      country = 046
      codepage = 850
      language = SVE
      end /* do */
otherwise do
   call help
   exit
   end /* do */
end  /* select */

temp = path
path = directory(path)
if path = "" then do
   say "Il percorso "temp" non Š valido"
   "pause"
   exit
   end /* do */

/*call SysFileTree "..\bitmaps", "artwork", "DO";*/
/*say artwork.1*/
/*"@pause"*/
artwork = "..\bitmaps"
"SET IPFCARTWORK="artwork
call directory path

"MODE CO80,102"
"IPFBLD STYLER2.LST"
"pause"
"IPFC -s -x -D:"country" -C:"codepage" -L:"language" STYLER2.IPF"
"pause"
"IPFC -i -s -D:"country" -C:"codepage" -L:"language" STYLER2.IPF"
say "copia dei file nei percorsi di installazione"
"pause"
"copy styler2.hlp ..\..\warp4"
"copy styler2.hlp ..\..\warp4\install\"lp
if lp = "EN" then "copy styler2.hlp ..\..\warp4\install"
"copy styler2.hlp ..\..\warp3"
"copy styler2.hlp ..\..\warp3\install\"lp
if lp = "EN" then "copy styler2.hlp ..\..\warp3\install"

"cd .."
exit


/* mostra messaggio help */

help: procedure
   say "Questo script crea un file di help"
   say "E' necessario specificare il nome della lingua:"
   say "english, german, italian or russian"
   say ""
   "pause"
return
