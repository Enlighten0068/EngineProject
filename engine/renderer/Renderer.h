#pragma once

class Shader;
class VertexArray;
class IndexBuffer;

class Renderer{
public:
    static void DrawQuad(Shader& shader,VertexArray& vertexArray,IndexBuffer& indexBuffer);
};
