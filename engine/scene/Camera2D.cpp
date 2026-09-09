#include "diagnostics/Log.h"
#include "scene/Camera2D.h"
#include "math/Matrix4.h"
#include <format>

/**
 * @brief Constructs a 2D camera with specified view volume.
 *
 * Initializes the camera with the given view volume, sets default position
 * to (0,0,0), with no rotation, zoom=1, and calculates the initial projection matrix.
 */
Camera2D::Camera2D(float left, float right, float bottom, float top, float nearPlane, float farPlane)
: m_Position(0.0f, 0.0f, 0.0f), m_Rotation(0.0f), m_Zoom(1.0f),
m_Width((right - left) / 2.0f), m_Height((top - bottom) / 2.0f),
m_Near(nearPlane), m_Far(farPlane), m_IsDirty(true){
    UpdateProjection();
}

/**
 * @brief Sets the camera position and marks the view matrix as dirty.
 * @param position New position.
 */
void Camera2D::SetPosition(const Vector3D& position){
    m_Position = position;
    m_IsDirty = true;
}

/**
 * @brief Sets the camera rotation and marks the view matrix as dirty.
 * @param angleRadians Rotation angle in radians.
 */
void Camera2D::SetRotation(float angleRadians){
    m_Rotation = angleRadians;
    m_IsDirty = true;
}

/**
 * @brief Sets the camera zoom level and recalculates the projection matrix.
 * @param zoom Zoom factor (clamped to a minimum of 0.1).
 */
void Camera2D::SetZoom(float zoom){
    if(zoom < 0.1f) zoom = 0.1f;
    m_Zoom = zoom;
    UpdateProjection();
}

/**
 * @brief Sets the camera projection volume.
 * @param left Left boundary.
 * @param right Right boundary.
 * @param bottom Bottom boundary.
 * @param top Top boundary.
 */
void Camera2D::SetProjection(float left, float right, float bottom, float top){
    m_Width = (right - left) / 2.0f;
    m_Height = (top - bottom) / 2.0f;
    UpdateProjection();
    Log::Info(std::format("Camera Projection: left={}, right={}, bottom={}, top={}, width={}, height={}",
                          left, right, bottom, top, m_Width, m_Height));
}

/**
 * @brief Updates the view matrix ONLY when the camera is dirty.
 *
 * The view matrix is the inverse of the camera's transform:
 * 1. Apply translation to -position
 * 2. Apply rotation around Z-axis (inverse angle)
 */
void Camera2D::Update(){
    if(!m_IsDirty) return;

    //Build view matrix
    Matrix4 view = Matrix4::Translation(Vector3D(-m_Position.x, -m_Position.y, -m_Position.z));

    if(m_Rotation != 0.0f){
        Matrix4 rot = Matrix4::RotationZ(-m_Rotation);
        view = rot * view;
    }
    m_ViewMatrix = view;
    m_IsDirty = false;
}

/**
 * @brief Recalculates the orthographic projection matrix based on zoom.
 */
void Camera2D::UpdateProjection(){
    float left = -m_Width * (1.0f / m_Zoom);
    float right = m_Width * (1.0f / m_Zoom);
    float bottom = -m_Height * (1.0f / m_Zoom);
    float top = m_Height * (1.0f / m_Zoom);
    m_ProjectionMatrix = Matrix4::Orthographic(left, right, bottom, top, m_Near, m_Far);
}

/**
 * @brief Gets the view matrix.
 * @return Const reference to the view matrix.
 */
const Matrix4& Camera2D::GetViewMatrix() const{
    return m_ViewMatrix;
}

/**
 * @brief Gets the projection matrix.
 * @return Const reference to the projection matrix.
 */
const Matrix4& Camera2D::GetProjectionMatrix() const{
    return m_ProjectionMatrix;
}
