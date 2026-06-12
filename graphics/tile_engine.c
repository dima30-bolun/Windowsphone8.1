#include "bolun/graphics.h"

BolunStatus bolun_tile_draw(BolunFramebuffer *fb, BolunRect rect, const char *label, uint32_t color) {
    if (!fb || !label) return BOLUN_ERROR_INVALID_ARGUMENT;
    BolunStatus fill = bolun_framebuffer_fill_rect(fb, rect, color);
    if (fill != BOLUN_OK) return fill;
    return bolun_font_draw_text(fb, rect.x + 6, rect.y + 6, label, 0xffffffffu);
}
