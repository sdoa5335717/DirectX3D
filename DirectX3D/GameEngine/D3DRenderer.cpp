#include "stdafx.h"
#include "D3DRenderer.h"
#include <atlconv.h>

CD3DFont* Font    = 0;
DWORD FrameCnt    = 0;
float TimeElapsed = 0;
float FPS         = 0;
TCHAR FPSString[9] = {};

unsigned long CreateD3DFVF(int flags)
{
	unsigned long fvf = 0;

	if (flags == GUI_FVF)
	{
		fvf = D3DFVF_GUI;
	}

	return fvf;
}

inline unsigned long FtoDW(float val) 
{
	return *((unsigned long*)&val);
}

BOOL CreateD3DRenderer(CRenderInterface** pObj)
{
	if (!*pObj)
	{
		*pObj = new CD3DRenderer;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

CD3DRenderer::CD3DRenderer()
{
	m_pDirect3D = NULL;
	m_pDevice = NULL;
	m_bRenderingScene = FALSE;
	m_numStaticBuffers = 0;
	m_numActiveStaticBuffer = GM_INVALID;
	m_staticBufferList = NULL;

	m_textureList = NULL;
	m_numTextures = 0;
	m_totalFonts = 0;
	m_guiList = NULL;
	m_totalGUIs = 0;
	m_fonts = NULL;

	m_screenHeight = 0;
	m_screenWidth = 0;

	m_fontFPS = NULL;

}

CD3DRenderer::~CD3DRenderer()
{
	ShutDown();
}

D3DMULTISAMPLE_TYPE GetD3DMultiSampleType(LPDIRECT3D9 d3d,
										  UGP_MS_TYPE ms, D3DDEVTYPE type, D3DFORMAT format, bool fullscreen)
{
	D3DMULTISAMPLE_TYPE t = D3DMULTISAMPLE_NONE;

	if(d3d)
	{
		switch(ms)
		{
		case UGP_MS_NONE:
			t = D3DMULTISAMPLE_NONE;
			break;

		case UGP_MS_SAMPLES_2:
			if(d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
				type, format, !fullscreen, D3DMULTISAMPLE_2_SAMPLES,
				NULL) == D3D_OK) t = D3DMULTISAMPLE_2_SAMPLES;
			break;

		case UGP_MS_SAMPLES_4:
			if(d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
				type, format, !fullscreen, D3DMULTISAMPLE_2_SAMPLES,
				NULL) == D3D_OK) t = D3DMULTISAMPLE_4_SAMPLES;
			break;

		case UGP_MS_SAMPLES_8:
			if(d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
				type, format, !fullscreen, D3DMULTISAMPLE_8_SAMPLES,
				NULL) == D3D_OK) t = D3DMULTISAMPLE_8_SAMPLES;
			break;

		case UGP_MS_SAMPLES_16:
			if(d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
				type, format, !fullscreen, D3DMULTISAMPLE_16_SAMPLES,
				NULL) == D3D_OK) t = D3DMULTISAMPLE_16_SAMPLES;
			break;

		default:
			break;
		}
	}

	return t;
}


