#pragma once

#include <string>
#include <fstream>
#include <chrono>
//#include <mutex>
#include <iomanip>
#include <sstream>
#include <unordered_map>

/**
 * @brief Log levels for filtering messages.
 *
 * The log level determines which messages are written to the console and log file.
 * Messages with a level below the current minimum level are ignored.
 */
enum class LogLevel{
    Debug,
    Info,
    Warning,
    Error
};

/**
 * @brief Centralized logging system with file output and throttling.
 *
 * The Log class provides static methods for logging messages to both the console
 * and a timestamped log file. It supports different logging levels, log throttling to prevent spam,
 * timestamped log files. Thread-safe file writing via mutex to be implemented later.
 *
 * @note All methods are static; no instantiation is required.
 * @note The log file is created with a timestamp in the name: log_YYYY-MM-DD_HH-MM-SS.log
 */
class Log{
public:

    /**
     * @brief Initializes the logging system.
     *
     * Creates the log directory (if it doesn't exist) and opens a new log file
     * with a timestamped filename. Should be called once at application startup.
     *
     * @param logDir Directory where log files will be stored (default: "logs/").
     */
    static void Initialize(const std::string& logDir = "logs/");

    /**
     * @brief Shuts down the logging system.
     *
     * Flushes and closes the log file. Should be called at application shutdown.
     */
    static void Shutdown();

    //Logging Methods
    static void Info(const std::string& message);
    static void Warning(const std::string& message);
    static void Error(const std::string& message);

    /**
     * @brief Logs an informational message with throttling.
     *
     * Prevents the same message from being logged too frequently.
     * The message is logged only if the specified interval has elapsed
     * since the last log with the same key.
     *
     * @param message The message to log.
     * @param key Unique identifier for this throttled message.
     * @param intervalSeconds Minimum time between logs.
     */
    static void InfoThrottled(const std::string& message, const std::string& key, float intervalSeconds = 2.0f);

    /**
     * @brief Sets the minimum log level.
     *
     * Messages with a level below this value will be ignored.
     *
     * @param level Minimum log level (default: LogLevel::Info).
     */
    static void SetMinLevel(LogLevel level){ s_MinLevel = level; }

private:
    static std::ofstream s_File;
    static LogLevel s_MinLevel;
    static bool s_Initialized;

    static void Write(LogLevel level, const std::string& message);
    static std::string GenerateTimestampFilename(const std::string& logDir);
    static const char* LevelToString(LogLevel level);

    static std::unordered_map<std::string, float> s_LastLogTime;
    //static std::mutex s_LogMutex;
};
