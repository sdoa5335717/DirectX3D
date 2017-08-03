#include "stranded_app.h"

HWND g_hWnd;

// GUI�ؼ�ID
int g_mainGui = -1;     // ���˵�����
int g_startGui = -1;    // ��ʼ�˵�����
int g_creditsGui = -1;  // ��Ļ�˵�����
int g_currentGUI = GUI_MAIN_SCREEN;   // ��ǰ�˵�����

// ��������ID
int g_arialID = -1;

// ����״̬��Ϣ
bool LMBDown = false;         // �������Ƿ񱻰���
int	mouseX = 0, mouseY = 0;   // ���ָ��λ��(X��Y)

CRenderInterface *g_Render = NULL;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
			break;
		}
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
			return 0;
		}
		// ������������
	case WM_LBUTTONDOWN:
		{
			LMBDown = true;
			break;
		}
		// ����������
	case WM_LBUTTONUP:
		{
			LMBDown = false;
			break;
		}

		// ����ƶ�
	case WM_MOUSEMOVE:
		{
			// ��ȡ����XY����
			mouseX = LOWORD (lParam);
			mouseY = HIWORD (lParam);
			break;
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
	if (!g_Render->Initialize(WIN_WIDTH, WIN_HEIGHT, g_hWnd, FULLSCREEN, UGP_MS_SAMPLES_4))
	{
		return FALSE;
	}

	g_Render->SetClearColor(0,0,0);
	// ΪҪ��ʾ���ı�����Arial����

	if(! g_Render->CreateText(_T("Arial"), 0, true, 18, g_arialID) )
	{
		return false;
	}
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
	// �˵������ʼ��
	if (! InitializeMainMenu() )
	{
		return false;
	}
	return TRUE;
}

void GameLoop()
{
	if (!g_Render)
	{
		return;
	}

	//g_Render->StartRender(1,1,0);
	// ���Ʋ˵�����
	MainMenuRender();

	//g_Render->EndRendering();
}

void GameShutDown()
{

}

