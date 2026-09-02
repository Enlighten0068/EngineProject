#include "diagnostics/Log.h"
#include "graphics/Texture2D.h"
#include "graphics/Shader.h"
#include "graphics/VertexArray.h"
#include "graphics/IndexBuffer.h"
#include "graphics/Texture2D.h"
#include "renderer/Renderer.h"
#include <glad/glad.h>
#include <format>

void Renderer::DrawQuad(Shader& shader, VertexArray& vertexArray, IndexBuffer& indexBuffer){
    shader.Bind();
    vertexArray.Bind();
    indexBuffer.Bind();

    glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawTexturedQuad(Shader& shader, VertexArray& vertexArray, IndexBuffer& indexBuffer, Texture2D& texture,
                                const Matrix4& model, const Matrix4& view, const Matrix4& projection, float tileScale){
    shader.Bind();
    texture.Bind();

    shader.SetUniformMat4("u_Model", model);
    shader.SetUniformMat4("u_View", view);
    shader.SetUniformMat4("u_Projection", projection);
    shader.SetUniformFloat("u_TileScale", tileScale);

    GLint modelLoc = glGetUniformLocation(shader.GetRendererID(), "u_Model");
    if (modelLoc != -1) {
        float gpuModel[16];
        glGetUniformfv(shader.GetRendererID(), modelLoc, gpuModel);
        //Log::Info(std::format("[Renderer] GPU Model[0]={}, Model[5]={}", gpuModel[0], gpuModel[5]));
    } //else Log::Error("[Renderer] Uniform 'u_Model' not found!");

    GLint viewLoc  = glGetUniformLocation(shader.GetRendererID(), "u_View");
    GLint projLoc  = glGetUniformLocation(shader.GetRendererID(), "u_Projection");

    vertexArray.Bind();
    indexBuffer.Bind();
    glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}
