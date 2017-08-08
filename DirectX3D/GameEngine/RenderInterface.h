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
	// 调用一次，例如设置投影矩阵
	virtual void OneTimeInit() = 0;
	// 关闭清楚渲染系统
	virtual void ShutDown() = 0;
	// 设置后台缓存颜色
	virtual void SetClearColor(float r, float g, float b) = 0;
	// 启动渲染
	virtual void StartRender(BOOL bColor, BOOL bDepth, BOOL bStencil) = 0;
	// 清理缓存
	virtual void ClearBuffers(BOOL bColor, BOOL bDepth, BOOL bStencil) = 0;

	virtual void EndRendering() = 0;
	// 计算投影矩阵

	virtual void CalculateProjMatrix(float fov, float n, float f) = 0;
	// 计算正交矩阵
	virtual void CalculateOrthoMatrix(float n, float f) = 0;

	virtual int CreateStaticBuffer(VextexType, PrimType, UINT totalVerts, 
		UINT totalIndices, int stride, void** data, UINT* indices, UINT* staticID) = 0;

	virtual int Render(UINT staticID) = 0;

	// 设置材质,光照
	virtual void SetMaterial(stMaterial* mat) = 0;
	virtual void SetLight(stLight* light, int index) = 0;
	virtual void DisableLight(int index) = 0;

	// 设置透明度
	virtual void SetTransparency(RenderState state,
		TransState src, TransState dst) = 0;

	

	// 设置纹理过滤器
	virtual void SetTextureFilter(int index, int filter,
		int val) = 0;

	// 设置多纹理贴图
	virtual void SetMultiTexture() = 0;

	// 应用纹理
	virtual void ApplyTexture(int index, int texId) = 0;

	// 保存屏幕截图
#ifdef UNICODE
	virtual void SaveScreenShot(TCHAR *file) = 0;
	// 添加2D纹理
	virtual int AddTexture2D(TCHAR *file, int *texId) = 0;
#else 
	virtual void SaveScreenShot(char *file) = 0;
	// 添加2D纹理
	virtual int AddTexture2D(char *file, int *texId) = 0;
#endif
	// 启用点状sprite
	virtual void EnablePointSprites(float size, float min,
		float a, float b, float c) = 0;

	// 禁用点状sprite
	virtual void DisablePointSprites() = 0;

	// 创建新的GUI对象，并将该对象添加到链表中
	bool CreateGUI(int &id)
	{
		// 首次创建GUI系统CGUISystem对象时，创建一个含有1个元素的对象链表
		if (!m_guiList)
		{
			// m_guiList初始化为动态创建的对象数组
			m_guiList = new CGUISystem[1];

			if (!m_guiList)
			{
				return GM_FAIL;
			}
		}
		else
		{
			// 临时的GUI系统对象指针，用于保存当前对象数组的值
			CGUISystem *temp;

			// 指向动态开辟的GUI系统对象数组
			temp = new CGUISystem[m_totalGUIs + 1];

			// 将m_guiList对象数组的全部数据复制到temp中
			memcpy(temp, m_guiList, sizeof(CGUISystem) * m_totalGUIs);

			delete[] m_guiList;  // 销毁m_guiList对象数组
			m_guiList = temp;    // m_guiList对象数组获得当前的temp对象数组的全部数据
		}

		id = m_totalGUIs;   // 记录当前GUI系统对象ID
		m_totalGUIs++;      // GUI系统对象数量递增1

		return true;
	}

	
#ifdef UNICODE
	// 添加背景图GUI
	virtual bool AddGUIBackdrop(int guiId, TCHAR *fileName) = 0;

	// 添加静态文本GUI
	virtual bool AddGUIStaticText(int guiId, int id,
		TCHAR *text, int x, int y, unsigned long color,
		int fontID) = 0;

	// 添加按钮GUI
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

	// 添加静态文本GUI
	virtual bool AddGUIStaticText(int guiId, int id,
		char *text, int x, int y, unsigned long color,
		int fontID) = 0;

	// 添加按钮GUI
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
	// 绘制GUI
	virtual void ProcessGUI(int guiID, bool LMBDown,
		int mouseX, int mouseY,
		void(*funcPtr)(int id, int state)) = 0;
	
	// 启用雾
	virtual void EnableFog(float start, float end,
		UGP_FOG_TYPE type, unsigned long color,
		bool rangeFog) = 0;
	// 禁用雾
	virtual void DisableFog() = 0;

	// FPS
	//virtual void DisplayFPS() = 0;

protected:
	int m_nSrceenWidth, m_nScreenHeight;
	BOOL m_fullScreen;

	WinHWND m_hMainWnd;

	float m_fNear, m_fFar;

	CGUISystem *m_guiList;  // GUI系统CGUISystem的对象链表(数组)
	int m_totalGUIs;        // GUI系统CGUISystem的对象数量

};
GAMEENGINE_API BOOL CreateD3DRenderer(CRenderInterface** pObj);
#endif