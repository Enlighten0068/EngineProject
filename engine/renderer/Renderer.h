#pragma once

#include "math/Matrix4.h"

class Shader;
class VertexArray;
class IndexBuffer;
class Texture2D;

/**
 * @brief Static rendering API for drawing 2D geometry.
 *
 * The Renderer provides a simple interface for drawing textured or untextured
 * quads using the engine's shader and buffer objects. All methods are static,
 * making it easy to use from anywhere in the engine.
 *
 * @note The Renderer assumes that the shader already has the correct uniforms
 *       for the model, view, and projection matrices, as well as a texture sampler.
 * @see Shader, VertexArray, IndexBuffer, Texture2D
 */
class Renderer{
public:

    /**
     * @brief Draws a flat (untextured) quad.
     *
     * Binds the shader, vertex array, and index buffer, then issues a draw call.
     *
     * @param shader The shader program to use.
     * @param vertexArray The vertex array object containing vertex data.
     * @param indexBuffer The index buffer containing the quad indices.
     */
    static void DrawQuad(Shader& shader, VertexArray& vertexArray, IndexBuffer& indexBuffer);

    /**
     * @brief Draws a textured quad with transformation matrices.
     *
     * Binds the shader, texture, vertex array, and index buffer, then sets
     * the model, view, projection, and tile scale uniforms before drawing.
     *
     * @param shader The shader program to use.
     * @param vertexArray The vertex array object containing vertex data.
     * @param indexBuffer The index buffer containing the quad indices.
     * @param texture The texture to apply to the quad.
     * @param model The model matrix (position, rotation, scale).
     * @param view The view matrix (camera position).
     * @param projection The projection matrix (orthographic or perspective).
     * @param tileScale The texture tile scale (number of repetitions per unit).
     */
    static void DrawTexturedQuad(Shader& shader, VertexArray& vertexArray, IndexBuffer& indexBuffer, Texture2D& texture,
                                 const Matrix4& model, const Matrix4& view, const Matrix4& projection,
                                 float tileScaleX = 1.0f, float tileScaleY = 1.0f);
};
