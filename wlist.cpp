#include "header.h"

static HWND  max_handle;
static int	 max_level;
static DWORD max_pid;
static int	 len_handle, len_level, len_pid, len_image;
static int	 len_begin;

//+---------------------------------------------------------------------------
// Function: GetMyHandle
//
// Synopsis: Determines handle of this console window
//
// Arguments: None
//
// Returns: Handle of this console window (which could theoretically be NULL)
//
// Notes:
//
//----------------------------------------------------------------------------
HWND GetMyHandle(void)
{
	return GetConsoleWindow();
}

//+---------------------------------------------------------------------------
// Function: GetWindowList
//
// Synopsis: Causes ALL windows to be enumerated
//
// Arguments: [*w] pointer to WLIST structure
//
// Returns: none
//
// Notes: This function first gets info about the desktop window by a call to
// GetWindowInf, then enumerates all top level windows, with call to
// EnumWindows, the callback function get windows info, then uses recursion to
// enumerate child windows of each toplevel window
//
//----------------------------------------------------------------------------
void GetWindowList(struct WLIST *w)
{
	GetWindowInf(GetDesktopWindow(), w);
	EnumWindows( (WNDENUMPROC) GetWindowListProc, (LPARAM) w);
}

//+---------------------------------------------------------------------------
// Function: GetWindowListProc
//
// Synopsis: This is the callback function defined by EnumWindows in
// GetWindowList. The O/S calls this function with a handle to every toplevel
// window.
//
// Arguments: [hwnd] Window handle supplied by O/S
//            [wptr] pointer to WLIST structure. Application defined parameter
//                   supplied by GetWindowList.
//
// Returns: TRUE to tell O/S its ready for (and wants) the next window handle
//
// Notes: wptr is initially copied into w. Everytime function is called by O/S
// it dynamically creates a new WLIST structure thats linked to the original
// pointer. The w pointer is then aimed at the new struct and info about the
// window is gathered. If the window is a toplevel window (->level==1) then
// a call to EnumChildWindows is made, again using this function as the callback
//
//----------------------------------------------------------------------------
BOOL CALLBACK GetWindowListProc(HWND hwnd, LPARAM wptr)
{
	//
	// onetime init of *w
	//
	static struct WLIST *w = (struct WLIST *) wptr;

	//
	// create a new WLIST struct
	//
	w->next = (struct WLIST *) HeapAlloc(GetProcessHeap(), 0, sizeof(struct WLIST));
	if(!(w->next)) Quit(MEMERR);

	//
	// point to the new struct, then gather window info
	//
	w = w->next;
	GetWindowInf(hwnd, w);

	//
	// make sure 'next' pointer is NULL terminated in case this is the last record.
	//
	w->next = NULL;

	//
	// If this is a toplevel window, then enumerate all its child windows
	//

	if(w->level==1) EnumChildWindows(hwnd, (WNDENUMPROC) GetWindowListProc, 0);

	return TRUE;
}

