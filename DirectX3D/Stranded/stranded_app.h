#ifndef STRANDED_APP_H
#define STRANDED_APP_H
#include <tchar.h>
#include "..\\GameEngine\\engine.h"

//
#pragma comment(lib, "..\\Debug\\GameEngine")

#define WINDOW_CLASS L"StrandedGame"
#define WINDOW_NAME	 L"Stranded"
#define WIN_WIDTH	800
#define WIN_HEIGHT	600
#define FULLSCREEN 0

bool InitializeEngine();
void ShutdownEngine();

// ����ļ��ж���ĺ���ԭ������
bool GameInitialize();	// ���������е���ض��󣬶������Ⱦ��������
void GameLoop();		
void GameShutDown();

// ���˵������ʼ��
bool InitializeMainMenu();
// �˵�����Ļص���������ProcessGUI()�������ã�
void MainMenuCallback(int id, int state);

void MainMenuRender();

// GUI���˵��������ض�����Ϣ
#define GUI_MAIN_SCREEN       1   // ���˵�����
#define GUI_START_SCREEN      2   // ��ʼ�˵�����
#define GUI_CREDITS_SCREEN    3   // ��Ļ�˵�����

// ids for our GUI controls.
#define STATIC_TEXT_ID     1
#define BUTTON_START_ID    2
#define BUTTON_CREDITS_ID  3
#define BUTTON_QUIT_ID     4
#define BUTTON_BACK_ID     5
#define BUTTON_LEVEL_1_ID  6

#endif