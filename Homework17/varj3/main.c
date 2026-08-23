#include <stdio.h>
#include<string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include<signal.h>
#include<unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
void wait_pong();
void send_ping();
int main(){
     pid_t pid = fork();
     if (pid < 0){
        perror("no");
        exit(0);
     }
     if(pid == 0){
        wait_pong();
     }else {
         sleep(1);
        send_ping(pid);
        wait(NULL);
     }
     return 0;
}