BOOL CD3DRenderer::Initialize(int w, int h, WinHWND mainWin, BOOL fullScreen, UGP_MS_TYPE ms)
{
	ShutDown();

	m_screenWidth = w;
	m_screenHeight = h;

	m_hMainWnd = mainWin;
	if (!m_hMainWnd)
	{
		return FALSE;
	}

	m_fullScreen = fullScreen;

	D3DDISPLAYMODE mode;
	D3DCAPS9 caps;
	D3DPRESENT_PARAMETERS Params;

	ZeroMemory(&Params, sizeof(Params));

	m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_pDirect3D)
	{
		return FALSE;
	}

	if (FAILED(m_pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode)))
	{
		return FALSE;
	}

	if (FAILED(m_pDirect3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return FALSE;
	}

	DWORD processing = 0;
	if (caps.VertexProcessingCaps != 0)
	{
		processing = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
	}
	else
	{
		processing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	if (m_fullScreen)
	{
		Params.FullScreen_RefreshRateInHz = mode.RefreshRate;
		Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}
	else
	{
		Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	Params.Windowed = !fullScreen;
	Params.BackBufferWidth = w;
	Params.BackBufferHeight = h;
	Params.hDeviceWindow = m_hMainWnd;
	Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Params.BackBufferFormat = mode.Format;
	Params.BackBufferCount = 1;
	Params.EnableAutoDepthStencil = TRUE;
	Params.AutoDepthStencilFormat = D3DFMT_D16;
	Params.MultiSampleType = GetD3DMultiSampleType(m_pDirect3D, ms,
		D3DDEVTYPE_HAL, mode.Format, m_fullScreen);

	m_nSrceenWidth = w;
	m_nScreenHeight= h;

	if (FAILED(m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hMainWnd, 
		processing, &Params, &m_pDevice)))
	{
		return FALSE;
	}

	//m_fontFPS = new CD3DFont(_T("Times New Roman"), 16, 0);
	//m_fontFPS->InitDeviceObjects( m_pDevice );
	//m_fontFPS->RestoreDeviceObjects();
	
	/*m_fontFPS = new LPD3DXFONT[1];

	if(FAILED(D3DXCreateFont(m_pDevice, size, 0, weight, 1, italic,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, font,
		&m_fontFPS[0]))) return false;*/

	OneTimeInit();

	return TRUE;
}

void CD3DRenderer::OneTimeInit()
{
	if (!m_pDevice)
	{
		return;
	}

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	CalculateProjMatrix(D3DX_PI/4, 0.1f, 1000);
}

void CD3DRenderer::ShutDown()
{
	for (int i = 0;i<m_numStaticBuffers;i++)
	{
		STRAND_SAFE_RELEASE(m_staticBufferList[i].vbPtr);
		STRAND_SAFE_RELEASE(m_staticBufferList[i].ibPtr);
	}

	m_numStaticBuffers = 0;

	STRAND_SAFE_DELETE_ARRAY(m_staticBufferList)
	// 释放纹理对象列表
	for(int s = 0; s < m_numTextures; s++)
	{
		STRAND_SAFE_DELETE_ARRAY(m_textureList[s].fileName)		// 销毁文件名对象
		STRAND_SAFE_RELEASE(m_textureList[s].image);			// 释放纹理对象
	}

	m_numTextures = 0;   // 数目赋值为0
	STRAND_SAFE_DELETE_ARRAY(m_textureList)   // 销毁纹理对象列表内存区域
	/////////////////////////////////////////////////////
	// 释放所有的GUI系统对象
	for(int s=0; s < m_totalGUIs; s++)
	{
		m_guiList[s].Shutdown();
	}
	m_totalGUIs = 0;    // GUI系统对象数量清0

	STRAND_SAFE_DELETE_ARRAY(m_guiList)	// 删除GUI系统对象内存块

	STRAND_SAFE_RELEASE(m_pDevice)
	STRAND_SAFE_RELEASE(m_pDirect3D)
}

void CD3DRenderer::SetClearColor(float r, float g, float b)
{
	m_Color = D3DCOLOR_COLORVALUE(r, g, b, 1.0f);
}

void CD3DRenderer::StartRender(BOOL bColor, BOOL bDepth, BOOL bStencil)
{
	if (!m_pDevice) return;
	
	unsigned int buffers = 0;

	if (bColor)
	{
		buffers |= D3DCLEAR_TARGET;
	}

	if (bDepth)
	{
		buffers |= D3DCLEAR_ZBUFFER;
	}

	if (bStencil)
	{
		buffers |= D3DCLEAR_STENCIL;
	}

	if (FAILED(m_pDevice->Clear(0, NULL, buffers, m_Color, 1, 0)))
	{
		return;
	}

	if (FAILED(m_pDevice->BeginScene())) 
	{
		return;
	}

	m_bRenderingScene = TRUE;
}

void CD3DRenderer::ClearBuffers(BOOL bColor, BOOL bDepth, BOOL bStencil)
{
	if (!m_pDevice)
	{
		return;
	}

	unsigned int buffers = 0;
	if (bColor)
	{
		buffers |= D3DCLEAR_TARGET;
	}
	if (bDepth)
	{
		buffers |= D3DCLEAR_ZBUFFER;
	}
	if (bStencil)
	{
		buffers |= D3DCLEAR_STENCIL;
	}

	if (m_bRenderingScene)
	{
		m_pDevice->EndScene();
	}

	if (FAILED(m_pDevice->Clear(0, NULL, buffers, m_Color, 1, 0)))
	{
		return;
	}

	if (m_bRenderingScene)
	{
		if (FAILED(m_pDevice->BeginScene()))
		{
			return;
		}
	}
}

void CD3DRenderer::EndRendering()
{
	if (!m_pDevice)
	{
		return;
	}

	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, NULL, NULL);

	m_bRenderingScene = FALSE;
}

void CD3DRenderer::CalculateProjMatrix(float fov, float n, float f)
{
	if (!m_pDevice)
	{
		return;
	}

	D3DXMATRIX projection;

	D3DXMatrixPerspectiveFovLH(&projection, fov, (float)m_nSrceenWidth/(float)m_nScreenHeight, n, f);

	m_pDevice->SetTransform(D3DTS_PROJECTION, &projection);
}

void CD3DRenderer::CalculateOrthoMatrix(float n, float f)
{
	if (!m_pDevice)
	{
		return;
	}

	D3DXMATRIX ortho;

	D3DXMatrixOrthoLH(&ortho, (float)m_nSrceenWidth, (float)m_nScreenHeight, n, f);

	m_pDevice->SetTransform(D3DTS_PROJECTION, &ortho);
}

