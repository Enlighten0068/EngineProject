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

Demo2DFixedScene::~Demo2DFixedScene(){ OnExit(); }

/**
 * @brief Called when the scene becomes active.
 *
 * Sets the clear color, sets up the scene with all entities,
 * and logs the scene entry.
 */
void Demo2DFixedScene::OnEnter(){
    Log::Info("Entering Demo2DFixedScene with fixed camera.");
    glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
    SetupScene();
    Log::Info("Entered Demo(fixed) successfully.");
}

/**
 * @brief Called when the scene is exited.
 *
 * Releases all resources used by the scene.
 */
void Demo2DFixedScene::OnExit(){
    m_PlayerController.reset();
    m_FpsCounter.reset();
    m_World.reset();
    m_ECSScene.reset();
    m_EnemyEntities.clear();
    m_EnemyDefinitions.clear();
    Log::Info("Exiting Demo2DFixedScene");
}

/**
 * @brief Sets up the scene with all entities.
 *
 * Creates the ECS scene, world, camera, player, platforms, and enemies.
 * Also initializes the player controller and FPS counter.
 */
void Demo2DFixedScene::SetupScene(){
    //Create ECS scene and world
    m_ECSScene = std::make_unique<ECSScene>(m_Shader, m_VertexArray, m_IndexBuffer);
    m_World = std::make_unique<GameWorld>(-20.0f, 20.0f, -10.0f, 10.0f);

    //Set camera projection to show the entire world
    m_Camera.SetProjection(m_World->GetMinX(), m_World->GetMaxX(),
                           m_World->GetMinY(), m_World->GetMaxY());

    float centerX = (m_World->GetMinX() + m_World->GetMaxX()) * 0.5f;
    float centerY = (m_World->GetMinY() + m_World->GetMaxY()) * 0.5f;
    m_Camera.SetPosition(Vector3D(centerX, centerY, 0.0f));
    m_Camera.Update();

    Log::Info("Camera projection set to world bounds.");

    //Load textures
    auto playerTex = ResourceManager::GetInstance().LoadTexture("assets/textures/player.png");
    auto tileTex = ResourceManager::GetInstance().LoadTexture("assets/textures/tile.png");
    if(!playerTex || !tileTex){
        Log::Error("Failed to load one or more textures.");
        return;
    }
    auto wallTex = tileTex;

    //Create player entity
    m_PlayerEntity = m_ECSScene->CreateSpriteEntity(Vector3D(0.0f, 0.0f, 0.0f),
                                                    Vector3D(3.0f, 3.0f, 1.0f),
                                                    playerTex);

    //Create platform entities
    //Left Wall
    auto left_wall = m_ECSScene->CreateSpriteEntity(Vector3D(m_World->GetMinX() + 0.50f, 0.0f, 0.0f),
                                                    Vector3D(1.0f, m_World->GetMaxY() - m_World->GetMinY(), 1.0f),
                                                    wallTex);
    m_PlatformEntities.push_back(left_wall);
    m_ECSScene->GetRegistry().emplace<Components::TileScale>(left_wall, 1.0f, 20.0f);

    //Right Wall
    auto rgt_wall = m_ECSScene->CreateSpriteEntity(Vector3D(m_World->GetMaxX() - 0.50f, 0.0f, 0.0f),
                                                   Vector3D(1.0f, m_World->GetMaxY() - m_World->GetMinY(), 1.0f),
                                                   wallTex);
    m_PlatformEntities.push_back(rgt_wall);
    m_ECSScene->GetRegistry().emplace<Components::TileScale>(rgt_wall, 1.0f, 20.0f);

    //Ceiling
    auto ceiling = m_ECSScene->CreateSpriteEntity(Vector3D(0.0f, m_World->GetMaxY() - 0.50f, 0.0f),
                                                  Vector3D(m_World->GetMaxX() - m_World->GetMinX(), 1.0f, 1.0f),
                                                  wallTex);
    m_PlatformEntities.push_back(ceiling);
    m_ECSScene->GetRegistry().emplace<Components::TileScale>(ceiling, 39.0f, 1.0f);

    //Floor
    auto plt_floor = m_ECSScene->CreateSpriteEntity(Vector3D(0.0f, m_World->GetMinY() + 0.50f, 0.0f),
                                                    Vector3D(m_World->GetMaxX() - m_World->GetMinX(), 1.0f, 1.0f),
                                                    tileTex);
    m_PlatformEntities.push_back(plt_floor);
    m_ECSScene->GetRegistry().emplace<Components::TileScale>(plt_floor, 39.0f, 1.0f);

    //Floating platform 1
    auto plt_float1 = m_ECSScene->CreateSpriteEntity(Vector3D(-8.0f, -3.0f, 0.0f),
                                                     Vector3D(4.0f, 1.0f, 1.0f),
                                                     tileTex);
    m_PlatformEntities.push_back(plt_float1);
    m_ECSScene->GetRegistry().emplace<Components::TileScale>(plt_float1, 4.0f, 1.0f);

    //Floating platform 2
    auto plt_float2 = m_ECSScene->CreateSpriteEntity(Vector3D(8.0f, -3.0f, 0.0f),
                                                     Vector3D(4.0f, 1.0f, 1.0f),
                                                     tileTex);
    m_PlatformEntities.push_back(plt_float2);
    m_ECSScene->GetRegistry().emplace<Components::TileScale>(plt_float2, 4.0f, 1.0f);

    //Central platform
    auto plt_floatcenter = m_ECSScene->CreateSpriteEntity(Vector3D(0.0f, 2.0f, 0.0f),
                                                          Vector3D(8.0f, 1.0f, 1.0f),
                                                          tileTex);
    m_PlatformEntities.push_back(plt_floatcenter);
    m_ECSScene->GetRegistry().emplace<Components::TileScale>(plt_floatcenter, 8.0f, 1.0f);

    //Spawn enemies
    SpawnEnemies();

    //Initialize player controller
    m_PlayerController = std::make_unique<PlayerController>(m_ECSScene->GetRegistry(),
                                                            m_PlayerEntity, *m_World);
    m_PlayerController->SetSpeed(4.0f);

    //Initialize FPS counter
    m_FpsCounter = std::make_unique<FpsCounter>();
}

