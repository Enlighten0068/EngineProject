#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <vector>

class SoundEffect{
public:
    SoundEffect();
    ~SoundEffect();

    bool Load(const std::string& filepath);
    void Play(int volume = 128);
    void Unload();
    bool IsLoaded() const{ return !m_Data.empty(); }

    static SDL_AudioDeviceID GetAudioDevice(){ return s_AudioDevice; }
    static void CloseAudioDevice();

private:
    std::vector<Uint8> m_Data;
    SDL_AudioSpec m_Spec;
    static SDL_AudioSpec m_DeviceSpec;
    SDL_AudioStream* m_Stream = nullptr;

    static SDL_AudioDeviceID s_AudioDevice;
    static bool s_DeviceOpen;
    bool OpenDevice();
};
