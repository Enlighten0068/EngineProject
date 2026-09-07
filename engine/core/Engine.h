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
     * @return true if initialization succeeded, false otherwise.
     */
    bool Initialize(const char* title, int width, int height);

    /**
     * @brief Runs the engine's internal event loop.
     * @deprecated This method is kept for compatibility; the main loop is in Application.
     */
    //void Run();

    /**
     * @brief Shuts down the engine and destroys the game window.
     */
    void Shutdown();
    /**
     * @brief Get the SDLWindow instance.
     * @return Reference to the SDLWindow.
     */
    SDLWindow& GetWindow();

    /**
     * @brief Check if the engine is still running.
     * @return true if running, false otherwise.
     */
    bool IsRunning() const{ return m_Running; }

    /**
     * @brief Set the running state of the engine.
     * @param running New running state.
     */
    void SetRunning(bool running){ m_Running = running; }

private:
    SDLWindow m_Window;
    bool m_Running; //Flag indicating if engine is running

    /**
     * @brief Internal event processor for SDL events.
     * @deprecated Event processing is now handled by Application. Kept for compatibility purposes.
     */
    //void ProcessEvents();
};
