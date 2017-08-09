#include "stdafx.h"
#include "EngineInterface.h"

#include "D3DRenderer.h"
#include "DirectInput.h"

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

BOOL CreateDIInput(CInputInterface **pObj, HWND hwnd, HINSTANCE hInst, bool exclusive)
{
	if(!*pObj) *pObj = new CDirectInputSystem(hwnd, hInst, exclusive);
	else return false;

	return true;
}