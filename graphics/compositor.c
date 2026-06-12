#include "bolun/graphics.h"

BolunStatus bolun_compositor_present(BolunFramebuffer *fb, const BolunWindow *windows, size_t count) {
    if (!fb || (!windows && count != 0)) return BOLUN_ERROR_INVALID_ARGUMENT;
    BolunStatus clear = bolun_framebuffer_clear(fb, 0xff000000u);
    if (clear != BOLUN_OK) return clear;
    for (size_t i = 0; i < count; ++i) {
        BolunStatus status = bolun_window_draw(fb, &windows[i]);
        if (status != BOLUN_OK) return status;
    }
    return BOLUN_OK;
}
