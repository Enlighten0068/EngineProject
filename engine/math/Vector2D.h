#pragma once

#include <cmath>
struct Vector2D{
    float x;
    float y;

    Vector2D() : x(0.0f), y(0.0f){}

    Vector2D(float xValue,float yValue) : x(xValue), y(yValue){}

    Vector2D operator+(const Vector2D& other) const{
        return {x + other.x, y + other.y};
    }

    Vector2D operator-(const Vector2D& other) const{
        return {x - other.x, y - other.y};
    }

    Vector2D operator*(float scalar) const{
        return {x * scalar, y * scalar};
    }

    Vector2D operator/(float scalar) const{
        return {x / scalar, y / scalar};
    }

    float Length() const{
        return std::sqrt(x * x + y * y);
    }

    Vector2D Normalized() const{
        float len = Length();
        if (len > 0.0f){
            return *this / len;
        }
        return *this;
    }
};
