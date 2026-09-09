#pragma once

#include <cstdint>

/**
 * @brief OpenGL Vertex Buffer Object (VBO) wrapper.
 *
 * A VBO stores vertex data (positions, texture coordinates, normals, etc.)
 * in GPU memory.
 *
 * @note The buffer is created with GL_STATIC_DRAW usage.
 * @see VertexArray
 */
class VertexBuffer{
public:
    /**
     * @brief Constructs a vertex buffer and uploads data to the GPU.
     * @param vertices Pointer to vertex data.
     * @param size Size of the vertex data in bytes.
     */
    VertexBuffer(const void* vertices, uint32_t size);

    ~VertexBuffer();

    /**
     * @brief Binds the buffer as the current GL_ARRAY_BUFFER.
     */
    void Bind() const;

    /**
     * @brief Unbinds the buffer.
     */
    void Unbind() const;

private:
    uint32_t m_RendererID; //OpenGL VBO ID
};
