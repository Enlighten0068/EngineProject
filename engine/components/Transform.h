#pragma once

#include "math/Vector3D.h"

namespace Components{

    /**
     * @brief Defines the position, rotation, and scale of an entity.
     *
     * This is the most fundamental component for any entity that exists in the world.
     * It defines where the entity is located, how it is oriented, and its size.
     *
     * @note Rotation is stored in radians for mathematical consistency.
     * @see SpriteRenderer, PhysicsBody, RenderSystem
     */
    struct Transform{
        Vector3D Position{0.0f, 0.0f, 0.0f}; //World position in 3D space, if 2D Z is depth
        Vector3D Rotation{0.0f, 0.0f, 0.0f}; //Tait-Bryan angles in radians
        Vector3D Scale{1.0f, 1.0f, 1.0f}; //Scaling
    };
}
