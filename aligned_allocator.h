#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <new>

class AlignedAllocator {
public:
    explicit AlignedAllocator(std::size_t size, std::size_t alignment)
        : pool_size(size), alignment(alignment) {
        void* ptr = nullptr;
        if (posix_memalign(&ptr, alignment, size) != 0) {
            throw std::bad_alloc();
        }
        base = reinterpret_cast<std::uint8_t*>(ptr);
        current = base;
    }

    void* allocate(std::size_t size) {
        std::uintptr_t curr = reinterpret_cast<std::uintptr_t>(current);
        std::uintptr_t aligned = (curr + alignment - 1) & ~(alignment - 1);
        std::uint8_t* aligned_ptr = reinterpret_cast<std::uint8_t*>(aligned);

        if (aligned_ptr + size > base + pool_size) {
            throw std::bad_alloc();
        }

        current = aligned_ptr + size;
        return aligned_ptr;
    }

    void reset() {
        current = base;
    }

    ~AlignedAllocator() {
        std::free(base);
    }

private:
    std::size_t pool_size;
    std::size_t alignment;
    std::uint8_t* base;
    std::uint8_t* current;
};
