#pragma once
#include "CommonFile.h"

class CEllipse
{
public:
	CEllipse(void);
	CEllipse(FLOAT fA, 
		FLOAT fB, FLOAT fX = 0.0f, FLOAT fY = 0.0f, FLOAT fTilt = 0.0f, FLOAT dParameter = 0.01f);
	~CEllipse(void);

	FLOAT m_fX;
	FLOAT m_fY;

	FLOAT m_fA;
	FLOAT m_fB;

	FLOAT m_fTilt;
	// 将点序列返回
	vector<FPoint> CreateEllipse();
private:
	FLOAT m_dParameter;
	vector<FPoint> m_vecPoint;
};
