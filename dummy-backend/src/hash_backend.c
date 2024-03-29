#include <stddef.h>
#include <string.h>
#include "hash_backend.h"

#define DUMMY_BACKEND_NAME "DummyHashBackend"

int dummy_hash(unsigned char *out, size_t outlen,
                const unsigned char *in, unsigned long long inlen) {
    memcpy(out, in, outlen);
    return 0;
}

int get_hash_backend(struct HashBackend *backend) {
    *backend = (struct HashBackend) {
        .name=DUMMY_BACKEND_NAME, .hashfn=dummy_hash, .hashlen=20
    };
    return 0;
}
