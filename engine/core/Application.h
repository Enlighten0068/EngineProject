#pragma once

#include "events/Event.h"
#include "graphics/Shader.h"
#include "graphics/IndexBuffer.h"
#include "graphics/Texture2D.h"
#include "graphics/VertexArray.h"
#include "graphics/VertexBuffer.h"
#include "platform/sdl/SDLWindow.h"
#include <memory>


class Application{
public:
    Application();
    ~Application();

    bool Initialize();
    void Run();

    void Shutdown();

private:
    SDLWindow m_Window;
    bool m_Running;
    void ProcessEvents();
    void Update();
    void Render();
    void OnEvent(Event& event);
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<Texture2D> m_Texture;
};
