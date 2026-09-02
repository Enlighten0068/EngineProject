#pragma once

#include "math/Matrix4.h"

class Shader;
class VertexArray;
class IndexBuffer;
class Texture2D;

class Renderer{
public:
    static void DrawQuad(Shader& shader, VertexArray& vertexArray, IndexBuffer& indexBuffer);
    static void DrawTexturedQuad(Shader& shader, VertexArray& vertexArray, IndexBuffer& indexBuffer, Texture2D& texture,
                                 const Matrix4& model, const Matrix4& view, const Matrix4& projection, float tileScale = 1.0f);
};
