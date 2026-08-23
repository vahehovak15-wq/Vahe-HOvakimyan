#ifndef PING_PONG_H
#define PING_PONG_H

#include <sys/types.h>

void send_ping(pid_t target);
void wait_pong(void);

#endif