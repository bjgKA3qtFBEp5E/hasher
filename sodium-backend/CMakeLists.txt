project(sodium-backend)

set(HASHER_INCLUDE_DIRS ../hasher/src)

include_directories(${HASHER_INCLUDE_DIRS})

set(SODIUM_BACKEND_SRC_FILES src/hash_backend.c)
set(LIBSODIUM_PREFIX "" CACHE STRING "Local prefix to prebuilt sodium")

if(NOT LIBSODIUM_PREFIX)
    include(ExternalProject)

    set(LIBSODIUM_NAME libsodium)
    set(LIBSODIUM_VERSION "1.0.17")
    set(LIBSODIUM_NAME_VERSION ${LIBSODIUM_NAME}-${LIBSODIUM_VERSION})
    set(LIBSODIUM_URL https://download.libsodium.org/libsodium/releases/${LIBSODIUM_NAME_VERSION}.tar.gz)
    set(LIBSODIUM_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/deps/${LIBSODIUM_NAME_VERSION}")

    set(LIBSODIUM_CONFIGURE_COMMAND ./configure --disable-shared --prefix ${LIBSODIUM_PREFIX})

    if(ANDROID_TOOLCHAIN)

        if(ANDROID_ABI STREQUAL arm)
            set(EABI eabi)
        endif()

        set(LIBSODIUM_TOOLCHAIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/toolchain)

        string(REPLACE "-" "" MAKE_ARCH ${CMAKE_SYSTEM_PROCESSOR})

        set(SODIUM_CC ${LIBSODIUM_TOOLCHAIN_DIR}/bin/${MAKE_ARCH}-linux-android${EABI}-clang)

        # TODO --host is not proper
        set(LIBSODIUM_CONFIGURE_COMMAND CC=${SODIUM_CC} ${LIBSODIUM_CONFIGURE_COMMAND} --host ${ANDROID_ABI}-linux-androideabi)

        set(ANDROID_TOOLCHAIN_CONFIGURE_COMMAND ${ANDROID_NDK}/build/tools/make_standalone_toolchain.py --arch ${ANDROID_ABI} --api ${ANDROID_PLATFORM_LEVEL} --install-dir ${LIBSODIUM_TOOLCHAIN_DIR})
    endif()

    ExternalProject_Add(LibSodiumDownload
      URL ${LIBSODIUM_URL}
      UPDATE_COMMAND ""
      PREFIX ${LIBSODIUM_PREFIX}
      BUILD_IN_SOURCE 1
      CONFIGURE_COMMAND ${ANDROID_TOOLCHAIN_CONFIGURE_COMMAND} COMMAND ${LIBSODIUM_CONFIGURE_COMMAND}
      INSTALL_COMMAND make install
    )
endif()

SET(LIBSODIUM_INCLUDE_DIR ${LIBSODIUM_PREFIX}/include)
SET(LIBSODIUM_LIB_DIR ${LIBSODIUM_PREFIX}/lib)

include_directories(${LIBSODIUM_INCLUDE_DIR})

add_library(sodium-backend STATIC ${SODIUM_BACKEND_SRC_FILES})
add_library(libsodium STATIC IMPORTED)

set_target_properties(libsodium PROPERTIES IMPORTED_LOCATION ${LIBSODIUM_LIB_DIR}/libsodium.a)

add_dependencies(sodium-backend libsodium)

target_link_libraries(sodium-backend PRIVATE libsodium)


# test config

include_directories(${HASHER_TEST_FW})
add_executable(hash_backend_test test/hash_backend_test.c)
target_link_libraries(hash_backend_test ${BACKEND}-backend)

add_test(SodiumBackendTests hash_backend_test)
