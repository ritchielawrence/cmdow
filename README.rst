=====
Cmdow
=====

Table of Contents
-----------------

* Synopsis_
* Usage_
* Screenshots_
* Revisions_
* License_

Synopsis
--------

Cmdow is a Win32 commandline utility for NT4/2000/XP/2003/2008/7 that allows windows to be listed, moved, resized, renamed, hidden/unhidden, disabled/enabled, minimized, maximized, restored, activated/inactivated, closed, killed and more.

It is a small standalone executable. It does not create any temporary files, nor does it write to the registry. There is no installation procedure, just run it. To completely remove all traces of it from your system, delete it.

Cmdow was written with batch file programmers in mind. Particular attention has been paid to Cmdows output making it easy to process with the 'FOR /F' command found in the Windows command processor (cmd.exe).

Cmdow is simple and intuitive to use. To list all its options, type cmdow /?. For detailed help on any option type cmdow /? <option>. Eg cmdow /? /run.

Usage
-----

This form of CMDOW lists window details.

::

  CMDOW [window | /T] [/B] [/F] [/P]

  window  Window to list. Specify a window by its handle (in hex format, eg
          0x1A0142) or caption (window title). Caption is case insensitive.
          Use asterisk before/after caption to match zero or more characters.
          Windows without captions are matched by their ClassName. Use double
          quotes for captions containing whitespace or control characters. The
          At symbol '@' may be used to refer to this window. eg 'CMDOW @ /HID'.
  /T      List only the windows shown on the taskbar (these are typically
          visible level 1 windows that do not have an owner).
  /B      Bare format. Does not print the field names when listing windows.
  /F      Full format. By default CMDOW truncates Image names to eight
          characters and Captions/ClassNames to fit under 80 characters.
          Use this option to disable this feature.
  /P      Shows window position, coords of top left corner of the window and
          its absolute width and height in pixels.

Here is a description of the output fields:-

::

  Handle         Handle of the window, in hexadecimal format (0x000123AB).
  Lev            Level of the window. The desktop window at level 0 is the area
                 on which all other windows are painted. Top level windows are
                 level 1 (and may be shown on the taskbar), all windows of
                 level 2+ are child windows.
  Pid            Process Id of the window (Task Manager also shows Pids).
  Window status  How the window is displayed (Minimized, Maximized, Restored,
                 Active, Inactive, Enabled, Disabled, Visible or Hidden).
  Left, Top      Coords of left top corner of the window relative to the
                 parent window. For top level windows this is the desktop, and
                 for child windows it is their top level window.
  Width, Height  Absolute width and height of the window in pixels.
  Image          Executable that created the window (Task Manager also shows
                 Image Names on its Process tab).
  Caption        Window title. If a window has no caption, its ClassName is
                 listed instead.

This form of CMDOW performs a single action typically affecting all windows.

::

  CMDOW /TH | /TV | /CW | /MA | /UW | /AT | /FS | /WM

  /TH     Tiles all windows horizontally. Functionally equivalent to right-
          clicking the taskbar and selecting 'Tile Windows Horizontally'.
  /TV     Tiles all windows vertically. Functionally equivalent to right-
          clicking the taskbar and selecting 'Tile Windows Vertically'.
  /CW     Functionally equivalent to right-clicking the taskbar and selecting
          'Cascade Windows'.
  /MA     Minimizes all windows. Functionally equivalent to right-clicking the
          taskbar and selecting 'Minimize All Windows' or using the keyboard
          shortcut Win+M. A window may not be minimized if it has a modal
          dialog box open (typically the window displayed by Help | About).
  /UW     Undo the effect of /TH, /TV, /CW and /MA. Functionally equivalent to
          right-clicking the taskbar and selecting 'Undo...' or using the
          keyboard shortcut Win+Shift+M.
  /AT     Similar to the keyboard shortcut Alt+Tab but instead of activating
          the next highest window in the Z order, /AT activates the window at
          the bottom of the Z order. Consequently, /AT can be used to activate
          every window on the taskbar sequentially. To prevent windows on the 
          taskbar from flashing, set the following registry value to zero:-
          HKCU\Control Panel\Desktop\ForegroundLockTimeout.
  /FS     Switch the window running this process into full screen mode.
  /WM     Switch the window running this process into window mode.

  This example batch file activates a different window every ten seconds:-

    @ECHO OFF
    :LOOP
    CMDOW /AT&PING 127.0.0.1 -n 11 >NUL&GOTO :LOOP

This form of CMDOW performs specified action(s) on the specified window(s).
Some of these commands allow you to manipulate windows in ways not normally
possible. Improper use may cause unexpected results and system instability.