int CD3DRenderer::CreateStaticBuffer(VextexType vType, PrimType primType, UINT totalVerts, 
									 UINT totalIndices, int stride, 
									 void** data, UINT* indices, UINT* staticID)
{
	void* ptr;
	int index = m_numStaticBuffers;

	if (!m_staticBufferList)
	{
		m_staticBufferList = new stD3DStaticBuffer[1];
		if (!m_staticBufferList)
		{
			return GM_FAIL;
		}
	}
	else
	{
		stD3DStaticBuffer* temp = NULL;
		temp = new stD3DStaticBuffer[m_numStaticBuffers+1];

		memcpy(temp, m_staticBufferList, sizeof(stD3DStaticBuffer)*m_numStaticBuffers);

		delete[] m_staticBufferList;
		m_staticBufferList = temp;
	}

	m_staticBufferList[index].numVerts	= totalVerts;
	m_staticBufferList[index].numIndices = totalIndices;
	m_staticBufferList[index].primType = primType;
	m_staticBufferList[index].stride = stride;
	m_staticBufferList[index].fvf = CreateD3DFVF(vType);

	if (totalIndices > 0)
	{
		if (FAILED(m_pDevice->CreateIndexBuffer(
			sizeof(unsigned int)*totalIndices, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, 
			&m_staticBufferList[index].ibPtr, NULL)))
		{
			return GM_FAIL;
		}

		if (FAILED(m_staticBufferList[index].ibPtr->Lock(0,0,(void**)&ptr, 0)))
		{
			return GM_FAIL;
		}

		memcpy(ptr, indices, sizeof(unsigned int)*totalIndices);

		m_staticBufferList[index].ibPtr->Unlock();
	}
	else
	{
		m_staticBufferList[index].ibPtr = NULL;
	}

	if (FAILED(m_pDevice->CreateVertexBuffer(totalVerts*stride, D3DUSAGE_WRITEONLY, m_staticBufferList[index].fvf, 
		D3DPOOL_DEFAULT, &m_staticBufferList[index].vbPtr, NULL)))
	{
		return GM_FAIL;
	}

	if (FAILED(m_staticBufferList[index].vbPtr->Lock(0,0,(void**)&ptr, 0)))
	{
		return GM_FAIL;
	}

	memcpy(ptr, data, totalVerts*stride);
	m_staticBufferList[index].vbPtr->Unlock();

	*staticID = m_numStaticBuffers;

	m_numStaticBuffers++;

	return GM_OK;
}

int CD3DRenderer::Render(UINT staticID)
{
	if (staticID>=(UINT)m_numStaticBuffers)
	{
		return GM_FAIL;
	}

	if (m_numActiveStaticBuffer != staticID)
	{
		if (m_staticBufferList[staticID].ibPtr != NULL)
		{
			m_pDevice->SetIndices(m_staticBufferList[staticID].ibPtr);
		}

		HRESULT hr = m_pDevice->SetStreamSource(0, m_staticBufferList[staticID].vbPtr, 0, m_staticBufferList[staticID].stride);

		m_pDevice->SetFVF(m_staticBufferList[staticID].fvf);

		m_numActiveStaticBuffer = staticID;

	}

	if (m_staticBufferList[staticID].ibPtr != NULL)
	{
		switch(m_staticBufferList[staticID].primType)
		{
		case POINT_LIST:
			{
				if (FAILED(m_pDevice->DrawPrimitive(D3DPT_POINTLIST, 0, m_staticBufferList[staticID].numVerts)))
				{
					return GM_FAIL;
				}
				break;
			}
		case TRIANGLE_LIST:
			{
				if (FAILED(m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,
					0, m_staticBufferList[staticID].numVerts/3, 0, m_staticBufferList[staticID].numIndices)))
				{
					return GM_FAIL;
				}
				break;
			}
		case TRIANGLE_STRIP:
			{
				if (FAILED(m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0,
					0, m_staticBufferList[staticID].numVerts/2,
					0, m_staticBufferList[staticID].numIndices)))
				{
					return GM_FAIL;
				}
				break;
			}
		case TRIANGLE_FAN:
			{
				if (FAILED(m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0,
					0, m_staticBufferList[staticID].numVerts/2,
					0, m_staticBufferList[staticID].numIndices)))
				{
					return GM_FAIL;
				}
				break;
			}
		case LINE_LIST:
			{
				if (FAILED(m_pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0,
					0, m_staticBufferList[staticID].numVerts/2,
					0, m_staticBufferList[staticID].numIndices)))
				{
					return GM_FAIL;
				}
				break;
			}
		case LINE_STRIP:
			{
				if (FAILED(m_pDevice->DrawIndexedPrimitive(
					D3DPT_LINESTRIP, 0, 
					0, m_staticBufferList[staticID].numVerts,
					0, m_staticBufferList[staticID].numIndices)))
				{
					return GM_FAIL;
				}
				break;
			}
		default:
			return GM_FAIL;
		}
	}
	else
	{
		switch(m_staticBufferList[staticID].primType)
		{
		case POINT_LIST:
			{
				if (FAILED(m_pDevice->DrawPrimitive(D3DPT_POINTLIST, 0,
					m_staticBufferList[staticID].numVerts)))
				{
					return GM_FAIL;
				}
				break;
			}
		case TRIANGLE_LIST:
			{
				if (FAILED(m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 
					(int)(m_staticBufferList[staticID].numVerts/3))))
				{
					return GM_FAIL;
				}
				break;
			}
		case TRIANGLE_STRIP:
			{
				HRESULT hr = m_pDevice->DrawPrimitive(
					D3DPT_TRIANGLESTRIP, 0,
					(int)(m_staticBufferList[staticID].numVerts/2));
				if (FAILED(hr))
				/*if (FAILED(m_pDevice->DrawPrimitive(
					D3DPT_TRIANGLESTRIP, 0,
					(int)(m_staticBufferList[staticID].numVerts/2))))*/
				{
					return GM_FAIL;
				}
				break;
			}
		case TRIANGLE_FAN:
			{
				if (FAILED(m_pDevice->DrawPrimitive(
					D3DPT_TRIANGLEFAN, 0,
					(int)(m_staticBufferList[staticID].numVerts/2))))
				{
					return GM_FAIL;
				}
				break;
			}
		case LINE_LIST:
			{
				if (FAILED(m_pDevice->DrawPrimitive(D3DPT_LINELIST, 0,
					m_staticBufferList[staticID].numVerts/2)))
				{
					return GM_FAIL;
				}
				break;
			}
		case LINE_STRIP:
			{
				if (FAILED(m_pDevice->DrawPrimitive(D3DPT_LINESTRIP, 0,
					m_staticBufferList[staticID].numVerts)))
				{
					return GM_FAIL;
				}
				break;
			}
		default:
			return GM_FAIL;
		}
	}
	return GM_OK;
}

