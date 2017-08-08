#ifndef RENDERINTERFACE_H
#define RENDERINTERFACE_H


#include "stdafx.h"
#include "defines.h"
#include "light.h"
#include "material.h"
#include "GUI.h"
#include "structs.h"
#include "EngineInterface.h"
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
	virtual BOOL Initialize(int w, int h, WinHWND mainWin, BOOL fullScreen, UGP_MS_TYPE ms) = 0;
	// ����һ�Σ���������ͶӰ����
	virtual void OneTimeInit() = 0;
	// �ر������Ⱦϵͳ
	virtual void ShutDown() = 0;
	// ���ú�̨������ɫ
	virtual void SetClearColor(float r, float g, float b) = 0;
	// ������Ⱦ
	virtual void StartRender(BOOL bColor, BOOL bDepth, BOOL bStencil) = 0;
	// ������
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

	// ����͸����
	virtual void SetTransparency(RenderState state,
		TransState src, TransState dst) = 0;

	

	// �������������
	virtual void SetTextureFilter(int index, int filter,
		int val) = 0;

	// ���ö�������ͼ
	virtual void SetMultiTexture() = 0;

	// Ӧ������
	virtual void ApplyTexture(int index, int texId) = 0;

	// ������Ļ��ͼ
#ifdef UNICODE
	virtual void SaveScreenShot(TCHAR *file) = 0;
	// ���2D����
	virtual int AddTexture2D(TCHAR *file, int *texId) = 0;
#else 
	virtual void SaveScreenShot(char *file) = 0;
	// ���2D����
	virtual int AddTexture2D(char *file, int *texId) = 0;
#endif
	// ���õ�״sprite
	virtual void EnablePointSprites(float size, float min,
		float a, float b, float c) = 0;

	// ���õ�״sprite
	virtual void DisablePointSprites() = 0;

	// �����µ�GUI���󣬲����ö�����ӵ�������
	bool CreateGUI(int &id)
	{
		// �״δ���GUIϵͳCGUISystem����ʱ������һ������1��Ԫ�صĶ�������
		if (!m_guiList)
		{
			// m_guiList��ʼ��Ϊ��̬�����Ķ�������
			m_guiList = new CGUISystem[1];

			if (!m_guiList)
			{
				return GM_FAIL;
			}
		}
		else
		{
			// ��ʱ��GUIϵͳ����ָ�룬���ڱ��浱ǰ���������ֵ
			CGUISystem *temp;

			// ָ��̬���ٵ�GUIϵͳ��������
			temp = new CGUISystem[m_totalGUIs + 1];

			// ��m_guiList���������ȫ�����ݸ��Ƶ�temp��
			memcpy(temp, m_guiList, sizeof(CGUISystem) * m_totalGUIs);

			delete[] m_guiList;  // ����m_guiList��������
			m_guiList = temp;    // m_guiList���������õ�ǰ��temp���������ȫ������
		}

		id = m_totalGUIs;   // ��¼��ǰGUIϵͳ����ID
		m_totalGUIs++;      // GUIϵͳ������������1

		return true;
	}

	
#ifdef UNICODE
	// ��ӱ���ͼGUI
	virtual bool AddGUIBackdrop(int guiId, TCHAR *fileName) = 0;

	// ��Ӿ�̬�ı�GUI
	virtual bool AddGUIStaticText(int guiId, int id,
		TCHAR *text, int x, int y, unsigned long color,
		int fontID) = 0;

	// ��Ӱ�ťGUI
	virtual bool AddGUIButton(int guiId, int id, int x,
		int y, TCHAR *up, TCHAR *over, TCHAR *down) = 0;

	virtual bool CreateText(TCHAR *font, int weight, bool italic,
		int size, int &id) = 0;

	virtual void DisplayText(int id, long x, long y,
		int r, int g, int b,
		TCHAR *text, ...) = 0;

	virtual void DisplayText(int id, long x, long y,
		unsigned long color,
		TCHAR *text, ...) = 0;

#else
	virtual bool AddGUIBackdrop(int guiId, char *fileName) = 0;

	// ��Ӿ�̬�ı�GUI
	virtual bool AddGUIStaticText(int guiId, int id,
		char *text, int x, int y, unsigned long color,
		int fontID) = 0;

	// ��Ӱ�ťGUI
	virtual bool AddGUIButton(int guiId, int id, int x,
		int y, char *up, char *over, char *down) = 0;

	virtual bool CreateText(char *font, int weight, bool italic,
		int size, int &id) = 0;
	virtual void DisplayText(int id, long x, long y,
		int r, int g, int b,
		char *text, ...) = 0;
	virtual void DisplayText(int id, long x, long y,
		unsigned long color,
		char *text, ...) = 0;

#endif
	// ����GUI
	virtual void ProcessGUI(int guiID, bool LMBDown,
		int mouseX, int mouseY,
		void(*funcPtr)(int id, int state)) = 0;
	
	// ������
	virtual void EnableFog(float start, float end,
		UGP_FOG_TYPE type, unsigned long color,
		bool rangeFog) = 0;
	// ������
	virtual void DisableFog() = 0;

	// FPS
	//virtual void DisplayFPS() = 0;

protected:
	int m_nSrceenWidth, m_nScreenHeight;
	BOOL m_fullScreen;

	WinHWND m_hMainWnd;

	float m_fNear, m_fFar;

	CGUISystem *m_guiList;  // GUIϵͳCGUISystem�Ķ�������(����)
	int m_totalGUIs;        // GUIϵͳCGUISystem�Ķ�������

};
GAMEENGINE_API BOOL CreateD3DRenderer(CRenderInterface** pObj);
#endif