#include "events/EventBus.h"

EventBus& EventBus::GetInstance(){
    static EventBus instance;
    return instance;
}
