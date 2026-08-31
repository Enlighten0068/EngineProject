#include "diagnostics/Log.h"
#include "renderer/Renderer.h"
#include "scene/GameWorld.h"
#include <glad/glad.h>

GameWorld::GameWorld(float minX, float maxX, float minY, float maxY)
: m_MinX(minX), m_MaxX(maxX), m_MinY(minY), m_MaxY(maxY){ InitializeBorders(); }

void GameWorld::InitializeBorders(){
    float vertices[] ={
        m_MinX, m_MinY, 0.0f,
        m_MaxX, m_MinY, 0.0f,
        m_MaxX, m_MaxY, 0.0f,
        m_MinX, m_MaxY, 0.0f
    };

    uint32_t indices[] = {
        0, 1,
        1, 2,
        2, 3,
        3, 0
    };

    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
    m_VertexArray = std::make_unique<VertexArray>();
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 8);

    m_VertexArray->Bind();
    m_VertexBuffer->Bind();
    m_IndexBuffer->Bind();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    m_Initialized = true;
    Log::Info("GameWorld initialized with borders.");
}

Vector3D GameWorld::ClampPosition(const Vector3D& position) const{
    Vector3D clamped = position;
    clamped.x = std::max(m_MinX, std::min(m_MaxX, clamped.x));
    clamped.y = std::max(m_MinY, std::min(m_MaxY, clamped.y));
    return clamped;
}

void GameWorld::Render(Shader& shader, const Matrix4& view, const Matrix4& projection){
    if (!m_Initialized) return;

    Matrix4 model = Matrix4::Identity();

    shader.Bind();
    shader.SetUniformMat4("u_Model", model);
    shader.SetUniformMat4("u_View", view);
    shader.SetUniformMat4("u_Projection", projection);
}
