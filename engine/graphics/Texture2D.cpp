#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "graphics/Texture2D.h"
#include "diagnostics/Log.h"
#include <glad/glad.h>
#include <format>
#include <cstdint>

Texture2D::Texture2D() : m_RendererID(0),m_Width(0),m_Height(0),m_Channels(0){
}

Texture2D::~Texture2D(){
    if (m_RendererID) glDeleteTextures(1,&m_RendererID);

}

bool Texture2D::Load(const std::string& filepath){
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filepath.c_str(),&m_Width,&m_Height,&m_Channels,0);

    if (!data){
        Log::Error(std::format("Failed loading texture: {}",filepath));
        return false;
    }

    glGenTextures(1,&m_RendererID);
    glBindTexture(GL_TEXTURE_2D,m_RendererID);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    GLenum format = GL_RGB;

    if (m_Channels == 4) format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D,0,format,m_Width,m_Height,0,format,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    Log::Info(std::format("Texture loaded: {} ({}x{})",filepath,m_Width,m_Height));

    return true;
}

void Texture2D::Bind(uint32_t slot) const{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D,m_RendererID);
}

void Texture2D::Unbind() const{
    glBindTexture(GL_TEXTURE_2D,0);
}

int Texture2D::GetWidth() const{
    return m_Width;
}

int Texture2D::GetHeight() const{
    return m_Height;
}
