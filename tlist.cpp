#include "header.h"

#define TITLE_SIZE          64
#define PROCESS_SIZE        MAX_PATH
#define INITIAL_SIZE        51200
#define EXTEND_SIZE         25600
#define REGKEY_PERF         "software\\microsoft\\windows nt\\currentversion\\perflib"
#define REGSUBKEY_COUNTERS  "Counters"
#define PROCESS_COUNTER     "process"
#define PROCESSID_COUNTER   "id process"
#define UNKNOWN_TASK        "Unknown"

struct TLIST {
    DWORD			dwProcessId;
    CHAR			ProcessName[PROCESS_SIZE];
	struct TLIST	*next;
};

int GetTaskList(struct TLIST *tlist)
{
    DWORD                        rc;
    HKEY                         hKeyNames;
    DWORD                        dwType;
    DWORD                        dwSize;
    LPBYTE                       buf = NULL;
    CHAR                         szSubKey[1024];
    LANGID                       lid;
    LPSTR                        p;
    LPSTR						 p2;
    PPERF_DATA_BLOCK             pPerf;
    PPERF_OBJECT_TYPE            pObj;
    PPERF_INSTANCE_DEFINITION    pInst;
    PPERF_COUNTER_BLOCK          pCounter;
    PPERF_COUNTER_DEFINITION     pCounterDef;
    DWORD                        i;
    DWORD                        dwProcessIdTitle;
    DWORD                        dwProcessIdCounter;
    CHAR                         szProcessName[MAX_PATH];
    DWORD                        dwNumTasks;

    // this returns "009" on my system
	lid = MAKELANGID( LANG_ENGLISH, SUBLANG_NEUTRAL );
    wsprintf( szSubKey, "%s\\%03x", REGKEY_PERF, lid );

	// Get handle to "khlm\sw\ms\winnt\cv\perflib\009"
    rc = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                       szSubKey,
                       0,
                       KEY_READ,
                       &hKeyNames
                     );
    if (rc != ERROR_SUCCESS) {
        goto exit;
    }

    // get the buffer size for the counter value
    rc = RegQueryValueEx( hKeyNames, REGSUBKEY_COUNTERS, NULL, &dwType, NULL, &dwSize);

    if (rc != ERROR_SUCCESS) goto exit;

    // allocate the counter names buffer
    buf = (LPBYTE) malloc(dwSize);

	if (buf == NULL) goto exit;

    memset( (void *) buf, 0, dwSize );

    // read the counter names from the registry
    rc = RegQueryValueEx(hKeyNames, REGSUBKEY_COUNTERS,NULL, &dwType, buf, &dwSize);

    if (rc != ERROR_SUCCESS) {
        goto exit;
    }

    p = (char *) buf;

	dwProcessIdTitle = 0; // added by me as dwProcessIdTitle may be used without init

    while (*p) {
        if (p > (char *) buf) {
            for( p2=p-2; isdigit(*p2); p2--) ;
            }
        if (lstrcmpi(p, PROCESS_COUNTER) == 0) {
            for( p2=p-2; isdigit(*p2); p2--) ;
            lstrcpy( szSubKey, p2+1 );
        }
        else
        if (lstrcmpi(p, PROCESSID_COUNTER) == 0) {
            for( p2=p-2; isdigit(*p2); p2--) ;
            dwProcessIdTitle = atol( p2+1 );
        }
        p += (lstrlen(p) + 1);
    }
    free( buf );

    dwSize = INITIAL_SIZE;
    buf = (LPBYTE) malloc( dwSize );
    if (buf == NULL) {
        goto exit;
    }
    memset( buf, 0, dwSize );

    while (TRUE) {
        rc = RegQueryValueEx( HKEY_PERFORMANCE_DATA,szSubKey,NULL,&dwType,buf,&dwSize);
        pPerf = (PPERF_DATA_BLOCK) buf;
        if ((rc == ERROR_SUCCESS) &&
            (dwSize > 0) &&
            (pPerf)->Signature[0] == (WCHAR)'P' &&
            (pPerf)->Signature[1] == (WCHAR)'E' &&
            (pPerf)->Signature[2] == (WCHAR)'R' &&
            (pPerf)->Signature[3] == (WCHAR)'F' ) {
            break;
        }
        if (rc == ERROR_MORE_DATA) {
            dwSize += EXTEND_SIZE;
            buf = (LPBYTE) realloc( buf, dwSize );
            memset( buf, 0, dwSize );
        }
        else {
            goto exit;
        }
    }

	dwProcessIdCounter = 0; /* added by me as var could be used without init */

    pObj = (PPERF_OBJECT_TYPE) ((DWORD)pPerf + pPerf->HeaderLength);
    pCounterDef = (PPERF_COUNTER_DEFINITION) ((DWORD)pObj + pObj->HeaderLength);
    for (i=0; i<(DWORD)pObj->NumCounters; i++) {
        if (pCounterDef->CounterNameTitleIndex == dwProcessIdTitle) {
            dwProcessIdCounter = pCounterDef->CounterOffset;
            break;
        }
        pCounterDef++;
    }

    dwNumTasks = (DWORD) pObj->NumInstances;

    pInst = (PPERF_INSTANCE_DEFINITION) ((DWORD)pObj + pObj->DefinitionLength);

    for (i=0; i<dwNumTasks; i++) {

        p = (LPSTR) ((DWORD)pInst + pInst->NameOffset);
        rc = WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)p, -1, szProcessName,
			sizeof(szProcessName), NULL, NULL);

        if (!rc) {
            lstrcpy( tlist->ProcessName, UNKNOWN_TASK );
        }

		// load process name into tlist
		lstrcpy( tlist->ProcessName, szProcessName );

		// load Pid into tlist
	    pCounter = (PPERF_COUNTER_BLOCK) ((DWORD)pInst + pInst->ByteLength);
        tlist->dwProcessId = *((LPDWORD) ((DWORD)pCounter + dwProcessIdCounter));
        pInst = (PPERF_INSTANCE_DEFINITION) ((DWORD)pCounter + pCounter->ByteLength);

		// create a new tlist record
		tlist->next = (struct TLIST *) HeapAlloc(GetProcessHeap(), 0, sizeof(struct TLIST));
		if(!(tlist->next)) return(1);

		tlist = tlist->next; // point to the new record
		tlist->next = NULL; // in case this is last record, null serves as a flag
		// note, last record of tlist will be undefined except next which is NULL
    }
	return(0);
exit:
    if(buf) free(buf);
    RegCloseKey( hKeyNames );
    RegCloseKey( HKEY_PERFORMANCE_DATA );
	return(1);
}

char *GetImageName(long pid)
{
	static int init;
	static struct TLIST tlist;
	struct TLIST *t;
	static char Unknown[] = "Unknown";

	if(!init++) {
		tlist.next = NULL;
		if(GetTaskList(&tlist)) return NULL;
		// build list of tasks
	}

	t = &tlist;
	while(t->next) {
		if(t->dwProcessId == (DWORD) pid) return t->ProcessName;
		t = t->next;
	}
	return Unknown;
}
