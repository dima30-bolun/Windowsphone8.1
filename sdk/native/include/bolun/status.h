#ifndef BOLUN_STATUS_H
#define BOLUN_STATUS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum BolunStatus {
    BOLUN_OK = 0,
    BOLUN_ERROR_INVALID_ARGUMENT = 1,
    BOLUN_ERROR_NOT_FOUND = 2,
    BOLUN_ERROR_NO_CAPACITY = 3,
    BOLUN_ERROR_DENIED = 4,
    BOLUN_ERROR_EXPIRED = 5,
    BOLUN_ERROR_QUOTA = 6,
    BOLUN_ERROR_INTEGRITY = 7
} BolunStatus;

const char *bolun_status_string(BolunStatus status);

#ifdef __cplusplus
}
#endif

#endif
