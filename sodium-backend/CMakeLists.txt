cmake_minimum_required(VERSION 3.2)
project(sodium-backend)

set(HASHER_INCLUDE_DIRS ../hasher/src)

include_directories(${HASHER_INCLUDE_DIRS})

set(SODIUM_BACKEND_SRC_FILES src/hash_backend.c)

include(ExternalProject)

set(LIBSODIUM_NAME libsodium)
set(LIBSODIUM_VERSION "1.0.18")
set(LIBSODIUM_NAME_VERSION ${LIBSODIUM_NAME}-${LIBSODIUM_VERSION})
set(LIBSODIUM_URL https://download.libsodium.org/libsodium/releases/${LIBSODIUM_NAME_VERSION}.tar.gz)
set(LIBSODIUM_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/deps/${LIBSODIUM_NAME_VERSION}")

set(LIBSODIUM_CONFIGURE_COMMAND ./configure --disable-shared --prefix ${LIBSODIUM_PREFIX})

if(ANDROID_TOOLCHAIN)
    string(REPLACE "-" "" MAKE_ARCH ${CMAKE_SYSTEM_PROCESSOR})
    set(SODIUM_CC ${ANDROID_TOOLCHAIN_ROOT}/bin/${MAKE_ARCH}-linux-androideabi${ANDROID_PLATFORM_LEVEL}-clang)
    set(LIBSODIUM_CONFIGURE_COMMAND "CC=${SODIUM_CC} ${LIBSODIUM_CONFIGURE_COMMAND} --host armv7a-linux-androideabi")
endif()

message(${LIBSODIUM_CONFIGURE_COMMAND})
message("OK")
ExternalProject_Add(LibSodiumDownload
  URL ${LIBSODIUM_URL}
  UPDATE_COMMAND ""
  PREFIX ${LIBSODIUM_PREFIX}
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ${LIBSODIUM_CONFIGURE_COMMAND}
  #CONFIGURE_COMMAND ./configure --disable-shared --prefix ${LIBSODIUM_PREFIX}
  INSTALL_COMMAND make install
)

SET(LIBSODIUM_INCLUDE_DIR ${LIBSODIUM_PREFIX}/include)
SET(LIBSODIUM_LIB_DIR ${LIBSODIUM_PREFIX}/lib)

include_directories(${LIBSODIUM_INCLUDE_DIR})

add_library(sodium-backend STATIC ${SODIUM_BACKEND_SRC_FILES})
add_library(libsodium STATIC IMPORTED)

set_target_properties(libsodium PROPERTIES IMPORTED_LOCATION ${LIBSODIUM_LIB_DIR}/libsodium.a)

add_dependencies(sodium-backend libsodium)

target_link_libraries(sodium-backend PRIVATE libsodium)
