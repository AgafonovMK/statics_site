#pragma once

#include "log_buffer.h"
#include <fstream>
#include <atomic>
#include <thread>
#include <sstream>

class LogWriter {
public:
    LogWriter(LogBuffer& buffer, const std::string& filename)
        : buffer(buffer), shutdown_flag(false), out(filename, std::ios::out | std::ios::app) {
        if (!out) {
            throw std::runtime_error("Failed to open log file");
        }

        worker = std::thread([this]() { this->run(); });
    }

    void shutdown() {
        shutdown_flag.store(true, std::memory_order_release);
        if (worker.joinable()) {
            worker.join();
        }
    }

    ~LogWriter() {
        shutdown();
    }

private:
    void run() {
        while (!shutdown_flag.load(std::memory_order_acquire)) {
            LogEntry* entry = buffer.consume();
            if (entry) {
                std::stringstream ss;
                ss << "[" << std::chrono::duration_cast<std::chrono::milliseconds>(
                    entry->timestamp.time_since_epoch()).count()
                   << "] ";

                ss << "[TID " << entry->thread_id << "] ";
                ss << "[" << level_to_string(entry->level) << "] ";
                ss << entry->message << "\n";

                out << ss.str();
            } else {
                std::this_thread::yield();
            }
        }
    }

    const char* level_to_string(LogLevel lvl) {
        switch (lvl) {
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }

    LogBuffer& buffer;
    std::atomic<bool> shutdown_flag;
    std::ofstream out;
    std::thread worker;
};