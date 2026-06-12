#include <stdio.h>
#include <string.h>
#include "bolun/hal.h"

typedef enum BolunBootTarget { BOLUN_BOOT_MAINOS, BOLUN_BOOT_RECOVERY, BOLUN_BOOT_UPDATEOS, BOLUN_BOOT_DIAGNOSTICS } BolunBootTarget;

const char *bolun_boot_target_name(BolunBootTarget target) {
    switch (target) {
        case BOLUN_BOOT_MAINOS: return "MainOS";
        case BOLUN_BOOT_RECOVERY: return "Recovery";
        case BOLUN_BOOT_UPDATEOS: return "UpdateOS";
        case BOLUN_BOOT_DIAGNOSTICS: return "Diagnostics";
        default: return "Unknown";
    }
}

BolunStatus bolun_boot_select_target(unsigned failed_boots, int update_pending, BolunBootTarget *out) {
    if (!out) return BOLUN_ERROR_INVALID_ARGUMENT;
    if (failed_boots >= 3) *out = BOLUN_BOOT_RECOVERY;
    else if (update_pending) *out = BOLUN_BOOT_UPDATEOS;
    else *out = BOLUN_BOOT_MAINOS;
    return BOLUN_OK;
}

BolunStatus bolun_boot_build_log(const BolunBoardProfile *profile, BolunBootTarget target, char *out, size_t out_size) {
    if (!profile || !out || out_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    snprintf(out, out_size, "Bolun Boot:%s:%s:%s", bolun_boot_target_name(target), profile->model, bolun_hal_soc_name(profile->soc));
    return BOLUN_OK;
}
