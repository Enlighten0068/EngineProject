#pragma once

class Time{
public:
    static float DeltaTime();
    static float ElapsedTime();
    static void Update();

private:
    static float s_DeltaTime;
    static float s_ElapsedTime;
};
