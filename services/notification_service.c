#include <stdio.h>
#include <string.h>
#include "bolun/services.h"

static int notification_started;

BolunStatus bolun_notification_service_start(void) { notification_started = 1; return BOLUN_OK; }

BolunStatus bolun_notification_service_handle(const char *request, char *response, size_t response_size) {
    if (!notification_started || !request || !response || response_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    snprintf(response, response_size, "notification:handled:%s", request);
    return BOLUN_OK;
}

BolunStatus bolun_notification_service_stop(void) { notification_started = 0; return BOLUN_OK; }
