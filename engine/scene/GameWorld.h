#pragma once

#include "graphics/Shader.h"
#include "graphics/VertexArray.h"
#include "graphics/VertexBuffer.h"
#include "graphics/IndexBuffer.h"
#include "math/Vector3D.h"
#include <memory>

/**
 * @brief Defines the boundaries of the game world and provides clamping.
 *
 * The GameWorld class holds the minimum and maximum coordinates of the world
 * in both X and Y axes. It provides a ClampPosition method to keep entities
 * within these bounds.
 *
 */
class GameWorld{
public:
    GameWorld(float minX, float maxX, float minY, float maxY);
    ~GameWorld() = default;

    //Getters - Min/Max world coordinates
    float GetMinX() const{ return m_MinX; }
    float GetMaxX() const{ return m_MaxX; }
    float GetMinY() const{ return m_MinY; }
    float GetMaxY() const{ return m_MaxY; }

    /**
     * @brief Renders the world borders.
     * @param shader Shader to use for rendering.
     * @param view View matrix.
     * @param projection Projection matrix.
     */
    Vector3D ClampPosition(const Vector3D& position) const;

    void Render(Shader& shader, const Matrix4& view, const Matrix4& projection);

private:
    float m_MinX, m_MaxX, m_MinY, m_MaxY;

    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    bool m_Initialized = false;

    /**
     * @brief Initializes the border geometry.
     */
    void InitializeBorders();
};
