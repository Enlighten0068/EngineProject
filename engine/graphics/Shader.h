#pragma once

#include "math/Matrix4.h"

#include <string>
#include <cstdint>

class Shader{
public:
    Shader();
    ~Shader();

    bool Compile(
        const std::string& vertexSource,
        const std::string& fragmentSource);

    void Bind() const;
    void Unbind() const;
    void SetUniformMat4(const std::string& name, const Matrix4& matrix) const;
    void SetUniformFloat(const std::string& name, float value) const;

    unsigned int GetRendererID() const { return m_RendererID; }

private:
    unsigned int m_RendererID;
};
