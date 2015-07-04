#include "header.h"

extern "C" {
	WINBASEAPI DWORD WINAPI	SetConsoleDisplayMode(HANDLE, DWORD, LPDWORD);
}


int main(int argc, char* argv[])
{
	struct ARGS args;				/* Holds commandline options */
	struct WLIST wlist;				/* First record of linked list of windows */
	struct WLIST *w;				/* Pointer to linked list of windows */
	BOOL done;						/* Flag to signal whether task is done */
	BOOL found;						/* Flag to signal if window was found by its caption */
	int i;							/* General counter */
	void (*fp) (WLIST *, ARGS *);	/* Function pointer */
	OSVERSIONINFO VerInf;			/* O/S version info */
	DWORD res;						// Used by SetConsoleDisplayMode()
	HWND hWnd;
	wlist.next = NULL; // init wlist

	//
	// parse and load the commandline options into the args struct
	//
	ParseArgs(argc, argv, &args);

	//
	// If O/S not (NT4/W2K or greater) then only allow /RUN or /? command
	//
	VerInf.dwOSVersionInfoSize = sizeof(VerInf);
	GetVersionEx(&VerInf);

	if(
		(!(VerInf.dwPlatformId >= VER_PLATFORM_WIN32_NT) &&
		(VerInf.dwMajorVersion >= 4)) &&  ((*args.tasks != RUN) && (*args.tasks != HELP))
	) Quit(VERERR);

	//
	// The tasks in this switch statement are single (one off) tasks
	//
	done = TRUE;
	switch(*args.tasks) {
		case HELP:
			ShowHelp(args.helpcmd);
			break;
		case CW:
			//CascadeWindows(NULL, 0, NULL, 0, NULL);
			hWnd = FindWindow("Shell_TrayWnd", "");
			if(hWnd) PostMessage(hWnd, WM_COMMAND, WM_CASCADE_WINDOWS, 0);
			break;
		case TV: // tile vertically
			//TileWindows(NULL, MDITILE_VERTICAL, NULL, 0, NULL);
			hWnd = FindWindow("Shell_TrayWnd", "");
			if(hWnd) PostMessage(hWnd, WM_COMMAND, WM_TILE_VERTICALLY, 0);
			break;
		case TH: // tile horizontally
			//TileWindows(NULL, MDITILE_HORIZONTAL, NULL, 0, NULL);
			hWnd = FindWindow("Shell_TrayWnd", "");
			if(hWnd) PostMessage(hWnd, WM_COMMAND, WM_TILE_HORIZONTALLY, 0);
			break;
		case AT: // alt tab
			AltTab();
			break;
		case MA: // minimize all
			//keybd_event(VK_LWIN, 0, 0, 0);
			//keybd_event(77, 0, 0, 0); // 77 is 'M'
			//keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
			hWnd = FindWindow("Shell_TrayWnd", "");
			if(hWnd) PostMessage(hWnd, WM_COMMAND, WM_MINIMIZE_ALL, 0);
 			break;
		case UW: // undo minimize all
			//keybd_event(VK_LWIN, 0, 0, 0);
			//keybd_event(VK_SHIFT, 0, 0, 0);
			//keybd_event(77, 0, 0, 0);
			//keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
			//keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
			//break;
			hWnd = FindWindow("Shell_TrayWnd", "");
			if(hWnd) PostMessage(hWnd, WM_COMMAND, 416, 0);
			break;
		case FS: // switch to full screen mode
			SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), FULL_SCREEN_MODE, &res);
			break;
		case WM: // switch to window mode
			SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), WINDOW_MODE, &res);
			break;
		case LISTALL:
			GetWindowList(&wlist); // create the linked list of windows
			w = &wlist;
			while(w) {
				LstWin(w, &args);
				w = w->next;
			}
			break;
		case RUN:
			RunWin(&wlist, &args);
			break;
		default: done = FALSE;
	}

	//
	// all tasks onwards operate on a single window (may be multiple tasks)
	//
	if(!done) {
		//
		// if handle specified, load the info for it and init the WLIST pointer
		//
		if(args.hwnd) {
			GetWindowInf(args.hwnd, &wlist);
			w = &wlist;
		}
		//
		// if handle not specified then create list of windows and search for caption
		//
		else {
			//
			// GetWindowList also loads the info for each window
			//
			GetWindowList(&wlist);
			found = FALSE;
			w = &wlist;
			while(w) {
				if(!lstrcmpi(w->caption, args.caption)) {
					found = TRUE;
					break; // w now pointing at found record
				}
				w = w->next;
			}
			if(!found) Quit(NOTFND);
		}

		//
		// perform all the tasks in the task queue on the specified window
		//
		i = 0;
		while(args.tasks[i]) {
			switch(args.tasks[i++]) {
				case RCW: fp = RenWin; break;
				case LST: fp = LstWin; break;
				case MIN: fp = MinWin; break;
				case MAX: fp = MaxWin; break;
				case RES: fp = ResWin; break;
				case ACT: fp = ActWin; break;
				case ENA: fp = EnaWin; break;
				case INA: fp = InaWin; break;
				case DIS: fp = DisWin; break;
				case HID: fp = HidWin; break;
				case VIS: fp = VisWin; break;
				case REN: fp = RenWin; break;
				case MOV: fp = MovWin; break;
				case SIZ: fp = SizWin; break;
				case CLS: fp = ClsWin; break;
				case END: fp = EndWin; break;
				case TOP: fp = TopWin; break;
				case NOT: fp = NotWin; break;
				default: fp = 0;
			}
			//
			// check if window still exists and also update its info, as it could be out
			// of date by now, eg window could have been moved/resized
			//
			if(!GetWindowInf(w->hwnd, w)) Quit(NOTFND);

			//
			// perform task on specified window
			//
			fp(w, &args);
		}
	}
	return(0);
}
