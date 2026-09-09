#include "core/Input.h"
#include "demo/Demo2DScene.h"
#include "diagnostics/Log.h"
#include "resources/ResourceManager.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>

Demo2DScene::Demo2DScene(Shader& shader, VertexArray& va, IndexBuffer& ib)
: m_Shader(shader), m_VertexArray(va), m_IndexBuffer(ib){}

Demo2DScene::~Demo2DScene(){ OnExit(); }

/**
 * @brief Called when the scene becomes active.
 *
 * Sets the clear color, sets up the scene with all entities,
 * and logs the scene entry.
 */
void Demo2DScene::OnEnter(){
    Log::Info("Entering Demo2DScene");
    glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
    SetupScene();
    Log::Info("Entered Demo successfully.");
}

/**
 * @brief Called when the scene is exited.
 *
 * Releases all resources used by the scene.
 */
void Demo2DScene::OnExit(){
    m_PlayerController.reset();
    m_CameraController.reset();
    m_FpsCounter.reset();
    m_Camera.reset();
    m_World.reset();
    m_ECSScene.reset();
    Log::Info("Exiting Demo2DScene");
}

/**
 * @brief Sets up the scene with all entities.
 *
 * Creates the ECS scene, world, camera, player, and platforms.
 * Initializes the player controller, camera controller, and FPS counter.
 */
void Demo2DScene::SetupScene(){
    //Create ECS scene and world
    m_ECSScene = std::make_unique<ECSScene>(m_Shader, m_VertexArray, m_IndexBuffer);
    m_World = std::make_unique<GameWorld>(-20.0f, 20.0f, -10.0f, 10.0f);

    //Create camera with aspect ratio
    int winWidth = Input::GetWindowWidth();
    int winHeight = Input::GetWindowHeight();
    float aspect = static_cast<float>(winWidth) / static_cast<float>(winHeight);
    float height = 5.0f;
    float width = height * aspect;
    m_Camera = std::make_unique<Camera2D>(-width, width, -height, height);
    m_Camera->SetPosition(Vector3D(0.0f, 0.0f, 0.0f));

    //Load textures
    auto playerTex = ResourceManager::GetInstance().LoadTexture("assets/textures/test.png");
    auto tileTex = ResourceManager::GetInstance().LoadTexture("assets/textures/tile.png");
    if(!playerTex || !tileTex){
        Log::Error("Failed to load one or more textures.");
        return;
    }
    auto wallTex = tileTex;

    //Create player entity
    m_PlayerEntity = m_ECSScene->CreateSpriteEntity(Vector3D(0.0f, 0.0f, 0.0f),
                                                    Vector3D(5.0f, 5.0f, 1.0f),
                                                    playerTex);

    //Create platform entities
    //Left Wall
    auto left_wall = m_ECSScene->CreateSpriteEntity(Vector3D(m_World->GetMinX() + 0.25f, 0.0f, 0.0f),
                                                    Vector3D(1.0f, m_World->GetMaxY() - m_World->GetMinY(), 1.0f),
                                                    wallTex);
    m_PlatformEntities.push_back(left_wall);

    //Right Wall
    auto rgt_wall = m_ECSScene->CreateSpriteEntity(Vector3D(m_World->GetMaxX() - 0.25f, 0.0f, 0.0f),
                                                   Vector3D(1.0f, m_World->GetMaxY() - m_World->GetMinY(), 1.0f),
                                                   wallTex);
    m_PlatformEntities.push_back(rgt_wall);

    //Ceiling
    auto ceiling = m_ECSScene->CreateSpriteEntity(Vector3D(0.0f, m_World->GetMaxY() - 0.25f, 0.0f),
                                                  Vector3D(m_World->GetMaxX() - m_World->GetMinX(), 1.0f, 1.0f),
                                                  wallTex);
    m_PlatformEntities.push_back(ceiling);

    //Floor
    auto plt_floor = m_ECSScene->CreateSpriteEntity(Vector3D(0.0f, m_World->GetMinY() + 0.25f, 0.0f),
                                                    Vector3D(m_World->GetMaxX() - m_World->GetMinX(), 1.0f, 1.0f),
                                                    tileTex);
    m_PlatformEntities.push_back(plt_floor);

    //Floating platform 1
    auto plt_float1 = m_ECSScene->CreateSpriteEntity(Vector3D(-8.0f, -3.0f, 0.0f),
                                                     Vector3D(4.0f, 1.0f, 1.0f),
                                                     tileTex);
    m_PlatformEntities.push_back(plt_float1);

    //Floating platform 2
    auto plt_float2 = m_ECSScene->CreateSpriteEntity(Vector3D(8.0f, -3.0f, 0.0f),
                                                     Vector3D(4.0f, 1.0f, 1.0f),
                                                     tileTex);
    m_PlatformEntities.push_back(plt_float2);

    //Central platform
    auto plt_floatcenter = m_ECSScene->CreateSpriteEntity(Vector3D(0.0f, 2.0f, 0.0f),
                                                          Vector3D(6.0f, 1.0f, 1.0f),
                                                          tileTex);
    m_PlatformEntities.push_back(plt_floatcenter);

    //Initialize player controller
    m_PlayerController = std::make_unique<PlayerController>(m_ECSScene->GetRegistry(),
                                                            m_PlayerEntity, *m_World);
    m_PlayerController->SetSpeed(4.0f);

    //Initialize camera controller with follow mode
    m_CameraController = std::make_unique<CameraController>(*m_Camera, *m_World);
    m_CameraController->SetFollowEntity(true);
    m_CameraController->SetMinZoom(0.2f);
    m_CameraController->SetMaxZoom(1.0f);
    m_CameraController->SetZoomSpeed(1.0f);

    //Initialize FPS counter
    m_FpsCounter = std::make_unique<FpsCounter>();
}

