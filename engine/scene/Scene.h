#pragma once

#include <string>

class Scene{
public:
    virtual ~Scene() = default;
    virtual void OnEnter() = 0;
    virtual void OnExit() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    virtual std::string GetName() const = 0;
};
