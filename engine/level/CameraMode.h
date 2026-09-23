#pragma once

/**
 * @brief Camera behavior for a level.
 *
 * Determines how the camera tracks the world during gameplay.
 *
 * @see BaseLevelScene, LevelDefinition
 */
enum class CameraMode{
    Fixed, //Camera shows the entire world at once (no follow)
    Follow //Camera follows the player, keeping vertical extent fixed
};
