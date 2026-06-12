#include <stdio.h>
#include <string.h>
#include "bolun/services.h"

static int storage_started;

BolunStatus bolun_storage_service_start(void) { storage_started = 1; return BOLUN_OK; }

BolunStatus bolun_storage_service_handle(const char *request, char *response, size_t response_size) {
    if (!storage_started || !request || !response || response_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    snprintf(response, response_size, "storage:handled:%s", request);
    return BOLUN_OK;
}

BolunStatus bolun_storage_service_stop(void) { storage_started = 0; return BOLUN_OK; }
