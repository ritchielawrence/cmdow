#include "header.h"

void ShowHelp(char *cmd)
{
	if( (!lstrcmpi("window", cmd)) ||
		(!lstrcmpi("/window", cmd)) ||
        (!lstrcmpi("/T", cmd)) ||
		(!lstrcmpi("/B", cmd)) ||
		(!lstrcmpi("/F", cmd)) ||
		(!lstrcmpi("/P", cmd))) {
		printf(
		"This form of CMDOW lists window details.\n\n"
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"  CMDOW [window | /T] [/B] [/F] [/P]\n\n"
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"  window  Window to list. Specify a window by its handle (in hex format, eg\n"
		"          0x1A0142) or caption (window title). Caption is case insensitive.\n"
        "          Use asterisk before/after caption to match zero or more characters.\n"
		"          Windows without captions are matched by their ClassName. Use double\n"
        "          quotes for captions containing whitespace or control characters. The\n"
		"          At symbol '@' may be used to refer to this window. eg 'CMDOW @ /HID'.\n"
		"  /T      List only the windows shown on the taskbar (these are typically\n"
		"          visible level 1 windows that do not have an owner).\n");
printf(
		"  /B      Bare format. Does not print the field names when listing windows.\n"
		"  /F      Full format. By default CMDOW truncates Image names to eight\n"
		"          characters and Captions/ClassNames to fit under 80 characters.\n"
		"          Use this option to disable this feature.\n"
		"  /P      Shows window position, coords of top left corner of the window and\n"
		"          its absolute width and height in pixels.\n\n");
		printf(
		"  Here is a description of the output fields:-\n\n"
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"  Handle         Handle of the window, in hexadecimal format (0x000123AB).\n"
		"  Lev            Level of the window. The desktop window at level 0 is the area\n"
		"                 on which all other windows are painted. Top level windows are\n"
		"                 level 1 (and may be shown on the taskbar), all windows of\n"
		"                 level 2+ are child windows.\n"
		"  Pid            Process Id of the window (Task Manager also shows Pids).\n"
		"  Window status  How the window is displayed (Minimized, Maximized, Restored,\n"
		"                 Active, Inactive, Enabled, Disabled, Visible or Hidden).\n");
		printf(
		"  Left, Top      Coords of left top corner of the window relative to the\n"
		"                 parent window. For top level windows this is the desktop, and\n"
		"                 for child windows it is their top level window.\n"
		"  Width, Height  Absolute width and height of the window in pixels.\n"
		"  Image          Executable that created the window (Task Manager also shows\n"
		"                 Image Names on its Process tab).\n"
		"  Caption        Window title. If a window has no caption, its ClassName is\n"
		"                 listed instead.\n\n");
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
	}
	else if((!lstrcmpi("/TH", cmd)) ||
			(!lstrcmpi("/TV", cmd)) ||
			(!lstrcmpi("/MA", cmd)) ||
			(!lstrcmpi("/UM", cmd)) ||
			(!lstrcmpi("/UW", cmd)) ||
			(!lstrcmpi("/CW", cmd)) ||
			(!lstrcmpi("/AT", cmd)) ||
			(!lstrcmpi("/FS", cmd)) ||
			(!lstrcmpi("/WM", cmd))) {
		printf(
		"This form of CMDOW performs a single action typically affecting all windows.\n\n"
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"  CMDOW /TH | /TV | /CW | /MA | /UW | /AT | /FS | /WM\n\n"
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"  /TH     Tiles all windows horizontally. Functionally equivalent to right-\n"
		"          clicking the taskbar and selecting 'Tile Windows Horizontally'.\n"
		"  /TV     Tiles all windows vertically. Functionally equivalent to right-\n"
		"          clicking the taskbar and selecting 'Tile Windows Vertically'.\n"
		"  /CW     Functionally equivalent to right-clicking the taskbar and selecting\n"
		"          'Cascade Windows'.\n");
		printf(
		"  /MA     Minimizes all windows. Functionally equivalent to right-clicking the\n"
		"          taskbar and selecting 'Minimize All Windows' or using the keyboard\n"
		"          shortcut Win+M. A window may not be minimized if it has a modal\n"
		"          dialog box open (typically the window displayed by Help | About).\n"
		"  /UW     Undo the effect of /TH, /TV, /CW and /MA. Functionally equivalent to\n"
		"          right-clicking the taskbar and selecting 'Undo...' or using the\n"
		"          keyboard shortcut Win+Shift+M.\n");
		printf(
		"  /AT     Similar to the keyboard shortcut Alt+Tab but instead of activating\n"
		"          the next highest window in the Z order, /AT activates the window at\n"
		"          the bottom of the Z order. Consequently, /AT can be used to activate\n"
		"          every window on the taskbar sequentially. To prevent windows on the \n"
		"          taskbar from flashing, set the following registry value to zero:-\n"
		"          HKCU\\Control Panel\\Desktop\\ForegroundLockTimeout.\n");
		printf(
		"  /FS     Switch the window running this process into full screen mode.\n"
		"  /WM     Switch the window running this process into window mode.\n\n");
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		printf(
		"  This example batch file activates a different window every ten seconds:-\n\n"

		"    @ECHO OFF\n"
		"    :LOOP\n"
		"    CMDOW /AT&PING 127.0.0.1 -n 11 >NUL&GOTO :LOOP\n");
	}
	else if((!lstrcmpi("/ACT", cmd)) ||
			(!lstrcmpi("/INA", cmd)) ||
			(!lstrcmpi("/ENA", cmd)) ||
			(!lstrcmpi("/DIS", cmd)) ||
			(!lstrcmpi("/VIS", cmd)) ||
			(!lstrcmpi("/HID", cmd)) ||
			(!lstrcmpi("/MIN", cmd)) ||
			(!lstrcmpi("/MAX", cmd)) ||
			(!lstrcmpi("/RES", cmd))) {
		printf(
		"This form of CMDOW performs specified action(s) on the specified window(s).\n"
		"Some of these commands allow you to manipulate windows in ways not normally\n"
		"possible. Improper use may cause unexpected results and system instability.\n\n"
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"  CMDOW window {[/ACT] [/INA] [/ENA] [/DIS] [/VIS] [/HID] [/MIN] [/MAX] [/RES]}\n\n"
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"  window  Window to act on. Specify a window by its handle (in hex format, eg\n"
		"          0x1A0142) or caption (window title). Caption is case insensitive.\n"
        "          Use asterisk before/after caption to match zero or more characters.\n"
        "          Windows without captions are matched by their ClassName. Use double\n"
        "          quotes for captions containing whitespace or control characters. The\n"
		"          At symbol '@' may be used to refer to this window. eg 'CMDOW @ /HID'.\n"
		"  /ACT    Activates the specified window. It's possible to activate a hidden\n"
		"          and/or disabled window (although doing so will not make it visible or\n"
		"          enabled). If a minimized window is activated, it remains minimized.\n");
		printf(
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"  /INA    Inactivates the specified window (activates a window shown on the\n"
		"          taskbar with the lowest Z order). If the specified window is already\n"
		"          inactive or it is the only window shown on the taskbar, then /INA has\n"
		"          no effect. This action added for completeness rather than usefulness.\n"
		"  /ENA    Enables the specified window (allowing user interaction).\n"
		"  /DIS    Disables the specified window. A disabled window will continue to run\n"
		"          as usual, although it will not accept user input until it is enabled.\n"
		"  /VIS    Unhides the specified window (make it visible).\n"
		"  /HID    Hides the specified window. Although a hidden window will not be\n"
		"          shown on the taskbar, it will still run and may be unhidden later.\n");
		printf(
		"  /MIN    Minimizes specified window. Not all windows can be minimized, dialog\n"
		"          boxes for instance.\n"
		"  /MAX    Maximizes specified window (even those not having a Maximize button).\n"
		"  /RES    Restores the specified window to its former size and position. If a\n"
		"          minimized window was originally maximized, then /RES will maximize\n"
		"          the window. Applying /RES a second time, will restore the window to\n"
		"          the size and position it was before it was maximized.\n\n"
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"  If more than one window matches the specified caption, CMDOW only acts on the\n"
		"  first one. To override this add the /DBM (Don't Blame Me) switch - DANGEROUS.\n"
		"  Multiple actions are performed in order, from left to right. Eg:-\n\n"

		"    CMDOW 0x0E0144 /hid /ren \"10%% complete\" /mov 0 0 /siz 300 100 /act /vis\n");
	}
	else if((!lstrcmpi("caption", cmd)) ||
			(!lstrcmpi("/caption", cmd)) ||
			(!lstrcmpi("/REN", cmd)) ||
			(!lstrcmpi("/MOV", cmd)) ||
			(!lstrcmpi("top", cmd)) ||
			(!lstrcmpi("/top", cmd)) ||
			(!lstrcmpi("left", cmd)) ||
			(!lstrcmpi("/left", cmd)) ||
			(!lstrcmpi("/SIZ", cmd)) ||
			(!lstrcmpi("width", cmd)) ||
			(!lstrcmpi("/width", cmd)) ||
			(!lstrcmpi("height", cmd)) ||
			(!lstrcmpi("/height", cmd)) ||
			(!lstrcmpi("/TOP", cmd)) ||
			(!lstrcmpi("/NOT", cmd)) ||
			(!lstrcmpi("/CLS", cmd)) ||
			(!lstrcmpi("/END", cmd))) {
		printf(
		"This form of CMDOW performs specified action(s) on the specified window(s).\n"
		"Some of these commands allow you to manipulate windows in ways not normally\n"
		"possible. Improper use may cause unexpected results and system instability.\n\n"
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"  CMDOW window {[/REN newcaption] [/TOP] [/NOT] [/MOV left top]\n"
		"        [/SIZ width height] [/CLS] [/END]}\n\n"
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"  window  Window to act on. Specify a window by its handle (in hex format, eg\n"
		"          0x1A0142) or caption (window title). Caption is case insensitive.\n"
        "          Use asterisk before/after caption to match zero or more characters.\n"
        "          Windows without captions are matched by their ClassName. Use double\n"
        "          quotes for captions containing whitespace or control characters. The\n"
		"          At symbol '@' may be used to refer to this window. eg 'CMDOW @ /HID'.\n"
		"  /REN    Renames the caption of the specified window to the caption specified\n"
		"          after /REN. Use double quotes for captions containing whitespace.\n");
		printf(
		"  /TOP    Makes the specified window always on top. It will remain on top even\n"
		"          when another window is activated. If two or more windows have this\n"
		"          attribute set, the topmost window will be that which had the\n"
		"          attribute set first.\n"
		"  /NOT    Makes window not always on top. This is the normal setting for most\n"
		"          windows.\n");
		printf(
		"  /MOV    Moves the specified window. The left and top arguments specify the\n"
		"          new position of the left top corner of the window. For level one\n"
		"          windows this is relative to the desktop, and for child windows this\n"
		"          is relative to its parent window. Level one windows can be moved off\n"
		"          the screen using negative coords or coords greater than the desktop\n"
		"          size. You should not move a minimized or maximized window without\n"
		"          first restoring it.\n"
		"  /SIZ    Resizes the specified window. The width and height arguments specify\n"
		"          the new absolute width and height. In general, windows cannot be made\n"
		"          smaller using /SIZ than they could if using conventional methods. You\n"
		"          should not resize a minimized or maximized window without first\n"
		"          restoring it.\n");
		printf(
		"  /CLS    Politely asks the specified window to close. Most applications will\n"
		"          respond to this request, although some may display a message box\n"
		"          asking you what to do with unsaved work.\n"
		"  /END    Kills the process associated with the specified window, the result is\n"
		"          all windows with the same Pid are also killed. Use very carefully.\n\n");
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		printf(
		"  If more than one window matches the specified caption, CMDOW only acts on the\n"
		"  first one. To override this add the /DBM (Don't Blame Me) switch - DANGEROUS.\n"
		"  Multiple actions are performed in order, from left to right. Eg:-\n\n"

		"    CMDOW 0x0E0144 /hid /ren \"10%% complete\" /mov 0 0 /siz 300 100 /act /vis\n");
	}
	else if((!lstrcmpi("/RUN", cmd)) ||
			(!lstrcmpi("state", cmd)) ||
			(!lstrcmpi("/state", cmd)) ||
			(!lstrcmpi("file", cmd)) ||
			(!lstrcmpi("/file", cmd)) ||
			(!lstrcmpi("args", cmd)) ||
			(!lstrcmpi("/args", cmd))) {
		printf(
		"This form of CMDOW executes the specified file or opens it using the\n"
		"application associated with the filename extension.\n\n"
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"  CMDOW /RUN [state] file [args]\n\n"
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"  /RUN    Executes the specified file if possible, otherwise opens it using the\n"
		"          application associated with the filename extension.\n"
		"  state   Requests how the application's window is initially displayed. Use\n"
		"          /MIN for minimized, /MAX for maximized and /HID for hidden. If\n"
		"          omitted the application is started in its default show state. Some\n"
		"          applications ignore this, use CMDOW to alter the window state after\n"
		"          the application has started.\n");
		printf(
		"  file    File to execute/open. Use double quotes if filename contains spaces.\n"
		"  args    Optional arguments passed on to the application. Specify any args\n"
		"          exactly as you would if executing/opening the file directly.\n\n"
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"  Eg /P instructs Notepad to send the specified file to the default printer,\n"
		"  then quit. This example prints readme.txt without displaying any windows:-\n\n"

		"    CMDOW /RUN /HID notepad /P readme.txt\n\n");
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		printf(
		"  CMDOW can be used to create autorun CDs that work on all Win32 platforms.\n"
		"  Copy CMDOW.EXE to the CD and create an autorun.inf file. Here is a sample:-\n\n"
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"    [autorun]\n"
		"    open=cmdow /run /max \\video.mpg\n"
		"    icon=myicon.ico\n"
		"    shell\\readme=Read &Me\n"
		"    shell\\readme\\command=cmdow /run \\readme.htm\n");
	}
	else {
		printf(
		"CMDOW [Version 1.4.8] Win32 Commandline Window Utility for NT4/2000/XP/7.\n"
		"(C) Copyright 2001-2014 Ritchie Lawrence, http://www.commandline.co.uk.\n\n"
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"  CMDOW [window | /T] [/B] [/F] [/P]\n"
		"  CMDOW /TH | /TV | /MA | /CW | /UW | /AT | /FS | /WM\n"
		"  CMDOW window {[/ACT] [/INA] [/ENA] [/DIS] [/VIS] [/HID] [/MIN] [/MAX] [/RES]\n"
		"        [/REN caption] [/TOP] [/NOT] [/MOV left top] [/SIZ width height] [/CLS]\n"
		"        [/END]}\n"
		"  CMDOW /RUN [state] file [args]\n\n");
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		printf(
		"  window  List specified window(s) (if omitted, all windows are listed).\n"
		"  /T      List windows only shown on the taskbar.\n"
		"  /B      List windows using bare format (no heading information).\n"
		"  /F      List windows showing full information (don't truncate any fields).\n"
		"  /P      List windows showing position and size (left, top, width and height).\n\n"
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"  /TH     Tile windows horizontally.    /TV     Tile windows vertically.\n"
		"  /MA     Minimize all windows.         /CW     Cascade windows.\n"
		"  /UW     Undo tile/minimize/cascade.   /AT     Intelligent [Alt]-[Tab].\n"
		"  /FS     Switch to full screen mode.   /WM     Switch to window mode.\n\n");
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		printf(
		"  /ACT    Activate specified window.    /INA    Inactivate specified window.\n"
		"  /ENA    Enable specified window.      /DIS    Disable specified window.\n"
		"  /VIS    Unhide specified window.      /HID    Hide specified window.\n"
		"  /MIN    Minimize specified window.    /MAX    Maximize specified window.\n"
		"  /RES    Restore specified window.     /REN    Rename specified window.\n"
		"  /TOP    Make window always on top.    /NOT    Make window not always on top.\n"
		"  /MOV    Move specified window.        /SIZ    Resize specified window.\n"
		"  /CLS    Close specified window.       /END    Kill process linked to window.\n\n");
//		 ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		printf(
		"  /RUN    Executes or opens specified file using associated application.\n"
		"  state   Initial show state of window (/MIN, /MAX or /HID). Default is normal.\n"
		"  args    Optional commandline arguments passed to launched application.\n\n");
		printf(
		"  Specify a window by its caption (case insensitive) or handle in hex format.\n"
		"  Wildcards may be used. Use At symbol to refer to this window. For more help\n"
		"  use CMDOW /? <parameter>. Eg CMDOW /? /window or CMDOW /? /act.\n");
	}
}

