#ifndef GFXDEVICE_H
#define GFXDEVICE_H

#include "../window/window.h"
#include "shader.h"

namespace Au{
namespace GFX{

class Device
{
public:
    bool Init(Window& window);
    void Cleanup();
    
    void Clear();
    void SwapBuffers();
    
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
