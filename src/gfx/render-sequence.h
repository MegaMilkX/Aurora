#ifndef RENDER_SEQUENCE_H
#define RENDER_SEQUENCE_H

#include "device.h"

namespace Au{
namespace GFX{

class RenderSequence
{
public:
    RenderSequence& operator<<(RenderState* state);
    RenderSequence& operator<<(Mesh* mesh);
    
    void Execute(Device* device);
};

}
}

#endif
