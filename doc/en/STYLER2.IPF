:userdoc.
:docprof toc=12345.
:title.Styler/2 User's Guide

.* htmlsource = index.htm
:h1 res=1 x=left y=bottom width=100% height=100%.Styler&slr.2 User&csq.s Guide
.br
.br
:lines align=center.
:font facename='Helv' size=32x20.
Styler&slr.2 User&csq.s Guide
:font facename=default size=0x0.
:font facename='Helv' size=16x10.:hp2.
version 1&per.7&per.4
:font facename=default size=0x0.:ehp2.
by Alessandro Cantatore
:elines.
:p.
[5 December 2003]
:ul compact.
.br
:li.
:link reftype=hd res=2.License agreement:elink.
:li.
:link reftype=hd res=3.Introduction:elink.
:li.
:link reftype=hd res=4.What&csq.s new:elink.
:li.
:link reftype=hd res=5.Installation-uninstallation:elink.
:li.
:link reftype=hd res=6.Features and settings:elink.
:li.
:link reftype=hd res=21.Known problems:elink.
:li.
:link reftype=hd res=22.Planned features:elink.
:li.
:link reftype=hd res=23.Registration:elink.
:eul.
.br

.* htmlsource = license.htm
:h1 res=2 x=left y=bottom width=100% height=100%.License agreement
:ul.
.br
:li.
You are allowed to use this program only for the evaluation period of thirty
days&per. If you intend to use the program beyond the evaluation period, you
must register it&per. See the
:link reftype=hd res=23.registration section:elink.
for
more details&per.
:li.
You are not allowed to distribute the registration key that you receive when
purchase the program license&per.
:li.
The program may be installed on as many machines as you like, provided
that it will be used only by one person per license&per. That is, two or more
persons who will be using the program on two or more different machines
at the same time need to apply for a multiple license&per.
:li.
You are free to distribute the program provided that you include all the
files in the original archive without any modification&per.
:li.
You are not allowed to sell the program, but can charge a reasonable amount
to cover the cost of the distribution media&per.
:li.
Under no circumstances will the author be liable for any loss or damage that
may be derived from the use of the program&per.
:li.
Styler&slr.2, also known as Smart Windows in previous releases, is the
copyrighted property of Alessandro Felice Cantatore, Bitonto, Bari, Italy&per.
:eul.
.br
:p.
:hp2.The use of the program implies acceptance of the conditions stated above!:ehp2.
:p.
Alessandro Cantatore
.br
:link reftype=fn refid=29.acantatore&atsign.tin&per.it:elink.
.br
:link reftype=fn refid=30.alscant&atsign.libero&per.it:elink.
.br
:link reftype=fn refid=31.http&colon.&slr.&slr.space&per.tin&per.it&slr.scienza&slr.acantato:elink.

.* htmlsource = intro.htm
:h1 res=3 x=left y=bottom width=100% height=100%.Introduction
.br
Styler&slr.2 is an user interface enhancer for OS&slr.2 Warp 3 and 4&per. Both the
aesthetics and the behavior of various OS&slr.2 Presentation Manager windows
and controls can be easily modified to meet your needs&per.
.br
You will be able to use bitmaps and shades as titlebar backgrounds, choose
any bitmap (of appropriate size) for the window buttons and enable all dialog
windows to use your favorite font&per.
:p.
Other features include&colon.
:ul compact.
.br
:li.window rolling,
:li.titlebar hiding,
:li.overriding the default size and position of maximized windows,
:li.smooth horizontal movement of windows,
:li.snap windows to the desktop edges,
:li.moving a window by dragging its frame,
:li.pushing windows behind the other windows,
:li.selection of the entry field contents,
:li.facilitating clipboard operations via mouse and&slr.or keyboard
combinations in entry fields and command windows,
:li.autofocus,
:li.mouse pointer hiding,
:li.mouse button double-click emulation,
:li.automatically moving the mouse pointer to the focus window&per.
:eul.
.br
:p.
This shareware version is fully functional and will work, without any
limitation, for an evaluation period of 60 days or 300 reboots, whichever
comes first&per.

.* htmlsource = new.htm
:h1 res=4 x=left y=bottom width=100% height=100%.What&csq.s new
.br
:font facename='Helv' size=16x10.:hp2.
Version 1&per.7&per.4
:font facename=default size=0x0.:ehp2.
:ul compact.
.br
:li.Now it is possible to use bitmaps of any size for the frame
controls (i&per.e&per. maximize, minimize, close buttons etc&per.)&per.
.br
I provided a sample set of frame control bitmaps for XGA or better
resolution (win9x-s&per.bmp) which are 18x22 rather than 22x22 pixels
(with the exception of the system menu which is 24x22 pixels)&per.
.br
The image below shows the difference between standard
and custom sized frame controls&per.
.br
:artwork name='framectls.bmp' align=center.
:hp2.Notes&colon.:ehp2.
:ol compact.
.br
:li.this new feature doesn&csq.t work correctly with windows
based on the Odin library&per. The problem will be corrected in the
near future&per.
:li.the titlebar preview, in the Styler&slr.2 preferences program, cannot
display correctly custom size bitmaps&per.
:eol.
.br
:li.The :hp1.Roll&slr.unroll all windows :ehp1. and
:hp1.Hide&slr.unhide all windows :ehp1. functions, previously
available only via hot key, can now be executed by opening
the corresponding icons created by the Styler&slr.2 installer&per.
.br
The objects icon can be dragged onto the Launchpad&slr.WarpCenter&slr.xCenter&slr.eCenter
to roll&slr.unroll or hide&slr.show all windows with a simple mouse click&per.
:li.Usually the installer asks for confirmation before overwriting
a bitmap file if it is newer then the file being installed&per.
.br
This is useful only if you modified the original bitmaps&per. Overwriting
does not affect new bitmaps you created, with a different name, in
the application subdirectories&per.
.br
To exclude the overwrite prompt open a command window in the path
where you unzipped the distribution package and type&colon.
.br
:font facename='System VIO' size=12x5.
SET STLROVERWRITE=YES
:font facename=default size=0x0.
:hp1.<enter>:ehp1.
.br
then&colon.
.br
:font facename='System VIO' size=12x5.
INSTALL
:font facename=default size=0x0.
:hp1.<enter>:ehp1.
:li.In some particular cases, in Warp 3, the frame controls were
not displayed in the correct order&per.
:li.Frame windows based on the Odin libraries now respect the position
of the close button with the exception of the left position option
(which currently does not work in any kind of window)&per.
:li.Various other minor bugs have been corrected&per.
:eul.
.br
:font facename='Helv' size=16x10.:hp2.
Version 1&per.7&per.3
:font facename=default size=0x0.:ehp2.
:ul compact.
.br
:li.Styler&slr.2 is now distributed in two separated packages&colon. one for
installation on Warp 3 and another for installation on Warp version 4 or
greater (MCP or eCS)&per.
:li.Improved compatibility with Odin based applications (in some cases
Styler&slr.2 enhancements were not correctly applied)&per.
:li.Drop down lists enhancement&colon. press the left mouse button on the
entry field part of a drop down list, then move the mouse down without
releasing the mouse button, the listbox appears and you can
directly select an item by moving the mouse and releasing the button&per.
:li.Documentation of all the Styler&slr.2 programs now is in OS&slr.2 HELP format&per.
:eul.
.br
:font facename='Helv' size=16x10.:hp2.
Version 1&per.7&per.2
:font facename=default size=0x0.:ehp2.
:ul compact.
.br
:li.Mouse motion events were causing SYS3175 exceptions when the option
&cdq.Allow vertical resizing of rolled windows&cdq. (Window rolling page) was
disabled or when the option &cdq.Don&csq.t allow sizing&cdq. (Maximized windows page)
was enabled&per.
:li.In the preferences notebook, the &cdq.Default&cdq. checkbox of the
image selection dialog was not working properly&per.
:li.Titlebar and window controls enhancements (minimize&slr.maximize buttons)
were not applied to all the windows based on the Odin libraries&per.
:li.Redundant and obsolete code has been removed from the procedures of
the frame, titlebar and menu windows, with a small improvement
in speed and resources usage&per.
:li.The &cdq.Snap windows to the screen edges&cdq. feature has been extended to
secondary windows&colon. in a multidocument interface application
(for instance VIEW&per.EXE or Embellish) you can now snap secondary windows
to the edges of the container window&per.
:eul.
.br
:font facename='Helv' size=16x10.:hp2.
Version 1&per.7&per.1
:font facename=default size=0x0.:ehp2.
:ul compact.
.br
:li.Clipboard operations via mouse shortcuts stopped to work
in VIO windows if, while marking, the Ctrl key was released before the
mouse button&per.
:li.Some programs listed in the mouse exceptions crashed with
a SYS3175 exception as soon as the mouse moved on a program window&per.
:li.The preferences program, in some cases, wrongly wrote updated
settings to OS2&per.INI rather than to SMARTWIN&per.INI&per.
:li.In the preferences program, the dialog showing the settings of
the close-window titlebar button, was not displaying the correct
text in the dropdown list used to select the button position&per.
:li.Avoided some system hangs with applications using Odin libraries
(e&per.g&per. innotek java 1&per.4 and innotek adobe acrobat 4)&per.
:li.The preferences program failed to properly save new titlebar
settings when multiple titlebar&slr.titlebar-button images were applied at once&per.
:eul.
.br
:font facename='Helv' size=16x10.:hp2.
Version 1&per.7&per.0
:font facename=default size=0x0.:ehp2.
:ul compact.
.br
:li.Styler&slr.2 has been completely rewritten&per.
:li.Now Styler&slr.2 works correctly with Connectix Virtual PC and hopefully
with other applications based on the Odin libraries&per.
.br
Currently the :hp1.Window rolling :ehp1. and :hp1.Titlebar hiding :ehp1.
features have been disabled for windows relying on those libraries&per.
:li.Most limitations regarding bitmap formats have been removed&per.
.br
Now any kind of OS&slr.2 bitmap is valid provided that its size does not
exceed 64 KBs&per.
:li.Thanks to an undocumented OS&slr.2 API, now Styler&slr.2 shares the same bitmap
resources among all the system processes&per.
.br
This provides a small increment in speed (when starting a new process)
and a large decrease in resource usage (when a lot of processes are
running)&per.
:li.Titlebar gradients use much less memory and are displayed
much faster as they are converted to shared bitmaps and then stretched
as needed&per.
:li.The titlebar sample in the preferences notebook correctly shows
centered text when the :hp1.Center :ehp1. option is selected&per.
:li.When a mouse button is pressed on the window border now the window
correctly receive the focus&per.
:li.Some new bitmaps for the titlebars and the window controls have
been added&per.
:eul.
.br
:p.

