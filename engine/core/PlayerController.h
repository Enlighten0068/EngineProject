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
    entt::registry& m_Registry;
    entt::entity m_PlayerEntity;
    float m_Speed = 3.0f;
};
