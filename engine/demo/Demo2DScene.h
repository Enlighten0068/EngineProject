#pragma once

#include "core/PlayerController.h"
#include "core/CameraController.h"
#include "core/FpsCounter.h"
#include "scene/Scene.h"
#include "scene/ECSScene.h"
#include "scene/GameWorld.h"
#include "scene/Camera2D.h"
#include <memory>
#include <vector>

/**
 * @brief Demo 2D Scene with a dynamic camera that follows the player.
 *
 * This scene features a camera that follows the player entity, making it
 * suitable for larger levels where the player can move beyond the screen.
 * It includes platform collision and player movement mechanics.
 *
 * @note This scene does not include enemies (unlike Demo2DFixedScene).
 * @see Demo2DFixedScene, PlayerController, CameraController
 */
class Demo2DScene : public Scene{
public:
    /**
     * @brief Constructs the demo scene.
     * @param shader Reference to the main shader.
     * @param va Reference to the vertex array.
     * @param ib Reference to the index buffer.
     */
    Demo2DScene(Shader& shader, VertexArray& va, IndexBuffer& ib);
    ~Demo2DScene();

    void OnEnter() override;
    void OnExit() override;
    void Update(float deltaTime) override;
    void Render() override;
    std::string GetName() const override{ return "Demo2D"; }

private:
    Shader& m_Shader; //Main shader for rendering
    VertexArray& m_VertexArray; //Vertex array for geometry
    IndexBuffer& m_IndexBuffer; //Index buffer for geometry

    std::unique_ptr<ECSScene> m_ECSScene; //ECS scene containing all entities
    std::unique_ptr<GameWorld> m_World; //World boundaries
    std::unique_ptr<Camera2D> m_Camera; //Dynamic camera
    std::unique_ptr<CameraController> m_CameraController; //Camera controller
    std::unique_ptr<PlayerController> m_PlayerController; //Player controller

    std::unique_ptr<FpsCounter> m_FpsCounter; //FPS counter
    entt::entity m_PlayerEntity; //Player entity ID

    std::vector<entt::entity> m_PlatformEntities; //Platform entities for collision

    void ResolveCollisions(); //Resolves player-platform collisions
    void SetupScene(); //Sets up the scene with all entities
};
