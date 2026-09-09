#include "graphics/Shader.h"
#include "diagnostics/Log.h"
#include <glad/glad.h>
#include <format>

Shader::Shader() : m_RendererID(0){}

Shader::~Shader(){
    if(m_RendererID) glDeleteProgram(m_RendererID);
}

/**
 * @brief Compiles and links the vertex and fragment shaders.
 *
 * This method creates and compiles the vertex shader and then fragment shader.
 * Then links both shaders into a program and then clears its objects.
 * Also clears it in case of failure.
 *
 * @param vertexSource GLSL source code for the vertex shader.
 * @param fragmentSource GLSL source code for the fragment shader.
 * @return true if compilation and linking succeeded, false otherwise.
 */
bool Shader::Compile(const std::string& vertexSource, const std::string& fragmentSource){
    //Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSrc = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        char infoLog[1024];
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), nullptr, infoLog);
        Log::Error(std::format("Vertex shader compilation failed:\n{}", infoLog));
        glDeleteShader(vertexShader);
        return false;
    }
    Log::Info("Vertex shader compiled successfully.");

    //Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSrc = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        char infoLog[1024];
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);
        Log::Error(std::format("Fragment shader compilation failed:\n{}", infoLog));
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }
    Log::Info("Fragment shader compiled successfully.");

    //Link shader program
    m_RendererID = glCreateProgram();
    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);
    glLinkProgram(m_RendererID);

    glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
    if(!success){
        char infoLog[512];
        glGetProgramInfoLog(m_RendererID, sizeof(infoLog), nullptr, infoLog);
        Log::Error(std::format("Shader program linking failed:\n{}", infoLog));
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(m_RendererID);
        return false;
    }

    //Cleanup shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    Log::Info("Shader compiled and linked successfully.");
    return true;
}

/**
 * @brief Binds the shader program.
 */
void Shader::Bind() const{
    glUseProgram(m_RendererID);
}

/**
 * @brief Unbinds the shader program.
 */
void Shader::Unbind() const{
    glUseProgram(0);
}

/**
 * @brief Sets a 4x4 matrix uniform.
 * @param name Uniform name in the shader.
 * @param matrix Matrix to set.
 */
void Shader::SetUniformMat4(const std::string& name, const Matrix4& matrix) const{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    if(location != -1){
        glUniformMatrix4fv(location, 1, GL_FALSE, matrix.Data());
    } else{
        Log::Warning(std::format("Uniform '{}' not found in shader.", name));
    }
}

/**
 * @brief Sets a float uniform.
 * @param name Uniform name in the shader.
 * @param value Float value to set.
 */
void Shader::SetUniformFloat(const std::string& name, float value) const{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    if(location != -1) glUniform1f(location, value);
}

/**
 * @brief Sets a 3D vector uniform (three floats).
 * @param name Uniform name in the shader.
 * @param x X component.
 * @param y Y component.
 * @param z Z component.
 */
void Shader::SetUniformFloat3(const std::string& name, float x, float y, float z) const{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    if(location != -1) glUniform3f(location, x, y, z);
}

/**
 * @brief Sets a 3D vector uniform from a Vector3D.
 * @param name Uniform name in the shader.
 * @param value Vector3D value to set.
 */
void Shader::SetUniformFloat3(const std::string& name, const Vector3D& value) const{
    SetUniformFloat3(name, value.x, value.y, value.z);
}

/**
 * @brief Sets a 2D vector uniform (two floats).
 * @param name Uniform name in the shader.
 * @param x X component.
 * @param y Y component.
 */
void Shader::SetUniformVec2(const std::string& name, float x, float y) const{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    if(location != -1) glUniform2f(location, x, y);
}

/**
 * @brief Sets a 2D vector uniform from a Vector2D.
 * @param name Uniform name in the shader.
 * @param value Vector2D value to set.
 */
void Shader::SetUniformVec2(const std::string& name, const Vector2D& value) const{
    SetUniformVec2(name, value.x, value.y);
}