.* htmlsource = install.htm
:h1 res=5 x=left y=bottom width=100% height=100%.Installation - uninstallation
.br
The program can be installed over a previous installation of Styler&slr.2 (Smart
Windows) v&per.0&per.81 or higher, without any need to disable the prior version
or to uninstall it&per.
.br
If you installed a version older than v&per.0&per.81 you must delete the
:font facename='System VIO' size=12x5.
SMARTWIN&per.INI
:font facename=default size=0x0.
file from the installation path before installing this version&per.
:p.
The archive smw174&per.zip (or smw174w3 - if you are installing on Warp 3)
must be unzipped in a temporary directory,
taking care to preserve the archive&csq.s path structure&per.
.br
If you are using the Info-ZIP group&csq.s free UnZip extraction utility,
the archive will unzip correctly by default&per. Simply open an OS&slr.2
command window, move to the temporary directory and type&colon.
:xmp.:font facename='System VIO' size=12x5.
UNZIP SMW174&per.ZIP
:exmp.:font facename=default size=0x0.
If you are using a different extraction utility, read its documentation for
the appropriate unzip parameters&per.
:p.
After unzipping the archive, you must run INSTALL&per.EXE and click on the
:hp1.Forward :ehp1.
button to select the action you want to perform&colon.
:ol compact.
.br
:li.Install
:li.Recreate desktop objects
:li.Uninstall
:eol.
.br
:p.
Once you select the proper action, click on the :hp1.Forward :ehp1.
button to install, to recreate Styler&slr.2 desktop objects, or to uninstall&per.
:p.
The other buttons will allow you to&per.&per.&per.
:ul compact.
.br
:li.
go back to the previous page in order to change some previous selection
(for instance, the action to be performed or the installation path)&semi.
:li.
continue with the procedure&semi.
:li.
terminate the program immediately&semi.
:li.
read this documentation&per.
:eul.
.br
:hp2.Note&colon.:ehp2.
If you want to install over a previous version, overwriting all the
files, without being prompted for confirmation on overwriting files
newer than the file being installed (for instance a bitmap file you
modified without assigning it a different name) you have to run the
installer from the command line&colon.
:ol compact.
.br
:li.open a command line window in the path where you unzipped the
distribution package&per.
:li.type&colon.
:font facename='System VIO' size=12x5.
SET STLROVERWRITE=YES
:font facename=default size=0x0.
:hp1.<enter>:ehp1.
:li.type&colon.
:font facename='System VIO' size=12x5.
INSTALL
:font facename=default size=0x0.
:hp1.<enter>:ehp1.
:eol.
.br
:p.
:hp2.Installation details:ehp2.
:p.
Here are the step-by-step details of what happens when you click on the
:hp1.Forward :ehp1. button&per. Please refer to this section if you cannot
properly install the program on your system&per.
:ol.
.br
:li.
The installation program first looks for the flag file
:font facename='System VIO' size=12x5.
SMARTWIN&per.&atsign.
:font facename=default size=0x0.
to enable the :hp1.Install  :ehp1.radio button, then looks in the
:font facename='System VIO' size=12x5.
&bsl.OS2&bsl.DLL
:font facename=default size=0x0.
directory and the
:font facename='System VIO' size=12x5.
OS2&per.INI
:font facename=default size=0x0.
file for any item from a previous installation&per.
.br
If any trace of a previous installation is found (or if you run the
:hp1.Installer&slr.Uninstaller :ehp1.
object later) the :hp1.Recreate desktop objects :ehp1. and
:hp1.Uninstall :ehp1.
radio buttons will be enabled as well&per.
:li.
If you select to install, the next page will show you the :hp1.License agreement:ehp1.
&per. Please read it carefully before continuing with the installation&per.
:li.
On the next page, you can choose the installation path&per. The default drive
is your boot drive and the default path is
:font facename='System VIO' size=12x5.
&bsl.SMARTWIN
:font facename=default size=0x0.
unless you
are upgrading a previous installation, in which case the previous installation
path will be the default&per.
:p.
Note that if you ever choose to uninstall Styler&slr.2, the program files
will be deleted and then a message box will ask your permission to
remove the entire installation path with all of its contents&per.
Therefore, it is advisable to avoid installing the program files in a
directory which will contain other files or folders not related to
Styler&slr.2&per.
:li.
Click on :hp1.Forward :ehp1. to create the selected path&per. If the path doesn&csq.t
exist, the program will create all the necessary directories&per. For instance,
if you selected to install the program files in
:font facename='System VIO' size=12x5.
C&colon.&bsl.APPS&bsl.TEST&bsl.SMW
:font facename=default size=0x0.
and none of the directories in the path exist, then the installation program
will create all of them&per.
.br
If, for any reason, it is not possible to create the selected path,
you should click on :hp1.Back:ehp1.  and select a different path&per.
:li.
The final panel allows you to modify your previous
selections&per.
:p.
Click on :hp1.Forward :ehp1. to start installing the files&per.
:li.
A progress dialog will allow you to check the state of the installation&per.
:p.
The installation procedure consists of the following steps&colon.
:ul compact.
.br
:li.
If the :hp1.Preferences:ehp1.  program is open, it will be closed&per.
:li.
STYLER20 and SMARTWIN are added to the OS2&per.INI list of DLLs to be loaded
every time a new process is started (:hp1.SYS_DLLS :ehp1. - :hp1.LoadPerProcess:ehp1.)&per.
:li.
The installation path is stored in OS2&per.INI&per.
:li.
If the program MPTS&per.EXE is present in one of the directories mentioned
in the path statement of the CONFIG&per.SYS file, it is added to the
:link reftype=hd res=8.exceptions:elink.
in order to let it work properly with Styler&slr.2&per.
:li.
All unneeded files from a previous installation are deleted&per.
:li.
The following files needed for the current installation are copied to the
selected path&colon.
:ul compact.
.br
:li.
:font facename='System VIO' size=12x5.
AFCUTL05&per.DLL
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
INSTALL&per.EXE
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
LICENSE&per.TXT
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
REGISTER&per.EXE
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
REGISTER&per.TXT
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
SMARTWIN&per.EXE
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
STLRES13&per.DLL
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
STYLER2&per.HLP
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
README&per.HTM
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
RUAW&per.ICO
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
HSAW&per.ICO
:font facename=default size=0x0.
:eul.
.br
:li.
The file SMARTWIN&per.170 is copied as SMARTWIN&per.DLL in &bsl.OS2&bsl.DLL&per.
:li.
The file STYLER20&per.170 is copied as STYLER20&per.DLL in &bsl.OS2&bsl.DLL&per.
:li.
All the bitmap files used for the titlebar and the window controls are
copied to the installation directory&per. If you have modified a bitmap, the
program will ask for your confirmation to overwrite it&per.
:li.
A Styler&slr.2 folder object is created on the desktop,
with the following items inside&colon.
:ul compact.
.br
:li.
User&csq.s Guide -- the program documentation in HLP format&semi.
:li.
Read Me -- an HTML file with some notes on the program&semi.
:li.
Preferences -- the preferences program&semi.
:li.
License Agreement -- a text file&semi.
:li.
Registration Notes -- short notes on registering Styler&slr.2&semi.
:li.
Registration -- the registration program&semi.
:li.
Installer&slr.Uninstaller -- the installation&slr.uninstallation program&per.
:li.
Hide&slr.show all windows -- a program object to hide&slr.show all the desktop windows&per.
:li.
Roll&slr.unroll all windows -- a program object to roll&slr.unroll all the desktop windows&per.
:eul.
.br
:li.
A shadow of the Preferences Notebook is created in the OS&slr.2 System Setup folder&per.
:eul.
.br
:li.
At the end of the installation procedure, a message will indicate if the installation
has been successful, or if any critical or non-critical error has occurred&per.
:li.
On the last page, the log file will show the details of the installation&per.
:eol.
.br
:p.
:hp2.Uninstallation details:ehp2.
:p.
The uninstallation procedure reverses all of the operations described above&per.
Besides that, it will delete any new bitmap files you added to the
:font facename='System VIO' size=12x5.
&bsl.BITMAPS
:font facename=default size=0x0.
subdirectories&per. In any case, a message box will ask you to confirm the deletion of
the installation path and other files which still can be there (such as
:font facename='System VIO' size=12x5.
SMARTWIN&per.INI
:font facename=default size=0x0.
created by the application, plus any other files that you yourself put there)&per.
:p.
At the end of the uninstallation procedure, the
:font facename='System VIO' size=12x5.
SMWSETUP&per.LOG
:font facename=default size=0x0.
file will be written in the root of the boot drive&per.

