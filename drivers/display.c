#include <stdio.h>
#include "bolun/driver.h"

static int display_running;

static BolunStatus display_probe(const BolunBoardProfile *profile) {
    if (!profile) return BOLUN_ERROR_INVALID_ARGUMENT;
    if (BOLUN_DRIVER_DISPLAY == BOLUN_DRIVER_NFC && (profile->feature_flags & BOLUN_FEATURE_NFC) == 0) return BOLUN_ERROR_NOT_FOUND;
    if (BOLUN_DRIVER_DISPLAY == BOLUN_DRIVER_CAMERA && (profile->feature_flags & BOLUN_FEATURE_CAMERA) == 0) return BOLUN_ERROR_NOT_FOUND;
    return bolun_hal_validate_profile(profile);
}

static BolunStatus display_init(const BolunBoardProfile *profile) {
    BolunStatus probe = display_probe(profile);
    if (probe == BOLUN_ERROR_NOT_FOUND) return BOLUN_OK;
    return probe;
}

static BolunStatus display_start(void) { display_running = 1; return BOLUN_OK; }
static BolunStatus display_suspend(void) { if (!display_running) return BOLUN_ERROR_NOT_FOUND; display_running = 0; return BOLUN_OK; }
static BolunStatus display_resume(void) { display_running = 1; return BOLUN_OK; }
static BolunStatus display_stop(void) { display_running = 0; return BOLUN_OK; }
static const char *display_diagnostics(void) { return display_running ? "display:running" : "display:stopped"; }

const BolunDriver *bolun_display_driver(void) {
    static const BolunDriver driver = {
        "display", BOLUN_DRIVER_DISPLAY, display_probe, display_init, display_start, display_suspend, display_resume, display_stop, display_diagnostics
    };
    return &driver;
}
