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

// 入口文件中定义的函数原型声明
bool GameInitialize();	// 创建引擎中的相关对象，对相关渲染进行设置
void GameLoop();		
void GameShutDown();

// 主菜单界面初始化
bool InitializeMainMenu();
// 菜单界面的回调函数（由ProcessGUI()函数调用）
void MainMenuCallback(int id, int state);

void MainMenuRender();

// GUI主菜单界面的相关定义信息
#define GUI_MAIN_SCREEN       1   // 主菜单界面
#define GUI_START_SCREEN      2   // 开始菜单界面
#define GUI_CREDITS_SCREEN    3   // 字幕菜单界面

// ids for our GUI controls.
#define STATIC_TEXT_ID     1
#define BUTTON_START_ID    2
#define BUTTON_CREDITS_ID  3
#define BUTTON_QUIT_ID     4
#define BUTTON_BACK_ID     5
#define BUTTON_LEVEL_1_ID  6

#endif