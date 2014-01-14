#ifndef __cplusplus
#define inline
#endif

#include <stdarg.h>
#include <stdlib.h>

static void munmap(void* a, size_t b) {}
static void* mmap(void* a, size_t b, int c, int d, int e, int f) { return NULL; }
static int mprotect(void* a, size_t b, int c) { return 0; }

#define PROT_READ 1
#define PROT_WRITE 2
#define PROT_EXEC 4

#define MAP_PRIVATE 4
#define MAP_ANON 8

#define MAP_FAILED 0
