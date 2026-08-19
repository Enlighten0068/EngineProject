#pragma once

struct Vector3D{
    float x;
    float y;
    float z;

    Vector3D() : x(0.0f), y(0.0f), z(0.0f){
    }

    Vector3D(float xValue, float yValue, float zValue) : x(xValue), y(yValue), z(zValue){
    }

    Vector3D operator+(const Vector3D& other) const{
            return {
                x + other.x,
                y + other.y,
                z + other.z
            };
        }

    Vector3D operator-(const Vector3D& other) const{
                return {
                    x - other.x,
                    y - other.y,
                    z - other.z
                };
            }

    Vector3D operator*(float scalar) const{
                    return {
                        x * scalar,
                        y * scalar,
                        z * scalar
                    };
                }
};
