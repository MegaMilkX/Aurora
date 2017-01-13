#ifndef RENDER_SEQUENCE_H
#define RENDER_SEQUENCE_H

#include "render-state.h"
#include "mesh.h"

namespace Au{
namespace GFX{

class RenderSequence
{
public:
    RenderSequence& operator<<(RenderState* state);
    RenderSequence& operator<<(Mesh* mesh);
};

}
}

#endif
