#include "header.h"

void ParseArgs(int argc, char *argv[], struct ARGS *a)
{
	register int i;
	int form1;	/* count of form1 options - list window(s) */
	int form2;	/* count of form2 options - operation performed on all windows */
	int form3;	/* count of form3 options - operation performed on selected windows */
	int form4;	/* count of form4 options - run/open a program/file */
	int form5;  /* count of form5 options - rename this console window 7*/
	int fcount;	/* count of number of forms used */

	//
	// init the args struct
	//
	a->caption = NULL;
	a->newcapt = NULL;
	a->file = NULL;
	a->params = NULL;
	a->sw_state = SW_SHOWNORMAL;
	a->hwnd = (HWND) 0;
	*a->tasks = (TASK) 0;
	a->listopts = 0;
	a->actopts = 0;
	a->cc = 0;
	a->helpcmd = NULL;
	a->myhwnd = GetMyHandle();
	LoadString(&a->exename, argv[0]);

	//
	// if no args default task is to list all windows
	//
	if(argc == 1) {
		PushTask(a->tasks, LISTALL);
		return;
	}

	//
	// grab every arg and push onto task queue/set options
	//
	for(i=1;i<argc;++i) {
		if(!lstrcmpi("/?", argv[i])) {
			//
			// see if user wants help on particular command
			//
			if(argc != 2) LoadString(&a->helpcmd, argv[++i]);
			PushTask(a->tasks, HELP);
			return;
		}
		else if(!lstrcmpi("/B", argv[i])) a->listopts |= BARE;
		else if(!lstrcmpi("/F", argv[i])) a->listopts |= FULLCAPT;
		else if(!lstrcmpi("/P", argv[i])) a->listopts |= SHOWPOS;
		else if(!lstrcmpi("/T", argv[i])) a->listopts |= SHOWTB;
		else if(!lstrcmpi("/CW", argv[i])) PushTask(a->tasks, CW);
		else if(!lstrcmpi("/TV", argv[i])) PushTask(a->tasks, TV);
		else if(!lstrcmpi("/TH", argv[i])) PushTask(a->tasks, TH);
		else if(!lstrcmpi("/MA", argv[i])) PushTask(a->tasks, MA);
		else if(!lstrcmpi("/UM", argv[i])) PushTask(a->tasks, UW); // maintain /um for compatability with old scripts
		else if(!lstrcmpi("/UW", argv[i])) PushTask(a->tasks, UW);
		else if(!lstrcmpi("/AT", argv[i])) PushTask(a->tasks, AT);
		else if(!lstrcmpi("/FS", argv[i])) PushTask(a->tasks, FS);
		else if(!lstrcmpi("/WM", argv[i])) PushTask(a->tasks, WM);
		else if(!lstrcmpi("/DBM", argv[i])) a->actopts |= DONTBLAMEME;
		else if(!lstrcmpi("/MIN", argv[i])) PushTask(a->tasks, MIN);
		else if(!lstrcmpi("/MAX", argv[i])) PushTask(a->tasks, MAX);
		else if(!lstrcmpi("/RES", argv[i])) PushTask(a->tasks, RES);
		else if(!lstrcmpi("/ACT", argv[i])) PushTask(a->tasks, ACT);
		else if(!lstrcmpi("/INA", argv[i])) PushTask(a->tasks, INA);
		else if(!lstrcmpi("/ENA", argv[i])) PushTask(a->tasks, ENA);
		else if(!lstrcmpi("/DIS", argv[i])) PushTask(a->tasks, DIS);
		else if(!lstrcmpi("/HID", argv[i])) PushTask(a->tasks, HID);
		else if(!lstrcmpi("/VIS", argv[i])) PushTask(a->tasks, VIS);
		else if(!lstrcmpi("/END", argv[i])) PushTask(a->tasks, END);
		else if(!lstrcmpi("/CLS", argv[i])) PushTask(a->tasks, CLS);
		else if(!lstrcmpi("/TOP", argv[i])) PushTask(a->tasks, TOP);
		else if(!lstrcmpi("/NOT", argv[i])) PushTask(a->tasks, NOT);
		else if(!lstrcmpi("/MOV", argv[i])) {
			PushTask(a->tasks, MOV);
			//
			// next two args MUST be the new left and top coords
			//
			if( (i+2) < argc) {
				a->left = atoi(argv[++i]);
				a->top = atoi(argv[++i]);
				if( (!a->left) && lstrcmp(argv[i-1], "0") ) Quit(MOVERR);
				if( (!a->top) && lstrcmp(argv[i], "0") ) Quit(MOVERR);
				}
			else Quit(MOVERR);
			}
		else if(!lstrcmpi("/SIZ", argv[i])) {
			//
			// next two args MUST be the new width and height
			//
			PushTask(a->tasks, SIZ);
			if( (i+2) < argc) {
				a->width = atoi(argv[++i]);
				a->height = atoi(argv[++i]);
				if( (!a->width) && lstrcmp(argv[i-1], "0") ) Quit(SIZERR);
				if( (!a->height) && lstrcmp(argv[i], "0") ) Quit(SIZERR);
				}
			else Quit(SIZERR);
			}
		else if(!lstrcmpi("/REN", argv[i])) {
			//
			// next arg MUST be the new caption
			//
			PushTask(a->tasks, REN);
			if( (i+1) < argc) LoadString(&a->newcapt, argv[++i]);
			else Quit(RENERR);
			}
		else if(!lstrcmpi("/_RENAME_MYCONSOLE_WINDOW", argv[i])) {
			//
			// next arg MUST be the new caption
			//
			PushTask(a->tasks, RCW);
			if( (i+1) < argc) LoadString(&a->newcapt, argv[++i]);
			else Quit(RENERR);
			}
		else if(!lstrcmpi("/RUN", argv[i])) {
			PushTask(a->tasks, RUN);
			//
			// point to next arg and see if show state has been specced
			//
			if( !( ( (i++) +1 ) < argc)) Quit(INCARG);
			if(!lstrcmpi("/MIN", argv[i])) a->sw_state = SW_SHOWMINIMIZED;
			else if(!lstrcmpi("/MAX", argv[i])) a->sw_state = SW_SHOWMAXIMIZED;
			else if(!lstrcmpi("/HID", argv[i])) a->sw_state = SW_HIDE;
			else --i;
			//
			// next arg MUST be file to open/run
			//
			if( (i+1) < argc) LoadString(&a->file, argv[++i]);
			else Quit(INCARG);
			//
			// if anymore args, get a pointer to remainder of commandline
			//
			if( (i+1) < argc) {
				LoadString(&a->params, GetRestCmdline(argv[i]));
				i = argc; // prevent any more iterations of for loop
			}
		}
		else {
			//
			// got this far, so assume unrecognized arg is a caption
			// keep track of how many 'unknown' args. One unknown is assumed to be a caption
			//
			++a->cc;
			if(i != 1) Quit(UNRARG); // caption must be first arg
			if(!lstrcmpi("@", argv[i])) a->hwnd = a->myhwnd; // @ signifies this cmd prompt
			else {
				LoadString(&a->caption, argv[i]);
				a->hwnd = atoHandle(a->caption); // try and convert caption to a windows handle
			}
		}
	}

	//
	// now check for valid combinations of args
	//
	form5 = IsTask(a->tasks, RCW);

	form4 = IsTask(a->tasks, RUN); // run/open program/file

	form3 = ( // op on single window
		IsTask(a->tasks, MIN) + IsTask(a->tasks, MAX) + IsTask(a->tasks, RES) +
		IsTask(a->tasks, ACT) + IsTask(a->tasks, INA) + IsTask(a->tasks, ENA) +
		IsTask(a->tasks, DIS) + IsTask(a->tasks, HID) + IsTask(a->tasks, VIS) +
		IsTask(a->tasks, END) + IsTask(a->tasks, CLS) + IsTask(a->tasks, REN) +
		IsTask(a->tasks, MOV) + IsTask(a->tasks, SIZ) + IsTask(a->tasks, TOP) +
		IsTask(a->tasks, NOT)
	);

	form2 = ( // op on all windows
		IsTask(a->tasks, CW) + IsTask(a->tasks, TV) + IsTask(a->tasks, TH) +
		IsTask(a->tasks, MA) + IsTask(a->tasks, UW) + IsTask(a->tasks, AT) +
		IsTask(a->tasks, FS) + IsTask(a->tasks, WM)
	);

	// if forms 5 to 2 not used or any listopts used, then form1 in use
	if( ((form5 + form4 + form3 + form2) == 0) || a->listopts ) form1 = 1;
	else form1 = 0;

	fcount = 0;
	if(form5) ++fcount;
	if(form4) ++fcount;
	if(form3) ++fcount;
	if(form2) ++fcount;
	if(form1) ++fcount;

	// make sure only one, and only one form has been used
	if(fcount != 1) Quit(CONARG);

	if(form5) {
	;
	}
	else if(form4) {
	;
	}
	else if(form3) { // op on single window
		if(!a->cc) Quit(NOCAPT);
	}
	else if(form2) { // op on all windows, one arg and no caption
		if(a->cc || (form2 > 1) ) Quit(INCARG);
	}
	else { // form1. If caption do a LST else LISTALL
		if( (a->listopts & SHOWTB) && (a->cc) ) Quit(INCARG);
		if(a->cc) PushTask(a->tasks, LST);
		else PushTask(a->tasks, LISTALL);
	}
}

