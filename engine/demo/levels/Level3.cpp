#include "demo/levels/Levels.h"
#include "level/LevelMacros.h"

void BuildLevel3(LevelSystem& level){
    level.SetWorldBounds(-20.0f, 20.0f, -10.0f, 10.0f);

    level.CreatePlayer(0.0f, 5.0f);

    //Arena walls
    level.CreatePlatform(-19.5f,  0.0f, 1.0f, 20.0f, 1.0f, 20.0f);
    level.CreatePlatform( 19.5f,  0.0f, 1.0f, 20.0f, 1.0f, 20.0f);
    level.CreatePlatform(  0.0f, -9.5f, 39.0f, 1.0f, 39.0f, 1.0f);
    level.CreatePlatform(  0.0f,  9.5f, 39.0f, 1.0f, 39.0f, 1.0f);

    //Central arena platforms
    level.CreatePlatform(-10.0f, -4.0f, 4.0f, 1.0f);
    level.CreatePlatform( 10.0f, -4.0f, 4.0f, 1.0f);
    level.CreatePlatform(  0.0f, -6.0f, 8.0f, 1.0f);

    //Enemies — many, to test performance
    for(int i = -3; i <= 3; ++i){
        level.CreateEnemy(static_cast<float>(i * 5), -8.0f,
                          static_cast<float>(i * 5 - 2), static_cast<float>(i * 5 + 2), 3.0f);
    }
}

REGISTER_LEVEL(Level3, "Level 3", "Arena with many enemies", CameraMode::Fixed, BuildLevel3);
