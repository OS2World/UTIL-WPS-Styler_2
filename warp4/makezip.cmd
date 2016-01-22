/**/
mkdir = directory()
zipname = 'smw1xx.zip'
zipfile = mkdir'\'zipname

say 'Creating Styler/2 install package' zipfile '...'
call directory 'install'
'@zip -r9' zipfile '*'

call directory mkdir
call directory '..\pref'
'@zip -r9' zipfile 'bitmaps'

call directory mkdir
return rc
