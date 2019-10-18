cmake_minimum_required(VERSION 3.2)
project (Hasher)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(USE_DUMMY "Use Dummy" ON)
option(USE_SODIUM "Use Sodium" OFF)


if(USE_SODIUM)
    set(BACKEND sodium)
    add_subdirectory(sodium-backend)
    #add_dependencies(hasher backend-sodium)
elseif(USE_DUMMY)
    set(BACKEND dummy)
    add_subdirectory(dummy-backend)
endif()

add_subdirectory (hasher)
