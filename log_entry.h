/**
 * @enum LogLevel
 * @brief Represents the severity level of a log message.
 */
enum class LogLevel {
    INFO,    ///< Informational message
    WARNING, ///< Warning condition
    ERROR    ///< Error condition
};

/**
 * @struct LogEntry
 * @brief Represents a single log message with timestamp, thread ID, level, and message content.
 *
 * This struct is 64-byte aligned to optimize for cache line usage and reduce false sharing.
 */
struct alignas(64) LogEntry {
    std::chrono::steady_clock::time_point timestamp; ///< Timestamp of log creation.
    std::thread::id thread_id;                       ///< Thread ID where log was generated.
    LogLevel level;                                  ///< Severity level.
    char message[256];                               ///< Log message (null-terminated).

    /**
     * @brief Default constructor.
     */
    LogEntry() = default;

    /**
     * @brief Constructs a new log entry with the given level and message.
     * @param lvl Log severity level.
     * @param msg Null-terminated message string. Truncated to 255 characters if longer.
     */
    LogEntry(LogLevel lvl, const char* msg);
};
