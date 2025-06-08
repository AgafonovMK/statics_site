#pragma once

#include "log_entry.h"
#include "lock_free_queue.h"
#include "aligned_allocator.h"

#include <mutex>
#include <stdexcept>

class LogBuffer {
public:
    LogBuffer(std::size_t capacity, AlignedAllocator& allocator)
        : allocator(allocator), queue(capacity, allocator) {}

    bool log(LogLevel level, const char* message) {
        std::lock_guard<std::mutex> guard(mutex); // сериализуем аллокации
        void* mem = nullptr;

        try {
            mem = allocator.allocate(sizeof(LogEntry));
        } catch (const std::bad_alloc&) {
            return false;
        }

        LogEntry* entry = new (mem) LogEntry(level, message);
        return queue.enqueue(entry);
    }

    LogEntry* consume() {
        return queue.dequeue();
    }

private:
    AlignedAllocator& allocator;
    LockFreeQueue<LogEntry> queue;
    std::mutex mutex; // сериализуем allocate, т.к. аллокатор не thread-safe
};