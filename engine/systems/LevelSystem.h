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
 * world bounds. The GameWorld itself is created by the scene after the
 * level is built (via GetWorldBounds accessors). This separation avoids
 * copying a non-copyable GameWorld.
 *
 * Each level is defined in a separate .cpp file as a function that receives
 * a reference to a LevelSystem and calls its methods.
 *
 * @see BaseLevelScene, LevelRegistry, REGISTER_LEVEL
 */
class LevelSystem{
public:
    /**
     * @brief Definition of an enemy for respawn.
     *
     * Stores all the data needed to recreate an enemy entity.
     */
    struct EnemyDefinition{
        Vector3D Position; //Spawn position
        Vector3D Scale; //Size of the enemy
        std::shared_ptr<Texture2D> Texture; //Enemy texture
        Vector3D PatrolStart; //Start of patrol range
        Vector3D PatrolEnd; //End of patrol range
        float PatrolSpeed; //Movement speed during patrol
    };

    /**
     * @brief Constructs a LevelSystem.
     * @param scene Reference to the ECS scene.
     * @param playerTex Texture for the player.
     * @param tileTex Texture for platforms and walls.
     * @param enemyTex Texture for enemies.
     */
    LevelSystem(ECSScene& scene, std::shared_ptr<Texture2D> playerTex,
                std::shared_ptr<Texture2D> tileTex,
                std::shared_ptr<Texture2D> enemyTex);

    //Level definition API
    /**
     * @brief Sets the world bounds (does NOT create the GameWorld).
     *
     * The GameWorld is created by the scene after the level is built.
     *
     * @param minX Minimum X coordinate.
     * @param maxX Maximum X coordinate.
     * @param minY Minimum Y coordinate.
     * @param maxY Maximum Y coordinate.
     */
    void SetWorldBounds(float minX, float maxX, float minY, float maxY);

    /**
     * @brief Creates the player entity at the given position.
     * @param x Spawn X.
     * @param y Spawn Y.
     * @param scale Scale vector (default: 3x3).
     */
    void CreatePlayer(float x, float y, const Vector3D& scale = Vector3D(3.0f, 3.0f, 1.0f));

    /**
     * @brief Creates the player with its bottom edge sitting on the floor.
     *
     * Convenience method that calculates the correct Y position so that the
     * player spawns exactly on top of the given floor level.
     *
     * @param x Spawn X.
     * @param floorY Y coordinate of the floor's top edge.
     * @param scale Scale vector (default: 3x3).
     */
    void CreatePlayerOnFloor(float x, float floorY,
                             const Vector3D& scale = Vector3D(3.0f, 3.0f, 1.0f));

    /**
     * @brief Creates a platform at the given position and size.
     * @param x Center X position.
     * @param y Center Y position.
     * @param w Width.
     * @param h Height.
     * @param tileX Number of horizontal texture repetitions (default: w).
     * @param tileY Number of vertical texture repetitions (default: 1).
     */
    void CreatePlatform(float x, float y, float w, float h,
                        float tileX = -1.0f, float tileY = 1.0f);

    /**
     * @brief Creates an enemy with patrol behavior.
     * @param x Spawn X.
     * @param y Spawn Y.
     * @param patrolStartX Left bound of patrol range.
     * @param patrolEndX Right bound of patrol range.
     * @param speed Movement speed in units per second.
     * @param scale Scale vector (default: 3x3).
     */
    void CreateEnemy(float x, float y,
                     float patrolStartX, float patrolEndX, float speed,
                     const Vector3D& scale = Vector3D(3.0f, 3.0f, 1.0f));

    //Accessors(used by the scene after building)
    float GetMinX() const{ return m_MinX; }
    float GetMaxX() const{ return m_MaxX; }
    float GetMinY() const{ return m_MinY; }
    float GetMaxY() const{ return m_MaxY; }

    entt::entity GetPlayerEntity() const{ return m_PlayerEntity; }
    const std::vector<entt::entity>& GetPlatforms() const{ return m_PlatformEntities; }
    const std::vector<entt::entity>& GetEnemies() const{ return m_EnemyEntities; }
    const std::vector<EnemyDefinition>& GetEnemyDefinitions() const{ return m_EnemyDefinitions; }

private:
    ECSScene& m_Scene; //Reference to the ECS scene
    std::shared_ptr<Texture2D> m_PlayerTex; //Player texture
    std::shared_ptr<Texture2D> m_TileTex; //Tile/platform texture
    std::shared_ptr<Texture2D> m_EnemyTex; //Enemy texture

    float m_MinX = -20.0f, m_MaxX = 20.0f; //World X bounds
    float m_MinY = -10.0f, m_MaxY = 10.0f; //World Y bounds

    entt::entity m_PlayerEntity = entt::null; //Player entity ID
    std::vector<entt::entity> m_PlatformEntities; //Platform entity IDs
    std::vector<entt::entity> m_EnemyEntities; //Enemy entity IDs
    std::vector<EnemyDefinition> m_EnemyDefinitions; //Enemy definitions for respawn
};
