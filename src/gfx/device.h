#ifndef GFXDEVICE_H
#define GFXDEVICE_H

#include "../window/window.h"
#include "mesh.h"
#include "shader.h"

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
    Shader* CreateShader();
    void Destroy(Mesh* mesh);
    void Destroy(Shader* shader);
    
    int APIVersion();
private:
    HDC deviceContext;
    HGLRC context;
    HGLRC threadingContext;
    int contextVersion = 0;
};

}
}

#endif
