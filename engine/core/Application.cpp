#include "core/Application.h"
#include "core/Time.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>

Application::Application() : m_Running(false) {}

Application::~Application(){
    Shutdown();
}

bool Application::Initialize(){
    std::cout << "SDL3 Runtime initialization.\n";

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Initializing failed: " << SDL_GetError() << '\n';
        return false;
    }

    const char* videoDriver = SDL_GetCurrentVideoDriver();

    if (videoDriver) std::cout << "Video Driver: " << videoDriver << '\n';

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
    std::cout << "SDL3 terminated.\n";
}

void Application::ProcessEvents(){
    SDL_Event event;

    while (SDL_PollEvent(&event)){
        if (event.type == SDL_EVENT_QUIT){
            std::cout << "QuitEvent received. Closing.\n";
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
