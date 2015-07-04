#include "header.h"

#ifdef __MINGW32__
int _CRT_glob = 0;
#endif

typedef DWORD (WINAPI *SetConsoleDisplayMode_)(HANDLE, DWORD, LPDWORD);


int main(int argc, char* argv[])
{
	struct ARGS args;				/* Holds commandline options */
	struct WLIST wlist;				/* First record of linked list of windows */
	struct WLIST *w;				/* Pointer to linked list of windows */
	BOOL done;						/* Flag to signal whether task is done */
	BOOL found;						/* Flag to signal if window was found by its caption */
	int i;							/* General counter */
	void (*fp) (WLIST *, ARGS *);	/* Function pointer */
	DWORD res = FULL_SCREEN_MODE;	// Used by SetConsoleDisplayMode()
	SetConsoleDisplayMode_ SCDM;
	HWND hWnd;
	wlist.next = NULL; // init wlist
	int wcount = 0; // keep track of how many windows acted upon

	//
	// parse and load the commandline options into the args struct
	//
	ParseArgs(argc, argv, &args);

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
		case WM: // switch to window mode
			res = WINDOW_MODE;
		case FS: // switch to full screen mode
			SCDM = (SetConsoleDisplayMode_)GetProcAddress(GetModuleHandle("kernel32.dll"), "SetConsoleDisplayMode");
			if(SCDM) SCDM(GetStdHandle(STD_OUTPUT_HANDLE), res, &res);
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
	// all tasks onwards may operate on multiple windows (may be multiple tasks)
	//
	if(!done) {
		WLIST wtlist, *wt = &wtlist;
		wtlist.next = NULL;
		//
		// if handle specified, load the info for it and init the WLIST pointer
		//
		if(args.hwnd) {
			GetWindowInf(args.hwnd, &wlist);
			wtlist.next = &wlist;
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
			HWND me = GetMyHandle(); // don't match self (use @ for that)
			int len = lstrlen(args.caption);
			int op = 0; // exact match, 1 = left, 2 = right, 3 = in
			if(args.caption[len-1] == '*') {
				args.caption[--len] = '\0';
				op = 1;
			}
			if(*args.caption == '*') {
				++args.caption;
				--len;
				op |= 2;
			}
			while(w) {
				found = FALSE;
				if(op == 0) {
					if(!lstrcmpi(w->caption, args.caption)) {
						found = TRUE;
					}
				}
				else if(w->hwnd != me) {
					int s = lstrlen(w->caption);
					if (s >= len) {
						if(op == 1) {
							if(!(CompareString(GetThreadLocale(),
											   NORM_IGNORECASE,
											   w->caption, len,
											   args.caption, len) - 2)) {
								found = TRUE;
							}
						}
						else if(op == 2) {
							if(!lstrcmpi(w->caption+s-len, args.caption)) {
								found = TRUE;
							}
						}
						else /*(op == 3)*/ {
							for(s -= len; s >= 0; --s) {
                                // CompareString function reference:-
                                // http://msdn.microsoft.com/en-gb/library/windows/desktop/dd317759%28v=vs.85%29.aspx
								if(!(CompareString(GetThreadLocale(),
												   NORM_IGNORECASE,
												   w->caption + s, len,
												   args.caption, len) - 2)) {
									found = TRUE;
								}
							}
						}
					}
				}

				if(found) {
					wt = wt->next = w;
					w = w->next;
					wt->next = NULL;
				} else w = w->next;
			}
			if(!wtlist.next) Quit(NOTFND);
		}

		//
		// perform all the tasks in the task queue on the specified window(s)
		//
		w = wtlist.next;
		do {
            // only act on more than one window if the /DBM (DONTBLAMEME) switch is used
            if(wcount & (!(args.actopts && DONTBLAMEME)) ) break;
            if(args.tasks[0] != LST) wcount++; // keep track of windows acted upon, ignore listing a window
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
			w = w->next;
		} while (w);
	}
	return(0);
}
