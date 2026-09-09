#pragma once

#include "events/Event.h"

/**
 * @brief Utility class for dispatching events to specific handlers.
 *
 * EventDispatcher allows type-safe event handling by checking the event type
 * and casting to the appropriate derived class before invoking the callback.
 *
 * @note This is a lightweight wrapper; for global event handling, use EventBus.
 * @see EventBus
 */
class EventDispatcher{
public:
    /**
     * @brief Constructs an EventDispatcher for a specific event.
     * @param event The event to dispatch.
     */
    EventDispatcher(Event& event) : m_Event(event){}

    /**
     * @brief Dispatches the event to a handler if the type matches.
     *
     * If the event type matches T::GetStaticType(), the callback is invoked
     * with the event cast to T&. The Handled flag is set to the callback's return value.
     *
     * @tparam T The event type to handle.
     * @tparam F The callback type.
     * @param function The callback to invoke if the type matches.
     * @return true if the type matched and the callback was invoked, false otherwise.
     */
    template<typename T, typename F>
    bool Dispatch(const F& function){
        if(m_Event.GetEventType() == T::GetStaticType()){
            m_Event.Handled = function(static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }

private:
    Event& m_Event; //Reference to the event being dispatched
};
