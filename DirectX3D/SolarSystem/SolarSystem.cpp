#include "SolarSystem.h"

CSolarSystem::CSolarSystem(FLOAT fRadius)
{
	m_pRender = NULL;
	// 恒星
	m_fRadius = fRadius;
	// 行星
	CPlanet* pMercury = new CMercury(150.0f, 130.0f);
	m_vecPlanet.push_back(pMercury);

	CPlanet* pVenus = new CVenus(190.0f, 160.0f);
	m_vecPlanet.push_back(pVenus);

	CPlanet* pEarth = new CEarth(240.0f, 210.0f);
	m_vecPlanet.push_back(pEarth);

	CPlanet* pMars = new CMars(350.0f, 250.0f);
	m_vecPlanet.push_back(pMars);

	CPlanet* pJupiter = new CJupiter(480.0f, 400.0f);
	m_vecPlanet.push_back(pJupiter);

	CPlanet* pSaturn = new CSaturn(580.0f, 480.0f);
	m_vecPlanet.push_back(pSaturn);

	CPlanet* pUranus = new CUranus(730.0f, 630.0f);
	m_vecPlanet.push_back(pUranus);

	CPlanet* pNeptune = new CNeptune(830.0f, 730.0f);
	m_vecPlanet.push_back(pNeptune);

	CPlanet* pPluto = new CPluto(930.0f, 830.0f);
	m_vecPlanet.push_back(pPluto);

}

CSolarSystem::~CSolarSystem(void)
{
	for (unsigned int i=0; i<m_vecPlanet.size(); i++)
	{
		COMMON_CLASS_SAFEFREE(m_vecPlanet[i]);
	}
	m_vecPlanet.clear();
	COMMON_CLASS_SAFEFREE(m_pRender);
}
void CSolarSystem::CreateSolarSystem()
{
	if (m_pRender == NULL)
	{
		::MessageBox(0, _T("Render engine is not created!"), 0, 0);
		return;
	}
	// 创建恒星
	m_pRender->RenderStars(m_fRadius);
	// 创建行星运行轨道
	vector<FPoint> vecPoint;
	vector<vector<FPoint>> vecDraw;
	for (unsigned int i=0; i<m_vecPlanet.size();i++)
	{
		// 获得行星轨道参数
		vecDraw.push_back(m_vecPlanet[i]->CreateTrack());
		// 创建行星渲染模型
		m_pRender->RenderStars( m_vecPlanet[i]->m_sInfo.m_fRadius);

		vecPoint.clear();
	}
	m_pRender->RenderTracks(vecDraw);
	// 以恒星位置作为光源
	m_pRender->set_lighting(0.0f, 0.0f, 1.0f);
	m_pRender->set_projection();
	m_pRender->set_camera();
	m_pRender->SetStarsMatrial();
}
void CSolarSystem::RenderSolarSystem()
{
	if (m_pRender)
	{
		m_pRender->render();
	}
}
void CSolarSystem::CreateRenderEngine(HWND hWnd)
{
	m_pRender = new CSolarRenderD3D;
	m_pRender->initial_render(hWnd);
}