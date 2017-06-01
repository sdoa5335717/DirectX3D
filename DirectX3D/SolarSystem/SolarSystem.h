#pragma once

#include "CommonFile.h"
#include "Planet.h"
#include "Solar_render.h"

class CSolarSystem
{
public:
	CSolarSystem(FLOAT fRadius);

	~CSolarSystem(void);

	vector<CPlanet*> m_vecPlanet;

	void RenderSolarSystem();

	void CreateSolarSystem();

	void CreateRenderEngine(HWND hWnd);
private:
	FLOAT m_fRadius;
	// Draw Engine
	CSolarRenderD3D* m_pRender;
};
