/**
 * @class AlignedAllocator
 * @brief Provides memory allocation with specified alignment.
 *
 * Useful for performance-sensitive applications to ensure cache-aligned memory blocks.
 * Not thread-safe: must be externally synchronized.
 */
class AlignedAllocator {
public:
    /**
     * @brief Constructs an aligned memory allocator.
     * @param size The total size of the memory pool in bytes.
     * @param alignment The required memory alignment (e.g., 64 for cache line alignment).
     * @throws std::bad_alloc if memory allocation fails.
     */
    explicit AlignedAllocator(std::size_t size, std::size_t alignment);

    /**
     * @brief Allocates a block of memory from the internal pool.
     * @param size The size of memory to allocate in bytes.
     * @return Pointer to aligned memory block.
     * @throws std::bad_alloc if insufficient space remains in the pool.
     */
    void* allocate(std::size_t size);

    /**
     * @brief Resets the allocator to reuse the memory pool from the beginning.
     * All previously allocated memory is considered invalid after this call.
     */
    void reset();

    /**
     * @brief Destructor. Frees the allocated memory pool.
     */
    ~AlignedAllocator();

private:
    std::size_t pool_size;      ///< Total size of the memory pool.
    std::size_t alignment;      ///< Required memory alignment.
    std::uint8_t* base;         ///< Pointer to the start of the pool.
    std::uint8_t* current;      ///< Pointer to the current allocation position.
};
