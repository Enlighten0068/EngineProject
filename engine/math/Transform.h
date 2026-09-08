#pragma once

#include "math/Vector3D.h"

/**
 * @brief Represents a 3D transformation: position, rotation, and scale.
 *
 * This struct is used as a component in the ECS system to store the
 * spatial properties of an entity.
 *
 * @note Rotation is stored as Euler angles (pitch, yaw, roll) in radians.
 *       Usually only the roll is used in 2D games.
 * @see Matrix4::Translation, Matrix4::RotationZ, Matrix4::Scale
 */
struct Transform{
    Vector3D Position; //World position
    Vector3D Rotation; //Euler angle in radians
    Vector3D Scale; //Scale

    /**
     * @brief Default constructor: position at origin, no rotation, scale 1.
     */
    Transform() : Position(0.0f, 0.0f, 0.0f),Rotation(0.0f, 0.0f, 0.0f),Scale(1.0f, 1.0f, 1.0f){}
};
