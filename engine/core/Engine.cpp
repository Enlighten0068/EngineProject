#include "core/Engine.h"
#include "core/Input.h"
#include "diagnostics/Log.h"
#include "events/EventType.h"
#include "events/WindowEvent.h"
#include "events/EventDispatcher.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <format>

Engine::Engine() : m_Running(false) {}

Engine::~Engine(){
    Shutdown();
}

/**
 * @brief Initializes SDL and creates the window.
 *
 * This method initializes the SDL video subsystem and creates a window
 * with the specified title and dimensions.
 *
 * @return true if successful, false otherwise.
 */
bool Engine::Initialize(const char* title, int width, int height, bool fullscreen){
    Log::Info("Engine: Initializing SDL3 runtime and window.");

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        Log::Error(std::format("SDL3 Initialization failed: {}", SDL_GetError()));
        return false;
    }

    if (!m_Window.Create(title, width, height, fullscreen)){
        SDL_Quit();
        return false;
    }

    m_Running = true;
    Log::Info("Engine initialized successfully.");
    return true;
}

//Deprecated, delegated to application - keep it for compatibility purposes
/*void Engine::Run() {
    while (m_Running) {
        ProcessEvents();
    }
}*/

/**
 * @brief Shuts down the engine.
 *
 * Destroys the window and quits SDL.
 */
void Engine::Shutdown() {
    m_Window.Destroy();
    SDL_Quit();
    Log::Info("Engine shutdown.");
}

/**
 * @brief Gets the SDLWindow instance.
 * @return Reference to the SDLWindow.
 */
SDLWindow& Engine::GetWindow() {
    return m_Window;
}

//Deprecated, delegated to application - keep it for compatibility purposes
/*void Engine::ProcessEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        Input::ProcessEvent(event);

        switch (event.type) {
            case SDL_EVENT_QUIT: {
                WindowCloseEvent closeEvent;
                m_Running = false;
                break;
            }
            default:
                break;
        }
    }
}*/
