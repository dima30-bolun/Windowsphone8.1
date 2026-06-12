#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "bolun/license.h"

static BolunLicenseState global_license;

static char key_char(uint32_t *state) {
    static const char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    *state = (*state * 1664525u) + 1013904223u;
    return alphabet[(*state >> 16) % 36u];
}

static unsigned checksum(const char *key) {
    unsigned sum = 0;
    for (size_t i = 0; key[i]; ++i) if (key[i] != '-' && i != 21 && i != 22) sum = (sum * 33u + (unsigned char)key[i]) % 997u;
    return sum;
}

BolunStatus bolun_license_init(BolunLicenseState *state, uint32_t install_day, uint32_t current_day) {
    if (!state || current_day < install_day) return BOLUN_ERROR_INVALID_ARGUMENT;
    memset(state, 0, sizeof(*state));
    state->install_day = install_day;
    state->current_day = current_day;
    state->daily_usage_day = current_day;
    return BOLUN_OK;
}

BolunStatus bolun_license_set_day(BolunLicenseState *state, uint32_t current_day) {
    if (!state || current_day < state->current_day) return BOLUN_ERROR_DENIED;
    state->current_day = current_day;
    if (state->daily_usage_day != current_day) {
        state->daily_usage_day = current_day;
        state->daily_usage_count = 0;
    }
    return BOLUN_OK;
}

BolunStatus bolun_license_generate_key(uint32_t seed, char out_key[32]) {
    if (!out_key) return BOLUN_ERROR_INVALID_ARGUMENT;
    uint32_t rng = seed ? seed : 0xB01A5EEDu;
    snprintf(out_key, 32, "BOLUN-0000-0000-0000-0000");
    for (size_t i = 6; i < 25; ++i) if (out_key[i] != '-') out_key[i] = key_char(&rng);
    unsigned sum = checksum(out_key);
    out_key[21] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[(sum / 36u) % 36u];
    out_key[22] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[sum % 36u];
    return BOLUN_OK;
}

BolunStatus bolun_license_validate_key(const char *key) {
    if (!key || strlen(key) != BOLUN_LICENSE_KEY_LENGTH || strncmp(key, "BOLUN-", 6) != 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    for (size_t i = 0; i < BOLUN_LICENSE_KEY_LENGTH; ++i) {
        if (i == 5 || i == 10 || i == 15 || i == 20) { if (key[i] != '-') return BOLUN_ERROR_INVALID_ARGUMENT; }
        else if (!isalnum((unsigned char)key[i]) || islower((unsigned char)key[i])) return BOLUN_ERROR_INVALID_ARGUMENT;
    }
    unsigned sum = checksum(key);
    char expected_a = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[(sum / 36u) % 36u];
    char expected_b = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[sum % 36u];
    return key[21] == expected_a && key[22] == expected_b ? BOLUN_OK : BOLUN_ERROR_INTEGRITY;
}

BolunStatus bolun_license_activate(BolunLicenseState *state, const char *key) {
    if (!state) return BOLUN_ERROR_INVALID_ARGUMENT;
    BolunStatus valid = bolun_license_validate_key(key);
    if (valid != BOLUN_OK) return valid;
    snprintf(state->active_key, sizeof(state->active_key), "%s", key);
    return BOLUN_OK;
}

BolunLicenseStatus bolun_license_get_status(const BolunLicenseState *state) {
    BolunLicenseStatus status = {BOLUN_LICENSE_EXPIRED_INVALID, 0, 0};
    if (!state) return status;
    if (state->active_key[0] && bolun_license_validate_key(state->active_key) == BOLUN_OK) {
        status.mode = BOLUN_LICENSE_LICENSED;
        status.remaining_daily_requests = 0xffffffffu;
        return status;
    }
    uint32_t age = state->current_day - state->install_day;
    if (age < BOLUN_AI_TRIAL_DAYS) {
        status.mode = BOLUN_LICENSE_TRIAL;
        status.remaining_trial_days = BOLUN_AI_TRIAL_DAYS - age;
        status.remaining_daily_requests = 0xffffffffu;
        return status;
    }
    status.mode = BOLUN_LICENSE_FREE_QUOTA;
    status.remaining_daily_requests = state->daily_usage_count >= BOLUN_AI_FREE_DAILY_QUOTA ? 0 : BOLUN_AI_FREE_DAILY_QUOTA - state->daily_usage_count;
    return status;
}

int bolun_license_can_use_ai(const BolunLicenseState *state, const char *feature_id) {
    (void)feature_id;
    BolunLicenseStatus status = bolun_license_get_status(state);
    return status.mode == BOLUN_LICENSE_TRIAL || status.mode == BOLUN_LICENSE_LICENSED || status.remaining_daily_requests > 0;
}

BolunStatus bolun_license_record_ai_usage(BolunLicenseState *state, const char *feature_id) {
    (void)feature_id;
    if (!state) return BOLUN_ERROR_INVALID_ARGUMENT;
    BolunLicenseStatus status = bolun_license_get_status(state);
    if (status.mode == BOLUN_LICENSE_TRIAL || status.mode == BOLUN_LICENSE_LICENSED) return BOLUN_OK;
    if (status.remaining_daily_requests == 0) return BOLUN_ERROR_QUOTA;
    state->daily_usage_count++;
    return BOLUN_OK;
}

BolunLicenseState *bolun_service_license_state(void) { return &global_license; }

BolunStatus bolun_licensing_service_start(void) { return bolun_license_init(&global_license, 0, 0); }
BolunStatus bolun_licensing_service_handle(const char *request, char *response, size_t response_size) {
    if (!request || !response || response_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    BolunLicenseStatus status = bolun_license_get_status(&global_license);
    snprintf(response, response_size, "license:%d:trial:%u:quota:%u:req:%s", (int)status.mode, status.remaining_trial_days, status.remaining_daily_requests, request);
    return BOLUN_OK;
}
BolunStatus bolun_licensing_service_stop(void) { return BOLUN_OK; }
