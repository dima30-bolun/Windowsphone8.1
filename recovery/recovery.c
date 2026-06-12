#include <stdio.h>
#include "bolun/status.h"

BolunStatus bolun_recovery_action(const char *action, char *out, unsigned long out_size) {
    if (!action || !out || out_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    snprintf(out, out_size, "recovery:%s:ok", action);
    return BOLUN_OK;
}
