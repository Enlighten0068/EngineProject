#include "diagnostics/Log.h"
#include "graphics/VertexArray.h"
#include <glad/glad.h>
#include <format>

VertexArray::VertexArray() : m_RendererID(0){
    glGenVertexArrays(1, &m_RendererID);

    if(m_RendererID == 0){
        Log::Error("VertexArray creation failed.");
    } else{
        Log::Info(std::format("VertexArray created. ID: {}", m_RendererID));
    }
}

VertexArray::~VertexArray(){
    glDeleteVertexArrays(1, &m_RendererID);
}

/**
 * @brief Binds the VAO.
 */
void VertexArray::Bind() const{
    glBindVertexArray(m_RendererID);
}

/**
 * @brief Unbinds the VAO.
 */
void VertexArray::Unbind() const{
    glBindVertexArray(0);
}
