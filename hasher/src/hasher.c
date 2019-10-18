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
    struct HashBackend backend = { .name = "N/A" };
    get_hash_backend(&backend);

    printf("Hasher v%d.%d\nUsing %s\n",
        HASHER_VERSION_MAJOR, HASHER_VERSION_MINOR, backend.name
    );
}

static void print_hash(const unsigned char *input, unsigned long long inlen) {
    struct HashBackend backend;

    if (get_hash_backend(&backend) == EHASH_BACKEND_INIT_FAILED) {
        fprintf(stderr, "Failed to initialize hash backend.\n");
    } else {
        unsigned char output[backend.hashlen];
        backend.hashfn(output, backend.hashlen, input, inlen);
        printf("%s", output);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    } else if (strcmp(ARG_VERSION, argv[1]) == 0) {
        print_version();
    } else {
        print_hash((const unsigned char *)argv[1], strlen(argv[1]));
    }

    return 0;
}
