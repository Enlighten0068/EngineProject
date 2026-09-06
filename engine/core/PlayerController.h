#pragma once

#include "components/Transform.h"
#include "math/Vector3D.h"
#include "scene/GameWorld.h"
#include <entt.hpp>

class PlayerController{
public:
    PlayerController(entt::registry& registry, entt::entity playerEntity, const GameWorld& world);
    ~PlayerController() = default;

    void Update();

    void SetSpeed(float speed){ m_Speed = speed; }
    float GetSpeed() const{ return m_Speed; }

    void Die(const std::string& cause = "Unknown.");

    Vector3D& GetVelocity() { return m_Velocity; }
    const Vector3D& GetVelocity() const { return m_Velocity; }

    void SetGrounded(bool grounded) { m_IsGrounded = grounded; }
    bool IsGrounded() const { return m_IsGrounded; }
private:
    void SetupEventSubscriptions();

    entt::registry& m_Registry;
    entt::entity m_PlayerEntity;

    Vector3D m_Velocity{0.0f, 0.0f, 0.0f};
    float m_Speed = 3.0f;

    bool m_IsJumping = false;
    float m_JumpTimer = 0.0f;
    bool m_IsDead = false;
    bool m_IsGrounded = false;
    bool m_WasGrounded = false;

    const GameWorld& m_World;
};
