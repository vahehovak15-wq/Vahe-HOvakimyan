#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void register_safe_handler(int sig, void (*handler)(int)){
    sigset_t mask ,oldmask;
    
    sigfillset(&mask);
    sigdelset(&mask,sig);

    sigprocmask(SIG_SETMASK,&mask,&oldmask);

    handler(sig);

    
    sigprocmask(SIG_SETMASK,&oldmask,NULL);
}