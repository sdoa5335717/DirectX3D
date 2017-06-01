
#include "Solar_render.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif


CSolarRenderD3D::CSolarRenderD3D()
{
}

CSolarRenderD3D::~CSolarRenderD3D()
{
    SAFE_RELEASE(m_pDevice);
	for (unsigned int i=0 ;i<m_paVB.size();i++)
	{
		SAFE_RELEASE(m_paVB[i]);
	}
}



void CSolarRenderD3D::SetStarsMatrial()
{
	//D3DMATERIAL9 Mtrls[9];
	const D3DXCOLOR BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
	D3DXCOLOR color;
	// Sun
	color = D3DCOLOR_XRGB(255, 0,0);
	m_vecColor.push_back(InitMtrl(color, color, color, BLACK, 2.0f));
	// Mercury
	color = D3DCOLOR_XRGB(106, 106,106);
	m_vecColor.push_back(InitMtrl(color, color, color, BLACK, 2.0f));
	// Venus
	color = D3DCOLOR_XRGB(162, 131, 51);
	m_vecColor.push_back(InitMtrl(color, color, color, BLACK, 2.0f));
	// Earth
	color = D3DCOLOR_XRGB(97, 168, 45);
	m_vecColor.push_back(InitMtrl(color, color, color, BLACK, 2.0f));
	// Mars
	color = D3DCOLOR_XRGB(197, 84, 16);
	m_vecColor.push_back(InitMtrl(color, color, color, BLACK, 2.0f));
	// Jupiter
	color = D3DCOLOR_XRGB(194, 151, 18);
	m_vecColor.push_back(InitMtrl(color, color, color, BLACK, 2.0f));
	// Saturn
	color = D3DCOLOR_XRGB(121, 6, 206);
	m_vecColor.push_back(InitMtrl(color, color, color, BLACK, 2.0f));
	// Uranus
	color = D3DCOLOR_XRGB(70, 156, 251);
	m_vecColor.push_back(InitMtrl(color, color, color, BLACK, 2.0f));
	// Neptune
	color = D3DCOLOR_XRGB(5, 101, 207);
	m_vecColor.push_back(InitMtrl(color, color, color, BLACK, 2.0f));
	// Pluto
	color = D3DCOLOR_XRGB(113, 81, 132);
	m_vecColor.push_back(InitMtrl(color, color, color, BLACK, 2.0f));

}
BOOL CSolarRenderD3D::render()
{
	D3DXMATRIX Worlds[9];
	HRESULT hr = S_OK;
    m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    if( SUCCEEDED(m_pDevice->BeginScene()) )
    {
		for (unsigned int i=0; i<m_paVB.size(); i++)
		{
			hr = m_pDevice->SetFVF(Vertex_3D::FVF);
			hr = m_pDevice->SetStreamSource(0, m_paVB[i], 0, sizeof(Vertex_3D));
			hr = m_pDevice->DrawPrimitive(D3DPT_LINESTRIP,0, m_aPointNumber[i]);
		}

		//static int delta = 0;
		int delta[9] = {9, 8, 7, 6, 5, 4, 3, 2, 1};

		static int array[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
		// 
		for (unsigned int i=0; i<m_aObject.size()-1; i++)
		{
			//int nMove = (delta+1)%m_vecPoint[i].size();
			D3DXMatrixTranslation(&Worlds[i], m_vecPoint[i][array[i]].fX, m_vecPoint[i][array[i]].fY+200.0f, 0.0f);
			array[i]= (array[i]+delta[i])%m_vecPoint[i].size();
		}
		
		for (unsigned int i=0; i< m_aObject.size()-1; i++)
		{	
			m_pDevice->SetTransform(D3DTS_WORLD, &Worlds[i]);
			hr = m_pDevice->SetMaterial(&m_vecColor[i+1]);
			hr = m_aObject[i+1]->DrawSubset(0);
		}

		D3DXMatrixTranslation(&Worlds[0], 0.0f, 200.0f, 0.0f);
		m_pDevice->SetTransform(D3DTS_WORLD, &Worlds[0]);
		// Draw
		hr = m_pDevice->SetMaterial(&m_vecColor[0]);
		hr = m_aObject[0]->DrawSubset(0);

        m_pDevice->EndScene();

        m_pDevice->Present(NULL, NULL, NULL, NULL);
    }
	return TRUE;
}

void CSolarRenderD3D::RenderTracks(vector<vector<FPoint>> vecPoint)
{
	IDirect3DVertexBuffer9* pVB = NULL;
	IDirect3DIndexBuffer9* pVI = NULL;
	Vertex_3D *vertex = NULL;
	m_vecPoint = vecPoint;
	for (unsigned int i=0; i<vecPoint.size(); i++)
	{
		// 创建顶点
		m_pDevice->CreateVertexBuffer(
			vecPoint[i].size() * sizeof(Vertex_3D), 
			D3DUSAGE_WRITEONLY,
			Vertex_3D::FVF,
			D3DPOOL_MANAGED,
			&pVB,
			0);

		m_aPointNumber.push_back(vecPoint[i].size());
		//vertex = (Vertex_3D*)COMMON_CALLOC(sizeof(Vertex_3D)*vecPoint[i].size());
		pVB->Lock(0, 0, (void**)&vertex, 0);
		for (unsigned int j=0; j<vecPoint[i].size(); j++)
		{
			vertex[j].x = vecPoint[i][j].fX;
			vertex[j].y = vecPoint[i][j].fY;
		}
		pVB->Unlock();
		
		m_paVB.push_back(pVB);

	}

}

BOOL CSolarRenderD3D::set_camera()
{
	RECT rc;
	GetClientRect(m_hView, &rc);

	float fFovY = D3DX_PI/2.0f; // Y-direction fov 45 degree
	float fFocal =(rc.bottom/2.0f)/tanf(fFovY/2.0f); // camera distance from the projection focal plane (Z==0)
	D3DXVECTOR3 position(0.0f, 0.0f, - 2.0f*fFocal);

	D3DXMATRIX matWorld;
	D3DXMatrixRotationX( &matWorld,-D3DX_PI/6.0f);
	
	D3DXVECTOR3 pout;
	D3DXVec3TransformNormal(&pout, &position, &matWorld);

	
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &pout, &target, &up);

	m_pDevice->SetTransform(D3DTS_VIEW, &V);

	return TRUE;
}

