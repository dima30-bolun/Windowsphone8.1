#ifndef BOLUN_CPP_HPP
#define BOLUN_CPP_HPP

#include "bolun/status.h"
#include "bolun/apps.h"
#include "bolun/services.h"
#include "bolun/assistant.h"

namespace bolun {
inline const char *status_string(BolunStatus status) { return bolun_status_string(status); }
inline size_t app_count() { return bolun_app_count(); }
}

#endif
