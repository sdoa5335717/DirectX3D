#ifndef D3DRENDERER_H
#define D3DRENDERER_H
#include "defines.h"

#include "RenderInterface.h"

#pragma comment(lib, "d3d9")
#pragma comment(lib, "d3dx9")



class CD3DRenderer : public CRenderInterface
{
public:
	CD3DRenderer();
	~CD3DRenderer();

	BOOL Initialize(int w, int h, WinHWND mainWin, BOOL fullScreen);

	void ShutDown();

	void SetClearColor(float r, float g, float b);

	void StartRender(BOOL bColor, BOOL bDepth, BOOL bStencil);

	void ClearBuffers(BOOL bColor, BOOL bDepth, BOOL bStencil);

	void EndRendering();

	void CalculateProjMatrix(float fov, float n, float f);

	void CalculateOrthoMatrix(float n, float f);

	int CreateStaticBuffer(VextexType, PrimType, UINT totalVerts, UINT totalIndices, int stride, void** data, UINT* indices, UINT* staticID);

	int Render(UINT staticID);

private:
	void OneTimeInit();

private:
	D3DCOLOR m_Color;
	LPDIRECT3D9 m_pDirect3D;
	LPDIRECT3DDEVICE9 m_pDevice;
	BOOL m_bRenderingScene;

	stD3DStaticBuffer *m_staticBufferList;

	int m_numStaticBuffers;
	int m_numActiveStaticBuffer;
};
#endif