.* htmlsource = features.htm
:h1 res=6 x=left y=bottom width=100% height=100%.Features and settings
.br
The program consists of a DLL loaded by the system every time a
new process is started, and a Preferences Notebook which runs Styler&slr.2
according to your favorite customized settings&per.
.br
The Notebook pages share a common area with a five-button tool bar and
a status bar providing contextual help&per. If you cannot read a help
message because the text doesn&csq.t fit inside the status bar, you can
enlarge the Preferences window-- it is resizeable!
:p.
The toolbar buttons have the following functions&colon.
:p.
:sl compact.
:li.:artwork name='styler03.bmp' align=left runin.
The first button :hp2.enables&slr.disables:ehp2. all Styler&slr.2 features&per.
:p.
:artwork name='styler04.bmp' align=left runin.
This button is required to :hp2.apply:ehp2. the settings of the current page&per.
Most pages do not need it since the settings become effective as soon as they
are changed&per. When required, the button is enabled as soon as a setting
is modified&per.
:p.
:artwork name='styler05.bmp' align=left runin.
The third button is an :hp2.undo:ehp2. button&per. It will set the settings of
the current page to the values they held when the Preferences Window
was opened&per.
:p.
:artwork name='styler06.bmp' align=left runin.
The fourth button is the :hp2.default:ehp2. button and it resets the current
page preferences to the default values&per.
:p.
:artwork name='styler07.bmp' align=left runin.
The fifth button is the help button&per. By clicking on it, you will get the
Styler&slr.2 User&csq.s Guide page corresponding to the current settings page&per.:esl.
There are still some features not yet implemented though the corresponding
buttons&slr.checkboxes appear in the Preferences application&per.
:p.
The Styler&slr.2 Preferences Notebook consists of the following pages&colon.
:ul.
.br
:li.
:hp2.Styler&slr.2:ehp2.
:ul compact.
.br
:li.
Product information (just the logo and the version number)
:li.
:link reftype=hd res=7.General:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:eul.
.br
:li.
:hp2.Dialogs:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=9.General options:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:eul.
.br
:li.
:hp2.Titlebar:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=10.Active window:elink.
:li.
:link reftype=hd res=10.Inactive window:elink.
:li.
:link reftype=hd res=12.Titlebar hiding:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:eul.
.br
:li.
:hp2.Window rolling:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=13.General options:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:eul.
.br
:li.
:hp2.Maximized windows:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=14.General options:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:eul.
.br
:li.
:hp2.Sizing&slr.moving:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=15.General options:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:eul.
.br
:li.
:hp2.Text controls:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=16.General options:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:eul.
.br
:li.
:hp2.VIO windows:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=17.General options:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:eul.
.br
:li.
:hp2.Mouse:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=18.General options:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:li.
:link reftype=hd res=19.Mapping:elink.
:eul.
.br
:li.
:hp2.Keyboard:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=19.Mapping:elink.
:li.
:link reftype=hd res=20.Windows keys:elink.
:eul.
.br
:eul.
.br
:hp2.Other dialogs:ehp2. used by the application are&colon.
:ul compact.
.br
:li.
The
:link reftype=hd res=11.bitmap selection dialog:elink.
:li.
The
:link reftype=hd res=19.mouse&slr.keyboard functions dialog:elink.
:eul.
.br

.* htmlsource = pggener.htm
:h2 res=7 x=left y=bottom width=100% height=100%.Styler&slr.2 - General page
.br
The settings in this page affect the behaviour of the Preferences
Notebook itself&colon.
:dl break=all.
.br
:dt.
:hp1.Apply on exit:ehp1.
:dd.
All new settings will be automatically
:link reftype=hd res=6.applied:elink.
when the program window is closed&per.
:dt.
:hp1.Save on exit:ehp1.
:dd.
If not checked, the current settings are not saved in SMARTWIN&per.INI, so you will
get back the previous settings when you reboot&per.
:dt.
:hp1.Remember last viewed page:ehp1.
:dd.
This option works when the program is started and brings up the page
which was topmost when the program was closed&per.
:dt.
:hp1.Apply all:ehp1.
:dd.
The first button simulates clicking on the
:hp1.
:link reftype=hd res=6.Apply:elink.
 :ehp1. toolbar icon
on all pages (which require that) to make the current preferences
the active ones&per.
:dt.
:hp1.Undo all:ehp1.
:dd.
The middle button simulates clicking on the
:hp1.
:link reftype=hd res=6.Undo:elink.
 :ehp1. toolbar icon on all pages
where some settings have been modified&per.
:dt.
:hp1.Default all:ehp1.
:dd.
The third button simulates clicking on the
:hp1.
:link reftype=hd res=6.Default :elink.
:ehp1. toolbar icon
in all pages&per.
:edl.
.br
:p.

.* htmlsource = pgexcept.htm
:h2 res=8 x=left y=bottom width=100% height=100%.Styler&slr.2 - Exceptions page
.br
Although Styler&slr.2 integrates seamlessly with the system, generally
with no problem for all the other programs, some applications still cannot properly
cooperate with it and won&csq.t work properly when some Styler&slr.2 features have been set&per.
.br
In order to avoid these kinds of problems, each Preferences page with a given
category of features is followed by an :hp1.Exceptions :ehp1. page allowing
you to enter the pathnames of the programs for which that particular group
of options causes conflicts&per.
.br
In some cases, you would find it more comfortable just to completely disable
Styler&slr.2 enhancements for a given program&per. For this purpose, you should
use the Exception page inserted under the :hp1.Styler&slr.2 :ehp1. main tab&per.
.br
To add a program to the exception list, either type its full pathname (i&per.e&per. its
name including the program path) in the entry field and click on the :hp1.Add :ehp1.
button or get it from the file dialog by clicking on the :hp1.Find :ehp1.
button&per. The :hp1.Remove :ehp1. button will delete a file name from the
list&per.
:p.
:hp2.Note&colon.:ehp2.
:p.
Currently it is not possible to specify the names of programs
whose windows are executed as a PMSHELL process&per. These include all text mode
programs (DOS and OS&slr.2), all Win-OS&slr.2 programs, all folders and all WPS
applications (like the System clock, the color and font palettes, etc&per.)&per.
:p.
In the next version, all these scattered Exceptions pages will be grouped
in a unique dialog, enabling you to add program names by drag &amp. drop
or directly by the system menu of a running application, and also to detect
the various windows belonging to the PMSHELL process&per.

.* htmlsource = pgdialog.htm
:h2 res=9 x=left y=bottom width=100% height=100%.Dialogs - General options page
.br
Usually, when no other kind of font has been explicitly specified
by the programmer, all dialog windows will use the ugly :hp1.System Proportional :ehp1.
font&per.
:p.
Styler&slr.2 allows you to override this system behaviour and force all dialog
windows to use your favourite font&per. Just check :hp1.Use Styler&slr.2 font :ehp1.
to enable this function&per.
.br
This will work in all dialog windows which use the system default font&per.
If you want to force all windows to use the font you selected, then check
:hp1.Override
Presentation Parameters :ehp1.&per.
:p.
The :hp1.Font :ehp1. button brings the :hp1.Dialog font :ehp1.
dialog to the top&per.
:p.
:hp2.Note&colon.:ehp2.
:p.
dialog controls inherit the font attribute (presentation
parameter) from the dialog unless they have been explicitly set to use
a different font&per. In this latter case, the :hp1.Use Styler&slr.2 font :ehp1.
feature won&csq.t work&per.

.* htmlsource = pgtbar.htm
:h2 res=10 x=left y=bottom width=100% height=100%.Titlebar - Active &slr. Inactive window pages
.br
Through this page it is possible to change the look of titlebars
completely&per.
:p.
You can use any bitmap for the titlebar background and the window controls
provided that they are proper OS&slr.2 bitmaps and smaller than 64 KBs&per.
:p.
The settings in this page must be applied through the :hp1.
:link reftype=hd res=6.Apply:elink.
 :ehp1.
