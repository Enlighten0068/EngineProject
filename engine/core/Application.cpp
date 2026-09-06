#include "audio/SoundManager.h"
#include "audio/SoundEffect.h"
#include "components/Transform.h"
#include "components/SpriteRenderer.h"
#include "core/Application.h"
#include "core/Time.h"
#include "core/Input.h"
#include "core/Engine.h"
#include "core/PlayerController.h"
#include "core/CameraController.h"
#include "core/FpsCounter.h"
#include "demo/Demo2DScene.h"
#include "diagnostics/Log.h"
#include "events/EventType.h"
#include "events/WindowEvent.h"
#include "events/SDLEventTranslator.h"
#include "graphics/VertexArray.h"
#include "graphics/VertexBuffer.h"
#include "graphics/Shader.h"
#include "graphics/Texture2D.h"
#include "math/Matrix4.h"
#include "renderer/Renderer.h"
#include "resources/ResourceManager.h"
#include "scene/Camera2D.h"
#include "scene/GameWorld.h"
#include "scene/MenuScene.h"
#include "systems/RenderSystem.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <iostream>
#include <format>
#include <cstdint>

Application::Application() = default;
Application::~Application(){ Shutdown(); }


bool Application::Initialize(){
    Log::Initialize("logs/");
    //For testing purposes
    /*Log::Info("Logging...");
    Log::Warning("Warning test.");
    Log::Error("Error test.");*/
    Log::Info("Application: Initializing...");

    //Game window initialization
    if (!m_Engine.Initialize("Game Window", 1920, 1080)){
        Log::Error("Failed to initialize Engine.");
        return false;
    }

    //Event Handling initialization
    EventBus::GetInstance().Subscribe<WindowCloseEvent>([this](Event& e) {
        Log::Info("WindowCloseEvent received, shutting down game.");
        m_Engine.SetRunning(false);
    });

    EventBus::GetInstance().Subscribe<WindowResizeEvent>([](Event& e) {
        WindowResizeEvent& resize = static_cast<WindowResizeEvent&>(e);
        Log::Info(std::format("Window resized to {}x{}", resize.GetWidth(), resize.GetHeight()));
    });


    SDLEventTranslator::SetUnhandledCallback([](const SDL_Event& event) {
        Log::Warning(std::format("Unhandled SDL event type: {}", event.type));
    });

    //GraphicsContext initialization
    m_Graphics = std::make_unique<GraphicsContext>();
    if (!m_Graphics->Initialize()) {
        Log::Error("GraphicsContext initialization failed.");
        return false;
    }

    //Camera initialization
    int winWidth, winHeight;
    SDL_GetWindowSize(m_Engine.GetWindow().GetNativeWindow(), &winWidth, &winHeight);
    Input::SetWindowSize(winWidth, winHeight);
    float aspect = static_cast<float>(winWidth) / static_cast<float>(winHeight);
    float height = 5.0f;
    float width = height * aspect;
    m_Camera = std::make_unique<Camera2D>(-width, width, -height, height);
    m_Camera->SetPosition(Vector3D(0.0f, 0.0f, 0.0f));

    //Scene initialization
    m_Scene = std::make_unique<ECSScene>(m_Graphics->GetShader(),
                                      m_Graphics->GetVertexArray(),
                                      m_Graphics->GetIndexBuffer()
    );

    m_World = std::make_unique<GameWorld>(-20.0f, 20.0f, -10.0f, 10.0f);

    //Texture Loading & entity testing - to be adjusted
    auto texture = ResourceManager::GetInstance().LoadTexture("assets/textures/test.png");
    if (!texture){
        Log::Error("Failed to load texture.");
        return false;
    }
    m_PlayerEntity = m_Scene->CreateSpriteEntity(
        Vector3D(0.0f, 0.0f, 0.0f), Vector3D(5.0f, 5.0f, 1.0f),texture);

    SetupControllers();

    //Audio initialization
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
        Log::Error(std::format("SDL Init failed: {}", SDL_GetError()));
        return false;
    }

    //Check if any gamepad is available
    if (!GamepadManager::GetInstance().Initialize()) {
        Log::Warning("Gamepad initialization failed. Continuing without gamepad support.");
    }

    //FPS Counter
    m_FpsCounter = std::make_unique<FpsCounter>();

    Log::Info("Application initialized successfully.");

    auto initialScene = std::make_unique<Demo2DScene>(m_Graphics->GetShader(),
                                                      m_Graphics->GetVertexArray(),
                                                      m_Graphics->GetIndexBuffer());

    auto menuScene = std::make_unique<MenuScene>(m_SceneManager);
    m_SceneManager.SetScene(std::move(menuScene));
    return true;
}


void Application::Run(){
    while (m_Engine.IsRunning()){
        Time::Update();
        ProcessEvents();
        Update();
        Input::Update();
        Render();
    }
}


void Application::Shutdown(){
    Log::Info("Shutting down Application...");
    Log::Shutdown();

    //m_CameraController.reset();
    m_PlayerController.reset();
    m_FpsCounter.reset();
    m_World.reset();
    m_Scene.reset();
    m_Camera.reset();


    if (m_Graphics){
        m_Graphics->Shutdown();
        m_Graphics.reset();
    }

    EventBus::GetInstance().Clear();
    Log::Info("EventBus cleared.");
    SoundEffect::CloseAudioDevice();
    GamepadManager::GetInstance().Shutdown();

    ResourceManager::GetInstance().Clear();
    m_Engine.Shutdown();
    Log::Info("Application shut down.");
}


void Application::ProcessEvents(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        Input::ProcessEvent(event);
        GamepadManager::GetInstance().ProcessEvent(event);
        SDLEventTranslator::TranslateAndDispatch(event);
    }
}


void Application::Update(){
    float dt = Time::DeltaTime();

    GamepadManager::GetInstance().Update();
    m_SceneManager.Update(dt);
}


void Application::Render(){
    glClear(GL_COLOR_BUFFER_BIT);
    m_SceneManager.Render();
    m_Engine.GetWindow().SwapBuffers();
}


void Application::SetupControllers() {

    //For Fixed Camera Demo
    m_PlayerController = std::make_unique<PlayerController>(m_Scene->GetRegistry(), m_PlayerEntity, *m_World);
    m_PlayerController->SetSpeed(3.0f);

    //For 2D demo with follow Camera
    /*m_CameraController = std::make_unique<CameraController>(*m_Camera);
    m_CameraController->SetSpeed(5.0f);
    m_CameraController->SetFollowEntity(true);
    m_CameraController->SetZoomSpeed(1.0f);*/

    Log::Info("Controllers initialized.");
}
