cmake_minimum_required(VERSION 3.2)
project(hasher)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(HASHER_SRC_FILES src/hasher.c)
set(HASHER_INCLUDE_DIRS src)

add_executable(hasher ${HASHER_SRC_FILES})
target_link_libraries(hasher ${BACKEND}-backend)