// ���˵������ʼ��
bool InitializeMainMenu()
{
	// Create gui screens.
	if(!g_Render->CreateGUI(g_mainGui)) return false;
	if(!g_Render->CreateGUI(g_startGui)) return false;
	if(!g_Render->CreateGUI(g_creditsGui)) return false;

	// Load backdrops.
	if(!g_Render->AddGUIBackdrop(g_mainGui, _T("menu/mainMenu.jpg")))
		return false;
	if(!g_Render->AddGUIBackdrop(g_startGui, _T("menu/startMenu.jpg")))
		return false;
	if(!g_Render->AddGUIBackdrop(g_creditsGui,
		_T("menu/creditsMenu.jpg"))) return false;

	// Set main screen elements.
	if(!g_Render->AddGUIStaticText(
		g_mainGui,
		STATIC_TEXT_ID,
		_T("Version: 1.0"),
		PERCENT_OF(WIN_WIDTH, 0.85),
		PERCENT_OF(WIN_WIDTH, 0.05),
		UGPCOLOR_ARGB(255,255,255,255),
		g_arialID)) return false;

	if(!g_Render->AddGUIButton(g_mainGui, BUTTON_START_ID,
		PERCENT_OF(WIN_WIDTH, 0.05), PERCENT_OF(WIN_HEIGHT, 0.40),
		_T("menu/startUp.png"), _T("menu/StartOver.png"),
		_T("menu/startDown.png"))) return false;

	if(!g_Render->AddGUIButton(g_mainGui, BUTTON_CREDITS_ID,
		PERCENT_OF(WIN_WIDTH, 0.05), PERCENT_OF(WIN_HEIGHT, 0.50),
		_T("menu/creditsUp.png"), _T("menu/creditsOver.png"),
		_T("menu/creditsDown.png"))) return false;

	if(!g_Render->AddGUIButton(g_mainGui, BUTTON_QUIT_ID,
		PERCENT_OF(WIN_WIDTH, 0.05), PERCENT_OF(WIN_HEIGHT, 0.60),
		_T("menu/quitUp.png"), _T("menu/quitOver.png"),
		_T("menu/quitDown.png"))) return false;


	// Set start screen elements.
	if(!g_Render->AddGUIButton(g_startGui, BUTTON_LEVEL_1_ID,
		PERCENT_OF(WIN_WIDTH, 0.1), PERCENT_OF(WIN_HEIGHT, 0.15),
		_T("menu/level1Up.png"), _T("menu/level1Over.png"),
		_T("menu/level1Down.png"))) return false;

	if(!g_Render->AddGUIButton(g_startGui, BUTTON_BACK_ID,
		PERCENT_OF(WIN_WIDTH, 0.1), PERCENT_OF(WIN_HEIGHT, 0.80),
		_T("menu/backUp.png"), _T("menu/backOver.png"),
		_T("menu/backDown.png"))) return false;


	// Set credits screen elements.
	if(!g_Render->AddGUIStaticText(g_creditsGui, STATIC_TEXT_ID,
		_T("Game Design -"),
		PERCENT_OF(WIN_WIDTH, 0.1), PERCENT_OF(WIN_HEIGHT, 0.15),
		UGPCOLOR_ARGB(255,255,255,255),
		g_arialID)) return false;

	if(!g_Render->AddGUIStaticText(g_creditsGui, STATIC_TEXT_ID,
		_T("              Allen Sherrod"),
		PERCENT_OF(WIN_WIDTH, 0.1), PERCENT_OF(WIN_HEIGHT, 0.20),
		UGPCOLOR_ARGB(255,255,255,255),
		g_arialID)) return false;

	if(!g_Render->AddGUIStaticText(g_creditsGui, STATIC_TEXT_ID,
		_T("Programming -"),
		PERCENT_OF(WIN_WIDTH, 0.1), PERCENT_OF(WIN_HEIGHT, 0.25),
		UGPCOLOR_ARGB(255,255,255,255),
		g_arialID)) return false;

	if(!g_Render->AddGUIStaticText(g_creditsGui, STATIC_TEXT_ID,
		_T("              Allen Sherrod"),
		PERCENT_OF(WIN_WIDTH, 0.1), PERCENT_OF(WIN_HEIGHT, 0.30),
		UGPCOLOR_ARGB(255,255,255,255),
		g_arialID)) return false;

	if(!g_Render->AddGUIStaticText(g_creditsGui, STATIC_TEXT_ID,
		_T("Sound -"),
		PERCENT_OF(WIN_WIDTH, 0.1), PERCENT_OF(WIN_HEIGHT, 0.35),
		UGPCOLOR_ARGB(255,255,255,255),
		g_arialID)) return false;

	if(!g_Render->AddGUIStaticText(g_creditsGui, STATIC_TEXT_ID,
		_T("              Allen Sherrod"),
		PERCENT_OF(WIN_WIDTH, 0.1), PERCENT_OF(WIN_HEIGHT, 0.40),
		UGPCOLOR_ARGB(255,255,255,255),
		g_arialID)) return false;

	if(!g_Render->AddGUIStaticText(g_creditsGui, STATIC_TEXT_ID,
		_T("Level Design -"),
		PERCENT_OF(WIN_WIDTH, 0.1), PERCENT_OF(WIN_HEIGHT, 0.45),
		UGPCOLOR_ARGB(255,255,255,255),
		g_arialID)) return false;

	if(!g_Render->AddGUIStaticText(g_creditsGui, STATIC_TEXT_ID,
		_T("              Allen Sherrod"),
		PERCENT_OF(WIN_WIDTH, 0.1), PERCENT_OF(WIN_HEIGHT, 0.50),
		UGPCOLOR_ARGB(255,255,255,255),
		g_arialID)) return false;

	if(!g_Render->AddGUIStaticText(g_creditsGui, STATIC_TEXT_ID,
		_T("Speical Thanks -"),
		PERCENT_OF(WIN_WIDTH, 0.1), PERCENT_OF(WIN_HEIGHT, 0.55),
		UGPCOLOR_ARGB(255,255,255,255),
		g_arialID)) return false;

	if(!g_Render->AddGUIStaticText(g_creditsGui, STATIC_TEXT_ID,
		_T("              Jenifer Niles"),
		PERCENT_OF(WIN_WIDTH, 0.1), PERCENT_OF(WIN_HEIGHT, 0.60),
		UGPCOLOR_ARGB(255,255,255,255),
		g_arialID)) return false;

	if(!g_Render->AddGUIStaticText(g_creditsGui, STATIC_TEXT_ID,
		_T("              Bryan Davidson"),
		PERCENT_OF(WIN_WIDTH, 0.1), PERCENT_OF(WIN_HEIGHT, 0.65),
		UGPCOLOR_ARGB(255,255,255,255),
		g_arialID)) return false;

	if(!g_Render->AddGUIStaticText(g_creditsGui, STATIC_TEXT_ID,
		_T("              Charles River Media"),
		PERCENT_OF(WIN_WIDTH, 0.1), PERCENT_OF(WIN_HEIGHT, 0.70),
		UGPCOLOR_ARGB(255,255,255,255),
		g_arialID)) return false;

	if(!g_Render->AddGUIStaticText(g_creditsGui, STATIC_TEXT_ID,
		_T("              Readers of this book"),
		PERCENT_OF(WIN_WIDTH, 0.1), PERCENT_OF(WIN_HEIGHT, 0.75),
		UGPCOLOR_ARGB(255,255,255,255),
		g_arialID)) return false;

	if(!g_Render->AddGUIButton(g_creditsGui, BUTTON_BACK_ID,
		PERCENT_OF(WIN_WIDTH, 0.1), PERCENT_OF(WIN_HEIGHT, 0.80),
		_T("menu/backUp.png"), _T("menu/backOver.png"),
		_T("menu/backDown.png"))) return false;

	return true;
}


