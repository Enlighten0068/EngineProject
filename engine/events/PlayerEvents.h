#pragma once

#include "events/Event.h"
#include "math/Vector3D.h"

/**
 * @brief Event dispatched when the player jumps.
 *
 * Contains the player's position at the time of the jump and the jump force.
 *
 * @note Dispatched by PlayerController.
 * @see PlayerController
 */
class PlayerJumpedEvent : public Event{
public:
    PlayerJumpedEvent(const Vector3D& position, float jumpForce)
    : m_Position(position), m_JumpForce(jumpForce) {}

    const Vector3D& GetPosition() const{ return m_Position; }
    float GetJumpForce() const{ return m_JumpForce; }

    static EventType GetStaticType(){ return EventType::PlayerJumped; }
    EventType GetEventType() const override{ return EventType::PlayerJumped; }

private:
    Vector3D m_Position;
    float m_JumpForce;
};

/**
 * @brief Event dispatched when the player lands on a surface.
 *
 * Contains the player's position at landing and the fall speed.
 *
 * @note Dispatched by PlayerController when grounded detection triggers.
 */
class PlayerLandedEvent : public Event{
public:
    PlayerLandedEvent(const Vector3D& position, float fallSpeed)
    : m_Position(position), m_FallSpeed(fallSpeed) {}

    const Vector3D& GetPosition() const{ return m_Position; }
    float GetFallSpeed() const{ return m_FallSpeed; }

    static EventType GetStaticType(){ return EventType::PlayerLanded; }
    EventType GetEventType() const override{ return EventType::PlayerLanded; }

private:
    Vector3D m_Position;
    float m_FallSpeed;
};

/**
 * @brief Event dispatched when the player dies.
 *
 * Contains the player's position at death and a cause string.
 *
 * @note Dispatched by PlayerController::Die().
 */
class PlayerDiedEvent : public Event{
public:
    PlayerDiedEvent(const Vector3D& position, const std::string& cause = "Unknown")
    : m_Position(position), m_Cause(cause) {}

    const Vector3D& GetPosition() const{ return m_Position; }
    const std::string& GetCause() const{ return m_Cause; }

    static EventType GetStaticType(){ return EventType::PlayerDied; }
    EventType GetEventType() const override{ return EventType::PlayerDied; }

private:
    Vector3D m_Position;
    std::string m_Cause;
};
