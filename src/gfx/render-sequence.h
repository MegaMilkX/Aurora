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
        RENDER,
        UNIFORM_FLOAT,
        UNIFORM_FLOAT2,
        UNIFORM_FLOAT3,
        UNIFORM_FLOAT4,
        UNIFORM_INT,
        UNIFORM_INT2,
        UNIFORM_INT3,
        UNIFORM_INT4,
        UNIFORM_UINT,
        UNIFORM_UINT2,
        UNIFORM_UINT3,
        UNIFORM_UINT4,
        UNIFORM_MAT3F,
        UNIFORM_MAT4F
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
    
    void Execute(Device* device);
    
    RenderSequence& operator<<(RenderState* state);
    RenderSequence& operator<<(Mesh* mesh);
    RenderSequence& operator<<(float uniform);
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
private:
    std::vector<Step> steps;
};

}
}

#endif