enum TASK PushTask(enum TASK tasks[], enum TASK t)
{
	register int i;
	i = 0;

	//
	// goto last task plus one, null terminate task queue, then add task to queue
	//
	while(tasks[i++]);
	if(i == MAXTASKS) Quit(TASKOL);
	tasks[i] = NONE;
	return tasks[--i] = t;
}

enum TASK PopTask(enum TASK tasks[])
{
	register int i;
	enum TASK t;
	i = 0;

	while(tasks[i++]);
	if(!i) return NONE;
	i--;
	t = tasks[i];
	tasks[i] = NONE;
	return t;
}

int IsTask(enum TASK tasks[], enum TASK t)
{
	register int i;
	i = 0;

	while(tasks[i]) if(tasks[i++]==t) return(1);
	return(0);
}

HWND atoHandle(const char *src)
{
	char *p;
	HWND h;

	// string must begin with 0x or 0X
	if( strncmp("0x", src, 2) && strncmp("0X", src, 2) ) return (HWND) (0);
	h = (HWND) strtoul(src, &p, 16);
	// all characters must be interpreted
	if(*p) return (HWND) (0);
	else return h;
}

int atoCoord(const char *src)
{
	int i;
	i = 0; atoi(src);
	return i;
}


char *LoadString(char **dest, const char *src)
{
	*dest = (char *) HeapAlloc(GetProcessHeap(), 0, sizeof(char) * (lstrlen(src) + 1));
	if(!*dest) Quit(MEMERR);
	lstrcpy(*dest, src);
	return(*dest);
}

