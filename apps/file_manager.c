#include <stdio.h>
#include "bolun/apps.h"

static BolunStatus file_manager_launch(BolunFramebuffer *fb, char *out, size_t out_size) {
    if (!fb || !out || out_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    BolunStatus tile = bolun_tile_draw(fb, (BolunRect){8, 28, 120, 60}, "File Manager", 0xff0066ccu);
    if (tile != BOLUN_OK) return tile;
    snprintf(out, out_size, "File Manager:root=/");
    return BOLUN_OK;
}

const BolunAppDescriptor *bolun_app_file_manager(void) {
    static const BolunAppDescriptor app = {"file_manager", "File Manager", "storage", file_manager_launch};
    return &app;
}
