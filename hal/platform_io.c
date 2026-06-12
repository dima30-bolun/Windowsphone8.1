#include <string.h>
#include "bolun/hal.h"

static uint32_t default_frequency_for_soc(BolunSoc soc) {
    switch (soc) {
        case BOLUN_SOC_SNAPDRAGON_S4: return 1000;
        case BOLUN_SOC_SNAPDRAGON_200: return 1200;
        case BOLUN_SOC_SNAPDRAGON_400: return 1200;
        case BOLUN_SOC_SNAPDRAGON_800: return 2200;
        case BOLUN_SOC_SNAPDRAGON_808: return 1800;
        case BOLUN_SOC_SNAPDRAGON_810: return 2000;
        default: return 800;
    }
}

BolunStatus bolun_hal_controller_init(BolunHalControllerState *state, const BolunBoardProfile *profile) {
    if (!state || bolun_hal_validate_profile(profile) != BOLUN_OK) return BOLUN_ERROR_INVALID_ARGUMENT;
    memset(state, 0, sizeof(*state));
    state->profile = profile;
    state->cpu_frequency_mhz = default_frequency_for_soc(profile->soc);
    state->watchdog_timeout_ms = 10000;
    return BOLUN_OK;
}

BolunStatus bolun_hal_gpio_write(BolunHalControllerState *state, uint32_t pin, uint8_t value) {
    if (!state || pin >= BOLUN_HAL_MAX_GPIO_PINS) return BOLUN_ERROR_INVALID_ARGUMENT;
    state->gpio[pin] = value ? 1u : 0u;
    return BOLUN_OK;
}

BolunStatus bolun_hal_gpio_read(const BolunHalControllerState *state, uint32_t pin, uint8_t *value_out) {
    if (!state || !value_out || pin >= BOLUN_HAL_MAX_GPIO_PINS) return BOLUN_ERROR_INVALID_ARGUMENT;
    *value_out = state->gpio[pin];
    return BOLUN_OK;
}

BolunStatus bolun_hal_i2c_transfer(BolunHalControllerState *state, uint8_t address, const uint8_t *tx, size_t tx_len, uint8_t *rx, size_t rx_len) {
    if (!state || address == 0 || (!tx && tx_len) || (!rx && rx_len)) return BOLUN_ERROR_INVALID_ARGUMENT;
    for (size_t i = 0; i < rx_len; ++i) rx[i] = (uint8_t)(address + i + (tx_len ? tx[0] : 0));
    return BOLUN_OK;
}

BolunStatus bolun_hal_spi_transfer(BolunHalControllerState *state, const uint8_t *tx, uint8_t *rx, size_t len) {
    if (!state || !tx || !rx) return BOLUN_ERROR_INVALID_ARGUMENT;
    for (size_t i = 0; i < len; ++i) rx[i] = (uint8_t)(tx[i] ^ 0xffu);
    return BOLUN_OK;
}

BolunStatus bolun_hal_uart_write(BolunHalControllerState *state, const char *text, size_t *written_out) {
    if (!state || !text || !written_out) return BOLUN_ERROR_INVALID_ARGUMENT;
    *written_out = strlen(text);
    return BOLUN_OK;
}

BolunStatus bolun_hal_dma_copy(BolunHalControllerState *state, void *dst, const void *src, size_t bytes) {
    if (!state || !dst || !src) return BOLUN_ERROR_INVALID_ARGUMENT;
    memcpy(dst, src, bytes);
    return BOLUN_OK;
}

BolunStatus bolun_hal_timer_tick(BolunHalControllerState *state, uint64_t delta_ticks) {
    if (!state) return BOLUN_ERROR_INVALID_ARGUMENT;
    state->timer_ticks += delta_ticks;
    return BOLUN_OK;
}

BolunStatus bolun_hal_watchdog_pet(BolunHalControllerState *state) {
    if (!state) return BOLUN_ERROR_INVALID_ARGUMENT;
    state->watchdog_timeout_ms = 10000;
    return BOLUN_OK;
}

BolunStatus bolun_hal_set_cpu_frequency(BolunHalControllerState *state, uint32_t mhz) {
    if (!state || mhz < 200 || mhz > 2600) return BOLUN_ERROR_INVALID_ARGUMENT;
    state->cpu_frequency_mhz = mhz;
    return BOLUN_OK;
}
