cmake_minimum_required(VERSION 3.15)
project(task_1)

set(SRC_CODE queue_on_list.h producer.h consumer.h config_runner.h)

add_executable(task_1 main.cpp ${SRC_CODE})
target_link_libraries(task_1 Threads::Threads)

add_executable(task_1_run_tests tests.cpp ${SRC_CODE})
target_link_libraries(task_1_run_tests Threads::Threads)
