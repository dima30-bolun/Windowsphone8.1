#include <stdio.h>
#include "bolun/apps.h"

static BolunStatus messaging_launch(BolunFramebuffer *fb, char *out, size_t out_size) {
    if (!fb || !out || out_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    BolunStatus tile = bolun_tile_draw(fb, (BolunRect){8, 28, 120, 60}, "Messaging", 0xff0066ccu);
    if (tile != BOLUN_OK) return tile;
    snprintf(out, out_size, "Messaging:inbox=0");
    return BOLUN_OK;
}

const BolunAppDescriptor *bolun_app_messaging(void) {
    static const BolunAppDescriptor app = {"messaging", "Messaging", "messaging", messaging_launch};
    return &app;
}
