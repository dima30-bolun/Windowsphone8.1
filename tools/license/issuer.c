#include <stdio.h>
#include <stdlib.h>
#include "bolun/license.h"

int main(int argc, char **argv) {
    uint32_t seed = argc > 1 ? (uint32_t)strtoul(argv[1], 0, 10) : 0xB01A0001u;
    char key[32];
    BolunStatus status = bolun_license_generate_key(seed, key);
    if (status != BOLUN_OK) {
        fprintf(stderr, "failed:%s\n", bolun_status_string(status));
        return 1;
    }
    puts(key);
    return 0;
}
