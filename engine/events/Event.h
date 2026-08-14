#pragma once

#include "events/EventType.h"

class Event{
public:
    virtual ~Event() = default;
    virtual EventType GetEventType() const = 0;
    bool Handled = false;
};
