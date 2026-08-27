#pragma once

#include "graphics/Shader.h"
#include "graphics/VertexArray.h"
#include "graphics/VertexBuffer.h"
#include "graphics/IndexBuffer.h"
#include <memory>

class GraphicsContext{
public:
    GraphicsContext();
    ~GraphicsContext();

    bool Initialize();
    void Shutdown();

    Shader& GetShader(){ return *m_Shader; }
    VertexArray& GetVertexArray(){ return *m_VertexArray; }
    VertexBuffer& GetVertexBuffer(){ return *m_VertexBuffer; }
    IndexBuffer& GetIndexBuffer(){ return *m_IndexBuffer; }

private:
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;

    bool CreateBuffers();
    bool CompileShaders();
};
