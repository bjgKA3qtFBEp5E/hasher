#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define _GNU_SOURCE
#define COMBINE1(X,Y) X##Y  // helper macro
#define COMBINE(X,Y) COMBINE1(X,Y)
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)


#define outputf(f, indents, fmt, ...) fprintf (f, "%*s" fmt "", indents * 4, "", ##__VA_ARGS__);

#define output(indents, fmt, ...) outputf(stdout, indents, fmt, ##__VA_ARGS__)
#define output_err(indents, fmt, ...) outputf(stdout, indents, fmt, ##__VA_ARGS__)

#define abort goto __test_end;

#define FMT_ASSERT_INT_EQUALS "Expected %s to be %d, actual value was %d"
#define FMT_ASSERT_STR_EQUALS "Expected %s to be \"%s\", actual value was \"%s\""
#define FMT_ASSERT_NULL "Expected %s to be null"
#define FMT_ASSERT_NOT_NULL "Expected %s to be not null"
#define FMT_ASSERT_TRUE "Expected %s to be true"
#define FMT_ASSERT_FALSE "Expected %s to be false"

#define generate_assert_generic_error(actual, err) \
    size_t msg_len = snprintf(NULL, 0, err, #actual) + 1;\
    __cxt.fail_msg = malloc(msg_len);\
    snprintf(__cxt.fail_msg, msg_len, err, #actual);\
    __cxt.free_fail_msg = true;\
    abort;

#define generate_assert_equals_error(fmt, expected, actual) \
    size_t msg_len = snprintf(NULL, 0, fmt, #actual, expected, actual) + 1;\
    __cxt.fail_msg = malloc(msg_len);\
    snprintf(__cxt.fail_msg, msg_len, fmt, #actual, expected, actual);\
    __cxt.free_fail_msg = true;\
    abort;

#define assert_null(actual) \
    if ((actual) != NULL ) {\
        generate_assert_generic_error(actual, FMT_ASSERT_NULL)\
    }

#define assert_true(actual) \
    if (!(actual)) {\
        generate_assert_generic_error(actual, FMT_ASSERT_TRUE)\
    }

#define assert_false(actual) \
    if ((actual)) {\
        generate_assert_generic_error(actual, FMT_ASSERT_FALSE)\
    }

#define assert_not_null(actual) \
    if ((actual) == NULL ) {\
        generate_assert_generic_error(actual, FMT_ASSERT_NOT_NULL)\
    }

#define assert_int_equals(expected, actual) \
    if ((expected) != (actual)) {\
        generate_assert_equals_error(FMT_ASSERT_INT_EQUALS, expected, actual)\
    }

#define assert_arrays_equals(expected, actual) \
    if(sizeof(expected) != sizeof(actual)) {\
        generate_assert_equals_error(FMT_ASSERT_STR_EQUALS, expected, actual)\
    }\
    for(int __assert_arr_equals_i = 0; __assert_arr_equals_i < sizeof(actual); __assert_arr_equals_i++) {\
        if(expected[__assert_arr_equals_i] != actual[__assert_arr_equals_i]) {\
            generate_assert_equals_error(FMT_ASSERT_STR_EQUALS, expected, actual)\
            break;\
        }\
    }

#define assert_str_equals(expected, actual) \
    if (strcmp((expected),(actual)) != 0) {\
        generate_assert_equals_error(FMT_ASSERT_STR_EQUALS, expected, actual)\
    }

#define with(stream_name, buf, mode, ...) \
{\
FILE *stream_name;\
stream_name = tmpfile();\
__VA_ARGS__\
 fseek(stream_name, 0, SEEK_SET);\
 fread(buf, sizeof(buf), 1, stream_name);\
 fclose(stream_name);\
}

struct DescribeContext {
    int indents;
    char *fail_msg;
    bool free_fail_msg;
    int run_mode;
    int num_tests;
    int tests_ran;
    int processed_tests_count;
    int result;
    void *tests_start;
    void *before_ptr;
    void *after_ptr;
    bool init;
};

#define Spec(spec_name, ...)\
int suite_runner() {\
    int __final_result = 0;\
    output(0, "running spec: %s\n", #spec_name);\
    __VA_ARGS__\
    return __final_result;\
}\
 int main(int argc, char *argv[]) {\
    int result = suite_runner();\
    return result;\
}

#define it(description, ...)\
COMBINE(__test_, __LINE__):\
    if (__cxt.tests_start == NULL) {\
            __cxt.tests_start = &&COMBINE(__test_, __LINE__);\
    }\
    if (__cxt.init) {\
        __cxt.num_tests ++;\
    } else {\
        __cxt.processed_tests_count++;\
        if (__cxt.processed_tests_count == __cxt.tests_ran + 1) {\
            output(__cxt.indents, "it %s", description);\
            __VA_ARGS__\
            goto __test_end;\
        }\
    }

#define before_each(...) {\
__real_before:\
    __cxt.before_ptr = &&__real_before;\
    if (!__cxt.init) {\
        __VA_ARGS__\
        goto *__cxt.tests_start;\
    }\
}

#define after_each(...) {\
__real_after:\
    __cxt.after_ptr = &&__real_after;\
    if (!__cxt.init) {\
        __VA_ARGS__\
        goto __tests_next;\
    }\
}

#define describe(target_name, ...)\
{\
    __label__ __test_begin, __test_end, __tests_next, __real_before, __real_after;\
    struct DescribeContext __cxt = {\
        .init = true,\
        .indents = 1\
    };\
    output(__cxt.indents++, "describe: %s\n", #target_name);\
    __VA_ARGS__\
    if(__cxt.init) {\
        __cxt.init = false;\
    }\
__test_begin:\
    if (__cxt.before_ptr == NULL)\
        goto *__cxt.tests_start;\
    goto *__cxt.before_ptr;\
\
__test_end:\
    if (__cxt.fail_msg == NULL) {\
        output(0, "  [ok]\n");\
    }\
    else {\
        __final_result++;\
        output_err(0, "  [failed]\n");\
        output_err((__cxt.indents + 1), "> E: %s \n", __cxt.fail_msg);\
        if (__cxt.free_fail_msg)\
            free(__cxt.fail_msg);\
        __cxt.free_fail_msg = false;\
        __cxt.fail_msg = NULL;\
    }\
    if (__cxt.after_ptr == NULL)\
        goto __tests_next;\
    goto *__cxt.after_ptr;\
\
__tests_next:\
    __cxt.tests_ran++;\
    __cxt.processed_tests_count = 0;\
    if (__cxt.tests_ran < __cxt.num_tests) {\
        goto __test_begin;\
    }\
}
