#include "core/Application.h"
#include "core/Time.h"
#include <SDL3/SDL.h>


float Time::s_DeltaTime = 0.0f;
float Time::s_ElapsedTime = 0.0f;

/**
 * @brief Updates delta time and elapsed time.
 *
 * Uses SDL's counters to measure time accurately.
 * Should be called once per frame at the start of the frame.
 */
void  Time::Update(){
    static Uint64 previousCounter = SDL_GetPerformanceCounter();
    Uint64 currentCounter = SDL_GetPerformanceCounter();
    Uint64 frequency = SDL_GetPerformanceFrequency();

    s_DeltaTime = static_cast<float>(currentCounter - previousCounter) / static_cast<float>(frequency);
    s_ElapsedTime += s_DeltaTime;

    previousCounter = currentCounter;
}

/**
 * @brief Returns the delta time for the current frame.
 * @return Delta time in seconds.
 */
float Time::DeltaTime(){
    return s_DeltaTime;
}

/**
 * @brief Returns the total elapsed time.
 * @return Elapsed time in seconds.
 */
float Time::ElapsedTime(){
    return s_ElapsedTime;
}
