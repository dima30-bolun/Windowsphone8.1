#include <stdio.h>
#include "bolun/driver.h"

static int sensors_running;

static BolunStatus sensors_probe(const BolunBoardProfile *profile) {
    if (!profile) return BOLUN_ERROR_INVALID_ARGUMENT;
    if (BOLUN_DRIVER_SENSORS == BOLUN_DRIVER_NFC && (profile->feature_flags & BOLUN_FEATURE_NFC) == 0) return BOLUN_ERROR_NOT_FOUND;
    if (BOLUN_DRIVER_SENSORS == BOLUN_DRIVER_CAMERA && (profile->feature_flags & BOLUN_FEATURE_CAMERA) == 0) return BOLUN_ERROR_NOT_FOUND;
    return bolun_hal_validate_profile(profile);
}

static BolunStatus sensors_init(const BolunBoardProfile *profile) {
    BolunStatus probe = sensors_probe(profile);
    if (probe == BOLUN_ERROR_NOT_FOUND) return BOLUN_OK;
    return probe;
}

static BolunStatus sensors_start(void) { sensors_running = 1; return BOLUN_OK; }
static BolunStatus sensors_suspend(void) { if (!sensors_running) return BOLUN_ERROR_NOT_FOUND; sensors_running = 0; return BOLUN_OK; }
static BolunStatus sensors_resume(void) { sensors_running = 1; return BOLUN_OK; }
static BolunStatus sensors_stop(void) { sensors_running = 0; return BOLUN_OK; }
static const char *sensors_diagnostics(void) { return sensors_running ? "sensors:running" : "sensors:stopped"; }

const BolunDriver *bolun_sensors_driver(void) {
    static const BolunDriver driver = {
        "sensors", BOLUN_DRIVER_SENSORS, sensors_probe, sensors_init, sensors_start, sensors_suspend, sensors_resume, sensors_stop, sensors_diagnostics
    };
    return &driver;
}
