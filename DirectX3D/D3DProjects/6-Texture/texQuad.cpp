//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: texQuad.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Renders a textured quad.  Demonstrates creating a texture, setting
//       texture filters, enabling a texture, and texture coordinates.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"
#include "..\include\wke.h"
//
// Globals
//

IDirect3DDevice9*     Device = 0; 

const int Width  = 640;
const int Height = 480;
 
IDirect3DVertexBuffer9* Quad = 0;
IDirect3DTexture9*      Tex  = 0;

PDIRECT3DSURFACE9 pRenderSurface = NULL,pBackBuffer = NULL, pTempSurface; 
LPDIRECT3DTEXTURE9 pRenderTexture = NULL; // 目标纹理  
//
// Classes and Structures
//
LPD3DXMESH g_torus = NULL;

struct Vertex
{
	Vertex(){}
	Vertex(
		float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v)
	{
		_x  = x;  _y  = y;  _z  = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u  = u;  _v  = v;
	}
    float _x, _y, _z;
    float _nx, _ny, _nz;
    float _u, _v; // texture coordinates

	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

//
// Framework Functions
//
bool Setup()
{
	//
	// Create the quad vertex buffer and fill it with the
	// quad geoemtry.
	//

	Device->CreateVertexBuffer(
		6 * sizeof(Vertex), 
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&Quad,
		0);

	Vertex* v;
	Quad->Lock(0, 0, (void**)&v, 0);

	// quad built from two triangles, note texture coordinates:
	v[0] = Vertex(-50.0f, -50.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-50.0f,  50.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex( 50.0f,  50.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[3] = Vertex(-50.0f, -50.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = Vertex( 50.0f, 50.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = Vertex( 50.0f, -50.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	Quad->Unlock();

	//s
	// Create the texture and set filters.
	//
	D3DXCreateTeapot(Device, &g_torus, 0);
	//D3DXCreateTorus(Device, 0.5f, 1.2f, 25,25, &g_torus, NULL);

	//Device->CreateTexture( 100,100,1,D3DUSAGE_RENDERTARGET,  
	//	D3DFMT_R5G6B5,D3DPOOL_DEFAULT,&pRenderTexture,NULL);  

	//HRESULT  hr = Device->CreateTexture(
	//	256,//3*(rect.right - rect.left)/5,
	//	256,//3*(rect.bottom - rect.top)/5,
	//	0,
	//	D3DUSAGE_DYNAMIC,
	//	D3DFMT_A8R8G8B8,
	//	D3DPOOL_DEFAULT,
	//	&pRenderTexture,
	//	NULL);

	//获得pRenderTexture对应的Surface  
	


	D3DXCreateTextureFromFile(
		Device,
		"dx5_logo.bmp",
		&pRenderTexture);

	pRenderTexture->GetSurfaceLevel(0,&pRenderSurface);

	//Device->SetTexture(0, Tex);

	//Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	D3DXVECTOR3 position(0.0f, 0.0f, -240.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &V);

	//
	// Don't use lighting for this sample.
	//
	Device->SetRenderState(D3DRS_LIGHTING, false);

	//
	// Set the projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
			&proj,
			D3DX_PI /2,//0.5f, // 90 - degree
			(float)Width / (float)Height,
			0.0f,
			1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(Quad);
	d3d::Release<IDirect3DTexture9*>(Tex);
}

bool Display(float timeDelta)
{

	//Device->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 128, 64), 1.0f, 0);
	//if( SUCCEEDED(Device->BeginScene()) )
	//{
	//	Device->SetTexture(0, pRenderTexture);
	//	Device->SetFVF(Vertex::FVF);
	//	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, , sizeof(Vertex));
	//	Device->EndScene();

	//	Device->Present(NULL, NULL, NULL, NULL);
	//}


	Device->GetRenderTarget(0, &pBackBuffer);
	
	// Set texture surface as RenderTarget
	Device->SetRenderTarget(0, pRenderSurface);

	Device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff0000ff, 1.0f, 0 );

	if( SUCCEEDED( Device->BeginScene() ) )
	{
		Device->SetTexture(0, NULL) ;

		Device->SetRenderState( D3DRS_LIGHTING , FALSE ); 
		Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		g_torus->DrawSubset(0);

		Device->EndScene();
	}

	Device->SetRenderTarget(0, pBackBuffer);

	Device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff00ff00, 1.0f, 0 );
	if( SUCCEEDED( Device->BeginScene() ) )
	{
		Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID) ;

		//Draw texture
		//RenderQuad() ; 
		//Device->SetTexture(0, pRenderTexture);
		Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
		Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );


		Device->SetStreamSource(0, Quad, 0, sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);


		Device->EndScene();
	}

	Device->Present(0, 0, 0, 0);

	//if( Device )
	//{
	//	Device->GetRenderTarget(0,&pBackBuffer); 

	//	Device->BeginScene();

	//	Device->SetRenderTarget(0, pRenderSurface); 

	//	Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	//	

	//	Device->SetStreamSource(0, Quad, 0, sizeof(Vertex));
	//	Device->SetFVF(Vertex::FVF);
	//	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	//	//重新将render target设置为帧缓存  
	//	Device->SetRenderTarget(0, pBackBuffer); 

	//	Device->EndScene();
	//	Device->Present(0, 0, 0, 0);
	//}
	return true;
}


//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
		
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	if(!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}
		
	if(!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop( Display );

	Cleanup();

	Device->Release();

	return 0;
}