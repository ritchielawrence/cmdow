//#ifdef _MERGE_RDATA_
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <shellapi.h>
#include <winperf.h>
#include <conio.h>

//
#define WM_CASCADE_WINDOWS		(403)
#define WM_TILE_HORIZONTALLY	(404)
#define WM_TILE_VERTICALLY		(405)
#define WM_MINIMIZE_ALL			(415)
#define WM_UNDO_WINDOWS			(413)

// Used by Set/GetConsoleDisplayMode functions
#define FULL_SCREEN_MODE	1
#define WINDOW_MODE			0

/* Error messages */
#define MEMERR		0		/* Memory allocation failed */
#define MOVERR		1		/* /MOV command requires left and top arguments */
#define SIZERR		2		/* /SIZ command requires width and height agrguments */
#define RENERR		3		/* /REN command requires a new caption to be specified */
#define UNRARG		4		/* Unrecognized argument(s). Use CMDOW /? for help */
#define CONARG		5		/* Conflicting argument(s). Use CMDOW /? for help */
#define INCARG		6		/* Incorrect argument(s). Use CMDOW /? for help */
#define NOCAPT		7		/* A window must be specified by its caption or handle */
#define NOTFND		8		/* The specified window was not found */
#define BADIMG		9		/* Unable to retrieve image names */
#define TASKOL		10		/* Too many tasks have been specified */
#define EXEERR		11		/* Unable to execute/open specified file */
#define VERERR		12      /* Only the /? and /RUN commands are supported on W95/98/ME platforms */



#define MAXTASKS	50		/* Maximum size of task queue */

enum TASK {
	NONE = 0, HELP, LST, LISTALL,
	CW, TV, TH, MA, UW, AT, FS, WM,
	ACT, INA,
	DIS, ENA,
	HID, VIS,
	MAX, MIN, RES,
	REN, MOV, SIZ,
	CLS, END, RUN,
	TOP, NOT,
	RCW
};

enum LISTOPTS {
	SHOWTB		= 0x0001,			/* Only list windows on the taskbar */
	SHOWPOS		= 0x0002,			/* Include window position (coords) in the list */
	FULLCAPT	= 0x0004,			/* Don't truncate window captions to fit console screen */
	BARE		= 0x0008			/* Bare format (no heading information) */
};

enum ACTOPTS {
	DONTBLAMEME = 0x0001			/* Allows actions to be performed on more than one window */
};

struct ARGS {
	char			*caption;		/* Caption to list, or operate on */
	char			*newcapt;		/* New caption, used with /REN */
	char			*exename;		/* Path/name of this executable */
	int				cc;				/* If to determine if caption is ZLS or NULL string */
	HWND			hwnd;			/* Window handle to operate on */
	HWND			myhwnd;			/* Handle of this console window */
	enum TASK		tasks[MAXTASKS];/* Operations to perform on specified window */
	int				listopts;		/* Options for listing windows */
	int 			actopts;		/* Options for actions upon selected windows */
	int				left, top;		/* Holds coords for moving window */
	int				width, height;	/* Used for resizing a window */
	char			*file;			/* File to open/run, used with /RUN */
	char			*params;		/* Commandline params used with /RUN */
	int				sw_state;		/* How window is displayed, used with /RUN */
	char			*helpcmd;		/* Holds command that user wants help with */
};

struct WLIST {
	HWND			hwnd;			/* Handle of this window */
	HWND			owner;			/* Handle to owner of this window */
	HWND			parent;			/* Handle to parent of this window */
	LONG			styles;			/* Styles of this window */
	LONG			exstyles;		/* Extended styles of this window */
	int 			level;			/* Level (desktop=0, toplevel=1, child=2) */
	DWORD			pid;			/* Process ID of this window */
	char			*caption;		/* Caption of this window */
	char			*image;			/* Name of executable */
	struct WLIST	*next;			/* Pointer to next structure */
	int				width, height;  /* Width and height of window */
	int				top, left;		/* Coords of top left corner of window */
};

/* args.c functions */
void ParseArgs(int argv, char *argc[], struct ARGS *args);
enum TASK PushTask(enum TASK tasks[], enum TASK t);
enum TASK PopTask(enum TASK tasks[]);
int IsTask(enum TASK tasks[], enum TASK t);
char *LoadString(char **dest, const char *src);
HWND atoHandle(const char *);
int atoCoord(const char *);
char *GetArgs();
char *GetRestCmdline(char *Cmd);

/* help.c functions */
void ShowHelp(char *);
void Quit(const int Err);

/* tlist.c functions */
//BOOL EnableDebugPrivNT(void);
//void GetTaskList(struct TLIST *);
char *GetImageName(long);

/* wlist.c functions */
HWND GetMyHandle(void);
BOOL IsTaskbarWindow(struct WLIST *w);

void GetWindowList(struct WLIST *w);
BOOL CALLBACK GetWindowListProc(HWND, LPARAM);
BOOL GetWindowInf(HWND, struct WLIST *); // get info about window from its handle

void PrintWindowInfHeadings(BOOL showpos, BOOL fullcapt);
void PrintWindowInf(struct WLIST *w, BOOL showpos, BOOL showtb);
void AltTab(void);
void SetFGWindow(HWND);
void MaxWin(struct WLIST *w, struct ARGS *a);
void MinWin(struct WLIST *w, struct ARGS *a);
void ResWin(struct WLIST *w, struct ARGS *a);
void ActWin(struct WLIST *w, struct ARGS *a);
void EnaWin(struct WLIST *w, struct ARGS *a);
void InaWin(struct WLIST *w, struct ARGS *a);
void DisWin(struct WLIST *w, struct ARGS *a);
void HidWin(struct WLIST *w, struct ARGS *a);
void VisWin(struct WLIST *w, struct ARGS *a);
void RenWin(struct WLIST *w, struct ARGS *a);
void MovWin(struct WLIST *w, struct ARGS *a);
void SizWin(struct WLIST *w, struct ARGS *a);
void LstWin(struct WLIST *w, struct ARGS *a);
void ClsWin(struct WLIST *w, struct ARGS *a);
void EndWin(struct WLIST *w, struct ARGS *a);
void RunWin(struct WLIST *w, struct ARGS *a);
void TopWin(struct WLIST *w, struct ARGS *a);
void NotWin(struct WLIST *w, struct ARGS *a);

int __cdecl _ConvertCommandLineToArgcArgv( void );
