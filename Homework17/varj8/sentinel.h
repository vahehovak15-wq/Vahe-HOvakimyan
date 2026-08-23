#ifndef SENTINEL_H
#define SENTINEL_H

#include <stddef.h>

void* safe_malloc(size_t size);
void safe_free(void* ptr);

void force_cleanup(void);

#endif