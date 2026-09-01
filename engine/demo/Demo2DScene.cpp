#include "core/Input.h"
#include "demo/Demo2DScene.h"
#include "diagnostics/Log.h"
#include "resources/ResourceManager.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>

Demo2DScene::Demo2DScene(Shader& shader, VertexArray& va, IndexBuffer& ib)
: m_Shader(shader), m_VertexArray(va), m_IndexBuffer(ib){}

Demo2DScene::~Demo2DScene() { OnExit(); }

void Demo2DScene::OnEnter(){
    Log::Info("Entering Demo2DScene");
    glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
    SetupScene();
    Log::Info("Entered Demo successfully.");
}

void Demo2DScene::OnExit(){
    m_PlayerController.reset();
    m_CameraController.reset();
    m_FpsCounter.reset();
    m_Camera.reset();
    m_World.reset();
    m_ECSScene.reset();
    Log::Info("Exiting Demo2DScene");
}

void Demo2DScene::SetupScene(){
    m_ECSScene = std::make_unique<ECSScene>(m_Shader, m_VertexArray, m_IndexBuffer);

    m_World = std::make_unique<GameWorld>(-20.0f, 20.0f, -10.0f, 10.0f);

    int winWidth = Input::GetWindowWidth();
    int winHeight = Input::GetWindowHeight();
    float aspect = static_cast<float>(winWidth) / static_cast<float>(winHeight);
    float height = 5.0f;
    float width = height * aspect;
    m_Camera = std::make_unique<Camera2D>(-width, width, -height, height);
    m_Camera->SetPosition(Vector3D(0.0f, 0.0f, 0.0f));

    auto playerTex = ResourceManager::GetInstance().LoadTexture("assets/textures/test.png");
    auto tileTex = ResourceManager::GetInstance().LoadTexture("assets/textures/tile.png");
    if (!playerTex || !tileTex){
        Log::Error("Failed to load one or more textures.");
        return;
    }

    m_PlayerEntity = m_ECSScene->CreateSpriteEntity(Vector3D(0.0f, 0.0f, 0.0f),
                                                    Vector3D(5.0f, 5.0f, 1.0f),
                                                    playerTex);

    //Floor
    m_ECSScene->CreateSpriteEntity(Vector3D(0.0f, -9.0f, 0.0f),
                                   Vector3D(40.0f, 1.0f, 1.0f),
                                   tileTex);
    //Floating Platform
    m_ECSScene->CreateSpriteEntity(Vector3D(-8.0f, -3.0f, 0.0f),
                                   Vector3D(4.0f, 1.0f, 1.0f),
                                   tileTex);
    m_ECSScene->CreateSpriteEntity(
        Vector3D(8.0f, -3.0f, 0.0f),
                                   Vector3D(4.0f, 1.0f, 1.0f),
                                   tileTex
    );
    //Central platform
    m_ECSScene->CreateSpriteEntity(Vector3D(0.0f, 2.0f, 0.0f),
                                   Vector3D(6.0f, 1.0f, 1.0f),
                                   tileTex);

    m_PlayerController = std::make_unique<PlayerController>(m_ECSScene->GetRegistry(),
                                                            m_PlayerEntity,*m_World);
    m_PlayerController->SetSpeed(4.0f);

    m_CameraController = std::make_unique<CameraController>(*m_Camera, *m_World);
    m_CameraController->SetFollowEntity(true);
    m_CameraController->SetZoomSpeed(1.0f);

    m_FpsCounter = std::make_unique<FpsCounter>();
}

void Demo2DScene::Update(float deltaTime){
    Log::InfoThrottled("Update running...", "update_diagnostic", 2.0f);
    m_PlayerController->Update();

    auto& transform = m_ECSScene->GetRegistry().get<Components::Transform>(m_PlayerEntity);
    m_CameraController->SetTargetPosition(transform.Position);
    m_CameraController->Update(deltaTime);

    m_FpsCounter->Update();
    m_ECSScene->Update(deltaTime);
    Log::InfoThrottled(std::format("Player: ({:.2f}, {:.2f})", transform.Position.x, transform.Position.y),
                       "player_pos", 2.0f);
}

void Demo2DScene::Render(){
    const Matrix4& view = m_Camera->GetViewMatrix();
    const Matrix4& projection = m_Camera->GetProjectionMatrix();

    m_ECSScene->Render(view, projection);
}
