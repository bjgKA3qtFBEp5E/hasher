# Hasher

Hasher is a command line tool for hashing input strings

```
Usage: hasher [--version] [STRING]
```

## Summary

- Hasher uses an underlying ```HashBackend``` which provides the implementation of the hashing function.
- Multiple backends could exist, and only one backend can be specified using
build options.
- The chosen backend is the only one going to be linked to the binary
during build, other backends will not be built.

## Build

The project is based on cmake and the following options are available:

- `USE_SODIUM` instructs the project to use sodium backend. If not specified a dummy backend will be used.
- `LIBSODIUM_PREFIX` specify prefix to use prebuilt libsodium and header from. If not specified, libsodium will be downloaded and built.

### Build for local host
```
mkdir build && cd build
cmake .. -DUSE_SODIUM=yes
```

### Build for Android
```
mkdir build && cd build
cmake .. \
-DUSE_SODIUM=yes \
-DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
-DANDROID_ABI=$ABI \
-DANDROID_PLATFORM=$MINSDKVERSION
```

- Replace `$NDK` or set it to local path to Android NDK.
- Replace `$ABI` or set it to target ABI [arm, x86]
- Replace `$MINSDKVERION` or set it to desired value

## Testing

After building, execute tests using:

```
CTEST_OUTPUT_ON_FAILURE=1 make tests
```

Note that only tests for the selected backend are built, and that this will not
work if this was an android build (see Limitations and potential improvements).


## Limitations and potential improvements

### Testing on Android

Although tests build for target platform, they have to be manually pushed and
run on a connected device. This could be achievable using cmake but the
decision was eventually not to address it.

### Basic argument handling

Arguments handling is done in a very basic way. Hasher is not able to hash any
input that is also a name of an accepted hasher option. Because `--version` is
a valid Hasher argument that prints the program version, this specific value
is not hashable.

### Backend versions

The argument `--version` only prints Hasher version and name of backend that
was built in. It would be useful if backends are able to specify their versions which would also be included in the output from `hasher --version`.

### libsodium auto find

Build does not support searching for an already installed libsodium on the
system level. At the moment it's either automatically downloaded and built as
part of the build, or prebuilt by the user who would set its path via
`-DLIBSODIUM_PREFIX /path/to/prefix`

### Static linking

Everything is statically linked. Depending on the runtime environment, if
libsodium is guaranteed to be available on the target platform a dynamic linking option could be useful.
