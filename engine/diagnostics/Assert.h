#pragma once

#include "diagnostics/Log.h"
#include <cstdlib>

/**
 * @def ENGINE_ASSERT(condition, message)
 * @brief Asserts a condition and aborts the program if it fails.
 *
 * If the condition evaluates to false, logs an error message with the file name
 * and line number, then terminates the program with std::abort().
 *
 * @param condition The condition to evaluate.
 * @param message The error message to log if the condition fails.
 *
 * @note Should be used only in debug builds.
 * @see Log::Error
 */
#define ENGINE_ASSERT(condition, message) \
do{ \
    if(!(condition)){ \
        Log::Error(std::format("Assertion Failed: {}\nFile: {}\n Line: {}", message, __FILE__, __LINE__)); \
        std::abort(); \
    } \
} while(false)
