#include "demo/levels/Levels.h"
#include "level/LevelMacros.h"

void BuildLevel1(LevelSystem& level){
    level.SetWorldBounds(-20.0f, 20.0f, -10.0f, 10.0f);

    //Player spawn
    level.CreatePlayer(0.0f, 0.0f);

    //Walls, floor, ceiling
    level.CreatePlatform(-19.5f,  0.0f, 1.0f, 20.0f, 1.0f, 20.0f);
    level.CreatePlatform( 19.5f,  0.0f, 1.0f, 20.0f, 1.0f, 20.0f);
    level.CreatePlatform(  0.0f, -9.5f, 39.0f, 1.0f, 39.0f, 1.0f);
    level.CreatePlatform(  0.0f,  9.5f, 39.0f, 1.0f, 39.0f, 1.0f);

    //Floating platforms
    level.CreatePlatform(-8.0f, -3.0f, 4.0f, 1.0f);
    level.CreatePlatform( 8.0f, -3.0f, 4.0f, 1.0f);
    level.CreatePlatform( 0.0f,  2.0f, 8.0f, 1.0f);

    //Enemies
    level.CreateEnemy(-12.0f, -8.0f, -15.0f, -10.0f, 2.0f);
    level.CreateEnemy( -8.0f, -2.0f, -10.0f,  -6.0f, 1.5f);
    level.CreateEnemy(  0.0f,  3.0f,  -2.0f,   2.0f, 1.8f);
}

REGISTER_LEVEL(Level1, "Level 1", "Simple platformer", CameraMode::Fixed, BuildLevel1);
