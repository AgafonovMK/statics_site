#include "aligned_allocator.h"
#include "log_buffer.h"
#include "log_writer.h"
#include <thread>

int main() {
    constexpr std::size_t pool_size = 1024 * 1024; // 1MB
    constexpr std::size_t alignment = 64;

    AlignedAllocator allocator(pool_size, alignment);
    LogBuffer buffer(1024, allocator);
    LogWriter writer(buffer, "log.txt");

    std::thread producer([&buffer]() {
        for (int i = 0; i < 1000; ++i) {
            buffer.log(LogLevel::INFO, "Log message from producer thread");
        }
    });

    producer.join();
    writer.shutdown();
    return 0;
}