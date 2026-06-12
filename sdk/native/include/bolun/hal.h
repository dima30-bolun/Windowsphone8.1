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

#define BOLUN_FEATURE_TOUCH      0x0001u
#define BOLUN_FEATURE_CAMERA     0x0002u
#define BOLUN_FEATURE_NFC        0x0004u
#define BOLUN_FEATURE_LTE        0x0008u
#define BOLUN_FEATURE_GLANCE     0x0010u
#define BOLUN_FEATURE_WIRELESS_CHARGING 0x0020u

size_t bolun_hal_profile_count(void);
const BolunBoardProfile *bolun_hal_profile_at(size_t index);
const BolunBoardProfile *bolun_hal_find_profile(const char *model);
BolunStatus bolun_hal_validate_profile(const BolunBoardProfile *profile);
const char *bolun_hal_arch_name(BolunArch arch);
const char *bolun_hal_soc_name(BolunSoc soc);

#ifdef __cplusplus
}
#endif

#endif
