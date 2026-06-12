#include <string.h>
#include "bolun/apps.h"

const BolunAppDescriptor *bolun_app_calculator(void);
const BolunAppDescriptor *bolun_app_calendar(void);
const BolunAppDescriptor *bolun_app_clock(void);
const BolunAppDescriptor *bolun_app_contacts(void);
const BolunAppDescriptor *bolun_app_messaging(void);
const BolunAppDescriptor *bolun_app_phone(void);
const BolunAppDescriptor *bolun_app_camera(void);
const BolunAppDescriptor *bolun_app_gallery(void);
const BolunAppDescriptor *bolun_app_music(void);
const BolunAppDescriptor *bolun_app_video(void);
const BolunAppDescriptor *bolun_app_browser(void);
const BolunAppDescriptor *bolun_app_file_manager(void);
const BolunAppDescriptor *bolun_app_notes(void);
const BolunAppDescriptor *bolun_app_maps(void);
const BolunAppDescriptor *bolun_app_diagnostics(void);
const BolunAppDescriptor *bolun_app_terminal(void);
const BolunAppDescriptor *bolun_app_store(void);
const BolunAppDescriptor *bolun_app_email(void);
const BolunAppDescriptor *bolun_app_settings(void);

typedef const BolunAppDescriptor *(*AppFactory)(void);
static AppFactory apps[] = {
    bolun_app_calculator,
    bolun_app_calendar,
    bolun_app_clock,
    bolun_app_contacts,
    bolun_app_messaging,
    bolun_app_phone,
    bolun_app_camera,
    bolun_app_gallery,
    bolun_app_music,
    bolun_app_video,
    bolun_app_browser,
    bolun_app_file_manager,
    bolun_app_notes,
    bolun_app_maps,
    bolun_app_diagnostics,
    bolun_app_terminal,
    bolun_app_store,
    bolun_app_email,
    bolun_app_settings
};

size_t bolun_app_count(void) { return sizeof(apps) / sizeof(apps[0]); }
const BolunAppDescriptor *bolun_app_at(size_t index) { return index < bolun_app_count() ? apps[index]() : 0; }
const BolunAppDescriptor *bolun_app_find(const char *id) {
    if (!id) return 0;
    for (size_t i = 0; i < bolun_app_count(); ++i) { const BolunAppDescriptor *app = bolun_app_at(i); if (strcmp(app->id, id) == 0) return app; }
    return 0;
}
BolunStatus bolun_app_launch(const char *id, BolunFramebuffer *fb, char *out, size_t out_size) {
    const BolunAppDescriptor *app = bolun_app_find(id);
    if (!app) return BOLUN_ERROR_NOT_FOUND;
    return app->launch(fb, out, out_size);
}
