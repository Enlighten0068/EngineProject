#pragma once

#include <string>

class FpsCounter{
public:
    FpsCounter();
    ~FpsCounter() = default;

    void Update();

    float GetFPS() const{ return m_FPS; }
    float GetDeltaTime() const{ return m_DeltaTime; }
    float GetElapsedTime() const{ return m_ElapsedTime; }

    void SetLogInterval(float interval) { m_LogInterval = interval; }

private:
    float m_Timer = 0.0f;
    float m_FPS = 0.0f;
    int m_FrameCount = 0;
    float m_DeltaTime = 0.0f;
    float m_ElapsedTime = 0.0f;
    float m_LogInterval;
};
