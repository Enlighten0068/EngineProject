#include "components/Transform.h"
#include "core/PlayerController.h"
#include "core/Input.h"
#include "core/Time.h"
#include "diagnostics/Log.h"
#include <format>

PlayerController::PlayerController(entt::registry& registry, entt::entity playerEntity)
: m_Registry(registry), m_PlayerEntity(playerEntity){}

void PlayerController::Update(){
    float dt = Time::DeltaTime();
    auto& transform = m_Registry.get<Components::Transform>(m_PlayerEntity);


    if (Input::IsKeyHeld(SDL_SCANCODE_UP)){
        transform.Position.y += m_Speed * dt;
        Log::Info("Up key held");
    }
    if (Input::IsKeyHeld(SDL_SCANCODE_DOWN)){
        transform.Position.y -= m_Speed * dt;
        Log::Info("Down key held");
    }
    if (Input::IsKeyHeld(SDL_SCANCODE_LEFT)){
        transform.Position.x -= m_Speed * dt;
        Log::Info("Left key held");
    }
    if (Input::IsKeyHeld(SDL_SCANCODE_RIGHT)){
        transform.Position.x += m_Speed * dt;
        Log::Info("Right key held");
    }

    Vector2D scroll = Input::GetScrollDelta();
    if (scroll.y != 0.0f){
        transform.Scale.x += scroll.y * 0.5f;
        transform.Scale.y += scroll.y * 0.5f;
        Log::Info(std::format("Scale: ({}, {})", transform.Scale.x, transform.Scale.y));
    }

    transform.Rotation.z = 0.785f;
}
