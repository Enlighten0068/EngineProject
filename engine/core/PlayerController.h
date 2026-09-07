#pragma once

#include "components/Transform.h"
#include "math/Vector3D.h"
#include "scene/GameWorld.h"
#include <entt.hpp>

/**
 * @brief Controller for the player entity, handling movement, jumping, and physics.
 *
 * This class processes keyboard and gamepad input, updates the player
 * position and velocity, and handles jump physics (variable jump height,
 * gravity, and grounded detection). It also integrates with the audio system
 * to play jump and landing sound effects.
 *
 * @note The controller works in conjunction with the physics system and
 *       collision resolution performed in the scene.
 */
class PlayerController{
public:
    PlayerController(entt::registry& registry, entt::entity playerEntity, const GameWorld& world);
    ~PlayerController() = default;

    /**
     * @brief Updates the player's state (movement, jump, physics).
     *
     * Should be called once per frame. Reads input, updates velocity and position,
     * and handles jump logic. Also detects landing events and plays sound effects.
     */
    void Update();

    /**
     * @brief Sets the movement speed of the player.
     * @param speed Speed in units per second.
     */
    void SetSpeed(float speed){ m_Speed = speed; }
    float GetSpeed() const{ return m_Speed; }

    /**
     * @brief Kills the player and triggers respawn.
     * @param cause Reason for death (e.g., "Player fell off the world").
     */
    void Die(const std::string& cause = "Unknown.");

    //Physics
    Vector3D& GetVelocity(){ return m_Velocity; }
    const Vector3D& GetVelocity() const{ return m_Velocity; }
    void SetGrounded(bool grounded){ m_IsGrounded = grounded; }
    bool IsGrounded() const{ return m_IsGrounded; }

private:
    /**
     * @brief Sets up event subscriptions.
     *
     * @note Still in development.
     */
    void SetupEventSubscriptions();

    //Entity
    entt::registry& m_Registry;
    entt::entity m_PlayerEntity;

    Vector3D m_Velocity{0.0f, 0.0f, 0.0f};
    float m_Speed = 3.0f;

    //Physics flags
    bool m_IsJumping = false;
    float m_JumpTimer = 0.0f;
    bool m_IsDead = false;
    bool m_IsGrounded = false;
    bool m_WasGrounded = false;

    const GameWorld& m_World;
};