// �˵�����Ļص���������ProcessGUI()�������ã�
void MainMenuCallback(int id, int state)
{
	switch(id)
	{
		// ��ʼ��ť�����µ����
	case BUTTON_START_ID:
		if (state == UGP_BUTTON_DOWN)
		{
			g_currentGUI = GUI_START_SCREEN;
		}
		break;

		// ��Ļ��ť�����µ����
	case BUTTON_CREDITS_ID:
		if (state == UGP_BUTTON_DOWN)
		{
			g_currentGUI = GUI_CREDITS_SCREEN;
		}
		break;

		// ���ذ�ť�����µ����
	case BUTTON_BACK_ID:
		if (state == UGP_BUTTON_DOWN)
		{
			g_currentGUI = GUI_MAIN_SCREEN;
		}
		break;

		// �˳���ť�����µ����
	case BUTTON_QUIT_ID:
		if (state == UGP_BUTTON_DOWN)
		{
			PostQuitMessage(0);
		}
		break;

		// ����1��ť�����µ����
	case BUTTON_LEVEL_1_ID:
		// Start first level.
		break;
	}
}


void MainMenuRender()
{
	if(!g_Render) return;

	g_Render->StartRender(1, 1, 0);

	if(g_currentGUI == GUI_MAIN_SCREEN)
		g_Render->ProcessGUI(g_mainGui, LMBDown, mouseX,
		mouseY, MainMenuCallback);
	else if(g_currentGUI == GUI_START_SCREEN)
		g_Render->ProcessGUI(g_startGui, LMBDown, mouseX,
		mouseY, MainMenuCallback);
	else
		g_Render->ProcessGUI(g_creditsGui, LMBDown, mouseX,
		mouseY, MainMenuCallback);

	g_Render->EndRendering();
}