#ifndef BOLUN_DRIVER_H
#define BOLUN_DRIVER_H

#include <stddef.h>
#include "bolun/status.h"
#include "bolun/hal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum BolunDriverKind {
    BOLUN_DRIVER_DISPLAY,
    BOLUN_DRIVER_TOUCH,
    BOLUN_DRIVER_AUDIO,
    BOLUN_DRIVER_CAMERA,
    BOLUN_DRIVER_BLUETOOTH,
    BOLUN_DRIVER_WIFI,
    BOLUN_DRIVER_RADIO_4G5G,
    BOLUN_DRIVER_GPS,
    BOLUN_DRIVER_NFC,
    BOLUN_DRIVER_USB,
    BOLUN_DRIVER_SENSORS,
    BOLUN_DRIVER_CELLULAR,
    BOLUN_DRIVER_POWER_MANAGEMENT,
    BOLUN_DRIVER_STORAGE,
    BOLUN_DRIVER_BATTERY,
    BOLUN_DRIVER_CHARGING,
    BOLUN_DRIVER_VIBRATION,
    BOLUN_DRIVER_LTE
} BolunDriverKind;

typedef struct BolunDriver {
    const char *name;
    BolunDriverKind kind;
    BolunStatus (*probe)(const BolunBoardProfile *profile);
    BolunStatus (*init)(const BolunBoardProfile *profile);
    BolunStatus (*start)(void);
    BolunStatus (*suspend)(void);
    BolunStatus (*resume)(void);
    BolunStatus (*stop)(void);
    const char *(*diagnostics)(void);
} BolunDriver;

size_t bolun_driver_count(void);
const BolunDriver *bolun_driver_at(size_t index);
BolunStatus bolun_driver_start_all(const BolunBoardProfile *profile);

#ifdef __cplusplus
}
#endif

#endif
