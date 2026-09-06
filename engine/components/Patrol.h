#pragma once

#include "math/Vector3D.h"

struct Patrol{
    Vector3D StartPosition;
    Vector3D EndPosition;
    float Speed = 2.0f;
    bool MovingRight = true;
};
