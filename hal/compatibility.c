#include "bolun/hal.h"

BolunStatus bolun_hal_validate_all_profiles(void) {
    for (size_t i = 0; i < bolun_hal_profile_count(); ++i) {
        BolunStatus status = bolun_hal_validate_profile(bolun_hal_profile_at(i));
        if (status != BOLUN_OK) return status;
    }
    return BOLUN_OK;
}
