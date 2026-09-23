#pragma once

#include "components/Transform.h"
#include "components/Enemy.h"
#include "components/Patrol.h"
#include "components/SpriteRenderer.h"
#include "components/PhysicsBody.h"
#include "core/Time.h"
#include "diagnostics/Log.h"
#include "math/Vector3D.h"
#include <entt.hpp>

/**
 * @brief System that updates enemy entities with patrol, physics, and collision.
 *
 * The EnemySystem processes all entities that have the Enemy, Patrol, and PhysicsBody
 * components. It handles:
 *
 * - Horizontal patrol movement between two points
 * - Gravity and vertical velocity
 * - Collision detection and resolution with platforms
 * - Grounded state detection
 * - World boundary clamping
 *
 * @note This system is designed to be called once per frame from the scene's Update().
 * @see Components::Enemy, Components::Patrol, Components::PhysicsBody, Components::Transform
 */
class EnemySystem{
public:
    /**
        * @brief Updates all active enemy entities.
        *
        * This method iterates over all entities with Transform, Enemy, Patrol, and PhysicsBody
        * components. For each active enemy, it:
        * 1. Moves the enemy horizontally according to its patrol path.
        * 2. Applies gravity to the enemy's vertical velocity.
        * 3. Checks and resolves collisions with platforms.
        * 4. Clamps the enemy to the world floor.
        * 5. Updates the grounded state.
        *
        * @param registry Reference to the ECS registry containing all entities.
        * @param deltaTime Time elapsed since the last frame (seconds).
        * @param platforms List of entity IDs that act as collision platforms.
        * @param worldMinY The minimum Y coordinate of the world (floor level).
        */
    static void Update(entt::registry& registry, float deltaTime,
                        const std::vector<entt::entity>& platforms, float worldMinY){
        //View: entities with Transform, Enemy, Patrol, and PhysicsBody
        auto view = registry.view<Components::Transform, Components::Enemy,
        Components::Patrol, Components::PhysicsBody>();

        for(auto [entity, transform, enemy, patrol, physics] : view.each()){
            if(!enemy.IsActive) continue;

            //Enemy patrol movement — clamped to patrol bounds
            float speed = patrol.Speed * deltaTime;
            if(patrol.MovingRight){
                transform.Position.x += speed;
                if(transform.Position.x >= patrol.EndPosition.x){
                    transform.Position.x = patrol.EndPosition.x; //hard clamp
                    patrol.MovingRight = false;
                }
            } else{
                transform.Position.x -= speed;
                if(transform.Position.x <= patrol.StartPosition.x){
                    transform.Position.x = patrol.StartPosition.x; //hard clamp
                    patrol.MovingRight = true;
                }
            }

            //Apply gravity
            float gravity = -9.8f * physics.GravityScale;
            physics.Velocity.y += gravity * deltaTime;
            transform.Position.y += physics.Velocity.y * deltaTime;

            //Collision detection and resolution with platforms
            Vector3D enemyHalfSize = transform.Scale * 0.5f;
            bool grounded = false;

            for(entt::entity platform : platforms){
                auto& platformTransform = registry.get<Components::Transform>(platform);
                Vector3D platformHalfSize = platformTransform.Scale * 0.5f;

                //Calculate AABB bounds
                float enemyLeft = transform.Position.x - enemyHalfSize.x;
                float enemyRight = transform.Position.x + enemyHalfSize.x;
                float enemyBottom = transform.Position.y - enemyHalfSize.y;
                float enemyTop = transform.Position.y + enemyHalfSize.y;

                float platformLeft = platformTransform.Position.x - platformHalfSize.x;
                float platformRight = platformTransform.Position.x + platformHalfSize.x;
                float platformTop = platformTransform.Position.y + platformHalfSize.y;
                float platformBottom = platformTransform.Position.y - platformHalfSize.y;

                bool overlapX = (enemyRight > platformLeft && enemyLeft < platformRight);
                bool overlapY = (enemyTop > platformBottom && enemyBottom < platformTop);

                if(overlapX && overlapY){
                    //Resolve collision
                    Vector3D delta = transform.Position - platformTransform.Position;
                    Vector3D overlap = enemyHalfSize + platformHalfSize -
                    Vector3D(std::abs(delta.x), std::abs(delta.y), 0.0f);

                    if(overlap.y < overlap.x){
                        //Vertical collision
                        if(delta.y > 0.0f){
                            transform.Position.y += overlap.y;
                            physics.Velocity.y = 0.0f;
                            grounded = true;
                        } else{
                            transform.Position.y -= overlap.y;
                            if(physics.Velocity.y > 0.0f){
                                physics.Velocity.y = 0.0f;
                            }
                        }
                    } else{
                        //Horizontal collision
                        if(delta.x > 0.0f) transform.Position.x += overlap.x;
                        else transform.Position.x -= overlap.x;
                    }
                }
            }

            //Clamp to world floor
            float worldFloor = worldMinY + enemyHalfSize.y;
            if(transform.Position.y < worldFloor){
                transform.Position.y = worldFloor;
                physics.Velocity.y = 0.0f;
                grounded = true;
            }

            //Update grounded state
            physics.IsGrounded = grounded;
        }
    }

    /**
    * @brief Performs AABB collision detection between the player and an enemy.
    *
    * This method checks if the player's bounding box overlaps with an enemy's
    * bounding box using the Axis-Aligned Bounding Box (AABB) algorithm.
    *
    * @param playerPos The player's world position.
    * @param playerSize The player's scale (width and height).
    * @param enemyPos The enemy's world position.
    * @param enemySize The enemy's scale (width and height).
    * @return true if the player and enemy are overlapping, false otherwise.
    */
    static bool CheckCollision(const Vector3D& playerPos, const Vector3D& playerSize,
                                const Vector3D& enemyPos, const Vector3D& enemySize){
        Vector3D playerHalf = playerSize * 0.5f;
        Vector3D enemyHalf = enemySize * 0.5f;

        float dx = std::abs(playerPos.x - enemyPos.x);
        float dy = std::abs(playerPos.y - enemyPos.y);

        return (dx < (playerHalf.x + enemyHalf.x)) &&
        (dy < (playerHalf.y + enemyHalf.y));
        }
};
