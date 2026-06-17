#ifndef BUFFER_H
#define BUFFER_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#define MAX_LINE_LENGTH 1024


typedef struct {
    char** lines;
    int capacity;
    int count;
    int front;
    int rear;
    int is_done;
} Buffer;

void buffer_init(Buffer* buffer, int capacity);
void buffer_destroy(Buffer* buffer);
void buffer_add(Buffer* buffer, char* line);
char* buffer_get(Buffer* buffer);

#endif