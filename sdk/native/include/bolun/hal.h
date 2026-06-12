#ifndef BOLUN_HAL_H
#define BOLUN_HAL_H

#include <stddef.h>
#include <stdint.h>
#include "bolun/status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum BolunArch {
    BOLUN_ARCH_ARMV7,
    BOLUN_ARCH_ARMV8,
    BOLUN_ARCH_ARM64
} BolunArch;

typedef enum BolunSoc {
    BOLUN_SOC_SNAPDRAGON_S4,
    BOLUN_SOC_SNAPDRAGON_200,
    BOLUN_SOC_SNAPDRAGON_400,
    BOLUN_SOC_SNAPDRAGON_800,
    BOLUN_SOC_SNAPDRAGON_808,
    BOLUN_SOC_SNAPDRAGON_810
} BolunSoc;

typedef struct BolunBoardProfile {
    const char *model;
    BolunArch arch;
    BolunSoc soc;
    uint32_t ram_mb;
    uint32_t display_width;
    uint32_t display_height;
    uint32_t feature_flags;
    const char *quirks;
} BolunBoardProfile;

#define BOLUN_HAL_MAX_GPIO_PINS 192

typedef enum BolunHalBus {
    BOLUN_HAL_BUS_GPIO,
    BOLUN_HAL_BUS_I2C,
    BOLUN_HAL_BUS_SPI,
    BOLUN_HAL_BUS_UART,
    BOLUN_HAL_BUS_DMA,
    BOLUN_HAL_BUS_TIMER,
    BOLUN_HAL_BUS_WATCHDOG,
    BOLUN_HAL_BUS_CLOCK
} BolunHalBus;

typedef struct BolunHalControllerState {
    const BolunBoardProfile *profile;
    uint8_t gpio[BOLUN_HAL_MAX_GPIO_PINS];
    uint32_t cpu_frequency_mhz;
    uint32_t watchdog_timeout_ms;
    uint64_t timer_ticks;
} BolunHalControllerState;

#define BOLUN_FEATURE_TOUCH      0x0001u
#define BOLUN_FEATURE_CAMERA     0x0002u
#define BOLUN_FEATURE_NFC        0x0004u
#define BOLUN_FEATURE_LTE        0x0008u
#define BOLUN_FEATURE_GLANCE     0x0010u
#define BOLUN_FEATURE_WIRELESS_CHARGING 0x0020u
#define BOLUN_FEATURE_REMOVABLE_STORAGE 0x0040u
#define BOLUN_FEATURE_FRONT_CAMERA 0x0080u
#define BOLUN_FEATURE_HIGH_RES_CAMERA 0x0100u

size_t bolun_hal_profile_count(void);
const BolunBoardProfile *bolun_hal_profile_at(size_t index);
const BolunBoardProfile *bolun_hal_find_profile(const char *model);
BolunStatus bolun_hal_validate_profile(const BolunBoardProfile *profile);
const char *bolun_hal_arch_name(BolunArch arch);
const char *bolun_hal_soc_name(BolunSoc soc);
BolunStatus bolun_hal_controller_init(BolunHalControllerState *state, const BolunBoardProfile *profile);
BolunStatus bolun_hal_gpio_write(BolunHalControllerState *state, uint32_t pin, uint8_t value);
BolunStatus bolun_hal_gpio_read(const BolunHalControllerState *state, uint32_t pin, uint8_t *value_out);
BolunStatus bolun_hal_i2c_transfer(BolunHalControllerState *state, uint8_t address, const uint8_t *tx, size_t tx_len, uint8_t *rx, size_t rx_len);
BolunStatus bolun_hal_spi_transfer(BolunHalControllerState *state, const uint8_t *tx, uint8_t *rx, size_t len);
BolunStatus bolun_hal_uart_write(BolunHalControllerState *state, const char *text, size_t *written_out);
BolunStatus bolun_hal_dma_copy(BolunHalControllerState *state, void *dst, const void *src, size_t bytes);
BolunStatus bolun_hal_timer_tick(BolunHalControllerState *state, uint64_t delta_ticks);
BolunStatus bolun_hal_watchdog_pet(BolunHalControllerState *state);
BolunStatus bolun_hal_set_cpu_frequency(BolunHalControllerState *state, uint32_t mhz);

#ifdef __cplusplus
}
#endif

#endif
