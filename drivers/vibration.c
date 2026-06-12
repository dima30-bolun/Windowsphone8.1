#include "bolun/driver.h"

static int vibration_running;
static unsigned vibration_events;

static BolunStatus vibration_probe(const BolunBoardProfile *profile) {
    if (!profile) return BOLUN_ERROR_INVALID_ARGUMENT;
    return bolun_hal_validate_profile(profile);
}

static BolunStatus vibration_init(const BolunBoardProfile *profile) {
    BolunStatus probe = vibration_probe(profile);
    if (probe == BOLUN_ERROR_NOT_FOUND) return BOLUN_OK;
    return probe;
}

static BolunStatus vibration_start(void) { vibration_running = 1; vibration_events++; return BOLUN_OK; }
static BolunStatus vibration_suspend(void) { if (!vibration_running) return BOLUN_ERROR_NOT_FOUND; vibration_running = 0; return BOLUN_OK; }
static BolunStatus vibration_resume(void) { vibration_running = 1; vibration_events++; return BOLUN_OK; }
static BolunStatus vibration_stop(void) { vibration_running = 0; return BOLUN_OK; }
static const char *vibration_diagnostics(void) { return vibration_running ? "vibration:running" : "vibration:stopped"; }

unsigned bolun_vibration_event_count(void) { return vibration_events; }

const BolunDriver *bolun_vibration_driver(void) {
    static const BolunDriver driver = {
        "vibration", BOLUN_DRIVER_VIBRATION, vibration_probe, vibration_init, vibration_start, vibration_suspend, vibration_resume, vibration_stop, vibration_diagnostics
    };
    return &driver;
}
