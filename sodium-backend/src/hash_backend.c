#include <stdio.h>
#include "sodium.h"
#include "hash_backend.h"

#define SODIUM_BACKEND_NAME "SodiumHashBackend"

static int sodium_backend_hash(
    unsigned char *out, size_t outlen, const unsigned char *in,
    unsigned long long inlen
) {
    return crypto_generichash(out, outlen, in, inlen, NULL, 0);
}

int get_hash_backend(struct HashBackend *backend) {
    if (sodium_init() < 0) {
        return EHASH_BACKEND_INIT_FAILED;
    }

    *backend = (struct HashBackend) {
        .name=SODIUM_BACKEND_NAME,
        .hashfn=sodium_backend_hash,
        .hashlen=crypto_generichash_BYTES
    };

    return 0;
}
