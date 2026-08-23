#ifndef SIG_MANAGER_H
#define SIG_MANAGER_H

#include <stdio.h>
void register_safe_handler(int sig, void (*handler)(int));

#endif