#include "scene/BaseLevelScene.h"
#include "core/Input.h"
#include "diagnostics/Log.h"
#include "resources/ResourceManager.h"
#include "level/LevelSelectorScene.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <format>
#include <algorithm>

BaseLevelScene::BaseLevelScene(Shader& shader, VertexArray& va, IndexBuffer& ib,
                               Camera2D& camera, Shader& lineShader, SceneManager& sceneManager,
                               BuilderFn builder, const std::string& levelName, CameraMode cameraMode)
: m_Shader(shader), m_VertexArray(va), m_IndexBuffer(ib),
m_Camera(camera), m_LineShader(lineShader), m_SceneManager(sceneManager),
m_Builder(builder), m_LevelName(levelName), m_CameraMode(cameraMode){}

BaseLevelScene::~BaseLevelScene(){ OnExit(); }

/**
 * @brief Called when the scene becomes active.
 *
 * Sets the clear color and builds the level.
 */
void BaseLevelScene::OnEnter(){
    Log::Info("Entering BaseLevelScene: " + m_LevelName);
    glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
    SetupScene();
}

/**
 * @brief Called when the scene is exited.
 *
 * Releases all resources and clears entity lists.
 */
void BaseLevelScene::OnExit(){
    m_CameraController.reset();
    m_PlayerController.reset();
    m_FpsCounter.reset();
    m_World.reset();
    m_ECSScene.reset();
    m_EnemyEntities.clear();
    m_EnemyDefinitions.clear();
    m_PlatformEntities.clear();
    Log::Info("Exiting BaseLevelScene: " + m_LevelName);
}

/**
 * @brief Called when the window is resized.
 *
 * For Fixed camera: recompute projection from world bounds (keep everything visible).
 * For Follow camera: keep vertical extent constant, adjust horizontal by aspect.
 *
 * @param width New window width in pixels.
 * @param height New window height in pixels.
 */
void BaseLevelScene::OnResize(int width, int height){
    if(!m_World) return;

    if(m_CameraMode == CameraMode::Fixed){
        m_Camera.SetProjection(m_World->GetMinX(), m_World->GetMaxX(),
                               m_World->GetMinY(), m_World->GetMaxY());
    } else{
        float aspect = static_cast<float>(width) / static_cast<float>(height);
        float viewHeight = 5.0f;
        float viewWidth = viewHeight * aspect;
        m_Camera.SetProjection(-viewWidth, viewWidth, -viewHeight, viewHeight);
    }
    m_Camera.Update();
}

/**
 * @brief Builds the level and initializes all controllers.
 *
 * Loads textures, calls the level builder function, creates the GameWorld
 * from the bounds registered by the LevelSystem, configures the camera
 * (Fixed or Follow), and initializes the player controller.
 */
void BaseLevelScene::SetupScene(){
    m_ECSScene = std::make_unique<ECSScene>(m_Shader, m_VertexArray, m_IndexBuffer);

    //Load textures
    auto playerTex = ResourceManager::GetInstance().LoadTexture("assets/textures/player.png");
    auto tileTex   = ResourceManager::GetInstance().LoadTexture("assets/textures/tile.png");
    auto enemyTex  = ResourceManager::GetInstance().LoadTexture("assets/textures/enemy.png");
    if(!playerTex || !tileTex || !enemyTex){
        Log::Error("Failed to load one or more textures.");
        return;
    }

    //Build level via LevelSystem
    LevelSystem level(*m_ECSScene, playerTex, tileTex, enemyTex);
    m_Builder(level);

    //Create the GameWorld from the bounds the level registered
    m_World = std::make_unique<GameWorld>(
        level.GetMinX(), level.GetMaxX(),
                                          level.GetMinY(), level.GetMaxY());

    //Transfer entities
    m_PlayerEntity = level.GetPlayerEntity();
    m_PlatformEntities = level.GetPlatforms();
    m_EnemyEntities = level.GetEnemies();
    m_EnemyDefinitions = level.GetEnemyDefinitions();

    //Camera setup based on mode
    if(m_CameraMode == CameraMode::Fixed){
        m_Camera.SetProjection(m_World->GetMinX(), m_World->GetMaxX(),
                               m_World->GetMinY(), m_World->GetMaxY());
        m_Camera.SetPosition(Vector3D(0.0f, 0.0f, 0.0f));
    } else{
        //Follow mode: fixed view size around the player
        int w = Input::GetWindowWidth();
        int h = Input::GetWindowHeight();
        float aspect = static_cast<float>(w) / static_cast<float>(h);
        float viewHeight = 5.0f;
        float viewWidth = viewHeight * aspect;
        m_Camera.SetProjection(-viewWidth, viewWidth, -viewHeight, viewHeight);

        //Start camera at the player
        auto& pt = m_ECSScene->GetRegistry().get<Components::Transform>(m_PlayerEntity);
        m_Camera.SetPosition(pt.Position);

        //Camera controller follows the player
        m_CameraController = std::make_unique<CameraController>(m_Camera, *m_World);
        m_CameraController->SetFollowEntity(true);
        m_CameraController->SetMinZoom(0.5f);
        m_CameraController->SetMaxZoom(2.0f);
    }
    m_Camera.Update();

    //Player controller
    m_PlayerController = std::make_unique<PlayerController>(
        m_ECSScene->GetRegistry(), m_PlayerEntity, *m_World);
    m_PlayerController->SetSpeed(4.0f);

    m_FpsCounter = std::make_unique<FpsCounter>();

    Log::Info(std::format("Level '{}' built: {} platforms, {} enemies",
                          m_LevelName, m_PlatformEntities.size(), m_EnemyEntities.size()));
}

