#include "bolun/driver.h"

static int charging_running;
static unsigned charging_events;

static BolunStatus charging_probe(const BolunBoardProfile *profile) {
    if (!profile) return BOLUN_ERROR_INVALID_ARGUMENT;
    return bolun_hal_validate_profile(profile);
}

static BolunStatus charging_init(const BolunBoardProfile *profile) {
    BolunStatus probe = charging_probe(profile);
    if (probe == BOLUN_ERROR_NOT_FOUND) return BOLUN_OK;
    return probe;
}

static BolunStatus charging_start(void) { charging_running = 1; charging_events++; return BOLUN_OK; }
static BolunStatus charging_suspend(void) { if (!charging_running) return BOLUN_ERROR_NOT_FOUND; charging_running = 0; return BOLUN_OK; }
static BolunStatus charging_resume(void) { charging_running = 1; charging_events++; return BOLUN_OK; }
static BolunStatus charging_stop(void) { charging_running = 0; return BOLUN_OK; }
static const char *charging_diagnostics(void) { return charging_running ? "charging:running" : "charging:stopped"; }

unsigned bolun_charging_event_count(void) { return charging_events; }

const BolunDriver *bolun_charging_driver(void) {
    static const BolunDriver driver = {
        "charging", BOLUN_DRIVER_CHARGING, charging_probe, charging_init, charging_start, charging_suspend, charging_resume, charging_stop, charging_diagnostics
    };
    return &driver;
}
