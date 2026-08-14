#pragma once

#include "events/Event.h"
//#include "events/WindowEvent.h"

class EventDispatcher{
public:
    EventDispatcher(Event& event) : m_Event(event){}

    template<typename T, typename F>
    bool Dispatch(const F& function){
        if (m_Event.GetEventType() == T::GetStaticType()){
            m_Event.Handled =
            function(static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }

private:
    Event& m_Event;
};
