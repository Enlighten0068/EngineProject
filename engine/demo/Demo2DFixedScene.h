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

class Demo2DFixedScene : public Scene{
public:
    Demo2DFixedScene(Shader& shader, VertexArray& va, IndexBuffer& ib);
    ~Demo2DFixedScene();

    void OnEnter() override;
    void OnExit() override;
    void Update(float deltaTime) override;
    void Render() override;
    std::string GetName() const override { return "Demo2D_FixedCamera"; }

private:
    Shader& m_Shader;
    VertexArray& m_VertexArray;
    IndexBuffer& m_IndexBuffer;

    Matrix4 m_ViewMatrix;
    Matrix4 m_ProjectionMatrix;

    std::unique_ptr<ECSScene> m_ECSScene;
    std::unique_ptr<GameWorld> m_World;
    std::unique_ptr<PlayerController> m_PlayerController;

    std::unique_ptr<FpsCounter> m_FpsCounter;

    entt::entity m_PlayerEntity;
    std::vector<entt::entity> m_EnemyEntities;
    std::vector<entt::entity> m_PlatformEntities;

    void ResolveCollisions();
    void SetupScene();
    void SpawnEnemies();
};
