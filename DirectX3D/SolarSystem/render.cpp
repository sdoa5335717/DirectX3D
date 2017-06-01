//#include "renderGDI.h"
#include "renderD3D.h"

CRender* CRender::create(RenderType type)
{
    return new CRenderD3D;
}
