#include <stdio.h>
#include <string.h>
#include "bolun/services.h"

static int update_started;

BolunStatus bolun_update_service_start(void) { update_started = 1; return BOLUN_OK; }

BolunStatus bolun_update_service_handle(const char *request, char *response, size_t response_size) {
    if (!update_started || !request || !response || response_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    snprintf(response, response_size, "update:handled:%s", request);
    return BOLUN_OK;
}

BolunStatus bolun_update_service_stop(void) { update_started = 0; return BOLUN_OK; }
