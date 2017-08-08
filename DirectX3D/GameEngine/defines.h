#ifndef DEFINES_H
#define DEFINES_H
#include <Windows.h>

#define GM_INVALID -1

#define GM_OK		 1
#define GM_FAIL		 0

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
									((b)&0xff)))


#define STRAND_SAFE_RELEASE(p) if(p) p->Release(); p = NULL;

#define STRAND_SAFE_DELETE_ARRAY(p) if(p) {\
									delete[] p;\
									p = NULL;}

#define STRAND_SAFE_DELETE(p)		if(p) {\
									delete p;\
									p = NULL;}
// 一些渲染状态
enum RenderState
{
	CULL_NONE,
	CULL_CW,
	CULL_CCW,
	DEPTH_NONE,
	DEPTH_READONLY,
	DEPTH_READWRITE,
	SHADE_POINTS,
	SHADE_SOLIDTRI,
	SHADE_WIRETRI,
	SHADE_WIREPOLY,
	TRANSPARENCY_NONE,     // 禁用透明度
	TRANSPARENCY_ENABLE    // 启用透明度
};



// 透明度渲染状态值
enum TransState
{
	TRANS_ZERO = 1,
	TRANS_ONE,
	TRANS_SRCCOLOR,
	TRANS_INVSRCCOLOR,
	TRANS_SRCALPHA,
	TRANS_INVSRCALPHA,
	TRANS_DSTALPHA,
	TRANS_INVDSTALPHA,
	TRANS_DSTCOLOR,
	TRANS_INVDSTCOLOR,
	TRANS_SRCALPHASAT,
	TRANS_BOTHSRCALPHA,
	TRANS_INVBOTHSRCALPHA,
	TRANS_BLENDFACTOR,
	TRANS_INVBLENDFACTOR
};



// 纹理过滤器的过滤模式
enum TextureState
{
	MIN_FILTER,   // 缩小率
	MAG_FILTER,   // 放大率
	MIP_FILTER    // mipmap纹理级别
};



// 纹理过滤器类型
enum FilterType
{
	POINT_TYPE,        // 最近点采样
	LINEAR_TYPE,       // 线性纹理过滤
	ANISOTROPIC_TYPE   // 各向异性纹理过滤
};

// Multi-sampling.
enum UGP_MS_TYPE
{
	UGP_MS_NONE,
	UGP_MS_SAMPLES_2,
	UGP_MS_SAMPLES_4,
	UGP_MS_SAMPLES_8,
	UGP_MS_SAMPLES_16
};

// Fog.
enum UGP_FOG_TYPE
{
	UGP_VERTEX_FOG,
	UGP_PIXEL_FOG
};


// 用于确定GUI元素在屏幕上的位置
#define PERCENT_OF(a, b) (int)(a * b)

// GUI控件矩形的灵活顶点格式
#define GUI_FVF      2

// GUI控件的灵活顶点格式
#define D3DFVF_GUI (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// GUI控件类型
#define UGP_GUI_STATICTEXT 1    // 静态文本控件
#define UGP_GUI_BUTTON     2    // 按钮控件
#define UGP_GUI_BACKDROP   3    // 背景图控件
#define UGP_GUI_FPS 4
// 鼠标的状态
#define UGP_BUTTON_UP      1    // 左键在按钮上弹起
#define UGP_BUTTON_OVER    2    // 停靠在按钮上
#define UGP_BUTTON_DOWN    3    // 左键在按钮上按下

//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }


#define KEYS_SIZE 256

#endif