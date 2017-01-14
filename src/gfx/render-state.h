#ifndef RENDERSTATE_H
#define RENDERSTATE_H

#include <set>

#include "attribute.h"
#include "shader.h"
#include "uniform.h"

namespace Au{
namespace GFX{

class RenderState
{
public:
    RenderState();
    ~RenderState();
    void AttribFormat(const std::vector<AttribInfo>& vertexFormat);
    void SetShader(Shader* shaderStage);
    template<typename T>
    void AddUniform(const std::string& name);
    
    void Bind();
    
    std::string StatusString() { return statusString; }
private:
    GLuint shaderProgram;
    std::map<Shader::STAGE, unsigned int> shaders;
    std::vector<AttribInfo> vertexFormat;
    
    std::vector<unsigned int> uniformLocations;
    std::vector<Uniform> uniforms;
    
    std::string statusString;
    
    void _createProgramIfNotExists();
};

template<typename T>
void RenderState::AddUniform(const std::string& name)
{
    Uniform uniform = GetUniform<T>(name);
    
    uniformLocations.push_back(glGetUniformLocation(shaderProgram, name.c_str()));
    uniforms.push_back(uniform);
}
   
}
}

#endif
