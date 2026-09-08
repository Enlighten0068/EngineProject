#include "components/TileScale.h"
#include "core/Input.h"
#include "demo/Demo2DFixedScene.h"
#include "diagnostics/Log.h"
#include "resources/ResourceManager.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <format>

Demo2DFixedScene::Demo2DFixedScene(Shader& shader, VertexArray& va, IndexBuffer& ib,
                                   Camera2D& camera, Shader& lineShader)
: m_Shader(shader), m_VertexArray(va), m_IndexBuffer(ib),
  m_Camera(camera), m_LineShader(lineShader){}

Demo2DFixedScene::~Demo2DFixedScene() { OnExit(); }

void Demo2DFixedScene::OnEnter(){
    Log::Info("Entering Demo2DFixedScene with fixed camera.");
    glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
    SetupScene();
    Log::Info("Entered Demo(fixed) successfully.");
}

void Demo2DFixedScene::OnExit(){
    m_PlayerController.reset();
    m_FpsCounter.reset();
    m_World.reset();
    m_ECSScene.reset();
    Log::Info("Exiting Demo2DFixedScene");
}

void Demo2DFixedScene::SetupScene(){
    m_ECSScene = std::make_unique<ECSScene>(m_Shader, m_VertexArray, m_IndexBuffer);
    m_World = std::make_unique<GameWorld>(-20.0f, 20.0f, -10.0f, 10.0f);

    m_Camera.SetProjection(m_World->GetMinX(), m_World->GetMaxX(),
                           m_World->GetMinY(), m_World->GetMaxY());

    float centerX = (m_World->GetMinX() + m_World->GetMaxX()) * 0.5f;
    float centerY = (m_World->GetMinY() + m_World->GetMaxY()) * 0.5f;
    m_Camera.SetPosition(Vector3D(centerX, centerY, 0.0f));
    m_Camera.Update();

    Log::Info("Camera projection set to world bounds.");

    //Textures
    auto playerTex = ResourceManager::GetInstance().LoadTexture("assets/textures/test.png");
    auto tileTex = ResourceManager::GetInstance().LoadTexture("assets/textures/tile.png");
    if (!playerTex || !tileTex){
        Log::Error("Failed to load one or more textures.");
        return;
    }
    auto wallTex = tileTex;

    //Player
    m_PlayerEntity = m_ECSScene->CreateSpriteEntity(Vector3D(0.0f, 0.0f, 0.0f),
                                                    Vector3D(3.0f, 3.0f, 1.0f),
                                                    playerTex);

    //Below are platform entities
    //CreateSpriteEntity: (Position(x,y,z), Scale(x,y,z), texture)

    //Left Wall
    auto left_wall = m_ECSScene->CreateSpriteEntity(Vector3D(m_World->GetMinX() + 0.50f, 0.0f, 0.0f),
                                                    Vector3D(1.0f, m_World->GetMaxY() - m_World->GetMinY(), 1.0f),
                                                    wallTex);
    m_PlatformEntities.push_back(left_wall);
    m_ECSScene->GetRegistry().emplace<TileScale>(left_wall, 1.0f, 20.0f);

    //Right Wall
    auto rgt_wall = m_ECSScene->CreateSpriteEntity(Vector3D(m_World->GetMaxX() - 0.50f, 0.0f, 0.0f),
                                                   Vector3D(1.0f, m_World->GetMaxY() - m_World->GetMinY(), 1.0f),
                                                   wallTex);
    m_PlatformEntities.push_back(rgt_wall);
    m_ECSScene->GetRegistry().emplace<TileScale>(rgt_wall, 1.0f, 20.0f);

    //Ceiling
    auto ceiling = m_ECSScene->CreateSpriteEntity(Vector3D(0.0f, m_World->GetMaxY() - 0.50f, 0.0f),
                                                  Vector3D(m_World->GetMaxX() - m_World->GetMinX(), 1.0f, 1.0f),
                                                  wallTex);
    m_PlatformEntities.push_back(ceiling);
    m_ECSScene->GetRegistry().emplace<TileScale>(ceiling, 39.0f, 1.0f);

    //Floor
    auto plt_floor = m_ECSScene->CreateSpriteEntity(Vector3D(0.0f, m_World->GetMinY() + 0.50f, 0.0f),
                                                    Vector3D(m_World->GetMaxX() - m_World->GetMinX(), 1.0f, 1.0f),
                                                    tileTex);
    m_PlatformEntities.push_back(plt_floor);
    m_ECSScene->GetRegistry().emplace<TileScale>(plt_floor, 39.0f, 1.0f);

    //Floating Platform
    auto plt_float1 = m_ECSScene->CreateSpriteEntity(Vector3D(-8.0f, -3.0f, 0.0f),
                                                     Vector3D(4.0f, 1.0f, 1.0f),
                                                     tileTex);
    m_PlatformEntities.push_back(plt_float1);
    m_ECSScene->GetRegistry().emplace<TileScale>(plt_float1, 4.0f, 1.0f);

    auto plt_float2 = m_ECSScene->CreateSpriteEntity(Vector3D(8.0f, -3.0f, 0.0f),
                                                     Vector3D(4.0f, 1.0f, 1.0f),
                                                     tileTex);
    m_PlatformEntities.push_back(plt_float2);
    m_ECSScene->GetRegistry().emplace<TileScale>(plt_float2, 4.0f, 1.0f);

    //Central platform
    auto plt_floatcenter = m_ECSScene->CreateSpriteEntity(Vector3D(0.0f, 2.0f, 0.0f),
                                                          Vector3D(8.0f, 1.0f, 1.0f),
                                                          tileTex);
    m_PlatformEntities.push_back(plt_floatcenter);
    m_ECSScene->GetRegistry().emplace<TileScale>(plt_floatcenter, 8.0f, 1.0f);

    SpawnEnemies();

    m_PlayerController = std::make_unique<PlayerController>(m_ECSScene->GetRegistry(),
                                                            m_PlayerEntity,*m_World);
    m_PlayerController->SetSpeed(4.0f);

    m_FpsCounter = std::make_unique<FpsCounter>();
}

