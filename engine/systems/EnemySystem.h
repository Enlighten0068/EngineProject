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

class EnemySystem{
public:
    static void Update(entt::registry& registry, float deltaTime,
                       const std::vector<entt::entity>& platforms,float worldMinY){
        auto view = registry.view<Components::Transform, Enemy, Patrol, Components::PhysicsBody>();

        for (auto [entity, transform, enemy, patrol, physics] : view.each()){
            if (!enemy.IsActive) continue;

            //Horizontal enemy movement
            float speed = patrol.Speed * deltaTime;
            if (patrol.MovingRight){
                transform.Position.x += speed;
                if (transform.Position.x >= patrol.EndPosition.x){
                    patrol.MovingRight = false;
                }
            } else{
                transform.Position.x -= speed;
                if (transform.Position.x <= patrol.StartPosition.x){
                    patrol.MovingRight = true;
                }
            }

            //Enemy gravity
            float gravity = -9.8f * physics.GravityScale;
            physics.Velocity.y += gravity * deltaTime;

            transform.Position.y += physics.Velocity.y * deltaTime;

            //Enemy collision
            Vector3D enemyHalfSize = transform.Scale * 0.5f;
            bool grounded = false;

            for (entt::entity platform : platforms){
                auto& platformTransform = registry.get<Components::Transform>(platform);
                Vector3D platformHalfSize = platformTransform.Scale * 0.5f;

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

                if (overlapX && overlapY){
                    Vector3D delta = transform.Position - platformTransform.Position;
                    Vector3D overlap = enemyHalfSize + platformHalfSize -
                    Vector3D(std::abs(delta.x), std::abs(delta.y), 0.0f);

                    if (overlap.y < overlap.x){
                        if (delta.y > 0.0f){
                            transform.Position.y += overlap.y;
                            physics.Velocity.y = 0.0f;
                            grounded = true;
                        } else{
                            transform.Position.y -= overlap.y;
                            if (physics.Velocity.y > 0.0f){
                                physics.Velocity.y = 0.0f;
                            }
                        }
                    } else{
                        if (delta.x > 0.0f) transform.Position.x += overlap.x;
                            else transform.Position.x -= overlap.x;
                    }
                }
            }

            float worldFloor = worldMinY + enemyHalfSize.y;
            if (transform.Position.y < worldFloor){
                transform.Position.y = worldFloor;
                physics.Velocity.y = 0.0f;
                grounded = true;
            }

            physics.IsGrounded = grounded;

        }
    }

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
