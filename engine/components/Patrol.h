#pragma once

#include "math/Vector3D.h"

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
    //Starting and ending point for patrol range
    Vector3D StartPosition;
    Vector3D EndPosition;

    float Speed = 2.0f;
    bool MovingRight = true; //Moves right if true, left otherwise
};
