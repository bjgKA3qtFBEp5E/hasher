#include <stdio.h>
#include "hasher_cmdclient.h"


int main(int argc, char *argv[]) {
    return HasherCmdClient_parse_arguments(
        stdout, stderr, argc, argv
    );
}
