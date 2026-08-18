#include "core/Application.h"
#include "core/Time.h"
#include "diagnostics/Log.h"
#include "events/EventType.h"
#include "events/WindowEvent.h"
#include "events/EventDispatcher.h"
#include "graphics/VertexArray.h"
#include "graphics/VertexBuffer.h"
#include "graphics/Shader.h"
#include "graphics/Texture2D.h"
#include "renderer/Renderer.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <format>
#include <cstdint>

Application::Application() : m_Running(false) {}

Application::~Application(){
    Shutdown();
}

bool Application::Initialize(){
    //for testing purposes
    /*Log::Info("Logging...");
    Log::Warning("Warning test.");
    Log::Error("Error test.");*/

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

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    m_Running = true;

    float vertices[]{
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
    };

    uint32_t indices[]{
        0, 1, 2,
        2, 3, 0
    };

    m_Shader = std::make_unique<Shader>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
    m_VertexArray = std::make_unique<VertexArray>();
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices,6);
    m_Texture = std::make_unique<Texture2D>();

    if (!m_Texture->Load("assets/textures/test.png")) return false;

    m_VertexArray->Bind();
    m_Texture->Bind();
    m_VertexBuffer->Bind();
    m_IndexBuffer->Bind();

    const std::string vertexShaderSource = R"(
    #version 460 core

    layout(location = 0) in vec3 a_Position;
    layout(location = 1) in vec2 a_TexCoord;

    out vec2 v_TexCoord;

    void main(){
        gl_Position = vec4(a_Position,1.0);
        v_TexCoord = a_TexCoord;
    })";

    const std::string fragmentShaderSource = R"(
    #version 460 core

    in vec2 v_TexCoord;
    out vec4 FragColor;

    uniform sampler2D u_Texture;

    void main(){
        FragColor = texture(u_Texture,v_TexCoord);
    })";

    //m_Shader->Compile(vertexShaderSource, fragmentShaderSource);
    if (!m_Shader->Compile(vertexShaderSource,fragmentShaderSource)){
        Log::Error("Shader compilation failed.");
        return false;
    }

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

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
    m_Shader.reset();
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
    //Renderer::DrawQuad(*m_Shader,*m_VertexArray,*m_IndexBuffer);
    Renderer::DrawTexturedQuad(*m_Shader,*m_VertexArray,*m_IndexBuffer,*m_Texture);


    /*glClear(GL_COLOR_BUFFER_BIT);

    m_Shader->Bind();
    m_VertexArray->Bind();
    m_IndexBuffer->Bind();

    //glDrawArrays(GL_TRIANGLES,0,3);
    glDrawElements(GL_TRIANGLES,m_IndexBuffer->GetCount(),GL_UNSIGNED_INT,nullptr);*/

    m_Window.SwapBuffers();
}
