#include <any>
#include <functional>
#include <future>
#include <vector>


// Task with possible continuations.
// Task can not be copied or moved, use Task_ptr as alternative
// Continue_with returns *this rather than a new task.
// Task is completed at some point of time if all continuations are executed.
// Later user can add new continuations and resubmit to a thread pool or run it manually.

// User has to pass functions with std::any in signature.

class Task;
using Task_ptr          = std::shared_ptr<Task>;
using Init_task         = std::function<std::any()>;
using Continuation_task = std::function<std::any(std::any)>;

class Task {
public:

    static constexpr int SHOULD_CALL_INIT = -1;

    explicit Task(Init_task func)
        : f(std::move(func)),
          cur_func(-1)
    {}

    Task(const Task& other) = delete;
    Task& operator=(const Task& other) = delete;

    Task(Task&& other) = delete;
    Task& operator=(Task&& other) = delete;

    void operator()() {
        std::lock_guard lock(cont_mutex);
        if (cur_func == SHOULD_CALL_INIT) {
            res = f();
        } else {
            // here cur_func >= 0
            if (static_cast<size_t>(cur_func) == continuations.size()) {
                return;
            } else {
                try {
                    res = continuations[cur_func](res);
                } catch (const std::exception& e) {
                    std::throw_with_nested(std::runtime_error(e.what()));
                }
            }
        }
        ++cur_func;
        if (static_cast<size_t>(cur_func) == continuations.size()) {
            last_completed_res = res;
        }
    }

    Task& continue_with(Continuation_task next_f) {
        std::lock_guard lock(cont_mutex);
        continuations.emplace_back(std::move(next_f));
        return *this;
    }

    bool is_completed() const {
        std::lock_guard lock(cont_mutex);
        return (cur_func != SHOULD_CALL_INIT)
            && (static_cast<size_t>(cur_func) == continuations.size());
    }

    std::any result() const {
        while (!is_completed()) {
            std::this_thread::yield();
        }
        return last_completed_res;
    }

private:
    Init_task f;
    std::any res;
    std::any last_completed_res;
    int cur_func;
    mutable std::mutex cont_mutex;
    std::vector<Continuation_task> continuations;
};
