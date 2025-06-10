/**
 * @class LogWriter
 * @brief Consumes log entries from a LogBuffer and writes them to a file in a background thread.
 *
 * Automatically starts on construction and shuts down cleanly on destruction.
 */
class LogWriter {
public:
    /**
     * @brief Constructs and starts the log writer.
     * @param buffer Reference to a LogBuffer instance.
     * @param filename Name of the log file to write to.
     * @throws std::runtime_error if the file cannot be opened.
     */
    LogWriter(LogBuffer& buffer, const std::string& filename);

    /**
     * @brief Signals the worker thread to stop and waits for it to finish.
     */
    void shutdown();

    /**
     * @brief Destructor. Automatically calls shutdown().
     */
    ~LogWriter();

private:
    /**
     * @brief Background thread function to write log entries to the file.
     */
    void run();

    /**
     * @brief Converts log level enum to human-readable string.
     * @param lvl Log level.
     * @return C-string representation of the log level.
     */
    const char* level_to_string(LogLevel lvl);

    LogBuffer& buffer;               ///< Log buffer to consume entries from.
    std::atomic<bool> shutdown_flag;///< Flag to signal shutdown to the worker thread.
    std::ofstream out;              ///< Output stream for the log file.
    std::thread worker;             ///< Worker thread that writes logs to the file.
};
