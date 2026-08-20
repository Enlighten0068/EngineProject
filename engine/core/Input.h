#pragma once

#include "math/Vector2D.h"

#include <SDL3/SDL.h>
#include <cstdint>

class Input {
public:
    static void Update();
    static void ProcessEvent(const SDL_Event& event);

    static bool IsKeyPressed(SDL_Scancode key);
    static bool IsKeyHeld(SDL_Scancode key);
    static bool IsKeyReleased(SDL_Scancode key);

    static Vector2D GetMousePosition();
    static Vector2D GetMouseDelta();
    static bool IsMouseButtonPressed(uint8_t button);
    static bool IsMouseButtonHeld(uint8_t button);
    static bool IsMouseButtonReleased(uint8_t button);

    static Vector2D GetScrollDelta();


    static bool WasWindowResized();
    static int  GetWindowWidth();
    static int  GetWindowHeight();


    static void SetWindowSize(int width, int height);

private:
    static bool s_KeyState[SDL_SCANCODE_COUNT];
    static bool s_KeyStatePrevious[SDL_SCANCODE_COUNT];

    static Vector2D s_MousePosition;
    static Vector2D s_MouseDelta;
    static bool s_MouseButtonState[5];
    static bool s_MouseButtonPrevious[5];
    static Vector2D s_ScrollDelta;

    static bool s_WindowResized;
    static int s_WindowWidth;
    static int s_WindowHeight;
};