void CD3DRenderer::SetMaterial(stMaterial* mat)
{
	if (!mat || !m_pDevice)
	{
		return;
	}

	D3DMATERIAL9 m = { mat->diffuseR, mat->diffuseG, mat->diffuseB, mat->diffuseA,
					   mat->ambientR, mat->ambientG, mat->ambientB, mat->ambientA,
					   mat->specularR, mat->specularG, mat->specularB, mat->specularA,
					   mat->emissiveR, mat->emissiveG, mat->ambientB, mat->emissiveA,
						mat->power };

	m_pDevice->SetMaterial(&m);
}

void CD3DRenderer::SetLight(stLight* light, int index)
{
	if (!light || !m_pDevice || index < 0)
	{
		return;
	}

	D3DLIGHT9 l;
	l.Ambient.a = light->ambientA;
	l.Ambient.r = light->ambientR;
	l.Ambient.g = light->ambientG;
	l.Ambient.b = light->ambientB;

	l.Attenuation0 = light->attenuation0;
	l.Attenuation1 = light->attenuation1;
	l.Attenuation2 = light->attenuation2;

	l.Diffuse.a = light->diffuseA;
	l.Diffuse.r = light->diffuseR;
	l.Diffuse.g = light->diffuseG;
	l.Diffuse.b = light->diffuseB;

	l.Direction.x = light->dirX;
	l.Direction.y = light->dirY;
	l.Direction.z = light->dirZ;

	l.Falloff = light->falloff;
	l.Phi =light->phi;

	l.Position.x = light->posX;
	l.Position.y = light->posY;
	l.Position.z = light->posZ;

	l.Range = light->range;

	l.Specular.a = light->specularA;
	l.Specular.r = light->specularR;
	l.Specular.g = light->specularG;
	l.Specular.b = light->specularB;

	l.Theta = light->theta;

	if (light->type == LIGHT_POINT)
	{
		l.Type = D3DLIGHT_POINT;
	}
	else if (light->type == LIGHT_SPOT)
	{
		l.Type = D3DLIGHT_SPOT;
	}
	else
	{
		l.Type = D3DLIGHT_DIRECTIONAL;
	}

	m_pDevice->SetLight(index, &l);
	m_pDevice->LightEnable(index, TRUE);
}

void CD3DRenderer::DisableLight(int index)
{
	if (!m_pDevice)
	{
		return;
	}

	m_pDevice->LightEnable(index, FALSE);
}

// 设置透明度
void CD3DRenderer::SetTransparency(
								   RenderState state,  // 正在设置的渲染状态
								   TransState src,     // 源混合操作(源融合因子)
								   TransState dst      // 目的混合操作(目标融合因子)
								   )
{
	if(!m_pDevice) return;

	// 若状态标识符相符，则禁用透明度
	if(state == TRANSPARENCY_NONE)
	{
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		return;
	}

	// 若状态标识符相符
	if(state == TRANSPARENCY_ENABLE)
	{
		// 启用用透明度
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

		// 设置源混合操作
		switch(src)
		{
		case TRANS_ZERO:
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,
				D3DBLEND_ZERO);
			break;

		case TRANS_ONE:
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,
				D3DBLEND_ONE);
			break;

		case TRANS_SRCCOLOR:
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,
				D3DBLEND_SRCCOLOR);
			break;

		case TRANS_INVSRCCOLOR:
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,
				D3DBLEND_INVSRCCOLOR);
			break;

		case TRANS_SRCALPHA:
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,
				D3DBLEND_SRCALPHA);
			break;

		case TRANS_INVSRCALPHA:
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,
				D3DBLEND_INVSRCALPHA);
			break;

		case TRANS_DSTALPHA:
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,
				D3DBLEND_DESTALPHA);
			break;

		case TRANS_INVDSTALPHA:
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,
				D3DBLEND_INVDESTALPHA);
			break;

		case TRANS_DSTCOLOR:
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,
				D3DBLEND_DESTCOLOR);
			break;

		case TRANS_INVDSTCOLOR:
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,
				D3DBLEND_INVDESTCOLOR);
			break;

		case TRANS_SRCALPHASAT:
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,
				D3DBLEND_SRCALPHASAT);
			break;

		case TRANS_BOTHSRCALPHA:
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,
				D3DBLEND_BOTHSRCALPHA);
			break;

		case TRANS_INVBOTHSRCALPHA:
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,
				D3DBLEND_BOTHINVSRCALPHA);
			break;

		case TRANS_BLENDFACTOR:
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,
				D3DBLEND_BLENDFACTOR);
			break;                  

		case TRANS_INVBLENDFACTOR:
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,
				D3DBLEND_INVBLENDFACTOR);
			break;

		default:
			m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,
				false);
			return;
			break;
		}

		// 设置目地混合操作
		switch(dst)
		{
		case TRANS_ZERO:
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,
				D3DBLEND_ZERO);
			break;

		case TRANS_ONE:
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,
				D3DBLEND_ONE);
			break;

		case TRANS_SRCCOLOR:
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,
				D3DBLEND_SRCCOLOR);
			break;

		case TRANS_INVSRCCOLOR:
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,
				D3DBLEND_INVSRCCOLOR);
			break;

		case TRANS_SRCALPHA:
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,
				D3DBLEND_SRCALPHA);
			break;

		case TRANS_INVSRCALPHA:
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,
				D3DBLEND_INVSRCALPHA);
			break;

		case TRANS_DSTALPHA:
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,
				D3DBLEND_DESTALPHA);
			break;

		case TRANS_INVDSTALPHA:
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,
				D3DBLEND_INVDESTALPHA);
			break;

		case TRANS_DSTCOLOR:
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,
				D3DBLEND_DESTCOLOR);
			break;

		case TRANS_INVDSTCOLOR:
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,
				D3DBLEND_INVDESTCOLOR);
			break;

		case TRANS_SRCALPHASAT:
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,
				D3DBLEND_SRCALPHASAT);
			break;

		case TRANS_BOTHSRCALPHA:
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,
				D3DBLEND_BOTHSRCALPHA);
			break;

		case TRANS_INVBOTHSRCALPHA:
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,
				D3DBLEND_BOTHINVSRCALPHA);
			break;

		case TRANS_BLENDFACTOR:
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,
				D3DBLEND_BLENDFACTOR);
			break;                  

		case TRANS_INVBLENDFACTOR:
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,
				D3DBLEND_INVBLENDFACTOR);
			break;

		default:
			m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,
				false);
			break;
		}
	}
}

