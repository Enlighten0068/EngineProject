#pragma once

#include "platform/sdl/SDLWindow.h"

class Application{
public:
    Application();
    ~Application();

    bool Initialize();
    void Run();

    void Shutdown();

private:
    SDLWindow m_Window;
    bool m_Running;
    void ProcessEvents();
    void Update();
    void Render();
};
