#include "components/Transform.h"
#include "components/SpriteRenderer.h"
#include "core/Application.h"
#include "core/Time.h"
#include "core/Input.h"
#include "core/Engine.h"
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
#include "resources/ResourceManager.h"
#include "scene/Camera2D.h"
#include "systems/RenderSystem.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <iostream>
#include <format>
#include <cstdint>
//#include <memory>

Application::Application() = default;
Application::~Application() { Shutdown(); }


bool Application::Initialize(){
    //For testing purposes
    /*Log::Info("Logging...");
    Log::Warning("Warning test.");
    Log::Error("Error test.");*/

    Log::Info("Application: Initializing...");

    if (!m_Engine.Initialize("Game Window", 1920, 1080)){
        Log::Error("Failed to initialize Engine.");
        return false;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    float vertices[]{
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
    };

    uint32_t indices[]{ 0, 1, 2, 2, 3, 0 };

    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
    m_VertexArray = std::make_unique<VertexArray>();
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

    m_VertexArray->Bind();
    m_VertexBuffer->Bind();
    m_IndexBuffer->Bind();

    const std::string vertexShaderSource = R"(
    #version 460 core
    //#error FORCED ERROR - TEST

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

    //Uncomment to check if ShaderSource cache is updated
    //Log::Info("Vertex Shader Source:\n" + vertexShaderSource);

    const std::string fragmentShaderSource = R"(
    #version 460 core

    in vec2 v_TexCoord;
    out vec4 FragColor;

    uniform sampler2D u_Texture;

    void main(){
        FragColor = texture(u_Texture, v_TexCoord);
        //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    })";

    //Uncomment to check if ShaderSource cache is updated
    //Log::Info("Fragment Shader Source:\n" + fragmentShaderSource);

    m_Shader = std::make_unique<Shader>();
    if (!m_Shader->Compile(vertexShaderSource, fragmentShaderSource)){
        Log::Error("Shader compilation failed.");
        return false;
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    int winWidth, winHeight;
    SDL_GetWindowSize(m_Engine.GetWindow().GetNativeWindow(), &winWidth, &winHeight);
    Input::SetWindowSize(winWidth, winHeight);
    float aspect = static_cast<float>(winWidth) / static_cast<float>(winHeight);
    float height = 5.0f;
    float width = height * aspect;
    m_Camera = std::make_unique<Camera2D>(-width, width, -height, height);
    m_Camera->SetPosition(Vector3D(0.0f, 0.0f, 0.0f));

    m_Scene = std::make_unique<Scene>(*m_Shader, *m_VertexArray, *m_IndexBuffer);

    // 8. Carregar textura via ResourceManager
    auto texture = ResourceManager::GetInstance().LoadTexture("assets/textures/test.png");
    if (!texture){
        Log::Error("Failed to load texture.");
        return false;
    }

    m_PlayerEntity = m_Scene->CreateSpriteEntity(
        Vector3D(0.0f, 0.0f, 0.0f), Vector3D(5.0f, 5.0f, 1.0f),texture);

    Log::Info("Application initialized successfully.");
    return true;
}


void Application::Run(){
    while (m_Engine.IsRunning()){
        Time::Update();
        ProcessEvents();
        Update();
        Input::Update();
        Render();
        //m_Engine.SwapBuffers();
    }
}


void Application::Shutdown(){
    m_Scene.reset();
    m_Shader.reset();
    m_VertexBuffer.reset();
    m_VertexArray.reset();
    m_IndexBuffer.reset();
    m_Camera.reset();
    m_Engine.Shutdown();
    ResourceManager::GetInstance().Clear();
    Log::Info("Application shutdown.");
}


void Application::ProcessEvents(){
    SDL_Event event;

    while(SDL_PollEvent(&event)){
        Input::ProcessEvent(event);
        switch(event.type){
            case SDL_EVENT_QUIT:{
                WindowCloseEvent closeEvent;
                OnEvent(closeEvent);
                break;
            }
            case SDL_EVENT_MOUSE_WHEEL:{
                Log::Info(std::format("Scroll event raw: x={}, y={}", event.wheel.x, event.wheel.y));
                break;
            }
            default:
                break;
        }
    }
}


void Application::Update(){
    static float timer = 0.0f;
    timer += Time::DeltaTime();

    auto& transform = m_Scene->GetRegistry().get<Components::Transform>(m_PlayerEntity);

    float speed = 3.0f;
    if (Input::IsKeyHeld(SDL_SCANCODE_UP)){
        transform.Position.y += speed * Time::DeltaTime();
        Log::Info("Up key held");
    }
    if (Input::IsKeyHeld(SDL_SCANCODE_DOWN)){
        transform.Position.y -= speed * Time::DeltaTime();
        Log::Info("Down key held");
    }
    if (Input::IsKeyHeld(SDL_SCANCODE_LEFT)){
        transform.Position.x -= speed * Time::DeltaTime();
        Log::Info("Left key held");
    }
    if (Input::IsKeyHeld(SDL_SCANCODE_RIGHT)){
        transform.Position.x += speed * Time::DeltaTime();
        Log::Info("Right key held");
    }

    Vector2D scroll = Input::GetScrollDelta();
    if (scroll.y != 0.0f){
        Log::Info(std::format("Scroll delta: {}", scroll.y));
        transform.Scale.x += scroll.y * 0.5f;
        transform.Scale.y += scroll.y * 0.5f;
        Log::Info(std::format("Scale: ({}, {})", transform.Scale.x, transform.Scale.y));
    }

    transform.Rotation.z = 0.785f;

    if(timer >= 1.0f){
        Log::Info(std::format("FPS: {}", (1.0f / Time::DeltaTime())));
        Log::Info(std::format("Delta: {} | Elapsed: {}", Time::DeltaTime(), Time::ElapsedTime()));
        timer = 0.0f;
    }

    m_Scene->Update(Time::DeltaTime());
}


void Application::Render(){
    m_Camera->Update();
    const Matrix4& view = m_Camera->GetViewMatrix();
    const Matrix4& projection = m_Camera->GetProjectionMatrix();

    glClear(GL_COLOR_BUFFER_BIT);

    m_Scene->Render(view, projection);
    m_Engine.GetWindow().SwapBuffers();
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
     *   );*/

    if(event.GetEventType() == EventType::WindowClose){
        m_Engine.SetRunning(false);
        Log::Warning("Quit event received. Closing window.");
    }
}
