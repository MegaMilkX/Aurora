#include "render-state.h"

namespace Au{
namespace GFX{

RenderState::RenderState()
: shaderProgram(0)
{
    
}

RenderState::~RenderState()
{
    if(shaderProgram)
        glDeleteProgram(shaderProgram);
}

void RenderState::AttribFormat(const std::vector<AttribInfo>& vertexFormat)
{
    _createProgramIfNotExists();
    
    std::cout << "Formatting shader attribs" << std::endl;
    this->vertexFormat = vertexFormat;
    std::map<typeindex, AttribInstance> instanceCounters;
    for(unsigned int i = 0; i < vertexFormat.size(); ++i)
    {
        const AttribInfo& attrInfo = vertexFormat[i];
        unsigned int instance = instanceCounters[attrInfo.typeIndex]++;
        AttribIndex globalAttribIndex = 
            GetGlobalAttribIndex(attrInfo.typeIndex, instance);
        
        std::string shaderInputName = attrInfo.name;
        if(instance > 0)
            shaderInputName += std::to_string(instance);
                                 
        glBindAttribLocation(shaderProgram, globalAttribIndex, shaderInputName.c_str());
        attrInfo.Print();
        std::cout << "globalAttribIndex: " << globalAttribIndex << std::endl;
    }
}
void RenderState::SetShader(Shader* shaderStage)
{
    _createProgramIfNotExists();
    shaders.insert(std::make_pair(shaderStage->Stage(), shaderStage->_uniqueIndex()));
    
    if(shaders.size() < 2)
        return;
    
    std::map<Shader::STAGE, unsigned int>::iterator it = shaders.begin();
    for(it; it != shaders.end(); ++it)
    {
        glAttachShader(shaderProgram, it->second);
    }
    glLinkProgram(shaderProgram);
    
    GLint Result = GL_FALSE;
    int InfoLogLength;

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Result);
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &InfoLogLength);
    
    if (InfoLogLength > 0)
    {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(shaderProgram, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        statusString += &ProgramErrorMessage[0];
    }
    
    it = shaders.begin();
    for(it; it != shaders.end(); ++it)
    {
        glDetachShader(shaderProgram, it->second);
    }
}

void RenderState::Bind()
{
    glUseProgram(shaderProgram);
    for(unsigned int i = 0; i < uniforms.size(); ++i)
    {
        uniforms[i].Upload(uniformLocations[i]);
    }
}

void RenderState::_createProgramIfNotExists()
{
    if(shaderProgram > 0)
        return;
    shaderProgram = glCreateProgram();
}

}
}
