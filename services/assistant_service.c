#include <stdio.h>
#include <string.h>
#include "bolun/assistant.h"
#include "bolun/services.h"

static int starts_with(const char *text, const char *prefix) {
    return strncmp(text, prefix, strlen(prefix)) == 0;
}

BolunStatus bolun_assistant_execute(BolunLicenseState *license, const char *command, BolunAssistantResult *result) {
    if (!command || !result) return BOLUN_ERROR_INVALID_ARGUMENT;
    memset(result, 0, sizeof(*result));
    if (starts_with(command, "open ")) {
        result->kind = BOLUN_ASSISTANT_OPEN_APP;
        snprintf(result->action, sizeof(result->action), "open_app");
        snprintf(result->detail, sizeof(result->detail), "%s", command + 5);
        return BOLUN_OK;
    }
    if (starts_with(command, "call ")) {
        result->kind = BOLUN_ASSISTANT_CALL_CONTACT;
        snprintf(result->action, sizeof(result->action), "call_contact");
        snprintf(result->detail, sizeof(result->detail), "%s", command + 5);
        return BOLUN_OK;
    }
    if (strcmp(command, "play music") == 0) {
        result->kind = BOLUN_ASSISTANT_PLAY_MUSIC;
        snprintf(result->action, sizeof(result->action), "play_music");
        snprintf(result->detail, sizeof(result->detail), "music");
        return BOLUN_OK;
    }
    if (strcmp(command, "enable bluetooth") == 0) {
        result->kind = BOLUN_ASSISTANT_ENABLE_BLUETOOTH;
        snprintf(result->action, sizeof(result->action), "enable_bluetooth");
        snprintf(result->detail, sizeof(result->detail), "bluetooth:on");
        return BOLUN_OK;
    }
    if (strcmp(command, "open settings") == 0) {
        result->kind = BOLUN_ASSISTANT_OPEN_SETTINGS;
        snprintf(result->action, sizeof(result->action), "open_settings");
        snprintf(result->detail, sizeof(result->detail), "settings");
        return BOLUN_OK;
    }
    if (starts_with(command, "set alarm ")) {
        result->kind = BOLUN_ASSISTANT_SET_ALARM;
        snprintf(result->action, sizeof(result->action), "set_alarm");
        snprintf(result->detail, sizeof(result->detail), "%s", command + 10);
        return BOLUN_OK;
    }
    result->kind = BOLUN_ASSISTANT_AI_QUERY;
    result->requires_ai_license = 1;
    if (!license || !bolun_license_can_use_ai(license, "assistant.ai")) return BOLUN_ERROR_QUOTA;
    BolunStatus usage = bolun_license_record_ai_usage(license, "assistant.ai");
    if (usage != BOLUN_OK) return usage;
    snprintf(result->action, sizeof(result->action), "ai_query");
    snprintf(result->detail, sizeof(result->detail), "%s", command);
    return BOLUN_OK;
}

BolunStatus bolun_assistant_format_result(const BolunAssistantResult *result, char *out, size_t out_size) {
    if (!result || !out || out_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    snprintf(out, out_size, "assistant:%s:%s:licensed=%d", result->action, result->detail, result->requires_ai_license);
    return BOLUN_OK;
}

BolunStatus bolun_assistant_service_start(void) { return BOLUN_OK; }
BolunStatus bolun_assistant_service_handle(const char *request, char *response, size_t response_size) {
    BolunAssistantResult result;
    BolunStatus status = bolun_assistant_execute(bolun_service_license_state(), request, &result);
    if (status != BOLUN_OK) return status;
    return bolun_assistant_format_result(&result, response, response_size);
}
BolunStatus bolun_assistant_service_stop(void) { return BOLUN_OK; }