void Demo2DFixedScene::Update(float deltaTime){
    Log::InfoThrottled("Update running...", "update_diagnostic", 2.0f);
    m_PlayerController->Update();
    ResolveCollisions();

    auto& transform = m_ECSScene->GetRegistry().get<Components::Transform>(m_PlayerEntity);

    EnemySystem::Update(m_ECSScene->GetRegistry(), deltaTime,
                        m_PlatformEntities, m_World->GetMinY());

    auto& registry = m_ECSScene->GetRegistry();
    auto& playerTransform = registry.get<Components::Transform>(m_PlayerEntity);
    Vector3D playerSize = playerTransform.Scale;

    auto enemyView = m_ECSScene->GetRegistry().view<Components::Transform, Enemy>();
    for (auto [entity, transform, enemy] : enemyView.each()){
        if (!enemy.IsActive) continue;
        if (EnemySystem::CheckCollision(playerTransform.Position, playerSize, transform.Position, transform.Scale)){
            Log::Info("Player collided with enemy!");
            m_PlayerController->Die("Killed by enemy");
            break;
        }
    }

    m_FpsCounter->Update();
    //m_ECSScene->Update(deltaTime);
    Log::InfoThrottled(std::format("Player: ({:.2f}, {:.2f})", transform.Position.x, transform.Position.y),
                       "player_pos", 2.0f);
}

void Demo2DFixedScene::Render(){
    const Matrix4& view = m_Camera.GetViewMatrix();
    const Matrix4& projection = m_Camera.GetProjectionMatrix();

    Log::InfoThrottled(std::format("View[12]={}, View[13]={}",
                                   view.Data()[12], view.Data()[13]), "view_pos", 2.0f);
    Log::InfoThrottled(std::format("Proj[0]={}, Proj[5]={}, Proj[12]={}, Proj[13]={}",
                                   projection.Data()[0], projection.Data()[5],
                                   projection.Data()[12], projection.Data()[13]), "proj_debug", 2.0f);

    m_ECSScene->Render(view, projection);
    m_World->Render(m_LineShader, view, projection);
}

