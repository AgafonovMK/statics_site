/**
 * @class LockFreeQueue
 * @brief A single-producer, single-consumer lock-free circular queue.
 *
 * Allocates memory from an external aligned allocator. Not safe for multiple producers or consumers.
 * Template parameter T must be a pointer type.
 *
 * @tparam T Type of the items to store (should be a pointer to a struct).
 */
template <typename T>
class LockFreeQueue {
public:
    /**
     * @brief Constructs the queue with a fixed capacity.
     * @param capacity Number of elements the queue can hold.
     * @param allocator Reference to an AlignedAllocator used for memory.
     */
    explicit LockFreeQueue(std::size_t capacity, AlignedAllocator& allocator);

    /**
     * @brief Adds an item to the queue.
     * @param item Pointer to the item to enqueue.
     * @return True if successfully enqueued; false if the queue is full.
     */
    bool enqueue(T* item);

    /**
     * @brief Removes and returns the next item from the queue.
     * @return Pointer to the dequeued item, or nullptr if the queue is empty.
     */
    T* dequeue();

private:
    std::size_t capacity;           ///< Max number of elements in the queue.
    AlignedAllocator& allocator;   ///< Reference to aligned memory allocator.
    T** buffer;                    ///< Internal array of pointers to elements.
    alignas(64) std::atomic<std::size_t> head; ///< Index of next item to dequeue.
    alignas(64) std::atomic<std::size_t> tail; ///< Index of next slot to enqueue.
};
