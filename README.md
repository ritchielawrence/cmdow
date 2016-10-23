## Table of Contents

* [Is Cmdow Malware?](#is-cmdow-malware)
* [Synopsis](#synopsis)
* [Usage](#usage)
* [Examples](#examples)
* [FAQs](#faqs)
* [Screenshots](#screenshots)
* [Revisions](#revisions)
* [Copyright and License](#copyright-and-license)

## Is Cmdow Malware?<a name="is-cmdow-malware"></a>

**Of course not!** However, not everyone agrees... One of Cmdow's many features allow the user to hide program windows which has led to 40% of anti-virus vendors classifying Cmdow as anything from a hacking tool to a trojan<sup>[&dagger;](#trojan)</sup>. On the other hand, 60% of anti-virus vendors regard Cmdow as completely safe.

You can view the anti-virus scan results yourself. Download and extract the latest version of the Cmdow archive, then upload cmdow.exe to [VirusTotal](https://virustotal.com/). You'll end up at a page like this: 
[https://virustotal.com/en/file/767b877e735c425bf05c34683356abfde4070b092f17a4741ea5ac490611f3de/analysis/](https://virustotal.com/en/file/767b877e735c425bf05c34683356abfde4070b092f17a4741ea5ac490611f3de/analysis/) (note, the long hexadecimal string in the URL matches the [SHA256 checksum](https://en.wikipedia.org/wiki/Sha1sum) of cmdow.exe, v1.4.8 in this instance).

Obviously, if you have any doubts about the safety of Cmdow then don't use it or audit the source code yourself. Auditing should be fairly straightforward as the code is self explanatory with liberal comments and there is no assembly or binary blobs. It compiles without errors or warnings using the free [Code::Blocks](http://www.codeblocks.org/) IDE. There is even a Code::Blocks project file ([cmdow.cbp](https://github.com/ritchielawrence/cmdow/blob/master/cmdow.cbp)) included in the Cmdow download.

<sup><a name="trojan">&dagger;</a></sup> Ten vendors actually regard passing a zero to the Windows API function [ShowWindowAsync](https://msdn.microsoft.com/en-us/library/windows/desktop/ms633549%28v=vs.85%29.aspx) as enough to create a trojan!

## Synopsis<a name="synopsis"></a>

Cmdow is a Win32 commandline utility for NT4/2000/XP/2003/2008/7 that allows windows to be listed, moved, resized, renamed, hidden/unhidden, disabled/enabled, minimized, maximized, restored, activated/inactivated, closed, killed and more.

It is a small standalone executable. It does not create any temporary files, nor does it write to the registry. There is no installation procedure, just run it. To completely remove all traces of it from your system, delete it.

Cmdow was written with batch file programmers in mind. Particular attention has been paid to Cmdows output making it easy to process with the 'FOR /F' command found in the Windows command processor (cmd.exe).

Cmdow is simple and intuitive to use. To list all its options, type:-

```batch
cmdow /?
```

## Usage<a name="usage"></a>

This form of CMDOW lists window details.

<pre>
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
</pre>

Here is a description of the output fields:-

<pre>
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
</pre>

This form of CMDOW performs a single action typically affecting all windows.

<pre>
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
</pre>

This form of CMDOW performs specified action(s) on the specified window(s).
Some of these commands allow you to manipulate windows in ways not normally
possible. Improper use may cause unexpected results and system instability.

<pre>
  CMDOW window {[/ACT] [/INA] [/ENA] [/DIS] [/VIS] [/HID] [/MIN] [/MAX] [/RES]}
  CMDOW window {[/REN newcaption] [/TOP] [/NOT] [/MOV left top]
        [/SIZ width height] [/CLS] [/END]}

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
  /REN    Renames the caption of the specified window to the caption specified
          after /REN. Use double quotes for captions containing whitespace.
  /TOP    Makes the specified window always on top. It will remain on top even
          when another window is activated. If two or more windows have this
          attribute set, the topmost window will be that which had the
          attribute set first.
  /NOT    Makes window not always on top. This is the normal setting for most
          windows.
  /MOV    Moves the specified window. The left and top arguments specify the
          new position of the left top corner of the window. For level one
          windows this is relative to the desktop, and for child windows this
          is relative to its parent window. Level one windows can be moved off
          the screen using negative coords or coords greater than the desktop
          size. You should not move a minimized or maximized window without
          first restoring it.
  /SIZ    Resizes the specified window. The width and height arguments specify
          the new absolute width and height. In general, windows cannot be made
          smaller using /SIZ than they could if using conventional methods. You
          should not resize a minimized or maximized window without first
          restoring it.
  /CLS    Politely asks the specified window to close. Most applications will
          respond to this request, although some may display a message box
          asking you what to do with unsaved work.
  /END    Kills the process associated with the specified window, the result is
          all windows with the same Pid are also killed. Use very carefully.

  If more than one window matches the specified caption, CMDOW only acts on the
  first one. To override this add the /DBM (Don't Blame Me) switch - DANGEROUS.
  Multiple actions are performed in order, from left to right. Eg:-

    CMDOW 0x0E0144 /hid /ren "10% complete" /mov 0 0 /siz 300 100 /act /vis
</pre>

This form of CMDOW executes the specified file or opens it using the
application associated with the filename extension.

<pre>
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
</pre>

## Examples<a name="examples"></a>

View the Cmdow main help screen:-

```batch
cmdow /?
```

View help about the /MOV (move a window) option:-

```batch
cmdow /? /mov
```

To list details of all windows:-

```batch
cmdow
```

To list details of only the windows listed on the taskbar:-

```batch
cmdow /t
```

To list details and positions of only the windows listed on the taskbar:-

```batch
cmdow /t /p
```

To list details of a particular window:-

```batch
cmdow "untitled - notepad" or Cmdow 0x010052
```

where 0x010052 is the window handle of the window titled "untitled - notepad".

Tile all windows vertically:-

```batch
cmdow /tv
```

Bearing in mind that the Cmdow actions are carried out in the order in which they are specified (that is, from left to right), this example restores, moves, renames and finally activates Calc.exe:-

```batch
cmdow Calculator /res /mov 100 200 /ren "New Caption" /act
```

Batch file to activate a different window every 10 seconds:-

```batch
@echo off
:loop
cmdow /AT
ping 127.0.0.1 -n 11 >nul
goto :loop
```

Batch file to close all windows listed on the taskbar:-

```batch
@echo off
:: Hide this console window so its not shown on taskbar
cmdow @ /hid
for /f %%a in ('cmdow /t') do cmdow %%a /cls
:: Now close this console window
cmdow @ /cls
```

Stupid batch file to remove all the buttons from Calculator:-

```batch
@echo off
:: run calc and give it time to fully load
start calc & ping 127.0.0.1 -n 2 >nul
:: hide windows at level 2 and whose image is calc
for /f "tokens=1-2,8" %%a in ('cmdow') do (
  if /i "%%c"=="calc" if "%%b"=="2" cmdow %%a /hid
)
```

Batch file to retrieve display resolution. The co-ords of the first window listed by Cmdow is the screen resolution (this window also has a level of zero which can be identifed using FOR /F):-

```batch
@echo off
for /f "tokens=2,10-11" %%a in ('cmdow /p') do (
  if "%%a"=="0" set "WIDTH=%%b" & set "HEIGHT=%%c"
)
echo Resolution is %WIDTH%x%HEIGHT%
```

Run a program hidden:-

```batch
cmdow /run /hid myprog.exe
```

Run a batch file hidden passing it parameters:-

```batch
cmdow /run /hid mybat arg1 "arg 2"
```

Batch file to alert Administrator if the number of windows shown on the taskbar changes (as might be the case when an application or the operating system generates an error message, or may be backup software is prompting for a tape etc). Loops every 60 seconds until number of windows changes.

```batch
@echo off&setlocal&set LAST=
cmdow @ /hid
:loop
ping 127.0.0.1 -n 61 >nul & set "THIS=0"
for /f %%a in ('cmdow /t /b') do set /a THIS+=1
if not defined LAST set "LAST=%THIS%"
if %THIS% NEQ %LAST% (goto :alert) else (set LAST=%THIS%)
goto :loop
:alert
net send administrator Change in windows on taskbar.
cmdow @ /vis
```

Creating an Autorun CD. Copy your autorun.inf file and cmdow.exe to the root of the CD. Here is a sample autorun.inf. It also shows how add a context menu for the CD. This could be used to install software required by your CD or to view a readme file etc:-

```text
[autorun]
open=cmdow /run /max \video.mpg
icon=myicon.ico
shell\readme=Read &Me
shell\readme\command=cmdow /run \readme.htm
shell\install\=&Install Realplayer
shell\install\command=rp8-complete2-u1-setup.exe
```

Here is another use suggested by a Cmdow user:-

> Cmdow can be used to save the window status and restore it, after the execution of a program. For example, if you are displaying information in a maximised Command Prompt window and then need to call a program (NOTEPAD, perhaps), which gives a second window, the current Command Prompt window is minimised to the taskbar. You would normally need to click on it to restore the original window and give it the focus. You can use CMDOW to save the status of the window, and cause it to be restored automatically in a maximised form, using the following:

```batch
:: save the current window status
for /f "tokens=4" %%a in ('cmdow @ /b') do set wstate=%%a
:: call NOTEPAD to display this BATch file (or anything else!)
call notepad "%~f0"
:: (it is assumed that the user now closes the NOTEPAD window)
:: if the window status was maximised previously, return it to that state
if "%wstate%"=="Max" cmdow @ /max
echo Here we are again, back in our maximised window!
```

## FAQs<a name="faqs"></a>

How can I filter on image name and caption?

> See [issue 5](https://github.com/ritchielawrence/cmdow/issues/5).

How can I filter on multiple window properties, for example, visible explorer.exe windows wider than 1000 pixels?

> See [issue 4](https://github.com/ritchielawrence/cmdow/issues/4).

How can I disable or hide the desktop?

> Running Cmdow without any options lists all windows. The window at the top of the list is the desktop window only as far Windows is concerned (and has a level of 0). However, the window that you and I know as the desktop is found at the end of the list. It will typically be the last window listed with a level of 1 (and by default its caption is "Program Manager").
So running Cmdow "Program Manager" /hid will hide the deskop. If you have another window titled "Program Manager", then you'll need to use an alternative method. See the example scripts.

---

On a W2K machine running a number of applications that monitor our servers and networks, I have a batch file that runs Cmdow /AT every 20 seconds. Sometimes Cmdow fails to activate an application window, instead the taskbar flashes blue.

> This is a new 'feature' of W2K. Its known as Foreground Lock Timeout, and basically prevents another application setting the foreground window. You can safely disable this feature using TweakUI or by setting the value of this registry key to zero:-

<pre>
  HKEY_CURRENT_USER\Control Panel\Desktop\ForegroundLockTimeout
</pre>

---

When I use Cmdow at a command prompt or in a batch file and try to change the console title. Why does the title revert back when Cmdow finishes or when the batch file ends?

> This has been fixed in version 1.4.1.

---

How can I undo the effects of tiling, cascading or minimizing all windows?

> In version 1.4.2 the /UM switch has been superseded by /UW. This new switch will undo the effect of tiling/cascading and minimizing all windows. To maintain compatability with existing scripts, the /UM switch can still be used even though it's no longer documented on Cmdow's helpscreen. /UM now has exactly the same effect as /UW.

## Screenshots<a name="screenshots"></a>

![Screenshot of Cmdow](https://raw.githubusercontent.com/ritchielawrence/cmdow/master/cmdow-screenshot0.png)

![Screenshot of Cmdow](https://raw.githubusercontent.com/ritchielawrence/cmdow/master/cmdow-screenshot1.png)

![Screenshot of Cmdow](https://raw.githubusercontent.com/ritchielawrence/cmdow/master/cmdow-screenshot2.png)

![Screenshot of Cmdow](https://raw.githubusercontent.com/ritchielawrence/cmdow/master/cmdow-screenshot3.png)

![Screenshot of Cmdow](https://raw.githubusercontent.com/ritchielawrence/cmdow/master/cmdow-screenshot4.png)

![Screenshot of Cmdow](https://raw.githubusercontent.com/ritchielawrence/cmdow/master/cmdow-screenshot5.png)

## Revisions<a name="revisions"></a>

Revision | Date | Changes
---|---|---
1.4.8 | 2014-12-30 | Help page typos corrected. Output can handle 5-digit PIDs.
1.4.7 | 2014-12-29 | Wildcards can now be used to filter window captions. Also now possible to perform actions on multiple windows. Handle display is extended to eight digits. Level is dynamically sized. Class name is dynamically sized. Newlines in the caption are converted to 1.4.4 | 2014-11-08 | Cmdow is now open source software released under the MIT license.
1.4.3 | 2004-12-19 | Fixed bug where renaming the current console window would fail if the new caption title contained whitespace. Sizes are based on the entire window list, not just what is displayed. Help pages updated. Internally: Remove process name length test, as WideCharToMultiByte already restricts it. Combined multiple printfs. Substituted some if/else assignments for ternary operator.
1.4.2 | 2003-08-18 | /UM seperseded by /UW switch. See FAQ's for details.
1.4.1 | 2003-05-14 | Cmdow can now rename the console window that launched it. Previously the title would revert when Cmdow exited.
1.4 | 2002-11-07 | Added /TOP and /NOT options to allow the setting/clearing of windows 'always on top'. Fixed argument parsing bug for /MOV and /REN options.
1.3 | 2002-11-20 | Added /FS and /WM options to allow switching between full screen mode and window mode.
1.2 | 2002-10-29 | Fixed issue caused when conole windows have no title. Enhance /AT feature to restore an activated window if it was minimized.
1.1 | 2002-02-25 | Added /RUN command.
1.0 | 2001-12-20 | Cmdow created.

## Copyright and License<a name="copyright-and-license"></a>

Code and documentation copyright 2001-2014 Ritchie Lawrence. Code released under [MIT License](https://github.com/ritchielawrence/cmdow/blob/master/LICENSE.txt)
