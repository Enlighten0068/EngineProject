#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>

/**
 * @brief Singleton manager for gamepad input using SDL3 Gamepad API.
 *
 * Provides a unified interface for detecting and querying gamepad state
 * (buttons, axes, triggers, and rumble). Supports multiple players
 * and handles gamepad connection/disconnection events.
 *
 * @note This class is a singleton to ensure a single point of control
 *       for all gamepad input across the engine.
 * @see SDL_Gamepad, SDL_JoystickID
 */
class GamepadManager{
public:

    /**
     * @brief Get the singleton instance.
     * @return Reference to the GamepadManager instance.
     */
    static GamepadManager& GetInstance();

    /**
     * @brief Initializes the gamepad subsystem and detects connected devices.
     * @return true if initialization succeeded, false otherwise.
     */
    bool Initialize();

    void Shutdown(); //Shutdown the gamepad system

    /**
     * @brief Updates the state of all connected gamepads.
     *
     * Should be called once per frame to update button states and axes.
     */
    void Update();

    /**
     * @brief Check if a gamepad is connected for a specific player.
     * @param playerIndex Player index (0-3).
     * @return true if a gamepad is connected, false otherwise.
     */
    bool IsConnected(int playerIndex = 0) const;

    //Button states
    bool IsButtonPressed(int playerIndex, SDL_GamepadButton button) const;
    bool IsButtonHeld(int playerIndex, SDL_GamepadButton button) const;
    bool IsButtonReleased(int playerIndex, SDL_GamepadButton button) const;

    float GetAxis(int playerIndex, SDL_GamepadAxis axis) const;
    float GetTrigger(int playerIndex, SDL_GamepadAxis axis) const;

    /**
     * @brief Triggers rumble/vibration on the gamepad.
     * @param playerIndex Player index (0-3).
     * @param lowFrequency Low-frequency rumble intensity (0.0 to 1.0).
     * @param highFrequency High-frequency rumble intensity (0.0 to 1.0).
     * @param durationMs Duration of the rumble in milliseconds.
     */
    void SetRumble(int playerIndex, float lowFrequency, float highFrequency, Uint32 durationMs);

    /**
     * @brief Processes SDL gamepad events (added/removed).
     * @param event The SDL event to process.
     */
    void ProcessEvent(const SDL_Event& event);

private:
    GamepadManager() = default;
    ~GamepadManager() = default;
    GamepadManager(const GamepadManager&) = delete;
    GamepadManager& operator=(const GamepadManager&) = delete;

    std::unordered_map<int, SDL_Gamepad*> m_Gamepads;
    std::unordered_map<int, Uint64> m_ButtonState[4];
    std::unordered_map<int, Uint64> m_ButtonPrevious[4];

    static constexpr int MAX_PLAYERS = 4; //Maximum number of supported players

    void AddGamepad(SDL_JoystickID deviceID);
    void RemoveGamepad(SDL_JoystickID deviceID);
    void UpdateButtonState(int playerIndex);
};
