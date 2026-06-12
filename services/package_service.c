#include <stdio.h>
#include <string.h>
#include "bolun/services.h"

static int package_started;

BolunStatus bolun_package_service_start(void) { package_started = 1; return BOLUN_OK; }

BolunStatus bolun_package_service_handle(const char *request, char *response, size_t response_size) {
    if (!package_started || !request || !response || response_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    snprintf(response, response_size, "package:handled:%s", request);
    return BOLUN_OK;
}

BolunStatus bolun_package_service_stop(void) { package_started = 0; return BOLUN_OK; }
