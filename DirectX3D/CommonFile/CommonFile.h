#ifndef COMMONFILE_H
#define COMMONFILE_H

#include <Windows.h>
#include <strsafe.h>
#include <WindowsX.h>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

#define COMMON_USEPRINTF() TCHAR pBuf[1024] = {}
#define COMMON_PRINTF(...) \
	StringCchPrintf(pBuf,1024,__VA_ARGS__);\
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),pBuf,lstrlen(pBuf),NULL,NULL);

#define COMMON_ALLOC(sz)		HeapAlloc(GetProcessHeap(),0,sz)
#define COMMON_CALLOC(sz)		HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sz)
#define COMMON_SAFEFREE(p)		if(NULL != p){HeapFree(GetProcessHeap(),0,p);p=NULL;}
#define COMMON_CLASS_SAFEFREE(p) if(NULL != p){delete p; p=NULL;}
#define COMMON_ASSERT(s) if(!(s)) {::DebugBreak();}

#define chHANDLE_DLGMSG(hWnd, message, fn)                 \
   case (message): return (SetDlgMsgResult(hWnd, uMsg,     \
   HANDLE_##message((hWnd), (wParam), (lParam), (fn))))

#define POINT_DISTANCE(x1,x2,y1,y2) (sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)))



#endif

