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
    
    unsigned int program = glCreateProgram();
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
    
}
}
