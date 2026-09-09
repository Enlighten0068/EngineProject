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
 */
class Demo2DFixedScene : public Scene{
public:
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
        Vector3D Position;
        Vector3D Scale;
        std::shared_ptr<Texture2D> Texture;
        Vector3D PatrolStart;
        Vector3D PatrolEnd;
        float PatrolSpeed;
    };

    Shader& m_Shader;
    VertexArray& m_VertexArray;
    IndexBuffer& m_IndexBuffer;

    Camera2D& m_Camera;
    Shader& m_LineShader;

    std::unique_ptr<ECSScene> m_ECSScene;
    std::unique_ptr<GameWorld> m_World;
    std::unique_ptr<PlayerController> m_PlayerController;

    std::unique_ptr<FpsCounter> m_FpsCounter;

    entt::entity m_PlayerEntity;
    std::vector<entt::entity> m_EnemyEntities; //Active enemy entities
    std::vector<EnemyDefinition> m_EnemyDefinitions; //Saved initial enemy entities for respawn
    std::vector<entt::entity> m_PlatformEntities;

    void ResolveCollisions();
    void SetupScene();
    void SpawnEnemies();
    void CheckEnemyCollisions();
    void DestroyEnemy(entt::entity enemy);
};
