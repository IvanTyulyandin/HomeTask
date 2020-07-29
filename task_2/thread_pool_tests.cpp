#include <cassert>
#include <iostream>

#include "Thread_pool.h"

template <typename O>
Init_task make_init_task(std::function<O()> func) {
     return std::function(
         [func]() {
             return std::any{func()};
         });
}

template <typename I, typename O>
Continuation_task make_continuation_task(std::function<O(I)> func) {
    return std::function(
        [func](std::any i) {
            auto inp = std::any_cast<I>(i);
            return std::any{func(inp)};
        });
}


int main() {
    auto return_zero = make_init_task(std::function([](){return 0;}));
    auto add_one = make_continuation_task(std::function([](int a){return a + 1;}));

    auto return_hello = make_init_task(std::function([](){return std::string("hello");}));
    auto append_world = make_continuation_task(
            std::function([](const std::string& s){return s + ", world!";}));

    auto return_four = make_init_task(std::function([](){return 4;}));
    auto return_five = make_init_task(std::function([](){return 5;}));
    std::function<int(int)> factorial = [&factorial](int n) {
        if (n > 0) {
            return n * factorial(n - 1);
        } else {
            return 1;
        }
    };
    auto factorial_cont = make_continuation_task(factorial);



    // Test task submission
    {
        auto pool = Thread_pool(2);

        auto str_task_ptr = std::make_shared<Task>(return_hello);
        str_task_ptr->continue_with(append_world);
        pool.submit(str_task_ptr);
        assert(std::any_cast<std::string>(str_task_ptr->result()) == std::string("hello, world!"));
    }

    // Test if amount of tasks is greater than threads in pool
    {
        auto pool = Thread_pool(2);

        auto int_task_ptr = std::make_shared<Task>(return_zero);
        int_task_ptr->continue_with(add_one).continue_with(add_one);
        pool.submit(int_task_ptr);

        auto str_task_ptr = std::make_shared<Task>(return_hello);
        str_task_ptr->continue_with(append_world);
        pool.submit(str_task_ptr);

        auto factorial_5_ptr = std::make_shared<Task>(return_five);
        factorial_5_ptr->continue_with(factorial_cont);
        pool.submit(factorial_5_ptr);

        auto factorial_4_ptr = std::make_shared<Task>(return_four);
        factorial_4_ptr->continue_with(factorial_cont);
        pool.submit(factorial_4_ptr);

        assert(std::any_cast<int>(int_task_ptr->result()) == 2);
        assert(std::any_cast<std::string>(str_task_ptr->result()) == std::string("hello, world!"));
        assert(std::any_cast<int>(factorial_5_ptr->result()) == 120);
        assert(std::any_cast<int>(factorial_4_ptr->result()) == 24);

        auto multiplier_by_factorial_5 = make_continuation_task(std::function(
                [&factorial_5_ptr](int x) {
                    return x * std::any_cast<int>(factorial_5_ptr->result());
                }));

        factorial_4_ptr->continue_with(multiplier_by_factorial_5);
        pool.submit(factorial_4_ptr);
        assert(std::any_cast<int>(factorial_4_ptr->result()) == 24 * 120);
    }
    std::cout << "If did not failed with assert, than all tests are passed.\n";
    return 0;
}
