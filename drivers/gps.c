#include <stdio.h>
#include "bolun/driver.h"

static int gps_running;

static BolunStatus gps_probe(const BolunBoardProfile *profile) {
    if (!profile) return BOLUN_ERROR_INVALID_ARGUMENT;
    if (BOLUN_DRIVER_GPS == BOLUN_DRIVER_NFC && (profile->feature_flags & BOLUN_FEATURE_NFC) == 0) return BOLUN_ERROR_NOT_FOUND;
    if (BOLUN_DRIVER_GPS == BOLUN_DRIVER_CAMERA && (profile->feature_flags & BOLUN_FEATURE_CAMERA) == 0) return BOLUN_ERROR_NOT_FOUND;
    return bolun_hal_validate_profile(profile);
}

static BolunStatus gps_init(const BolunBoardProfile *profile) {
    BolunStatus probe = gps_probe(profile);
    if (probe == BOLUN_ERROR_NOT_FOUND) return BOLUN_OK;
    return probe;
}

static BolunStatus gps_start(void) { gps_running = 1; return BOLUN_OK; }
static BolunStatus gps_suspend(void) { if (!gps_running) return BOLUN_ERROR_NOT_FOUND; gps_running = 0; return BOLUN_OK; }
static BolunStatus gps_resume(void) { gps_running = 1; return BOLUN_OK; }
static BolunStatus gps_stop(void) { gps_running = 0; return BOLUN_OK; }
static const char *gps_diagnostics(void) { return gps_running ? "gps:running" : "gps:stopped"; }

const BolunDriver *bolun_gps_driver(void) {
    static const BolunDriver driver = {
        "gps", BOLUN_DRIVER_GPS, gps_probe, gps_init, gps_start, gps_suspend, gps_resume, gps_stop, gps_diagnostics
    };
    return &driver;
}
