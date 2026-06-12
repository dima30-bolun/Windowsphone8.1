#include <stdio.h>
#include "bolun/driver.h"

static int cellular_running;

static BolunStatus cellular_probe(const BolunBoardProfile *profile) {
    if (!profile) return BOLUN_ERROR_INVALID_ARGUMENT;
    if (BOLUN_DRIVER_CELLULAR == BOLUN_DRIVER_NFC && (profile->feature_flags & BOLUN_FEATURE_NFC) == 0) return BOLUN_ERROR_NOT_FOUND;
    if (BOLUN_DRIVER_CELLULAR == BOLUN_DRIVER_CAMERA && (profile->feature_flags & BOLUN_FEATURE_CAMERA) == 0) return BOLUN_ERROR_NOT_FOUND;
    return bolun_hal_validate_profile(profile);
}

static BolunStatus cellular_init(const BolunBoardProfile *profile) {
    BolunStatus probe = cellular_probe(profile);
    if (probe == BOLUN_ERROR_NOT_FOUND) return BOLUN_OK;
    return probe;
}

static BolunStatus cellular_start(void) { cellular_running = 1; return BOLUN_OK; }
static BolunStatus cellular_suspend(void) { if (!cellular_running) return BOLUN_ERROR_NOT_FOUND; cellular_running = 0; return BOLUN_OK; }
static BolunStatus cellular_resume(void) { cellular_running = 1; return BOLUN_OK; }
static BolunStatus cellular_stop(void) { cellular_running = 0; return BOLUN_OK; }
static const char *cellular_diagnostics(void) { return cellular_running ? "cellular:running" : "cellular:stopped"; }

const BolunDriver *bolun_cellular_driver(void) {
    static const BolunDriver driver = {
        "cellular", BOLUN_DRIVER_CELLULAR, cellular_probe, cellular_init, cellular_start, cellular_suspend, cellular_resume, cellular_stop, cellular_diagnostics
    };
    return &driver;
}
