#pragma once

#include "math/Vector3D.h"
#include <cmath>

/**
 * @brief 4x4 matrix class for 3D transformations and projections.
 *
 * The matrix is stored in column-major order (OpenGL style), meaning the data
 * array is organized as:
 *
 *   [ col0, col1, col2, col3 ]
 *
 * where each column is a Vector4.
 *
 * @note All static methods return a new Matrix4 instance.
 * @see Vector3D
 */
class Matrix4{
public:
    /**
     * @brief Constructs a zero-initialized matrix.
     */
    Matrix4();

    /**
     * @brief Returns a pointer to the underlying data array.
     * @return Pointer to the first element of the 16-float array.
     */
    const float* Data() const;

    /**
     * @brief Creates an identity matrix (1 on the main diagonal, 0 elsewhere).
     * @return Identity matrix.
     */
    static Matrix4 Identity();

    /**
     * @brief Creates a translation matrix.
     * @param position Translation vector (x, y, z).
     * @return Translation matrix.
     */
    static Matrix4 Translation(const Vector3D& position);

    /**
     * @brief Creates a scale matrix.
     * @param scale Scale factors (x, y, z).
     * @return Scale matrix.
     */
    static Matrix4 Scale(const Vector3D& scale);

    /**
     * @brief Matrix multiplication operator.
     * @param other Matrix to multiply with (on the right).
     * @return Result of `this * other`.
     */
    Matrix4 operator*(const Matrix4& other) const;

    /**
     * @brief Creates an orthographic projection matrix.
     *
     * This maps a 3D volume (left-right, bottom-top, near-far) to a 2D screen.
     *
     * @param left Left clipping plane.
     * @param right Right clipping plane.
     * @param bottom Bottom clipping plane.
     * @param top Top clipping plane.
     * @param nearPlane Near clipping plane.
     * @param farPlane Far clipping plane.
     * @return Orthographic projection matrix.
     */
    static Matrix4 Orthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);

    /**
     * @brief Creates a rotation matrix around the Z-axis.
     * @param angleRadians Rotation angle in radians.
     * @return Rotation matrix (Z-axis roll).
     */
    static Matrix4 RotationZ(float angleRadians);

private:
    float m_Data[16]; //Matrix data in column-major order
};
