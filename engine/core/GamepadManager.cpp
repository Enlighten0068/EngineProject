#include "core/GamepadManager.h"
#include "diagnostics/Log.h"
#include <format>

//Singleton instance
GamepadManager& GamepadManager::GetInstance(){
    static GamepadManager instance;
    return instance;
}

/**
 * @brief Initializes the gamepad subsystem and detects connected devices.
 *
 * Tries SDL_INIT_GAMEPAD first; if that fails, falls back to SDL_INIT_JOYSTICK.
 * This provides compatibility with systems that don't fully support the gamepad API.
 *
 * @return true if initialization succeeded, false otherwise.
 */
bool GamepadManager::Initialize(){
    //Try gamepad subsystem first
    if(SDL_InitSubSystem(SDL_INIT_GAMEPAD) != 0){
        Log::Error(std::format("Failed to initialize gamepad subsystem: {}", SDL_GetError()));

        //Fallback to joystick subsystem
        Log::Info("Trying to initialize joystick subsystem instead...");
        if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) != 0){
            Log::Error(std::format("Failed to initialize joystick subsystem: {}", SDL_GetError()));
            return false;
        }
        Log::Info("Joystick subsystem initialized successfully.");
    } else{
        Log::Info("Gamepad subsystem initialized successfully.");
    }

    //Get list of connected gamepads
    int numGamepads = 0;
    SDL_JoystickID* gamepadIDs = SDL_GetGamepads(&numGamepads);

    if(!gamepadIDs){
        //Fallback to joystick if gamepad detection fails
        Log::Warning("SDL_GetGamepads failed. Trying SDL_GetJoysticks...");
        int numJoysticks = 0;
        SDL_JoystickID* joystickIDs = SDL_GetJoysticks(&numJoysticks);

        if(joystickIDs){
            for(int i = 0; i < numJoysticks; ++i){
                if(SDL_IsGamepad(joystickIDs[i])){
                    AddGamepad(joystickIDs[i]);
                }
            }
            SDL_free(joystickIDs);
        } else{
            Log::Error(std::format("Failed to get joystick list: {}", SDL_GetError()));
        }
    } else{
        for(int i = 0; i < numGamepads; ++i){
            if(SDL_IsGamepad(gamepadIDs[i])){
                AddGamepad(gamepadIDs[i]);
            }
        }
        SDL_free(gamepadIDs);
    }

    Log::Info(std::format("GamepadManager initialized. {} gamepad(s) connected.", m_Gamepads.size()));
    return true;
}

/**
 * @brief Shuts down the gamepad subsystem and releases all devices.
 */
void GamepadManager::Shutdown(){
    //Close all open gamepads
    for(auto& [playerIndex, gamepad] : m_Gamepads){
        SDL_CloseGamepad(gamepad);
    }
    m_Gamepads.clear();

    //Clear button states for each player
    for(int i = 0; i < MAX_PLAYERS; ++i){
        m_ButtonState[i].clear();
        m_ButtonPrevious[i].clear();
    }
    SDL_QuitSubSystem(SDL_INIT_GAMEPAD);
    Log::Info("GamepadManager shut down.");
}

/**
 * @brief Updates the state of all connected gamepads.
 *
 * Saves the previous button states and updates the current ones.
 * Should be called once per frame.
 */
void GamepadManager::Update(){
    for(auto& [playerIndex, gamepad] : m_Gamepads){
        m_ButtonPrevious[playerIndex] = m_ButtonState[playerIndex];
        UpdateButtonState(playerIndex);
    }
}

/**
 * @brief Adds a new gamepad and assigns it to the first available player slot.
 * @param deviceID SDL Joystick ID of the gamepad.
 */
void GamepadManager::AddGamepad(SDL_JoystickID deviceID){
    if(!SDL_IsGamepad(deviceID)) return;

    SDL_Gamepad* gamepad = SDL_OpenGamepad(deviceID);
    if(!gamepad){
        Log::Error(std::format("Failed to open gamepad {}: {}", deviceID, SDL_GetError()));
        return;
    }

    //Find first available player slot
    int playerIndex = 0;
    while(playerIndex < MAX_PLAYERS && m_Gamepads.find(playerIndex) != m_Gamepads.end()){
        playerIndex++;
    }

    if(playerIndex >= MAX_PLAYERS){
        Log::Warning("Maximum number of gamepads reached.");
        SDL_CloseGamepad(gamepad);
        return;
    }

    //Store gamepad and initialize button states
    m_Gamepads[playerIndex] = gamepad;
    m_ButtonState[playerIndex][playerIndex] = 0;
    m_ButtonPrevious[playerIndex][playerIndex] = 0;

    const char* name = SDL_GetGamepadName(gamepad);
    Log::Info(std::format("Gamepad {} connected: {} (Player {})", deviceID, name ? name : "Unknown", playerIndex));
}