button to become active&per.
:dl break=all.
.br
:dt.
:hp1.Enable titlebar enhancements:ehp1.
:dd.
It enables&slr.disables all the controls in the page and the corresponding settings&per.
When it is checked, a titlebar sample will be displayed&per.
:dt.
:hp1.Context menu:ehp1.
:dd.
A context menu event (right or left button click according to what you
set in the system mouse preferences) on the titlebar will make the system
menu pop up next to the mouse pointer&per.
:dt.
:hp1.Titlebar sample:ehp1.
:dd.
It is interactive! By clicking on the titlebar buttons you can modify the
default button bitmaps or the close button position&per. By clicking on the
titlebar you&csq.ll be able to select a different background color, gradient
direction or bitmap according to the selected background mode&per.
:edl.
.br
:hp2.Background group&colon.:ehp2.
:dl break=all.
.br
:dt.
:hp1.Solid color:ehp1.
:dd.
This replicates the default titlebar style&per. The Background button allows you to
select the background color&per. The color selection button can be used like
the titlebar sample&per.
:dt.
:hp1.Gradient:ehp1.
:dd.
It allows the use of a gradient as a titlebar background&per. It will display
four buttons to select the two different colors, to swap them or change the
gradient direction&per. The gradient direction button can be used like the titlebar
sample&per.
:p.
The direction button will allow you to cycle through the available gradient
styles&colon. horizontal, vertical, horizontal-center and vertical-center&per.
:dt.
:hp1.Bitmap:ehp1.
:dd.
Use a bitmap as titlebar background&per. When selected, a bitmap
selection button will be displayed&per. The bitmap selection button can be
used like the titlebar sample&per.
.br
The bitmap is tiled by default&per. If the :hp1.Stretch :ehp1. checkbox
is checked it will be stretched to fit the titlebar size&per.
:dt.
:hp1.Override Presentation Parameters:ehp1.
:dd.
When unchecked, titlebars which do not use default colors for the active
or inactive state won&csq.t use the Styler&slr.2 titlebar background settings&per.
:edl.
.br
:dl break=all.
.br
:dt.
:hp2.3D border group&colon.:ehp2.
:dd.
.br
Use this setting to surround the titlebar with a 3D border&per. The buttons
allow you to select and swap the colors&per.
:edl.
.br
:hp2.Text style group&colon.:ehp2.
:dl break=all.
.br
:dt.
:hp1.Center:ehp1.
:dd.
When checked, the text is centered in the titlebar rather than left-aligned&per.
:dt.
:hp1.3D text:ehp1.
:dd.
When checked, this gives a 3D effect to the text&per. By selecting the colors, you
can get a raised or depressed effect&per.
:dt.
:hp1.Font:ehp1.
:dd.
The :hp1.Font:ehp1.  button allows you to set the titlebar font&per. The same effect
can be attained by dragging a font from the font palette and dropping it
on the titlebar sample&per.
:edl.
.br
:hp2.Note&colon.:ehp2.
:ol.
.br
:li.
On titlebar buttons, the same bitmap is used both for the button in ordinary
state and for the button in depressed state&per. The bitmap is shifted one
pixel left and down for user feedback when the button is depressed&per. In
a future version I&csq.ll try to use icons, i&per.e&per. transparent bitmaps, for extending
the titlebar background along the bottom of the buttons, and to use different images for
the different states of the buttons&per.
:li.
When moved to the left side, the close button replicates the system menu
icon if present&per. The problem will be solved in a future version&per.
:li.
The next version will allow more control of the titlebar settings&colon.
:ul compact.
.br
:li.
3-color gradients, with some dithering and choice of any direction (even
oblique)
:li.
pattern overlay for titlebar background
:li.
text formatting (left, center, right)
:li.
opaque text background etc&per.
:eul.
.br
:eol.
.br

.* htmlsource = pgbmpdlg.htm
:h2 res=11 x=left y=bottom width=100% height=100%.The bitmap selection dialog
.br
This dialog will pop up when you click on the titlebar sample buttons,
on the titlebar background bitmap button or on the buttons in the :hp1.Titlebar
hiding :ehp1. and :hp1.Window rolling :ehp1. pages&per.
.br
This dialog is resizeable and its last size and position are saved in
SMARTWIN&per.INI&per.
:p.
The dialogs used to select the bitmaps for the close button, window
roll&slr.unroll and titlebar hide&slr.unhide have the :hp1.Disable :ehp1. check
box which allows you to remove the button from the titlebar controls&per.
.br
Besides that, the :hp1.Close button bitmap :ehp1. dialog has a combo box
for selecting the button position&per.
:p.
The :hp1.Edit :ehp1. button opens the Icon editor with the bitmap currently
selected&per.
:p.
The :hp1.Import&per.&per.&per. :ehp1. button opens a file dialog for copying the
selected bitmap file to the appropriate path and then refreshes the bitmap list box
contents&per.
:p.
The :hp1.OK :ehp1. and :hp1.Cancel :ehp1. buttons have the usual functions,
while the Help  button will open the appropriate page of
the
:hp1.Styler&slr.2 User&csq.s Guide :ehp1.&per.
:p.
:hp2.Note&colon.:ehp2.
:p.
Currently it is necessary to dismiss the dialog in order to update
the titlebar sample&per. In Styler&slr.2 version 2, the bitmap sample will be
updated as soon as a new bitmap is selected&per.

.* htmlsource = pghtbar.htm
:h2 res=12 x=left y=bottom width=100% height=100%.Titlebar - Titlebar hiding page
.br
The options in this page allow you to hide the titlebar and controls
(optionally including the actionbar menu) of any window&per. The titlebar pops
up again as soon as the mouse pointer moves on the upper window border
and is hidden when the mouse pointer is moved downwards and away from the
titlebar area&per.
:dl break=all.
.br
:dt.
:hp1.Enable titlebar hiding:ehp1.
:dd.
Enable&slr.disable all the titlebar hiding options&per.
:dt.
:hp1.Hide by&colon.:ehp1.
:dl compact break=all.
.br
:dt.
:hp1.System menu:ehp1.
:dd.
Add a &cdq.Titlebar hiding&cdq. item to the window system menu&per.
:dt.
:hp1.Mouse:ehp1.
:dd.
Select which mouse event, performed on the titlebar, toggles titlebar hiding
on&slr.off&per.
:dt.
:hp1.Keyboard:ehp1.
:dd.
Set a keyboard accelerator for toggling titlebar hiding on&slr.off&per.
:dt.
:hp1.Titlebar Hide&slr.Unhide button:ehp1.
:dd.
Use a titlebar button for toggling titlebar hiding on&slr.off&per. Two
different bitmaps are used for hiding&slr.unhiding&per.
:edl.
.br
:dt.
:hp1.Reduce folders height:ehp1.
:dd.
When hiding the titlebar of folders, the total window height is reduced
by the titlebar height&per.
:dt.
:hp1.Exclude dialog windows:ehp1.
:dd.
Disable titlebar hiding for dialog windows&per. Dialog windows without a sizing
border are treated as OS&slr.2-DOS command windows&colon. the titlebar is removed
by reducing the size of the window&per.
:dt.
:hp1.Include menu:ehp1.
:dd.
Hide the actionbar menu alongside the titlebar and window controls&per.
:dt.
:hp1.Pop up delay:ehp1.
:dd.
Sets how long (hundredths of a second) the mouse must stay on the upper window
border before the titlebar is displayed&per.
:dt.
:hp1.Hide delay:ehp1.
:dd.
Sets how long the mouse pointer must stay away from the titlebar area before
the titlebar is hidden again&per.
:edl.
.br
:p.
:hp2.Note&colon.:ehp2.
:p.
Titlebars in hidden state are hidden again only when the mouse pointer
moves downwards inside the window!
.br
There is no reason to hide the titlebar of rolled windows so you get
just a system warning sound if you try to do so&per.

.* htmlsource = pgwroll.htm
:h2 res=13 x=left y=bottom width=100% height=100%.Window rolling - General options page
:dl break=all.
.br
:dt.
:hp1.Enable window rolling:ehp1.
:dd.
Enable&slr.disable all the window rolling options&per.
:dt.
:hp1.Roll by&colon.:ehp1.
:dl compact break=all.
.br
:dt.
:hp1.System menu:ehp1.
:dd.
Add a &cdq.Roll&slr.Unroll&cdq. item to the window system menu&per.
:dt.
:hp1.Mouse:ehp1.
:dd.
Select which mouse event, performed on the titlebar, toggles window rolling
on&slr.off&per.
:dt.
:hp1.Keyboard:ehp1.
:dd.
Set a keyboard accelerator for toggling
window rolling on&slr.off&per.
:dt.
:hp1.Titlebar Roll&slr.Unroll button:ehp1.
:dd.
Use a titlebar button for toggling window rolling on&slr.off&per. Two different
bitmaps are used for rolling&slr.unrolling&per.
:edl.
.br
:dt.
:hp1.Auto-unroll when getting focus:ehp1.
:dd.
Rolled windows are automatically unrolled when getting focus&per.
:dt.
:hp1.Auto-roll when losing focus:ehp1.
:dd.
Windows (if their rolled state has been previously set) are rolled up as they
lose focus&per.
:dt.
:hp1.Allow vertical resizing of rolled windows:ehp1.
:dd.
Disables window resizing for windows in rolled state&per.
:dt.
:hp1.Auto-unroll windows vertically resized:ehp1.
:dd.
Rolled windows are automatically unrolled if resized along the y-axis&per.
:dt.
:hp1.Roll to the bottom:ehp1.
:dd.
The window will roll downward, i&per.e&per., the left bottom corner of the window
won&csq.t move&per.
:dt.
:hp1.Auto-unroll when closing window:ehp1.
:dd.
Rolled windows are unrolled when they are closed or when the system is
closed through the default shutdown dialog&per.
:edl.
.br
:p.
:hp2.Note&colon.:ehp2.
:p.
The :hp1.Auto-roll when losing focus :ehp1. feature is a counterpart of
:hp1.Auto unroll when getting focus :ehp1.&per. When both features are enabled, the
first roll command (through the titlebar, system menu or Titlebar Roll
button) sets the window in an :hp2.:hp1.auto-roll state:ehp1.:ehp2.&colon. windows in
this state are rolled&slr.unrolled as they get&slr.lose focus&per.
:p.
Until the roll state is active, the titlebar button will display the Unroll
bitmap even if the window is currently unrolled (because it is the focus
window)&per.
:p.
If a window in auto-roll state has the focus, it is automatically
unrolled&per. At this point, if you want to roll it again while it is still
the focus window (this is likely to happen only when you are dealing with
maximized windows), two consecutive toggle-roll-state commands are required&colon.
the first will reset the auto-roll state, and the second command will
actually roll the window&per.
:p.
In order to avoid such repetition, you can keep the
Shift key pressed while issuing the roll command&per.

