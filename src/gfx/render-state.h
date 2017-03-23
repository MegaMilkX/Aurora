#ifndef RENDERSTATE_H
#define RENDERSTATE_H

#include <set>

#include "../util/attribute.h"
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
    void AddUniform(const std::string& name, unsigned int count = 1);
    
    void DepthTest(bool);
    
    void Bind();
    
    std::string StatusString() { return statusString; }
private:
    GLuint shaderProgram;
    std::map<Shader::STAGE, unsigned int> shaders;
    std::vector<AttribInfo> vertexFormat;
    
    std::vector<unsigned int> uniformLocations;
    std::vector<Uniform> uniforms;
    
    std::string statusString;
    
    bool depthTest;
    
    void _createProgramIfNotExists();
    void _linkProgram();
};

template<typename T>
void RenderState::AddUniform(const std::string& name, unsigned int count)
{
    Uniform uniform = GetUniform<T>(name, count);
    
    uniformLocations.push_back(glGetUniformLocation(shaderProgram, name.c_str()));
    uniforms.push_back(uniform);
}
   
}
}

#endif
