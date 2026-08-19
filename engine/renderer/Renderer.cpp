#include "graphics/Texture2D.h"
#include "graphics/Shader.h"
#include "graphics/VertexArray.h"
#include "graphics/IndexBuffer.h"
#include "graphics/Texture2D.h"
#include "renderer/Renderer.h"
#include <glad/glad.h>

void Renderer::DrawQuad(Shader& shader, VertexArray& vertexArray, IndexBuffer& indexBuffer){
    shader.Bind();
    vertexArray.Bind();
    indexBuffer.Bind();

    glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawTexturedQuad(Shader& shader, VertexArray& vertexArray, IndexBuffer& indexBuffer, Texture2D& texture,
                                const Matrix4& model, const Matrix4& view, const Matrix4& projection){
    shader.Bind();
    texture.Bind();

    shader.SetUniformMat4("u_Model", model);
    shader.SetUniformMat4("u_View", view);
    shader.SetUniformMat4("u_Projection", projection);

    vertexArray.Bind();
    indexBuffer.Bind();

    glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}
