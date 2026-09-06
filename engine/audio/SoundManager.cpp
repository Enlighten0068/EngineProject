#include "audio/SoundManager.h"
#include "diagnostics/Log.h"

SoundManager& SoundManager::GetInstance(){
    static SoundManager instance;
    return instance;
}

std::shared_ptr<SoundEffect> SoundManager::LoadSound(const std::string& filepath){
    auto it = m_Sounds.find(filepath);
    if (it != m_Sounds.end()){
        return it->second;
    }

    auto sound = std::make_shared<SoundEffect>();
    if (!sound->Load(filepath)){
        Log::Error(std::format("Failed to load sound: {}", filepath));
        return nullptr;
    }

    m_Sounds[filepath] = sound;
    Log::Info(std::format("Sound cached: {}", filepath));
    return sound;
}

void SoundManager::PlaySound(const std::string& filepath, int volume){
    auto sound = LoadSound(filepath);
    if (sound) {
        sound->Play(volume);
    }
}

void SoundManager::Clear(){
    m_Sounds.clear();
    Log::Info("SoundManager cleared.");
}
