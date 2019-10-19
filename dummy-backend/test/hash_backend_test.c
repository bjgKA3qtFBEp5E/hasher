#include "tunit.h"
#include "hash_backend.h"

Spec(HashBackend, {
    describe(DummyHashBackend, {

        struct HashBackend backend;
        int get_backend_result;

        before_each({
            get_backend_result = get_hash_backend(&backend);
        })
        after_each({})

        it("gets backend", {
            assert_int_equals(0, get_backend_result);
            assert_str_equals("DummyHashBackend", backend.name);
            assert_not_null(backend.hashfn);
            assert_int_equals(20, backend.hashlen);
        })

        it("hashes correctly", {
            assert_int_equals(0, get_backend_result);
            unsigned char in[] = { 'H', 'e', 'l', 'l', 'o' };

            unsigned char out[20] = { 0 };
            unsigned char expected[20] = { 'H', 'e', 'l', 'l', 'o' };

            backend.hashfn(out, sizeof out, in, sizeof in);

            assert_arrays_equals(expected, out);
        })
    })
})