/**
 * @brief Updates the scene logic.
 * @param deltaTime Time elapsed since the last frame.
 */
void Demo2DFixedScene::Update(float deltaTime){
    Log::InfoThrottled("Update running...", "update_diagnostic", 2.0f);

    //Update player
    m_PlayerController->Update();
    ResolveCollisions();

    auto& transform = m_ECSScene->GetRegistry().get<Components::Transform>(m_PlayerEntity);

    //Update enemies
    EnemySystem::Update(m_ECSScene->GetRegistry(), deltaTime,
                        m_PlatformEntities, m_World->GetMinY());

    //Check enemy collisions
    CheckEnemyCollisions();

    //Check player-enemy collisions
    auto& registry = m_ECSScene->GetRegistry();
    auto& playerTransform = registry.get<Components::Transform>(m_PlayerEntity);
    Vector3D playerSize = playerTransform.Scale;

    auto enemyView = m_ECSScene->GetRegistry().view<Components::Transform, Components::Enemy>();
    for(auto [entity, transform, enemy] : enemyView.each()){
        if(!enemy.IsActive) continue;
        if(EnemySystem::CheckCollision(playerTransform.Position, playerSize, transform.Position, transform.Scale)){
            Log::Info("Player collided with enemy!");
            m_PlayerController->Die("Killed by enemy");
            break;
        }
    }

    //Update FPS counter
    m_FpsCounter->Update();

    //Log player position for debugging
    Log::InfoThrottled(std::format("Player: ({:.2f}, {:.2f})", transform.Position.x, transform.Position.y),
                       "player_pos", 2.0f);
}

