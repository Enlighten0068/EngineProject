#pragma once

#include "events/Event.h"

class WindowCloseEvent : public Event{
public:
    EventType GetEventType() const override{
        return EventType::WindowClose;
    }
};

class WindowResizeEvent : public Event{
public:
    WindowResizeEvent(int width, int height): m_Width(width),m_Height(height){}

    EventType GetEventType() const override{
        return EventType::WindowResize;
    }

    int GetWidth() const{
        return m_Width;
    }

    int GetHeight() const{
        return m_Height;
    }

private:
    int m_Width;
    int m_Height;
};
