#include "math/Matrix4.h"


Matrix4::Matrix4(){
    for (int i = 0; i < 16; i++){
        m_Data[i] = 0.0f;
    }
}

Matrix4 Matrix4::Identity(){
    Matrix4 result;

    result.m_Data[0]  = 1.0f;
    result.m_Data[5]  = 1.0f;
    result.m_Data[10] = 1.0f;
    result.m_Data[15] = 1.0f;

    return result;
}

const float* Matrix4::Data() const{
    return m_Data;
}

Matrix4 Matrix4::Translation(const Vector3D& position){
    Matrix4 result = Identity();

    result.m_Data[12] = position.x;
    result.m_Data[13] = position.y;
    result.m_Data[14] = position.z;

    return result;
}

Matrix4 Matrix4::Scale(const Vector3D& scale){
    Matrix4 result = Identity();

    result.m_Data[0]  = scale.x;
    result.m_Data[5]  = scale.y;
    result.m_Data[10] = scale.z;

    return result;
}

Matrix4 Matrix4::operator*(const Matrix4& other) const{
        Matrix4 result;

        for (int row = 0; row < 4; row++){
            for (int column = 0; column < 4; column++){
                result.m_Data[column * 4 + row] =
                m_Data[0 * 4 + row] * other.m_Data[column * 4 + 0] +
                m_Data[1 * 4 + row] * other.m_Data[column * 4 + 1] +
                m_Data[2 * 4 + row] * other.m_Data[column * 4 + 2] +
                m_Data[3 * 4 + row] * other.m_Data[column * 4 + 3];
            }
        }
        return result;
    }

Matrix4 Matrix4::Orthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane){
        Matrix4 result = Identity();

        result.m_Data[0] = 2.0f / (right - left);
        result.m_Data[5] = 2.0f / (top - bottom);
        result.m_Data[10] = -2.0f / (farPlane - nearPlane);
        result.m_Data[12] =-(right + left) / (right - left);
        result.m_Data[13] =-(top + bottom) /(top - bottom);
        result.m_Data[14] =-(farPlane + nearPlane) /(farPlane - nearPlane);

        return result;
    }

Matrix4 Matrix4::RotationZ(float angle) {
        Matrix4 result = Identity();

        float c = std::cos(angle);
        float s = std::sin(angle);
        result.m_Data[0] = c;
        result.m_Data[1] = -s;
        result.m_Data[4] = s;
        result.m_Data[5] = c;

        return result;
    }
