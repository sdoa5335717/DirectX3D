#include "Ellipse.h"

CEllipse::CEllipse(void)
{
}

CEllipse::~CEllipse(void)
{
}

CEllipse::CEllipse(FLOAT fA, 
		 FLOAT fB, FLOAT fX, FLOAT fY, FLOAT fTilt, FLOAT dParameter)
{
	// 中心点
	m_fX = fX;
	m_fY = fY;
	// 长短轴
	m_fA = fA;
	m_fB = fB;
	// 倾斜角
	m_fTilt = fTilt;
	// 
	m_dParameter = dParameter;
}

vector<FPoint> CEllipse::CreateEllipse()
{
	m_vecPoint.clear();

	FLOAT fStart = 0.0f;
	for (int i=0; i<(360/m_dParameter);i++)
	{
		FPoint fPoint;
		fPoint.fX = m_fA*(cos(m_dParameter*i*PI/180))*cos(m_fTilt*PI/180)-m_fB*sin(m_dParameter*i*PI/180)*sin(m_fTilt*PI/180)+m_fX;
		fPoint.fY = m_fA*(cos(m_dParameter*i*PI/180))*sin(m_fTilt*PI/180)-m_fB*sin(m_dParameter*i*PI/180)*cos(m_fTilt*PI/180)+m_fY;

		m_vecPoint.push_back(fPoint);
	}

	return m_vecPoint;
}