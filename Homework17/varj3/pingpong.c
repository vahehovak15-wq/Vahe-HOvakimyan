#include <stdio.h>
#include<string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include<signal.h>
#include<unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "pingpong.h"

void handler(int sig){
    printf("Pong received:\n");
}

void wait_pong(){
    signal(SIGUSR1,handler);
    pause();
}
void send_ping(pid_t target){
    kill(target, SIGUSR1);
} 