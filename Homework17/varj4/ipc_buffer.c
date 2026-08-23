#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "ipc_buffer.h"

int* read_dynamic_numbers(const char* file, int* out_count){
    FILE *fp = fopen(file,"rb");
    int capacity = 5;
    int count = 0;

    int *arr =(int*)malloc(capacity * sizeof(int));

    int num;

    while (fread(&num ,sizeof(int),1,fp) == 1)
    {
        if (count == capacity){
            capacity *=2;
            int * temp =(int *)realloc(arr,capacity * sizeof(int));
            if(!temp){
                perror("Realloc failed");
                free(arr);
                fclose(fp);
                return(NULL);
            }
            arr = temp;
        }
        arr[count++]=num;
    }
    fclose(fp);
    return arr;

}