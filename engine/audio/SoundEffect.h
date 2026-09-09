#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <vector>

/**
 * @brief Handles a sound effect that is loaded from a .wav file.
 *
 * It uses SDL3 audio stream API to manage the loading, playback and unloading of audio data.
 */
class SoundEffect{
public:
    SoundEffect();
    ~SoundEffect();

    /**
     * @brief Loads a .wav file from a specified location.
     * @param filepath Path to the .wav file.
     * @return true if loaded successfully, false otherwise.
     */
    bool Load(const std::string& filepath);

    /**
     * @brief Plays the loaded sound effect once.
     * @param volume Volume level (0-128).
     */
    void Play(int volume = 128);

    /**
     * @brief Unloads the sound data and releases resources.
     */
    void Unload();

    /**
     * @brief Checks if a sound is currently loaded.
     * @return true if sound data is loaded, false otherwise.
     */
    bool IsLoaded() const{ return !m_Data.empty(); }

    /**
     * @brief Gets the shared audio device ID used by all SoundEffect instances.
     * @return SDL_AudioDeviceID of the shared audio device.
     */
    static SDL_AudioDeviceID GetAudioDevice(){ return s_AudioDevice; }

    /**
     * @brief Closes the shared audio device.
     *
     * Should be called during application shutdown for cleanup.
     */
    static void CloseAudioDevice();

private:
    std::vector<Uint8> m_Data; //Raw audio data as PCM samples
    SDL_AudioSpec m_Spec; //Audio specification for the loaded sound
    static SDL_AudioSpec m_DeviceSpec; //Actual audio device specification
    SDL_AudioStream* m_Stream = nullptr; //Audio stream for playback
    static SDL_AudioDeviceID s_AudioDevice; //Shared audio device ID
    static bool s_DeviceOpen; //Flag indicating if the audio device is open

    /**
     * @brief Opens the shared audio device if not already open.
     * @return true if the device is (or was) successfully opened, false otherwise.
     */
    bool OpenDevice();
};
