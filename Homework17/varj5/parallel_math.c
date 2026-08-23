#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "parallel_math.h"


long parallel_sum(int* arr, int size, int chunks) {
    pid_t *pids = (pid_t*)malloc(chunks * sizeof(pid_t));
    if (!pids) {
        perror("malloc failed");
        return 0;
    }

    int chunk_size = size / chunks; 
    int remainder = size % chunks; 
    for (int i = 0; i < chunks; i++) {
        int start_idx = i * chunk_size;
        int current_chunk_size = (i == chunks - 1) ? (chunk_size + remainder) : chunk_size;

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            free(pids);
            return 0;
        }

        if (pid == 0) {
            long local_sum = 0;
            for (int j = 0; j < current_chunk_size; j++) {
                local_sum += arr[start_idx + j];
            }
            char filename[64];
            snprintf(filename, sizeof(filename), "tmp_chunk_%d.txt", i);

            FILE* fp = fopen(filename, "w");
            if (!fp) {
                perror("fopen failed in child");
                exit(EXIT_FAILURE);
            }
            fprintf(fp, "%ld\n", local_sum);
            fclose(fp);
            free(pids);
            exit(EXIT_SUCCESS);
        } else {
            pids[i] = pid; 
        }
    }
    for (int i = 0; i < chunks; i++) {
        waitpid(pids[i], NULL, 0);
    }

    long total_sum = 0;

    for (int i = 0; i < chunks; i++) {
        char filename[64];
        snprintf(filename, sizeof(filename), "tmp_chunk_%d.txt", i);

        FILE* fp = fopen(filename, "r");
        if (fp) {
            long child_sum = 0;
            if (fscanf(fp, "%ld", &child_sum) == 1) {
                total_sum += child_sum; 
            }
            fclose(fp);

            
            unlink(filename);
        }
    }

    free(pids);
    return total_sum;
}