void CSolarRenderD3D::RenderStars(FLOAT fRadius)
{
	ID3DXMesh* pObject = NULL;
	// 创建球体
	D3DXCreateSphere(m_pDevice, fRadius, 
		(UINT)(fRadius),
		(UINT)(fRadius),
		&pObject, 0);

	m_aObject.push_back(pObject);
}

BOOL CSolarRenderD3D::set_projection()
{
	RECT rc;
	GetClientRect(m_hView, &rc);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI/2.0f, // 90 - degree
		(float)rc.right / (float)rc.bottom,
		0.0f,
		1000.0f);

	m_pDevice->SetTransform(D3DTS_PROJECTION, &proj);

	return TRUE;
}

BOOL CSolarRenderD3D::set_lighting(FLOAT fX, FLOAT fY, FLOAT fZ)
{
	D3DXCOLOR     WHITE( D3DCOLOR_XRGB(255, 255, 255) );
	D3DXVECTOR3	  dir(fX, fY, fZ);
	D3DXCOLOR     c = WHITE;
	D3DLIGHT9     dirLight = InitLight(D3DLIGHT_DIRECTIONAL, &dir, &c);

	//
	// Set and Enable the light.
	//

	m_pDevice->SetLight(0, &dirLight);
	m_pDevice->LightEnable(0, true);

	//
	// Set lighting related render states.
	//

	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, false);

	return TRUE;
}