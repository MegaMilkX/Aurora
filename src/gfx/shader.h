#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <vector>
#include <string>
#include "gl/glextutil.h"

#include "attribute.h"

#include "../math.h"

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
    
    void AddUniform(IUniform* uniform);
    
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, Math::Vec2f& value);
    void SetUniform(const std::string& name, Math::Vec3f& value);
    void SetUniform(const std::string& name, Math::Vec4f& value);
    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, Math::Vec2i& value);
    void SetUniform(const std::string& name, Math::Vec3i& value);
    void SetUniform(const std::string& name, Math::Vec4i& value);
    void SetUniform(const std::string& name, unsigned int value);
    void SetUniform(const std::string& name, Math::Vec2ui& value);
    void SetUniform(const std::string& name, Math::Vec3ui& value);
    void SetUniform(const std::string& name, Math::Vec4ui& value);
    void SetUniform(const std::string& name, Math::Mat3f& value);
    void SetUniform(const std::string& name, Math::Mat4f& value);
    
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
