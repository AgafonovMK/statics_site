#pragma once

#include <chrono>
#include <string>
#include <thread>
#include <cstring>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

struct alignas(64) LogEntry {
    std::chrono::steady_clock::time_point timestamp;
    std::thread::id thread_id;
    LogLevel level;
    char message[256];

    LogEntry() = default;

    LogEntry(LogLevel lvl, const char* msg)
        : timestamp(std::chrono::steady_clock::now()), thread_id(std::this_thread::get_id()), level(lvl) {
        std::strncpy(message, msg, sizeof(message));
        message[sizeof(message) - 1] = '\0';
    }
};
