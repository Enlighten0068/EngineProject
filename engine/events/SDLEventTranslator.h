#pragma once

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/WindowEvent.h"
#include "events/EventBus.h"
#include <SDL3/SDL.h>
#include <functional>

/**
 * @brief Translates SDL events into engine events and dispatches them via EventBus.
 *
 * This class is responsible for converting low-level SDL events (SDL_Event)
 * into engine-specific event objects and dispatching them to the EventBus.
 *
 * @note All methods are static; this class is not meant to be instantiated.
 */
class SDLEventTranslator{
public:
    using UnhandledCallback = std::function<void(const SDL_Event&)>; //Callback for unhandled events

    /**
     * @brief Translates an SDL event and dispatches it to the EventBus.
     *
     * Returns true if the event was handled, false if it was unhandled.
     *
     * @param sdlEvent The SDL event to translate.
     * @return true if the event was translated, false otherwise.
     */
    static bool TranslateAndDispatch(const SDL_Event& sdlEvent);

    /**
     * @brief Sets a callback for unhandled SDL events.
     * @param callback Function to call for unhandled events.
     */
    static void SetUnhandledCallback(const UnhandledCallback& callback);

private:
    static UnhandledCallback s_UnhandledCallback; //Callback for unhandled events

    //Dispatch methods for each event type
    static void DispatchKeyEvent(const SDL_Event& sdlEvent);
    static void DispatchMouseMotionEvent(const SDL_Event& sdlEvent);
    static void DispatchMouseButtonEvent(const SDL_Event& sdlEvent);
    static void DispatchMouseWheelEvent(const SDL_Event& sdlEvent);
    static void DispatchWindowEvent(const SDL_Event& sdlEvent);

    SDLEventTranslator() = delete; //Prevent instantiation
    ~SDLEventTranslator() = delete; //Prevent destruction
};
