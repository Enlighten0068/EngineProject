#pragma once

#include <cstdint>

/**
 * @brief OpenGL Vertex Array Object (VAO) wrapper.
 *
 * A VAO stores the vertex attribute configuration for a vertex buffer.
 * It encapsulates the state of vertex attributes (position, texture coordinates, etc.).
 *
 * @note The VAO must be bound before configuring vertex attributes.
 * @see VertexBuffer, IndexBuffer
 */
class VertexArray{
public:
    VertexArray();
    ~VertexArray();

    /**
     * @brief Binds the VAO.
     */
    void Bind() const;

    /**
     * @brief Unbinds the VAO.
     */
    void Unbind() const;

private:
    uint32_t m_RendererID; //OpenGL VAO ID
};
