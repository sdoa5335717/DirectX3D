#ifndef RENDER_H
#define RENDER_H

#define WIN32_LEAN_AND_MEAN

class CRender
{
public:
    enum RenderType
    {
        GDI_RENDER,
        D3D_RENDER,
    };
    static CRender* create(RenderType type = D3D_RENDER);

    virtual bool init(HWND hView) = 0;
    virtual void destroy() = 0;
    //virtual void resize(unsigned int w, unsigned int h) = 0;
    //virtual void render(wkeWebView webView) = 0;
};

#endif