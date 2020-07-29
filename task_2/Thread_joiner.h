#include <thread>
#include <vector>

// Thread_joiner is used to prevent "thread-leaking"
// from thread pool is case of exceptions

class Thread_joiner {
public:
    explicit Thread_joiner(std::vector<std::thread>& threads_)
        : threads(threads_)
    {}

    ~Thread_joiner() {
        for(auto & thread : threads) {
            if(thread.joinable())
                thread.join();
        }
    }

private:
    std::vector<std::thread>& threads;
};
