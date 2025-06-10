/**
 * @class LogBuffer
 * @brief Combines memory allocation and queuing to store log entries.
 *
 * Provides a thread-safe interface for log producers. Uses a lock-free queue internally.
 */
class LogBuffer {
public:
    /**
     * @brief Constructs the log buffer.
     * @param capacity Maximum number of log entries the queue can hold.
     * @param allocator Reference to a shared memory allocator.
     */
    LogBuffer(std::size_t capacity, AlignedAllocator& allocator);

    /**
     * @brief Logs a message into the buffer.
     * @param level Log severity level.
     * @param message Null-terminated log message string.
     * @return True if the message was successfully logged, false if allocation failed.
     */
    bool log(LogLevel level, const char* message);

    /**
     * @brief Retrieves the next log entry from the buffer.
     * @return Pointer to the log entry, or nullptr if the buffer is empty.
     */
    LogEntry* consume();

private:
    AlignedAllocator& allocator;       ///< Memory allocator for log entries.
    LockFreeQueue<LogEntry> queue;     ///< Internal lock-free queue.
    std::mutex mutex;                  ///< Mutex to serialize allocator access.
};
