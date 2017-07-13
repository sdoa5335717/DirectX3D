#ifndef RENDERINTERFACE_H
#define RENDERINTERFACE_H

#ifdef GAMEENGINE_EXPORTS
#define GAMEENGINE_API __declspec(dllexport)
#else
#define GAMEENGINE_API __declspec(dllimport)
#endif

#include "stdafx.h"
#include "defines.h"
#include "light.h"
#include "material.h"
struct stD3DStaticBuffer
{

	LPDIRECT3DVERTEXBUFFER9 vbPtr;
	LPDIRECT3DINDEXBUFFER9	ibPtr;

	int numVerts;
	int numIndices;
	int stride;
	unsigned long fvf;
	PrimType primType;

	stD3DStaticBuffer():vbPtr(0),ibPtr(0),numVerts(0),
		numIndices(0), stride(0), fvf(0),
		primType(NULL_TYPES) {}
};


// Render engine interface
class CRenderInterface
{
public:
	CRenderInterface() : m_nSrceenWidth(0),
		m_nScreenHeight(0), m_fNear(0), m_fFar(0) {}
	virtual ~CRenderInterface() {};
	// initialize render engine
	virtual BOOL Initialize(int w, int h, WinHWND mainWin, BOOL fullScreen) = 0;
	// ����һ�Σ���������ͶӰ����
	virtual void OneTimeInit() = 0;
	// �ر������Ⱦϵͳ
	virtual void ShutDown() = 0;
	// ���ú�̨������ɫ
	virtual void SetClearColor(float r, float g, float b) = 0;

	virtual void StartRender(BOOL bColor, BOOL bDepth, BOOL bStencil) = 0;

	virtual void ClearBuffers(BOOL bColor, BOOL bDepth, BOOL bStencil) = 0;

	virtual void EndRendering() = 0;
	// ����ͶӰ����

	virtual void CalculateProjMatrix(float fov, float n, float f) = 0;
	// ������������
	virtual void CalculateOrthoMatrix(float n, float f) = 0;

	virtual int CreateStaticBuffer(VextexType, PrimType, UINT totalVerts, 
		UINT totalIndices, int stride, void** data, UINT* indices, UINT* staticID) = 0;

	virtual int Render(UINT staticID) = 0;

	// ���ò���,����
	virtual void SetMaterial(stMaterial* mat) = 0;
	virtual void SetLight(stLight* light, int index) = 0;
	virtual void DisableLight(int index) = 0;

protected:
	int m_nSrceenWidth, m_nScreenHeight;
	BOOL m_fullScreen;

	WinHWND m_hMainWnd;

	float m_fNear, m_fFar;
};
GAMEENGINE_API BOOL CreateD3DRenderer(CRenderInterface** pObj);
#endif