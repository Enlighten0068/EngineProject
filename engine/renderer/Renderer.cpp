#include "renderer/Renderer.h"
#include "graphics/Shader.h"
#include "graphics/VertexArray.h"
#include "graphics/IndexBuffer.h"
#include <glad/glad.h>

void Renderer::DrawQuad(Shader& shader,VertexArray& vertexArray,IndexBuffer& indexBuffer){
    shader.Bind();
    vertexArray.Bind();
    indexBuffer.Bind();

    glDrawElements(GL_TRIANGLES,indexBuffer.GetCount(),GL_UNSIGNED_INT,nullptr);
}
