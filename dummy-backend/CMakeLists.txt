cmake_minimum_required(VERSION 3.2)
project(dummy-backend)

include_directories(../hasher/src)

set(DUMMY_BACKEND_SRC_FILES src/hash_backend.c)

add_library(dummy-backend STATIC ${DUMMY_BACKEND_SRC_FILES})

# test config

include_directories(${HASHER_TEST_FW})
add_executable(hash_backend_test test/hash_backend_test.c)
target_link_libraries(hash_backend_test ${BACKEND}-backend)

add_test(DummyBackendTests hash_backend_test)
