#pragma once

#include "graphics/Shader.h"
#include "graphics/VertexArray.h"
#include "graphics/VertexBuffer.h"
#include "graphics/IndexBuffer.h"
#include <memory>

/**
 * @brief Centralized manager for OpenGL graphics resources.
 *
 * The GraphicsContext is responsible for creating and managing all OpenGL
 * resources used by the engine: shaders, vertex array objects,
 * vertex buffers, and index buffers. It encapsulates the
 * OpenGL initialization and provides access to these resources.
 *
 * @note This class should be initialized after the OpenGL context is created.
 * @see Shader, VertexArray, VertexBuffer, IndexBuffer
 */
class GraphicsContext{
public:
    GraphicsContext();
    ~GraphicsContext();

    /**
     * @brief Initializes all graphics resources.
     *
     * Creates buffers, compiles shaders, and sets up vertex attribute layouts.
     *
     * @return true if initialization succeeded, false otherwise.
     */
    bool Initialize();

    /**
     * @brief Shuts down and releases all graphics resources.
     */
    void Shutdown();

    //Getters
    Shader& GetShader(){ return *m_Shader; }
    VertexArray& GetVertexArray(){ return *m_VertexArray; }
    VertexBuffer& GetVertexBuffer(){ return *m_VertexBuffer; }
    IndexBuffer& GetIndexBuffer(){ return *m_IndexBuffer; }

private:
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;

    /**
     * @brief Creates the vertex and index buffers for a quad.
     *
     * Quads are a unit square centered at the origin with texture coordinates.
     *
     * @return true if buffers were created successfully, false otherwise.
     */
    bool CreateBuffers();

    /**
     * @brief Compiles the vertex and fragment shaders.
     *
     * @return true if shaders compiled and linked successfully, false otherwise.
     */
    bool CompileShaders();
};
