#pragma once

#include "level/LevelRegistry.h"

/**
 * @brief Registers a level at program startup.
 *
 * Usage: place this macro at file scope (outside any function) in a level .cpp.
 *
 * @param id Unique identifier (no quotes).
 * @param displayName Display name (quoted string).
 * @param description Short description (quoted string).
 * @param cameraMode Camera mode (CameraMode::Fixed or CameraMode::Follow).
 * @param builderFn Function that builds the level.
 *
 * @example
 * REGISTER_LEVEL(Level1, "Level 1", "Simple platformer", CameraMode::Fixed, BuildLevel1);
 */
#define REGISTER_LEVEL(id, displayName, description, cameraMode, builderFn)   \
namespace{                                                                \
    struct LevelRegistrar_##id{                                           \
        LevelRegistrar_##id(){                                            \
            LevelRegistry::GetInstance().Register({                       \
                displayName, description, builderFn, cameraMode, false    \
            });                                                           \
        }                                                                 \
    };                                                                    \
    static LevelRegistrar_##id g_LevelRegistrar_##id;                     \
}

/**
 * @brief Same as REGISTER_LEVEL but marks the level as "new" (highlighted).
 *
 * Used for levels added during a live demo or presentation.
 */
#define REGISTER_LEVEL_NEW(id, displayName, description, cameraMode, builderFn) \
namespace{                                                                \
    struct LevelRegistrar_##id{                                           \
        LevelRegistrar_##id(){                                            \
            LevelRegistry::GetInstance().Register({                       \
                displayName, description, builderFn, cameraMode, true     \
            });                                                           \
        }                                                                 \
    };                                                                    \
    static LevelRegistrar_##id g_LevelRegistrar_##id;                     \
}
