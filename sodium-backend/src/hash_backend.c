#include <stdio.h>
#include "sodium.h"
#include "hash_backend.h"

#define DUMMY_BACKEND_NAME "SodiumHashBackend"

static int dummy_hash(
    unsigned char *out, size_t outlen, const unsigned char *in,
    unsigned long long inlen
) {
    int result = crypto_generichash(out, outlen, in, inlen, NULL, 0);
    printf("genhash res = %d\n", result);
    return 0;
}

struct HashBackend get_hash_backend() {
    if (sodium_init() < 0) {
        printf("FAILED TO INIT!!\n");
    } else {
        printf("SODIUM INIT OK!\n");
    }
        struct HashBackend backend = {
            .name=DUMMY_BACKEND_NAME,
            .hashfn=dummy_hash,
            .hashlen=crypto_generichash_BYTES
        };
        return backend;
}
