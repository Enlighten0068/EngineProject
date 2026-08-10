#include "diagnostics/Log.h"
#include <iostream>

void Log::Info(const std::string& message){
    std::cout << "[INFO] " << message << '\n';
}

void Log::Warning(const std::string& message){
    std::cout << "[WARNING] " << message << '\n';
}

void Log::Error(const std::string& message){
    std::cerr << "[ERROR] " << message << '\n';
}