void Demo2DFixedScene::ResolveCollisions(){
    auto& registry = m_ECSScene->GetRegistry();
    auto& playerTransform = registry.get<Components::Transform>(m_PlayerEntity);
    Vector3D playerHalfSize = playerTransform.Scale * 0.5f;

    bool grounded = false;
    const float GROUND_MARGIN = 0.1f;

    float playerLeft = playerTransform.Position.x - playerHalfSize.x;
    float playerRight = playerTransform.Position.x + playerHalfSize.x;
    float playerBottom = playerTransform.Position.y - playerHalfSize.y;
    float playerTop = playerTransform.Position.y + playerHalfSize.y;

    for (entt::entity platform : m_PlatformEntities){
        auto& platformTransform = registry.get<Components::Transform>(platform);
        Vector3D platformHalfSize = platformTransform.Scale * 0.5f;

        float platformLeft = platformTransform.Position.x - platformHalfSize.x;
        float platformRight = platformTransform.Position.x + platformHalfSize.x;
        float platformTop = platformTransform.Position.y + platformHalfSize.y;
        float platformBottom = platformTransform.Position.y - platformHalfSize.y;

        bool overlapX = (playerRight > platformLeft && playerLeft < platformRight);
        bool overlapY = (playerTop > platformBottom && playerBottom < platformTop);

        if (overlapX){
            float distanceToTop = playerBottom - platformTop;
            if (distanceToTop <= GROUND_MARGIN && distanceToTop >= -GROUND_MARGIN){
                //Avoid landing if still going up
                if (m_PlayerController->GetVelocity().y <= 0.0f){
                    grounded = true;
                    playerTransform.Position.y = platformTop + playerHalfSize.y;
                    m_PlayerController->GetVelocity().y = 0.0f;
                    break;
                }
            }
        }

        if (!grounded && overlapX && overlapY){
            Vector3D delta = playerTransform.Position - platformTransform.Position;
            Vector3D overlap = playerHalfSize + platformHalfSize - Vector3D(std::abs(delta.x), std::abs(delta.y), 0.0f);

            if (overlap.x > 0.0f && overlap.y > 0.0f){
                if (overlap.x < overlap.y){
                    if (delta.x > 0.0f) playerTransform.Position.x += overlap.x;
                    else playerTransform.Position.x -= overlap.x;
                } else{

                    if (delta.y < 0.0f){
                        playerTransform.Position.y -= overlap.y;
                        if (m_PlayerController->GetVelocity().y > 0.0f){
                            m_PlayerController->GetVelocity().y = 0.0f;
                        }
                    }
                }
            }
        }
    }

    if (!grounded){
        float worldFloor = m_World->GetMinY() + playerHalfSize.y;
        if (playerTransform.Position.y <= worldFloor + GROUND_MARGIN){
            playerTransform.Position.y = worldFloor;
            if (m_PlayerController->GetVelocity().y <= 0.0f) {
                m_PlayerController->GetVelocity().y = 0.0f;
            }
            grounded = true;
        }
    }

    m_PlayerController->SetGrounded(grounded);
}

void Demo2DFixedScene::SpawnEnemies(){
    auto& registry = m_ECSScene->GetRegistry();
    auto enemyTex = ResourceManager::GetInstance().LoadTexture("assets/textures/enemy.png");
    if (!enemyTex){
        Log::Warning("Enemy texture not found. Using tile texture as fallback.");
        enemyTex = ResourceManager::GetInstance().LoadTexture("assets/textures/tile.png");
        if (!enemyTex) return;
    }

    //Enemy1
    auto enemy1 = m_ECSScene->CreateSpriteEntity(Vector3D(-12.0f, -8.0f, 0.0f),
                                                 Vector3D(3.0f, 3.0f, 1.0f), enemyTex);
    registry.emplace<Enemy>(enemy1);
    registry.emplace<Patrol>(enemy1, Vector3D(-15.0f, -8.0f, 0.0f),
                               Vector3D(-10.0f, -8.0f, 0.0f),2.0f);
    registry.emplace<Components::PhysicsBody>(enemy1);
    m_EnemyEntities.push_back(enemy1);

    //Enemy2
    auto enemy2 = m_ECSScene->CreateSpriteEntity(Vector3D(-8.0f, -2.0f, 0.0f),
                                                 Vector3D(3.0f, 3.0f, 1.0f), enemyTex);
    registry.emplace<Enemy>(enemy2);
    registry.emplace<Patrol>(enemy2, Vector3D(-10.0f, -2.0f, 0.0f),
                               Vector3D(-6.0f, -2.0f, 0.0f), 1.5f);
    registry.emplace<Components::PhysicsBody>(enemy2);
    m_EnemyEntities.push_back(enemy2);

    //Enemy3
    auto enemy3 = m_ECSScene->CreateSpriteEntity(Vector3D(0.0f, 3.0f, 0.0f),
                                                 Vector3D(3.0f, 3.0f, 1.0f), enemyTex);
    registry.emplace<Enemy>(enemy3);
    registry.emplace<Patrol>(enemy3, Vector3D(-2.0f, 3.0f, 0.0f),
                               Vector3D(2.0f, 3.0f, 0.0f), 1.8f);
    registry.emplace<Components::PhysicsBody>(enemy3);
    m_EnemyEntities.push_back(enemy3);

    Log::Info(std::format("Spawned {} enemies", m_EnemyEntities.size()));
}
