#include "bolun/driver.h"

static int battery_running;
static unsigned battery_events;

static BolunStatus battery_probe(const BolunBoardProfile *profile) {
    if (!profile) return BOLUN_ERROR_INVALID_ARGUMENT;
    return bolun_hal_validate_profile(profile);
}

static BolunStatus battery_init(const BolunBoardProfile *profile) {
    BolunStatus probe = battery_probe(profile);
    if (probe == BOLUN_ERROR_NOT_FOUND) return BOLUN_OK;
    return probe;
}

static BolunStatus battery_start(void) { battery_running = 1; battery_events++; return BOLUN_OK; }
static BolunStatus battery_suspend(void) { if (!battery_running) return BOLUN_ERROR_NOT_FOUND; battery_running = 0; return BOLUN_OK; }
static BolunStatus battery_resume(void) { battery_running = 1; battery_events++; return BOLUN_OK; }
static BolunStatus battery_stop(void) { battery_running = 0; return BOLUN_OK; }
static const char *battery_diagnostics(void) { return battery_running ? "battery:running" : "battery:stopped"; }

unsigned bolun_battery_event_count(void) { return battery_events; }

const BolunDriver *bolun_battery_driver(void) {
    static const BolunDriver driver = {
        "battery", BOLUN_DRIVER_BATTERY, battery_probe, battery_init, battery_start, battery_suspend, battery_resume, battery_stop, battery_diagnostics
    };
    return &driver;
}
