#pragma once

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

private:
    unsigned int m_RendererID;
};
