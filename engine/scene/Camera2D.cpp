#include "scene/Camera2D.h"
#include "math/Matrix4.h"

Camera2D::Camera2D(float left, float right, float bottom, float top, float nearPlane, float farPlane)
: m_Position(0.0f, 0.0f, 0.0f), m_Rotation(0.0f), m_Zoom(1.0f), m_Dirty(true) {
    m_ProjectionMatrix = Matrix4::Orthographic(left, right, bottom, top, nearPlane, farPlane);
}

void Camera2D::SetPosition(const Vector3D& position) {
    m_Position = position;
    m_Dirty = true;
}

void Camera2D::SetRotation(float angleRadians) {
    m_Rotation = angleRadians;
    m_Dirty = true;
}

void Camera2D::SetZoom(float zoom) {
    m_Zoom = zoom;
    m_Dirty = true;
}

void Camera2D::Update() {
    if (!m_Dirty) return;

    Matrix4 view = Matrix4::Translation(Vector3D(-m_Position.x, -m_Position.y, -m_Position.z));


    if (m_Rotation != 0.0f) {
        Matrix4 rot = Matrix4::RotationZ(-m_Rotation);
        view = rot * view;
    }


    if (m_Zoom != 1.0f) {
        Matrix4 scale = Matrix4::Scale(Vector3D(m_Zoom, m_Zoom, 1.0f));
        view = view * scale;
    }

    m_ViewMatrix = view;
    m_Dirty = false;
}

const Matrix4& Camera2D::GetViewMatrix() const {
    return m_ViewMatrix;
}

const Matrix4& Camera2D::GetProjectionMatrix() const {
    return m_ProjectionMatrix;
}
