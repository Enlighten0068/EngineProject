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
 * within these bounds and renders the world borders.
 */
class GameWorld{
public:
    /**
     * @brief Constructs a GameWorld with specified boundaries.
     * @param minX Minimum X coordinate.
     * @param maxX Maximum X coordinate.
     * @param minY Minimum Y coordinate.
     * @param maxY Maximum Y coordinate.
     */
    GameWorld(float minX, float maxX, float minY, float maxY);
    ~GameWorld() = default;

    //Getters
    float GetMinX() const{ return m_MinX; }
    float GetMaxX() const{ return m_MaxX; }
    float GetMinY() const{ return m_MinY; }
    float GetMaxY() const{ return m_MaxY; }

    /**
     * @brief Clamps a position to stay within the world boundaries.
     * @param position The position to clamp.
     * @return Clamped position.
     */
    Vector3D ClampPosition(const Vector3D& position) const;

    /**
     * @brief Renders the world borders as red lines.
     * @param shader Shader to use for rendering.
     * @param view View matrix.
     * @param projection Projection matrix.
     */
    void Render(Shader& shader, const Matrix4& view, const Matrix4& projection);

private:
    float m_MinX, m_MaxX; //X boundaries
    float m_MinY, m_MaxY; //Y boundaries

    std::unique_ptr<VertexArray> m_VertexArray; //VAO for border lines
    std::unique_ptr<VertexBuffer> m_VertexBuffer; //VBO for border vertices
    std::unique_ptr<IndexBuffer> m_IndexBuffer; //IBO for border indices
    bool m_Initialized = false; //Flag indicating if borders are initialized

    /**
     * @brief Initializes the border geometry.
     */
    void InitializeBorders();
};
