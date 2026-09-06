#include "audio/SoundManager.h"
#include "components/Transform.h"
#include "core/PlayerController.h"
#include "core/Input.h"
#include "core/Time.h"
#include "diagnostics/Log.h"
#include "events/PlayerEvents.h"
#include "events/EventBus.h"
#include "events/KeyEvent.h"
#include <algorithm>
#include <format>

PlayerController::PlayerController(entt::registry& registry, entt::entity playerEntity, const GameWorld& world)
: m_Registry(registry), m_PlayerEntity(playerEntity), m_World(world){ SetupEventSubscriptions(); }

void PlayerController::SetupEventSubscriptions(){
    EventBus::GetInstance().Subscribe<KeyEvent>([this](Event& e){
        KeyEvent& keyEvent = static_cast<KeyEvent&>(e);
        if (keyEvent.GetAction() == KeyAction::Pressed && keyEvent.GetScancode() == SDL_SCANCODE_E){
            Log::Info("Interact key pressed.");
            //Interaction function to be added
        }
    });
}

void PlayerController::Update(){
    if (m_IsDead) return;

    float dt = Time::DeltaTime();
    auto& transform = m_Registry.get<Components::Transform>(m_PlayerEntity);

    float speed = m_Speed * dt;
    if (Input::IsKeyHeld(SDL_SCANCODE_LEFT)) transform.Position.x -= speed;
    if (Input::IsKeyHeld(SDL_SCANCODE_RIGHT)) transform.Position.x += speed;

    //Jump
    if (m_IsGrounded && Input::IsKeyPressed(SDL_SCANCODE_SPACE)){
        m_IsJumping = true;
        m_IsGrounded = false;
        m_JumpTimer = 0.0f;
        m_Velocity.y = 15.0f;

        PlayerJumpedEvent jumpEvent(transform.Position, m_Velocity.y);
        EventBus::GetInstance().Dispatch(jumpEvent);
        SoundManager::GetInstance().PlaySound("assets/sounds/jump.wav");
        Log::Info("Jump started (event-driven)");
    }

    if (m_IsJumping){
        if (Input::IsKeyHeld(SDL_SCANCODE_SPACE)){
            m_JumpTimer += dt;
            float gravityReduction = 1.0f - (m_JumpTimer / 0.5f);
            if (gravityReduction < 0.0f) gravityReduction = 0.0f;
            m_Velocity.y += (gravityReduction * 0.5f - 9.8f) * dt;
        } else{
            m_Velocity.y = std::min(m_Velocity.y, 0.0f);
            m_IsJumping = false;
        }
    } else{
        if (!m_IsGrounded) m_Velocity.y -= 9.8f * dt;
        else m_Velocity.y = 0.0f;
    }

    if (!m_IsGrounded || m_Velocity.y > 0.0f) transform.Position.y += m_Velocity.y * dt;

    if (m_IsGrounded && !m_WasGrounded){
        SoundManager::GetInstance().PlaySound("assets/sounds/landing.wav");
        Log::Info("Landed (sound played)");
    }

    m_WasGrounded = m_IsGrounded;

    transform.Position = m_World.ClampPosition(transform.Position);

    Log::InfoThrottled(std::format("Player grounded: {}, Is jumping: {}, Vertical velocity: {:.2f}",
                       m_IsGrounded, m_IsJumping, m_Velocity.y), "check_grounded_state", 2.0f);

    /*Vector3D halfSize = transform.Scale * 0.5f;
    float minX = m_World.GetMinX() + halfSize.x;
    float maxX = m_World.GetMaxX() - halfSize.x;
    float minY = m_World.GetMinY() + halfSize.y;
    float maxY = m_World.GetMaxY() - halfSize.y;
    transform.Position.x = std::clamp(transform.Position.x, minX, maxX);
    transform.Position.y = std::clamp(transform.Position.y, minY, maxY);*/

    if (transform.Position.y < -15.0f) Die("Fell off the world");

    if (m_IsGrounded) m_Velocity.y = 0.0f;

}

void PlayerController::Die(const std::string& cause) {
    if (m_IsDead) return;
    m_IsDead = true;

    auto& transform = m_Registry.get<Components::Transform>(m_PlayerEntity);

    PlayerDiedEvent deathEvent(transform.Position, cause);
    EventBus::GetInstance().Dispatch(deathEvent);

    Log::Info(std::format("Player died: {}", cause));

    transform.Position = Vector3D(0.0f, 0.0f, 0.0f);
    m_Velocity = Vector3D(0.0f, 0.0f, 0.0f);
    m_IsJumping = false;
    m_IsDead = false;
}
