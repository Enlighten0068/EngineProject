#pragma once

#include "math/Vector3D.h"

namespace Components{

    /**
     * @brief Adds physics properties to an entity.
     *
     * Entities with this component are affected by gravity and can participate
     * in collision detection and resolution. The velocity is updated each frame
     * by the physics system.
     *
     * @note This component is used for both the player and enemies.
     * @see Transform, EnemySystem (for enemies), PlayerController (for the player)
     */
    struct PhysicsBody{
        Vector3D Velocity{0.0f, 0.0f, 0.0f}; //Default velocity in units per second
        bool IsGrounded = false; //Tags if standing on a surface
        float GravityScale = 1.0f; //Gravity multiplier
    };
}
