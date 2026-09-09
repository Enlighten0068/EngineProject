#include "core/FpsCounter.h"
#include "core/Time.h"
#include "diagnostics/Log.h"
#include <format>

FpsCounter::FpsCounter() = default;

/**
 * @brief Updates the FPS counter and logs when interval elapses.
 *
 * This method should be called once per frame. It accumulates delta time
 * and frame count, and when the interval is reached,
 * it calculates the average FPS and logs it.
 */
void FpsCounter::Update(){
    //Store current frame timing
    m_DeltaTime = Time::DeltaTime();
    m_ElapsedTime = Time::ElapsedTime();

    //Accumulate time and frame count
    m_Timer += m_DeltaTime;
    m_FrameCount++;

    //Log FPS when interval is reached
    if(m_Timer >= m_LogInterval){
        m_FPS = static_cast<float>(m_FrameCount) / m_Timer;
        Log::Info(std::format("FPS: {:.1f} | Delta: {:.4f}s | Elapsed: {:.2f}s",
                              m_FPS, m_DeltaTime, m_ElapsedTime));

        //Reset counters
        m_Timer = 0.0f;
        m_FrameCount = 0;
    }
}
