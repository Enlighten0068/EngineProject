#pragma once

/**
 * @brief Enumeration of all event types supported by the engine.
 *
 * Each event class has a corresponding EventType value. This allows
 * fast type checking without RTTI.
 *
 * @note Add new event types here when creating new event classes.
 */
enum class EventType{
    None = 0,

    //Windows events
    WindowClose,
    WindowResize,

    //Input events
    Key,
    MouseMotion,
    MouseButton,
    MouseWheel,

    //Gameplay events
    PlayerJumped,
    PlayerLanded,
    PlayerDied,
    EnemyDied
};