// 添加2D纹理
#ifdef UNICODE
int CD3DRenderer::AddTexture2D(
							   TCHAR *file,  // 图像文件名
							   int *texId   // 存储新创建的纹理对象的纹理ID
							   )
#else
int CD3DRenderer::AddTexture2D(
							   char *file,  // 图像文件名
							   int *texId   // 存储新创建的纹理对象的纹理ID
							   )
#endif
{
	if(!file || !m_pDevice) return GM_FAIL;

	// 获取文件名的字节长度
	size_t len = 0;
	StringCchLength(file,STRSAFE_MAX_CCH, &len);
	if(!len) return GM_FAIL;

	// 获取纹理对象数目
	int index = m_numTextures;

	// 如果 m_textureList对象列表为空，则表明是第一次创建
	if(!m_textureList)
	{
		// m_textureList获得了动态开辟的一段有1个元素的stD3DTexture结构数组内存块
		m_textureList = new stD3DTexture[1];

		if(!m_textureList) return GM_FAIL;
	}
	else
	{
		stD3DTexture *temp;   // 临时对象列表

		// temp获得比现有对象列表大1位的内存块
		temp = new stD3DTexture[m_numTextures + 1];

		// 将 m_textureList内存块的数据复制到temp中
		memcpy(temp, m_textureList,
			sizeof(stD3DTexture) * m_numTextures);

		delete[] m_textureList;  // 销毁对象列表 m_textureList的内存块
		m_textureList = temp;    // 对象列表 m_textureList获得了新的对象列表数据
	}

	// 开辟fileName的内存区域
#ifdef UNICODE
	m_textureList[index].fileName = new TCHAR[len+1];
	memset(m_textureList[index].fileName, 0, sizeof(TCHAR)*(len+1));
	// 将图像文件名复制到 m_textureList[index].fileName
	memcpy(m_textureList[index].fileName, file, len*sizeof(TCHAR));
#else
	m_textureList[index].fileName = new char[len+1];
	memset(m_textureList[index].fileName, 0, sizeof(char)*(len+1));
	memcpy(m_textureList[index].fileName, file, len);
#endif
	

	D3DCOLOR colorkey = 0xff000000;
	D3DXIMAGE_INFO info;

	// 从文件加载纹理图像
	if(D3DXCreateTextureFromFileEx(m_pDevice, file, 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT,  colorkey, &info, NULL,
		&m_textureList[index].image) != D3D_OK) return false;

	m_textureList[index].image->SetAutoGenFilterType(D3DTEXF_LINEAR);
	m_textureList[index].image->GenerateMipSubLevels();   // 生成mipmaps

	m_textureList[index].width = info.Width;    // 在对象中保存纹理图像的宽度
	m_textureList[index].height = info.Height;  // 在对象中保存纹理图像的高度

	*texId = m_numTextures;  // 保存纹理ID
	m_numTextures++;         // 增加纹理总数

	return GM_OK;
}

