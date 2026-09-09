#include "core/CameraController.h"
#include "core/Input.h"
#include "core/Time.h"
#include "diagnostics/Log.h"
#include "events/EventBus.h"
#include "events/MouseEvent.h"
#include <algorithm>

CameraController::CameraController(Camera2D& camera, const GameWorld& world)
: m_Camera(camera), m_World(world){}

/**
 * @brief Updates the camera position and zoom based on input and mode.
 *
 * In follow mode, smoothly interpolates toward the target position.
 * In manual mode, moves the camera with WASD keys.
 * Also applies zoom from mouse scroll and clamps the camera to the world boundaries.
 *
 * @param deltaTime Time elapsed since the last frame.
 */
void CameraController::Update(float deltaTime){
    //Follow Mode - smoothly interpolate to target
    if(m_FollowEntity){
        Vector3D pos = m_Camera.GetPosition();
        Vector3D diff = m_TargetPosition - pos;
        if(diff.x != 0.0f || diff.y != 0.0f){
            float lerpFactor = 0.05f;
            pos.x += diff.x * lerpFactor;
            pos.y += diff.y * lerpFactor;
            m_Camera.SetPosition(pos);
        }
    }
    //Manual Mode - WASD controls
    else{
        Vector3D pos = m_Camera.GetPosition();
        float speed = m_Speed * deltaTime;
        if(Input::IsKeyHeld(SDL_SCANCODE_W)) pos.y += speed;
        if(Input::IsKeyHeld(SDL_SCANCODE_S)) pos.y -= speed;
        if(Input::IsKeyHeld(SDL_SCANCODE_A)) pos.x -= speed;
        if(Input::IsKeyHeld(SDL_SCANCODE_D)) pos.x += speed;
        m_Camera.SetPosition(pos);
    }

    //World boundary clamping - prevents the camera from showing the void beyond the world edges
    Vector3D pos = m_Camera.GetPosition();
    float marginX = 5.0f;
    float marginY = 3.0f;

    //Clamp X axis with safety check
    float minX = m_World.GetMinX() + marginX;
    float maxX = m_World.GetMaxX() - marginX;
    if(minX < maxX) pos.x = std::clamp(pos.x, minX, maxX);

    //Clamp Y axis with safety check
    float minY = m_World.GetMinY() + marginY;
    float maxY = m_World.GetMaxY() - marginY;
    if(minY < maxY) pos.y = std::clamp(pos.y, minY, maxY);

    m_Camera.SetPosition(pos);

    //Zoom with mouse scroll
    Vector2D scroll = Input::GetScrollDelta();
    if(scroll.y != 0.0f){
        float zoom = m_Camera.GetZoom() + scroll.y * 0.1f;
        zoom = std::clamp(zoom, m_MinZoom, m_MaxZoom);
        m_Camera.SetZoom(zoom);
    }

    m_Camera.Update();
}
