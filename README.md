# Mtee

## Table of Contents

* [Synopsis](#synopsis)
* [Usage](#usage)
* [Examples](#examples)
* [FAQs](#faqs)
* [Screenshots](#screenshots)
* [Revisions](#revisions)
* [Copyright and License](#copyright-and-license)

## Synopsis<a name="synopsis"></a>

Mtee is a Win32 console application that sends any data it receives to stdout and to any number of files. Useful if you want to watch and record the output from a batch file or program. It can also prefix each line of output with a timestamp.

Mtee is a 17kb standalone executable. It does not create any temporary files or write to the registry. There is no installation procedure, just run it. To remove all traces of Mtee from your system, just delete it.

Mtee is simple to use and only has several options. To list them, type:-

```batch
mtee /?
```

## Usage<a name="usage"></a>

<pre>
  MTEE [/A | /U] [/C] [/D] [/T] [[/+] file] [...]

  /A    Convert output to ANSI. Default output is same as input.
  /C    Continue if errors occur opening/writing to file (advanced users only).
  /D    Prefix each line of output with local date in YYYY-MM-DD format.
  /T    Prefix each line of output with local time in HH:MM:SS.MSS format.
  /U    Convert output to UNICODE. Default output is same as input.
  /+    Append to existing file. If omitted, existing file is overwritten.
  file  File to receive the output. File is overwritten if /+ not specified.
  ...   Any number of additional files. Use /+ before each file to append.

  Example usage:-

  1) script.cmd | mtee result.txt
  2) ftp -n -s:ftp.scr | mtee local.log /+ \\server\logs$\remote.log
  3) update.cmd 2>&1 | mtee/d/t/+ log.txt

  1) Sends the output of script.cmd to the console and to result.log. If
     result.txt already exists, it will be overwritten.
  2) Sends output of automated ftp session to the console and two log files,
     local.log is overwritten if it already exists, remote.log is appended to.
  3) Redirects stdout and stderr from update.cmd to console and appends to
     log.txt. Each line is prefixed with local date and time.
</pre>

## Examples<a name="examples"></a>

View Mtee help screen:-

```batch
mtee/?
````

Send the output of script.cmd to the console and to RESULT.LOG. If RESULT.LOG already exists, it will be overwritten:-

```batch
script.cmd | mtee result.log
```

Send the output of the automated ftp session to the console and to two log files, LOCAL.LOG is overwritten if it already exists. REMOTE.LOG is appended to if it exists, otherwise it is created:-

```batch
ftp -n -s:ftp.scr | mtee local.log /+ \\server\logs\remote.log
```

Make two copies of LOG whilst viewing LOG on the screen. If NEW1 and NEW2 already exist, they are overwritten:-

```batch
mtee < log new1 new2
```

Redirect stdout and stderr from UPDATE.CMD to the console and appends to LOG.TXT. Each line is prefixed with local date and time:-

```batch
update.cmd 2>&1 | mtee/d/t/+ log.txt
```

Send the output from BACKUP.CMD to the console and two remote log files. If there is an error opening any of the log files (server offline for instance) MTEE will continue. If the destination files already exist, they are appended to:-

```batch
backup.cmd | mtee /c/+ \\svr1\log$\bu.log /+ \\svr2\logs$\bu.log
```

Make multiple carbon copies of patch.exe:-

```batch
type patch.exe|mtee \\pc1\c$\patch.exe \\pc2\c$\patch.exe \\pc3\c$\patch.exe
```

Make a unicode log of HFNETCHK:-

```batch
hfnetchk|mtee/u log
```

Display stdout on the console, and stderr on the console and also to a log file with each line of stderr prefixed with local date and time:-

```batch
batch.cmd 2>&1 1>&3 3>&1 |mtee/t/d log
```

## FAQs<a name="faqs"></a>

How can I determine the exit code of the process piped into Mtee?

> The next release of Mtee (v2.3) will have a new option to instruct Mtee to exit with the exit code of the process that was piped into it.

## Screenshots<a name="screenshots"></a>

![Screenshot of Mtee](https://raw.githubusercontent.com/ritchielawrence/mtee/master/mtee-screenshot1.png)

## Revisions<a name="revisions"></a>

Revision | Date | Changes
---|---|---
2.2 | 2016-06-10 | Credit to Jari Kulmala for implementing workaround to avoid possible bug in Windows 10 where program takes 30 seconds to exit.
2.1 | 2013-03-01 |Mtee is now open source software released under the MIT License. Credit to Jari Kulmala for addressing the following:-<ul><li>mtee is now Windows 8 compatible</li><li>mtee assumed all files < 4GB</li><li>echo "t013\|mtee /u con" entered a continuous loop</li><li>"echo x x x x \| mtee" caused mtee to guess input was unicode</li><li>redirection to console and con device as output file was not supported</li></ul>
2.0 | 2003-08-27 | The following features are new to Mtee v2.0<br>Read and output unicode<br>Convert ANSI to unicode (and vice-versa)<br>Reads text and binary data without performing any character translations<br>Support for unicode filenames of ~32,000 characters<br>Smaller than ever. Mtee is now just 11kb (and no, it's not compressed!)

## Copyright and License<a name="copyright-and-license"></a>

Code and documentation copyright 2001-2016 Ritchie Lawrence. Code released under [MIT License](https://github.com/ritchielawrence/mtee/blob/master/LICENSE.txt)
