#include <string.h>
#include "bolun/hal.h"

static const BolunBoardProfile profiles[] = {
    {"Lumia 520", BOLUN_ARCH_ARMV7, BOLUN_SOC_SNAPDRAGON_S4, 512, 480, 800, BOLUN_FEATURE_TOUCH, "low_ram;legacy_gpu"},
    {"Lumia 530", BOLUN_ARCH_ARMV7, BOLUN_SOC_SNAPDRAGON_200, 512, 480, 854, BOLUN_FEATURE_TOUCH, "low_ram"},
    {"Lumia 630", BOLUN_ARCH_ARMV7, BOLUN_SOC_SNAPDRAGON_400, 512, 480, 854, BOLUN_FEATURE_TOUCH, "no_flash"},
    {"Lumia 735", BOLUN_ARCH_ARMV7, BOLUN_SOC_SNAPDRAGON_400, 1024, 720, 1280, BOLUN_FEATURE_TOUCH | BOLUN_FEATURE_CAMERA | BOLUN_FEATURE_LTE | BOLUN_FEATURE_NFC, "oled_panel"},
    {"Lumia 830", BOLUN_ARCH_ARMV7, BOLUN_SOC_SNAPDRAGON_400, 1024, 720, 1280, BOLUN_FEATURE_TOUCH | BOLUN_FEATURE_CAMERA | BOLUN_FEATURE_LTE | BOLUN_FEATURE_NFC, "pureview_camera"},
    {"Lumia 930", BOLUN_ARCH_ARMV7, BOLUN_SOC_SNAPDRAGON_800, 2048, 1080, 1920, BOLUN_FEATURE_TOUCH | BOLUN_FEATURE_CAMERA | BOLUN_FEATURE_LTE | BOLUN_FEATURE_NFC | BOLUN_FEATURE_WIRELESS_CHARGING, "high_dpi"},
    {"Lumia 950", BOLUN_ARCH_ARM64, BOLUN_SOC_SNAPDRAGON_808, 3072, 1440, 2560, BOLUN_FEATURE_TOUCH | BOLUN_FEATURE_CAMERA | BOLUN_FEATURE_LTE | BOLUN_FEATURE_NFC | BOLUN_FEATURE_GLANCE, "usb_c;iris"},
    {"Lumia 950 XL", BOLUN_ARCH_ARM64, BOLUN_SOC_SNAPDRAGON_810, 3072, 1440, 2560, BOLUN_FEATURE_TOUCH | BOLUN_FEATURE_CAMERA | BOLUN_FEATURE_LTE | BOLUN_FEATURE_NFC | BOLUN_FEATURE_GLANCE, "thermal_big_little;usb_c;iris"}
};

size_t bolun_hal_profile_count(void) { return sizeof(profiles) / sizeof(profiles[0]); }

const BolunBoardProfile *bolun_hal_profile_at(size_t index) {
    return index < bolun_hal_profile_count() ? &profiles[index] : 0;
}

const BolunBoardProfile *bolun_hal_find_profile(const char *model) {
    if (!model) return 0;
    for (size_t i = 0; i < bolun_hal_profile_count(); ++i) {
        if (strcmp(profiles[i].model, model) == 0) return &profiles[i];
    }
    return 0;
}

BolunStatus bolun_hal_validate_profile(const BolunBoardProfile *profile) {
    if (!profile || !profile->model || !profile->quirks) return BOLUN_ERROR_INVALID_ARGUMENT;
    if (profile->ram_mb < 512 || profile->display_width < 480 || profile->display_height < 800) return BOLUN_ERROR_INTEGRITY;
    if ((profile->feature_flags & BOLUN_FEATURE_TOUCH) == 0) return BOLUN_ERROR_INTEGRITY;
    return BOLUN_OK;
}

const char *bolun_hal_arch_name(BolunArch arch) {
    switch (arch) {
        case BOLUN_ARCH_ARMV7: return "ARMv7";
        case BOLUN_ARCH_ARMV8: return "ARMv8";
        case BOLUN_ARCH_ARM64: return "ARM64";
        default: return "unknown";
    }
}

const char *bolun_hal_soc_name(BolunSoc soc) {
    switch (soc) {
        case BOLUN_SOC_SNAPDRAGON_S4: return "Qualcomm Snapdragon S4";
        case BOLUN_SOC_SNAPDRAGON_200: return "Qualcomm Snapdragon 200";
        case BOLUN_SOC_SNAPDRAGON_400: return "Qualcomm Snapdragon 400";
        case BOLUN_SOC_SNAPDRAGON_800: return "Qualcomm Snapdragon 800";
        case BOLUN_SOC_SNAPDRAGON_808: return "Qualcomm Snapdragon 808";
        case BOLUN_SOC_SNAPDRAGON_810: return "Qualcomm Snapdragon 810";
        default: return "unknown";
    }
}
