#include "diagnostics/Log.h"
#include "graphics/Texture2D.h"
#include "graphics/Shader.h"
#include "graphics/VertexArray.h"
#include "graphics/IndexBuffer.h"
#include "graphics/Texture2D.h"
#include "renderer/Renderer.h"
#include <glad/glad.h>
#include <format>

/**
 * @brief Draws a flat (untextured) quad.
 *
 * This method is used for simple geometry that does not require texturing.
 * It binds the shader, VAO, and IBO, then issues a single draw call.
 *
 * @param shader The shader program to use.
 * @param vertexArray The vertex array object.
 * @param indexBuffer The index buffer containing the quad indices.
 */
void Renderer::DrawQuad(Shader& shader, VertexArray& vertexArray, IndexBuffer& indexBuffer){
    shader.Bind();
    vertexArray.Bind();
    indexBuffer.Bind();

    glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}

/**
 * @brief Draws a textured quad with transformation matrices.
 *
 * This is the primary rendering method for 2D sprites. It applies the model,
 * view, and projection matrices to position the quad in world space, and uses
 * a tile scale to repeat the texture across the quad surface.
 *
 * @param shader The shader program to use.
 * @param vertexArray The vertex array object.
 * @param indexBuffer The index buffer containing the quad indices.
 * @param texture The texture to apply to the quad.
 * @param model The model matrix (position, rotation, scale).
 * @param view The view matrix (camera position).
 * @param projection The projection matrix (orthographic or perspective).
 * @param tileScale The texture tile scale (number of repetitions per unit).
 */
void Renderer::DrawTexturedQuad(Shader& shader, VertexArray& vertexArray, IndexBuffer& indexBuffer, Texture2D& texture,
                                const Matrix4& model, const Matrix4& view, const Matrix4& projection, float tileScale){
    shader.Bind();
    texture.Bind();

    shader.SetUniformMat4("u_Model", model);
    shader.SetUniformMat4("u_View", view);
    shader.SetUniformMat4("u_Projection", projection);
    shader.SetUniformFloat("u_TileScale", tileScale);

    //For testing purposes
    /*GLint projLoc = glGetUniformLocation(shader.GetRendererID(), "u_Projection");
    if (projLoc != -1){
        float projMat[16];
        glGetUniformfv(shader.GetRendererID(), projLoc, projMat);
        Log::Info(std::format("Projection[0]={}, Projection[5]={}, Projection[10]={}, Projection[15]={}",
                              projMat[0], projMat[5], projMat[10], projMat[15]));
    } else Log::Error("u_Projection not found!");

    GLint viewLoc = glGetUniformLocation(shader.GetRendererID(), "u_View");
    if (viewLoc != -1){
        float viewMat[16];
        glGetUniformfv(shader.GetRendererID(), viewLoc, viewMat);
        Log::Info(std::format("View[12]={}, View[13]={}", viewMat[12], viewMat[13]));
    } */

    vertexArray.Bind();
    indexBuffer.Bind();
    glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}
