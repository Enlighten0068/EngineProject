#pragma once

#include "scene/GameObject.h"
#include <vector>
#include <memory>

class Scene {
public:
    Scene();
    ~Scene();

    void AddGameObject(std::shared_ptr<GameObject> obj);
    void Update(float deltaTime);
    void Render(Shader& shader, const Matrix4& view, const Matrix4& projection,
                VertexArray& va, IndexBuffer& ib);

private:
    std::vector<std::shared_ptr<GameObject>> m_Objects;
};
