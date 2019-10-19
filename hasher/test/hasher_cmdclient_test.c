#include <stdio.h>
#include <stdbool.h>
#include "tunit.h"
#include "hash_backend.h"
#include "hasher_cmdclient.h"

static int hash_fn_for_testing(unsigned char *out, size_t outlen,
                const unsigned char *in, unsigned long long inlen);

static bool force_fail_backend = false;

Spec(HasherCmdClient, {

    describe(print_version, {
        before_each({})
        after_each({})
        it("should print version and backend", {
            char expected[] = "Hasher v" STR(HASHER_VERSION_MAJOR) "." STR(HASHER_VERSION_MINOR) "\nUsing TestBackend\n";

            char outputbuf[sizeof(expected)];
            memset(outputbuf, 0, sizeof(outputbuf));

            struct HashBackend hashbackend = {
                .name = "TestBackend"
            };

            with(outstream, outputbuf, "w", {
                HasherCmdClient_print_version(outstream, &hashbackend);
            })

            assert_str_equals(expected, outputbuf);
        })
    })

    describe(print_hash, {

        unsigned char expected[10] = { 'H', 0x2, 0x0, 'l', 0x3 };
        unsigned char in_original[] = { 'H', 'e', 'l', 'l', 'o' };
        unsigned char in[sizeof in_original];
        struct HashBackend backend;

        before_each({
            //reset in case it has changed by a test
            memcpy(in, in_original, sizeof(in));
            get_hash_backend(&backend);
        })
        after_each({})

        it("should fail if backend not properly initialized", {
            backend.hashfn = NULL;
            char expected_err[] = ERROR_BAD_BACKEND "\n";

            char streambuf[sizeof(expected)];
            char errstreambuf[sizeof(expected_err)];

            memset(streambuf, 0, sizeof(streambuf));
            memset(errstreambuf, 0, sizeof(errstreambuf));

            with(outstream, streambuf, "w", {
                with(errstream, errstreambuf, "w", {
                    HasherCmdClient_print_hash(
                        outstream, errstream, &backend, in, sizeof in
                    );
                })
            })

            assert_str_equals(expected_err, errstreambuf);

        })

        it("should print hash", {
            char streambuf[sizeof(expected)];
            memset(streambuf, 0, sizeof(streambuf));
            int result;

            with(outstream, streambuf, "w", {
                result = HasherCmdClient_print_hash(
                    outstream, stderr, &backend, in, sizeof in
                );
            })
            assert_int_equals(0, result);
            assert_arrays_equals(expected, streambuf);
        })
    })

    describe(print_usage, {
        before_each({})
        after_each({})
        it("should print usage given a binary", {
            char expected[] = "Usage: /path/to/bin [--version] [STRING]\n";

            char outputbuf[sizeof(expected)];
            memset(outputbuf, 0, sizeof(outputbuf));

            with(outstream, outputbuf, "w", {
                HasherCmdClient_print_usage(outstream, "/path/to/bin");
            })

            assert_str_equals(expected, outputbuf);
        })
    })

    describe(parse_arguments, {
        int parse_arguments_return;
        bool force_fail_backend_original = force_fail_backend;

        before_each({
            parse_arguments_return = 0;
        })

        after_each({
            force_fail_backend = force_fail_backend_original;
        })

        it("handles --version", {
            char expected[] = "Hasher v" STR(HASHER_VERSION_MAJOR) "." STR(HASHER_VERSION_MINOR) "\nUsing TestingBackend\n";

            char streambuf[sizeof(expected)];
            char errstreambuf[10] = { 0 }; //should be empty eventually
            char zeros[10] = { 0 };

            memset(streambuf, 0, sizeof(streambuf));

            int argc = 2;
            char *argv[] = {
                "/path/to/bin", "--version"
            };

            with(outstream, streambuf, "w", {
                with(errstream, errstreambuf, "w", {
                    parse_arguments_return = HasherCmdClient_parse_arguments(
                        outstream, errstream, argc, argv
                    );
                })
            })

            assert_int_equals(0, parse_arguments_return);
            assert_str_equals(expected, streambuf);
            assert_true(memcmp(errstreambuf, zeros, sizeof(zeros)) == 0);
        })

        it("prints usage if no arguments", {
            char expected[] = "Usage: /path/to/bin [--version] [STRING]\n";

            char streambuf[sizeof(expected)];
            char errstreambuf[10] = { 0 }; //should be empty eventually
            char zeros[10] = { 0 };

            memset(streambuf, 0, sizeof(streambuf));

            int argc = 1;
            char *argv[] = {
                "/path/to/bin"
            };

            with(outstream, streambuf, "w", {
                with(errstream, errstreambuf, "w", {
                    parse_arguments_return = HasherCmdClient_parse_arguments(
                        outstream, errstream, argc, argv
                    );
                })
            })

            assert_int_equals(0, parse_arguments_return);
            assert_str_equals(expected, streambuf);
            assert_true(memcmp(errstreambuf, zeros, sizeof(zeros)) == 0);
        })
        it("prints usage if too many arguments given", {
            char expected[] = "Usage: /path/to/bin [--version] [STRING]\n";

            char streambuf[sizeof(expected)];
            char errstreambuf[10] = { 0 }; //should be empty eventually
            char zeros[10] = { 0 };

            memset(streambuf, 0, sizeof(streambuf));

            int argc = 3;
            char *argv[] = {
                "/path/to/bin", "--version", "bla"
            };

            with(outstream, streambuf, "w", {
                with(errstream, errstreambuf, "w", {
                    parse_arguments_return = HasherCmdClient_parse_arguments(
                        outstream, errstream, argc, argv
                    );
                })
            })

            assert_int_equals(0, parse_arguments_return);
            assert_str_equals(expected, streambuf);
            assert_true(memcmp(errstreambuf, zeros, sizeof(zeros)) == 0);
        })
        it("does not hash if hash backend failed", {
            char expected[] = ERROR_BAD_BACKEND "\n";

            char streambuf[10] = { 0 }; // should be empty eventually
            char errstreambuf[sizeof(expected)];
            char zeros[10] = { 0 };
            force_fail_backend = true;

            memset(errstreambuf, 0, sizeof(errstreambuf));

            int argc = 2;
            char *argv[] = {
                "/path/to/bin", "Hello"
            };

            with(outstream, streambuf, "w", {
                with(errstream, errstreambuf, "w", {
                    parse_arguments_return = HasherCmdClient_parse_arguments(
                        outstream, errstream, argc, argv
                    );
                })
            })

            assert_int_equals(1, parse_arguments_return);
            assert_str_equals(expected, errstreambuf);
            assert_true(memcmp(streambuf, zeros, sizeof(zeros)) == 0);
        })

        it("hashes given argument", {
            unsigned char expected[10] = { 'H', 0x2, 0x0, 'l', 0x3 };

            char streambuf[sizeof(expected)];
            char errstreambuf[10] = { 0 }; //should be empty eventually
            char zeros[10] = { 0 };

            memset(streambuf, 0, sizeof(streambuf));

            int argc = 2;
            char *argv[] = {
                "/path/to/bin", "Hello"
            };

            with(outstream, streambuf, "w", {
                with(errstream, errstreambuf, "w", {
                    parse_arguments_return = HasherCmdClient_parse_arguments(
                        outstream, errstream, argc, argv
                    );
                })
            })

            assert_int_equals(0, parse_arguments_return);
            assert_arrays_equals(expected, streambuf);
            assert_true(memcmp(errstreambuf, zeros, sizeof(zeros)) == 0);
        })
    })
})

int get_hash_backend(struct HashBackend *backend) {
    if (force_fail_backend)
        return EHASH_BACKEND_INIT_FAILED;

    *backend = (struct HashBackend){
        .name = "TestingBackend",
        .hashlen = 10,
        .hashfn = hash_fn_for_testing
    };
    return 0;
}
static int hash_fn_for_testing(unsigned char *out, size_t outlen,
                const unsigned char *in, unsigned long long inlen) {
    unsigned char dummy_hash_val[10] = { 'H', 0x2, 0x0, 'l', 0x3 };
    memcpy(out, dummy_hash_val, outlen);
    return 0;
}
