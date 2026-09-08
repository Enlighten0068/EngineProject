#pragma once

#include "math/Matrix4.h"
#include <string>
#include <cstdint>

/**
 * @brief OpenGL shader program wrapper.
 *
 * Manages the compilation, linking, and usage of vertex and fragment shaders.
 * Provides methods for setting uniform values (matrices and floats).
 *
 * @note Shaders are compiled from source strings at runtime.
 */
class Shader{
public:
    Shader();
    ~Shader();

    /**
     * @brief Compiles and links the vertex and fragment shaders.
     *
     * @param vertexSource GLSL source code for the vertex shader.
     * @param fragmentSource GLSL source code for the fragment shader.
     * @return true if compilation and linking succeeded, false otherwise.
     */
    bool Compile(
        const std::string& vertexSource,
        const std::string& fragmentSource);

    /**
     * @brief Binds the shader program for use.
     */
    void Bind() const;

    /**
     * @brief Unbinds the shader program.
     */
    void Unbind() const;

    /**
     * @brief Sets a 4x4 matrix uniform.
     * @param name Uniform name in the shader.
     * @param matrix Matrix to set.
     */
    void SetUniformMat4(const std::string& name, const Matrix4& matrix) const;

    /**
     * @brief Sets a float uniform.
     * @param name Uniform name in the shader.
     * @param value Float value to set.
     */
    void SetUniformFloat(const std::string& name, float value) const;

    unsigned int GetRendererID() const{ return m_RendererID; } //Gets OpenGL program ID

private:
    unsigned int m_RendererID;
};
