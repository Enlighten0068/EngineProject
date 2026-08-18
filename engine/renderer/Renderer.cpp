#include "graphics/Texture2D.h"
#include "graphics/Shader.h"
#include "graphics/VertexArray.h"
#include "graphics/IndexBuffer.h"
#include "renderer/Renderer.h"
#include <glad/glad.h>

void Renderer::DrawQuad(Shader& shader,VertexArray& vertexArray,IndexBuffer& indexBuffer){
    shader.Bind();
    vertexArray.Bind();
    indexBuffer.Bind();

    glDrawElements(GL_TRIANGLES,indexBuffer.GetCount(),GL_UNSIGNED_INT,nullptr);
}

void Renderer::DrawTexturedQuad(Shader& shader,VertexArray& vertexArray,IndexBuffer& indexBuffer,Texture2D& texture){
    shader.Bind();
    texture.Bind();
    vertexArray.Bind();
    indexBuffer.Bind();

    glDrawElements(GL_TRIANGLES,indexBuffer.GetCount(),GL_UNSIGNED_INT,nullptr);
}
