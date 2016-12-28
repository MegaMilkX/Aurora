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
    std::vector<IUniform*> uniforms;
    
    std::string statusString;
    
    void _createProgramIfNotExists();
};

template<typename T>
void RenderState::AddUniform(const std::string& name)
{
    Au::GFX::Uniform<T> uniform = Au::GFX::Uniform<T>::Get(name);
    
    uniformLocations.push_back(glGetUniformLocation(shaderProgram, uniform.Name().c_str()));
    uniforms.push_back(uniform.Copy());
}
   
}
}

#endif
