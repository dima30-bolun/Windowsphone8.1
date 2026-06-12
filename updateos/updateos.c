#include <stdio.h>
#include "bolun/status.h"

BolunStatus bolun_updateos_stage_package(const char *package_name, unsigned battery_percent, char *out, unsigned long out_size) {
    if (!package_name || !out || out_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    if (battery_percent < 30) return BOLUN_ERROR_DENIED;
    snprintf(out, out_size, "update:%s:staged", package_name);
    return BOLUN_OK;
}
