cmake_minimum_required(VERSION 3.13.4)
project (Hasher)

option(USE_DUMMY "Use Dummy" ON)
option(USE_SODIUM "Use Sodium" OFF)

enable_testing()

if(USE_SODIUM)
    set(BACKEND sodium)
elseif(USE_DUMMY)
    set(BACKEND dummy)
endif()

set(HASHER_TEST_FW ../test)

add_subdirectory(${BACKEND}-backend)
add_subdirectory(hasher)
add_subdirectory(test)
