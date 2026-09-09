#pragma once

#include "math/Vector3D.h"
#include "scene/Camera2D.h"
#include "scene/GameWorld.h"

/**
 * @brief Controls a 2D camera with follow and zoom capabilities.
 *
 * The CameraController manages camera movement in two modes:
 * - Follow mode: smoothly tracks a target entity (player)
 * - Manual mode: controlled by WASD keys
 * It also handles zoom via mouse scroll and clamps the camera to world boundaries.
 *
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

    //Follow mode control
    void SetFollowEntity(bool follow){ m_FollowEntity = follow; }
    bool IsFollowing() const{ return m_FollowEntity; }

    //Speed control
    void SetSpeed(float speed){ m_Speed = speed; }
    void SetZoomSpeed(float speed){ m_ZoomSpeed = speed; }

    //Target position
    void SetTargetPosition(const Vector3D& target){ m_TargetPosition = target; }

    //Zoom limits
    void SetMinZoom(float minZoom){ m_MinZoom = minZoom; }
    void SetMaxZoom(float maxZoom){ m_MaxZoom = maxZoom; }

private:
    Camera2D& m_Camera; //Camera to control
    const GameWorld& m_World; //World for boundary clamping
    bool m_FollowEntity = false; //Whether to follow a target
    float m_Speed = 5.0f; //Movement speed in manual mode
    float m_MinZoom = 0.5f; //Minimum zoom level (zoom out limit)
    float m_MaxZoom = 5.0f; //Maximum zoom level (zoom in limit)
    float m_ZoomSpeed = 1.0f; //Zoom adjustment speed
    Vector3D m_TargetPosition{0.0f, 0.0f, 0.0f}; //Position to follow
};
