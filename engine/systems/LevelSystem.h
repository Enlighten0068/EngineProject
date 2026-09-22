#pragma once

#include "components/Transform.h"
#include "components/SpriteRenderer.h"
#include "components/TileScale.h"
#include "components/Enemy.h"
#include "components/Patrol.h"
#include "components/PhysicsBody.h"
#include "graphics/Texture2D.h"
#include "math/Vector3D.h"
#include "scene/ECSScene.h"
#include <entt.hpp>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief System that provides a high-level API for building game levels.
 *
 * The LevelSystem accumulates entities as they are created and records the
 * world bounds. The GameWorld itself is created by the scene after the level
 * is built (via GetWorldBounds accessors).
 *
 * @see BaseLevelScene, LevelRegistry
 */
class LevelSystem{
public:
    struct EnemyDefinition{
        Vector3D Position;
        Vector3D Scale;
        std::shared_ptr<Texture2D> Texture;
        Vector3D PatrolStart;
        Vector3D PatrolEnd;
        float PatrolSpeed;
    };

    LevelSystem(ECSScene& scene,
                std::shared_ptr<Texture2D> playerTex,
                std::shared_ptr<Texture2D> tileTex,
                std::shared_ptr<Texture2D> enemyTex);

    // ---------- Level definition API ----------

    void SetWorldBounds(float minX, float maxX, float minY, float maxY);
    void CreatePlayer(float x, float y);
    void CreatePlatform(float x, float y, float w, float h,
                        float tileX = -1.0f, float tileY = 1.0f);
    void CreateEnemy(float x, float y,
                     float patrolStartX, float patrolEndX, float speed);

    // ---------- Accessors ----------

    float GetMinX() const{ return m_MinX; }
    float GetMaxX() const{ return m_MaxX; }
    float GetMinY() const{ return m_MinY; }
    float GetMaxY() const{ return m_MaxY; }

    entt::entity GetPlayerEntity() const{ return m_PlayerEntity; }
    const std::vector<entt::entity>& GetPlatforms() const{ return m_PlatformEntities; }
    const std::vector<entt::entity>& GetEnemies() const{ return m_EnemyEntities; }
    const std::vector<EnemyDefinition>& GetEnemyDefinitions() const{ return m_EnemyDefinitions; }

private:
    ECSScene& m_Scene;
    std::shared_ptr<Texture2D> m_PlayerTex;
    std::shared_ptr<Texture2D> m_TileTex;
    std::shared_ptr<Texture2D> m_EnemyTex;

    float m_MinX = -20.0f, m_MaxX = 20.0f;
    float m_MinY = -10.0f, m_MaxY = 10.0f;

    entt::entity m_PlayerEntity = entt::null;
    std::vector<entt::entity> m_PlatformEntities;
    std::vector<entt::entity> m_EnemyEntities;
    std::vector<EnemyDefinition> m_EnemyDefinitions;
};
