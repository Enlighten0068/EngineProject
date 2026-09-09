#pragma once

#include "events/Event.h"

/**
 * @brief Event dispatched when the window close button is clicked or a SDL_EVENT_QUIT is received.
 *
 * This event is typically used to signal the application to shut down.
 *
 * @note Dispatched by SDLEventTranslator.
 * @see SDLEventTranslator
 */
class WindowCloseEvent : public Event{
public:
    static EventType GetStaticType(){ return EventType::WindowClose; }
    EventType GetEventType() const override{ return EventType::WindowClose; }
};

/**
 * @brief Event dispatched when the window is resized.
 *
 * Contains the new width and height of the window.
 *
 * @note Dispatched by SDLEventTranslator on SDL_EVENT_WINDOW_RESIZED.
 */
class WindowResizeEvent : public Event{
public:
    /**
     * @brief Constructs a WindowResizeEvent.
     * @param width New window width in pixels.
     * @param height New window height in pixels.
     */
    WindowResizeEvent(int width, int height) : m_Width(width), m_Height(height){}

    EventType GetEventType() const override{ return EventType::WindowResize; }

    int GetWidth() const{ return m_Width; }
    int GetHeight() const{ return m_Height; }

private:
    int m_Width; //New window width
    int m_Height; //New window height
};
