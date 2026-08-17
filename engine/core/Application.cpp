#include "core/Application.h"
#include "core/Time.h"
#include "diagnostics/Log.h"
#include "events/EventType.h"
#include "events/WindowEvent.h"
#include "events/EventDispatcher.h"
#include "graphics/VertexArray.h"
#include "graphics/VertexBuffer.h"
#include "graphics/Shader.h"
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

    float vertices[]{
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
    };

    m_Shader = std::make_unique<Shader>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
    m_VertexArray = std::make_unique<VertexArray>();

    m_VertexArray->Bind();
    m_VertexBuffer->Bind();

    const std::string vertexShaderSource = R"(
    #version 460 core

    layout(location = 0) in vec3 a_Position;

    void main(){
        gl_Position = vec4(a_Position,1.0);
    })";

    const std::string fragmentShaderSource = R"(
    #version 460 core

    out vec4 FragColor;

    void main(){
        FragColor = vec4(1.0,0.5,0.2,1.0);
    })";

    m_Shader->Compile(vertexShaderSource, fragmentShaderSource);
    if (!m_Shader->Compile(vertexShaderSource,fragmentShaderSource)){
        Log::Error("Shader compilation failed.");
        return false;
    }

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),nullptr);

    glEnableVertexAttribArray(0);

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
    m_VertexBuffer.reset();
    m_VertexArray.reset();
    SDL_Quit();
    Log::Info("SDL3 terminated.");
}

void Application::OnEvent(Event& event){
    EventDispatcher dispatcher(event);

    //Event Dispatcher
    /*dispatcher.Dispatch<WindowCloseEvent>(
    *    [&](WindowCloseEvent& event){
    *        Log::Warning("Quit event received. Closing window.");
    *        m_Running = false;
    *        return true;
    *    }
    );*/

    if(event.GetEventType() == EventType::WindowClose){
        m_Running = false;
        Log::Warning("Quit event received. Closing window.");
    }
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
        Log::Info(std::format("FPS: {}", (1.0f / Time::DeltaTime())));
        //std::cout << "FPS: ~" << (1.0f / Time::DeltaTime()) << '\n';
        Log::Info(std::format("Delta: {} | Elapsed: {}", Time::DeltaTime(), Time::ElapsedTime()));
        //std::cout << "Delta: " << Time::DeltaTime() << " | Elapsed: " << Time::ElapsedTime() << '\n';
        timer = 0.0f;
    }
}

void Application::Render(){
    glClear(GL_COLOR_BUFFER_BIT);

    m_Shader->Bind();
    m_VertexArray->Bind();

    glDrawArrays(GL_TRIANGLES,0,3);

    m_Window.SwapBuffers();
}
