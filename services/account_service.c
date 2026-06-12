#include <stdio.h>
#include <string.h>
#include "bolun/services.h"

static int account_started;

BolunStatus bolun_account_service_start(void) { account_started = 1; return BOLUN_OK; }

BolunStatus bolun_account_service_handle(const char *request, char *response, size_t response_size) {
    if (!account_started || !request || !response || response_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    snprintf(response, response_size, "account:handled:%s", request);
    return BOLUN_OK;
}

BolunStatus bolun_account_service_stop(void) { account_started = 0; return BOLUN_OK; }
