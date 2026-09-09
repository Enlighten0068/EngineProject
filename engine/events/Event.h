#pragma once

#include "events/EventType.h"

/**
 * @brief Base class for all events in the engine.
 *
 * All events derive from this class and must implement GetEventType().
 * The Handled flag can be used by event dispatchers to mark an event as processed.
 *
 * @note Events are typically dispatched via EventBus or EventDispatcher.
 * @see EventType, EventBus, EventDispatcher
 */
class Event{
public:
    virtual ~Event() = default;

    /**
     * @brief Returns the type of the event.
     * @return EventType enum value identifying the event.
     */
    virtual EventType GetEventType() const = 0;

    bool Handled = false; //Flag indicating if the event has been handled
};
