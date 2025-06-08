#pragma once

#include <atomic>
#include <cstddef>
#include <cassert>

template <typename T>
class LockFreeQueue {
public:
    explicit LockFreeQueue(std::size_t capacity, AlignedAllocator& allocator)
        : capacity(capacity), allocator(allocator), head(0), tail(0) {
        buffer = reinterpret_cast<T**>(allocator.allocate(sizeof(T*) * capacity));
        for (size_t i = 0; i < capacity; ++i) {
            buffer[i] = nullptr;
        }
    }

    bool enqueue(T* item) {
        auto current_tail = tail.load(std::memory_order_relaxed);
        auto next_tail = (current_tail + 1) % capacity;
        if (next_tail == head.load(std::memory_order_acquire)) {
            return false; // queue is full
        }

        buffer[current_tail] = item;
        tail.store(next_tail, std::memory_order_release);
        return true;
    }

    T* dequeue() {
        auto current_head = head.load(std::memory_order_relaxed);
        if (current_head == tail.load(std::memory_order_acquire)) {
            return nullptr; // queue is empty
        }

        T* item = buffer[current_head];
        head.store((current_head + 1) % capacity, std::memory_order_release);
        return item;
    }

private:
    std::size_t capacity;
    AlignedAllocator& allocator;
    T** buffer;

    alignas(64) std::atomic<std::size_t> head;
    alignas(64) std::atomic<std::size_t> tail;
};
