#pragma once

#include "math/Matrix4.h"
#include "math/Vector2D.h"
#include "math/Vector3D.h"
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

    /**
     * @brief Sets a 3D vector uniform (three floats).
     * @param name Uniform name in the shader.
     * @param x X component of the vector.
     * @param y Y component of the vector.
     * @param z Z component of the vector.
     */
    void SetUniformFloat3(const std::string& name, float x, float y, float z) const;

    /**
     * @brief Sets a 3D vector uniform from a Vector3D.
     * @param name Uniform name in the shader.
     * @param value Vector3D value to set.
     */
    void SetUniformFloat3(const std::string& name, const Vector3D& value) const;

    /**
     * @brief Sets a 2D vector uniform (two floats).
     * @param name Uniform name in the shader.
     * @param x X component of the vector.
     * @param y Y component of the vector.
     */
    void SetUniformVec2(const std::string& name, float x, float y) const;

    /**
     * @brief Sets a 2D vector uniform from a Vector2D.
     * @param name Uniform name in the shader.
     * @param value Vector2D value to set.
     */
    void SetUniformVec2(const std::string& name, const Vector2D& value) const;

    unsigned int GetRendererID() const{ return m_RendererID; } //Gets OpenGL program ID

private:
    unsigned int m_RendererID; //OpenGL program ID
};
