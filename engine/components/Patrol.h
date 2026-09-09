#pragma once

#include "math/Vector3D.h"

namespace Components{

    /**
     * @brief Defines patrol behavior for an entity.
     *
     * An entity with this component will move back and forth between two positions
     * at a specified speed. Used primarily for enemy AI in platformer games.
     *
     * @note The movement direction (MovingRight) is automatically toggled when the entity
     *       reaches either end of the patrol range.
     * @see Enemy, PhysicsBody, EnemySystem
     */
    struct Patrol{
        Vector3D StartPosition; //Starting point for patrol range
        Vector3D EndPosition; //Ending point for patrol range
        float Speed = 2.0f; //Movement speed in units per second
        bool MovingRight = true; //Direction flag: true moves right, false moves left
    };
}
