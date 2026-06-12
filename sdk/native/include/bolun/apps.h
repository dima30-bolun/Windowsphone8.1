#ifndef BOLUN_APPS_H
#define BOLUN_APPS_H

#include <stddef.h>
#include "bolun/status.h"
#include "bolun/graphics.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct BolunAppDescriptor {
    const char *id;
    const char *name;
    const char *required_capability;
    BolunStatus (*launch)(BolunFramebuffer *fb, char *out, size_t out_size);
} BolunAppDescriptor;

size_t bolun_app_count(void);
const BolunAppDescriptor *bolun_app_at(size_t index);
const BolunAppDescriptor *bolun_app_find(const char *id);
BolunStatus bolun_app_launch(const char *id, BolunFramebuffer *fb, char *out, size_t out_size);

#ifdef __cplusplus
}
#endif

#endif
