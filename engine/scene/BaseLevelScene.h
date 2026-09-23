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
 * receives a builder function (e.g., BuildLevel1) and uses it to construct
 * the level. It also provides ESC-to-return-to-selector functionality.
 *
 * @see LevelSystem, LevelSelectorScene
 */
class BaseLevelScene : public Scene{
public:
    using BuilderFn = std::function<void(LevelSystem&)>;

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

    void RespawnEnemies();

    std::vector<std::string> GetHUDLines() const override{
        return {
            "F11: Toggle Fullscreen",
            "ESC: Exit Level"
        };
    }

private:
    void SetupScene();
    void ResolveCollisions();
    void CheckEnemyCollisions();
    void DestroyEnemy(entt::entity enemy);

    Shader& m_Shader;
    VertexArray& m_VertexArray;
    IndexBuffer& m_IndexBuffer;
    Camera2D& m_Camera;
    Shader& m_LineShader;
    SceneManager& m_SceneManager;
    BuilderFn m_Builder;
    std::string m_LevelName;
    CameraMode m_CameraMode;

    std::unique_ptr<ECSScene> m_ECSScene;
    std::unique_ptr<GameWorld> m_World;
    std::unique_ptr<CameraController> m_CameraController;
    std::unique_ptr<PlayerController> m_PlayerController;
    std::unique_ptr<FpsCounter> m_FpsCounter;

    entt::entity m_PlayerEntity;
    std::vector<entt::entity> m_EnemyEntities;
    std::vector<LevelSystem::EnemyDefinition> m_EnemyDefinitions;
    std::vector<entt::entity> m_PlatformEntities;
};