// 设置纹理过滤器
void CD3DRenderer::SetTextureFilter(
									int index,   // 纹理单位
									int filter,  // 滤波器模式
									int val      // 滤波器值
									)
{
	if(!m_pDevice || index < 0) return;

	D3DSAMPLERSTATETYPE fil = D3DSAMP_MINFILTER;
	int v = D3DTEXF_POINT;

	if(filter == MIN_FILTER) fil = D3DSAMP_MINFILTER;
	if(filter == MAG_FILTER) fil = D3DSAMP_MAGFILTER;
	if(filter == MIP_FILTER) fil = D3DSAMP_MIPFILTER;

	if(val == POINT_TYPE) v = D3DTEXF_POINT;
	if(val == LINEAR_TYPE) v = D3DTEXF_LINEAR;
	if(val == ANISOTROPIC_TYPE) v = D3DTEXF_ANISOTROPIC;

	m_pDevice->SetSamplerState(index, fil, v);
}

// 设置多纹理贴图
void CD3DRenderer::SetMultiTexture()
{
	if(!m_pDevice) return;

	m_pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP,
		D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1,
		D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2,
		D3DTA_DIFFUSE);

	m_pDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
	m_pDevice->SetTextureStageState(1, D3DTSS_COLOROP,
		D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(1, D3DTSS_COLORARG1,
		D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(1, D3DTSS_COLORARG2,
		D3DTA_CURRENT);
}

// 应用纹理
void CD3DRenderer::ApplyTexture(int index, int texId)
{
	if(!m_pDevice) return;
	HRESULT hr = S_OK;
	if(index < 0 || texId < 0)
		hr = m_pDevice->SetTexture(0, NULL);
	else
		hr = m_pDevice->SetTexture(0, m_textureList[texId].image);

	//OutputDebugString(L"Apply Texture\n");
}
// 保存屏幕截图
#ifdef UNICODE
void CD3DRenderer::SaveScreenShot(TCHAR *file)
#else
void CD3DRenderer::SaveScreenShot(char *file)
#endif
{
	if(!file) return;

	LPDIRECT3DSURFACE9 surface = NULL;
	D3DDISPLAYMODE disp;

	// 获得m_Device设备所指向的场景的显示模式
	m_pDevice->GetDisplayMode(0, &disp);

	// 创建表面
	m_pDevice->CreateOffscreenPlainSurface(disp.Width, disp.Height,
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);

	// 使用渲染好的场景填充表面 surface
	m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &surface);

	// 保存该表面数据为指定文件
	D3DXSaveSurfaceToFile(file, D3DXIFF_JPG, surface, NULL, NULL);

	if(surface != NULL) surface->Release();
	surface = NULL;
}

// 启用点状sprite
void CD3DRenderer::EnablePointSprites(
									  float size,  // sprite的尺寸
									  float min,   // 最小尺寸
									  float a,     // 刻度值
									  float b,     // 刻度值
									  float c      // 刻度值
									  )
{
	if(!m_pDevice) return;

	m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);    // 启用点状sprite
	m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);     // 启用点状sprite的比例
	m_pDevice->SetRenderState(D3DRS_POINTSIZE, FtoDW(size));     // 设置sprite的尺寸
	m_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(min));  // 设置点状sprite的最小尺寸
	m_pDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(a));     // 根据距离更改点状sprite的形状
	m_pDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(b));     // 根据距离更改点状sprite的形状
	m_pDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(c));     // 根据距离更改点状sprite的形状
}

// 禁用点状sprite
void CD3DRenderer::DisablePointSprites()
{
	m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);   // 禁用sprite
	m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);    // 禁用sprite比例
}

// 添加背景图控件
#ifdef UNICODE
bool CD3DRenderer::AddGUIBackdrop(int guiId, TCHAR *fileName)
#else
bool CD3DRenderer::AddGUIBackdrop(int guiId, char *fileName)
#endif 
{
	if (guiId >= m_totalGUIs)
	{
		return false;
	}

	UINT texID = 0, staticID = 0;

	// 添加纹理对象到纹理对象链表中
	int nRet =  AddTexture2D(fileName, (INT*)&texID);
	if (!nRet)
	{
		return false;
	}

	unsigned long col = D3DCOLOR_XRGB(255,255,255);

	// 填充GUI顶点结构数组
	stGUIVertex obj[] =
	{
		{0, (float)m_screenHeight, 0, 1, col, 0, 1},
		{0, 0, 0, 1, col, 0, 0},
		{(float)m_screenWidth, (float)m_screenHeight, 0, 1, col, 1, 1},
		{(float)m_screenWidth, 0, 0, 1, col, 1, 0}
	};

	// 创建静态缓存到链表中
	if (! CreateStaticBuffer(GUI_FVF, TRIANGLE_STRIP, 4, 0,
		sizeof(stGUIVertex), (void**)&obj, NULL,
		&staticID) )
	{
		return false;
	}

	// 添加背景图控件到链表，并返回布尔值
	return m_guiList[guiId].AddBackdrop(texID, staticID);
}


// 添加静态文本控件
#ifdef UNICODE
bool CD3DRenderer::AddGUIStaticText(int guiId, int id, TCHAR *text,
									int x, int y, unsigned long color, int fontID)
#else
bool CD3DRenderer::AddGUIStaticText(int guiId, int id, char *text,
									int x, int y, unsigned long color, int fontID)
#endif
{
	if (guiId >= m_totalGUIs)
	{
		return false;
	}

	// 添加静态文本控件到链表，并返回布尔值
	return m_guiList[guiId].AddStaticText(id, text, x, y, color, fontID);
}


