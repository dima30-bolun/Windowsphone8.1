#ifndef BOLUN_KERNEL_H
#define BOLUN_KERNEL_H

#include <stddef.h>
#include <stdint.h>
#include "bolun/status.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BOLUN_MAX_PROCESSES 64
#define BOLUN_MAX_THREADS 128
#define BOLUN_MAX_HANDLES 256
#define BOLUN_MAX_OBJECTS 128
#define BOLUN_IPC_MESSAGE_BYTES 96

typedef enum BolunThreadState {
    BOLUN_THREAD_READY,
    BOLUN_THREAD_RUNNING,
    BOLUN_THREAD_SLEEPING,
    BOLUN_THREAD_STOPPED
} BolunThreadState;

typedef struct BolunKernelConfig {
    size_t physical_pages;
    size_t page_size;
    size_t heap_bytes;
} BolunKernelConfig;

typedef struct BolunKernelState BolunKernelState;

typedef struct BolunThreadSnapshot {
    int thread_id;
    int process_id;
    int priority;
    uint64_t fifo_sequence;
    BolunThreadState state;
} BolunThreadSnapshot;

typedef struct BolunIpcMessage {
    int from_pid;
    int to_pid;
    char payload[BOLUN_IPC_MESSAGE_BYTES];
} BolunIpcMessage;

BolunStatus bolun_kernel_init(BolunKernelState *state, BolunKernelConfig config);
BolunStatus bolun_kernel_create_process(BolunKernelState *state, const char *name, int *pid_out);
BolunStatus bolun_kernel_create_thread(BolunKernelState *state, int pid, int priority, int *tid_out);
BolunStatus bolun_kernel_schedule_next(BolunKernelState *state, BolunThreadSnapshot *out);
BolunStatus bolun_kernel_allocate_pages(BolunKernelState *state, size_t pages, uintptr_t *virtual_address_out);
BolunStatus bolun_kernel_heap_alloc(BolunKernelState *state, size_t bytes, uintptr_t *address_out);
BolunStatus bolun_kernel_open_handle(BolunKernelState *state, int pid, const char *object_name, uint32_t rights, int *handle_out);
BolunStatus bolun_kernel_check_right(BolunKernelState *state, int pid, int handle, uint32_t right);
BolunStatus bolun_kernel_ipc_send(BolunKernelState *state, int from_pid, int to_pid, const char *payload);
BolunStatus bolun_kernel_ipc_receive(BolunKernelState *state, int to_pid, BolunIpcMessage *out);
BolunStatus bolun_kernel_syscall(BolunKernelState *state, int pid, int syscall_id, uintptr_t arg0, uintptr_t *result_out);

struct BolunKernelState {
    BolunKernelConfig config;
    size_t used_pages;
    size_t heap_offset;
    int next_pid;
    int next_tid;
    int next_handle;
    uint64_t fifo_counter;
    struct { int id; char name[32]; int alive; } processes[BOLUN_MAX_PROCESSES];
    struct { int id; int pid; int priority; uint64_t fifo_sequence; BolunThreadState state; } threads[BOLUN_MAX_THREADS];
    struct { int id; int pid; char object_name[48]; uint32_t rights; int active; } handles[BOLUN_MAX_HANDLES];
    struct { char name[48]; uint32_t required_right; int active; } objects[BOLUN_MAX_OBJECTS];
    BolunIpcMessage ipc[64];
    size_t ipc_head;
    size_t ipc_tail;
    size_t ipc_count;
};

#ifdef __cplusplus
}
#endif

#endif
