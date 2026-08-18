#pragma once

#include <cstdint>
#include <string>

class Texture2D{
public:
    Texture2D();
    ~Texture2D();

    bool Load(const std::string& filepath);

    void Bind(uint32_t slot = 0) const;
    void Unbind() const;

    int GetWidth() const;
    int GetHeight() const;

    int GetChannels() const;

private:
    uint32_t m_RendererID;

    int m_Width;
    int m_Height;
    int m_Channels;
};