void Quit(const int Err)
{
	char szBuff[1024] = "Error: ";
	const char *msg[] = {
//       --1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
		"Memory allocation failed",											/* MEMERR 0 */
		"/MOV command requires left and top arguments",						/* MOVERR 1 */
		"/SIZ command requires width and height agrguments",				/* SIZERR 2 */
		"/REN command requires a new caption to be specified",				/* RENERR 3 */
		"Unrecognized argument(s). Use CMDOW /? for help",					/* UNRARG 4 */
		"Incompatible argument(s). Use CMDOW /? for help",					/* CONARG 5 */
		"Incorrect argument(s). Use CMDOW /? for help",						/* INCARG 6 */
		"A window must be specified by its caption or handle", 				/* NOCAPT 7 */
		"The specified window was not found",								/* NOTFND 8 */
		"Unable to retrieve image names",									/* BADIMG 9 */
		"Too many tasks have been specified",								/* TASKOL 10 */
		"Unable to execute/open specified file",							/* EXEERR 11 */
		"Only the /? and /RUN commands are supported on W95/98/ME platforms"/* VERERR 12 */
	};

	DWORD cbWritten;
    int BytesToWrite;
	lstrcat(szBuff, msg[Err]);
	lstrcat(szBuff, ".\n");
	BytesToWrite = lstrlen(szBuff);

	WriteFile(GetStdHandle(STD_ERROR_HANDLE), szBuff, BytesToWrite, &cbWritten, 0);

    ExitProcess(1);
}

char *GetArgs()
{
	static char *SysCmdLine;	/* pointer to system commandline */
    static char *SclPos;		/* pointer to current position of system cmdline */
    static char *Argv;			/* pointer to allocated mem, holds one arg at a time */
	char *ArgvPos;				/* pointer to current position of argv memory */
	int i;						/* general purpose counter */
	int Quotes;


	//
	// if !SysCmdLine, initialise the static vars
	//
	if(!SysCmdLine) {
		SysCmdLine = SclPos = GetCommandLine();

		//
		// allocate enough memory to hold the longest argument
		//
		i = lstrlen(SysCmdLine);
		Argv = (char *) HeapAlloc(GetProcessHeap(), 0, i+1);
		if(!Argv) Quit(MEMERR);
	}

	//
	// init ArgvPos and NULL terminate Argv
	//
	ArgvPos = Argv;
	*ArgvPos = 0;

	while(1) {
		//
		// find start of argument, skipping any whitespace
		//
		while(*ArgvPos && ((' ' == *ArgvPos) || ('\t' == *ArgvPos)) ) ++ArgvPos;

		if(0 == *ArgvPos) return Argv;

		//
		// now work through any quotes, copying every third quote
		//
		Quotes = 0;
		while('"' == *ArgvPos) {
			++ArgvPos;
			++Quotes;
			if(3 == Quotes) {
				Quotes = 0;
				*ArgvPos++ = '"';
			}
		}

		if(0 == *ArgvPos) return Argv;

		//
		// now work through the argument, copying to Argv
		//

	}
	return Argv;
}

//+---------------------------------------------------------------------------
// Function: GetRestCmdline
//
// Synopsis: Finds the first non-whitespace character after specified
//           command.
//
// Arguments: [Cmd] Pointer to cmd to find and skip past
//
// Returns: Pointer to remainder of commandline after specified cmd
//
// Notes:
//
//----------------------------------------------------------------------------
char *GetRestCmdline(char *Cmd)
{
	char *SysCmdLine;
	char *p;

	SysCmdLine = GetCommandLine();
	p = strstr(SysCmdLine, Cmd);

	if(p) {
		p += lstrlen(Cmd); // skip pass the Cmd

		if('"' == *p) p++; // if cmd was quoted, skip past the end quote

		while(*p && ( (' ' == *p) || '\t' == *p) ) p++; // skip whitespace
	}

	return p;
}
