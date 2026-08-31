#include "components/Transform.h"
#include "core/PlayerController.h"
#include "core/Input.h"
#include "core/Time.h"
#include "diagnostics/Log.h"
#include "events/PlayerEvents.h"
#include "events/EventBus.h"
#include "events/KeyEvent.h"
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

    EventBus::GetInstance().Subscribe<KeyEvent>([this](Event& e){
        KeyEvent& keyEvent = static_cast<KeyEvent&>(e);
        if (keyEvent.GetAction() == KeyAction::Pressed && keyEvent.GetScancode() == SDL_SCANCODE_SPACE){
            if (!m_IsJumping && !m_IsDead){
                m_IsJumping = true;
                m_JumpTimer = 0.0f;
                m_Velocity.y = 10.0f;

                auto& transform = m_Registry.get<Components::Transform>(m_PlayerEntity);
                PlayerJumpedEvent jumpEvent(transform.Position, m_Velocity.y);
                EventBus::GetInstance().Dispatch(jumpEvent);
                Log::Info("Jump started (event-driven)");
            }
        }

        if (keyEvent.GetAction() == KeyAction::Released && keyEvent.GetScancode() == SDL_SCANCODE_SPACE){
            if (m_IsJumping){
                m_Velocity.y = std::min(m_Velocity.y, 0.0f);
                m_IsJumping = false;
                Log::Info("Jump cut (released early)");
            }
        }
    });
}

void PlayerController::Update(){
    static float lastLogTime = 0.0f;
    if (m_IsDead) return;

    float dt = Time::DeltaTime();
    auto& transform = m_Registry.get<Components::Transform>(m_PlayerEntity);


    float speed = m_Speed * dt;
    if (Input::IsKeyHeld(SDL_SCANCODE_UP)) transform.Position.y += speed;
    if (Input::IsKeyHeld(SDL_SCANCODE_DOWN)) transform.Position.y -= speed;
    if (Input::IsKeyHeld(SDL_SCANCODE_LEFT)) transform.Position.x -= speed;
    if (Input::IsKeyHeld(SDL_SCANCODE_RIGHT)) transform.Position.x += speed;

    Vector2D scroll = Input::GetScrollDelta();
    if (scroll.y != 0.0f){
        transform.Scale.x += scroll.y * 0.5f;
        transform.Scale.y += scroll.y * 0.5f;
    }

    if (m_IsJumping){
        if (Input::IsKeyHeld(SDL_SCANCODE_SPACE)){
            m_JumpTimer += dt;
            float gravityReduction = 1.0f - (m_JumpTimer / 0.5f);
            if (gravityReduction < 0.0f) gravityReduction = 0.0f;
            m_Velocity.y += (gravityReduction * 0.5f - 9.8f) * dt;
        }
    } else  m_Velocity.y -= 9.8f * dt;

    transform.Position.y += m_Velocity.y * dt;

    if (transform.Position.y < -10.0f) {
        transform.Position.y = -10.0f;
        m_Velocity.y = 0.0f;
        m_IsJumping = false;

        if (m_IsJumping) {
            m_IsJumping = false;
            PlayerLandedEvent landEvent(transform.Position, m_Velocity.y);
            EventBus::GetInstance().Dispatch(landEvent);
            Log::Info("Landed (event-driven)");
        }
    }

    transform.Position = m_World.ClampPosition(transform.Position);

    Log::InfoThrottled(std::format("Position after clamp: ({:.2f}, {:.2f})", transform.Position.x, transform.Position.y), "player_position", 2.0f);

    if (transform.Position.y < -15.0f) {
        Die("Fell off the world");
    }
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
