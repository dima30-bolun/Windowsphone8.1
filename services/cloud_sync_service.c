#include <stdio.h>
#include <string.h>
#include "bolun/services.h"

static int cloud_sync_started;

BolunStatus bolun_cloud_sync_service_start(void) { cloud_sync_started = 1; return BOLUN_OK; }

BolunStatus bolun_cloud_sync_service_handle(const char *request, char *response, size_t response_size) {
    if (!cloud_sync_started || !request || !response || response_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    snprintf(response, response_size, "cloud_sync:handled:%s", request);
    return BOLUN_OK;
}

BolunStatus bolun_cloud_sync_service_stop(void) { cloud_sync_started = 0; return BOLUN_OK; }
