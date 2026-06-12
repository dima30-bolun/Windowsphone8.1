#include <stdio.h>
#include "bolun/apps.h"

static BolunStatus clock_launch(BolunFramebuffer *fb, char *out, size_t out_size) {
    if (!fb || !out || out_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    BolunStatus tile = bolun_tile_draw(fb, (BolunRect){8, 28, 120, 60}, "Clock", 0xff0066ccu);
    if (tile != BOLUN_OK) return tile;
    snprintf(out, out_size, "Clock:12:00");
    return BOLUN_OK;
}

const BolunAppDescriptor *bolun_app_clock(void) {
    static const BolunAppDescriptor app = {"clock", "Clock", "time", clock_launch};
    return &app;
}
