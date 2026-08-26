#pragma once

#include "graphics/Texture2D.h"
#include <memory>

namespace Components {
    struct SpriteRenderer {
        std::shared_ptr<Texture2D> Texture;
    };
}
