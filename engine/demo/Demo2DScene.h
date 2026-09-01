#pragma once

#include "core/PlayerController.h"
#include "core/CameraController.h"
#include "core/FpsCounter.h"
#include "scene/Scene.h"
#include "scene/ECSScene.h"
#include "scene/GameWorld.h"
#include "scene/Camera2D.h"
#include <memory>

class Demo2DScene : public Scene{
public:
    Demo2DScene(Shader& shader, VertexArray& va, IndexBuffer& ib);
    ~Demo2DScene();

    void OnEnter() override;
    void OnExit() override;
    void Update(float deltaTime) override;
    void Render() override;
    std::string GetName() const override { return "Demo2D"; }

private:
    Shader& m_Shader;
    VertexArray& m_VertexArray;
    IndexBuffer& m_IndexBuffer;

    std::unique_ptr<ECSScene> m_ECSScene;
    std::unique_ptr<GameWorld> m_World;
    std::unique_ptr<Camera2D> m_Camera;
    std::unique_ptr<CameraController> m_CameraController;
    std::unique_ptr<PlayerController> m_PlayerController;

    std::unique_ptr<FpsCounter> m_FpsCounter;
    entt::entity m_PlayerEntity;

    void SetupScene();
};
