#include <stdio.h>
#include <string.h>
#include "bolun/services.h"

#define BOLUN_STORAGE_SLOTS 64
#define BOLUN_STORAGE_KEY_BYTES 32
#define BOLUN_STORAGE_VALUE_BYTES 96

typedef struct StorageSlot {
    int used;
    char key[BOLUN_STORAGE_KEY_BYTES];
    char value[BOLUN_STORAGE_VALUE_BYTES];
} StorageSlot;

static int storage_started;
static StorageSlot slots[BOLUN_STORAGE_SLOTS];

static StorageSlot *find_slot(const char *key) {
    for (size_t i = 0; i < BOLUN_STORAGE_SLOTS; ++i) {
        if (slots[i].used && strcmp(slots[i].key, key) == 0) return &slots[i];
    }
    return 0;
}

static StorageSlot *alloc_slot(void) {
    for (size_t i = 0; i < BOLUN_STORAGE_SLOTS; ++i) if (!slots[i].used) return &slots[i];
    return 0;
}

BolunStatus bolun_storage_service_start(void) {
    storage_started = 1;
    return BOLUN_OK;
}

BolunStatus bolun_storage_service_handle(const char *request, char *response, size_t response_size) {
    if (!storage_started || !request || !response || response_size == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    if (strncmp(request, "put ", 4) == 0) {
        const char *key = request + 4;
        const char *space = strchr(key, ' ');
        if (!space || space == key) return BOLUN_ERROR_INVALID_ARGUMENT;
        size_t key_len = (size_t)(space - key);
        if (key_len >= BOLUN_STORAGE_KEY_BYTES) return BOLUN_ERROR_NO_CAPACITY;
        char key_buffer[BOLUN_STORAGE_KEY_BYTES];
        memcpy(key_buffer, key, key_len);
        key_buffer[key_len] = '\0';
        StorageSlot *slot = find_slot(key_buffer);
        if (!slot) slot = alloc_slot();
        if (!slot) return BOLUN_ERROR_NO_CAPACITY;
        slot->used = 1;
        snprintf(slot->key, sizeof(slot->key), "%s", key_buffer);
        snprintf(slot->value, sizeof(slot->value), "%s", space + 1);
        snprintf(response, response_size, "storage:put:%s", slot->key);
        return BOLUN_OK;
    }
    if (strncmp(request, "get ", 4) == 0) {
        StorageSlot *slot = find_slot(request + 4);
        if (!slot) return BOLUN_ERROR_NOT_FOUND;
        snprintf(response, response_size, "storage:get:%s=%s", slot->key, slot->value);
        return BOLUN_OK;
    }
    if (strncmp(request, "delete ", 7) == 0) {
        StorageSlot *slot = find_slot(request + 7);
        if (!slot) return BOLUN_ERROR_NOT_FOUND;
        memset(slot, 0, sizeof(*slot));
        snprintf(response, response_size, "storage:delete:ok");
        return BOLUN_OK;
    }
    snprintf(response, response_size, "storage:handled:%s", request);
    return BOLUN_OK;
}

BolunStatus bolun_storage_service_stop(void) {
    storage_started = 0;
    return BOLUN_OK;
}
