#pragma once

#include <string>
#include <fstream>
#include <chrono>
#include <mutex>
#include <iomanip>
#include <sstream>
#include <unordered_map>

enum class LogLevel{
    Debug,
    Info,
    Warning,
    Error
};

class Log{
public:
    static void Initialize(const std::string& logDir = "logs/");
    static void Shutdown();

    static void Info(const std::string& message);
    static void Warning(const std::string& message);
    static void Error(const std::string& message);
    static void InfoThrottled(const std::string& message, const std::string& key, float intervalSeconds = 2.0f);

    static void SetMinLevel(LogLevel level) { s_MinLevel = level; }

private:
    static std::ofstream s_File;
    static LogLevel s_MinLevel;
    static bool s_Initialized;

    static void Write(LogLevel level, const std::string& message);
    static std::string GenerateTimestampFilename(const std::string& logDir);
    static const char* LevelToString(LogLevel level);

    static std::unordered_map<std::string, float> s_LastLogTime;
};
