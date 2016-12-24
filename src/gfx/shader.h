#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <vector>
#include <string>
#include "gl/glextutil.h"

#include "attribute.h"

namespace Au{
namespace GFX{
    
class Shader
{
public:
    enum STAGE
    {
        VERTEX,
        PIXEL
    };
    
    void AddStage(STAGE stage, const std::string& source);
    bool Compile();
    void AttribFormat(const std::vector<AttribInfo>& vertexFormat);
    
    void Bind();
    
    std::string StatusString(){ return status_string; }
private:
    std::map<STAGE, std::string> stages;
    std::string status_string;
    unsigned int program;
    
    std::vector<AttribInfo> vertexFormat;
};
    
}
}

#endif
