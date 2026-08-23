#include <stdio.h>
#include<string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include<signal.h>
#include<unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "ipc_buffer.h"


#define FILENAME "data.bin"

int main(void) {
    srand((unsigned int)time(NULL));

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        sleep(1);

        int count = 0;
        int* numbers = read_dynamic_numbers(FILENAME, &count);

        if (numbers) {
            printf("[Child Process] Read %d numbers from binary file:\n", count);
            for (int i = 0; i < count; i++) {
                printf("%d ", numbers[i]);
            }
            printf("\n");

            free(numbers);
        } else {
            printf("[Child Process] Failed to read numbers.\n");
        }

    } else {
        FILE* fp = fopen(FILENAME, "wb");
        if (!fp) {
            perror("Failed to open file for writing");
            exit(EXIT_FAILURE);
        }
        int num_count = rand() % 14 + 7;
        printf("[Parent Process] Writing %d numbers to '%s'...\n", num_count, FILENAME);

        for (int i = 0; i < num_count; i++) {
            int random_val = rand() % 100; 
            fwrite(&random_val, sizeof(int), 1, fp);
        }

        fclose(fp);

        wait(NULL);
    }

    return 0;
}