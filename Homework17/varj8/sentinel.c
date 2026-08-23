#include <stdio.h>
#include <stdlib.h>
#include "sentinel.h"

static void** allocated_ptrs = NULL;
static size_t ptr_count = 0;
static size_t ptr_capacity = 0;

void* safe_malloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        perror("safe_malloc: malloc error");
        return NULL;
    }

    if (ptr_count >= ptr_capacity) {
        size_t new_capacity = (ptr_capacity == 0) ? 8 : ptr_capacity * 2;
        void** new_array = realloc(allocated_ptrs, new_capacity * sizeof(void*));
        if (!new_array) {
            perror("safe_malloc: realloc error");
            free(ptr);
            return NULL;
        }
        allocated_ptrs = new_array;
        ptr_capacity = new_capacity;
    }

    allocated_ptrs[ptr_count++] = ptr;
    return ptr;
}

void safe_free(void* ptr) {
    if (!ptr) return;

    for (size_t i = 0; i < ptr_count; i++) {
        if (allocated_ptrs[i] == ptr) {
            free(ptr);
            allocated_ptrs[i] = allocated_ptrs[ptr_count - 1];
            ptr_count--;
            return;
        }
    }
    free(ptr);
}

void force_cleanup(void) {
    printf("\n\n[Memory Leak Sentinel] SIGINT (Ctrl+C) received: Starting memory cleanup...\n");
    
    size_t leaked_count = ptr_count;

    for (size_t i = 0; i < ptr_count; i++) {
        free(allocated_ptrs[i]);
    }

    free(allocated_ptrs);
    allocated_ptrs = NULL;
    ptr_count = 0;
    ptr_capacity = 0;

    printf("[Memory Leak Sentinel] Successfully freed %zu unreleased memory block(s).\n", leaked_count);
    printf("[Memory Leak Sentinel] Program terminated safely.\n");
    
    exit(0);
}