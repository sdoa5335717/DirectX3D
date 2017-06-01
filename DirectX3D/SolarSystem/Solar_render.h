#ifndef RENDER_D3D_H
#define RENDER_D3D_H

#include "CommonFile.h"
#include "Render3D.h"

class CSolarRenderD3D : public CRender3D
{
public:
    CSolarRenderD3D();
    ~CSolarRenderD3D();

	// 设定相机位置
	virtual BOOL set_camera();
	// 设定投影变换
	virtual BOOL set_projection();
	// 设置光照
	virtual BOOL set_lighting(FLOAT fX, FLOAT fY, FLOAT fZ);
	// 渲染
	virtual BOOL render();
	// 创建行星
	void RenderStars(FLOAT fRadius);
	// 创建轨道
	void RenderTracks(vector<vector<FPoint>> vecPoint);
	// 设定行星(后期加入纹理)颜色
	void SetStarsMatrial();
private:
	// 网格对象
	vector<ID3DXMesh*> m_aObject;
	// 顶点对象
	vector<IDirect3DVertexBuffer9*> m_paVB;
	// 颜色
	vector<D3DMATERIAL9> m_vecColor;
	// 轨道点数
	vector<int> m_aPointNumber;
	// 轨道参数点
	vector<vector<FPoint>> m_vecPoint;
};

#endif