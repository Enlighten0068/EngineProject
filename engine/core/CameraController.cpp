#include "core/CameraController.h"
#include "core/Input.h"
#include "core/Time.h"
#include "diagnostics/Log.h"
#include "events/EventBus.h"
#include "events/MouseEvent.h"
#include <algorithm>

CameraController::CameraController(Camera2D& camera, const GameWorld& world)
: m_Camera(camera), m_World(world){}

void CameraController::Update(float deltaTime){
    if (m_FollowEntity){
        Vector3D pos = m_Camera.GetPosition();
        Vector3D diff = m_TargetPosition - pos;
        if (diff.x != 0.0f || diff.y != 0.0f){
            float lerpFactor = 0.05f;
            pos.x += diff.x * lerpFactor;
            pos.y += diff.y * lerpFactor;
            m_Camera.SetPosition(pos);
        }
    } else{
        Vector3D pos = m_Camera.GetPosition();
        float speed = m_Speed * deltaTime;
        if (Input::IsKeyHeld(SDL_SCANCODE_W)) pos.y += speed;
        if (Input::IsKeyHeld(SDL_SCANCODE_S)) pos.y -= speed;
        if (Input::IsKeyHeld(SDL_SCANCODE_A)) pos.x -= speed;
        if (Input::IsKeyHeld(SDL_SCANCODE_D)) pos.x += speed;

        m_Camera.SetPosition(pos);
    }

    Vector3D pos = m_Camera.GetPosition();
    float marginX = 5.0f;
    float marginY = 3.0f;
    pos.x = std::clamp(pos.x, m_World.GetMinX() + marginX, m_World.GetMaxX() - marginX);
    pos.y = std::clamp(pos.y, m_World.GetMinY() + marginY, m_World.GetMaxY() - marginY);
    m_Camera.SetPosition(pos);

    Vector2D scroll = Input::GetScrollDelta();
    if (scroll.y != 0.0f){
        float zoom = m_Camera.GetZoom() + scroll.y * 0.1f;
        zoom = std::clamp(zoom, m_MinZoom, m_MaxZoom);
        m_Camera.SetZoom(zoom);
    }

    m_Camera.Update();
}
