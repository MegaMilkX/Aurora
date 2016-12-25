#include "shader.h"

namespace Au{
namespace GFX{
 
void Shader::AddStage(STAGE stage, const std::string& source)
{
    stages[stage] = source;
}
    
bool Shader::Compile()
{
    std::vector<unsigned int> compiled_stages;
    std::map<STAGE, std::string>::iterator it = stages.begin();
    for(it; it != stages.end(); ++it)
    {
        unsigned int type;
        if(it->first == STAGE::VERTEX)
            type = GL_VERTEX_SHADER;
        else if(it->first == STAGE::PIXEL)
            type = GL_FRAGMENT_SHADER;
        else
            continue;
        
        unsigned int shader = glCreateShader(type);
        
        const char* source = it->second.c_str();
        
        glShaderSource(shader, 1, &source, 0);
        glCompileShader(shader);
        
        GLint Result = GL_FALSE;
        int InfoLogLength;
        
        glGetShaderiv(shader, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
        
        if (InfoLogLength > 1)
        {
            std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(shader, InfoLogLength, NULL, &ShaderErrorMessage[0]);
            status_string += &ShaderErrorMessage[0];
        }
        
        compiled_stages.push_back(shader);
    }
    
    program = glCreateProgram();
    for(unsigned int i = 0; i < compiled_stages.size(); ++i)
        glAttachShader(program, compiled_stages[i]);
    
    glLinkProgram(program);
    
    GLint Result = GL_FALSE;
    int InfoLogLength;

    glGetProgramiv(program, GL_LINK_STATUS, &Result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
    
    if (InfoLogLength > 0)
    {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        status_string += &ProgramErrorMessage[0];
    }
    
    return true;
}

void Shader::AttribFormat(const std::vector<AttribInfo>& vertexFormat)
{
    this->vertexFormat = vertexFormat;
    std::map<typeindex, AttribInstance> instanceCounters;
    for(unsigned int i = 0; i < vertexFormat.size(); ++i)
    {
        const AttribInfo& attrInfo = vertexFormat[i];
        AttribIndex globalAttribIndex = 
            GetGlobalAttribIndex(attrInfo.typeIndex,
                                 instanceCounters[attrInfo.typeIndex]++);
        
        std::string shaderInputName = attrInfo.name;
        if(globalAttribIndex > 0)
            shaderInputName += std::to_string(globalAttribIndex);
                                 
        glBindAttribLocation(program, globalAttribIndex, shaderInputName.c_str());
    }
}

void Shader::Bind()
{
    glUseProgram(program);
}

void Shader::SetUniform(const std::string& name, float value) 
{ glUniform1f(glGetUniformLocation(program, name.c_str()), value); }
void Shader::SetUniform(const std::string& name, Math::Vec2f& value) 
{ glUniform2f(glGetUniformLocation(program, name.c_str()), value.x, value.y); }
void Shader::SetUniform(const std::string& name, Math::Vec3f& value) 
{ glUniform3f(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z); }
void Shader::SetUniform(const std::string& name, Math::Vec4f& value) 
{ glUniform4f(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z, value.w); }
void Shader::SetUniform(const std::string& name, int value) 
{ glUniform1i(glGetUniformLocation(program, name.c_str()), value); }
void Shader::SetUniform(const std::string& name, Math::Vec2i& value) 
{ glUniform2i(glGetUniformLocation(program, name.c_str()), value.x, value.y); }
void Shader::SetUniform(const std::string& name, Math::Vec3i& value) 
{ glUniform3i(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z); }
void Shader::SetUniform(const std::string& name, Math::Vec4i& value) 
{ glUniform4i(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z, value.w); }
void Shader::SetUniform(const std::string& name, unsigned int value) 
{ glUniform1ui(glGetUniformLocation(program, name.c_str()), value); }
void Shader::SetUniform(const std::string& name, Math::Vec2ui& value) 
{ glUniform2ui(glGetUniformLocation(program, name.c_str()), value.x, value.y); }
void Shader::SetUniform(const std::string& name, Math::Vec3ui& value) 
{ glUniform3ui(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z); }
void Shader::SetUniform(const std::string& name, Math::Vec4ui& value) 
{ glUniform4ui(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z, value.w); }
void Shader::SetUniform(const std::string& name, Math::Mat3f& value) 
{ glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, (float*)&value); }
void Shader::SetUniform(const std::string& name, Math::Mat4f& value) 
{ glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, (float*)&value); }
    
}
}
