#pragma once

#include "events/Event.h"
#include <SDL3/SDL.h>

/**
 * @brief Keyboard action types.
 */
enum class KeyAction{
    Pressed,
    Released,
    Repeated
};

/**
 * @brief Event representing a keyboard key press/release.
 *
 * Contains information about which key was pressed, the action type,
 * and the state of modifier keys (Ctrl, Shift, Alt).
 *
 * @note This event is dispatched by SDLEventTranslator.
 * @see SDLEventTranslator, EventBus
 */
class KeyEvent : public Event{
public:
    KeyEvent(SDL_Scancode scancode, KeyAction action, bool ctrl, bool shift, bool alt)
    : m_Scancode(scancode), m_Action(action), m_Ctrl(ctrl), m_Shift(shift), m_Alt(alt) {}

    /**
     * @brief Get the SDL scancode of the key.
     * @return SDL_Scancode value.
     */
    SDL_Scancode GetScancode() const{ return m_Scancode; }

    /**
     * @brief Get the action type (pressed, released, repeated).
     * @return KeyAction enum value.
     */
    KeyAction GetAction() const{ return m_Action; }
    bool IsCtrlPressed() const{ return m_Ctrl; }
    bool IsShiftPressed() const{ return m_Shift; }
    bool IsAltPressed() const{ return m_Alt; }

    static EventType GetStaticType(){ return EventType::Key; }
    EventType GetEventType() const override{ return EventType::Key; }

private:
    SDL_Scancode m_Scancode; //SDL scancode of the key
    KeyAction m_Action;
    bool m_Ctrl;
    bool m_Shift;
    bool m_Alt;
};
