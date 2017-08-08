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
// һЩ��Ⱦ״̬
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
	TRANSPARENCY_NONE,     // ����͸����
	TRANSPARENCY_ENABLE    // ����͸����
};



// ͸������Ⱦ״ֵ̬
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



// ����������Ĺ���ģʽ
enum TextureState
{
	MIN_FILTER,   // ��С��
	MAG_FILTER,   // �Ŵ���
	MIP_FILTER    // mipmap������
};



// �������������
enum FilterType
{
	POINT_TYPE,        // ��������
	LINEAR_TYPE,       // �����������
	ANISOTROPIC_TYPE   // ���������������
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


// ����ȷ��GUIԪ������Ļ�ϵ�λ��
#define PERCENT_OF(a, b) (int)(a * b)

// GUI�ؼ����ε������ʽ
#define GUI_FVF      2

// GUI�ؼ��������ʽ
#define D3DFVF_GUI (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// GUI�ؼ�����
#define UGP_GUI_STATICTEXT 1    // ��̬�ı��ؼ�
#define UGP_GUI_BUTTON     2    // ��ť�ؼ�
#define UGP_GUI_BACKDROP   3    // ����ͼ�ؼ�
#define UGP_GUI_FPS 4
// ����״̬
#define UGP_BUTTON_UP      1    // ����ڰ�ť�ϵ���
#define UGP_BUTTON_OVER    2    // ͣ���ڰ�ť��
#define UGP_BUTTON_DOWN    3    // ����ڰ�ť�ϰ���

//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }


#define KEYS_SIZE 256

#endif