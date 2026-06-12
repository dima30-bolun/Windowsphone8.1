#ifndef BOLUN_SHELL_H
#define BOLUN_SHELL_H

#include "bolun/status.h"
#include "bolun/graphics.h"

#ifdef __cplusplus
extern "C" {
#endif

BolunStatus bolun_shell_render_lock(BolunFramebuffer *fb);
BolunStatus bolun_shell_render_start(BolunFramebuffer *fb);
BolunStatus bolun_shell_render_notifications(BolunFramebuffer *fb);
BolunStatus bolun_shell_render_settings(BolunFramebuffer *fb);
BolunStatus bolun_shell_launch_app(BolunFramebuffer *fb, const char *app_id, char *out, unsigned long out_size);

#ifdef __cplusplus
}
#endif

#endif
