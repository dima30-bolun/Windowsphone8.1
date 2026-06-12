#include "bolun/driver.h"

const BolunDriver *bolun_display_driver(void);
const BolunDriver *bolun_touch_driver(void);
const BolunDriver *bolun_audio_driver(void);
const BolunDriver *bolun_camera_driver(void);
const BolunDriver *bolun_bluetooth_driver(void);
const BolunDriver *bolun_wifi_driver(void);
const BolunDriver *bolun_radio_4g5g_driver(void);
const BolunDriver *bolun_gps_driver(void);
const BolunDriver *bolun_nfc_driver(void);
const BolunDriver *bolun_usb_driver(void);
const BolunDriver *bolun_sensors_driver(void);
const BolunDriver *bolun_cellular_driver(void);
const BolunDriver *bolun_power_management_driver(void);


typedef const BolunDriver *(*DriverFactory)(void);
static DriverFactory factories[] = {
    bolun_display_driver,
    bolun_touch_driver,
    bolun_audio_driver,
    bolun_camera_driver,
    bolun_bluetooth_driver,
    bolun_wifi_driver,
    bolun_radio_4g5g_driver,
    bolun_gps_driver,
    bolun_nfc_driver,
    bolun_usb_driver,
    bolun_sensors_driver,
    bolun_cellular_driver,
    bolun_power_management_driver
};

size_t bolun_driver_count(void) { return sizeof(factories) / sizeof(factories[0]); }

const BolunDriver *bolun_driver_at(size_t index) {
    return index < bolun_driver_count() ? factories[index]() : 0;
}

BolunStatus bolun_driver_start_all(const BolunBoardProfile *profile) {
    if (!profile) return BOLUN_ERROR_INVALID_ARGUMENT;
    for (size_t i = 0; i < bolun_driver_count(); ++i) {
        const BolunDriver *driver = bolun_driver_at(i);
        BolunStatus init = driver->init(profile);
        if (init != BOLUN_OK) return init;
        BolunStatus start = driver->start();
        if (start != BOLUN_OK) return start;
    }
    return BOLUN_OK;
}
