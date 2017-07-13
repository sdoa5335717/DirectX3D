#ifndef DEFINES_H
#define DEFINES_H
#include <Windows.h>

#define GM_INVALID -1
#define GM_OK		 0
#define GM_FAIL		 1

// Light type defines.
#define LIGHT_POINT        1
#define LIGHT_DIRECTIONAL  2
#define LIGHT_SPOT         3


#define WinHWND HWND

typedef long VextexType;

enum PrimType
{
	NULL_TYPES,
	POINT_LIST,
	TRIANGLE_LIST,
	TRIANGLE_STRIP,
	TRIANGLE_FAN,
	LINE_LIST,
	LINE_STRIP
};
// color defines
#define UGPCOLOR_ARGB(a, r, g, b) ((unsigned long)((((a)&0xff)<<24)|\
									(((r)&0xff)<<16)|(((g)&0xff)<<8)|\
									((b)&0xff))
#endif