/**
 * @brief Renders the scene.
 *
 * Renders all entities and the world borders.
 */
void Demo2DFixedScene::Render(){
    const Matrix4& view = m_Camera.GetViewMatrix();
    const Matrix4& projection = m_Camera.GetProjectionMatrix();

    //Debug logging for camera matrices
    Log::InfoThrottled(std::format("View[12]={}, View[13]={}",
                                   view.Data()[12], view.Data()[13]), "view_pos", 2.0f);
    Log::InfoThrottled(std::format("Proj[0]={}, Proj[5]={}, Proj[12]={}, Proj[13]={}",
                                   projection.Data()[0], projection.Data()[5],
                                   projection.Data()[12], projection.Data()[13]), "proj_debug", 2.0f);

    //Render ECS scene and world borders
    m_ECSScene->Render(view, projection);
    m_World->Render(m_LineShader, view, projection);
}

/**
 * @brief Resolves collisions between the player and platforms.
 *
 * Performs AABB collision detection and resolution between the player
 * and all platform entities. Updates the player's grounded state.
 */
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

/**
 * @brief Spawns all enemies in the scene and saves their definitions for respawn.
 *
 * Creates enemy entities with Patrol and PhysicsBody components, and stores
 * their definitions in m_EnemyDefinitions for later respawning.
 */
void Demo2DFixedScene::SpawnEnemies(){
    //Clear previous enemies and definitions
    for(entt::entity enemy : m_EnemyEntities) m_ECSScene->GetRegistry().destroy(enemy);
    m_EnemyEntities.clear();
    m_EnemyDefinitions.clear();

    auto& registry = m_ECSScene->GetRegistry();
    auto enemyTex = ResourceManager::GetInstance().LoadTexture("assets/textures/enemy.png");
    if(!enemyTex){
        Log::Warning("Enemy texture not found. Using tile texture as fallback.");
        enemyTex = ResourceManager::GetInstance().LoadTexture("assets/textures/tile.png");
        if(!enemyTex) return;
    }

    //Helper lambda to create an enemy and store its definition
    auto createEnemy = [&](const Vector3D& pos, const Vector3D& scale,
                           const Vector3D& patrolStart, const Vector3D& patrolEnd,
                           float speed){
        //Create enemy entity
        auto enemy = m_ECSScene->CreateSpriteEntity(pos, scale, enemyTex);
        registry.emplace<Components::Enemy>(enemy);
        registry.emplace<Components::Patrol>(enemy, patrolStart, patrolEnd, speed);
        registry.emplace<Components::PhysicsBody>(enemy);
        m_EnemyEntities.push_back(enemy);

        //Save definition for respawn
        EnemyDefinition def;
        def.Position = pos;
        def.Scale = scale;
        def.Texture = enemyTex;
        def.PatrolStart = patrolStart;
        def.PatrolEnd = patrolEnd;
        def.PatrolSpeed = speed;
        m_EnemyDefinitions.push_back(def);
    };

        //Create enemies at different positions
        //Enemy 1: ground level left side
        createEnemy(Vector3D(-12.0f, -8.0f, 0.0f),
                    Vector3D(3.0f, 3.0f, 1.0f),
                    Vector3D(-15.0f, -8.0f, 0.0f),
                    Vector3D(-10.0f, -8.0f, 0.0f), 2.0f);

        //Enemy 2: left floating platform
        createEnemy(Vector3D(-8.0f, -2.0f, 0.0f),
                    Vector3D(3.0f, 3.0f, 1.0f),
                    Vector3D(-10.0f, -2.0f, 0.0f),
                    Vector3D(-6.0f, -2.0f, 0.0f), 1.5f);

        //Enemy 3: central platform
        createEnemy(Vector3D(0.0f, 3.0f, 0.0f),
                    Vector3D(3.0f, 3.0f, 1.0f),
                    Vector3D(-2.0f, 3.0f, 0.0f),
                    Vector3D(2.0f, 3.0f, 0.0f), 1.8f);

        Log::Info(std::format("Spawned {} enemies", m_EnemyEntities.size()));
}

