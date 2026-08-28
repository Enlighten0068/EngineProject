#pragma once

#include "events/Event.h"
#include <SDL3/SDL.h>

enum class KeyAction{
    Pressed,
    Released,
    Repeated
};

class KeyEvent : public Event{
public:
    KeyEvent(SDL_Scancode scancode, KeyAction action, bool ctrl, bool shift, bool alt)
    : m_Scancode(scancode), m_Action(action), m_Ctrl(ctrl), m_Shift(shift), m_Alt(alt) {}

    SDL_Scancode GetScancode() const{ return m_Scancode; }
    KeyAction GetAction() const{ return m_Action; }
    bool IsCtrlPressed() const{ return m_Ctrl; }
    bool IsShiftPressed() const{ return m_Shift; }
    bool IsAltPressed() const{ return m_Alt; }

    static EventType GetStaticType(){ return EventType::Key; }
    EventType GetEventType() const override{ return EventType::Key; }

private:
    SDL_Scancode m_Scancode;
    KeyAction m_Action;
    bool m_Ctrl;
    bool m_Shift;
    bool m_Alt;
};
