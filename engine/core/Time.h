#pragma once

/**
 * @brief Utility class for time management.
 *
 * Provides static methods to access delta time (time between frames)
 * and total elapsed time.
 *
 * @note All times are in seconds.
 */
class Time{
public:
    /**
     * @brief Gets the time elapsed since the last frame.
     * @return Delta time in seconds.
     */
    static float DeltaTime();

    /**
     * @brief Overrides the delta time for this frame.
     * @note Used to clamp delta time after long freezes.
     */
    static void SetDeltaTime(float dt){ s_DeltaTime = dt; }

    /**
     * @brief Gets the total time elapsed since application startup.
     * @return Elapsed time in seconds.
     */
    static float ElapsedTime();

    /**
     * @brief Updates the time system (should be called once per frame).
     *
     * This method updates both the delta time and elapsed time using
     * SDL performance counter.
     */
    static void Update();

private:
    static float s_DeltaTime; //Time between frames
    static float s_ElapsedTime; //Total elapsed time since startup
};
