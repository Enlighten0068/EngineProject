#pragma once

#include "diagnostics/Log.h"
#include <cstdlib>

#define ENGINE_ASSERT(condition, message) \
    do{ \
        if(!(condition)){ \
            Log::Error(std::format("Assertion Failed: {}\nFile: {}\n Line: {}", message, __FILE__, __LINE__)); \
            std::abort(); \
        } \
    } while(false)
