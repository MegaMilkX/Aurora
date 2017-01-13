#include "render-sequence.h"

namespace Au{
namespace GFX{

RenderSequence& RenderSequence::operator<<(RenderState* state)
{
    steps.push_back(Step(BIND_RENDERSTATE, state));
    return *this;
}

RenderSequence& RenderSequence::operator<<(Mesh* mesh)
{
    steps.push_back(Step(BIND_MESH, mesh));
    steps.push_back(Step(RENDER_MESH, mesh));
    return *this;
}

void RenderSequence::Execute(Device* device)
{
    for(unsigned i = 0; i < steps.size(); ++i)
    {
        CMD cmd = steps[i].cmd;
        switch(cmd)
        {
        case BIND_RENDERSTATE:
            device->Bind((RenderState*)(steps[i].data));
            break;
        case BIND_MESH:
            device->Bind((Mesh*)(steps[i].data));
            break;
        case RENDER_MESH:
            device->Render();
            break;
        }
    }
}
   
}
}