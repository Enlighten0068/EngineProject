#pragma once

#include "components/Transform.h"
#include <entt.hpp>

class PlayerController{
public:
    PlayerController(entt::registry& registry, entt::entity playerEntity);
    ~PlayerController() = default;

    void Update();

    void SetSpeed(float speed){ m_Speed = speed; }
    float GetSpeed() const{ return m_Speed; }

private:
    void SetupEventSubscriptions();

    entt::registry& m_Registry;
    entt::entity m_PlayerEntity;
    float m_Speed = 3.0f;

    bool m_IsJumping = false;
    float m_JumpTimer = 0.0f;
    Vector3D m_Velocity{0.0f, 0.0f, 0.0f};
};
