#pragma once

#include "events/Event.h"
#include <SDL3/SDL.h>

/**
 * @brief Keyboard action types.
 */
enum class KeyAction{
    Pressed,  //Key was pressed down
    Released, //Key was released
    Repeated  //Key is being held (repeat event)
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
    /**
     * @brief Constructs a KeyEvent.
     * @param scancode SDL scancode of the key.
     * @param action Action type (pressed, released, repeated).
     * @param ctrl True if Ctrl key is held.
     * @param shift True if Shift key is held.
     * @param alt True if Alt key is held.
     */
    KeyEvent(SDL_Scancode scancode, KeyAction action, bool ctrl, bool shift, bool alt)
    : m_Scancode(scancode), m_Action(action), m_Ctrl(ctrl), m_Shift(shift), m_Alt(alt){}

    SDL_Scancode GetScancode() const{ return m_Scancode; }
    KeyAction GetAction() const{ return m_Action; }
    bool IsCtrlPressed() const{ return m_Ctrl; }
    bool IsShiftPressed() const{ return m_Shift; }
    bool IsAltPressed() const{ return m_Alt; }

    static EventType GetStaticType(){ return EventType::Key; }
    EventType GetEventType() const override{ return EventType::Key; }

private:
    SDL_Scancode m_Scancode; //SDL scancode of the key
    KeyAction m_Action; //Action type
    bool m_Ctrl; //Ctrl key held
    bool m_Shift; //Shift key held
    bool m_Alt; //Alt key held
};
