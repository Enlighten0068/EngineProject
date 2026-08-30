#include "core/CameraController.h"
#include "core/Input.h"
#include "core/Time.h"
#include "diagnostics/Log.h"
#include "events/EventBus.h"
#include "events/MouseEvent.h"

CameraController::CameraController(Camera2D& camera) : m_Camera(camera){
    EventBus::GetInstance().Subscribe<MouseWheelEvent>([this](Event& e){
        MouseWheelEvent& wheel = static_cast<MouseWheelEvent&>(e);
        float zoom = m_Camera.GetZoom() + wheel.GetDelta().y * 0.1f;
        if (zoom > 0.1f){
            m_Camera.SetZoom(zoom);
            //Log::Info(std::format("Zoom: {}", zoom));
        }
        m_Camera.Update();
    });
}

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
    m_Camera.Update();
}
