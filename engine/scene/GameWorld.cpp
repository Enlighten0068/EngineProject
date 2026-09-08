#include "diagnostics/Log.h"
#include "renderer/Renderer.h"
#include "scene/GameWorld.h"
#include <glad/glad.h>

GameWorld::GameWorld(float minX, float maxX, float minY, float maxY)
: m_MinX(minX), m_MaxX(maxX), m_MinY(minY), m_MaxY(maxY){ InitializeBorders(); }

/**
 * @brief Initializes the border geometry.
 *
 */
void GameWorld::InitializeBorders(){
    //World corners
    float vertices[] ={
        m_MinX, m_MinY, 0.0f, //Bottom-left
        m_MaxX, m_MinY, 0.0f, //Bottom-right
        m_MaxX, m_MaxY, 0.0f, //Top-right
        m_MinX, m_MaxY, 0.0f //Top-left
    };

    //Line loop indices
    uint32_t indices[] = {
        0, 1, //Bottom-left to bottom-right (bottom edge)
        1, 2, //Bottom-right to top-right (right edge)
        2, 3, //Top-right to top-left ("ceiling")
        3, 0 //Top-left to bottom-left (left edge)
    };

    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
    m_VertexArray = std::make_unique<VertexArray>();
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 8);

    m_VertexArray->Bind();
    m_VertexBuffer->Bind();
    m_IndexBuffer->Bind();

    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    m_Initialized = true;
    Log::Info("GameWorld initialized with borders.");
}

/**
 * @brief Clamps a position to stay within the world boundaries.
 *
 * @param position The position to clamp.
 * @return Clamped position.
 */
Vector3D GameWorld::ClampPosition(const Vector3D& position) const{
    Vector3D clamped = position;
    clamped.x = std::max(m_MinX, std::min(m_MaxX, clamped.x));
    clamped.y = std::max(m_MinY, std::min(m_MaxY, clamped.y));
    return clamped;
}

/**
 * @brief Renders the world borders.
 *
 * @param shader Shader to use for rendering.
 * @param view View matrix.
 * @param projection Projection matrix.
 */
void GameWorld::Render(Shader& shader, const Matrix4& view, const Matrix4& projection){
    if (!m_Initialized) return;

    Matrix4 model = Matrix4::Identity();

    shader.Bind();
    shader.SetUniformMat4("u_Model", model);
    shader.SetUniformMat4("u_View", view);
    shader.SetUniformMat4("u_Projection", projection);
    shader.SetUniformFloat3("u_Color", Vector3D(1.0f, 0.0f, 0.0f));

    m_VertexArray->Bind();
    m_IndexBuffer->Bind();
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, nullptr);
}