//+---------------------------------------------------------------------------
// Function: GetWindowInf
//
// Synopsis: Gathers information about a window and stores it in a WLIST struct
//
// Arguments: [hwnd] handle of the window to gather info about
//             [*w] pointer to a WLIST struct
//
// Returns: FALSE if window does not exist, otherwise TRUE
//
// Notes:
//
//----------------------------------------------------------------------------
BOOL GetWindowInf(HWND hwnd, struct WLIST *w)
{
	#define MAX_CLASSNAME_LEN 63
	int len;
	HWND parent;
	POINT pt;
	RECT rect;

	//
	// get the desktop window only once
	//
	static HWND desktop = GetDesktopWindow();

	//
	// check window handle exists
	//
	if(!IsWindow(hwnd)) return FALSE;

	//
	// gather some window info
	//
	w->hwnd = hwnd;
	w->styles = GetWindowLong(hwnd, GWL_STYLE);
	w->exstyles = GetWindowLong(hwnd, GWL_EXSTYLE);
	w->owner = GetWindow(hwnd, GW_OWNER);
	w->parent = parent = GetParent(hwnd);

	//
	// determine windows' level. 0=desktop, 1=toplevel, 2+ = child
	//
	if(hwnd == desktop) w->level = 0;
	else if( (parent == w->owner) || (parent == desktop) ) w->level = 1;
	else {
		w->level = 1;
		while( (parent != NULL) && (parent != desktop) ) {
			w->level++;
			parent = GetParent(parent);
		}
	}

	//
	// Get windows' coords and determine width and height
	//
	GetWindowRect(hwnd, &rect);
	w->left = rect.left;
	w->top = rect.top;
	w->width = rect.right - rect.left;
	w->height = rect.bottom - rect.top;

	//
	// if child window, convert top left coords to position relative to parent window
	//
	if(w->level >1) {
		pt.x = w->left;
		pt.y = w->top;
		ScreenToClient(w->parent, &pt);
		w->left = pt.x;
		w->top = pt.y;
	}

	//
	// get process id
	//
	GetWindowThreadProcessId(hwnd, &w->pid);

	//
	// get the name of the executable that created the window
	//
	w->image = GetImageName(w->pid);

	//
	// Get the window caption if it has one, otherwise use the window's classname
	//
	len = GetWindowTextLength(hwnd);
	if(!len) len = MAX_CLASSNAME_LEN;
	w->caption = (char *) HeapAlloc(GetProcessHeap(), 0, sizeof(char) * (len + 1) );
	if(!(w->caption)) Quit(MEMERR);
	if(GetWindowTextLength(hwnd)) {
		GetWindowText(hwnd, w->caption, len + 1);
		for (char *p = w->caption; *p; ++p) {
			if (*p == '\n') *p = '_';
		}
	}
	else GetClassName(hwnd, w->caption, len + 1);

	return TRUE;
}

static void LstWinLen(struct WLIST *w, BOOL tb)
{
	max_handle = 0;
	max_level = max_pid = len_image = 0;
	while(w) {
		if (!tb || IsTaskbarWindow(w)) {
			if (max_handle < w->hwnd) max_handle = w->hwnd;
			if (max_level < w->level) max_level = w->level;
			if (max_pid < w->pid) max_pid = w->pid;
			int len = lstrlen(w->image);
			if (len_image < len) len_image = len;
		}
		w = w->next;
	}
	len_handle = (DWORD_PTR)max_handle > 0xFFFFFFF ? 8 :
				 (DWORD_PTR)max_handle > 0xFFFFFF ? 7 : 6;
	len_level = (max_level > 99) ? 3 : (max_level > 9) ? 2 : 1;
	len_pid = (max_pid > 9999) ? 5 : 4;
	len_begin = 2 + len_handle + 1 + len_level + 1 + len_pid + 1 + 16;
	if (len_image < 8) len_image = 8;
}

void PrintWindowInfHeadings(BOOL showpos, BOOL fullcapt)
{
	printf("%-*s%*s%*s -Window status- ",
		   len_handle + 2, "Handle",
		   len_level + 1, "Lev",
		   len_pid + (len_level != 1), "Pid");
	//
	// if showing window position and size, then print their headings
	//
	if(showpos) {
		printf("  Left    Top  Width Height ");
		len_begin += 28;
	}

	printf("%-*sCaption\n", fullcapt ? len_image + 1 :
							showpos && len_pid == 5 ? 8 : 9, "Image");
}