/**
 * @brief Updates the level: player, enemies, collisions, camera.
 * @param deltaTime Time elapsed since the last frame.
 */
void BaseLevelScene::Update(float deltaTime){
    //ESC: back to level selector
    if(Input::IsKeyPressed(SDL_SCANCODE_ESCAPE)){
        Log::Info("Returning to LevelSelector...");
        auto selector = std::make_unique<LevelSelectorScene>(
            m_SceneManager, m_Shader, m_VertexArray,
            m_IndexBuffer, m_Camera, m_LineShader);
        m_SceneManager.SetScene(std::move(selector));
        return;
    }

    if(!m_PlayerController || !m_ECSScene) return;

    m_PlayerController->Update();
    ResolveCollisions();

    EnemySystem::Update(m_ECSScene->GetRegistry(), deltaTime,
                        m_PlatformEntities, m_World->GetMinY());

    CheckEnemyCollisions();

    //Follow camera
    if(m_CameraMode == CameraMode::Follow && m_CameraController){
        auto& pt = m_ECSScene->GetRegistry().get<Components::Transform>(m_PlayerEntity);
        m_CameraController->SetTargetPosition(pt.Position);
        m_CameraController->Update(deltaTime);
    }

    m_FpsCounter->Update();
}

/**
 * @brief Renders the scene: entities and world borders.
 */
void BaseLevelScene::Render(){
    if(!m_ECSScene) return;

    const Matrix4& view = m_Camera.GetViewMatrix();
    const Matrix4& projection = m_Camera.GetProjectionMatrix();

    m_ECSScene->Render(view, projection);
    m_World->Render(m_LineShader, view, projection);
}

/**
 * @brief Resolves player-platform collisions using AABB.
 *
 * Handles both landing (player above platform) and ceiling hits (player below).
 */
