#pragma once

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/WindowEvent.h"
#include "events/EventBus.h"
#include <SDL3/SDL.h>

class SDLEventTranslator {
public:
    static bool TranslateAndDispatch(const SDL_Event& sdlEvent);

    using UnhandledCallback = std::function<void(const SDL_Event&)>;
    static void SetUnhandledCallback(const UnhandledCallback& callback);

private:
    static UnhandledCallback s_UnhandledCallback;

    static void DispatchKeyEvent(const SDL_Event& sdlEvent);
    static void DispatchMouseMotionEvent(const SDL_Event& sdlEvent);
    static void DispatchMouseButtonEvent(const SDL_Event& sdlEvent);
    static void DispatchMouseWheelEvent(const SDL_Event& sdlEvent);
    static void DispatchWindowEvent(const SDL_Event& sdlEvent);

    SDLEventTranslator() = delete;
    ~SDLEventTranslator() = delete;
};
