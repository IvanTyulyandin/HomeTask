#include <memory>
#include <thread>
#include <iostream>

#include "Thread_joiner.h"
#include "Threadsafe_queue.h"
#include "Work_stealing_queue.h"

class Thread_pool {
public:
    explicit Thread_pool(size_t thread_count)
        : done(false), joiner(threads)
    {
        try {
            for (size_t i = 0; i < thread_count; ++i) {
                queues.push_back(std::make_unique<Work_stealing_queue>());
                threads.emplace_back(std::thread(&Thread_pool::worker_thread, this, i));
            }
        } catch (const std::exception& e) {
            done.store(true);
            std::throw_with_nested(e);
        }
    }

    ~Thread_pool() {
        done.store(true);
    }

    void submit(Task_ptr task) {
        if (done.load()) {
            return;
        }
        if (local_work_queue) {
            local_work_queue->push(std::move(task));
        } else {
            pool_work_queue.push(std::move(task));
        }
    }

    void run_pending_task() {
        Task_ptr task;
        if (pop_task_from_local_queue(task)
            || pop_task_from_pool_queue(task)
            || pop_task_from_other_thread_queue(task))
        {
            (*task)();
            if (!(task->is_completed())) {
                submit(task);
            }
        } else {
            std::this_thread::yield();
        }
    }

private:
    void worker_thread(size_t index) {
        my_index = index;
        local_work_queue = queues[my_index].get();
        while (!done.load()) {
            run_pending_task();
        }
    }

    bool pop_task_from_local_queue(Task_ptr& task) {
        return local_work_queue && local_work_queue->try_pop(task);
    }

    bool pop_task_from_pool_queue(Task_ptr& task) {
        return pool_work_queue.try_pop(task);
    }

    bool pop_task_from_other_thread_queue(Task_ptr& task) {
        for (size_t i = 0; i < queues.size(); ++i) {
            auto index = (my_index + i + 1) % queues.size();
            if (queues[index]->try_steal(task)) {
                return true;
            }
        }
        return false;
    }

    std::atomic_bool done;
    Threadsafe_queue<Task_ptr> pool_work_queue;
    std::vector<std::unique_ptr<Work_stealing_queue>> queues;
    std::vector<std::thread> threads;
    Thread_joiner joiner;

    // inline is a hack to init static variables
    inline static thread_local Work_stealing_queue* local_work_queue;
    inline static thread_local size_t my_index;
};