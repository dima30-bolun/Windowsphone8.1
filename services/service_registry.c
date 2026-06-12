#include <string.h>
#include "bolun/services.h"

BolunStatus bolun_notification_service_start(void); BolunStatus bolun_notification_service_handle(const char*, char*, size_t); BolunStatus bolun_notification_service_stop(void);
BolunStatus bolun_package_service_start(void); BolunStatus bolun_package_service_handle(const char*, char*, size_t); BolunStatus bolun_package_service_stop(void);
BolunStatus bolun_update_service_start(void); BolunStatus bolun_update_service_handle(const char*, char*, size_t); BolunStatus bolun_update_service_stop(void);
BolunStatus bolun_storage_service_start(void); BolunStatus bolun_storage_service_handle(const char*, char*, size_t); BolunStatus bolun_storage_service_stop(void);
BolunStatus bolun_account_service_start(void); BolunStatus bolun_account_service_handle(const char*, char*, size_t); BolunStatus bolun_account_service_stop(void);
BolunStatus bolun_power_service_start(void); BolunStatus bolun_power_service_handle(const char*, char*, size_t); BolunStatus bolun_power_service_stop(void);
BolunStatus bolun_backup_service_start(void); BolunStatus bolun_backup_service_handle(const char*, char*, size_t); BolunStatus bolun_backup_service_stop(void);
BolunStatus bolun_cloud_sync_service_start(void); BolunStatus bolun_cloud_sync_service_handle(const char*, char*, size_t); BolunStatus bolun_cloud_sync_service_stop(void);
BolunStatus bolun_licensing_service_start(void); BolunStatus bolun_licensing_service_handle(const char*, char*, size_t); BolunStatus bolun_licensing_service_stop(void);

static BolunService services[] = {
    {"notification", bolun_notification_service_start, bolun_notification_service_handle, bolun_notification_service_stop},
    {"package", bolun_package_service_start, bolun_package_service_handle, bolun_package_service_stop},
    {"update", bolun_update_service_start, bolun_update_service_handle, bolun_update_service_stop},
    {"storage", bolun_storage_service_start, bolun_storage_service_handle, bolun_storage_service_stop},
    {"account", bolun_account_service_start, bolun_account_service_handle, bolun_account_service_stop},
    {"power", bolun_power_service_start, bolun_power_service_handle, bolun_power_service_stop},
    {"backup", bolun_backup_service_start, bolun_backup_service_handle, bolun_backup_service_stop},
    {"cloud_sync", bolun_cloud_sync_service_start, bolun_cloud_sync_service_handle, bolun_cloud_sync_service_stop},
    {"licensing", bolun_licensing_service_start, bolun_licensing_service_handle, bolun_licensing_service_stop}
};

size_t bolun_service_count(void) { return sizeof(services) / sizeof(services[0]); }
const BolunService *bolun_service_at(size_t index) { return index < bolun_service_count() ? &services[index] : 0; }
BolunStatus bolun_service_start_all(void) { for (size_t i = 0; i < bolun_service_count(); ++i) { BolunStatus s = services[i].start(); if (s != BOLUN_OK) return s; } return BOLUN_OK; }
BolunStatus bolun_service_request(const char *service_name, const char *request, char *response, size_t response_size) {
    if (!service_name) return BOLUN_ERROR_INVALID_ARGUMENT;
    for (size_t i = 0; i < bolun_service_count(); ++i) if (strcmp(services[i].name, service_name) == 0) return services[i].handle_request(request, response, response_size);
    return BOLUN_ERROR_NOT_FOUND;
}
