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

/**
 * @brief Opens the shared audio device with default parameters.
 *
 * The device is configured for 44.1 kHz, 16-bit stereo audio.
 * This is a one-time initialization - subsequent calls are ignored if the device is already open.
 *
 * @return true if the device was successfully opened or is already open, false on failure.
 */
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

    if (!SDL_GetAudioDeviceFormat(s_AudioDevice, &m_DeviceSpec, nullptr)) {
        Log::Error(std::format("Failed to get actual device format: {}", SDL_GetError()));
        SDL_CloseAudioDevice(s_AudioDevice);
        return false;
    }

    Log::Info(std::format("Audio device opened. ACTUAL format: {} Hz, {} channels",
                          m_DeviceSpec.freq, m_DeviceSpec.channels));

    s_DeviceOpen = true;
    return true;
}

/**
 * @brief Loads a .wav file from a specified location on disk.
 *
 * Opens the shared audio device if not already open, then reads the .wav file into memory.
 * The audio data is stored as raw PCM samples for later playback.
 *
 * @param filepath Path to the .wav file.
 * @return true if the file was loaded successfully, false otherwise.
 */
bool SoundEffect::Load(const std::string& filepath){
    if (!OpenDevice()) return false;

    //Uses SDL_IOStream struct to convert opened file
    SDL_IOStream* io = SDL_IOFromFile(filepath.c_str(), "rb");
    if (!io){
        Log::Error(std::format("Failed to open .wav file: {}", SDL_GetError()));
        return false;
    }

    Uint8* data = nullptr;
    Uint32 dataLen = 0;
    SDL_AudioSpec spec;

    //Loading .wav file into memory
    if (!SDL_LoadWAV_IO(io, 1, &spec, &data, &dataLen)){
        Log::Error(std::format("Failed to load .wav: {}", SDL_GetError()));
        return false;
    }

    //Audio data and specification storage
    m_Spec = spec;
    m_Data.assign(data, data + dataLen);
    SDL_free(data);

    Log::Info(std::format("Sound loaded: {} ({} bytes)", filepath, m_Data.size()));
    return true;
}

/**
 * @brief Plays the loaded sound effect once.
 *
 * Creates a new audio stream, pushes the audio data into it, and binds it to the shared device.
 * If there's a previous stream active, unbounds and destroys it to avoid errors.
 *
 * @param volume Volume level.
 */
void SoundEffect::Play(int volume){
    if (m_Data.empty()){
        Log::Warning("Attempted to play unloaded sound.");
        return;
    }

    if (!s_DeviceOpen){
        Log::Warning("Audio device not open.");
        return;
    }

    //Validates device format
    if (m_DeviceSpec.format == 0){
        Log::Error("Device format is invalid. Cannot create audio stream.");
        return;
    }

    //Previous stream cleanup - Not the same as SoundEffect::Unload found below
    if (m_Stream){
        SDL_UnbindAudioStream(m_Stream);
        SDL_DestroyAudioStream(m_Stream);
        m_Stream = nullptr;
    }

    //Creates a new audio stream to convert the .wav format to the device format
    m_Stream = SDL_CreateAudioStream(&m_Spec, &m_DeviceSpec);
    if (!m_Stream){
        Log::Error(std::format("Failed to create audio stream: {}", SDL_GetError()));
        return;
    }

    float volumeFactor = static_cast<float>(volume) / 128.0f;
    SDL_SetAudioStreamGain(m_Stream, volumeFactor);

    //Pushes the audio data into the audio stream
    if (!SDL_PutAudioStreamData(m_Stream, m_Data.data(), m_Data.size())) {
        Log::Error(std::format("Failed to put data into stream: {}", SDL_GetError()));
        SDL_DestroyAudioStream(m_Stream);
        m_Stream = nullptr;
        return;
    }

    //Binds the stream to the audio device for playback
    if (!SDL_BindAudioStream(s_AudioDevice, m_Stream)){
        Log::Error(std::format("Failed to bind audio stream: {}", SDL_GetError()));
        SDL_DestroyAudioStream(m_Stream);
        m_Stream = nullptr;
        return;
    }

    //From SDL documentation: Unpauses audio processing for a given device. Then logs for diagnostics purposes
    SDL_ResumeAudioDevice(s_AudioDevice);
    Log::Info(std::format("Sound played: {} bytes (converted from {} Hz, {} ch to {} Hz, {} ch)",
                          m_Data.size(), m_Spec.freq, m_Spec.channels,
                          m_DeviceSpec.freq, m_DeviceSpec.channels));
}

/**
 * @brief Unloads the sound effect and releases all resources.
 *
 * Unbinds and destroys the audio stream, clears the audio data, and resets the specification.
 */
void SoundEffect::Unload(){
    if (m_Stream){
        SDL_UnbindAudioStream(m_Stream);
        SDL_DestroyAudioStream(m_Stream);
        m_Stream = nullptr;
    }

    m_Data.clear();
    m_Spec = {};
}

/**
 * @brief Closes the shared audio device.
 *
 * This should be called during application shutdown to release the audio device
 * and free its associated resources.
 */
void SoundEffect::CloseAudioDevice(){
    if (s_AudioDevice != 0){
        SDL_CloseAudioDevice(s_AudioDevice);
        s_AudioDevice = 0;
        s_DeviceOpen = false;
        Log::Info("Audio device closed.");
    }
}
