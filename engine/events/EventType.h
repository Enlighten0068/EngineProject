#pragma once

enum class EventType{
    None = 0,

    WindowClose,
    WindowResize,
    Key,
    MouseMotion,
    MouseButton,
    MouseWheel
};
