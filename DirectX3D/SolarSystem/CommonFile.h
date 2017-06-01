#ifndef COMMONFILE_H
#define COMMONFILE_H

#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <math.h>
#include <vector>
using namespace std;

#define COMMON_USEPRINTF() TCHAR pBuf[1024] = {}
#define COMMON_PRINTF(...) \
	StringCchPrintf(pBuf,1024,__VA_ARGS__);\
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),pBuf,lstrlen(pBuf),NULL,NULL);

#define COMMON_ALLOC(sz)		HeapAlloc(GetProcessHeap(),0,sz)
#define COMMON_CALLOC(sz)		HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sz)
#define COMMON_SAFEFREE(p)		if(NULL != p){HeapFree(GetProcessHeap(),0,p);p=NULL;}
#define COMMON_CLASS_SAFEFREE(p) if(NULL != p){delete p; p=NULL;}
#define PI 3.14159265358979323846264338327950238f
typedef struct shape_point
{
	FLOAT fX;
	FLOAT fY;
}FPoint;

#endif