void LstWin(struct WLIST *w, struct ARGS *a)
{
	static int headings;
	const char *MinMaxRes, *ActiveInactive, *VisibleHidden, *EnabledDisabled;

	if( (!headings++) ) {
		LstWinLen(w, a->listopts & SHOWTB);
		if( !(a->listopts & BARE) ) PrintWindowInfHeadings(a->listopts & SHOWPOS, a->listopts & FULLCAPT);
	}

	if(a->listopts & SHOWTB)
		if(!IsTaskbarWindow(w)) return;

	MinMaxRes = (w->styles & WS_MINIMIZE) ? "Min" :
				(w->styles & WS_MAXIMIZE) ? "Max" :
											"Res";

	ActiveInactive	= (GetForegroundWindow() == w->hwnd) ? "Act" : "Ina";
	VisibleHidden	= (w->styles & WS_VISIBLE)  ? "Vis" : "Hid";
	EnabledDisabled = (w->styles & WS_DISABLED) ? "Dis" : "Ena";

	printf("0x%.*lX %*i %*lu %s %s %s %s ",
		   len_handle, (unsigned long) w->hwnd,
		   len_level, w->level,
		   len_pid, (unsigned long) w->pid,
	       MinMaxRes, ActiveInactive, EnabledDisabled, VisibleHidden);
	if(a->listopts & SHOWPOS) {
		printf("%6d %6d %6d %6d ", w->left, w->top, w->width, w->height);
	}
	if(a->listopts & FULLCAPT) {
		printf("%-*s %s\n", len_image, w->image, w->caption);
	}
	else {
		printf("%-8.8s %.*s\n",
			   w->image, 79 - 9 - len_begin, w->caption);
	}
}

//+---------------------------------------------------------------------------
// Function: IsTaskbarWindow
//
// Synopsis: Determines if specified is shown on the taskbar
//
// Arguments: [*w] Pointer to a WLIST struct holding information about the
// window by a call to GetWindowInfo()
//
// Returns: TRUE if window is on the taskbar, FALSE if not
//
// Notes: I couldn't find any documentation on this but it appears a window is
// shown on the taskbar if: -
//	> its not hidden AND
//	> does not have WS_EX_TOOLWINDOW style AND
//	> does not have an owner AND
//  > is level 1
//
//----------------------------------------------------------------------------
BOOL IsTaskbarWindow(struct WLIST *w)
{
	if( (!(w->styles & WS_VISIBLE)) ||
		(w->exstyles & WS_EX_TOOLWINDOW) ||
		w->owner || (w->level != 1) ) return FALSE;
	else return TRUE;
}

//+---------------------------------------------------------------------------
// Function: SetFGWindow
//
// Synopsis: Set the foreground window
//
// Arguments: [hwnd] handle to window to become foreground window
//
// Returns: None
//
// Notes: On W2K, if HKCU\Control Panel\Desktop\ForegroundLockTimeout != 0, as
//        by default, then the O/S will not allow a background window to set
//        the foreground window. I've worked around this by first activating this
//        window, which by being the foreground window gets permission from the
//        O/S to set another foreground window
//
//----------------------------------------------------------------------------
void SetFGWindow(HWND hwnd)
{
	int Res;
	DWORD FLT;								/* ForegroundLockTimeout */

	//
	// Read HKCU\Control Panel\Desktop\ForegroundLockTimeout, Res != 0 if read
	//
	Res = SystemParametersInfo(0x2000, 0, (PVOID) &FLT, 0x0000);

	//
	// if FLT was read and is not zero, then activate this window first, so it gets
	// permission to set a new foreground window
	//
	if(Res && FLT) SetForegroundWindow(GetMyHandle());
	SetForegroundWindow(hwnd);
}


//
// Maximize window, if window hidden it will unhide itself, if disabled, stays disabled
//
void MaxWin(struct WLIST *w, struct ARGS *a)
{
	HWND tmp;

	//
	// just in case maximized window gets focus, activate window that had focus
	//
	tmp = GetForegroundWindow();
	ShowWindowAsync(w->hwnd, SW_MAXIMIZE);
	if(tmp != GetForegroundWindow()) SetFGWindow(tmp);
}

//
// Minimize window, if window hidden it will become visible, if disabled, stays disabled
//
void MinWin(struct WLIST *w, struct ARGS *a)
{
	HWND tmp;
	//
	// just in case maximized window gets focus, activate window that had focus
	//
	tmp = GetForegroundWindow();
	ShowWindowAsync(w->hwnd, SW_MINIMIZE);
	if(tmp != GetForegroundWindow()) SetFGWindow(tmp);
}