/**
 * @brief Respawns all enemies from their saved definitions.
 *
 * Destroys all current enemy entities and recreates them using the
 * stored definitions. This is called when the player dies.
 */
void Demo2DFixedScene::RespawnEnemies(){
    //Destroy existing enemy entities
    for(entt::entity enemy : m_EnemyEntities) m_ECSScene->GetRegistry().destroy(enemy);
    m_EnemyEntities.clear();

    auto& registry = m_ECSScene->GetRegistry();

    //Recreate enemies from stored definitions
    for(const auto& def : m_EnemyDefinitions){
        auto enemy = m_ECSScene->CreateSpriteEntity(def.Position, def.Scale, def.Texture);
        registry.emplace<Components::Enemy>(enemy);
        registry.emplace<Components::Patrol>(enemy, def.PatrolStart, def.PatrolEnd, def.PatrolSpeed);
        registry.emplace<Components::PhysicsBody>(enemy);
        m_EnemyEntities.push_back(enemy);
    }

    Log::Info(std::format("Respawned {} enemies", m_EnemyEntities.size()));
}

/**
 * @brief Checks collisions between the player and all enemies.
 *
 * Determines the direction of collision. If the player is falling (velocity.y < 0)
 * and the collision is from above, the enemy is destroyed (stomp).
 * Otherwise, the player dies.
 */
void Demo2DFixedScene::CheckEnemyCollisions(){
    auto& registry = m_ECSScene->GetRegistry();
    auto& playerTransform = registry.get<Components::Transform>(m_PlayerEntity);
    Vector3D playerSize = playerTransform.Scale;
    Vector3D playerHalf = playerSize * 0.5f;
    float playerBottom = playerTransform.Position.y - playerHalf.y;

    auto enemyView = registry.view<Components::Transform, Components::Enemy, Components::PhysicsBody>();

    for(auto [entity, transform, enemy, physics] : enemyView.each()){
        if(!enemy.IsActive) continue;

        Vector3D enemyHalf = transform.Scale * 0.5f;

        //AABB collision check
        float dx = std::abs(playerTransform.Position.x - transform.Position.x);
        float dy = std::abs(playerTransform.Position.y - transform.Position.y);
        bool colliding = (dx < (playerHalf.x + enemyHalf.x)) &&
        (dy < (playerHalf.y + enemyHalf.y));

        if(!colliding) continue;

        //Determine collision direction
        //Check if player is falling and the collision is from above the enemy - "stomping"
        float enemyTop = transform.Position.y + enemyHalf.y;
        bool isAbove = (playerBottom < enemyTop + 0.1f) && (playerBottom > enemyTop - 0.2f);
        bool isFalling = (m_PlayerController->GetVelocity().y < 0.0f);

        if(isFalling && isAbove){
            //Stomp enemy and bounce
            DestroyEnemy(entity);
            m_PlayerController->GetVelocity().y = 5.0f;
            Log::Info("Enemy stomped!");
        } else{
            //Player dies and respawns all enemies
            Log::Info("Player collided with enemy!");
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
void Demo2DFixedScene::DestroyEnemy(entt::entity enemy){
    auto& registry = m_ECSScene->GetRegistry();
    //Mark enemy as inactive
    if(registry.all_of<Components::Enemy>(enemy)){
        registry.get<Components::Enemy>(enemy).IsActive = false;
    }

    //Remove enemy from list
    auto it = std::find(m_EnemyEntities.begin(), m_EnemyEntities.end(), enemy);
    if(it != m_EnemyEntities.end()) m_EnemyEntities.erase(it);

    //Destroy the entity
    registry.destroy(enemy);
}
