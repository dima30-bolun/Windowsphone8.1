#ifndef BOLUN_ASSISTANT_H
#define BOLUN_ASSISTANT_H

#include <stddef.h>
#include "bolun/status.h"
#include "bolun/license.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum BolunAssistantCommandKind {
    BOLUN_ASSISTANT_OPEN_APP,
    BOLUN_ASSISTANT_CALL_CONTACT,
    BOLUN_ASSISTANT_PLAY_MUSIC,
    BOLUN_ASSISTANT_ENABLE_BLUETOOTH,
    BOLUN_ASSISTANT_OPEN_SETTINGS,
    BOLUN_ASSISTANT_SET_ALARM,
    BOLUN_ASSISTANT_AI_QUERY
} BolunAssistantCommandKind;

typedef struct BolunAssistantResult {
    BolunAssistantCommandKind kind;
    int requires_ai_license;
    char action[64];
    char detail[96];
} BolunAssistantResult;

BolunStatus bolun_assistant_execute(BolunLicenseState *license, const char *command, BolunAssistantResult *result);
BolunStatus bolun_assistant_format_result(const BolunAssistantResult *result, char *out, size_t out_size);

#ifdef __cplusplus
}
#endif

#endif
