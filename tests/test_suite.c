#include <stdio.h>
#include <string.h>
#include "bolun/apps.h"
#include "bolun/assistant.h"
#include "bolun/driver.h"
#include "bolun/graphics.h"
#include "bolun/hal.h"
#include "bolun/kernel.h"
#include "bolun/license.h"
#include "bolun/services.h"
#include "bolun/shell.h"

static int failures;
#define CHECK(expr) do { if (!(expr)) { printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #expr); failures++; } } while (0)

static void test_hal(void) {
    CHECK(bolun_hal_profile_count() == 23);
    const BolunBoardProfile *p = bolun_hal_find_profile("Lumia 950 XL");
    CHECK(p != 0);
    CHECK(p && p->soc == BOLUN_SOC_SNAPDRAGON_810);
    CHECK(p && bolun_hal_validate_profile(p) == BOLUN_OK);
    CHECK(bolun_hal_find_profile("Lumia 1020") != 0);
    BolunHalControllerState ctl;
    CHECK(bolun_hal_controller_init(&ctl, p) == BOLUN_OK);
    CHECK(bolun_hal_set_cpu_frequency(&ctl, 1400) == BOLUN_OK);
    CHECK(ctl.cpu_frequency_mhz == 1400);
    CHECK(bolun_hal_gpio_write(&ctl, 7, 1) == BOLUN_OK);
    uint8_t pin = 0;
    CHECK(bolun_hal_gpio_read(&ctl, 7, &pin) == BOLUN_OK);
    CHECK(pin == 1);
    uint8_t tx[2] = {0x10, 0x20};
    uint8_t rx[2] = {0, 0};
    CHECK(bolun_hal_spi_transfer(&ctl, tx, rx, 2) == BOLUN_OK);
    CHECK(rx[0] == (uint8_t)0xef);
}

static void test_kernel(void) {
    BolunKernelState k;
    CHECK(bolun_kernel_init(&k, (BolunKernelConfig){128, 4096, 65536}) == BOLUN_OK);
    uintptr_t va = 0;
    CHECK(bolun_kernel_allocate_pages(&k, 2, &va) == BOLUN_OK);
    CHECK(va == 0x80000000u);
    uintptr_t heap = 0;
    CHECK(bolun_kernel_heap_alloc(&k, 24, &heap) == BOLUN_OK);
    int p1 = 0, p2 = 0, t1 = 0, t2 = 0;
    CHECK(bolun_kernel_create_process(&k, "shell", &p1) == BOLUN_OK);
    CHECK(bolun_kernel_create_process(&k, "service", &p2) == BOLUN_OK);
    CHECK(bolun_kernel_create_thread(&k, p1, 4, &t1) == BOLUN_OK);
    CHECK(bolun_kernel_create_thread(&k, p2, 9, &t2) == BOLUN_OK);
    BolunThreadSnapshot snap;
    CHECK(bolun_kernel_schedule_next(&k, &snap) == BOLUN_OK);
    CHECK(snap.thread_id == t2);
    int handle = 0;
    CHECK(bolun_kernel_open_handle(&k, p1, "system.display", 0x1u, &handle) == BOLUN_OK);
    CHECK(bolun_kernel_check_right(&k, p1, handle, 0x1u) == BOLUN_OK);
    CHECK(bolun_kernel_check_right(&k, p1, handle, 0x2u) == BOLUN_ERROR_DENIED);
    CHECK(bolun_kernel_ipc_send(&k, p1, p2, "hello") == BOLUN_OK);
    BolunIpcMessage msg;
    CHECK(bolun_kernel_ipc_receive(&k, p2, &msg) == BOLUN_OK);
    CHECK(strcmp(msg.payload, "hello") == 0);
}

static void test_drivers_services(void) {
    const BolunBoardProfile *p = bolun_hal_find_profile("Lumia 950");
    CHECK(bolun_driver_count() == 18);
    CHECK(bolun_driver_start_all(p) == BOLUN_OK);
    CHECK(bolun_service_start_all() == BOLUN_OK);
    char response[128];
    CHECK(bolun_service_request("notification", "ping", response, sizeof(response)) == BOLUN_OK);
    CHECK(strstr(response, "notification:handled:ping") != 0);
    CHECK(bolun_service_request("storage", "put notes hello", response, sizeof(response)) == BOLUN_OK);
    CHECK(bolun_service_request("storage", "get notes", response, sizeof(response)) == BOLUN_OK);
    CHECK(strstr(response, "notes=hello") != 0);
    CHECK(bolun_service_request("assistant", "open calculator", response, sizeof(response)) == BOLUN_OK);
    CHECK(strstr(response, "assistant:open_app:calculator") != 0);
}

static void test_graphics_shell_apps(void) {
    uint32_t pixels[240 * 160];
    BolunFramebuffer fb;
    CHECK(bolun_framebuffer_init(&fb, 240, 160, pixels, 240 * 160) == BOLUN_OK);
    CHECK(bolun_shell_render_start(&fb) == BOLUN_OK);
    CHECK(pixels[0] != 0);
    CHECK(bolun_app_count() == 19);
    char out[128];
    CHECK(bolun_shell_launch_app(&fb, "calculator", out, sizeof(out)) == BOLUN_OK);
    CHECK(strstr(out, "Calculator") != 0);
    CHECK(bolun_shell_render_notifications(&fb) == BOLUN_OK);
    CHECK(bolun_shell_render_settings(&fb) == BOLUN_OK);
}

static void test_license(void) {
    BolunLicenseState state;
    CHECK(bolun_license_init(&state, 10, 10) == BOLUN_OK);
    CHECK(bolun_license_can_use_ai(&state, "assistant") == 1);
    BolunAssistantResult result;
    CHECK(bolun_assistant_execute(&state, "call Alice", &result) == BOLUN_OK);
    CHECK(result.requires_ai_license == 0);
    CHECK(bolun_license_set_day(&state, 41) == BOLUN_OK);
    CHECK(bolun_assistant_execute(&state, "summarize document", &result) == BOLUN_OK);
    CHECK(result.requires_ai_license == 1);
    CHECK(bolun_license_record_ai_usage(&state, "assistant") == BOLUN_OK);
    CHECK(bolun_license_record_ai_usage(&state, "assistant") == BOLUN_OK);
    CHECK(bolun_license_record_ai_usage(&state, "assistant") == BOLUN_ERROR_QUOTA);
    char key[32];
    CHECK(bolun_license_generate_key(1234, key) == BOLUN_OK);
    CHECK(bolun_license_validate_key(key) == BOLUN_OK);
    CHECK(bolun_license_activate(&state, key) == BOLUN_OK);
    CHECK(bolun_license_record_ai_usage(&state, "assistant") == BOLUN_OK);
}

int main(void) {
    test_hal();
    test_kernel();
    test_drivers_services();
    test_graphics_shell_apps();
    test_license();
    if (failures) {
        printf("Bolun Phone OS tests failed: %d\n", failures);
        return 1;
    }
    printf("Bolun Phone OS vertical slice tests passed\n");
    return 0;
}
