#pragma once

/**
 * @brief Marks an entity as an enemy.
 *
 * This is a tag component used by the EnemySystem to identify and process enemy entities.
 * It contains a flag to enable/disable the enemy without destroying the entity.
 *
 * @note This component is used in conjunction with Transform, Patrol, and PhysicsBody.
 * @see Patrol, PhysicsBody, EnemySystem
 */
struct Enemy{
    bool IsActive = true; //Tag if enemy is active and should be processed
};
