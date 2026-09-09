#include "diagnostics/Log.h"
#include "graphics/IndexBuffer.h"
#include <glad/glad.h>
#include <format>

/**
 * @brief Creates an index buffer and uploads indices to the GPU.
 * @param indices Pointer to index data.
 * @param count Number of indices.
 */
IndexBuffer::IndexBuffer(const uint32_t* indices, uint32_t count) : m_RendererID(0), m_Count(count){
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);

    if(m_RendererID == 0){
        Log::Error("IndexBuffer creation failed.");
    } else{
        Log::Info(std::format("IndexBuffer created. ID: {}", m_RendererID));
    }
}

IndexBuffer::~IndexBuffer(){
    glDeleteBuffers(1, &m_RendererID);
}

/**
 * @brief Binds the buffer as the active GL_ELEMENT_ARRAY_BUFFER.
 */
void IndexBuffer::Bind() const{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

/**
 * @brief Unbinds the buffer.
 */
void IndexBuffer::Unbind() const{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/**
 * @brief Returns the number of indices.
 * @return Number of indices.
 */
uint32_t IndexBuffer::GetCount() const{
    return m_Count;
}
