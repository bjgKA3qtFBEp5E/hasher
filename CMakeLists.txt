cmake_minimum_required(VERSION 3.2)
project (Hasher)

option(USE_DUMMY "Use Dummy" ON)
option(USE_SODIUM "Use Sodium" OFF)


if(USE_SODIUM)
    set(BACKEND sodium)
    add_subdirectory(sodium-backend)
elseif(USE_DUMMY)
    set(BACKEND dummy)
    add_subdirectory(dummy-backend)
endif()

add_subdirectory(hasher)
