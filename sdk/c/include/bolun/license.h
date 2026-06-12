#ifndef BOLUN_LICENSE_H
#define BOLUN_LICENSE_H

#include <stdint.h>
#include "bolun/status.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BOLUN_LICENSE_KEY_LENGTH 25
#define BOLUN_AI_FREE_DAILY_QUOTA 3
#define BOLUN_AI_TRIAL_DAYS 30

typedef enum BolunLicenseMode {
    BOLUN_LICENSE_TRIAL,
    BOLUN_LICENSE_LICENSED,
    BOLUN_LICENSE_FREE_QUOTA,
    BOLUN_LICENSE_EXPIRED_INVALID
} BolunLicenseMode;

typedef struct BolunLicenseState {
    uint32_t install_day;
    uint32_t current_day;
    uint32_t daily_usage_day;
    uint32_t daily_usage_count;
    char active_key[32];
} BolunLicenseState;

typedef struct BolunLicenseStatus {
    BolunLicenseMode mode;
    uint32_t remaining_trial_days;
    uint32_t remaining_daily_requests;
} BolunLicenseStatus;

BolunStatus bolun_license_init(BolunLicenseState *state, uint32_t install_day, uint32_t current_day);
BolunStatus bolun_license_set_day(BolunLicenseState *state, uint32_t current_day);
BolunStatus bolun_license_generate_key(uint32_t seed, char out_key[32]);
BolunStatus bolun_license_validate_key(const char *key);
BolunStatus bolun_license_activate(BolunLicenseState *state, const char *key);
BolunLicenseStatus bolun_license_get_status(const BolunLicenseState *state);
int bolun_license_can_use_ai(const BolunLicenseState *state, const char *feature_id);
BolunStatus bolun_license_record_ai_usage(BolunLicenseState *state, const char *feature_id);

#ifdef __cplusplus
}
#endif

#endif
