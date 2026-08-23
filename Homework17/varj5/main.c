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

int main(void) {
    int size = 1000;  
    int chunks = 4;  
    int* arr = (int*)malloc(size * sizeof(int));
    if (!arr) {
        perror("malloc failed");
        return 1;
    }
    long expected_sum = 0;
    for (int i = 0; i < size; i++) {
        arr[i] = 1; 
        expected_sum += arr[i];
    }

    printf("Calculated expected sum: %ld\n", expected_sum);

    long result = parallel_sum(arr, size, chunks);

    printf("Parallel sum result:     %ld\n", result);

    free(arr);

    return 0;
}