// 添加按钮控件
#ifdef UNICODE
bool CD3DRenderer::AddGUIButton(int guiId, int id, int x, int y,
								TCHAR *up, TCHAR *over, TCHAR *down)
#else
bool CD3DRenderer::AddGUIButton(int guiId, int id, int x, int y,
								char *up, char *over, char *down)
#endif
{
	if (guiId >= m_totalGUIs)
	{
		return false;
	}

	UINT upID = 0, overID = 0, downID = 0, staticID = 0;

	// 添加按钮弹起状态的纹理对象
	if (! AddTexture2D(up, (INT*)&upID) )
	{
		return false;
	}
	// 添加鼠标停靠在按钮上的状态的纹理对象
	if (! AddTexture2D(over, (INT*)&overID) )
	{
		return false;
	}
	// 添加按钮被按下状态的纹理对象
	if (! AddTexture2D(down, (INT*)&downID) )
	{
		return false;
	}

	unsigned long col = D3DCOLOR_XRGB(255,255,255);

	int w = m_textureList[upID].width;
	int h = m_textureList[upID].height;

	// 填充GUI顶点结构
	stGUIVertex obj[] =
	{
		{(float)(w + x), (float)(0 + y), 0, 1, col, 1, 0},
		{(float)(w + x), (float)(h + y), 0, 1, col, 1, 1},
		{(float)(0 + x), (float)(0 + y), 0, 1, col, 0, 0},
		{(float)(0 + x), (float)(h + y), 0, 1, col, 0, 1},
	};

	// 创建顶点缓存到链表中
	if (! CreateStaticBuffer(GUI_FVF, TRIANGLE_STRIP, 4, 0,
		sizeof(stGUIVertex), (void**)&obj, NULL,
		&staticID) )
	{
		return false;
	}

	// 添加按钮控件，并返回布尔值
	return m_guiList[guiId].AddButton(id, x, y, w, h, upID, overID, downID, staticID);
}

void CD3DRenderer::ProcessGUI(
							  int guiId,      // GUI控件对象ID
							  bool LMBDown,   // 鼠标左键是否被按下
							  int mouseX, int mouseY,  // 鼠标的X、Y坐标
							  void(*funcPtr)(int id, int state)  // 指向回调函数
							  )
{
	if (guiId >= m_totalGUIs || !m_pDevice)
	{
		return;
	}

	// gui指向GUI系统对象数组中的guiId系统对象
	CGUISystem *gui = &m_guiList[guiId];

	if (!gui)
	{
		return;
	}

	// backDrop指向获得的背景图控件对象
	stGUIControl *backDrop = gui->GetBackDrop();

	// 绘制背景图纹理
	if (backDrop)
	{
		ApplyTexture(0, backDrop->m_upTex);  // 设置背景图纹理
		Render(backDrop->m_listID);          // 绘制该对象纹理
		//ApplyTexture(0, -1);                 // ??
	}

	// Initial button state.
	int status = UGP_BUTTON_UP;

	// 遍历GUI控件对象数组，并逐个
	for (int i=0; i<gui->GetTotalControls(); i++)
	{
		// 指向第i个GUI控件对象
		stGUIControl *pCnt = gui->GetGUIControl(i);

		if (!pCnt)
		{
			continue;
		}

		// 匹配控件对象的类型，并做出响应
		switch (pCnt->m_type)
		{
			// 静态文本控件
		case UGP_GUI_STATICTEXT:
			// 绘制该文本
		{
			DisplayText(pCnt->m_listID, pCnt->m_xPos, pCnt->m_yPos,
				pCnt->m_color, pCnt->m_text);

			break;
		}
		case UGP_GUI_FPS:
		{
			DisplayFPS(pCnt->m_listID, pCnt->m_xPos, pCnt->m_yPos);
			break;
		}
			// 按钮控件
		case UGP_GUI_BUTTON:
			status = UGP_BUTTON_UP;  // 设置按钮为弹起状态

			// 启用纹理混合运算
			m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			// 设置源混合因子
			m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

			// 设置目标混合因子
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// 若鼠标在按纽控件范围内
			if (mouseX > pCnt->m_xPos && mouseX < (pCnt->m_xPos + pCnt->m_width)
				&& mouseY > pCnt->m_yPos && mouseY < (pCnt->m_yPos + pCnt->m_height))
			{
				// 若鼠标左键被按下
				if (LMBDown)
				{
					// 设置为按下状态
					status = UGP_BUTTON_DOWN;
				}
				else
				{
					// 设置为停靠状态
					status = UGP_BUTTON_OVER;
				}
			}

			// 若按钮为弹起状态
			if (status == UGP_BUTTON_UP)
			{
				// 设置弹起状态的纹理
				ApplyTexture(0, pCnt->m_upTex);
			}
			// 若鼠标为停靠状态
			if (status == UGP_BUTTON_OVER)
			{
				// 设置停靠状态的纹理
				//ApplyTexture
				// 设置按钮被按下状态的纹理
				ApplyTexture(0, pCnt->m_downTex);
			}

			// 绘制按钮
			Render(pCnt->m_listID);

			// 禁用混合运算
			m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			break;
		}

		// 调用回调函数，以对按钮事件做出响应
		if (funcPtr)
		{
			funcPtr(pCnt->m_id, status);
		}
	}
}
#ifdef UNICODE
bool CD3DRenderer::CreateText(TCHAR *font, int weight, bool italic,
							  int size, int &id)