:: 

  CMDOW window {[/ACT] [/INA] [/ENA] [/DIS] [/VIS] [/HID] [/MIN] [/MAX] [/RES]}

  window  Window to act on. Specify a window by its handle (in hex format, eg
          0x1A0142) or caption (window title). Caption is case insensitive.
          Use asterisk before/after caption to match zero or more characters.
          Windows without captions are matched by their ClassName. Use double
          quotes for captions containing whitespace or control characters. The
          At symbol '@' may be used to refer to this window. eg 'CMDOW @ /HID'.
  /ACT    Activates the specified window. It's possible to activate a hidden
          and/or disabled window (although doing so will not make it visible or
          enabled). If a minimized window is activated, it remains minimized.
  /INA    Inactivates the specified window (activates a window shown on the
          taskbar with the lowest Z order). If the specified window is already
          inactive or it is the only window shown on the taskbar, then /INA has
          no effect. This action added for completeness rather than usefulness.
  /ENA    Enables the specified window (allowing user interaction).
  /DIS    Disables the specified window. A disabled window will continue to run
          as usual, although it will not accept user input until it is enabled.
  /VIS    Unhides the specified window (make it visible).
  /HID    Hides the specified window. Although a hidden window will not be
          shown on the taskbar, it will still run and may be unhidden later.
  /MIN    Minimizes specified window. Not all windows can be minimized, dialog
          boxes for instance.
  /MAX    Maximizes specified window (even those not having a Maximize button).
  /RES    Restores the specified window to its former size and position. If a
          minimized window was originally maximized, then /RES will maximize
          the window. Applying /RES a second time, will restore the window to
          the size and position it was before it was maximized.

  If more than one window matches the specified caption, CMDOW only acts on the
  first one. To override this add the /DBM (Don't Blame Me) switch - DANGEROUS.
  Multiple actions are performed in order, from left to right. Eg:-

    CMDOW 0x0E0144 /hid /ren "10% complete" /mov 0 0 /siz 300 100 /act /vis

This form of CMDOW executes the specified file or opens it using the
application associated with the filename extension.

::

  CMDOW /RUN [state] file [args]

  /RUN    Executes the specified file if possible, otherwise opens it using the
          application associated with the filename extension.
  state   Requests how the application's window is initially displayed. Use
          /MIN for minimized, /MAX for maximized and /HID for hidden. If
          omitted the application is started in its default show state. Some
          applications ignore this, use CMDOW to alter the window state after
          the application has started.
  file    File to execute/open. Use double quotes if filename contains spaces.
  args    Optional arguments passed on to the application. Specify any args
          exactly as you would if executing/opening the file directly.

  Eg /P instructs Notepad to send the specified file to the default printer,
  then quit. This example prints readme.txt without displaying any windows:-

    CMDOW /RUN /HID notepad /P readme.txt

  CMDOW can be used to create autorun CDs that work on all Win32 platforms.
  Copy CMDOW.EXE to the CD and create an autorun.inf file. Here is a sample:-

    [autorun]
    open=cmdow /run /max \video.mpg
    icon=myicon.ico
    shell\readme=Read &Me
    shell\readme\command=cmdow /run \readme.htm

Screenshots
-----------

.. image:: cmdow-screenshot0.png

.. image:: cmdow-screenshot1.png

.. image:: cmdow-screenshot2.png

.. image:: cmdow-screenshot3.png

.. image:: cmdow-screenshot4.png

.. image:: cmdow-screenshot5.png

Revisions
.........

+--------+----------+---------------------------------------------------------------------------------+
|Revision|Date      |Changes                                                                          |
+========+==========+=================================================================================+
|1.0     |2001-12-20|Cmdow created.                                                                   |
+--------+----------+---------------------------------------------------------------------------------+
|1.1     |2002-02-25|Added /RUN command.                                                              |
+--------+----------+---------------------------------------------------------------------------------+
|1.2     |2002-10-29|Fixed issue caused when conole windows have no title. Enhance /AT feature to     |
|        |          |restore an activated window if it was minimized.                                 |
+--------+----------+---------------------------------------------------------------------------------+
|1.3     |2002-11-20|Added /FS and /WM options to allow switching between full screen mode and window |
|        |          |mode.                                                                            |
+--------+----------+---------------------------------------------------------------------------------+
|1.4     |2002-11-07|Added /TOP and /NOT options to allow the setting/clearing of windows 'always on  |
|        |          |top'. Fixed argument parsing bug for /MOV and /REN options.                      |
+--------+----------+---------------------------------------------------------------------------------+
|1.4.1   |2003-05-14|Cmdow can now rename the console window that launched it. Previously the title   |
|        |          |would revert when Cmdow exited.                                                  |
+--------+----------+---------------------------------------------------------------------------------+
|1.4.2   |2003-08-18|/UM seperseded by /UW switch. See FAQ's for details.                             |
+--------+----------+---------------------------------------------------------------------------------+
|1.4.3   |2004-12-19|Fixed bug where renaming the current console window would fail if the new caption|
|        |          |title contained whitespace.                                                      |
+--------+----------+---------------------------------------------------------------------------------+
|1.4.4   |2014-11-08|Cmdow is now open source software released under the MIT license.                |
+--------+----------+---------------------------------------------------------------------------------+
|1.4.7   |2014-12-29|Wildcards can now be used to filter window captions. Also now possible to perform|
|        |          |actions on multiple windows. Handle display is extended to eight digits. Level is|
|        |          |dynamically sized. Class name is dynamically sized. Newlines in the caption are  |
|        |          |converted to underscore. Sizes are based on the entire window list, not just what|
|        |          |is displayed. Help pages updated. Internally: Remove process name length test, as|
|        |          |WideCharToMultiByte already restricts it. Combined multiple printfs. Substituted |
|        |          |some if/else assignments for ternary operator.                                   |
+--------+----------+---------------------------------------------------------------------------------+
|1.4.8   |2014-12-30|Help page typos corrected. Output can handle 5-digit PIDs.                       |
+--------+----------+---------------------------------------------------------------------------------+

License
.......

The MIT License (MIT)

Copyright (c) 2014 Ritchie Lawrence

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.