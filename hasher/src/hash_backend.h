#ifndef _HASH_BACKEND_H_
#define _HASH_BACKEND_H_


typedef int (*HashFn) (unsigned char *out, size_t outlen, const unsigned char *in, unsigned long long inlen);

struct HashBackend {
    const char *name;
    size_t hashlen;
    HashFn hashfn;
};

struct HashBackend get_hash_backend(void);

#endif
