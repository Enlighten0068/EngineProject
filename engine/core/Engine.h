#pragma once

#include "platform/sdl/SDLWindow.h"
#include <memory>

/**
 * @brief Core engine class responsible for window creation and SDL lifecycle.
 *
 * The Engine class abstracts the low-level window management and SDL initialization.
 * It owns the SDLWindow and manages the main loop's running state.
 *
 * @see SDLWindow, Application
 */
class Engine{
public:
    Engine();
    ~Engine();

    /**
     * @brief Initializes SDL and creates the game window.
     * @param title Window title.
     * @param width Window width in pixels.
     * @param height Window height in pixels.
     * @param fullscreen Whether to start in fullscreen mode.
     * @return true if initialization succeeded, false otherwise.
     */
    bool Initialize(const char* title, int width, int height, bool fullscreen = false);

    /**
     * @brief Shuts down the engine and destroys the game window.
     */
    void Shutdown();

    /**
     * @brief Gets the SDLWindow instance.
     * @return Reference to the SDLWindow.
     */
    SDLWindow& GetWindow();

    /**
     * @brief Checks if the engine is still running.
     * @return true if running, false otherwise.
     */

    /** @brief Toggles fullscreen mode on the window. */
    bool ToggleFullscreen(){ return m_Window.ToggleFullscreen(); }

    /** @brief Checks if the window is fullscreen. */
    bool IsFullscreen() const{ return m_Window.IsFullscreen(); }

    bool IsRunning() const{ return m_Running; }

    /**
     * @brief Sets the running state of the engine.
     * @param running New running state.
     */
    void SetRunning(bool running){ m_Running = running; }

private:
    SDLWindow m_Window; //SDL window wrapper
    bool m_Running; //Flag indicating if engine is running
};
