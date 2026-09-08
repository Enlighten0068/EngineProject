#pragma once

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/WindowEvent.h"
#include "events/EventBus.h"
#include <SDL3/SDL.h>

/**
 * @brief Translates SDL events into engine events and dispatches them via EventBus.
 *
 * This class is responsible for converting low-level SDL events (SDL_Event)
 * into engine-specific event objects and dispatching them to the EventBus.
 *
 */
class SDLEventTranslator {
public:
    /**
     * @brief Translates an SDL event and dispatches it to the EventBus.
     *
     * Returns true if the event was handled,
     * false if it was unhandled.
     *
     * @param sdlEvent The SDL event to translate.
     * @return true if the event was translated, false otherwise.
     */
    static bool TranslateAndDispatch(const SDL_Event& sdlEvent);

    using UnhandledCallback = std::function<void(const SDL_Event&)>;

    /**
     * @brief Sets a callback for unhandled SDL events.
     * @param callback Function to call for unhandled events.
     */
    static void SetUnhandledCallback(const UnhandledCallback& callback);

private:
    static UnhandledCallback s_UnhandledCallback;

    //Dispatch methods
    static void DispatchKeyEvent(const SDL_Event& sdlEvent);
    static void DispatchMouseMotionEvent(const SDL_Event& sdlEvent);
    static void DispatchMouseButtonEvent(const SDL_Event& sdlEvent);
    static void DispatchMouseWheelEvent(const SDL_Event& sdlEvent);
    static void DispatchWindowEvent(const SDL_Event& sdlEvent);

    SDLEventTranslator() = delete;
    ~SDLEventTranslator() = delete;
};
