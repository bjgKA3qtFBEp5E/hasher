#include <stdio.h>
#include <string.h>
#include "hasher_cmdclient.h"
#include "hash_backend.h"

void HasherCmdClient_print_usage(FILE *stream, char *bin) {
    fprintf(stream, HASHER_USAGE, bin);
}

void HasherCmdClient_print_version(FILE *stream, struct HashBackend *backend) {
    fprintf(stream, "Hasher v%d.%d\nUsing %s\n",
        HASHER_VERSION_MAJOR, HASHER_VERSION_MINOR, backend->name
    );
}

int HasherCmdClient_print_hash(FILE *outstream, FILE *errstream, struct HashBackend *backend,
    const unsigned char *input, unsigned long long inlen) {

    if(backend->hashfn == NULL) {
        fprintf(errstream, "%s\n", ERROR_BAD_BACKEND);
        return 1;
    }

    unsigned char output[backend->hashlen];
    memset(output, 0, sizeof(output));

    backend->hashfn(output, backend->hashlen, input, inlen);

    // output might contain null bytes, hence fwrite and not fprintf
    fwrite(output, sizeof(output), 1, outstream);

    return 0;
}

int HasherCmdClient_parse_arguments(FILE *outstream, FILE *errstream,
    int argc, char *argv[]) {

    if (argc != 2) {
        HasherCmdClient_print_usage(outstream, argv[0]);
    } else {
        struct HashBackend backend = { .name = "N/A" };
        int backend_init_result = get_hash_backend(&backend);

        if (strcmp(ARG_VERSION, argv[1]) == 0) {
            HasherCmdClient_print_version(outstream, &backend);
        } else if(backend_init_result == 0) {
            HasherCmdClient_print_hash(outstream, errstream, &backend, (const unsigned char *)argv[1], strlen(argv[1]));
        } else {
            fprintf(errstream, "%s\n", ERROR_BAD_BACKEND);
            return 1;
        }
    }
    return 0;
}
