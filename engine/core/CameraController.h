#pragma once

#include "math/Vector3D.h"
#include "scene/Camera2D.h"
#include "scene/GameWorld.h"

class CameraController{
public:
    CameraController(Camera2D& camera, const GameWorld& world);
    ~CameraController() = default;

    void Update(float deltaTime);

    void SetFollowEntity(bool follow){ m_FollowEntity = follow; }
    bool IsFollowing() const{ return m_FollowEntity; }

    void SetSpeed(float speed){ m_Speed = speed; }
    void SetZoomSpeed(float speed){ m_ZoomSpeed = speed; }

    void SetTargetPosition(const Vector3D& target){ m_TargetPosition = target; }

private:
    Camera2D& m_Camera;
    const GameWorld& m_World;
    bool m_FollowEntity = false;
    float m_Speed = 5.0f;
    float m_ZoomSpeed = 1.0f;
    Vector3D m_TargetPosition{0.0f, 0.0f, 0.0f};
};
