#include <stdio.h>
#include "bolun/apps.h"

static BolunStatus email_launch(BolunFramebuffer *fb, char *out, size_t out_size) {
    if (!fb || !out || out_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    BolunStatus tile = bolun_tile_draw(fb, (BolunRect){8, 28, 120, 60}, "Email", 0xff0066ccu);
    if (tile != BOLUN_OK) return tile;
    snprintf(out, out_size, "Email:inbox=0");
    return BOLUN_OK;
}

const BolunAppDescriptor *bolun_app_email(void) {
    static const BolunAppDescriptor app = {"email", "Email", "network", email_launch};
    return &app;
}
