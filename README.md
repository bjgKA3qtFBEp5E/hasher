# Hasher

## Architecture

- The project is designed in a way that provides flexibility in the hash function
implementation.
- A "backend" provides an implementation for the hashing function.
- The selection of the backend is determined during build using build options.
- The selected backend is the one going to be linked, other backends will not
be compiled.

## Limitations

- Primitive argument handling, at the moment it can't hash
--version.
- Backend version would be nice
- No auto findsodium
- Everything is statically linked, could also use shared lib
- Variable array limits C standard, needed because backend hashlen

## Build

The project is based in cmake and the following options are available:

- `USE_SODIUM` instructs the project to use sodium backend. If not specified will use dummy backend.
- `LIBSODIUM_PREFIX` specify prefix to use prebuilt libsodium and header from. If not specified will download and build libsodium if required.

### Build local host
```
mkdir build
cd build
cmake .. -DUSE_SODIUM=yes
```

### Build for Android
```
mkdir build
cd build
cmake .. \
-DUSE_SODIUM=yes \
-DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
-DANDROID_ABI=$ABI \
-DANDROID_PLATFORM=$MINSDKVERSION
```

- Replace `$NDK` or set it to local path to Android NDK.
- Replace `$ABI` or set it to target ABI [arm, x86]
- Replace `$MINSDKVERION` or set it to desired value
