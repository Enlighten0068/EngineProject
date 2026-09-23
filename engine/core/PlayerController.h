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
 * @see GameWorld, Transform, SoundManager
 */
class PlayerController{
public:
    /**
     * @brief Constructs a PlayerController.
     * @param registry Reference to the ECS registry.
     * @param playerEntity The entity ID of the player.
     * @param world Reference to the GameWorld for boundary clamping.
     */
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

    /**
     * @brief Gets the movement speed of the player.
     * @return Current speed in units per second.
     */
    float GetSpeed() const{ return m_Speed; }

    /**
     * @brief Kills the player and triggers respawn.
     * @param cause Reason for death (e.g., "Player fell off the world").
     */
    void Die(const std::string& cause = "Unknown.");

    //Physics state accessors
    Vector3D& GetVelocity(){ return m_Velocity; }
    const Vector3D& GetVelocity() const{ return m_Velocity; }
    void SetGrounded(bool grounded){ m_IsGrounded = grounded; }
    bool IsGrounded() const{ return m_IsGrounded; }

private:
    /**
     * @brief Sets up event subscriptions for player interactions.
     *
     * Currently subscribes to key events (E key for interaction).
     * Still under development.
     */
    void SetupEventSubscriptions();

    entt::registry& m_Registry; //ECS registry
    entt::entity m_PlayerEntity; //Player entity ID

    Vector3D m_Velocity{0.0f, 0.0f, 0.0f}; //Current velocity
    float m_Speed = 3.0f; //Movement speed

    //Physics state flags
    bool m_IsJumping = false; //Currently jumping
    float m_JumpTimer = 0.0f; //Time spent in current jump
    bool m_IsDead = false; //Dead state
    bool m_IsGrounded = false; //Touching ground
    bool m_WasGrounded = false; //Previous frame grounded state

    Vector3D m_SpawnPosition{0.0f, 0.0f, 0.0f}; //Capture Spawn Position for respawning

    const GameWorld& m_World; //World for boundary clamping
};