/**
 * @brief Removes a gamepad and frees its resources.
 * @param deviceID SDL Joystick ID of the gamepad to remove.
 */
void GamepadManager::RemoveGamepad(SDL_JoystickID deviceID){
    int playerIndex = -1;
    for(auto& [idx, gamepad] : m_Gamepads){
        if(SDL_GetGamepadID(gamepad) == static_cast<SDL_JoystickID>(deviceID)){
            playerIndex = idx;
            break;
        }
    }

    if(playerIndex != -1){
        SDL_CloseGamepad(m_Gamepads[playerIndex]);
        m_Gamepads.erase(playerIndex);
        Log::Info(std::format("Gamepad {} disconnected (Player {})", deviceID, playerIndex));
    }
}

/**
 * @brief Updates the button state bitmask for a specific player.
 * @param playerIndex Player index (0-3).
 */
void GamepadManager::UpdateButtonState(int playerIndex){
    auto it = m_Gamepads.find(playerIndex);
    if(it == m_Gamepads.end()) return;

    SDL_Gamepad* gamepad = it->second;
    Uint64 state = 0;

    //Build bitmask of all buttons
    for(int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; ++i){
        if(SDL_GetGamepadButton(gamepad, static_cast<SDL_GamepadButton>(i))){
            state |= (1ULL << i);
        }
    }
    m_ButtonState[playerIndex][playerIndex] = state;
}

bool GamepadManager::IsConnected(int playerIndex) const{
    return m_Gamepads.find(playerIndex) != m_Gamepads.end();
}

bool GamepadManager::IsButtonPressed(int playerIndex, SDL_GamepadButton button) const{
    auto itState = m_ButtonState[playerIndex].find(playerIndex);
    auto itPrev = m_ButtonPrevious[playerIndex].find(playerIndex);
    if(itState == m_ButtonState[playerIndex].end() || itPrev == m_ButtonPrevious[playerIndex].end()) return false;

    Uint64 mask = 1ULL << static_cast<int>(button);
    return (itState->second & mask) && !(itPrev->second & mask);
}

bool GamepadManager::IsButtonHeld(int playerIndex, SDL_GamepadButton button) const{
    auto itState = m_ButtonState[playerIndex].find(playerIndex);
    if(itState == m_ButtonState[playerIndex].end()) return false;

    Uint64 mask = 1ULL << static_cast<int>(button);
    return (itState->second & mask);
}

bool GamepadManager::IsButtonReleased(int playerIndex, SDL_GamepadButton button) const{
    auto itState = m_ButtonState[playerIndex].find(playerIndex);
    auto itPrev = m_ButtonPrevious[playerIndex].find(playerIndex);
    if(itState == m_ButtonState[playerIndex].end() || itPrev == m_ButtonPrevious[playerIndex].end()) return false;

    Uint64 mask = 1ULL << static_cast<int>(button);
    return !(itState->second & mask) && (itPrev->second & mask);
}

float GamepadManager::GetAxis(int playerIndex, SDL_GamepadAxis axis) const{
    auto it = m_Gamepads.find(playerIndex);
    if(it == m_Gamepads.end()) return 0.0f;

    Sint16 value = SDL_GetGamepadAxis(it->second, axis);
    return value / 32767.0f; //Convert from raw value to -1.0 to 1.0
}

float GamepadManager::GetTrigger(int playerIndex, SDL_GamepadAxis axis) const{
    auto it = m_Gamepads.find(playerIndex);
    if(it == m_Gamepads.end()) return 0.0f;

    Sint16 value = SDL_GetGamepadAxis(it->second, axis);
    return value / 32767.0f; //Convert from raw value to -1.0 to 1.0
}

void GamepadManager::SetRumble(int playerIndex, float lowFrequency, float highFrequency, Uint32 durationMs){
    auto it = m_Gamepads.find(playerIndex);
    if(it == m_Gamepads.end()) return;

    Uint16 low = static_cast<Uint16>(lowFrequency * 65535);
    Uint16 high = static_cast<Uint16>(highFrequency * 65535);
    SDL_RumbleGamepad(it->second, low, high, durationMs);
}

/**
 * @brief Processes SDL gamepad events (added/removed).
 * @param event The SDL event to process.
 */
void GamepadManager::ProcessEvent(const SDL_Event& event){
    switch(event.type){
        case SDL_EVENT_GAMEPAD_ADDED:
            Log::Info(std::format("Gamepad added: device index {}", event.gdevice.which));
            AddGamepad(event.gdevice.which);
            break;
        case SDL_EVENT_GAMEPAD_REMOVED:
            Log::Info(std::format("Gamepad removed: device index {}", event.gdevice.which));
            RemoveGamepad(event.gdevice.which);
            break;
        default:
            break;
    }
}
