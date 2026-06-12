#include <stdio.h>
#include "bolun/apps.h"

static BolunStatus store_launch(BolunFramebuffer *fb, char *out, size_t out_size) {
    if (!fb || !out || out_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    BolunStatus tile = bolun_tile_draw(fb, (BolunRect){8, 28, 120, 60}, "Store", 0xff0066ccu);
    if (tile != BOLUN_OK) return tile;
    snprintf(out, out_size, "Store:catalog=local");
    return BOLUN_OK;
}

const BolunAppDescriptor *bolun_app_store(void) {
    static const BolunAppDescriptor app = {"store", "Store", "network", store_launch};
    return &app;
}
