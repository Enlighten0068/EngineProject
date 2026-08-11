#include "core/Application.h"
#include "core/Time.h"
#include "diagnostics/Log.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <format>

Application::Application() : m_Running(false) {}

Application::~Application(){
    Shutdown();
}

bool Application::Initialize(){
    Log::Info("Logging...");
    Log::Warning("Warning test.");
    Log::Error("Error test.");

    Log::Info("SDL3 Runtime initialization.");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        Log::Error(std::format("Initializing failed: {}", SDL_GetError()));
        return false;
    }

    const char* videoDriver = SDL_GetCurrentVideoDriver();

    if (videoDriver) Log::Info(std::format("Video Driver: {}", videoDriver));

    if (!m_Window.Create("Game Window",1920,1080)){
        SDL_Quit();
        return false;
    }

    glClearColor(0.1f,0.2f,0.4f,1.0f);
    m_Running = true;

    return true;
}

void Application::Run(){
    while (m_Running){
        Time::Update();

        ProcessEvents();
        Update();
        Render();
    }
}

void Application::Shutdown(){
    SDL_Quit();
    Log::Info("SDL3 terminated.");
}

void Application::ProcessEvents(){
    SDL_Event event;

    while (SDL_PollEvent(&event)){
        if (event.type == SDL_EVENT_QUIT){
            Log::Warning("QuitEvent received. Closing.");
            m_Running = false;
        }
    }
}

void Application::Update(){
    static float timer = 0.0f;

    timer += Time::DeltaTime();
    if(timer >= 1.0f){
        std::cout << "FPS: ~" << (1.0f / Time::DeltaTime()) << '\n';
        std::cout << "Delta: " << Time::DeltaTime() << " | Elapsed: " << Time::ElapsedTime() << '\n';
        timer = 0.0f;
    }
}

void Application::Render(){
    glClear(GL_COLOR_BUFFER_BIT);
    m_Window.SwapBuffers();
}
