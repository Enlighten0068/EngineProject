#include "core/Application.h"
#include "core/Time.h"
#include "diagnostics/Log.h"
#include "events/EventType.h"
#include "events/WindowEvent.h"
#include "events/EventDispatcher.h"
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

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        Log::Error(std::format("Initializing failed: {}", SDL_GetError()));
        return false;
    }

    const char* videoDriver = SDL_GetCurrentVideoDriver();

    if(videoDriver) Log::Info(std::format("Video Driver: {}", videoDriver));

    if(!m_Window.Create("Game Window",1920,1080)){
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

void Application::OnEvent(Event& event){
    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<WindowCloseEvent>(WindowCloseEvent& event{
        Log::Warning("Quit event received. Closing window.")
        m_Running = false;
        return true;
    });

    //if(event.GetEventType() == EventType::WindowClose){
    //    m_Running = false;
    //    Log::Warning("Quit event received. Closing window.");
    //}
}

void Application::ProcessEvents(){
    SDL_Event event;

    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_EVENT_QUIT:{
                WindowCloseEvent closeEvent;
                OnEvent(closeEvent);
                break;
            }
        }
    }
}

void Application::Update(){
    static float timer = 0.0f;

    timer += Time::DeltaTime();
    if(timer >= 1.0f){
        Log::Info(std::format("FPS: ~{}", (1.0f / Time::DeltaTime())));
        //std::cout << "FPS: ~" << (1.0f / Time::DeltaTime()) << '\n';
        Log::Info(std::format("Delta: {} | Elapsed: {}", Time::DeltaTime(), Time::ElapsedTime()));
        //std::cout << "Delta: " << Time::DeltaTime() << " | Elapsed: " << Time::ElapsedTime() << '\n';
        timer = 0.0f;
    }
}

void Application::Render(){
    glClear(GL_COLOR_BUFFER_BIT);
    m_Window.SwapBuffers();
}
