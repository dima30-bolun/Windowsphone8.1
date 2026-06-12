#include <stdio.h>
#include "bolun/apps.h"

static BolunStatus maps_launch(BolunFramebuffer *fb, char *out, size_t out_size) {
    if (!fb || !out || out_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    BolunStatus tile = bolun_tile_draw(fb, (BolunRect){8, 28, 120, 60}, "Maps", 0xff0066ccu);
    if (tile != BOLUN_OK) return tile;
    snprintf(out, out_size, "Maps:map=offline");
    return BOLUN_OK;
}

const BolunAppDescriptor *bolun_app_maps(void) {
    static const BolunAppDescriptor app = {"maps", "Maps", "location", maps_launch};
    return &app;
}
