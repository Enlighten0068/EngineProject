#include "core/Input.h"

#include <cstring>

bool Input::s_KeyState[SDL_SCANCODE_COUNT] = {};
bool Input::s_KeyStatePrevious[SDL_SCANCODE_COUNT] = {};
Vector2D Input::s_MousePosition(0.0f, 0.0f);
Vector2D Input::s_MouseDelta(0.0f, 0.0f);
bool Input::s_MouseButtonState[5] = {};
bool Input::s_MouseButtonPrevious[5] = {};
Vector2D Input::s_ScrollDelta(0.0f, 0.0f);
bool Input::s_WindowResized = false;
int Input::s_WindowWidth = 0;
int Input::s_WindowHeight = 0;

void Input::Update() {
    std::memcpy(s_KeyStatePrevious, s_KeyState, sizeof(s_KeyState));
    std::memcpy(s_MouseButtonPrevious, s_MouseButtonState, sizeof(s_MouseButtonState));

    const bool* keyboardState = SDL_GetKeyboardState(nullptr);
    for (int i = 0; i < SDL_SCANCODE_COUNT; ++i) {
        s_KeyState[i] = keyboardState[i] != 0;
    }

    s_MouseDelta = Vector2D(0.0f, 0.0f);
    s_ScrollDelta = Vector2D(0.0f, 0.0f);
    s_WindowResized = false;
}

void Input::ProcessEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN:
            if (!event.key.repeat) {
                s_KeyState[event.key.scancode] = true;
            }
            break;
        case SDL_EVENT_KEY_UP:
            s_KeyState[event.key.scancode] = false;
            break;

        case SDL_EVENT_MOUSE_MOTION:
            s_MousePosition.x = static_cast<float>(event.motion.x);
            s_MousePosition.y = static_cast<float>(event.motion.y);
            s_MouseDelta.x += static_cast<float>(event.motion.xrel);
            s_MouseDelta.y += static_cast<float>(event.motion.yrel);
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button >= 1 && event.button.button <= 5) {
                s_MouseButtonState[event.button.button - 1] = true;
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (event.button.button >= 1 && event.button.button <= 5) {
                s_MouseButtonState[event.button.button - 1] = false;
            }
            break;

        case SDL_EVENT_MOUSE_WHEEL:
            s_ScrollDelta.x += static_cast<float>(event.wheel.x);
            s_ScrollDelta.y += static_cast<float>(event.wheel.y);
            break;

        case SDL_EVENT_WINDOW_RESIZED:
            s_WindowResized = true;
            s_WindowWidth = event.window.data1;
            s_WindowHeight = event.window.data2;
            break;

        default:
            break;
    }
}

bool Input::IsKeyPressed(SDL_Scancode key) {
    if (key < 0 || key >= SDL_SCANCODE_COUNT) return false;
    return s_KeyState[key] && !s_KeyStatePrevious[key];
}

bool Input::IsKeyHeld(SDL_Scancode key) {
    if (key < 0 || key >= SDL_SCANCODE_COUNT) return false;
    return s_KeyState[key];
}

bool Input::IsKeyReleased(SDL_Scancode key) {
    if (key < 0 || key >= SDL_SCANCODE_COUNT) return false;
    return !s_KeyState[key] && s_KeyStatePrevious[key];
}

Vector2D Input::GetMousePosition() {
    return s_MousePosition;
}

Vector2D Input::GetMouseDelta() {
    return s_MouseDelta;
}

bool Input::IsMouseButtonPressed(uint8_t button) {
    if (button < 1 || button > 5) return false;
    return s_MouseButtonState[button - 1] && !s_MouseButtonPrevious[button - 1];
}

bool Input::IsMouseButtonHeld(uint8_t button) {
    if (button < 1 || button > 5) return false;
    return s_MouseButtonState[button - 1];
}

bool Input::IsMouseButtonReleased(uint8_t button) {
    if (button < 1 || button > 5) return false;
    return !s_MouseButtonState[button - 1] && s_MouseButtonPrevious[button - 1];
}

Vector2D Input::GetScrollDelta() {
    return s_ScrollDelta;
}

// Janela
bool Input::WasWindowResized() {
    return s_WindowResized;
}

int Input::GetWindowWidth() {
    return s_WindowWidth;
}

int Input::GetWindowHeight() {
    return s_WindowHeight;
}

void Input::SetWindowSize(int width, int height) {
    s_WindowWidth = width;
    s_WindowHeight = height;
}