#else 
bool CD3DRenderer::CreateText(char *font, int weight, bool italic,
							  int size, int &id)
#endif
{
	if(!m_fonts)
	{
		m_fonts = new LPD3DXFONT[1];
		if(!m_fonts) return GM_FAIL;
	}
	else
	{
		LPD3DXFONT *temp;
		temp = new LPD3DXFONT[m_totalFonts + 1];

		memcpy(temp, m_fonts,
			sizeof(LPD3DXFONT) * m_totalFonts);

		delete[] m_fonts;
		m_fonts = temp;
	}

	if(FAILED(D3DXCreateFont(m_pDevice, size, 0, weight, 1, italic,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, font,
		&m_fonts[m_totalFonts]))) return false;

	id = m_totalFonts;
	m_totalFonts++;

	return true;
}

#ifdef UNICODE
void CD3DRenderer::DisplayText(int id, long x, long y,
							   int r, int g, int b, TCHAR *text, ...)
#else
void CD3DRenderer::DisplayText(int id, long x, long y,
							   int r, int g, int b, char *text, ...)
#endif

{
	RECT FontPosition = {x, y, m_screenWidth, m_screenWidth};
#ifdef UNICODE
	TCHAR message[1024] = {};
#else
	char message[1024];
#endif
	
	va_list argList;

	if(id >= m_totalFonts) return;

	va_start(argList, text);
	
#ifdef UNICODE
	vswprintf_s(message, 1024, text, argList);
#else 
	vsprintf(message, text, argList);
#endif
	
	va_end(argList);

	m_fonts[id]->DrawText(NULL, message, -1, &FontPosition,
		DT_SINGLELINE, D3DCOLOR_ARGB(255, r, g, b));
}

#ifdef UNICODE
void CD3DRenderer::DisplayText(int id, long x, long y,
							   unsigned long color, TCHAR *text, ...)
#else
void CD3DRenderer::DisplayText(int id, long x, long y,
							   unsigned long color, char *text, ...)
#endif
{
	RECT FontPosition = {x, y, m_screenWidth, m_screenWidth};
#ifdef UNICODE
	TCHAR message[1024] = {};
#else
	char message[1024];
#endif
	va_list argList;
	va_start(argList, text);
	if(id >= m_totalFonts) return;
#ifdef UNICODE
	vswprintf_s(message,1024,text, argList);
#else 
	vsprintf(message, text, argList);
#endif
	va_end(argList);
	m_fonts[id]->DrawText(NULL, message, -1, &FontPosition,
		DT_SINGLELINE, color);
}

void CD3DRenderer::EnableFog(float start, float end,
							 UGP_FOG_TYPE type, unsigned long color, bool rangeFog)
{
	if(!m_pDevice) return;

	D3DCAPS9 caps;
	m_pDevice->GetDeviceCaps(&caps);

	// Set fog properties.
	m_pDevice->SetRenderState(D3DRS_FOGENABLE, true);
	m_pDevice->SetRenderState(D3DRS_FOGCOLOR, color);

	// Start and end dist of fog.
	m_pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&start));
	m_pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&end));

	// Set based on type.
	if(type == UGP_VERTEX_FOG)
		m_pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
	else
		m_pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

	// Can only use if hardware supports it.
	if(caps.RasterCaps & D3DPRASTERCAPS_FOGRANGE)
	{
		if(rangeFog)
			m_pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, true);
		else
			m_pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, false);
	}
}


void CD3DRenderer::DisableFog()
{
	if(!m_pDevice) return;

	// Set fog properties.
	m_pDevice->SetRenderState(D3DRS_FOGENABLE, false);
}

// 设置细节映射
void CD3DRenderer::SetDetailMapping()
{
	if(!m_pDevice) return;

	m_pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP,
		D3DTOP_SELECTARG1);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1,
		D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2,
		D3DTA_DIFFUSE);

	m_pDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
	m_pDevice->SetTextureStageState(1, D3DTSS_COLOROP,
		D3DTOP_ADDSIGNED);
	m_pDevice->SetTextureStageState(1, D3DTSS_COLORARG1,
		D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(1, D3DTSS_COLORARG2,
		D3DTA_CURRENT);
}

void CD3DRenderer::DisplayFPS(int id, long x, long y)
{
	static float lastTime = (float)timeGetTime(); 
	float currTime  = (float)timeGetTime();
	float timeDelta = (currTime - lastTime)*0.001f;
	//
	// Update: Compute the frames per second.
	//
	FrameCnt++;
	TimeElapsed += timeDelta;

	if(TimeElapsed >= 1.0f)
	{
		FPS = (float)FrameCnt / TimeElapsed;


		StringCchPrintf(FPSString, 9, _T("%f"), FPS);
		FPSString[8] = _T('\0'); // mark end of string

		TimeElapsed = 0.0f;
		FrameCnt    = 0;
	}

	//
	// Render
	//

	RECT FontPosition = {x, y, m_screenWidth, m_screenHeight};

	HRESULT hr = S_OK;

	hr = m_fonts[id]->DrawText(NULL, FPSString, -1, &FontPosition,
		DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 255, 255));

	lastTime = currTime;
}