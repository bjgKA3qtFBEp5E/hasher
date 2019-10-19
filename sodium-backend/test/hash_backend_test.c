#include "tunit.h"
#include "hash_backend.h"

Spec(HashBackend, {
    describe(SodiumHashBackend, {

        struct HashBackend backend;
        int get_backend_result;

        before_each({
            get_backend_result = get_hash_backend(&backend);
        })
        after_each({})

        it("gets backend", {
            assert_int_equals(0, get_backend_result);
            assert_str_equals("SodiumHashBackend", backend.name);
            assert_not_null(backend.hashfn);
            assert_int_equals(32, backend.hashlen);
        })

        it("hashes correctly", {
            unsigned char in[] = { 'H', 'e', 'l', 'l', 'o' };

            unsigned char out[32] = { 0 };
            unsigned char expected[32] = {
                0x8b, 0x7c, 0xa7, 0xd2, 0x7d, 0x9f, 0xc5, 0x5f, 0xa3, 0xa,
                0xbf, 0xe5, 0x15, 0xb3, 0xaf, 0xb2, 0x4e, 0x3f, 0xe8, 0x9f,
                0xdd, 0x2, 0xe2, 0xac, 0x92, 0xbc, 0xa2, 0xc9, 0x66, 0x80,
                0x64, 0x2e
            };

            backend.hashfn(out, sizeof out, in, sizeof in);

            assert_arrays_equals(expected, out);
        })
    })
})
