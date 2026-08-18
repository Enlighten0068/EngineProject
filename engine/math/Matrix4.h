#pragma once

class Matrix4{
public:
    Matrix4();
    static Matrix4 Identity();
    const float* Data() const;

private:
    float m_Data[16];
};
