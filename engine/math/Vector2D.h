#pragma once

#include <cmath>

/**
 * @brief 2D vector struct with basic arithmetic operations.
 *
 * Used for positions, directions, and other 2D spatial data.
 * All operators are const and return a new vector.
 */
struct Vector2D{
    float x;
    float y;

    /**
     * @brief Default constructor: zero vector.
     */
    Vector2D() : x(0.0f), y(0.0f){}

    /**
     * @brief Constructs a vector from two floats.
     * @param xValue X component.
     * @param yValue Y component.
     */
    Vector2D(float xValue,float yValue) : x(xValue), y(yValue){}

    /**
     * @brief Vector addition.
     * @param other Vector to add.
     * @return New vector with component-wise sum.
     */
    Vector2D operator+(const Vector2D& other) const{
        return {x + other.x, y + other.y};
    }

    /**
     * @brief Vector subtraction.
     * @param other Vector to subtract.
     * @return New vector with component-wise difference.
     */
    Vector2D operator-(const Vector2D& other) const{
        return {x - other.x, y - other.y};
    }

    /**
     * @brief Scalar multiplication.
     * @param scalar Multiplicand.
     * @return New vector with components multiplied by scalar.
     */
    Vector2D operator*(float scalar) const{
        return {x * scalar, y * scalar};
    }

    /**
     * @brief Scalar division.
     * @param scalar Divisor (MUST NOT BE 0).
     * @return New vector with components divided by scalar.
     */
    Vector2D operator/(float scalar) const{
        return {x / scalar, y / scalar};
    }

    /**
     * @brief Computes the Euclidean length (magnitude) of the vector.
     * @return Length of the vector.
     */
    float Length() const{
        return std::sqrt(x * x + y * y);
    }

    /**
     * @brief Returns a normalized copy of the vector.
     * @return Normalized vector (unit length), or itself if length is zero.
     */
    Vector2D Normalized() const{
        float len = Length();
        if (len > 0.0f){
            return *this / len;
        }
        return *this;
    }
};
