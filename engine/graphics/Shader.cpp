#include "graphics/Shader.h"
#include "diagnostics/Log.h"
#include <glad/glad.h>
#include <format>

Shader::Shader() : m_RendererID(0){
}

Shader::~Shader(){
    if (m_RendererID) glDeleteProgram(m_RendererID);
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
    //Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSrc = vertexSource.c_str();
    glShaderSource(vertexShader,1,&vertexSrc, nullptr);
    glCompileShader(vertexShader);
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success){
        char infoLog[1024];
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), nullptr, infoLog);

        Log::Error(std::format("Vertex shader compilation failed:\n{}", infoLog));

        glDeleteShader(vertexShader);

        return false;
    }
    Log::Info("Shader program linked successfully.");

    //Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSrc = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success){
        char infoLog[1024];
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);

        Log::Error(std::format("Fragment shader compilation failed:\n{}", infoLog));

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return false;
    }

    //Linking
    m_RendererID = glCreateProgram();
    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);
    glLinkProgram(m_RendererID);
    Log::Info(std::format("Program ID: {}, Link status: {}", m_RendererID, success));
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);

    if (!success){
        char infoLog[512];
        glGetProgramInfoLog(m_RendererID, sizeof(infoLog), nullptr, infoLog);

        Log::Error(std::format("Shader program linking failed:\n{}", infoLog));

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return false;
    }

    //Cleanup in case of sucess, don't worry it also cleans up in case of shader compilation or linking failure
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    Log::Info("Shader compiled successfully.");

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
 *
 * If the uniform is not found in the shader, a warning is logged.
 *
 * @param name Uniform name in the shader.
 * @param matrix Matrix to set.
 */
void Shader::SetUniformMat4(const std::string& name, const Matrix4& matrix) const{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location != -1) glUniformMatrix4fv(location, 1, GL_FALSE, matrix.Data());
    else Log::Warning(std::format("Uniform '{}' not found in shader.", name));
}

/**
 * @brief Sets a float uniform.
 * @param name Uniform name in the shader.
 * @param value Float value to set.
 */
void Shader::SetUniformFloat(const std::string& name, float value) const{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location != -1) glUniform1f(location, value);

}
