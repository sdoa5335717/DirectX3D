#ifndef D3DRENDERER_H
#define D3DRENDERER_H
#include "defines.h"

#include "RenderInterface.h"

#pragma comment(lib, "d3d9")
#pragma comment(lib, "d3dx9")

// 纹理信息结构
struct stD3DTexture
{
	stD3DTexture() : fileName(0), image(0), width(0), height(0) {}

	char *fileName;            // 指明文件名
	int width, height;         // 纹理的宽度和高度
	LPDIRECT3DTEXTURE9 image;  // 纹理对象
};

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

	void SetMaterial(stMaterial* mat);

	void SetLight(stLight* light, int index);

	void DisableLight(int index);

	// 设置透明度
	void SetTransparency(RenderState state, TransState src,
		TransState dst);

	

	// 设置纹理过滤器
	void SetTextureFilter(int index, int filter, int val);

	// 设置多纹理贴图
	void SetMultiTexture();

	// 应用纹理
	void ApplyTexture(int index, int texId);

	
#ifdef UNICODE
	// 保存屏幕截图
	void SaveScreenShot(TCHAR *file);
	// 添加2D纹理
	int AddTexture2D(TCHAR *file, int *texId);
#else
	void SaveScreenShot(char *file);
	// 添加2D纹理
	int AddTexture2D(char *file, int *texId);
#endif
	// 启用点状sprite
	void EnablePointSprites(float size, float min, float a,
		float b, float c);

	// 禁用点状sprite
	void DisablePointSprites();
#ifdef UNICODE
	// 添加背景图GUI
	bool AddGUIBackdrop(int guiId, TCHAR *fileName);

	// 添加静态文本GUI
	bool AddGUIStaticText(int guiId, int id, TCHAR *text,
		int x, int y, unsigned long color, int fontID);

	// 添加按钮GUI
	bool AddGUIButton(int guiId, int id, int x, int y, TCHAR *up, TCHAR *over, TCHAR *down);

	bool CreateText(TCHAR *font, int weight, bool italic,
		int size, int &id);

	void DisplayText(int id, long x, long y, int r,
		int g, int b, TCHAR *text, ...);

	void DisplayText(int id, long x, long y,
		unsigned long color, TCHAR *text, ...);

#else
	// 添加背景图GUI
	bool AddGUIBackdrop(int guiId, char *fileName);

	// 添加静态文本GUI
	bool AddGUIStaticText(int guiId, int id, char *text,
		int x, int y, unsigned long color, int fontID);
	// 添加按钮GUI
	bool AddGUIButton(int guiId, int id, int x, int y, char *up, char *over, char *down);
	
	bool CreateText(char *font, int weight, bool italic,
		int size, int &id);

	void DisplayText(int id, long x, long y, int r,
		int g, int b, char *text, ...);

	void DisplayText(int id, long x, long y,
		unsigned long color, char *text, ...);
#endif
	// 绘制GUI
	void ProcessGUI(int guiID, bool LMBDown, int mouseX,int mouseY,
		void(*funcPtr)(int id, int state));
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
	// 纹理列表
	stD3DTexture *m_textureList;
	// 纹理计数
	int m_numTextures;

	int m_totalFonts;

	LPD3DXFONT *m_fonts;

	int m_screenWidth;
	int m_screenHeight;
};
#endif