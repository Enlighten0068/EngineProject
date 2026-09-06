#pragma once

#include "math/Vector3D.h"

namespace Components{
    struct PhysicsBody{
        Vector3D Velocity{0.0f, 0.0f, 0.0f};
        bool IsGrounded = false;
        float GravityScale = 1.0f;
    };
}
