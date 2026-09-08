#pragma once

#include <cstdint>

/**
 * @brief OpenGL Index Buffer Object (EBO/IBO) wrapper.
 *
 * An index buffer stores vertex indices for indexed drawing (glDrawElements).
 * This allows reusing vertices and reduces memory usage.
 *
 * @note The buffer is created with GL_STATIC_DRAW usage.
 */
class IndexBuffer{
public:
    /**
     * @brief Constructs an index buffer and uploads indices to the GPU.
     * @param indices Pointer to index data (uint32_t values).
     * @param count Number of indices.
     */
    IndexBuffer(const uint32_t* indices, uint32_t count);

    ~IndexBuffer();

    /**
     * @brief Binds the buffer as the current GL_ELEMENT_ARRAY_BUFFER.
     */
    void Bind() const;

    /**
     * @brief Unbinds the buffer.
     */
    void Unbind() const;

    uint32_t GetCount() const; //Count number of indices stored

private:
    uint32_t m_RendererID;
    uint32_t m_Count;
};
