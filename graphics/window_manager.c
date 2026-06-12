#include "bolun/graphics.h"

BolunStatus bolun_window_draw(BolunFramebuffer *fb, const BolunWindow *window) {
    if (!fb || !window || !window->title) return BOLUN_ERROR_INVALID_ARGUMENT;
    if (!window->visible) return BOLUN_OK;
    BolunStatus body = bolun_framebuffer_fill_rect(fb, window->rect, window->color);
    if (body != BOLUN_OK) return body;
    BolunStatus title = bolun_framebuffer_fill_rect(fb, (BolunRect){window->rect.x, window->rect.y, window->rect.w, 18}, 0xff202020u);
    if (title != BOLUN_OK) return title;
    return bolun_font_draw_text(fb, window->rect.x + 4, window->rect.y + 4, window->title, 0xffffffffu);
}
