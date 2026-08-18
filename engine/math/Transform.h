#pragma once

#include "math/Vector3D.h"

struct Transform{
    Vector3D Position;
    Vector3D Rotation;
    Vector3D Scale;

    Transform() : Position(0.0f, 0.0f, 0.0f),Rotation(0.0f, 0.0f, 0.0f),Scale(1.0f, 1.0f, 1.0f){
    }
};
