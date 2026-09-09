#pragma once

#include "audio/SoundEffect.h"
#include <unordered_map>
#include <memory>
#include <string>

/**
 * @brief Manager for loading, caching, and playing sound effects.
 *
 * Provides a centralized interface for sound management, ensuring that sound files
 * are loaded only once and reused across multiple playback requests.
 */
class SoundManager{
public:
    /**
     * @brief Gets the singleton instance of SoundManager.
     * @return Reference to the SoundManager instance.
     */
    static SoundManager& GetInstance();

    /**
     * @brief Loads a sound effect from file, caching it for future use.
     *
     * If the sound is already loaded, returns the cached instance.
     * Otherwise, loads the file and stores it in the cache.
     *
     * @param filepath Path to the WAV file.
     * @return Shared pointer to the loaded SoundEffect, or nullptr on failure.
     */
    std::shared_ptr<SoundEffect> LoadSound(const std::string& filepath);

    /**
     * @brief Plays a sound effect (loads it if not already cached).
     *
     * Convenience method that combines LoadSound and Play.
     *
     * @param filepath Path to the WAV file.
     * @param volume Volume level (0-128, default 128 = full volume).
     */
    void PlaySound(const std::string& filepath, int volume = 128);

    /**
     * @brief Clears all cached sound effects, releasing their resources.
     */
    void Clear();

private:
    std::unordered_map<std::string, std::shared_ptr<SoundEffect>> m_Sounds; //Cache of loaded sounds
};
