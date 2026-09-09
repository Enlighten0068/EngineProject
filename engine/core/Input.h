#pragma once

#include "math/Vector2D.h"
#include <SDL3/SDL.h>
#include <cstdint>

/**
 * @brief Static input manager for keyboard, mouse, and window state.
 *
 * Provides polling-based input detection with support for detecting
 * pressed, held, and released states. Also tracks mouse position,
 * delta movement, and window size changes.
 *
 * @note All methods are static; no instantiation is required.
 * @note The Update() method should be called once per frame to reset deltas.
 */
class Input{
public:
    /**
     * @brief Updates the input state (should be called once per frame).
     *
     * Saves the previous state of keys and mouse buttons, then fetches
     * the current keyboard state. Resets mouse delta and scroll delta.
     */
    static void Update();

    /**
     * @brief Processes an SDL event and updates the input state.
     * @param event SDL event to process.
     */
    static void ProcessEvent(const SDL_Event& event);

    //Keyboard handling
    static bool IsKeyPressed(SDL_Scancode key);
    static bool IsKeyHeld(SDL_Scancode key);
    static bool IsKeyReleased(SDL_Scancode key);

    //Mouse handling
    static Vector2D GetMousePosition();
    static Vector2D GetMouseDelta();
    static bool IsMouseButtonPressed(uint8_t button);
    static bool IsMouseButtonHeld(uint8_t button);
    static bool IsMouseButtonReleased(uint8_t button);

    //Mouse scroll
    static Vector2D GetScrollDelta();

    //Window state
    static bool WasWindowResized();
    static int GetWindowWidth();
    static int GetWindowHeight();
    static void SetWindowSize(int width, int height);

private:
    //Keyboard state arrays
    static bool s_KeyState[SDL_SCANCODE_COUNT];
    static bool s_KeyStatePrevious[SDL_SCANCODE_COUNT];

    //Mouse state and scroll
    static Vector2D s_MousePosition;
    static Vector2D s_MouseDelta;
    static bool s_MouseButtonState[5];
    static bool s_MouseButtonPrevious[5];
    static Vector2D s_ScrollDelta;

    //Window state
    static bool s_WindowResized;
    static int s_WindowWidth;
    static int s_WindowHeight;
};
