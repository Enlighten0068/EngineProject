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

    //Window events
    WindowClose,   //Window close requested
    WindowResize,  //Window resized

    //Input events
    Key,           //Keyboard key event
    MouseMotion,   //Mouse movement event
    MouseButton,   //Mouse button event
    MouseWheel,    //Mouse wheel event

    //Gameplay events
    PlayerJumped,  //Player jumped
    PlayerLanded,  //Player landed on surface
    PlayerDied,    //Player died
    EnemyDied      //Enemy died
};
