#ifndef ENGINEINTERFACE_H
#define ENGINEINTERFACE_H

#ifdef GAMEENGINE_EXPORTS
#define GAMEENGINE_API __declspec(dllexport)
#else
#define GAMEENGINE_API __declspec(dllimport)
#endif

#include "RenderInterface.h"
#include "InputInterface.h"

GAMEENGINE_API BOOL CreateD3DRenderer(CRenderInterface** pObj);
GAMEENGINE_API BOOL CreateDIInput(CInputInterface **pObj, HWND hwnd, HINSTANCE hInst, bool exclusive);

#endif
