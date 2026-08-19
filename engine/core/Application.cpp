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
#include "math/Matrix4.h"
#include "renderer/Renderer.h"
#include "scene/Camera2D.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <format>
#include <cstdint>

Application::Application() : m_Running(false), m_Camera(nullptr) {}

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

    if(!m_Window.Create("Game Window", 1920, 1080)){
        SDL_Quit();
        return false;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
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

    uniform mat4 u_Model;
    uniform mat4 u_View;
    uniform mat4 u_Projection;

    out vec2 v_TexCoord;

    void main(){
        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
        v_TexCoord = a_TexCoord;
    })";

    const std::string fragmentShaderSource = R"(
    #version 460 core

    in vec2 v_TexCoord;
    out vec4 FragColor;

    uniform sampler2D u_Texture;

    void main(){
        FragColor = texture(u_Texture, v_TexCoord);
    })";

    //Log::Info("Vertex Shader Source:\n" + vertexShaderSource);
    //Log::Info("Fragment Shader Source:\n" + fragmentShaderSource);

    //m_Shader->Compile(vertexShaderSource, fragmentShaderSource);
    if (!m_Shader->Compile(vertexShaderSource, fragmentShaderSource)){
        Log::Error("Shader compilation failed.");
        return false;
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    /* Identity Matrix testing
    Matrix4 identity = Matrix4::Identity();
    const float* data = identity.Data();

    Log::Info(std::format(
        "{} {} {} {}\n"
        "{} {} {} {}\n"
        "{} {} {} {}\n"
        "{} {} {} {}",

        data[0], data[1], data[2], data[3],
        data[4], data[5], data[6], data[7],
        data[8], data[9], data[10], data[11],
        data[12], data[13], data[14], data[15]));*/

    /* Translation Matrix testing
    Matrix4 translation = Matrix4::Translation(Vector3D(2.0f, 3.0f, 4.0f));
    const float* data = translation.Data();

    Log::Info(std::format(
        "{} {} {} {}\n"
        "{} {} {} {}\n"
        "{} {} {} {}\n"
        "{} {} {} {}",

        data[0], data[1], data[2], data[3],
        data[4], data[5], data[6], data[7],
        data[8], data[9], data[10], data[11],
        data[12], data[13], data[14], data[15]));*/

    /* Scaling Matrix testing
    Matrix4 scale = Matrix4::Scale(Vector3D(2.0f,3.0f,4.0f));
    const float* data = scale.Data();

    Log::Info(std::format(
        "{} {} {} {}\n"
        "{} {} {} {}\n"
        "{} {} {} {}\n"
        "{} {} {} {}",

        data[0], data[1], data[2], data[3],
        data[4], data[5], data[6], data[7],
        data[8], data[9], data[10], data[11],
        data[12], data[13], data[14], data[15]));*/

    /* Matrix Multiplication testing
    Matrix4 identityA = Matrix4::Identity();
    Matrix4 identityB = Matrix4::Identity();

    Matrix4 result = identityA * identityB;
    const float* data = result.Data();

    Log::Info(std::format(
        "{} {} {} {}\n"
        "{} {} {} {}\n"
        "{} {} {} {}\n"
        "{} {} {} {}",

        data[0], data[1], data[2], data[3],
        data[4], data[5], data[6], data[7],
        data[8], data[9], data[10], data[11],
        data[12], data[13], data[14], data[15]));*/

    int winWidth, winHeight;
    SDL_GetWindowSize(m_Window.GetNativeWindow(), &winWidth, &winHeight);
    float aspect = static_cast<float>(winWidth) / static_cast<float>(winHeight);
    float height = 5.0f;
    float width = height * aspect;
    m_Camera = std::make_unique<Camera2D>(-width, width, -height, height);
    m_Camera->SetPosition(Vector3D(0.0f, 0.0f, 0.0f));


    m_TestTransform.Position = Vector3D(0.0f, 0.0f, 0.0f);
    m_TestTransform.Scale = Vector3D(5.0f, 5.0f, 1.0f);

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

    //Event Dispatcher test
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
    m_TestTransform.Position.x = std::sin(timer) * 2.0f;
    m_TestTransform.Rotation.z = timer * 0.5f;
    if(timer >= 1.0f){
        Log::Info(std::format("FPS: {}", (1.0f / Time::DeltaTime())));
        //std::cout << "FPS: ~" << (1.0f / Time::DeltaTime()) << '\n';
        Log::Info(std::format("Delta: {} | Elapsed: {}", Time::DeltaTime(), Time::ElapsedTime()));
        //std::cout << "Delta: " << Time::DeltaTime() << " | Elapsed: " << Time::ElapsedTime() << '\n';
        timer = 0.0f;
    }
}

void Application::Render(){
    m_Camera->Update();

    Matrix4 model = Matrix4::Translation(m_TestTransform.Position);
    model = model * Matrix4::RotationZ(m_TestTransform.Rotation.z);
    model = model * Matrix4::Scale(m_TestTransform.Scale);

    const Matrix4& view = m_Camera->GetViewMatrix();
    const Matrix4& projection = m_Camera->GetProjectionMatrix();

    glClear(GL_COLOR_BUFFER_BIT);

    Renderer::DrawTexturedQuad(*m_Shader, *m_VertexArray, *m_IndexBuffer, *m_Texture, model, view, projection);

    m_Window.SwapBuffers();
}
