#ifndef BOLUN_SERVICES_H
#define BOLUN_SERVICES_H

#include <stddef.h>
#include "bolun/status.h"
#include "bolun/license.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct BolunService {
    const char *name;
    BolunStatus (*start)(void);
    BolunStatus (*handle_request)(const char *request, char *response, size_t response_size);
    BolunStatus (*stop)(void);
} BolunService;

size_t bolun_service_count(void);
const BolunService *bolun_service_at(size_t index);
BolunStatus bolun_service_start_all(void);
BolunStatus bolun_service_request(const char *service_name, const char *request, char *response, size_t response_size);
BolunLicenseState *bolun_service_license_state(void);

#ifdef __cplusplus
}
#endif

#endif
