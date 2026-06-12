#include "bolun/graphics.h"

uint32_t bolun_animation_interpolate_color(uint32_t from, uint32_t to, uint32_t step, uint32_t steps) {
    if (steps == 0) return to;
    if (step > steps) step = steps;
    uint32_t out = 0xff000000u;
    for (int shift = 0; shift <= 16; shift += 8) {
        uint32_t a = (from >> shift) & 0xffu;
        uint32_t b = (to >> shift) & 0xffu;
        uint32_t v = a + ((b > a ? b - a : 0u) * step) / steps - ((a > b ? a - b : 0u) * step) / steps;
        out |= (v & 0xffu) << shift;
    }
    return out;
}
