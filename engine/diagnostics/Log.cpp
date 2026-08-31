#include "diagnostics/Log.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <format>
#include <filesystem>

std::ofstream Log::s_File;
LogLevel Log::s_MinLevel = LogLevel::Info;
bool Log::s_Initialized = false;
std::unordered_map<std::string, float> Log::s_LastLogTime;

void Log::Initialize(const std::string& logDir){
    if (s_Initialized) return;

    std::filesystem::create_directories(logDir);
    std::string filename = GenerateTimestampFilename(logDir);
    s_File.open(filename, std::ios::out | std::ios::trunc);
    if (!s_File.is_open()) {
        std::cerr << "[ERROR] Failed to open log file: " << filename << std::endl;
    } else {
        s_Initialized = true;
        Info("Log system initialized. File: " + filename);
    }
}

void Log::Shutdown(){
    if (s_File.is_open()) {
        Info("Log system shutting down.");
        s_File.close();
    }
    s_Initialized = false;
}

void Log::Info(const std::string& message){
    Write(LogLevel::Info, message);
}

void Log::Warning(const std::string& message){
    Write(LogLevel::Warning, message);
}

void Log::Error(const std::string& message){
    Write(LogLevel::Error, message);
}

void Log::InfoThrottled(const std::string& message, const std::string& key, float intervalSeconds){
    auto now = std::chrono::steady_clock::now();
    float currentTime = std::chrono::duration<float>(now.time_since_epoch()).count();

    auto it = s_LastLogTime.find(key);
    if (it == s_LastLogTime.end() || (currentTime - it->second) >= intervalSeconds){
        s_LastLogTime[key] = currentTime;
        Info(message);
    }
}

void Log::Write(LogLevel level, const std::string& message) {
    if (level < s_MinLevel) return;

    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ) % 1000;

    std::tm tm_now;
    #if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm_now, &time_t_now);
    #else
    localtime_r(&time_t_now, &tm_now);
    #endif

    std::ostringstream oss;
    oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S") << "."
    << std::setw(3) << std::setfill('0') << ms.count();

    std::string timestamp = oss.str();

    std::string line = "[" + timestamp + "] [" + LevelToString(level) + "] " + message;

    if (level == LogLevel::Error) std::cerr << line << std::endl;
    else std::cout << line << std::endl;

    if (s_Initialized && s_File.is_open()){
        s_File << line << std::endl;
        s_File.flush();
    }
}

const char* Log::LevelToString(LogLevel level){
    switch (level) {
        case LogLevel::Info:    return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error:   return "ERROR";
        default:                return "UNKNOWN";
    }
}

std::string Log::GenerateTimestampFilename(const std::string& logDir){

    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
    #if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm_now, &time_t_now);
    #else
    localtime_r(&time_t_now, &tm_now);
    #endif

    std::ostringstream oss;
    oss << logDir << "log_" << std::put_time(&tm_now, "%Y-%m-%d_%H-%M-%S") << ".log";
    return oss.str();
}
