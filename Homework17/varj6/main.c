#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "zombie_hunter.h"

#define NUM_CHILDREN 5

int main() {
    pid_t pids[NUM_CHILDREN];
    pid_t new_pgid = 0;
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pids[i] = fork();
        

        if (pids[i] < 0) {
            perror("fork failed");
            exit(1);
        }

        if (pids[i] == 0) {
            while (1) {
                sleep(1);
            }
            exit(0);
        }else {
            // --- ԾՆՈՂ ՊՐՈՑԵՍ ---
            // Առաջին երեխային դարձնում ենք նոր խմբի ղեկավար (Leader), 
            // իսկ մյուս երեխաներին ավելացնում ենք այդ նույն նոր խմբի մեջ:
            if (i == 0) {
                new_pgid = pids[0]; // Առաջին երեխայի PID-ը դառնում է նոր PGID
            }
            // pids[i] պրոցեսին տեղափոխում ենք new_pgid խումբ
            setpgid(pids[i], new_pgid);
        }
    }
    pid_t group_id = getpgrp();

    printf("[PARENT] Created %d children in new group = %d.\n", NUM_CHILDREN, new_pgid);
    
    sleep(1);

    hunt_zombies(group_id);

    printf("[PARENT] All zombies cleared successfully.\n");
    return 0;
}