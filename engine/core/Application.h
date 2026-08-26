#pragma once

#include "components/Transform.h"
#include "components/SpriteRenderer.h"
#include "events/Event.h"
#include "graphics/Shader.h"
#include "graphics/IndexBuffer.h"
#include "graphics/Texture2D.h"
#include "graphics/VertexArray.h"
#include "graphics/VertexBuffer.h"
#include "math/Transform.h"
#include "platform/sdl/SDLWindow.h"
#include "resources/ResourceManager.h"
#include "scene/Camera2D.h"
#include <entt.hpp>
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
    std::unique_ptr<Camera2D> m_Camera;
    entt::registry m_Registry;
    entt::entity m_TestEntity;
};
