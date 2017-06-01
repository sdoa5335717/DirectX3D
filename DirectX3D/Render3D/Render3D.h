#ifndef RENDER3D_H
#define RENDER3D_H

#ifdef RENDER3D_EXPORTS
#define RENDER3D_API __declspec(dllexport)
#else
#define RENDER3D_API __declspec(dllimport)
#endif

#include <d3d9.h>
#include <d3dx9tex.h>

struct Vertex_3D
{
  float x, y, z;
  float u, v;
  DWORD colour;
  enum 
  {
            FVF = D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_DIFFUSE|D3DFVF_NORMAL,
  };
};

class RENDER3D_API CRender3D {
public:
	CRender3D(void);
	// 初始化D3D引擎
	virtual BOOL initial_render(HWND hView);
	// 设定相机位置
	virtual BOOL set_camera() { return TRUE; };
	// 设定投影变换
	virtual BOOL set_projection() { return TRUE; };
	// 设置光照
	virtual BOOL set_lighting(FLOAT fX, FLOAT fY, FLOAT fZ){ return TRUE; };

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	D3DLIGHT9	 InitLight(D3DLIGHTTYPE type, D3DXVECTOR3* direction, D3DXCOLOR* color);

	// 渲染需要继承重载
	virtual BOOL render(){ return TRUE; };

	IDirect3DDevice9* m_pDevice;

	HWND m_hView;
};

#endif
