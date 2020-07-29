#include <queue>
#include <mutex>

template <typename T>
class Threadsafe_queue {
public:

    Threadsafe_queue() = default;

    Threadsafe_queue(const Threadsafe_queue& other) = delete;

    Threadsafe_queue& operator=(
            const Threadsafe_queue& other) = delete;

    void push(T data) {
        std::lock_guard<std::mutex> lock(the_mutex);
        the_queue.push(std::move(data));
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(the_mutex);
        return the_queue.empty();
    }

    bool try_pop(T& res) {
        std::lock_guard<std::mutex> lock(the_mutex);
        if (the_queue.empty()) {
            return false;
        }
        res = std::move(the_queue.front());
        the_queue.pop();
        return true;
    }

private:
    std::queue<T> the_queue;
    mutable std::mutex the_mutex;
};
