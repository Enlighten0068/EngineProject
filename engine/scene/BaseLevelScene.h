#pragma once

#include "components/Enemy.h"
#include "components/Patrol.h"
#include "core/CameraController.h"
#include "core/PlayerController.h"
#include "core/FpsCounter.h"
#include "level/CameraMode.h"
#include "scene/Scene.h"
#include "scene/SceneManager.h"
#include "scene/ECSScene.h"
#include "scene/GameWorld.h"
#include "scene/Camera2D.h"
#include "systems/EnemySystem.h"
#include "systems/LevelSystem.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Generic scene that builds its content from a LevelSystem builder function.
 *
 * This scene replaces the old Demo2DFixedScene for level-based gameplay. It
 * receives a builder function (e.g., BuildLevel1) and a camera mode, and uses
 * them to construct and run the level. Pressing ESC returns to the level
 * selector.
 *
 * @see LevelSystem, LevelSelectorScene, LevelRegistry
 */
class BaseLevelScene : public Scene{
public:
    using BuilderFn = std::function<void(LevelSystem&)>;

    /**
     * @brief Constructs a BaseLevelScene.
     * @param shader Reference to the main shader.
     * @param va Reference to the vertex array.
     * @param ib Reference to the index buffer.
     * @param camera Reference to the 2D camera.
     * @param lineShader Reference to the line shader.
     * @param sceneManager Reference to the scene manager (for ESC transitions).
     * @param builder Level builder function.
     * @param levelName Display name of the level.
     * @param cameraMode Camera behavior (Fixed or Follow).
     */
    BaseLevelScene(Shader& shader, VertexArray& va, IndexBuffer& ib,
                   Camera2D& camera, Shader& lineShader,
                   SceneManager& sceneManager,
                   BuilderFn builder, const std::string& levelName,
                   CameraMode cameraMode);

    ~BaseLevelScene() override;

    void OnEnter() override;
    void OnExit() override;
    void OnResize(int width, int height) override;
    void Update(float deltaTime) override;
    void Render() override;
    std::string GetName() const override{ return m_LevelName; }

    /**
     * @brief Returns the HUD lines to show in this scene.
     */
    std::vector<std::string> GetHUDLines() const override{
        return {
            "F11: Toggle Fullscreen",
            "ESC: Exit Level"
        };
    }

    /**
     * @brief Respawns all enemies from their saved definitions.
     */
    void RespawnEnemies();

private:
    void SetupScene(); //Builds the level and initializes controllers
    void ResolveCollisions(); //Resolves player-platform collisions
    void CheckEnemyCollisions(); //Checks player-enemy collisions
    void DestroyEnemy(entt::entity enemy); //Destroys a single enemy entity

    Shader& m_Shader;
    VertexArray& m_VertexArray;
    IndexBuffer& m_IndexBuffer;
    Camera2D& m_Camera;
    Shader& m_LineShader;
    SceneManager& m_SceneManager;
    BuilderFn m_Builder;
    std::string m_LevelName;
    CameraMode m_CameraMode;

    std::unique_ptr<ECSScene> m_ECSScene; //ECS scene containing all entities
    std::unique_ptr<GameWorld> m_World; //World bounds and clamping
    std::unique_ptr<CameraController> m_CameraController; //Player controller
    std::unique_ptr<PlayerController> m_PlayerController; //Camera follow controller (nullptr for Fixed)
    std::unique_ptr<FpsCounter> m_FpsCounter; //FPS counter

    entt::entity m_PlayerEntity; //Player entity ID
    std::vector<entt::entity> m_EnemyEntities; //Active enemy entities
    std::vector<LevelSystem::EnemyDefinition> m_EnemyDefinitions; //Enemy definitions for respawn
    std::vector<entt::entity> m_PlatformEntities; //Platform entities for collision
};
