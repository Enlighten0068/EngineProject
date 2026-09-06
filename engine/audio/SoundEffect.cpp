#include "audio/SoundEffect.h"
#include "diagnostics/Log.h"
#include <format>
#include <fstream>
#include <SDL3/SDL_audio.h>

SDL_AudioDeviceID SoundEffect::s_AudioDevice = 0;
bool SoundEffect::s_DeviceOpen = false;
SDL_AudioSpec SoundEffect::m_DeviceSpec{};

SoundEffect::SoundEffect() : m_Spec{}, m_Stream(nullptr) {}

SoundEffect::~SoundEffect(){
    Unload();
}

bool SoundEffect::OpenDevice(){
    if (s_DeviceOpen) return true;

    SDL_AudioSpec desiredSpec;
    SDL_zero(desiredSpec);
    desiredSpec.format = SDL_AUDIO_S16;
    desiredSpec.channels = 2;
    desiredSpec.freq = 44100;

    s_AudioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &desiredSpec);
    if (s_AudioDevice == 0){
        Log::Error(std::format("Failed to open audio device: {}", SDL_GetError()));
        return false;
    }

    m_DeviceSpec = desiredSpec;

    Log::Info(std::format("Audio device opened. Using format: {} Hz, {} channels, format {}",
                          m_DeviceSpec.freq, m_DeviceSpec.channels,
                          static_cast<int>(m_DeviceSpec.format)));

    s_DeviceOpen = true;
    return true;
}

bool SoundEffect::Load(const std::string& filepath){
    if (!OpenDevice()) return false;

    SDL_IOStream* io = SDL_IOFromFile(filepath.c_str(), "rb");
    if (!io){
        Log::Error(std::format("Failed to open WAV file: {}", SDL_GetError()));
        return false;
    }

    Uint8* data = nullptr;
    Uint32 dataLen = 0;
    SDL_AudioSpec spec;

    if (!SDL_LoadWAV_IO(io, 1, &spec, &data, &dataLen)){
        Log::Error(std::format("Failed to load WAV: {}", SDL_GetError()));
        return false;
    }

    m_Spec = spec;
    m_Data.assign(data, data + dataLen);
    SDL_free(data);

    Log::Info(std::format("Sound loaded: {} ({} bytes)", filepath, m_Data.size()));
    return true;
}

void SoundEffect::Play(int volume){
    if (m_Data.empty()){
        Log::Warning("Attempted to play unloaded sound.");
        return;
    }

    if (!s_DeviceOpen){
        Log::Warning("Audio device not open.");
        return;
    }

    if (m_DeviceSpec.format == 0){
        Log::Error("Device format is invalid. Cannot create audio stream.");
        return;
    }

    if (m_Stream){
        SDL_UnbindAudioStream(m_Stream);
        SDL_DestroyAudioStream(m_Stream);
        m_Stream = nullptr;
    }

    m_Stream = SDL_CreateAudioStream(&m_Spec, &m_DeviceSpec);
    if (!m_Stream){
        Log::Error(std::format("Failed to create audio stream: {}", SDL_GetError()));
        return;
    }

    if (SDL_PutAudioStreamData(m_Stream, m_Data.data(), m_Data.size()) < 0) {
        Log::Error(std::format("Failed to put data into stream: {}", SDL_GetError()));
        SDL_DestroyAudioStream(m_Stream);
        m_Stream = nullptr;
        return;
    }

    if (SDL_BindAudioStream(s_AudioDevice, m_Stream) < 0){
        Log::Error(std::format("Failed to bind audio stream: {}", SDL_GetError()));
        SDL_DestroyAudioStream(m_Stream);
        m_Stream = nullptr;
        return;
    }

    SDL_ResumeAudioDevice(s_AudioDevice);
    Log::Info(std::format("Sound played: {} bytes (converted from {} Hz, {} ch to {} Hz, {} ch)",
                          m_Data.size(), m_Spec.freq, m_Spec.channels,
                          m_DeviceSpec.freq, m_DeviceSpec.channels));
}

void SoundEffect::Unload(){
    if (m_Stream){
        SDL_UnbindAudioStream(m_Stream);
        SDL_DestroyAudioStream(m_Stream);
        m_Stream = nullptr;
    }

    m_Data.clear();
    m_Spec = {};
}

void SoundEffect::CloseAudioDevice(){
    if (s_AudioDevice != 0){
        SDL_CloseAudioDevice(s_AudioDevice);
        s_AudioDevice = 0;
        s_DeviceOpen = false;
        Log::Info("Audio device closed.");
    }
}
