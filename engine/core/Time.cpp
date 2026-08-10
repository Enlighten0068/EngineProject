#include "core/Application.h"
#include "core/Time.h"
#include <SDL3/SDL.h>


float Time::s_DeltaTime = 0.0f;
float Time::s_ElapsedTime = 0.0f;

void  Time::Update(){
    static Uint64 previousCounter = SDL_GetPerformanceCounter();
    Uint64 currentCounter = SDL_GetPerformanceCounter();
    Uint64 frequency = SDL_GetPerformanceFrequency();

    s_DeltaTime = static_cast<float>(currentCounter - previousCounter) / static_cast<float>(frequency);
    s_ElapsedTime += s_DeltaTime;

    previousCounter = currentCounter;
}

float Time::DeltaTime(){
    return s_DeltaTime;
}

float Time::ElapsedTime(){
    return s_ElapsedTime;
}
