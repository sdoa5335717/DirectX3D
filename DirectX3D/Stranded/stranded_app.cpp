#include "stranded_app.h"

HWND g_hWnd;
CRenderInterface *g_Render = NULL;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
			return 0;
		}
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE p, LPSTR cmd, int show)
{
	WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc,
					0L,0L, GetModuleHandle(NULL), NULL, NULL,
					NULL, NULL, WINDOW_CLASS, NULL };

	RegisterClassEx(&wc);

	if (FULLSCREEN)
	{
		g_hWnd = CreateWindowEx(NULL, WINDOW_CLASS, WINDOW_NAME, WS_POPUP|WS_SYSMENU|WS_VISIBLE, 0,0, WIN_WIDTH, WIN_HEIGHT, 
					NULL, NULL, h, NULL);
	}
	else
	{
		g_hWnd = CreateWindowEx(NULL, WINDOW_CLASS, WINDOW_NAME, WS_OVERLAPPEDWINDOW|WS_VISIBLE,
			0,0, WIN_WIDTH, WIN_HEIGHT, NULL, NULL, h, NULL);
	}

	if (g_hWnd)
	{
		ShowWindow(g_hWnd, SW_SHOWDEFAULT);
		UpdateWindow(g_hWnd);
	}

	if (InitializeEngine())
	{
		if (GameInitialize())
		{
			MSG msg;
			ZeroMemory((void*)&msg,sizeof(msg));

			SetCursorPos(0,0);

			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					GameLoop();
				}
			}
		}
	}

	GameShutDown();
	ShutdownEngine();
	UnregisterClass(WINDOW_CLASS, wc.hInstance);

	return 0;
}

bool InitializeEngine()
{
	if (!CreateD3DRenderer(&g_Render))
	{
		return FALSE;
	}
	if (!g_Render->Initialize(WIN_WIDTH, WIN_HEIGHT, g_hWnd, FULLSCREEN))
	{
		return FALSE;
	}

	g_Render->SetClearColor(0,0,0);

	return TRUE;
}

void ShutdownEngine()
{
	if (g_Render)
	{
		g_Render->ShutDown();
		delete g_Render;
		g_Render = NULL;
	}
}

bool GameInitialize()
{
	return TRUE;
}

void GameLoop()
{
	if (!g_Render)
	{
		return;
	}

	g_Render->StartRender(1,1,0);

	g_Render->EndRendering();
}

void GameShutDown()
{

}