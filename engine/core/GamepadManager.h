#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>

class GamepadManager{
public:
    static GamepadManager& GetInstance();

    bool Initialize();
    void Shutdown();

    void Update();

    bool IsConnected(int playerIndex = 0) const;

    bool IsButtonPressed(int playerIndex, SDL_GamepadButton button) const;
    bool IsButtonHeld(int playerIndex, SDL_GamepadButton button) const;
    bool IsButtonReleased(int playerIndex, SDL_GamepadButton button) const;

    float GetAxis(int playerIndex, SDL_GamepadAxis axis) const;

    float GetTrigger(int playerIndex, SDL_GamepadAxis axis) const;

    void SetRumble(int playerIndex, float lowFrequency, float highFrequency, Uint32 durationMs);

    void ProcessEvent(const SDL_Event& event);

private:
    GamepadManager() = default;
    ~GamepadManager() = default;
    GamepadManager(const GamepadManager&) = delete;
    GamepadManager& operator=(const GamepadManager&) = delete;

    std::unordered_map<int, SDL_Gamepad*> m_Gamepads;
    std::unordered_map<int, Uint64> m_ButtonState[4];
    std::unordered_map<int, Uint64> m_ButtonPrevious[4];

    static constexpr int MAX_PLAYERS = 4;

    void AddGamepad(SDL_JoystickID deviceID);
    void RemoveGamepad(SDL_JoystickID deviceID);
    void UpdateButtonState(int playerIndex);
};
