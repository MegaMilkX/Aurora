#ifndef RENDER_SEQUENCE_H
#define RENDER_SEQUENCE_H

#include <vector>

#include "device.h"

namespace Au{
namespace GFX{

class RenderSequence
{
public:
    RenderSequence(Device* device)
    : device(device) {}
    template<typename T>
    RenderSequence& operator<<(T* data) { return *this; }
    template<typename T>
    RenderSequence& operator<<(const T& data)
    {
        currentUniform = data;
        return *this;
    }
private:
    Device* device;
    Uniform currentUniform;
};

template<>
inline RenderSequence& RenderSequence::operator<<(RenderState* state)
{
    device->Bind(state);
    return *this;
}
template<>
inline RenderSequence& RenderSequence::operator<<(Mesh* mesh)
{
    device->Bind(mesh);
    device->Render();
    return *this;
}

}
}

#endif
