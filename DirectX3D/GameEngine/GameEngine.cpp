// GameEngine.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "GameEngine.h"


// ���ǵ���������һ��ʾ��
GAMEENGINE_API int nGameEngine=0;

// ���ǵ���������һ��ʾ����
GAMEENGINE_API int fnGameEngine(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� GameEngine.h
CGameEngine::CGameEngine()
{
	return;
}
