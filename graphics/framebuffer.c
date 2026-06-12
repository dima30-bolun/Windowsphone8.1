#include "bolun/graphics.h"

BolunStatus bolun_framebuffer_init(BolunFramebuffer *fb, uint32_t width, uint32_t height, uint32_t *pixels, size_t pixel_count) {
    if (!fb || !pixels || width == 0 || height == 0 || pixel_count < (size_t)width * (size_t)height) return BOLUN_ERROR_INVALID_ARGUMENT;
    fb->width = width;
    fb->height = height;
    fb->pixels = pixels;
    fb->pixel_count = pixel_count;
    return BOLUN_OK;
}

BolunStatus bolun_framebuffer_clear(BolunFramebuffer *fb, uint32_t color) {
    if (!fb || !fb->pixels) return BOLUN_ERROR_INVALID_ARGUMENT;
    for (size_t i = 0; i < fb->pixel_count; ++i) fb->pixels[i] = color;
    return BOLUN_OK;
}

BolunStatus bolun_framebuffer_fill_rect(BolunFramebuffer *fb, BolunRect rect, uint32_t color) {
    if (!fb || !fb->pixels || rect.w < 0 || rect.h < 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    int x0 = rect.x < 0 ? 0 : rect.x;
    int y0 = rect.y < 0 ? 0 : rect.y;
    int x1 = rect.x + rect.w > (int)fb->width ? (int)fb->width : rect.x + rect.w;
    int y1 = rect.y + rect.h > (int)fb->height ? (int)fb->height : rect.y + rect.h;
    for (int y = y0; y < y1; ++y) for (int x = x0; x < x1; ++x) fb->pixels[(size_t)y * fb->width + (size_t)x] = color;
    return BOLUN_OK;
}
