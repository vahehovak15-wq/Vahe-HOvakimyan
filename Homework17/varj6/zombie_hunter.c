#include "zombie_hunter.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

void hunt_zombies(pid_t pgid) {
    killpg(pgid, SIGKILL);
    sleep(1);
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("[HUNTER] Zombie process with PID %d cleaned up.\n", pid);
    }
}