.* htmlsource = pgmaxwin.htm
:h2 res=14 x=left y=bottom width=100% height=100%.Maximized windows - General options page
.br
These settings allow you to change the behaviour of maximized windows&per.
:dl break=all.
.br
:dt.
:hp1.Override default size and position:ehp1.
:dd.
This option allows you to set the position and size of windows when they are
maximized&per. It doesn&csq.t work for OS&slr.2 and DOS command windows or other windows
whose maximized dimension is already smaller than the default size&per.
.br
The window position can be a negative number&per.
:dt.
:hp1.Don&csq.t allow moving:ehp1.
:dd.
This option prevents maximized windows from being moved from their default position&per.
:dt.
:hp1.Don&csq.t allow sizing:ehp1.
:dd.
This options prevents maximized windows from being resized&per.
:edl.
.br

.* htmlsource = pgmotion.htm
:h2 res=15 x=left y=bottom width=100% height=100%.Window moving - General options page
.br
These options control how windows are moved&per.
:dl break=all.
.br
:dt.
:hp1.Full window drag and size:ehp1.
:dd.
Repaint the window synchronously as it is
resized or moved on the screen&per.
:dt.
:hp1.Smooth horizontal movements:ehp1.
:dd.
Move the windows horizontally by 1-pixel steps instead
of the default 8-pixel steps&per.
.br
This feature doesn&csq.t work with OS&slr.2 and DOS command windows&per.
:dt.
:hp1.Snap windows to the screen edges :ehp1.
:dd.
Align the window along the screen edges&per.
.br
Clicking on the upper left corner in the titlebar will immediately move
the window to the upper left corner of the screen&per. Clicking
on the titlebar center will immediately center the window in the screen,
and so on&per.
.br
The :hp1.Snap by&colon. :ehp1. button sets
your favourite mouse &amp. keyboard combination to perform the operation&per.
:dt.
:hp1.Frame drag:ehp1.
:dd.
Drag windows by placing the mouse pointer on the window borders and
dragging them while holding down the mouse button&per. Optionally, set the
:hp1.Drag by&colon. :ehp1. button to use the mouse button and keys that suit
your needs&per.
:dt.
:hp1.Push behind all other windows:ehp1.
:dd.
Move windows along the z-axis, i&per.e&per. move the topmost
windows behind all other windows&per.
.br
The :hp1.Push one step behind :ehp1. sub-option allows you to push the window step
by step behind the other windows&per.
.br
The :hp1.Push by&colon. :ehp1. button allows you to select the mouse event
plus any optional key in order to push the window behind the other windows&per.
:edl.
.br
:p.
:hp2.Note&colon.:ehp2.
:p.
There are still a few minor problems related to the
:hp1.Full window drag &amp. size :ehp1. and the
:hp1.Frame drag :ehp1. features&colon.
:ul compact.
.br
:li.When moving OS&slr.2 and DOS command windows via the keyboard,
the mouse pointer doesn&csq.t follow the window movements&per.
:li.When the middle mouse button is selected for performing a
:hp1.Frame drag :ehp1. yet :hp1.Full window drag &amp. size :ehp1.
is not enabled, it will be necessary to click with the left or right mouse
button to terminate the drag operation&per.
:eul.
.br
These problems are related to system limitations and since they are not
likely to cause any real trouble, no fix is planned&per.

.* htmlsource = pgtext.htm
:h2 res=16 x=left y=bottom width=100% height=100%.Text controls - General options page
.br
These options enhance some text controls&colon.
:dl break=all.
.br
:dt.
:hp1.Entry field automatic text selection:ehp1.
:dd.
This option allows you to select the entire contents of any entry field&per.
.br
The :hp1.Select by&colon. :ehp1. button sets your favourite
mouse and keyboard combination to perform the operation&per.
:dt.
:hp1.Mouse copy&slr.cut&slr.paste:ehp1.
:dd.
This option enables the mouse to perform clipboard operations in various text controls&per.
.br
Only copy operations are allowed in non-editable controls such as :hp1.static
text:ehp1. , :hp1.buttons :ehp1.and :hp1.list boxes :ehp1.&per.
:edl.
.br
:p.
:hp2.Note&colon.:ehp2.
:ul compact.
.br
:li.
Only the selected items of a list box are copied to the clipboard&per.
:li.
The :hp1.Paste (insert) :ehp1. feature is redundant and will be removed
in the next version&per.
:li.
In order to avoid any conflict with events used by the system, carefully
select the combination of mouse buttons and keys that will perform a
clipboard operation&per.
.br
For instance, if the extensions are to be applied to list boxes, then the
combination of Ctrl + Mouse Button 1 for copying text to the clipboard
will conflict with item selection in :hp1.extended selection  :ehp1.list
boxes&per.
:eul.
.br

.* htmlsource = pgviowin.htm
:h2 res=17 x=left y=bottom width=100% height=100%.VIO windows - General options page
.br
These options allow you to enhance all DOS and OS&slr.2 command windows&per.
:dl break=all.
.br
:dt.
:hp1.Eliminate horizontal scrollbars:ehp1.
:dd.
When this feature is active, all command windows resized by a MODE command
are automatically widened to their maximum size and the horizontal scrollbar
is removed&per.
:dt.
:hp1.Make insert mode the default:ehp1.
:dd.
The command window&csq.s keyboard input is always in :hp1.overwrite :ehp1.
mode unless you press the :hp1.Insert :ehp1. key, and input normally will
revert to :hp1.overwrite :ehp1. mode as soon as you press :hp1.Enter :ehp1.&per.
.br
This feature allows you to work always in :hp1.insert :ehp1. mode, even
after you have pressed :hp1.Enter :ehp1.&per.
.br
To use this feature once the option has been activated, you must press
:hp1.Alt
+ Ins :ehp1. to toggle :hp1.insert :ehp1. mode On&slr.Off&per.
:dt.
:hp1.Enable mouse copy&slr.paste:ehp1.
:dd.
This option lets you use the mouse to copy and paste text in command
windows&per.
.br
In Warp 4 you can disable the system :hp1.VIO Mouse Actions :ehp1.
feature and instead use your favourite combinations of mouse and keyboard
to copy and paste text from the clipboard&per.
:dl break=all.
.br
:dt.
:hp1.Copy by&colon.:ehp1.
:dd.
Text marking starts as soon as the selected mouse button is pressed down,
and the marked text is copied to the clipboard when the mouse button is released&per.
.br
You can choose the mouse button and the optional control keys which
will be pressed to perform the clipboard copy&per.
:dt.
:hp1.Copy all by&colon.:ehp1.
:dd.
The selected mouse button click, plus optional control keys, allow you
to copy the entire visible contents of the command window to the clipboard&per.
:dt.
:hp1.Paste by&colon.:ehp1.
:dd.
Pressing a mouse button plus optional control keys will paste the text at
the cursor position&per.
:edl.
.br
.br
:hp2.Note&colon.:ehp2.
.br
The system :hp1.VIO Mouse Actions :ehp1. feature may be enabled or
disabled from the :hp1.User Interface :ehp1. page of the :hp1.System  :ehp1. object
in the :hp1.System Setup :ehp1. folder&per.
.br
The buttons grouped below this option allow you to select the mouse event
and control keys which perform the clipboard operations&per.
:dt.
:hp1.Enable keyboard copy&slr.paste:ehp1.
:dd.
Use the keyboard to copy and paste text in command windows, as you are
accustomed to doing in ordinary text windows&per.
:dl break=all.
.br
:dt.
:hp1.Copy by&colon.:ehp1.
:dd.
The selected combination of keys will switch the command window to marking
mode&per. You have to use the arrow keys to move the block cursor to the point
from which you want to start text selection&per.
.br
Then press the :hp1.Shift :ehp1. key and hold it down while still using the arrow
keys to define the selection area&per. As soon as you have selected all the
text, release the :hp1.Shift :ehp1. key to copy the selected text to the clipboard&per.
:dt.
:hp1.Copy all by&colon.:ehp1.
:dd.
The selected combination of keys will copy the entire visible contents of
the command window to the clipboard&per.
:dt.
:hp1.Paste by&colon.:ehp1.
:dd.
The selected combination of keys will paste the clipboard content, if it
is valid text data, at the current cursor position inside the command window&per.
:edl.
.br
:edl.
.br

