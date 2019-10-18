#include <stdio.h>
#include <string.h>
#include "hash_backend.h"

#define ARG_VERSION "--version"

#define HASHER_VERSION_MAJOR 0
#define HASHER_VERSION_MINOR 1
#define HASHER_USAGE "Usage: %s [--version] [STRING]\n"

static void print_usage(char *bin) {
    printf(HASHER_USAGE, bin);
}

static void print_version() {
    printf("%d.%d\n", HASHER_VERSION_MAJOR, HASHER_VERSION_MINOR);
}

static void print_hash(char *input) {
    struct HashBackend backend;

    if (get_hash_backend(&backend) == EHASH_BACKEND_INIT_FAILED) {
        fprintf(stderr, "Failed to initialize hash backend.\n");
    } else {
        char output[backend.hashlen]; //TODO: explain variable arrays need
        backend.hashfn(output, backend.hashlen, input, strlen(input));
        printf("%s\n", output);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    } else if (strcmp(ARG_VERSION, argv[1]) == 0) {
        print_version();
    } else {
        print_hash(argv[1]);
    }

    return 0;
}
