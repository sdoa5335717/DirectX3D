#include "stdafx.h"
#include "D3DRenderer.h"

unsigned long CreateD3DFVF(int flags)
{
	unsigned long fvf = 0;

	return fvf;
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
}

CD3DRenderer::~CD3DRenderer()
{
	ShutDown();
}

BOOL CD3DRenderer::Initialize(int w, int h, WinHWND mainWin, BOOL fullScreen)
{
	ShutDown();

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

	m_nSrceenWidth = w;
	m_nScreenHeight= h;

	if (FAILED(m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hMainWnd, 
		processing, &Params, &m_pDevice)))
	{
		return FALSE;
	}

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
		if (m_staticBufferList[i].vbPtr)
		{
			m_staticBufferList[i].vbPtr->Release();
			m_staticBufferList[i].vbPtr = NULL;
		}

		if (m_staticBufferList[i].ibPtr)
		{
			m_staticBufferList[i].ibPtr->Release();
			m_staticBufferList[i].ibPtr = NULL;
		}
	}

	m_numStaticBuffers = 0;

	if (m_staticBufferList)
	{
		delete[] m_staticBufferList;
		m_staticBufferList = NULL;
	}

	if (m_pDevice){ m_pDevice->Release();}
	if (m_pDirect3D) { m_pDirect3D->Release();}

	m_pDevice = NULL;
	m_pDirect3D = NULL;
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
		m_staticBufferList = NULL;
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
		m_staticBufferList[index].ibPtr->Unlock();
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
	if (staticID>=m_numStaticBuffers)
	{
		return GM_FAIL;
	}

	if (m_numActiveStaticBuffer != staticID)
	{
		if (m_staticBufferList[staticID].ibPtr != NULL)
		{
			m_pDevice->SetIndices(m_staticBufferList[staticID].ibPtr);
		}

		m_pDevice->SetStreamSource(0, m_staticBufferList[staticID].vbPtr, 0, m_staticBufferList[staticID].stride);

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
				if (FAILED(m_pDevice->DrawPrimitive(
					D3DPT_TRIANGLESTRIP, 0,
					(int)(m_staticBufferList[staticID].numVerts/2))))
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