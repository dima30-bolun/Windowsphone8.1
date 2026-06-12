#include <stdio.h>
#include "bolun/driver.h"

static int nfc_running;

static BolunStatus nfc_probe(const BolunBoardProfile *profile) {
    if (!profile) return BOLUN_ERROR_INVALID_ARGUMENT;
    if (BOLUN_DRIVER_NFC == BOLUN_DRIVER_NFC && (profile->feature_flags & BOLUN_FEATURE_NFC) == 0) return BOLUN_ERROR_NOT_FOUND;
    if (BOLUN_DRIVER_NFC == BOLUN_DRIVER_CAMERA && (profile->feature_flags & BOLUN_FEATURE_CAMERA) == 0) return BOLUN_ERROR_NOT_FOUND;
    return bolun_hal_validate_profile(profile);
}

static BolunStatus nfc_init(const BolunBoardProfile *profile) {
    BolunStatus probe = nfc_probe(profile);
    if (probe == BOLUN_ERROR_NOT_FOUND) return BOLUN_OK;
    return probe;
}

static BolunStatus nfc_start(void) { nfc_running = 1; return BOLUN_OK; }
static BolunStatus nfc_suspend(void) { if (!nfc_running) return BOLUN_ERROR_NOT_FOUND; nfc_running = 0; return BOLUN_OK; }
static BolunStatus nfc_resume(void) { nfc_running = 1; return BOLUN_OK; }
static BolunStatus nfc_stop(void) { nfc_running = 0; return BOLUN_OK; }
static const char *nfc_diagnostics(void) { return nfc_running ? "nfc:running" : "nfc:stopped"; }

const BolunDriver *bolun_nfc_driver(void) {
    static const BolunDriver driver = {
        "nfc", BOLUN_DRIVER_NFC, nfc_probe, nfc_init, nfc_start, nfc_suspend, nfc_resume, nfc_stop, nfc_diagnostics
    };
    return &driver;
}
