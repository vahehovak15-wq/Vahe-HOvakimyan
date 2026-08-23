#include <stdio.h>
#include<string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include<signal.h>
#include<unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include"runner.h"
void run_worker_bg(const char *arg){

    pid_t pid = fork();

    if(pid == 0){
        execlp("./worke","./worke","Barev",NULL);

        exit(0);
    }
}
