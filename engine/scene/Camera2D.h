#pragma once

#include "math/Matrix4.h"
#include "math/Vector3D.h"

/**
 * @brief 2D camera with orthographic projection, position, rotation, and zoom.
 *
 * The Camera2D class manages an orthographic projection matrix and a view matrix
 * based on the camera's position, rotation (around Z-axis), and zoom level.
 * It is used to define what part of the world is visible on screen.
 *
 * @note The camera uses a dirty flag to avoid recalculating the view matrix
 *       every frame unless the position or rotation has changed.
 */
class Camera2D{
public:
    /**
     * @brief Constructs a 2D camera with specified view volume.
     * @param left Left boundary of the view volume.
     * @param right Right boundary of the view volume.
     * @param bottom Bottom boundary of the view volume.
     * @param top Top boundary of the view volume.
     * @param nearPlane Near clipping plane (default: -1.0).
     * @param farPlane Far clipping plane (default: 1.0).
     */
    Camera2D(float left, float right, float bottom, float top, float nearPlane = -1.0f, float farPlane = 1.0f);

    /**
     * @brief Sets the camera position in world space.
     * @param position New position.
     */
    void SetPosition(const Vector3D& position);

    /**
     * @brief Sets the camera rotation around the Z-axis.
     * @param angleRadians Rotation angle in radians.
     */
    void SetRotation(float angleRadians);

    /**
     * @brief Sets the camera projection volume.
     * @param left Left boundary.
     * @param right Right boundary.
     * @param bottom Bottom boundary.
     * @param top Top boundary.
     */
    void SetProjection(float left, float right, float bottom, float top);

    /**
     * @brief Sets the camera zoom level.
     * @param zoom Zoom factor (clamped to a minimum of 0.1).
     */
    void SetZoom(float zoom);

    //Getters
    Vector3D GetPosition() const{ return m_Position; }
    float GetZoom() const{ return m_Zoom; }
    const Matrix4& GetViewMatrix() const;
    const Matrix4& GetProjectionMatrix() const;

    /**
     * @brief Updates the view matrix if the camera is dirty.
     * Should be called before rendering if the camera has moved or rotated.
     */
    void Update();

private:
    /**
     * @brief Recalculates the projection matrix based on zoom level.
     */
    void UpdateProjection();

    float m_Near; //Near clipping plane
    float m_Far; //Far clipping plane
    float m_Width; //Half-width of the view volume
    float m_Height; //Half-height of the view volume

    Vector3D m_Position; //Camera position
    float m_Rotation; //Rotation around Z-axis in radians
    float m_Zoom; //Zoom factor

    Matrix4 m_ProjectionMatrix; //Projection matrix
    Matrix4 m_ViewMatrix; //View matrix
    bool m_IsDirty; //True when view matrix needs recalculation
};
