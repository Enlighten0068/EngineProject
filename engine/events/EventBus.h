#pragma once

#include "events/Event.h"
#include <unordered_map>
#include <vector>
#include <functional>
#include <typeindex>

/**
 * @brief Singleton event bus for global event dispatching.
 *
 * The EventBus allows any part of the engine to subscribe to events
 * and be notified when they occur. It uses type-based subscription
 * (via std::type_index) for type-safe event handling.
 *
 * @note This is a singleton; use GetInstance() to access it.
 * @see Event, EventDispatcher
 */
class EventBus{
public:
    using Callback = std::function<void(Event&)>; //Callback type for event handlers

    /**
     * @brief Gets the singleton instance of the EventBus.
     * @return Reference to the EventBus instance.
     */
    static EventBus& GetInstance();

    /**
     * @brief Subscribes a callback to a specific event type.
     *
     * The callback will be invoked whenever an event of type T is dispatched.
     * Multiple callbacks can be subscribed to the same event type.
     *
     * @tparam T The event type to subscribe to (must inherit from Event).
     * @param callback Function to call when the event is dispatched.
     */
    template<typename T>
    void Subscribe(const Callback& callback){
        std::type_index typeIndex = std::type_index(typeid(T));
        m_Subscribers[typeIndex].push_back(callback);
    }

    /**
     * @brief Dispatches an event to all subscribed callbacks.
     *
     * All callbacks subscribed to the event's type will be invoked
     * in the order they were registered.
     *
     * @param event The event to dispatch.
     */
    void Dispatch(Event& event){
        std::type_index typeIndex = std::type_index(typeid(event));
        auto it = m_Subscribers.find(typeIndex);
        if(it != m_Subscribers.end()){
            for(auto& callback : it->second){
                callback(event);
            }
        }
    }

    /**
     * @brief Clears all subscriptions.
     *
     * Removes all registered callbacks. Should be used during shutdown.
     */
    void Clear(){
        m_Subscribers.clear();
    }

private:
    EventBus() = default;
    ~EventBus() = default;
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    std::unordered_map<std::type_index, std::vector<Callback>> m_Subscribers; //Map of event types to callbacks
};
