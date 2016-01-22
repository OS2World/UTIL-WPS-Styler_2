/**/
parse arg module, rcname, description, rcOptions
nl = "0d0a"x

Cfile = module".c"
if "" \= stream(Cfile, "c", "QUERY EXISTS") then
   call SysFileDelete Cfile
call charout Cfile, "/* "module".c */"
call charout Cfile
"@icc /C+" cfile

/* create/update the module definition file */
DEFfile = module".def"
if "" \= stream(DEFfile, "c", "QUERY EXISTS") then
   call SysFileDelete DEFfile
call charout DEFfile, "LIBRARY "module""nl
call charout DEFfile, "DESCRIPTION '"description"'"nl
call charout DEFfile

/* call the linker to create the DLL */
"ILINK "module".OBJ" DEFfile" /A:4 /O:"module".dll"

/* call the resource compiler to create the resource file */
"RC -r" rcOptions rcname module".res"

/* aggiunge le risorse alla DLL */
"RC /X2 "module".res" module".dll"

return rc

