project(hasher)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(HASHER_SRC_FILES src/hasher.c src/hasher_cmdclient.c)
set(HASHER_INCLUDE_DIRS src)

include_directories(${HASHER_INCLUDE_DIRS})
add_executable(hasher ${HASHER_SRC_FILES})
target_link_libraries(hasher ${BACKEND}-backend)

# test config

include_directories(${HASHER_TEST_FW})
add_executable(hasher_cmdclient_test test/hasher_cmdclient_test.c src/hasher_cmdclient.c)

add_test(HasherCmdClientTests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hasher_cmdclient_test)
