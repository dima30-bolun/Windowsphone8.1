#include <stdio.h>
#include "bolun/driver.h"

static int usb_running;

static BolunStatus usb_probe(const BolunBoardProfile *profile) {
    if (!profile) return BOLUN_ERROR_INVALID_ARGUMENT;
    if (BOLUN_DRIVER_USB == BOLUN_DRIVER_NFC && (profile->feature_flags & BOLUN_FEATURE_NFC) == 0) return BOLUN_ERROR_NOT_FOUND;
    if (BOLUN_DRIVER_USB == BOLUN_DRIVER_CAMERA && (profile->feature_flags & BOLUN_FEATURE_CAMERA) == 0) return BOLUN_ERROR_NOT_FOUND;
    return bolun_hal_validate_profile(profile);
}

static BolunStatus usb_init(const BolunBoardProfile *profile) {
    BolunStatus probe = usb_probe(profile);
    if (probe == BOLUN_ERROR_NOT_FOUND) return BOLUN_OK;
    return probe;
}

static BolunStatus usb_start(void) { usb_running = 1; return BOLUN_OK; }
static BolunStatus usb_suspend(void) { if (!usb_running) return BOLUN_ERROR_NOT_FOUND; usb_running = 0; return BOLUN_OK; }
static BolunStatus usb_resume(void) { usb_running = 1; return BOLUN_OK; }
static BolunStatus usb_stop(void) { usb_running = 0; return BOLUN_OK; }
static const char *usb_diagnostics(void) { return usb_running ? "usb:running" : "usb:stopped"; }

const BolunDriver *bolun_usb_driver(void) {
    static const BolunDriver driver = {
        "usb", BOLUN_DRIVER_USB, usb_probe, usb_init, usb_start, usb_suspend, usb_resume, usb_stop, usb_diagnostics
    };
    return &driver;
}
