#pragma once

/**
 * @brief Camera behavior for a level.
 */
enum class CameraMode{
    Fixed,   //Camera shows the entire world at once
    Follow   //Camera follows the player, keeping vertical extent fixed
};
