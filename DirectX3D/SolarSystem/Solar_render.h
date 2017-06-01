#ifndef RENDER_D3D_H
#define RENDER_D3D_H

#include "CommonFile.h"
#include "Render3D.h"

class CSolarRenderD3D : public CRender3D
{
public:
    CSolarRenderD3D();
    ~CSolarRenderD3D();

	// �趨���λ��
	virtual BOOL set_camera();
	// �趨ͶӰ�任
	virtual BOOL set_projection();
	// ���ù���
	virtual BOOL set_lighting(FLOAT fX, FLOAT fY, FLOAT fZ);
	// ��Ⱦ
	virtual BOOL render();
	// ��������
	void RenderStars(FLOAT fRadius);
	// �������
	void RenderTracks(vector<vector<FPoint>> vecPoint);
	// �趨����(���ڼ�������)��ɫ
	void SetStarsMatrial();
private:
	// �������
	vector<ID3DXMesh*> m_aObject;
	// �������
	vector<IDirect3DVertexBuffer9*> m_paVB;
	// ��ɫ
	vector<D3DMATERIAL9> m_vecColor;
	// �������
	vector<int> m_aPointNumber;
	// ���������
	vector<vector<FPoint>> m_vecPoint;
};

#endif