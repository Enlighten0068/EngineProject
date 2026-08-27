#pragma once

#include "math/Matrix4.h"
#include "math/Vector3D.h"

class Camera2D {
public:
    Camera2D(float left, float right, float bottom, float top, float nearPlane = -1.0f, float farPlane = 1.0f);

    void SetPosition(const Vector3D& position);
    void SetRotation(float angleRadians);
    void SetZoom(float zoom);

    Vector3D GetPosition() const{ return m_Position; }
    float GetZoom() const{ return m_Zoom; }

    const Matrix4& GetViewMatrix() const;
    const Matrix4& GetProjectionMatrix() const;
    void Update();

private:
    Vector3D m_Position;
    float    m_Rotation;
    float    m_Zoom;

    Matrix4 m_ProjectionMatrix;
    Matrix4 m_ViewMatrix;
    bool    m_Dirty;
};