void BaseLevelScene::ResolveCollisions(){
    auto& registry = m_ECSScene->GetRegistry();
    auto& playerTransform = registry.get<Components::Transform>(m_PlayerEntity);
    Vector3D playerHalfSize = playerTransform.Scale * 0.5f;

    bool grounded = false;
    const float GROUND_MARGIN = 0.1f;

    float playerLeft   = playerTransform.Position.x - playerHalfSize.x;
    float playerRight  = playerTransform.Position.x + playerHalfSize.x;
    float playerBottom = playerTransform.Position.y - playerHalfSize.y;
    float playerTop    = playerTransform.Position.y + playerHalfSize.y;

    for(entt::entity platform : m_PlatformEntities){
        auto& pt = registry.get<Components::Transform>(platform);
        Vector3D pHalf = pt.Scale * 0.5f;

        float pLeft   = pt.Position.x - pHalf.x;
        float pRight  = pt.Position.x + pHalf.x;
        float pTop    = pt.Position.y + pHalf.y;
        float pBottom = pt.Position.y - pHalf.y;

        bool overlapX = (playerRight > pLeft && playerLeft < pRight);
        bool overlapY = (playerTop > pBottom && playerBottom < pTop);

        if(overlapX){
            float distanceToTop = playerBottom - pTop;
            if(distanceToTop <= GROUND_MARGIN && distanceToTop >= -GROUND_MARGIN){
                if(m_PlayerController->GetVelocity().y <= 0.0f){
                    grounded = true;
                    playerTransform.Position.y = pTop + playerHalfSize.y;
                    m_PlayerController->GetVelocity().y = 0.0f;
                    break;
                }
            }
        }

        if(!grounded && overlapX && overlapY){
            Vector3D delta = playerTransform.Position - pt.Position;
            Vector3D overlap = playerHalfSize + pHalf -
            Vector3D(std::abs(delta.x), std::abs(delta.y), 0.0f);

            if(overlap.x > 0.0f && overlap.y > 0.0f){
                if(overlap.x < overlap.y){
                    //Horizontal resolution (player and platform overlap more vertically)
                    if(delta.x > 0.0f) playerTransform.Position.x += overlap.x;
                    else playerTransform.Position.x -= overlap.x;
                } else{
                    //Vertical resolution (player and platform overlap more horizontally)
                    if(delta.y > 0.0f){
                        //Player center is above platform center: push up (landing/spawned inside)
                        playerTransform.Position.y += overlap.y;
                        if(m_PlayerController->GetVelocity().y < 0.0f){
                            m_PlayerController->GetVelocity().y = 0.0f;
                        }
                        grounded = true;
                    } else{
                        //Player center is below platform center: push down (hit from underneath)
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

/**
 * @brief Checks collisions between the player and all enemies.
 *
 * Applies stomping if the player is falling and hits an enemy from above;
 * otherwise kills the player and respawns all enemies.
 */
void BaseLevelScene::CheckEnemyCollisions(){
    auto& registry = m_ECSScene->GetRegistry();
    auto& playerTransform = registry.get<Components::Transform>(m_PlayerEntity);
    Vector3D playerHalf = playerTransform.Scale * 0.5f;
    float playerBottom = playerTransform.Position.y - playerHalf.y;

    auto enemyView = registry.view<Components::Transform, Components::Enemy,
    Components::PhysicsBody>();

    for(auto [entity, transform, enemy, physics] : enemyView.each()){
        if(!enemy.IsActive) continue;

        Vector3D enemyHalf = transform.Scale * 0.5f;
        float dx = std::abs(playerTransform.Position.x - transform.Position.x);
        float dy = std::abs(playerTransform.Position.y - transform.Position.y);
        bool colliding = (dx < (playerHalf.x + enemyHalf.x)) &&
        (dy < (playerHalf.y + enemyHalf.y));
        if(!colliding) continue;

        float enemyTop = transform.Position.y + enemyHalf.y;
        bool isAbove = (playerBottom < enemyTop + 0.1f) &&
        (playerBottom > enemyTop - 0.2f);
        bool isFalling = (m_PlayerController->GetVelocity().y < 0.0f);

        if(isFalling && isAbove){
            //Stomp enemy
            DestroyEnemy(entity);
            m_PlayerController->GetVelocity().y = 5.0f;
            Log::Info("Enemy stomped!");
        } else{
            //Player dies and all enemies respawn
            m_PlayerController->Die("Killed by enemy");
            RespawnEnemies();
            break;
        }
    }
}

/**
 * @brief Destroys an enemy entity.
 * @param enemy The enemy entity to destroy.
 */
void BaseLevelScene::DestroyEnemy(entt::entity enemy){
    auto& registry = m_ECSScene->GetRegistry();
    if(registry.all_of<Components::Enemy>(enemy)){
        registry.get<Components::Enemy>(enemy).IsActive = false;
    }
    auto it = std::find(m_EnemyEntities.begin(), m_EnemyEntities.end(), enemy);
    if(it != m_EnemyEntities.end()) m_EnemyEntities.erase(it);
    registry.destroy(enemy);
}

/**
 * @brief Respawns all enemies from their saved definitions.
 */
void BaseLevelScene::RespawnEnemies(){
    for(entt::entity e : m_EnemyEntities) m_ECSScene->GetRegistry().destroy(e);
    m_EnemyEntities.clear();

    auto& registry = m_ECSScene->GetRegistry();
    for(const auto& def : m_EnemyDefinitions){
        auto enemy = m_ECSScene->CreateSpriteEntity(def.Position, def.Scale, def.Texture);
        registry.emplace<Components::Enemy>(enemy);
        registry.emplace<Components::Patrol>(enemy, def.PatrolStart, def.PatrolEnd, def.PatrolSpeed);
        registry.emplace<Components::PhysicsBody>(enemy);
        m_EnemyEntities.push_back(enemy);
    }
}
