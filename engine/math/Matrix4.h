#pragma once

#include "math/Vector3D.h"
#include <cmath>

class Matrix4{
public:
    Matrix4();
    const float* Data() const;

    static Matrix4 Identity();
    static Matrix4 Translation(const Vector3D& position);
    static Matrix4 Scale(const Vector3D& scale);
    Matrix4 operator*(const Matrix4 & other) const;
    static Matrix4 Orthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);
    static Matrix4 RotationZ(float angleRadians);

private:
    float m_Data[16];
};
