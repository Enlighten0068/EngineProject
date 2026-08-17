#include "graphics/Shader.h"
#include "diagnostics/Log.h"
#include <glad/glad.h>
#include <format>

Shader::Shader() : m_RendererID(0){
}

Shader::~Shader(){
    if (m_RendererID) glDeleteProgram(m_RendererID);
}

bool Shader::Compile(
    const std::string& vertexSource,
    const std::string& fragmentSource)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    const char* vertexSrc = vertexSource.c_str();

    glShaderSource(vertexShader,1,&vertexSrc,nullptr);

    glCompileShader(vertexShader);

    GLint success;

    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);

    if (!success){
        char infoLog[1024];
        glGetShaderInfoLog(vertexShader,sizeof(infoLog),nullptr,infoLog);

        Log::Error(std::format("Vertex shader compilation failed:\n{}",infoLog));

        glDeleteShader(vertexShader);

        return false;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* fragmentSrc = fragmentSource.c_str();

    glShaderSource(fragmentShader,1,&fragmentSrc,nullptr);

    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);

    if (!success){
        char infoLog[1024];
        glGetShaderInfoLog(fragmentShader,sizeof(infoLog),nullptr,infoLog);

        Log::Error(std::format("Fragment shader compilation failed:\n{}",infoLog));

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return false;
    }

    m_RendererID = glCreateProgram();

    glAttachShader(m_RendererID,vertexShader);

    glAttachShader(m_RendererID,fragmentShader);

    glLinkProgram(m_RendererID);

    glGetProgramiv(m_RendererID,GL_LINK_STATUS,&success);

    if (!success){
        char infoLog[512];
        glGetProgramInfoLog(m_RendererID,sizeof(infoLog),nullptr,infoLog);

        Log::Error(std::format("Shader program linking failed:\n{}",infoLog));

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    Log::Info("Shader compiled successfully.");

    return true;
}

void Shader::Bind() const{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const{
    glUseProgram(0);
}
