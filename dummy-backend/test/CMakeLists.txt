cmake_minimum_required(VERSION 3.2)
project(hasher_tests)

enable_testing()

add_test(TestStuff hash_backend_test)
add_test(TestStuff2 hash_backend_test)

set(HASHER_INCLUDE_DIRS ../../hasher/src)
include_directories(${HASHER_INCLUDE_DIRS})

add_executable(hash_backend_test hash_backend_test.c)
target_link_libraries(hash_backend_test ${BACKEND}-backend)
