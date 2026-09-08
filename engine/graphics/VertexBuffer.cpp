#include "graphics/VertexBuffer.h"
#include "diagnostics/Log.h"
#include <glad/glad.h>
#include <format>

/**
 * @brief Creates a vertex buffer and uploads vertex data to the GPU.
 *
 * @param vertices Pointer to vertex data.
 * @param size Size of the vertex data in bytes.
 */
VertexBuffer::VertexBuffer(const void* vertices, uint32_t size): m_RendererID(0){
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size,vertices, GL_STATIC_DRAW);

    if(m_RendererID == 0) Log::Error("VertexBuffer creation failed.");
    else{
        Log::Info(std::format("Renderer ID: {}", m_RendererID));
        Log::Info("VertexBuffer created.");
    }
}

VertexBuffer::~VertexBuffer(){
    glDeleteBuffers(1, &m_RendererID);
}

/**
 * @brief Binds the buffer as the active GL_ARRAY_BUFFER.
 */
void VertexBuffer::Bind() const{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

/**
 * @brief Unbinds the buffer.
 */
void VertexBuffer::Unbind() const{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
