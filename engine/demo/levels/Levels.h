#pragma once

#include "systems/LevelSystem.h"

/**
 * @brief Builder functions for all levels.
 *
 * Each function builds a complete level by calling methods on the LevelSystem.
 * Registration is done in each .cpp via the REGISTER_LEVEL macro.
 *
 * @see REGISTER_LEVEL, LevelRegistry
 */

void BuildLevel1(LevelSystem& level);
void BuildLevel2(LevelSystem& level);
void BuildLevel3(LevelSystem& level);
//void BuildLevelNewDemo(LevelSystem& level);
