#ifndef _HASHER_CMDCLIENT_H
#define _HASHER_CMDCLIENT_H

#include <stdio.h>
#include "hash_backend.h"

#define ARG_VERSION "--version"

#define HASHER_VERSION_MAJOR 0
#define HASHER_VERSION_MINOR 1
#define HASHER_USAGE "Usage: %s [--version] [STRING]\n"
#define ERROR_BAD_BACKEND "E: Backend not initialized"

void HasherCmdClient_print_usage(FILE *stream, char *bin);
void HasherCmdClient_print_version(FILE *stream, struct HashBackend *backend);
int HasherCmdClient_print_hash(FILE *outstream, FILE *errstream, struct HashBackend *backend, const unsigned char *input, unsigned long long inlen);
int HasherCmdClient_parse_arguments(FILE *outstream, FILE *errstream, int argc, char *argv[]);

#endif