//
// Restore
//
void ResWin(struct WLIST *w, struct ARGS *a)
{
	HWND tmp;
	tmp = GetForegroundWindow();
	// if was min, then res may make it active
	// win was max, then min - then res will max it, res again will restore it
	ShowWindowAsync(w->hwnd, SW_SHOWNOACTIVATE);
	if(tmp != GetForegroundWindow()) SetFGWindow(tmp);
}

//
// Activate the window, if window is hidden, it gets focus but stays hidden,
//
void ActWin(struct WLIST *w, struct ARGS *a)
{
	SetFGWindow(w->hwnd);
}

//
// Inactivate window
//
void InaWin(struct WLIST *w, struct ARGS *a)
{
	//
	// only activate another window if specified window actually has focus
	//
	if(w->hwnd == GetForegroundWindow()) AltTab();
}


//
// Enable window
//
void EnaWin(struct WLIST *w, struct ARGS *a)
{
	EnableWindow(w->hwnd, TRUE);
}

//
// Disable window
//
void DisWin(struct WLIST *w, struct ARGS *a)
{
	EnableWindow(w->hwnd, FALSE);
}

//
// Hide the window
//
void HidWin(struct WLIST *w, struct ARGS *a)
{
	ShowWindowAsync(w->hwnd, SW_HIDE);
}

//
// Make the window visible
//
void VisWin(struct WLIST *w, struct ARGS *a)
{
	ShowWindowAsync(w->hwnd, SW_SHOW);
}

//
// politely ask the window to close
//
void ClsWin(struct WLIST *w, struct ARGS *a)
{
	PostMessage(w->hwnd, WM_CLOSE, 0, 0);
}

//
// End process associated with window (all windows with same Pid are killed!!!
//
void EndWin(struct WLIST *w, struct ARGS *a)
{
	HANDLE hProcess;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, w->pid);
	if (hProcess) {
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, w->pid);
		if (hProcess == NULL) {
			return;
		}
		if(!TerminateProcess(hProcess, 1)) {
			CloseHandle(hProcess);
			return;
		}
		CloseHandle(hProcess);
		return;
	}
}

//
// Rename window to new caption
//
void RenWin(struct WLIST *w, struct ARGS *a)
{
	//
	// If being asked to rename this console window, then launch CMDOW as a detached
	// process with appropriate arguments
	//
	if(w->hwnd != a->myhwnd) {
		SetWindowText(w->hwnd, a->newcapt);
		return;
	}

	STARTUPINFO sInfo;

	sInfo.cb              = sizeof(STARTUPINFO);
	sInfo.lpReserved      = NULL;
	sInfo.lpDesktop       = NULL;
	sInfo.lpTitle         = NULL;
	sInfo.dwFlags         = 0;
	sInfo.dwFillAttribute = 0;
	sInfo.wShowWindow     = SW_SHOW;
	sInfo.cbReserved2     = 0;
	sInfo.lpReserved2     = NULL;

	int i;
	char buf[20];
	char *cmdline;

	//
	// Build the commandline to rename this console window
	//
	ltoa((long) a->myhwnd, buf, 16);

	i = lstrlen(a->exename);
	i += lstrlen(" 0x");
	i += lstrlen(buf);
	i += lstrlen(" /_RENAME_MYCONSOLE_WINDOW ");
	i += lstrlen(a->newcapt);

	// 2004-12-19 added line below
	i += lstrlen("\"\"");


	cmdline = (char *) HeapAlloc(GetProcessHeap(), 0, sizeof(char) * (i + 1));
	if(!cmdline) Quit(MEMERR);

	lstrcpy(cmdline, a->exename);
	lstrcat(cmdline, " 0x");
	lstrcat(cmdline, buf);
	lstrcat(cmdline, " /_RENAME_MYCONSOLE_WINDOW ");
	// 2004-12-19 added line below
	lstrcat(cmdline, "\"");
	lstrcat(cmdline, a->newcapt);
	// 2004-12-19 added line below
	lstrcat(cmdline, "\"");

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof pi);

	//BOOL retval;
	//retval =
	CreateProcess(
		NULL,
		cmdline,
		NULL,
		NULL,
		FALSE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&sInfo,
		&pi
	);
	return;
}

