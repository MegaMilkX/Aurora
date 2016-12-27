#ifndef RENDERSTATE_H
#define RENDERSTATE_H

#include "attribute.h"

namespace Au{
namespace GFX{

class RenderState
{
public:
    void AttribFormat(const std::vector<AttribInfo>& vertexFormat);
    void ShaderStage(STAGE stage, const std::string& source);
};
   
}
}

#endif
