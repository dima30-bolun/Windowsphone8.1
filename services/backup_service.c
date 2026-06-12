#include <stdio.h>
#include <string.h>
#include "bolun/services.h"

static int backup_started;

BolunStatus bolun_backup_service_start(void) { backup_started = 1; return BOLUN_OK; }

BolunStatus bolun_backup_service_handle(const char *request, char *response, size_t response_size) {
    if (!backup_started || !request || !response || response_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    snprintf(response, response_size, "backup:handled:%s", request);
    return BOLUN_OK;
}

BolunStatus bolun_backup_service_stop(void) { backup_started = 0; return BOLUN_OK; }
