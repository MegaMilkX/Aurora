#include "render-sequence.h"

namespace Au{
namespace GFX{

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
        case RENDER:
            device->Render();
            break;
        }
    }
}

RenderSequence& RenderSequence::operator<<(RenderState* state)
{
    steps.push_back(Step(BIND_RENDERSTATE, state));
    return *this;
}

RenderSequence& RenderSequence::operator<<(Mesh* mesh)
{
    steps.push_back(Step(BIND_MESH, mesh));
    steps.push_back(Step(RENDER));
    return *this;
}

RenderSequence& operator<<(const char* uniformName)
{
    cachedUniformName = std::string(uniformName);
    return *this;
}

RenderSequence& operator<<(const std::string& uniformName)
{
    cachedUniformName = uniformName;
    return *this;
}

RenderSequence& operator<<(float uniform)
{
    
}

RenderSequence& operator<<(const Math::Vec2f& uniform);
RenderSequence& operator<<(const Math::Vec3f& uniform);
RenderSequence& operator<<(const Math::Vec4f& uniform);
RenderSequence& operator<<(int uniform);
RenderSequence& operator<<(const Math::Vec2i& uniform);
RenderSequence& operator<<(const Math::Vec3i& uniform);
RenderSequence& operator<<(const Math::Vec4i& uniform);
RenderSequence& operator<<(unsigned int uniform);
RenderSequence& operator<<(const Math::Vec2ui& uniform);
RenderSequence& operator<<(const Math::Vec3ui& uniform);
RenderSequence& operator<<(const Math::Vec4ui& uniform);
RenderSequence& operator<<(const Math::Mat3f& uniform);
RenderSequence& operator<<(const Math::Mat4f& uniform);
   
}
}