#ifndef GFXDEVICE_H
#define GFXDEVICE_H

#include "../window/window.h"
#include "mesh.h"
#include "render-state.h"

namespace Au{
namespace GFX{

class Device
{
public:
    bool Init(Window& window);
    void Cleanup();
    
    void Clear();
    void Render();
    void SwapBuffers();
    
    Mesh* CreateMesh();
    Shader* CreateShader(Shader::STAGE stage);
    RenderState* CreateRenderState();
    void Destroy(Mesh* mesh);
    void Destroy(Shader* shader);
    
    void Bind(Mesh* mesh);
    void Bind(RenderState* state);
    
    int APIVersion();
private:
    HDC deviceContext;
    HGLRC context;
    HGLRC threadingContext;
    int contextVersion = 0;
    
    Mesh* boundMesh;
    RenderState* boundState;
};

}
}

#endif
