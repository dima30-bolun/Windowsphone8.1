#include "bolun/status.h"

const char *bolun_status_string(BolunStatus status) {
    switch (status) {
        case BOLUN_OK: return "ok";
        case BOLUN_ERROR_INVALID_ARGUMENT: return "invalid_argument";
        case BOLUN_ERROR_NOT_FOUND: return "not_found";
        case BOLUN_ERROR_NO_CAPACITY: return "no_capacity";
        case BOLUN_ERROR_DENIED: return "denied";
        case BOLUN_ERROR_EXPIRED: return "expired";
        case BOLUN_ERROR_QUOTA: return "quota";
        case BOLUN_ERROR_INTEGRITY: return "integrity";
        default: return "unknown";
    }
}
