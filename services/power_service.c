#include <stdio.h>
#include <string.h>
#include "bolun/services.h"

static int power_started;

BolunStatus bolun_power_service_start(void) { power_started = 1; return BOLUN_OK; }

BolunStatus bolun_power_service_handle(const char *request, char *response, size_t response_size) {
    if (!power_started || !request || !response || response_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    snprintf(response, response_size, "power:handled:%s", request);
    return BOLUN_OK;
}

BolunStatus bolun_power_service_stop(void) { power_started = 0; return BOLUN_OK; }
