#pragma once

/**
 * @brief Component that controls texture tiling (repetition) for an entity.
 *
 * Entities with this component will have their texture repeated according to
 * the Scale value. The texture coordinates are multiplied by this scale
 * in the vertex shader, causing the texture to tile across the surface.
 *
 * @note A value of 1.0f means no repetition (texture stretches to fill the quad).
 *       Values > 1.0f cause the texture to repeat that many times across the quad.
 * @see RenderSystem, Shader
 */
struct TileScale{
    float ScaleX= 1.0f;  //Number of texture repetitions per unit in the X-axis. 1.0 = no repetition.
    float ScaleY= 1.0f;  //Number of texture repetitions per unit in the Y-axis. 1.0 = no repetition.
};