/**
 * @brief Updates the scene logic.
 * @param deltaTime Time elapsed since the last frame.
 */
void Demo2DScene::Update(float deltaTime){
    Log::InfoThrottled("Update running...", "update_diagnostic", 2.0f);

    //Update player
    m_PlayerController->Update();
    ResolveCollisions();

    //Update camera to follow player
    auto& transform = m_ECSScene->GetRegistry().get<Components::Transform>(m_PlayerEntity);
    m_CameraController->SetTargetPosition(transform.Position);
    m_CameraController->Update(deltaTime);

    //Update FPS counter
    m_FpsCounter->Update();

    //Log player position for debugging
    Log::InfoThrottled(std::format("Player: ({:.2f}, {:.2f})", transform.Position.x, transform.Position.y),
                       "player_pos", 2.0f);
}

/**
 * @brief Renders the scene.
 *
 * Renders all entities in the ECS scene.
 */
void Demo2DScene::Render(){
    const Matrix4& view = m_Camera->GetViewMatrix();
    const Matrix4& projection = m_Camera->GetProjectionMatrix();

    m_ECSScene->Render(view, projection);
}

/**
 * @brief Resolves collisions between the player and platforms.
 *
 * Performs AABB collision detection and resolution between the player
 * and all platform entities. Updates the player's grounded state.
 */
void Demo2DScene::ResolveCollisions(){
    auto& registry = m_ECSScene->GetRegistry();
    auto& playerTransform = registry.get<Components::Transform>(m_PlayerEntity);
    Vector3D playerHalfSize = playerTransform.Scale * 0.5f;

    bool grounded = false;
    const float GROUND_MARGIN = 0.1f;

    float playerLeft = playerTransform.Position.x - playerHalfSize.x;
    float playerRight = playerTransform.Position.x + playerHalfSize.x;
    float playerBottom = playerTransform.Position.y - playerHalfSize.y;
    float playerTop = playerTransform.Position.y + playerHalfSize.y;

    for(entt::entity platform : m_PlatformEntities){
        auto& platformTransform = registry.get<Components::Transform>(platform);
        Vector3D platformHalfSize = platformTransform.Scale * 0.5f;

        float platformLeft = platformTransform.Position.x - platformHalfSize.x;
        float platformRight = platformTransform.Position.x + platformHalfSize.x;
        float platformTop = platformTransform.Position.y + platformHalfSize.y;
        float platformBottom = platformTransform.Position.y - platformHalfSize.y;

        bool overlapX = (playerRight > platformLeft && playerLeft < platformRight);
        bool overlapY = (playerTop > platformBottom && playerBottom < platformTop);

        //Check if player is landing on top of platform
        if(overlapX){
            float distanceToTop = playerBottom - platformTop;
            if(distanceToTop <= GROUND_MARGIN && distanceToTop >= -GROUND_MARGIN){
                if(m_PlayerController->GetVelocity().y <= 0.0f){
                    grounded = true;
                    playerTransform.Position.y = platformTop + playerHalfSize.y;
                    m_PlayerController->GetVelocity().y = 0.0f;
                    break;
                }
            }
        }

        //General collision resolution
        if(!grounded && overlapX && overlapY){
            Vector3D delta = playerTransform.Position - platformTransform.Position;
            Vector3D overlap = playerHalfSize + platformHalfSize - Vector3D(std::abs(delta.x), std::abs(delta.y), 0.0f);

            if(overlap.x > 0.0f && overlap.y > 0.0f){
                if(overlap.x < overlap.y){
                    //Horizontal collision
                    if(delta.x > 0.0f) playerTransform.Position.x += overlap.x;
                    else playerTransform.Position.x -= overlap.x;
                } else{
                    //Vertical collision
                    if(delta.y < 0.0f){
                        playerTransform.Position.y -= overlap.y;
                        if(m_PlayerController->GetVelocity().y > 0.0f){
                            m_PlayerController->GetVelocity().y = 0.0f;
                        }
                    }
                }
            }
        }
    }

    //Clamp to world floor
    if(!grounded){
        float worldFloor = m_World->GetMinY() + playerHalfSize.y;
        if(playerTransform.Position.y <= worldFloor + GROUND_MARGIN){
            playerTransform.Position.y = worldFloor;
            if(m_PlayerController->GetVelocity().y <= 0.0f){
                m_PlayerController->GetVelocity().y = 0.0f;
            }
            grounded = true;
        }
    }

    m_PlayerController->SetGrounded(grounded);
}
