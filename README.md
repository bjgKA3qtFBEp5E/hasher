# Hasher

## Architecture

- The project is designed in a way that provides flexibility in the hash function
implementation.
- A ```HashBackend``` provides an implementation for the hashing function.
- The selection of the backend is determined during build using build options.
- The selected backend is the one going to be linked, other backends will not
be built.

## Limitations and potential improvements

### Primitive argument handling

Arguments handling is done in a very basic way. Hasher is not able to hash any
input that is also a name of an accepted hasher option. Because `--version` is
a valid Hasher argument that prints the program version, this specific value
is not hashable.

### Backend versions

The argument `--version` only prints Hasher version and which backend is being
used. It would be useful if backends are able to specify their versions, which
would also be included in the output from `hasher --version`.

### libsodium auto find

Build does not support searching for an already installed libsodium on the
system level. At the moment it's either downloaded and built, or prebuilt
by the user who also sets its path via `-DLIBSODIUM_PREFIX /path/to/prefix`

### Static linking

Everything is statically linked. Depending on the runtime environment, if
libsodium is guaranteed to be available a dynamic linking option could be
useful.

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