.* htmlsource = pgmouse.htm
:h2 res=18 x=left y=bottom width=100% height=100%.Mouse - General options page
.br
The options of this page are related to the mouse events&per.
:dl break=all.
.br
:dt.
:hp1.Automatic focus:ehp1.
:dd.
The window (desktop child) under the mouse pointer is automatically made
active&per. The :hp1.Delay :ehp1. spin box allows you to set how long the mouse
must stay on a window before it becomes the active window&per. This option
works for seamless Win-OS2 windows as well&per.
:dl break=all.
.br
:dt.
:hp1.Ignore desktop:ehp1.
:dd.
The desktop window is not made active when the mouse pointer moves on it&per.
:dt.
:hp1.Don&csq.t bring to top:ehp1.
:dd.
The activated window is not made the topmost window&per. This feature is disabled
when moving to or from a seamless Win-OS2 window&per.
:dt.
:hp1.Ignore new windows:ehp1.
:dd.
The autofocus feature is suspended when a new window is opened, and is re-enabled
only after the mouse pointer has been moved to the new window&per.
:edl.
.br
:dt.
:hp1.Pointer hiding:ehp1.
:dd.
The pointer is hidden when it stops moving&per. The :hp1.Delay:ehp1. spin box allows
you to set how long it must stay still before it is hidden&per.
:dl break=all.
.br
:dt.
:hp1.Exclude buttons:ehp1.
:dd.
Disables pointer hiding when it is over a button&per.
:dt.
:hp1.Exclude menu:ehp1.
:dd.
Disables pointer hiding when it is over a menu&per.
:edl.
.br
:dt.
:hp1.Double click emulation:ehp1.
:dd.
Works with most PM windows and applications but still works randomly with
seamless Win-OS2 windows&per.
.br
The :hp1.Include titlebar :ehp1. sub-option extends the emulation
on titlebars&per.
.br
By default the double click emulation doesn&csq.t work on titlebars since
middle mouse button clicks are used for titlebar hiding and for window rolling&per.
By activating this sub-option, a middle mouse button click on the titlebar
will maximize&slr.restore the window&per.
:dt.
:hp1.Move the mouse pointer to the focus window:ehp1.
:dd.
This group of features allows you to automatically move the mouse pointer to
the window which gets the focus&per.
.br
When none of the sub-options is checked, the feature works with all windows&per.
.br
When :hp1.New windows :ehp1. and&slr.or :hp1.Dialogs :ehp1. are checked, the
feature works when a new window is created or when a dialog gets the focus&per.
.br
:hp1.Exclude buttonless dialogs :ehp1. lets Styler&slr.2 ignore dialogs without buttons&per.
.br
The :hp1.Steps :ehp1. spin button sets how many steps it takes to
move the mouse pointer to the destination&per.
.br
If you want to interrupt the mouse motion, move the pointer in the opposite
direction&semi. the mouse motion thread will detect that and will stop moving
the mouse&per.
:edl.
.br

.* htmlsource = pgmoukbd.htm
:h2 res=19 x=left y=bottom width=100% height=100%.The mouse&slr.keyboard functions dialog
.br
This dialog is present both as a notebook page and as an independent
dialog window&per. It will pop up each time that you click on a button to
select the keyboard or mouse event for a given option (for instance, the
:hp1.Mouse :ehp1. and :hp1.Keyboard :ehp1. buttons of the
:link reftype=hd res=13.Window rolling:elink.
page)&per.
.br
As an independent dialog, it has the ordinary :hp1.OK:ehp1. , :hp1.Cancel :ehp1.
and :hp1.Help :ehp1. buttons&per.
.br
Its purpose is to give the user a sort of summary of all the mouse
and keyboard events which have been selected&per.
:p.
The :hp1.Function&colon. :ehp1. list box shows all the available features&per.
:p.
The :hp1.Status bar :ehp1. shows the control which should detect the
chosen mouse&slr.keyboard event&per.
:p.
The :hp1.Enable :ehp1. checkbox is greyed out (unavailable) when the
given feature must be set from the appropriate checkbox on its own page&per.
:p.
When radio buttons for :hp1.Click :ehp1. and :hp1.Double click :ehp1.
are not displayed, you simply choose which mouse button and optional key
combination (:hp1.Ctrl :ehp1. and&slr.or :hp1.Alt :ehp1. and&slr.or
:hp1.Shift :ehp1.) will be used with that feature, e&per.g&per. :hp1.window frame
drag :ehp1. or :hp1.VIO clipboard copy :ehp1.&per.
:p.
The :hp1.Keyboard Mappings :ehp1. page (and the :hp1.Functions controlled
by the keyboard dialog :ehp1.) allows you to associate a function key (F1-F12)
plus :hp1.Insert :ehp1. , :hp1.Delete :ehp1. and :hp1.Backspace :ehp1.
with one of the modifier keys (:hp1.Ctrl :ehp1. , :hp1.Alt :ehp1. and
:hp1.Shift:ehp1.
) to perform one of the listed features&per.
:p.
:hp2.Note&colon.:ehp2.
:p.
Now it is possible to define keyboard accelerators for performing some
operations in the folder windows&colon. arrange icons, find, open parent, open
properties notebook, refresh, sort by creation date, sort by creation date,
sort by last access date, sort by last write date, sort by name, sort by
real name, sort by size, sort by type&per.
.br
:hp1.Sort by extension :ehp1. at the moment works only by activating
the corresponding feature of NPS WPS Enhancer (if installed)&per. In a future
release, this operation will be performed by a built-in routine&per.
.br
In Warp 4, :hp1.Ctrl-Ins :ehp1. lets you create a new file from the
clipboard contents&per. In a future release, this feature will be extended to
Warp 3&per.

.* htmlsource = pgwinkey.htm
:h2 res=20 x=left y=bottom width=100% height=100%.The Windows Keys functions dialog
.br
This dialog allows you to assign some useful functions to the :hp1.Windows
keys :ehp1. of your keyboard&per. The assigned functions are performed as soon
as the key is released&per.
:dl break=all.
.br
:dt.
:hp1.Do not perform any action:ehp1.
:dd.
This just disables the key&per.
:dt.
:hp1.Hide&slr.show all windows:ehp1.
:dd.
This hides&slr.unhides all windows visible in the current desktop
(just in case you are running a virtual desktop utility), allowing you to
access the desktop icons&per.
:dt.
:hp1.Roll&slr.unroll all windows:ehp1.
:dd.
This function rolls&slr.unrolls all windows visible in the current desktop
(just in case you are running a virtual desktop utility), enabling you to
access the desktop icons&per.
:dt.
:hp1.Show window list:ehp1.
:dd.
This function displays the window list at the current
mouse position&per.
:dt.
:hp1.Show context menu:ehp1.
:dd.
If the focus window has any context menu, it will pop up as the
selected key is released&per.
:dt.
:hp1.Lock up the desktop:ehp1.
:dd.
Locks the desktop like the corresponding item of the desktop popup menu&per.
:dt.
:hp1.Open drive folder:ehp1.
:dd.
This opens the drive folder&per. If the folder is already opened and
its :hp1.Object open behaviour :ehp1. (in the drive folder Properties Notebook)
is set to :hp1.Display existing window :ehp1., then the previously opened window
gets the focus and is brought to the top&per.
:dt.
:hp1.Open System Setup folder:ehp1.
:dd.
This opens the System Setup folder&per. If the folder is already opened and
its :hp1.Object open behaviour :ehp1.
(in the System Setup folder properties notebook)
is set to :hp1.Display existing window :ehp1., then the previously opened window
gets the focus and is brought to the top&per.
:dt.
:hp1.Open OS&slr.2 command window:ehp1.
:dd.
This opens an OS&slr.2 command window&per.
:dt.
:hp1.Show WarpCenter desktop menu:ehp1.
:dd.
This opens the Warp Center menu which displays the current desktop
content&per. This and the following features will work only if Warp Center
is currently running&per.
:dt.
:hp1.Show WarpCenter window list:ehp1.
:dd.
The Warp Center window list is displayed on the screen&per.
:dt.
:hp1.Show WarpCenter process list:ehp1.
:dd.
If the statement
.br
:font facename='System VIO' size=12x5.
SET KILLFEATUREENABLED=ON
:font facename=default size=0x0.
.br
is present in your CONFIG&per.SYS file, then this list will enable you to
kill processes&per.
:dt.
:hp1.Show WarpCenter tray list:ehp1.
:dd.
This function lists the Warp Center trays&per.
:dt.
:hp1.Switch to the next WarpCenter tray:ehp1.
:dd.
The next Warp Center tray is brought to the top as soon as the selected
key is released&per.
:dt.
:hp1.Show WarpCenter information menu:ehp1.
:dd.
This functions shows the content of the :hp1.Assistance center:ehp1.
folder as a Warp Center menu&per.
:edl.
.br

