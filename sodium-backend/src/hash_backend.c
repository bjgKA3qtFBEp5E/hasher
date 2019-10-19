#include <stdio.h>
#include "sodium.h"
#include "hash_backend.h"

#define SODIUM_BACKEND_NAME "SodiumHashBackend"

/*
 * Hashing function used by SodiumHashBackend. This is just a wrapper for
 * crypto_generichash to implement the interface needed by HashBackend.
 *
 * out: output buffer
 * outlen: size of output buffer
 * in: input buffer
 * inlen: size of input buffer
 *
 * returns: 0 if succeeds, 1 otherwise.
 */
static int sodium_backend_hash(
    unsigned char *out, size_t outlen, const unsigned char *in,
    unsigned long long inlen
) {
    return crypto_generichash(out, outlen, in, inlen, NULL, 0);
}
/*
 * Initialized a SodiumHashBackend
 *
 * backend: Pointer to HashBackend that will be initialized into.
 *
 * returns: 0 if succeeds, error code if fails.
 */
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
