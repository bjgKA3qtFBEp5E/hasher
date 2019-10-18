cmake_minimum_required(VERSION 3.2)
project(dummy-backend)

set(HASHER_INCLUDE_DIRS ../hasher/src)

include_directories(${HASHER_INCLUDE_DIRS})

set(DUMMY_BACKEND_SRC_FILES src/hash_backend.c)

add_library(dummy-backend STATIC ${DUMMY_BACKEND_SRC_FILES})

add_subdirectory(test)
