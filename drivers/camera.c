#include <stdio.h>
#include "bolun/driver.h"

static int camera_running;

static BolunStatus camera_probe(const BolunBoardProfile *profile) {
    if (!profile) return BOLUN_ERROR_INVALID_ARGUMENT;
    if (BOLUN_DRIVER_CAMERA == BOLUN_DRIVER_NFC && (profile->feature_flags & BOLUN_FEATURE_NFC) == 0) return BOLUN_ERROR_NOT_FOUND;
    if (BOLUN_DRIVER_CAMERA == BOLUN_DRIVER_CAMERA && (profile->feature_flags & BOLUN_FEATURE_CAMERA) == 0) return BOLUN_ERROR_NOT_FOUND;
    return bolun_hal_validate_profile(profile);
}

static BolunStatus camera_init(const BolunBoardProfile *profile) {
    BolunStatus probe = camera_probe(profile);
    if (probe == BOLUN_ERROR_NOT_FOUND) return BOLUN_OK;
    return probe;
}

static BolunStatus camera_start(void) { camera_running = 1; return BOLUN_OK; }
static BolunStatus camera_suspend(void) { if (!camera_running) return BOLUN_ERROR_NOT_FOUND; camera_running = 0; return BOLUN_OK; }
static BolunStatus camera_resume(void) { camera_running = 1; return BOLUN_OK; }
static BolunStatus camera_stop(void) { camera_running = 0; return BOLUN_OK; }
static const char *camera_diagnostics(void) { return camera_running ? "camera:running" : "camera:stopped"; }

const BolunDriver *bolun_camera_driver(void) {
    static const BolunDriver driver = {
        "camera", BOLUN_DRIVER_CAMERA, camera_probe, camera_init, camera_start, camera_suspend, camera_resume, camera_stop, camera_diagnostics
    };
    return &driver;
}
