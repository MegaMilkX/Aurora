#ifndef RENDER_SEQUENCE_H
#define RENDER_SEQUENCE_H

#include <vector>

#include "device.h"

namespace Au{
namespace GFX{

class RenderSequence
{
public:
    enum CMD
    {
        NO_CMD,
        BIND_MESH,
        BIND_RENDERSTATE,
        RENDER_MESH
    };
    
    struct Step
    {
        Step()
        : cmd(NO_CMD), data(0) {}
        Step(CMD cmd, void* data = 0)
        : cmd(cmd), data(data) {}
        CMD cmd;
        void* data;
    };
    
    RenderSequence& operator<<(RenderState* state);
    RenderSequence& operator<<(Mesh* mesh);
    
    void Execute(Device* device);
private:
    std::vector<Step> steps;
};

}
}

#endif
