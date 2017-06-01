#include <windows.h>
#include <tchar.h>
#include "CommonFile.h"
#include "SolarSystem.h"
#define WINDOW_WIDTH 1440
#define WINDOW_HEIGHT 900

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)WndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hinstance;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = _T("SolarSystem");

	if( !RegisterClass(&wc) ) 
	{
		::MessageBox(0, _T("RegisterClass() - FAILED"), 0, 0);
		return false;
	}
		
	HWND hWnd = 0;
	hWnd = ::CreateWindow(_T("SolarSystem"), _T("SolarSystem"), 
		WS_EX_TOPMOST,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		0 /*parent hwnd*/, 0 /* menu */, hinstance, 0 /*extra*/); 

	if( !hWnd )
	{
		::MessageBox(0, _T("CreateWindow() - FAILED"), 0, 0);
		return false;
	}

	::ShowWindow(hWnd, SW_SHOW);
	::UpdateWindow(hWnd);


	CSolarSystem solar(100.0f);
	solar.CreateRenderEngine(hWnd);
	solar.CreateSolarSystem();
	
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	while(msg.message != WM_QUIT)
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{	
			solar.RenderSolarSystem();
		}
	}
	return 0;
}


