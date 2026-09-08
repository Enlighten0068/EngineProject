#pragma once

/**
 * @brief 3D vector struct with basic arithmetic operations.
 *
 * Used for positions, directions, and other 3D spatial data.
 * All operators are const and return a new vector.
 */
struct Vector3D{
    float x;
    float y;
    float z;

    /**
     * @brief Default constructor: zero vector.
     */
    Vector3D() : x(0.0f), y(0.0f), z(0.0f){}

    /**
     * @brief Constructs a vector from three floats.
     * @param xValue X component.
     * @param yValue Y component.
     * @param zValue Z component.
     */
    Vector3D(float xValue, float yValue, float zValue) : x(xValue), y(yValue), z(zValue){}

    /**
     * @brief Vector addition.
     * @param other Vector to add.
     * @return New vector with component-wise sum.
     */
    Vector3D operator+(const Vector3D& other) const{
        return{
            x + other.x,
            y + other.y,
            z + other.z
        };
    }

    /**
     * @brief Vector subtraction.
     * @param other Vector to subtract.
     * @return New vector with component-wise difference.
     */
    Vector3D operator-(const Vector3D& other) const{
        return{
            x - other.x,
            y - other.y,
            z - other.z
        };
    }

    /**
     * @brief Scalar multiplication.
     * @param scalar Multiplicand.
     * @return New vector with components multiplied by scalar.
     */
    Vector3D operator*(float scalar) const{
        return{
            x * scalar,
            y * scalar,
            z * scalar
        };
    }
};
