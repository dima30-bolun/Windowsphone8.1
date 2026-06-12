#include "bolun/shell.h"
#include "bolun/apps.h"
#include "bolun/services.h"

BolunStatus bolun_shell_render_lock(BolunFramebuffer *fb) {
    if (!fb) return BOLUN_ERROR_INVALID_ARGUMENT;
    BolunStatus clear = bolun_framebuffer_clear(fb, 0xff101820u);
    if (clear != BOLUN_OK) return clear;
    return bolun_font_draw_text(fb, 20, 20, "Bolun Phone", 0xffffffffu);
}

BolunStatus bolun_shell_render_start(BolunFramebuffer *fb) {
    if (!fb) return BOLUN_ERROR_INVALID_ARGUMENT;
    BolunStatus clear = bolun_framebuffer_clear(fb, 0xff000000u);
    if (clear != BOLUN_OK) return clear;
    int x = 8, y = 8;
    for (size_t i = 0; i < bolun_app_count(); ++i) {
        const BolunAppDescriptor *app = bolun_app_at(i);
        BolunStatus tile = bolun_tile_draw(fb, (BolunRect){x, y, 72, 48}, app->name, 0xff004c99u + (uint32_t)i * 257u);
        if (tile != BOLUN_OK) return tile;
        x += 80;
        if (x + 72 > (int)fb->width) { x = 8; y += 56; }
    }
    return BOLUN_OK;
}

BolunStatus bolun_shell_render_notifications(BolunFramebuffer *fb) {
    if (!fb) return BOLUN_ERROR_INVALID_ARGUMENT;
    BolunStatus panel = bolun_framebuffer_fill_rect(fb, (BolunRect){0, 0, (int)fb->width, 42}, 0xff202020u);
    if (panel != BOLUN_OK) return panel;
    return bolun_font_draw_text(fb, 8, 8, "Notifications", 0xffffffffu);
}

BolunStatus bolun_shell_render_settings(BolunFramebuffer *fb) {
    if (!fb) return BOLUN_ERROR_INVALID_ARGUMENT;
    BolunStatus clear = bolun_framebuffer_clear(fb, 0xff111111u);
    if (clear != BOLUN_OK) return clear;
    return bolun_font_draw_text(fb, 8, 8, "Settings", 0xffffffffu);
}

BolunStatus bolun_shell_launch_app(BolunFramebuffer *fb, const char *app_id, char *out, unsigned long out_size) {
    return bolun_app_launch(app_id, fb, out, (size_t)out_size);
}
