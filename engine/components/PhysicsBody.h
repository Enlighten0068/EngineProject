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
     * @see Transform, EnemySystem, PlayerController
     */
    struct PhysicsBody{
        Vector3D Velocity{0.0f, 0.0f, 0.0f}; //Current velocity in units per second
        bool IsGrounded = false; //Flag indicating if the entity is standing on a surface
        float GravityScale = 1.0f; //Multiplier for gravity (1.0 = normal gravity)
    };
}
