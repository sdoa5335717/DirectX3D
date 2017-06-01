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
	// �������
	virtual vector<FPoint> CreateTrack() = 0;
	// ������Ϣ
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
	// ˮ�ǹ��
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
	// ���ǹ��
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
	// ���ǹ��
	vector<FPoint> CreateTrack() { 
		m_vecTrack = m_sInfo.m_pTrack->CreateEllipse();
		return m_vecTrack;
	}

	vector<FPoint> m_vecTrack;
};
// ����
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
// ľ��
class CJupiter : public CPlanet
{
public:
	CJupiter(FLOAT fA, FLOAT fB,FLOAT fRadius = 0.0f, FLOAT fSpeed = 0.0f)
	{
		m_sInfo.m_fSpeed = 5.0f;
		m_sInfo.m_fRadius = 40.0f;
		m_sInfo.m_pTrack = new CEllipse(fA, fB);
	};
	// ���ǹ��
	vector<FPoint> CreateTrack() { 
		m_vecTrack = m_sInfo.m_pTrack->CreateEllipse();
		return m_vecTrack;
	}

	vector<FPoint> m_vecTrack;
};

// ����
class CSaturn : public CPlanet
{
public:
	CSaturn(FLOAT fA, FLOAT fB,FLOAT fRadius = 0.0f, FLOAT fSpeed = 0.0f)
	{
		m_sInfo.m_fSpeed = 4.0f;
		m_sInfo.m_fRadius = 45.0f;
		m_sInfo.m_pTrack = new CEllipse(fA, fB);
	};
	// ���ǹ��
	vector<FPoint> CreateTrack() { 
		m_vecTrack = m_sInfo.m_pTrack->CreateEllipse();
		return m_vecTrack;
	}

	vector<FPoint> m_vecTrack;
};

// ������
class CUranus : public CPlanet
{
public:
	CUranus(FLOAT fA, FLOAT fB,FLOAT fRadius = 0.0f, FLOAT fSpeed = 0.0f)
	{
		m_sInfo.m_fSpeed = 3.0f;
		m_sInfo.m_fRadius = 20.0f;
		m_sInfo.m_pTrack = new CEllipse(fA, fB);
	};
	// ���ǹ��
	vector<FPoint> CreateTrack() { 
		m_vecTrack = m_sInfo.m_pTrack->CreateEllipse();
		return m_vecTrack;
	}

	vector<FPoint> m_vecTrack;
};
// ������
class CNeptune : public CPlanet
{
public:
	CNeptune(FLOAT fA, FLOAT fB,FLOAT fRadius = 0.0f, FLOAT fSpeed = 0.0f)
	{
		m_sInfo.m_fSpeed = 2.0f;
		m_sInfo.m_fRadius = 20.0f;
		m_sInfo.m_pTrack = new CEllipse(fA, fB);
	};
	// �����ǹ��
	vector<FPoint> CreateTrack() { 
		m_vecTrack = m_sInfo.m_pTrack->CreateEllipse();
		return m_vecTrack;
	}
	vector<FPoint> m_vecTrack;
};

// ڤ����
class CPluto : public CPlanet
{
public:
	CPluto(FLOAT fA, FLOAT fB,FLOAT fRadius = 0.0f, FLOAT fSpeed = 0.0f)
	{
		m_sInfo.m_fSpeed = 1.0f;
		m_sInfo.m_fRadius = 20.0f;
		m_sInfo.m_pTrack = new CEllipse(fA, fB);
	};
	// ڤ���ǹ��
	vector<FPoint> CreateTrack() { 
		m_vecTrack = m_sInfo.m_pTrack->CreateEllipse();
		return m_vecTrack;
	}
	vector<FPoint> m_vecTrack;
};
