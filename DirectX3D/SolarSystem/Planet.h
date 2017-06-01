#pragma once
#include "CommonFile.h"
#include "Ellipse.h"

typedef struct star_info
{
	FLOAT m_fRadius;
	FLOAT m_fSpeed;
	CEllipse* m_pTrack;
}SInfo;

class CPlanet
{
public:
	// 创建轨道
	virtual vector<FPoint> CreateTrack() = 0;
	// 行星信息
	SInfo m_sInfo;
};

class CMercury : public CPlanet
{
public:
	CMercury(FLOAT fA, FLOAT fB, FLOAT fRadius = 0.0f, FLOAT fSpeed = 0.0f)
	{
		m_sInfo.m_fSpeed = 9.0f;
		m_sInfo.m_fRadius = 10.0f;
		m_sInfo.m_pTrack = new CEllipse(fA, fB);
	};
	// 水星轨道
	vector<FPoint> CreateTrack() { 
		m_vecTrack = m_sInfo.m_pTrack->CreateEllipse();
		return m_vecTrack;
	}

	vector<FPoint> m_vecTrack;
};

class CVenus : public CPlanet
{
public:
	CVenus(FLOAT fA, FLOAT fB,FLOAT fRadius = 0.0f, FLOAT fSpeed = 0.0f)
	{
		m_sInfo.m_fSpeed = 8.0f;
		m_sInfo.m_fRadius = 20.0f;
		m_sInfo.m_pTrack = new CEllipse(fA, fB);
	};
	// 金星轨道
	vector<FPoint> CreateTrack() { 
		m_vecTrack = m_sInfo.m_pTrack->CreateEllipse();
		return m_vecTrack;
	}

	vector<FPoint> m_vecTrack;
};

class CEarth : public CPlanet
{
public:
	CEarth(FLOAT fA, FLOAT fB,FLOAT fRadius = 0.0f, FLOAT fSpeed = 0.0f)
	{
		m_sInfo.m_fSpeed = 7.0f;
		m_sInfo.m_fRadius = 25.0f;
		m_sInfo.m_pTrack = new CEllipse(fA, fB);
	};
	// 金星轨道
	vector<FPoint> CreateTrack() { 
		m_vecTrack = m_sInfo.m_pTrack->CreateEllipse();
		return m_vecTrack;
	}

	vector<FPoint> m_vecTrack;
};
// 火星
class CMars : public CPlanet
{
public:
	CMars(FLOAT fA, FLOAT fB,FLOAT fRadius = 0.0f, FLOAT fSpeed = 0.0f)
	{
		m_sInfo.m_fSpeed = 6.0f;
		m_sInfo.m_fRadius = 19.0f;
		m_sInfo.m_pTrack = new CEllipse(fA, fB);
	};

	vector<FPoint> CreateTrack() { 
		m_vecTrack = m_sInfo.m_pTrack->CreateEllipse();
		return m_vecTrack;
	}

	vector<FPoint> m_vecTrack;
};
// 木星
class CJupiter : public CPlanet
{
public:
	CJupiter(FLOAT fA, FLOAT fB,FLOAT fRadius = 0.0f, FLOAT fSpeed = 0.0f)
	{
		m_sInfo.m_fSpeed = 5.0f;
		m_sInfo.m_fRadius = 40.0f;
		m_sInfo.m_pTrack = new CEllipse(fA, fB);
	};
	// 金星轨道
	vector<FPoint> CreateTrack() { 
		m_vecTrack = m_sInfo.m_pTrack->CreateEllipse();
		return m_vecTrack;
	}

	vector<FPoint> m_vecTrack;
};

// 土星
class CSaturn : public CPlanet
{
public:
	CSaturn(FLOAT fA, FLOAT fB,FLOAT fRadius = 0.0f, FLOAT fSpeed = 0.0f)
	{
		m_sInfo.m_fSpeed = 4.0f;
		m_sInfo.m_fRadius = 45.0f;
		m_sInfo.m_pTrack = new CEllipse(fA, fB);
	};
	// 金星轨道
	vector<FPoint> CreateTrack() { 
		m_vecTrack = m_sInfo.m_pTrack->CreateEllipse();
		return m_vecTrack;
	}

	vector<FPoint> m_vecTrack;
};

// 天王星
class CUranus : public CPlanet
{
public:
	CUranus(FLOAT fA, FLOAT fB,FLOAT fRadius = 0.0f, FLOAT fSpeed = 0.0f)
	{
		m_sInfo.m_fSpeed = 3.0f;
		m_sInfo.m_fRadius = 20.0f;
		m_sInfo.m_pTrack = new CEllipse(fA, fB);
	};
	// 金星轨道
	vector<FPoint> CreateTrack() { 
		m_vecTrack = m_sInfo.m_pTrack->CreateEllipse();
		return m_vecTrack;
	}

	vector<FPoint> m_vecTrack;
};
// 海王星
class CNeptune : public CPlanet
{
public:
	CNeptune(FLOAT fA, FLOAT fB,FLOAT fRadius = 0.0f, FLOAT fSpeed = 0.0f)
	{
		m_sInfo.m_fSpeed = 2.0f;
		m_sInfo.m_fRadius = 20.0f;
		m_sInfo.m_pTrack = new CEllipse(fA, fB);
	};
	// 海王星轨道
	vector<FPoint> CreateTrack() { 
		m_vecTrack = m_sInfo.m_pTrack->CreateEllipse();
		return m_vecTrack;
	}
	vector<FPoint> m_vecTrack;
};

// 冥王星
class CPluto : public CPlanet
{
public:
	CPluto(FLOAT fA, FLOAT fB,FLOAT fRadius = 0.0f, FLOAT fSpeed = 0.0f)
	{
		m_sInfo.m_fSpeed = 1.0f;
		m_sInfo.m_fRadius = 20.0f;
		m_sInfo.m_pTrack = new CEllipse(fA, fB);
	};
	// 冥王星轨道
	vector<FPoint> CreateTrack() { 
		m_vecTrack = m_sInfo.m_pTrack->CreateEllipse();
		return m_vecTrack;
	}
	vector<FPoint> m_vecTrack;
};
