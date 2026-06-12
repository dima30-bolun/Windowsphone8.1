#include <stdio.h>
#include "bolun/apps.h"

static BolunStatus gallery_launch(BolunFramebuffer *fb, char *out, size_t out_size) {
    if (!fb || !out || out_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    BolunStatus tile = bolun_tile_draw(fb, (BolunRect){8, 28, 120, 60}, "Gallery", 0xff0066ccu);
    if (tile != BOLUN_OK) return tile;
    snprintf(out, out_size, "Gallery:photos=0");
    return BOLUN_OK;
}

const BolunAppDescriptor *bolun_app_gallery(void) {
    static const BolunAppDescriptor app = {"gallery", "Gallery", "media", gallery_launch};
    return &app;
}
