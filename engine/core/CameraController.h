#pragma once

#include "math/Vector3D.h"
#include "scene/Camera2D.h"
#include "scene/GameWorld.h"

/**
 * @brief Controls a 2D camera with follow and zoom capabilities.
 * @note The camera position is smoothly interpolated (lerp) when in follow mode.
 * @see Camera2D, GameWorld
 */
class CameraController{
public:

    /**
     * @brief Constructs a CameraController.
     * @param camera Reference to the Camera2D instance to control.
     * @param world Reference to the GameWorld for boundary clamping.
     */
    CameraController(Camera2D& camera, const GameWorld& world);
    ~CameraController() = default;

    /**
     * @brief Updates the camera position and zoom.
     * @param deltaTime Time elapsed since the last frame.
     */
    void Update(float deltaTime);

    //Follow mode
    void SetFollowEntity(bool follow){ m_FollowEntity = follow; }
    bool IsFollowing() const{ return m_FollowEntity; }

    //Speed
    void SetSpeed(float speed){ m_Speed = speed; }
    void SetZoomSpeed(float speed){ m_ZoomSpeed = speed; }

    void SetTargetPosition(const Vector3D& target){ m_TargetPosition = target; }

    //Zoom limits
    void SetMinZoom(float minZoom) { m_MinZoom = minZoom; }
    void SetMaxZoom(float maxZoom) { m_MaxZoom = maxZoom; }

private:
    Camera2D& m_Camera;
    const GameWorld& m_World;
    bool m_FollowEntity = false;
    float m_Speed = 5.0f;
    float m_MinZoom = 0.5f;
    float m_MaxZoom = 5.0f;
    float m_ZoomSpeed = 1.0f;
    Vector3D m_TargetPosition{0.0f, 0.0f, 0.0f};
};
