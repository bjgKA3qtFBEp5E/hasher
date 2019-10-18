#ifndef _HASH_BACKEND_H_
#define _HASH_BACKEND_H_

#define EHASH_BACKEND_INIT_FAILED -1

typedef int (*HashFn) (unsigned char *out, size_t outlen, const unsigned char *in, unsigned long long inlen);

struct HashBackend {
    const char *name;
    size_t hashlen;
    HashFn hashfn;
};

int get_hash_backend(struct HashBackend *backend);

#endif
