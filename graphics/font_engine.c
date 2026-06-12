#include "bolun/graphics.h"

BolunStatus bolun_font_draw_text(BolunFramebuffer *fb, int x, int y, const char *text, uint32_t color) {
    if (!fb || !text) return BOLUN_ERROR_INVALID_ARGUMENT;
    int cursor = x;
    for (const char *p = text; *p; ++p) {
        uint32_t glyph_color = color ^ ((uint32_t)(unsigned char)*p << 8);
        BolunStatus status = bolun_framebuffer_fill_rect(fb, (BolunRect){cursor, y, 5, 7}, glyph_color);
        if (status != BOLUN_OK) return status;
        cursor += 6;
    }
    return BOLUN_OK;
}
