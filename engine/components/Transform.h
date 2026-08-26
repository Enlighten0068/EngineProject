#pragma once

#include "math/Vector3D.h"

namespace Components {
    struct Transform {
        Vector3D Position{0.0f, 0.0f, 0.0f};
        Vector3D Rotation{0.0f, 0.0f, 0.0f};
        Vector3D Scale{1.0f, 1.0f, 1.0f};
    };
}
