#include <deque>
#include <mutex>

#include "Task.h"

class Work_stealing_queue {
public:

    Work_stealing_queue() = default;

    Work_stealing_queue(const Work_stealing_queue& other) = delete;

    Work_stealing_queue& operator=(
            const Work_stealing_queue& other) = delete;

    void push(Task_ptr data) {
        std::lock_guard<std::mutex> lock(the_mutex);
        the_queue.push_front(std::move(data));
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(the_mutex);
        return the_queue.empty();
    }

    bool try_pop(Task_ptr& res) {
        std::lock_guard<std::mutex> lock(the_mutex);
        if (the_queue.empty()) {
            return false;
        }
        res = std::move(the_queue.front());
        the_queue.pop_front();
        return true;
    }

    bool try_steal(Task_ptr& res) {
        std::lock_guard<std::mutex> lock(the_mutex);
        if (the_queue.empty()) {
            return false;
        }
        res = std::move(the_queue.back());
        the_queue.pop_back();
        return true;
    }

private:
    std::deque<Task_ptr> the_queue;
    mutable std::mutex the_mutex;
};
