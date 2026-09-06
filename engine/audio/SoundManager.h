#pragma once

#include "audio/SoundEffect.h"
#include <unordered_map>
#include <memory>
#include <string>

class SoundManager{
public:
    static SoundManager& GetInstance();

    std::shared_ptr<SoundEffect> LoadSound(const std::string& filepath);
    void PlaySound(const std::string& filepath, int volume = 128);
    void Clear();

private:
    std::unordered_map<std::string, std::shared_ptr<SoundEffect>> m_Sounds;
};
