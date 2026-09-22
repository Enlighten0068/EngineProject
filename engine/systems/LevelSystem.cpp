#include "systems/LevelSystem.h"
#include "diagnostics/Log.h"
#include <format>

LevelSystem::LevelSystem(ECSScene& scene,
                         std::shared_ptr<Texture2D> playerTex,
                         std::shared_ptr<Texture2D> tileTex,
                         std::shared_ptr<Texture2D> enemyTex)
: m_Scene(scene),
m_PlayerTex(playerTex),
m_TileTex(tileTex),
m_EnemyTex(enemyTex){}

void LevelSystem::SetWorldBounds(float minX, float maxX, float minY, float maxY){
    m_MinX = minX;
    m_MaxX = maxX;
    m_MinY = minY;
    m_MaxY = maxY;
    Log::Info(std::format("LevelSystem: world bounds set to [{},{}] x [{},{}]",
                          minX, maxX, minY, maxY));
}

void LevelSystem::CreatePlayer(float x, float y){
    m_PlayerEntity = m_Scene.CreateSpriteEntity(
        Vector3D(x, y, 0.0f), Vector3D(3.0f, 3.0f, 1.0f), m_PlayerTex);
    Log::Info(std::format("LevelSystem: player created at ({}, {})", x, y));
}

void LevelSystem::CreatePlatform(float x, float y, float w, float h,
                                 float tileX, float tileY){
    auto entity = m_Scene.CreateSpriteEntity(
        Vector3D(x, y, 0.0f), Vector3D(w, h, 1.0f), m_TileTex);
    m_PlatformEntities.push_back(entity);

    // Default tile scale = width (so texture repeats once per unit)
    float tx = (tileX < 0.0f) ? w : tileX;
    m_Scene.GetRegistry().emplace<Components::TileScale>(entity, tx, tileY);
}

void LevelSystem::CreateEnemy(float x, float y,
                            float patrolStartX, float patrolEndX, float speed){
    auto entity = m_Scene.CreateSpriteEntity(
        Vector3D(x, y, 0.0f), Vector3D(3.0f, 3.0f, 1.0f), m_EnemyTex);

    auto& registry = m_Scene.GetRegistry();
    registry.emplace<Components::Enemy>(entity);
    registry.emplace<Components::Patrol>(entity,
                                        Vector3D(patrolStartX, y, 0.0f),
                                        Vector3D(patrolEndX, y, 0.0f),
                                        speed);
    registry.emplace<Components::PhysicsBody>(entity);

    m_EnemyEntities.push_back(entity);

    EnemyDefinition def;
    def.Position = Vector3D(x, y, 0.0f);
    def.Scale = Vector3D(3.0f, 3.0f, 1.0f);
    def.Texture = m_EnemyTex;
    def.PatrolStart = Vector3D(patrolStartX, y, 0.0f);
    def.PatrolEnd = Vector3D(patrolEndX, y, 0.0f);
    def.PatrolSpeed = speed;
    m_EnemyDefinitions.push_back(def);
}
