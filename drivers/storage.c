#include "bolun/driver.h"

static int storage_running;
static unsigned storage_events;

static BolunStatus storage_probe(const BolunBoardProfile *profile) {
    if (!profile) return BOLUN_ERROR_INVALID_ARGUMENT;
    if ((profile->feature_flags & BOLUN_FEATURE_REMOVABLE_STORAGE) == 0) return BOLUN_ERROR_NOT_FOUND;
    return bolun_hal_validate_profile(profile);
}

static BolunStatus storage_init(const BolunBoardProfile *profile) {
    BolunStatus probe = storage_probe(profile);
    if (probe == BOLUN_ERROR_NOT_FOUND) return BOLUN_OK;
    return probe;
}

static BolunStatus storage_start(void) { storage_running = 1; storage_events++; return BOLUN_OK; }
static BolunStatus storage_suspend(void) { if (!storage_running) return BOLUN_ERROR_NOT_FOUND; storage_running = 0; return BOLUN_OK; }
static BolunStatus storage_resume(void) { storage_running = 1; storage_events++; return BOLUN_OK; }
static BolunStatus storage_stop(void) { storage_running = 0; return BOLUN_OK; }
static const char *storage_diagnostics(void) { return storage_running ? "storage:running" : "storage:stopped"; }

unsigned bolun_storage_event_count(void) { return storage_events; }

const BolunDriver *bolun_storage_driver(void) {
    static const BolunDriver driver = {
        "storage", BOLUN_DRIVER_STORAGE, storage_probe, storage_init, storage_start, storage_suspend, storage_resume, storage_stop, storage_diagnostics
    };
    return &driver;
}