//
// Move window to new top left coords
//
void MovWin(struct WLIST *w, struct ARGS *a)
{
	MoveWindow(w->hwnd, a->left, a->top,	w->width, w->height, TRUE);
}

//
// Resize window, to new height and width
//
void SizWin(struct WLIST *w, struct ARGS *a)
{
	MoveWindow(w->hwnd, w->left, w->top, a->width, a->height, TRUE);
}

//
// Run
//
void RunWin(struct WLIST *w, struct ARGS *a)
{
	#define SHELLEXEC_SUCCESS 33
	int RetVal;
	// FindExecutable() Retrieves the name of and handle to the executable (.exe) file associated with the specified file name
	/*STARTUPINFO si;
	PROCESS_INFORMATION pi;
	char buf[1024];
	HMODULE hModule;


	hModule = GetModuleHandle(a->cmdline);
    GetModuleFileName(hModule, buf, 1024);

	FindExecutable(a->cmdline, NULL, buf);


	ZeroMemory(&si, sizeof(si));

	//si.dwFlags = STARTF_USESHOWWINDOW;
	//si.wShowWindow = SW_HIDE;
	si.lpTitle = "Rascal";

	CreateProcess(
		NULL,						// appname
		a->cmdline,					// cmdline
		NULL,						// process security attribs
		NULL,						// thread security attribs
		TRUE,						// inherit handles
		0,							// creation flags
		NULL,						// new environment block
		NULL,						// current directory name
		&si,						// startupinfo
		&pi							// processinfo
	);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);*/
//	printf("file [%s], params [%s]\n", a->file, a->params);

	RetVal = (int) ShellExecute(NULL, NULL, a->file, a->params, NULL, a->sw_state);
	if(RetVal < SHELLEXEC_SUCCESS) Quit(EXEERR);
}

//+---------------------------------------------------------------------------
// Function: AltTab
//
// Synopsis: Activates taskbar window with the lowest Z order. In the event of
//           there being no taskbar windows, the desptop is activated.
//
// Arguments: None
//
// Returns: None
//
// Notes: User would typically use this feature to scroll through all windows
//        on the taskbar (ie with a time delay between each call). By activating
//        the lowest window, this moves to the top of the Z order, and all other
//        move down one. If this function was to activate the next window in the
//        Z order, then repeated calls to this function would only alternate
//        between two windows (as happens when repeatly pressing [Alt]-[Tab])
//
//----------------------------------------------------------------------------
void AltTab(void)
{
	HWND hwnd, me;
	struct WLIST w;
	long styles;
	//
	// a handle to any toplevel window is required, so may as well use this
	// consoles' handle, as guaranteed to exist
	//
	me = GetMyHandle();

	//
	// now get a toplevel window with the lowest Z order, and collect its info
	//
	hwnd = GetWindow(me, GW_HWNDLAST);
	GetWindowInf(hwnd, &w);

	//
	// search up the Z order for first window on the taskbar, and collect its info
	//
	while( (!IsTaskbarWindow(&w)) && hwnd) {
		hwnd = GetNextWindow(hwnd, GW_HWNDPREV);
		GetWindowInf(hwnd, &w);
	}

	//
	// hwnd maybe NULL if there weren't any taskbar windows, in which case do nothing
	//
	if(hwnd) SetFGWindow(hwnd);

	//
	// if window is minimized then restore it
	//
	styles = GetWindowLong(hwnd, GWL_STYLE);

	if(styles & WS_MINIMIZE) {
		w.hwnd = hwnd;
		ResWin(&w, NULL);
	}

}

void TopWin(struct WLIST *w, struct ARGS *a)
{
	SetWindowPos(w->hwnd, HWND_TOPMOST, 0, 0, 0, 0,
		SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE
	);
}

void NotWin(struct WLIST *w, struct ARGS *a)
{
	SetWindowPos(w->hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
		SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE
	);
}

