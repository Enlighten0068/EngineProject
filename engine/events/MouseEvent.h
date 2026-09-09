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
    /**
     * @brief Constructs a MouseMotionEvent.
     * @param position Current mouse position in screen coordinates.
     * @param delta Movement since last frame.
     */
    MouseMotionEvent(const Vector2D& position, const Vector2D& delta)
    : m_Position(position), m_Delta(delta){}

    Vector2D GetPosition() const{ return m_Position; }
    Vector2D GetDelta() const{ return m_Delta; }

    static EventType GetStaticType(){ return EventType::MouseMotion; }
    EventType GetEventType() const override{ return EventType::MouseMotion; }

private:
    Vector2D m_Position; //Current mouse position
    Vector2D m_Delta; //Mouse movement delta
};

/**
 * @brief Mouse button identifiers (matches SDL button numbers).
 */
enum class MouseButton{
    Left = 1,   //Left mouse button
    Middle = 2, //Middle mouse button
    Right = 3,  //Right mouse button
    X1 = 4,     //X1 mouse button (back)
    X2 = 5      //X2 mouse button (forward)
};

/**
 * @brief Mouse button action types.
 */
enum class MouseButtonAction{
    Pressed,  //Button was pressed down
    Released  //Button was released
};

/**
 * @brief Event representing a mouse button press/release.
 *
 * Contains which button was pressed/released, the action type,
 * and the mouse position at the time of the event.
 */
class MouseButtonEvent : public Event{
public:
    /**
     * @brief Constructs a MouseButtonEvent.
     * @param button The mouse button.
     * @param action Action type (pressed or released).
     * @param position Mouse position at the time of the event.
     */
    MouseButtonEvent(MouseButton button, MouseButtonAction action, const Vector2D& position)
    : m_Button(button), m_Action(action), m_Position(position){}

    MouseButton GetButton() const{ return m_Button; }
    MouseButtonAction GetAction() const{ return m_Action; }
    Vector2D GetPosition() const{ return m_Position; }

    static EventType GetStaticType(){ return EventType::MouseButton; }
    EventType GetEventType() const override{ return EventType::MouseButton; }

private:
    MouseButton m_Button; //Button identifier
    MouseButtonAction m_Action; //Action type
    Vector2D m_Position; //Mouse position
};

/**
 * @brief Event representing mouse wheel scroll.
 *
 * Contains the scroll delta.
 */
class MouseWheelEvent : public Event{
public:
    /**
     * @brief Constructs a MouseWheelEvent.
     * @param delta Scroll delta (x = horizontal, y = vertical).
     */
    MouseWheelEvent(const Vector2D& delta) : m_Delta(delta){}

    Vector2D GetDelta() const{ return m_Delta; }

    static EventType GetStaticType(){ return EventType::MouseWheel; }
    EventType GetEventType() const override{ return EventType::MouseWheel; }

private:
    Vector2D m_Delta; //Scroll delta
};
