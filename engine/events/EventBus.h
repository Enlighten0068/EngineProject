#pragma once

#include "events/Event.h"
#include <unordered_map>
#include <vector>
#include <functional>
#include <typeindex>

class EventBus{
public:
    using Callback = std::function<void(Event&)>;

    static EventBus& GetInstance();

    template<typename T>
    void Subscribe(const Callback& callback){
        std::type_index typeIndex = std::type_index(typeid(T));
        m_Subscribers[typeIndex].push_back(callback);
    }

    void Dispatch(Event& event){
        std::type_index typeIndex = std::type_index(typeid(event));
        auto it = m_Subscribers.find(typeIndex);
        if (it != m_Subscribers.end()) {
            for (auto& callback : it->second) {
                callback(event);
            }
        }
    }

    void Clear(){
        m_Subscribers.clear();
    }

private:
    EventBus() = default;
    ~EventBus() = default;
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    std::unordered_map<std::type_index, std::vector<Callback>> m_Subscribers;
};
