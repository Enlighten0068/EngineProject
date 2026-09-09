#include "diagnostics/Log.h"
#include "graphics/Texture2D.h"
#include "graphics/Shader.h"
#include "graphics/VertexArray.h"
#include "graphics/IndexBuffer.h"
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
 * @param tileScaleX Number of texture repetitions along the X axis.
 * @param tileScaleY Number of texture repetitions along the Y axis.
 */
void Renderer::DrawTexturedQuad(Shader& shader, VertexArray& vertexArray, IndexBuffer& indexBuffer, Texture2D& texture,
                                const Matrix4& model, const Matrix4& view, const Matrix4& projection,
                                float tileScaleX, float tileScaleY){
    //Bind shader and texture
    shader.Bind();
    texture.Bind();

    //Set transformation uniforms
    shader.SetUniformMat4("u_Model", model);
    shader.SetUniformMat4("u_View", view);
    shader.SetUniformMat4("u_Projection", projection);
    shader.SetUniformVec2("u_TileScale", tileScaleX, tileScaleY);

    //Bind geometry and draw
    vertexArray.Bind();
    indexBuffer.Bind();
    glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
                                }
