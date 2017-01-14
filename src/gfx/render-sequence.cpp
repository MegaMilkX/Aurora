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
        case UNIFORM_FLOAT:break;
        case UNIFORM_FLOAT2:break;
        case UNIFORM_FLOAT3:break;
        case UNIFORM_FLOAT4:break;
        case UNIFORM_INT:break;
        case UNIFORM_INT2:break;
        case UNIFORM_INT3:break;
        case UNIFORM_INT4:break;
        case UNIFORM_UINT:break;
        case UNIFORM_UINT2:break;
        case UNIFORM_UINT3:break;
        case UNIFORM_UINT4:break;
        case UNIFORM_MAT3F:break;
        case UNIFORM_MAT4F:break;
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

RenderSequence& RenderSequence::operator<<(float uniform)
{
    
}

RenderSequence& RenderSequence::operator<<(const Math::Vec2f& uniform)
{
    
}
RenderSequence& RenderSequence::operator<<(const Math::Vec3f& uniform)
{
    
}
RenderSequence& RenderSequence::operator<<(const Math::Vec4f& uniform)
{
    
}
RenderSequence& RenderSequence::operator<<(int uniform)
{
    
}
RenderSequence& RenderSequence::operator<<(const Math::Vec2i& uniform)
{
    
}
RenderSequence& RenderSequence::operator<<(const Math::Vec3i& uniform)
{
    
}
RenderSequence& RenderSequence::operator<<(const Math::Vec4i& uniform)
{
    
}
RenderSequence& RenderSequence::operator<<(unsigned int uniform)
{
    
}
RenderSequence& RenderSequence::operator<<(const Math::Vec2ui& uniform)
{
    
}
RenderSequence& RenderSequence::operator<<(const Math::Vec3ui& uniform)
{
    
}
RenderSequence& RenderSequence::operator<<(const Math::Vec4ui& uniform)
{
    
}
RenderSequence& RenderSequence::operator<<(const Math::Mat3f& uniform)
{
    
}
RenderSequence& RenderSequence::operator<<(const Math::Mat4f& uniform)
{
    
}
   
}
}