#include "diagnostics/Log.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <format>
#include <filesystem>

//Static initialization
std::ofstream Log::s_File;
LogLevel Log::s_MinLevel = LogLevel::Info;
bool Log::s_Initialized = false;
std::unordered_map<std::string, float> Log::s_LastLogTime;
//std::mutex Log::s_LogMutex;

/**
 * @brief Initializes the logging system.
 *
 * Creates the log directory if it doesn't exist, generates a timestamped
 * filename, and opens the log file. The first log message written will be
 * the initialization message.
 *
 * @param logDir Directory where log files will be stored.
 */
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

/**
 * @brief Shuts down the logging system.
 *
 * Writes a shutdown message, flushes the file, and closes it.
 */
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

/**
 * @brief Logs an informational message with throttling.
 *
 * Uses a key to track the last time a message was logged. If the current
 * time minus the last log time is less than the interval, the message is skipped.
 *
 * @param message The message to log.
 * @param key Unique identifier for this throttled message.
 * @param intervalSeconds Minimum time between logs (default: 2.0 seconds).
 */
void Log::InfoThrottled(const std::string& message, const std::string& key, float intervalSeconds){
    //std::lock_guard<std::mutex> lock(s_LogMutex);
    auto now = std::chrono::steady_clock::now();
    float currentTime = std::chrono::duration<float>(now.time_since_epoch()).count();

    auto it = s_LastLogTime.find(key);
    if (it == s_LastLogTime.end() || (currentTime - it->second) >= intervalSeconds){
        s_LastLogTime[key] = currentTime;
        Write(LogLevel::Info, message);
    }
}

/**
 * @brief Internal method that writes a log message to console and file.
 *
 * Formats the message with a timestamp and log level, then writes it to:
 * - Standard output (std::cout) for Info and Warning
 * - Standard error (std::cerr) for Error
 * - The log file (if initialized and open)
 *
 * @param level Log level of the message.
 * @param message The message to write.
 */
void Log::Write(LogLevel level, const std::string& message) {
    //std::lock_guard<std::mutex> lock(s_LogMutex);
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

/**
 * @brief Converts a LogLevel enum to a string.
 * @param level The log level.
 * @return String representation of the log level.
 */
const char* Log::LevelToString(LogLevel level){
    switch (level) {
        case LogLevel::Info:    return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error:   return "ERROR";
        default:                return "UNKNOWN";
    }
}

/**
 * @brief Generates a timestamped filename for the log file.
 *
 * Format: logs/log_YYYY-MM-DD_HH-MM-SS.log
 *
 * @param logDir Directory where the log file will be stored.
 * @return Full path to the log file.
 */
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
