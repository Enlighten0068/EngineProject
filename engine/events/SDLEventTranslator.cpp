#include "events/SDLEventTranslator.h"
#include "diagnostics/Log.h"
#include <format>

SDLEventTranslator::UnhandledCallback SDLEventTranslator::s_UnhandledCallback = nullptr;

/**
 * @brief Translates an SDL event and dispatches it to the EventBus.
 *
 * This method checks the event type and calls the appropriate dispatch function.
 * If the event type is unknown, the unhandled callback (if set) is invoked.
 *
 * @param sdlEvent The SDL event to translate.
 * @return true if the event was translated, false otherwise.
 */
bool SDLEventTranslator::TranslateAndDispatch(const SDL_Event& sdlEvent){
    switch (sdlEvent.type){
        case SDL_EVENT_QUIT:
        case SDL_EVENT_WINDOW_RESIZED:
            DispatchWindowEvent(sdlEvent);
            return true;
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
            DispatchKeyEvent(sdlEvent);
            return true;
        case SDL_EVENT_MOUSE_MOTION:
            DispatchMouseMotionEvent(sdlEvent);
            return true;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
            DispatchMouseButtonEvent(sdlEvent);
            return true;
        case SDL_EVENT_MOUSE_WHEEL:
            DispatchMouseWheelEvent(sdlEvent);
            return true;
        default:
            if (s_UnhandledCallback) {
                s_UnhandledCallback(sdlEvent);
            }
            return false;
    }
}

/**
 * @brief Sets the callback for unhandled SDL events.
 * @param callback Function to call for unhandled events.
 */
void SDLEventTranslator::SetUnhandledCallback(const UnhandledCallback& callback){
    s_UnhandledCallback = callback;
}

/**
 * @brief Translates an SDL key event into a KeyEvent and dispatches it.
 * @param sdlEvent The SDL key event.
 */
void SDLEventTranslator::DispatchKeyEvent(const SDL_Event& sdlEvent){
    KeyAction action;
    if (sdlEvent.type == SDL_EVENT_KEY_DOWN) action = (sdlEvent.key.repeat) ? KeyAction::Repeated : KeyAction::Pressed;
    else action = KeyAction::Released;

    KeyEvent event(
        sdlEvent.key.scancode,
        action,
        sdlEvent.key.mod & SDL_KMOD_CTRL,
        sdlEvent.key.mod & SDL_KMOD_SHIFT,
        sdlEvent.key.mod & SDL_KMOD_ALT
    );
    EventBus::GetInstance().Dispatch(event);
}

/**
 * @brief Translates an SDL mouse motion event into a MouseMotionEvent and dispatches it.
 * @param sdlEvent The SDL mouse motion event.
 */
void SDLEventTranslator::DispatchMouseMotionEvent(const SDL_Event& sdlEvent){
    MouseMotionEvent event(
        Vector2D(static_cast<float>(sdlEvent.motion.x), static_cast<float>(sdlEvent.motion.y)),
                           Vector2D(static_cast<float>(sdlEvent.motion.xrel), static_cast<float>(sdlEvent.motion.yrel))
    );
    EventBus::GetInstance().Dispatch(event);
}

/**
 * @brief Translates an SDL mouse button event into a MouseButtonEvent and dispatches it.
 * @param sdlEvent The SDL mouse button event.
 */
void SDLEventTranslator::DispatchMouseButtonEvent(const SDL_Event& sdlEvent){
    MouseButton button = static_cast<MouseButton>(sdlEvent.button.button);
    MouseButtonAction action = (sdlEvent.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    ? MouseButtonAction::Pressed
    : MouseButtonAction::Released;

    MouseButtonEvent event(
        button,
        action,
        Vector2D(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y))
    );
    EventBus::GetInstance().Dispatch(event);
}

/**
 * @brief Translates an SDL mouse wheel event into a MouseWheelEvent and dispatches it.
 * @param sdlEvent The SDL mouse wheel event.
 */
void SDLEventTranslator::DispatchMouseWheelEvent(const SDL_Event& sdlEvent){
    MouseWheelEvent event(
        Vector2D(static_cast<float>(sdlEvent.wheel.x), static_cast<float>(sdlEvent.wheel.y))
    );
    EventBus::GetInstance().Dispatch(event);
}

/**
 * @brief Translates an SDL window event (quit or resize) and dispatches it.
 * @param sdlEvent The SDL window event.
 */
void SDLEventTranslator::DispatchWindowEvent(const SDL_Event& sdlEvent){
    if (sdlEvent.type == SDL_EVENT_QUIT) {
        WindowCloseEvent event;
        EventBus::GetInstance().Dispatch(event);
    } else if (sdlEvent.type == SDL_EVENT_WINDOW_RESIZED) {
        WindowResizeEvent event(sdlEvent.window.data1, sdlEvent.window.data2);
        EventBus::GetInstance().Dispatch(event);
    }
}
