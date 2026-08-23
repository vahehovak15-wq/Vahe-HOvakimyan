#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "sentinel.h"

void handle_sigint(int sig) {
    (void)sig; 
    force_cleanup();
}

int main() {
    signal(SIGINT, handle_sigint);

    printf("=== Memory Leak Sentinel Test ===\n");
    printf("Process PID: %d\n", getpid());
    printf("Executing safe_malloc calls without calling safe_free...\n\n");

    for (int i = 1; i <= 5; i++) {
        int* data = (int*)safe_malloc(sizeof(int) * 100);
        if (data) {
            data[0] = i * 42; // Lvalue test
            printf("[%d] Allocated address: %p | Value: %d\n", i, (void*)data, data[0]);
        }
        usleep(200000); // 200ms դադար
    }

    printf("\nProgram is running in an infinite loop.\n");
    printf("Press Ctrl+C (SIGINT) to trigger forced memory cleanup...\n");

    while (1) {
        sleep(1);
    }

    return 0;
}