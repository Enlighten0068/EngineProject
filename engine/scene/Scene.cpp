#include "scene/Scene.h"
#include "renderer/Renderer.h"

Scene::Scene() = default;
Scene::~Scene() = default;

void Scene::AddGameObject(std::shared_ptr<GameObject> obj) {
    m_Objects.push_back(obj);
}

void Scene::Update(float deltaTime) {
    for (auto& obj : m_Objects) {
        obj->Update(deltaTime);
    }
}

void Scene::Render(Shader& shader, const Matrix4& view, const Matrix4& projection,
                   VertexArray& va, IndexBuffer& ib) {
    for (auto& obj : m_Objects) {

    }
}
