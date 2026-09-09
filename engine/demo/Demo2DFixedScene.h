#pragma once

#include "components/Enemy.h"
#include "components/Patrol.h"
#include "core/PlayerController.h"
#include "core/CameraController.h"
#include "core/FpsCounter.h"
#include "scene/Scene.h"
#include "scene/ECSScene.h"
#include "scene/GameWorld.h"
#include "systems/EnemySystem.h"
#include <memory>
#include <vector>

/**
 * @brief Demo 2D Scene with a fixed camera and enemy stomping mechanics.
 *
 * This scene implements Super Mario-style enemy stomping: jumping on top
 * of an enemy destroys it, while touching it from the sides or below
 * kills the player. Enemies respawn when the player dies.
 *
 * The camera is fixed to show the entire world, making it suitable for
 * small platformer levels where the whole level is visible on screen.
 *
 * @see Demo2DScene, EnemySystem, PlayerController
 */
class Demo2DFixedScene : public Scene{
public:
    /**
     * @brief Constructs the fixed camera demo scene.
     * @param shader Reference to the main shader.
     * @param va Reference to the vertex array.
     * @param ib Reference to the index buffer.
     * @param camera Reference to the 2D camera.
     * @param lineShader Reference to the line shader for world borders.
     */
    Demo2DFixedScene(Shader& shader, VertexArray& va, IndexBuffer& ib,
                     Camera2D& camera, Shader& lineShader);
    ~Demo2DFixedScene();

    void OnEnter() override;
    void OnExit() override;
    void Update(float deltaTime) override;
    void Render() override;
    std::string GetName() const override{ return "Demo2D_FixedCamera"; }

    /**
     * @brief Respawns all enemies in the scene.
     *
     * On player death, recreates all enemies from their saved definitions, restoring
     * their positions, patrol ranges, and physics state.
     */
    void RespawnEnemies();

private:
    /**
     * @brief Definition of an enemy for spawning and respawning.
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

    Shader& m_Shader; //Main shader for rendering
    VertexArray& m_VertexArray; //Vertex array for geometry
    IndexBuffer& m_IndexBuffer; //Index buffer for geometry

    Camera2D& m_Camera; //Fixed camera
    Shader& m_LineShader; //Line shader for world borders

    std::unique_ptr<ECSScene> m_ECSScene; //ECS scene containing all entities
    std::unique_ptr<GameWorld> m_World; //World boundaries
    std::unique_ptr<PlayerController> m_PlayerController; //Player controller

    std::unique_ptr<FpsCounter> m_FpsCounter; //FPS counter

    entt::entity m_PlayerEntity; //Player entity ID
    std::vector<entt::entity> m_EnemyEntities; //Active enemy entities
    std::vector<EnemyDefinition> m_EnemyDefinitions; //Saved enemy definitions for respawn
    std::vector<entt::entity> m_PlatformEntities; //Platform entities for collision

    void ResolveCollisions(); //Resolves player-platform collisions
    void SetupScene(); //Sets up the scene with all entities
    void SpawnEnemies(); //Spawns all enemies
    void CheckEnemyCollisions(); //Checks player-enemy collisions
    void DestroyEnemy(entt::entity enemy); //Destroys an enemy entity
};
