#pragma once

#include "events/Event.h"
#include "math/Vector2D.h"
#include <SDL3/SDL.h>

/**
 * @brief Event representing mouse movement.
 *
 * Contains the current mouse position and the delta (movement since last frame).
 *
 * @note This event is dispatched by SDLEventTranslator.
 * @see SDLEventTranslator
 */
class MouseMotionEvent : public Event{
public:
    MouseMotionEvent(const Vector2D& position, const Vector2D& delta)
    : m_Position(position), m_Delta(delta){}

    Vector2D GetPosition() const{ return m_Position; }
    Vector2D GetDelta() const{ return m_Delta; }

    static EventType GetStaticType(){ return EventType::MouseMotion; }
    EventType GetEventType() const override{ return EventType::MouseMotion; }

private:
    Vector2D m_Position;
    Vector2D m_Delta;
};

/**
 * @brief Mouse button identifiers (matches SDL button numbers).
 */
enum class MouseButton{
    Left = 1,
    Middle = 2,
    Right = 3,
    X1 = 4,
    X2 = 5
};

/**
 * @brief Mouse button action types.
 */
enum class MouseButtonAction{
    Pressed,
    Released
};

/**
 * @brief Event representing a mouse button press/release.
 *
 * Contains which button was pressed/released, the action type,
 * and the mouse position at the time of the event.
 */
class MouseButtonEvent : public Event{
public:
    MouseButtonEvent(MouseButton button, MouseButtonAction action, const Vector2D& position)
    : m_Button(button), m_Action(action), m_Position(position){}

    MouseButton GetButton() const{ return m_Button; }
    MouseButtonAction GetAction() const{ return m_Action; }
    Vector2D GetPosition() const{ return m_Position; }

    static EventType GetStaticType(){ return EventType::MouseButton; }
    EventType GetEventType() const override{ return EventType::MouseButton; }

private:
    MouseButton m_Button;
    MouseButtonAction m_Action;
    Vector2D m_Position;
};

/**
 * @brief Event representing mouse wheel scroll.
 *
 * Contains the scroll delta.
 */
class MouseWheelEvent : public Event{
public:
    MouseWheelEvent(const Vector2D& delta) : m_Delta(delta) {}

    Vector2D GetDelta() const{ return m_Delta; }

    static EventType GetStaticType(){ return EventType::MouseWheel; }
    EventType GetEventType() const override{ return EventType::MouseWheel; }

private:
    Vector2D m_Delta;
};