.* htmlsource = problems.htm
:h1 res=21 x=left y=bottom width=100% height=100%.Known problems
:ul.
.br
:li.
When you suspect that a particular program conflicts with Styler&slr.2, add
its name (including its full path) to the
:link reftype=hd res=8.General
Exceptions:elink.
page, then close the program (if it is still open) and restart
it&per. This usually works better than
:link reftype=hd res=6.disabling
Styler&slr.2:elink.
&per.
.br
If the program works correctly when listed in the General Exceptions page,
you can try to isolate the source of the conflict by deleting the program
pathname from the General Exceptions page and re-entering the pathname in
a list of local Exceptions for a given group of features&per. This approach lets you
fine-tune exceptions for a few particular options and avoid disabling
Styler&slr.2 globally for the program in question&per.
.br
As a last resort, if you do not succeed by adding the conflicting program
to the General Exceptions page nor by disabling Styler&slr.2, yet you want to
be sure that the problems do derive from Styler&slr.2, shutdown the system and
then reboot it&per. After the OS&slr.2 boot logo disappears, press the
:hp1.Control :ehp1. key and keep it pressed down until you hear a double
beep from the PC loudspeaker&per. This will avoid having to load SMARTWIN&per.DLL
during bootup and the system will behave as if the program never had been
installed&per.
.br
Now you can conduct all the tests needed to pinpoint the conflict&per. If test
results point to Styler&slr.2 as the cause, then
:link reftype=fn refid=29.send me:elink.
a report of the problem&per.
:li.
If you use Styler&slr.2 in combination with other PM enhancers like Xit, NPS
WPS Enhancer, Program Commander&slr.2, Object Desktop, etc&per., avoid conflicts
by disabling a given option under all but one enhancer&per.
:li.
The MPTS configuration program doesn&csq.t work properly with Styler&slr.2&per. The
window controls (rolling, close, etc) added by Styler&slr.2 seem
to be the cause of the problem&per.
.br
To solve this problem and let the program work properly, add MPTS&per.EXE
(including its full path) to the General Exceptions page or to the
list of exceptions for the particular features involved&colon.
:ol compact.
.br
:li.
titlebar exceptions (if you are using the titlebar enhancements)
:li.
titlebar hiding exceptions (only if you added the Hide Titlebar button
to the window controls)
:li.
window rolling (only if you added the Roll Windows button)&per.
:eol.
.br
:p.
:hp2.Note&colon.:ehp2.
.br
Since Styler&slr.2 version 1&per.1, the program MPTS&per.EXE is automatically added
to the exceptions by the installer&per.
:li.
Sometimes (it happened only a couple of times here) and for no apparent
reason, one of the mouse pointers in the current set (for instance, the :hp1.Size
NS :ehp1. pointer) gets corrupted&per. To get rid of the corrupted bitmap,
open the :hp1.Mouse:ehp1. object in the :hp1.System Setup:ehp1. folder, select
a different set of pointers and then restore the previous one through the
Undo  button&per.
:li.
The :hp1.View :ehp1. caption (i&per.e&per. Icon view, Tree view, etc&per.) is missing from
the folder titlebar&per.
:li.
The titlebar enhancements work only with real titlebars (FID_TITLEBAR windows)
so you get the default titlebar inside Pro-News&per.
:li.
Older releases of PMView (prior to PMView 2000) use their own procedure to paint
the titlebar&per. Upgrade to PMView 2000 if you want to get rid of this problem&per.
:li.
Some users reported that moving windows by dragging the titlebar did not always
work when Styler&slr.2 was installed along with Object Desktop&per. I modified
the mouse events detection procedure and the problem should have been reduced
if not completely eliminated&per. Another user suggested eliminating an Object
Desktop DLL from the list of the DLLs which are loaded during system startup&per.
.br
If you have Warp4 and installed fixpack 5 or a later one, you have
in your &bsl.OS2&bsl.SYSTEM directory the program REGEDIT2&per.EXE&semi. if you cannot find
it, you can find various free INI file editors on the Hobbes server&per. You
must edit your OS2&per.INI file, often defined as user profile or HINI_USER_PROFILE
in REGEDIT2&per.EXE, and look for the SYS_DLLS  item and its LoadPerProcess
sub-item&per.
.br
This sub-item contains a list of DLL names (not including the path
and the DLL extensions) separated by a space&per. To remove the Object Desktop
DLL from the list, delete the OBJDWCTL string, save the new INI file and
reboot&per.
.br
If the system works better this way and you are not missing any of
your favourite Object Desktop features, just leave it this way&per. If you
want to restore its previous state, just add the OBJDWCTL string to the
list&per. You could also try to change the order in the list by putting SMARTWIN
before OBJDWCTL&per.
.br
I do not own any version of Object Desktop and am not planning to buy
it just to make few tests, so I would be grateful if you let me know your
experiences with the above described modifications&per. If it works, I could
make a small application (and&slr.or add a setting in the Styler&slr.2 Preferences
Notebook) to automatically remove, restore or change the order of the Object
Desktop DLL in the list in OS2&per.INI&per.
:li.
Window rolling does not work properly with some programs (AICQ, MED)&per.
:li.
A user reported that in Warp 3, the color selection buttons do not work
when the mouse is set up for left-handed use&per. I tried unsuccessfully to
replicate the problem in a Warp 3 partition (refresh release - no fixpack)
so I guess it is some bug from one of the most recent fixpacks&per.
:li.
Window controls in Watch Cat do not work properly&per.
:li.
Various problems have been reported with Describe, OpenChat&slr.2&per.
:li.
The titlebar controls are not displayed properly with the IBM EWS application
PMCALC&per.
.br
If you check the :hp1.Context menu :ehp1. option in the Titlebar settings
page, you can access the system menu by clicking with mouse button 2
on the program titlebar&per.
:li.
The program has been only partially tested with Warp 3&per. I&csq.ve just installed
Warp 3 and soon will install the latest Warp 3 fixpack to check if everything
works properly&per. So Warp 3 compatibility should improve before the next
release&per.
:eul.
.br

.* htmlsource = planned.htm
:h1 res=22 x=left y=bottom width=100% height=100%.Planned features
.br
All the features planned for version 1 are present in this release&per.
.br
In future, only minor enhancements and bug fixes will be added to this
version&per.
.br
A preliminary list of the features planned for version 2 will be enclosed
in the first release of version 2&per.

.* htmlsource = register.htm
:h1 res=23 x=left y=bottom width=100% height=100%.Registration
.br
Styler&slr.2 is a shareware program&per. Unless you register it, the program will
stop working when the 30-day evaluation period expires&per.
.br
Before registering, check at
:link reftype=fn refid=31.http&colon.&slr.&slr.space&per.tin&per.it&slr.scienza&slr.acantato:elink.
for the latest news or for a newer version of the program&per.
:p.
:font facename='Helv' size=16x10.:hp2.
Contents
:font facename=default size=0x0.:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=24.Quick notes:elink.
:li.
:link reftype=hd res=25.How to register:elink.
:li.
:link reftype=hd res=26.Licenses:elink.
:li.
:link reftype=hd res=27.Payment:elink.
:li.
:link reftype=hd res=28.Contacts:elink.
:eul.
.br

.* htmlsource = rgquick.htm
:h2 res=24 x=left y=bottom width=100% height=100%.Quick notes
.br
You should be able to register even without reading this documentation&per.
As you move the mouse around on the registration application, you&csq.ll be
able to read in the application status bar some hints about the various
fields and buttons&per.
.br
In any case, please notice&colon.
:ol.
.br
:li.
The registration program uses your personal data to generate the :hp1.customer
code :ehp1.&per. You&csq.ll have to send your customer code to me to receive the
:hp1.registration key :ehp1. which will allow you to use the program beyond
the evaluation period&per.
:li.
You do not have to send all your data&colon. send me just your customer code&per.
:li.
Add in the body of the message any relevant information which does not fit in the
customer data form&per.
:li.
Your name, address, and other personal data will not be used for any other
purpose (like spamming) than issuing an invoice&per.
.br
You will receive e-mail from me about program updates and new versions only if you request it&per.
:li.
Faked data (for instance, John Doe living on Nowhere St&per. etc&per.) will not
be accepted&per.
:li.
Once you send your customer code, :hp2.DO NOT MODIFY YOUR DATA!!!:ehp2. Otherwise
the registration key you&csq.ll receive won&csq.t work&per.
:li.
Once registered :hp2.make a backup copy of SMARTWIN&per.INI:ehp2. if you do not
want to lose your data&per.
:li.
You&csq.ll receive your registration key only after I get your customer code
and receive the due amount&per.
:li.
The registration key will be valid from the current program version up
to but not including version 2&per.
:li.
There are two kinds of licenses-- Ordinary and Advanced (previously called an Upgrade License)&per.
The Advanced License will give you the right to receive a free registration
key for Styler&slr.2 version 2 up to but not including version 3&per. See the
:link reftype=hd res=26.Licenses:elink.
chapter for more details&per.
:eol.
.br
If you have any problem running the registration program, please read the following documentation carefully&per.
.br
If you still have any questions, feel free to
:link reftype=hd res=28.get
in touch with me:elink.
by e-mail or ordinary mail&per.

