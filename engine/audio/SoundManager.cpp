#include "audio/SoundManager.h"
#include "diagnostics/Log.h"

/**
 * @brief Returns the singleton instance of SoundManager.
 *
 * The instance is created lazily (on first call) and destroyed automatically when the program exits.
 *
 * @return Reference to the SoundManager instance.
 */
SoundManager& SoundManager::GetInstance(){
    static SoundManager instance;
    return instance;
}

/**
 * @brief Loads a sound effect from file, with caching.
 *
 * If the sound is already in the cache, returns the cached shared pointer.
 * Otherwise, creates a new SoundEffect instance, loads the file, and stores it.
 *
 * @param filepath Path to the WAV file.
 * @return Shared pointer to the loaded SoundEffect, or nullptr on failure.
 */
std::shared_ptr<SoundEffect> SoundManager::LoadSound(const std::string& filepath){
    //Checks if the sound is cached
    auto it = m_Sounds.find(filepath);
    if (it != m_Sounds.end()){
        return it->second;
    }

    auto sound = std::make_shared<SoundEffect>();
    if (!sound->Load(filepath)){
        Log::Error(std::format("Failed to load sound: {}", filepath));
        return nullptr;
    }

    //Stores the loaded file above into cache
    m_Sounds[filepath] = sound;
    Log::Info(std::format("Sound cached: {}", filepath));
    return sound;
}

/**
 * @brief Plays a sound effect, loading it if not already cached.
 * @param filepath Path to the .wav file.
 * @param volume Volume level (0-128, default 128 = full volume).
 */
void SoundManager::PlaySound(const std::string& filepath, int volume){
    auto sound = LoadSound(filepath);
    if (sound) {
        sound->Play(volume);
    }
}

/**
 * @brief Clears all cached sound effects.
 *
 * Removes all entries from the sound cache, releasing the shared pointers.
 */
void SoundManager::Clear(){
    m_Sounds.clear();
    Log::Info("SoundManager cleared.");
}
