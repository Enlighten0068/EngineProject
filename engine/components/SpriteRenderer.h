#pragma once

#include "graphics/Texture2D.h"
#include <memory>

namespace Components{

    /**
     * @brief Associates a texture with an entity for 2D rendering.
     *
     * Entities with this component will be rendered as a sprite using the assigned texture.
     * The RenderSystem reads this component along with Transform to draw the entity.
     *
     * @note The texture is stored as a shared_ptr, allowing multiple entities to share
     *       the same texture (efficient for tilemaps or repeated sprites).
     * @see Transform, RenderSystem
     */
    struct SpriteRenderer{
        std::shared_ptr<Texture2D> Texture; //Texture to render for this entity
    };
}
