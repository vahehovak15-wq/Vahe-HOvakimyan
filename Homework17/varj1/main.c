#include <stdio.h>
#include<string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include<signal.h>
#include<unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "sig_manager.h"


void handler (int sig){
    printf ("SIGINT caught\n");
} 
int main (){
    register_safe_handler(SIGINT,handler);

    while(1){
        pause();
    }
}