.* htmlsource = rghowto.htm
:h2 res=25 x=left y=bottom width=100% height=100%.How to register
.br
To register you must use the enclosed application
:hp1.Styler&slr.2 Registration
:ehp1.&per. As you run it, you will see an edit window showing this file and a
few buttons&per.
.br
By moving the mouse on the various window controls, you&csq.ll be able to read
in the status bar some hints on program usage&per.
:p.
In any case, here is the step-by-step registration procedure&per. (Go
directly to step 10 if you just need to insert your registration key)&per.
:ol.
.br
:li.
Click on the :hp1.Customer data :ehp1. button&semi. some entry fields will be shown&per.
:li.
Enter your company name and address&per. In the event that you are purchasing
the program for your personal use, enter your own name and address&per.
:li.
Enter the e-mail address to which you want me to send the registration key&per.
:li.
If, according to the rules of your country of residence, invoices should
mention your fiscal code or any other kind of similar data, don&csq.t forget
to enter it in the appropriate field&per.
.br
This is relevant only if you are purchasing the license directly from
me&per. If you are purchasing the license from a reseller or do not know how
to fill this field just write a dash&per.
:li.
If you want to receive an e-mail notification when Styler&slr.2 is updated or
when there is any news about any program of mine, then choose the appropriate
item from the spin button&per. If you do not want to receive any e-mail then
check that the text displayed in the spin button control is :hp1.I do not
want to receive any news by e-mail :ehp1.&per.
:li.
The :hp1.Customer code :ehp1. button will be enabled only when all the
fields have been filled&per. If you cannot fill some fields (for instance, you
do not have an e-mail address), then write just a dash (-)&per.
:li.
Now click on the :hp1.Customer code :ehp1. button to generate your own
:hp1.customer code :ehp1.&per.
:li.
The customer code contains all your personal data in an encrypted format&per.
Click on the :hp1.Save :ehp1. button to save your customer code in a file and send the
file by e-mail to acantatore&atsign.tin&per.it&per.
.br
In case you do not have access to a usable e-mail address, you can
print your customer code and send it to&colon.
:sl compact.
:li.Alessandro Cantatore
.br
via Raffaello 41
.br
70032 Bitonto (BA)
.br
:hp5.ITALY:ehp5.:esl.
:p.
:hp2.DO NOT MODIFY YOUR DATA AFTER YOU SEND YOUR CUSTOMER CODE:ehp2., otherwise
the registration key you&csq.ll receive will no longer be valid&per.
:li.
Now you can close the registration application and wait for your registration
key&per.
:li.
Once I receive your customer code and the confirmation
that the program has been paid, I&csq.ll send you the registration key (by
e-mail or ordinary mail)&per. As soon as you receive the key, run the registration
program and click on the :hp1.Registration key :ehp1. button&per.
:li.
Paste or type in the edit field your registration key and click on the
:hp1.OK :ehp1. button&per.
.br
You can even paste the whole e-mail message you receive from me since
the program should be able to find the key from the tags
:font facename='System VIO' size=12x5.
<BEGINKEY>
:font facename=default size=0x0.
and
:font facename='System VIO' size=12x5.
<ENDKEY>
:font facename=default size=0x0.
contained in the message&per.
.br
The message :hp1.The registration key has been saved! :ehp1. will
be displayed in the Edit window&per.
:li.
Now you can close the registration program and reboot&per.
:li.
After rebooting, open the Styler&slr.2 Preferences application&per. If the program
has been successfully registered, the window title will be just :hp1.Styler&slr.2
- Preferences :ehp1. without the notation :hp1.(unregistered) :ehp1.
at the end of it&per.
:eol.
.br

.* htmlsource = rglicens.htm
:h2 res=26 x=left y=bottom width=100% height=100%.Licenses
.br
Since I expect to release Styler&slr.2 version 2 in few weeks
(I&csq.m not able
to give you an exact date though) I decided to provide two different
kinds of licenses&colon.
:dl break=all.
.br
:dt.
:hp2.ordinary license&colon.:ehp2.
:dd.
will give you the right to get free upgrades up to but not including
version 2&per.
:dt.
:hp2.advanced license:ehp2. (formerly called upgrade license)&colon.
:dd.
will give you the right to receive a free registration key, as soon as
version 2 is released, with right to free upgrades up to but not including
version 3&per.
:edl.
.br

.* htmlsource = rgpay.htm
:h2 res=27 x=left y=bottom width=100% height=100%.Payment
.br
Registering Styler&slr.2 is easy&per. Order from
:link reftype=fn refid=32.Mensys:elink.
or directly
from the program author, with your choice of several payment methods&per.
:ul.
.br
:li.Ordering from
:link reftype=fn refid=32.
Mensys:elink.
:p.
Mensys offers convenient online shopping from anywhere in
the world&per. When you order from Mensys, the Styler&slr.2
registration prices are
:ul compact.
.br
:li.19 Euro for an
:link reftype=fn refid=32.Ordinary License:elink.
and
:li.23 Euro for an
:link reftype=fn refid=32.Advanced License:elink.
&per.
:eul.
.br
:li.
Ordering directly from the
:link reftype=fn refid=29.program author
:elink.
:ul compact.
.br
:li.a Styler&slr.2 version 1&per.0 Ordinary License costs 17 EURO while
:li.a Styler&slr.2 Advanced License costs 20 EURO&per.
:eul.
.br
:p.
If you order from Italy you must add I&per.V&per.A&per. (20 %)&per.
.br
It looks like in future V&per.A&per.T&per. will be required also for purchases
from other European countries belonging to the EC&per. Please ask before
ordering&per.
:p.
In this case you can pay by sending
an international money order to the author&csq.s address&colon.
:sl compact.
:li.Alessandro Cantatore
.br
via Raffaello 41
.br
70032 Bitonto (BA)
.br
:hp5.ITALY:ehp5.
.br
:hp2.collectable at&colon.:ehp2.
.br
Ufficio Postale Bitonto Succursale 1:esl.
:p.
You can send payment through the Post Office if you reside in one of the following countries&colon.
.br
Algeria, Austria, Belgium, Denmark, Finland, France, Germany, Japan,
United Kingdom, Luxemburg, Norway, Holland, Italy, Spain, Sweden, Switzerland,
or Tunisia&per.
.br
Credit the due amount directly to the author&csq.s current Postal Account&colon.
:sl compact.
:li.Poste Italiane S&per.p&per.A&per.
.br
account number&colon. 12590741
.br
account name&colon. Alessandro Felice Cantatore
.br
address&colon. see above
:esl.
:p.
:hp2.Note&colon.:ehp2.
:p.
Ask for informations at any Post Office in your
country for more details on this method of payment&per.
:eul.
.br

.* htmlsource = rgcontct.htm
:h2 res=28 x=left y=bottom width=100% height=100%.Contacts
:ul.
.br
:li.My e-mail addresses are&colon.
:ol compact.
.br
:li.
:link reftype=fn refid=29.acantatore&atsign.tin&per.it:elink.
:li.
:link reftype=fn refid=30.alscant&atsign.libero&per.it:elink.
:eol.
.br
:li.My home address is&colon.
:sl compact.
:li.
Alessandro Cantatore
.br
via Raffaello 41
.br
70032 Bitonto (BA)
.br
:hp5.ITALY:ehp5.
:esl.
:li.for the latest news check&colon.
:ol compact.
.br
:li.
:link reftype=fn refid=31.http&colon.&slr.&slr.space&per.tin&per.it&slr.scienza&slr.acantato:elink.
:li.
:link reftype=fn refid=33.http&colon.&slr.&slr.xoomer&per.virgilio&per.it&slr.acantato:elink.
:eol.
.br
:eul.
.br

.* mailto:acantatore@tin.it
:fn id=29.
:p.The link you selected points to an external resource&per. Click the URL below to launch Netscape Navigator
:p.
:link reftype=launch object='netscape.exe ' data='mailto:acantatore@tin.it'.
mailto&colon.acantatore&atsign.tin&per.it
:elink.
:efn.

.* mailto:alscant@libero.it
:fn id=30.
:p.The link you selected points to an external resource&per. Click the URL below to launch Netscape Navigator
:p.
:link reftype=launch object='netscape.exe ' data='mailto:alscant@libero.it'.
mailto&colon.alscant&atsign.libero&per.it
:elink.
:efn.

.* http://space.tin.it/scienza/acantato
:fn id=31.
:p.The link you selected points to an external resource&per. Click the URL below to launch Netscape Navigator
:p.
:link reftype=launch object='netscape.exe ' data='http://space.tin.it/scienza/acantato'.
http&colon.&slr.&slr.space&per.tin&per.it&slr.scienza&slr.acantato
:elink.
:efn.

.* http://shop.mensys.nl/catalogue/mns_styler.html
:fn id=32.
:p.The link you selected points to an external resource&per. Click the URL below to launch Netscape Navigator
:p.
:link reftype=launch object='netscape.exe ' data='http://shop.mensys.nl/catalogue/mns_styler.html'.
http&colon.&slr.&slr.shop&per.mensys&per.nl&slr.catalogue&slr.mns_styler&per.html
:elink.
:efn.

.* http://xoomer.virgilio.it/acantato
:fn id=33.
:p.The link you selected points to an external resource&per. Click the URL below to launch Netscape Navigator
:p.
:link reftype=launch object='netscape.exe ' data='http://xoomer.virgilio.it/acantato'.
http&colon.&slr.&slr.xoomer&per.virgilio&per.it&slr.acantato
:elink.
:efn.

:euserdoc.