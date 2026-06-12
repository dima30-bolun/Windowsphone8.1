#include "bolun/status.h"
#include "bolun/hal.h"
#include "bolun/driver.h"
#include "bolun/services.h"

BolunStatus bolun_mainos_start(const BolunBoardProfile *profile) {
    BolunStatus profile_status = bolun_hal_validate_profile(profile);
    if (profile_status != BOLUN_OK) return profile_status;
    BolunStatus drivers = bolun_driver_start_all(profile);
    if (drivers != BOLUN_OK) return drivers;
    return bolun_service_start_all();
}
