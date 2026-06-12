#include <stdio.h>
#include <string.h>
#include "bolun/kernel.h"

static int process_exists(BolunKernelState *state, int pid) {
    for (size_t i = 0; i < BOLUN_MAX_PROCESSES; ++i) if (state->processes[i].alive && state->processes[i].id == pid) return 1;
    return 0;
}

BolunStatus bolun_kernel_init(BolunKernelState *state, BolunKernelConfig config) {
    if (!state || config.page_size == 0 || config.physical_pages == 0 || config.heap_bytes == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    memset(state, 0, sizeof(*state));
    state->config = config;
    state->next_pid = 1;
    state->next_tid = 1;
    state->next_handle = 100;
    state->objects[0].active = 1;
    strcpy(state->objects[0].name, "system.display");
    state->objects[0].required_right = 0x1u;
    state->objects[1].active = 1;
    strcpy(state->objects[1].name, "system.ai");
    state->objects[1].required_right = 0x2u;
    return BOLUN_OK;
}

BolunStatus bolun_kernel_allocate_pages(BolunKernelState *state, size_t pages, uintptr_t *virtual_address_out) {
    if (!state || !virtual_address_out || pages == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    if (state->used_pages + pages > state->config.physical_pages) return BOLUN_ERROR_NO_CAPACITY;
    *virtual_address_out = 0x80000000u + (uintptr_t)(state->used_pages * state->config.page_size);
    state->used_pages += pages;
    return BOLUN_OK;
}

BolunStatus bolun_kernel_heap_alloc(BolunKernelState *state, size_t bytes, uintptr_t *address_out) {
    if (!state || !address_out || bytes == 0) return BOLUN_ERROR_INVALID_ARGUMENT;
    size_t aligned = (bytes + 15u) & ~15u;
    if (state->heap_offset + aligned > state->config.heap_bytes) return BOLUN_ERROR_NO_CAPACITY;
    *address_out = 0x90000000u + (uintptr_t)state->heap_offset;
    state->heap_offset += aligned;
    return BOLUN_OK;
}

BolunStatus bolun_kernel_create_process(BolunKernelState *state, const char *name, int *pid_out) {
    if (!state || !name || !pid_out || name[0] == '\0') return BOLUN_ERROR_INVALID_ARGUMENT;
    for (size_t i = 0; i < BOLUN_MAX_PROCESSES; ++i) {
        if (!state->processes[i].alive) {
            state->processes[i].alive = 1;
            state->processes[i].id = state->next_pid++;
            snprintf(state->processes[i].name, sizeof(state->processes[i].name), "%s", name);
            *pid_out = state->processes[i].id;
            return BOLUN_OK;
        }
    }
    return BOLUN_ERROR_NO_CAPACITY;
}

BolunStatus bolun_kernel_create_thread(BolunKernelState *state, int pid, int priority, int *tid_out) {
    if (!state || !tid_out || priority < 0 || priority > 31 || !process_exists(state, pid)) return BOLUN_ERROR_INVALID_ARGUMENT;
    for (size_t i = 0; i < BOLUN_MAX_THREADS; ++i) {
        if (state->threads[i].id == 0) {
            state->threads[i].id = state->next_tid++;
            state->threads[i].pid = pid;
            state->threads[i].priority = priority;
            state->threads[i].fifo_sequence = state->fifo_counter++;
            state->threads[i].state = BOLUN_THREAD_READY;
            *tid_out = state->threads[i].id;
            return BOLUN_OK;
        }
    }
    return BOLUN_ERROR_NO_CAPACITY;
}

BolunStatus bolun_kernel_schedule_next(BolunKernelState *state, BolunThreadSnapshot *out) {
    if (!state || !out) return BOLUN_ERROR_INVALID_ARGUMENT;
    int best = -1;
    for (size_t i = 0; i < BOLUN_MAX_THREADS; ++i) {
        if (state->threads[i].id != 0 && state->threads[i].state == BOLUN_THREAD_READY) {
            if (best < 0 || state->threads[i].priority > state->threads[best].priority ||
                (state->threads[i].priority == state->threads[best].priority && state->threads[i].fifo_sequence < state->threads[best].fifo_sequence)) best = (int)i;
        }
    }
    if (best < 0) return BOLUN_ERROR_NOT_FOUND;
    state->threads[best].state = BOLUN_THREAD_RUNNING;
    out->thread_id = state->threads[best].id;
    out->process_id = state->threads[best].pid;
    out->priority = state->threads[best].priority;
    out->fifo_sequence = state->threads[best].fifo_sequence;
    out->state = state->threads[best].state;
    state->threads[best].state = BOLUN_THREAD_READY;
    state->threads[best].fifo_sequence = state->fifo_counter++;
    return BOLUN_OK;
}

BolunStatus bolun_kernel_open_handle(BolunKernelState *state, int pid, const char *object_name, uint32_t rights, int *handle_out) {
    if (!state || !object_name || !handle_out || !process_exists(state, pid)) return BOLUN_ERROR_INVALID_ARGUMENT;
    int object_found = 0;
    for (size_t i = 0; i < BOLUN_MAX_OBJECTS; ++i) if (state->objects[i].active && strcmp(state->objects[i].name, object_name) == 0) object_found = 1;
    if (!object_found) return BOLUN_ERROR_NOT_FOUND;
    for (size_t i = 0; i < BOLUN_MAX_HANDLES; ++i) if (!state->handles[i].active) {
        state->handles[i].active = 1;
        state->handles[i].id = state->next_handle++;
        state->handles[i].pid = pid;
        state->handles[i].rights = rights;
        snprintf(state->handles[i].object_name, sizeof(state->handles[i].object_name), "%s", object_name);
        *handle_out = state->handles[i].id;
        return BOLUN_OK;
    }
    return BOLUN_ERROR_NO_CAPACITY;
}

BolunStatus bolun_kernel_check_right(BolunKernelState *state, int pid, int handle, uint32_t right) {
    if (!state || !process_exists(state, pid)) return BOLUN_ERROR_INVALID_ARGUMENT;
    for (size_t i = 0; i < BOLUN_MAX_HANDLES; ++i) {
        if (state->handles[i].active && state->handles[i].pid == pid && state->handles[i].id == handle) return (state->handles[i].rights & right) ? BOLUN_OK : BOLUN_ERROR_DENIED;
    }
    return BOLUN_ERROR_NOT_FOUND;
}

BolunStatus bolun_kernel_ipc_send(BolunKernelState *state, int from_pid, int to_pid, const char *payload) {
    if (!state || !payload || !process_exists(state, from_pid) || !process_exists(state, to_pid)) return BOLUN_ERROR_INVALID_ARGUMENT;
    if (state->ipc_count == 64) return BOLUN_ERROR_NO_CAPACITY;
    BolunIpcMessage *m = &state->ipc[state->ipc_tail];
    m->from_pid = from_pid;
    m->to_pid = to_pid;
    snprintf(m->payload, sizeof(m->payload), "%s", payload);
    state->ipc_tail = (state->ipc_tail + 1u) % 64u;
    state->ipc_count++;
    return BOLUN_OK;
}

BolunStatus bolun_kernel_ipc_receive(BolunKernelState *state, int to_pid, BolunIpcMessage *out) {
    if (!state || !out || !process_exists(state, to_pid)) return BOLUN_ERROR_INVALID_ARGUMENT;
    for (size_t n = 0; n < state->ipc_count; ++n) {
        size_t idx = (state->ipc_head + n) % 64u;
        if (state->ipc[idx].to_pid == to_pid) {
            *out = state->ipc[idx];
            for (size_t j = n; j + 1u < state->ipc_count; ++j) state->ipc[(state->ipc_head + j) % 64u] = state->ipc[(state->ipc_head + j + 1u) % 64u];
            state->ipc_tail = (state->ipc_tail + 63u) % 64u;
            state->ipc_count--;
            return BOLUN_OK;
        }
    }
    return BOLUN_ERROR_NOT_FOUND;
}

BolunStatus bolun_kernel_syscall(BolunKernelState *state, int pid, int syscall_id, uintptr_t arg0, uintptr_t *result_out) {
    if (!state || !result_out || !process_exists(state, pid)) return BOLUN_ERROR_INVALID_ARGUMENT;
    if (syscall_id == 1) return bolun_kernel_heap_alloc(state, (size_t)arg0, result_out);
    if (syscall_id == 2) { *result_out = (uintptr_t)pid; return BOLUN_OK; }
    return BOLUN_ERROR_NOT_FOUND;
}
