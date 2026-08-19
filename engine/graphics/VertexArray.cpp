#include "diagnostics/Log.h"
#include "graphics/VertexArray.h"
#include <glad/glad.h>
#include <format>

VertexArray::VertexArray() : m_RendererID(0){
    glGenVertexArrays(1, &m_RendererID);

    if (m_RendererID == 0) Log::Error("VertexArray creation failed.");
    else{
        Log::Info(std::format("VertexArray ID: {}", m_RendererID));
        Log::Info("VertexArray created.");
    }
}

VertexArray::~VertexArray(){
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const{
    glBindVertexArray(
        m_RendererID);
}

void VertexArray::Unbind() const{
    glBindVertexArray(0);
}
