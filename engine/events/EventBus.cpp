#include "events/EventBus.h"

/**
 * @brief Returns the singleton instance of EventBus.
 *
 * The instance is created on first call and destroyed automatically
 * when the program exits.
 *
 * @return Reference to the EventBus instance.
 */
EventBus& EventBus::GetInstance(){
    static EventBus instance;
    return instance;
}
