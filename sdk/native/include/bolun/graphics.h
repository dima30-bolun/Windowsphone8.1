#ifndef BOLUN_GRAPHICS_H
#define BOLUN_GRAPHICS_H

#include <stddef.h>
#include <stdint.h>
#include "bolun/status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct BolunFramebuffer {
    uint32_t width;
    uint32_t height;
    uint32_t *pixels;
    size_t pixel_count;
} BolunFramebuffer;

typedef struct BolunRect { int x; int y; int w; int h; } BolunRect;

typedef struct BolunWindow {
    int id;
    BolunRect rect;
    uint32_t color;
    const char *title;
    int visible;
} BolunWindow;

BolunStatus bolun_framebuffer_init(BolunFramebuffer *fb, uint32_t width, uint32_t height, uint32_t *pixels, size_t pixel_count);
BolunStatus bolun_framebuffer_clear(BolunFramebuffer *fb, uint32_t color);
BolunStatus bolun_framebuffer_fill_rect(BolunFramebuffer *fb, BolunRect rect, uint32_t color);
BolunStatus bolun_font_draw_text(BolunFramebuffer *fb, int x, int y, const char *text, uint32_t color);
BolunStatus bolun_tile_draw(BolunFramebuffer *fb, BolunRect rect, const char *label, uint32_t color);
BolunStatus bolun_window_draw(BolunFramebuffer *fb, const BolunWindow *window);
BolunStatus bolun_compositor_present(BolunFramebuffer *fb, const BolunWindow *windows, size_t count);
uint32_t bolun_animation_interpolate_color(uint32_t from, uint32_t to, uint32_t step, uint32_t steps);
const char *bolun_gpu_backend_name(void);

#ifdef __cplusplus
}
#endif

#endif
