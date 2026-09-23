#include "demo/levels/Levels.h"
#include "level/LevelMacros.h"

void BuildLevel2(LevelSystem& level){
    level.SetWorldBounds(-20.0f, 20.0f, -10.0f, 10.0f);

    level.CreatePlayer(-15.0f, -7.0f, Vector3D(2.0f, 2.0f, 1.0f));

    //Walls
    level.CreatePlatform(-19.5f,  0.0f, 1.0f, 20.0f, 1.0f, 20.0f);
    level.CreatePlatform( 19.5f,  0.0f, 1.0f, 20.0f, 1.0f, 20.0f);
    level.CreatePlatform(  0.0f, -9.5f, 39.0f, 1.0f, 39.0f, 1.0f);
    level.CreatePlatform(  0.0f,  9.5f, 39.0f, 1.0f, 39.0f, 1.0f);

    //Staircase
    level.CreatePlatform(-12.0f, -6.0f, 4.0f, 1.0f);
    level.CreatePlatform( -4.0f, -3.0f, 4.0f, 1.0f);
    level.CreatePlatform(  4.0f,  0.0f, 4.0f, 1.0f);
    level.CreatePlatform( 12.0f,  3.0f, 4.0f, 1.0f);

    //Enemies on the stairs
    level.CreateEnemy(-14.0f, -4.0f, -13.0f, -11.0f, 1.5f, Vector3D(4.0f, 4.0f, 1.0f));
    level.CreateEnemy(-4.0f, -2.0f, -5.0f, -3.0f, 3.0f, Vector3D(1.5f, 1.5f, 1.0f));
    level.CreateEnemy(  4.0f,  1.0f,   3.0f,   5.0f, 2.0f);
    level.CreateEnemy( 12.0f,  4.0f,  11.0f,  13.0f, 2.5f);
}

REGISTER_LEVEL(Level2, "Level 2", "Staircase (follow camera)", CameraMode::Follow, BuildLevel2);
