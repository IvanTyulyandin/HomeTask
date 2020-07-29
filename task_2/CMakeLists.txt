cmake_minimum_required(VERSION 3.15)
project(task_2)

set(SRC_CODE Task.h Work_stealing_queue.h Thread_joiner.h Thread_pool.h Threadsafe_queue.h)

add_executable(task_2_run_tests thread_pool_tests.cpp ${SRC_CODE})
target_link_libraries(task_2_run_tests Threads::Threads)
