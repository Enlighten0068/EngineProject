#include "core/FpsCounter.h"
#include "core/Time.h"
#include "diagnostics/Log.h"
#include <format>

FpsCounter::FpsCounter() = default;

void FpsCounter::Update(){
    m_DeltaTime = Time::DeltaTime();
    m_ElapsedTime = Time::ElapsedTime();

    m_Timer += m_DeltaTime;
    m_FrameCount++;
    if (m_Timer >= m_LogInterval){
        m_FPS = static_cast<float>(m_FrameCount) / m_Timer;
        Log::Info(std::format("FPS: {:.1f} | Delta: {:.4f}s | Elapsed: {:.2f}s",m_FPS, m_DeltaTime, m_ElapsedTime));

        m_Timer = 0.0f;
        m_FrameCount = 0;
    }
}
