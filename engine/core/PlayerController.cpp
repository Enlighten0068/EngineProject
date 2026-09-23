#include "audio/SoundManager.h"
#include "components/Transform.h"
#include "core/PlayerController.h"
#include "core/GamepadManager.h"
#include "core/Input.h"
#include "core/Time.h"
#include "diagnostics/Log.h"
#include "events/PlayerEvents.h"
#include "events/EventBus.h"
#include "events/KeyEvent.h"
#include <algorithm>
#include <format>

/**
 * @brief Constructs a PlayerController and captures the current transform
 *        as the spawn point for respawn.
 */
PlayerController::PlayerController(entt::registry& registry, entt::entity playerEntity, const GameWorld& world)
: m_Registry(registry), m_PlayerEntity(playerEntity), m_World(world){
    //Capture the current transform as the spawn point for respawn
    if(m_Registry.all_of<Components::Transform>(m_PlayerEntity)){
        m_SpawnPosition = m_Registry.get<Components::Transform>(m_PlayerEntity).Position;
        Log::Info(std::format("PlayerController: spawn captured at ({}, {})",
                              m_SpawnPosition.x, m_SpawnPosition.y));
    }
    SetupEventSubscriptions();
}

/**
 * @brief Sets up event subscriptions for the player.
 *
 * Currently subscribes to key events for interaction (E key).
 * Jump is handled via polling in Update() for more responsive input.
 */
void PlayerController::SetupEventSubscriptions(){
    EventBus::GetInstance().Subscribe<KeyEvent>([this](Event& e){
        KeyEvent& keyEvent = static_cast<KeyEvent&>(e);
        if(keyEvent.GetAction() == KeyAction::Pressed && keyEvent.GetScancode() == SDL_SCANCODE_E){
            Log::Info("Interact key pressed.");
        }
    });
}

/**
 * @brief Updates the player's state (movement, jump, physics).
 *
 * This method handles horizontal movement, jump detection and its physics,
 * vertical velocity, landing detection, boundary clamping and death.
 *
 * @note Vertical movement with UP/DOWN keys is commented out but can be enabled
 *       for debugging or special mechanics.
 */
