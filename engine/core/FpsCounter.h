#pragma once

#include <string>

/**
 * @brief Utility class for measuring and logging frames per second.
 *
 * This class calculates the average FPS over a configurable interval,
 * and logs it to the console. It also provides access to delta time
 * and elapsed time values.
 *
 * @note FPS is calculated as frame count divided by elapsed time over the logging interval.
 */
class FpsCounter{
public:
    FpsCounter();
    ~FpsCounter() = default;

    /**
     * @brief Updates the FPS counter (should be called once per frame).
     *
     * This method increments the frame count and, when the interval
     * has elapsed, calculates and logs the average FPS.
     */
    void Update();

    //Getters
    float GetFPS() const{ return m_FPS; }
    float GetDeltaTime() const{ return m_DeltaTime; }
    float GetElapsedTime() const{ return m_ElapsedTime; }

    /**
     * @brief Sets the logging interval.
     * @param interval Interval in seconds between FPS logs.
     */
    void SetLogInterval(float interval){ m_LogInterval = interval; }

private:
    float m_Timer = 0.0f; //Accumulated time since last log
    float m_FPS = 0.0f; //Calculated FPS
    int m_FrameCount = 0; //Number of frames since last log
    float m_DeltaTime = 0.0f; //Current frame delta time
    float m_ElapsedTime = 0.0f; //Total elapsed time
    float m_LogInterval = 1.0f; //Interval between logs - used usually for FPS logging
};
