#pragma once

#include "platform/sdl/SDLWindow.h"
#include <memory>

class Engine{
public:
    Engine();
    ~Engine();

    bool Initialize(const char* title, int width, int height);
    void Run();
    void Shutdown();

    SDLWindow& GetWindow();
    bool IsRunning() const { return m_Running; }
    void SetRunning(bool running) { m_Running = running; }

private:
    SDLWindow m_Window;
    bool m_Running;
    void ProcessEvents();
};