void PlayerController::Update(){
    if(m_IsDead) return;

    float dt = Time::DeltaTime();
    auto& transform = m_Registry.get<Components::Transform>(m_PlayerEntity);

    //Get direction from keyboard
    Vector2D keyboardDirection(0.0f, 0.0f);
    if(Input::IsKeyHeld(SDL_SCANCODE_LEFT))  keyboardDirection.x -= 1.0f;
    if(Input::IsKeyHeld(SDL_SCANCODE_RIGHT)) keyboardDirection.x += 1.0f;

    //Normalize diagonal movement
    if(keyboardDirection.x != 0.0f && keyboardDirection.y != 0.0f){
        keyboardDirection = keyboardDirection * 0.7071f;
    }

    //Get gamepad left joystick direction
    Vector2D gamepadDirection(0.0f, 0.0f);
    bool gamepadActive = false;
    if(GamepadManager::GetInstance().IsConnected(0)){
        gamepadDirection.x = GamepadManager::GetInstance().GetAxis(0, SDL_GAMEPAD_AXIS_LEFTX);

        float deadzone = 0.2f;
        if(std::abs(gamepadDirection.x) < deadzone) gamepadDirection.x = 0.0f;
        if(std::abs(gamepadDirection.y) < deadzone) gamepadDirection.y = 0.0f;

        float magnitude = std::sqrt(gamepadDirection.x * gamepadDirection.x + gamepadDirection.y * gamepadDirection.y);
        if(magnitude > 0.1f){
            gamepadActive = true;
            gamepadDirection = gamepadDirection / magnitude;
        }
    }

    //Combine directions: gamepad takes priority over keyboard
    Vector2D inputDirection(0.0f, 0.0f);
    if(gamepadActive) inputDirection = gamepadDirection;
    else inputDirection = keyboardDirection;

    //Apply horizontal movement
    float speed = m_Speed * dt;
    if(inputDirection.x != 0.0f || inputDirection.y != 0.0f){
        transform.Position.x += inputDirection.x * speed;
        transform.Position.y += inputDirection.y * speed;
    }

    //Jump detection
    bool wantToJump = false;
    if(Input::IsKeyPressed(SDL_SCANCODE_SPACE)) wantToJump = true;
    if(GamepadManager::GetInstance().IsButtonPressed(0, SDL_GAMEPAD_BUTTON_EAST)) wantToJump = true;

    //Execute jump
    if(wantToJump && m_IsGrounded){
        m_IsJumping = true;
        m_IsGrounded = false;
        m_JumpTimer = 0.0f;
        m_Velocity.y = 12.0f;

        //Dispatch jump event and play sound
        PlayerJumpedEvent jumpEvent(transform.Position, m_Velocity.y);
        EventBus::GetInstance().Dispatch(jumpEvent);
        SoundManager::GetInstance().PlaySound("assets/sounds/jump.wav");
        Log::Info("Jump started.");
    }

    //Vertical physics (jumping + gravity)
    if(m_IsJumping){
        if(Input::IsKeyHeld(SDL_SCANCODE_SPACE) || GamepadManager::GetInstance().IsButtonHeld(0, SDL_GAMEPAD_BUTTON_EAST)){
            m_JumpTimer += dt;
            float gravityReduction = 1.0f - (m_JumpTimer / 0.5f);
            if(gravityReduction < 0.0f) gravityReduction = 0.0f;
            m_Velocity.y += (gravityReduction * 0.5f - 9.8f) * dt;
        } else{
            m_Velocity.y = std::min(m_Velocity.y, 0.0f);
            m_IsJumping = false;
        }
    } else{
        if(!m_IsGrounded) m_Velocity.y -= 9.8f * dt;
        else m_Velocity.y = 0.0f;
    }

    //Apply vertical velocity
    if(!m_IsGrounded || m_Velocity.y > 0.0f) transform.Position.y += m_Velocity.y * dt;

    //Landing detection and sound
    if(m_IsGrounded && !m_WasGrounded){
        SoundManager::GetInstance().PlaySound("assets/sounds/landing.wav");
        Log::Info("Landed (sound played)");
    }
    m_WasGrounded = m_IsGrounded;

    //Clamp player position to world boundaries
    transform.Position = m_World.ClampPosition(transform.Position);

    //Log player state for debugging
    Log::InfoThrottled(std::format("Player grounded: {}, Is jumping: {}, Vertical velocity: {:.2f}",
                                   m_IsGrounded, m_IsJumping, m_Velocity.y), "check_grounded_state", 2.0f);

    //Death check - fell off the world
    if(transform.Position.y < -15.0f) Die("Fell off the world");

    if(m_IsGrounded) m_Velocity.y = 0.0f;
}

/**
 * @brief Player dies.
 *
 * Dispatches a PlayerDiedEvent, resets the player's position to the center
 * of the world, and resets all physics state to allow immediate respawn.
 *
 * @param cause Reason for death (e.g., "Killed by enemy").
 */
void PlayerController::Die(const std::string& cause){
    if(m_IsDead) return;
    m_IsDead = true;

    auto& transform = m_Registry.get<Components::Transform>(m_PlayerEntity);

    PlayerDiedEvent deathEvent(transform.Position, cause);
    EventBus::GetInstance().Dispatch(deathEvent);

    Log::Info(std::format("Player died: {}. Respawning at ({}, {})",
                          cause, m_SpawnPosition.x, m_SpawnPosition.y));

    //Respawn at the level's spawn point
    transform.Position = m_SpawnPosition;
    m_Velocity = Vector3D(0.0f, 0.0f, 0.0f);
    m_IsJumping = false;
    m_IsDead = false; //Instant respawn
}
