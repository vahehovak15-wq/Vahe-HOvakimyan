#include <stdio.h>
#include<string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include<signal.h>
#include<unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("[Worker] Արգումենտ չի փոխանցվել:\n");
        return 1;
    }

    printf("[Worker] Ստացված արգումենտը: %s\n", argv[1]);
    return 0;
}