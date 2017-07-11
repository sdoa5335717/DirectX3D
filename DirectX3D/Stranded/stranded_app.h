#ifndef STRANDED_APP_H
#define STRANDED_APP_H

#include "..\\GameEngine\\engine.h"
//
#pragma comment(lib, "..\\Debug\\GameEngine")

#define WINDOW_CLASS L"StrandedGame"
#define WINDOW_NAME	 L"Stranded"
#define WIN_WIDTH	800
#define WIN_HEIGHT	600
#define FULLSCREEN 1

bool InitializeEngine();
void  ShutdownEngine();

//
bool GameInitialize();
void GameLoop();
void GameShutDown();

#endif