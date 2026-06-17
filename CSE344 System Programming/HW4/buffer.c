
#include "buffer.h"

extern pthread_mutex_t buffer_mutex;
extern pthread_cond_t buffer_not_full;
extern pthread_cond_t buffer_not_empty;

void buffer_init(Buffer* buffer, int capacity) {
    buffer->lines = malloc(sizeof(char*) * capacity);
    buffer->capacity = capacity;
    buffer->count = 0;
    buffer->front = 0;
    buffer->rear = 0;
    buffer->is_done = 0;
}

void buffer_destroy(Buffer* buffer) {
    for (int i = 0; i < buffer->count; ++i) {
        free(buffer->lines[(buffer->front + i) % buffer->capacity]);
    }
    free(buffer->lines);
}

void buffer_add(Buffer* buffer, char* line) {
    pthread_mutex_lock(&buffer_mutex);
    while (buffer->count == buffer->capacity) {
        pthread_cond_wait(&buffer_not_full, &buffer_mutex);
    }
    buffer->lines[buffer->rear] = line;
    buffer->rear = (buffer->rear + 1) % buffer->capacity;
    buffer->count++;
    pthread_cond_signal(&buffer_not_empty);
    pthread_mutex_unlock(&buffer_mutex);
}

char* buffer_get(Buffer* buffer) {
    pthread_mutex_lock(&buffer_mutex);
    while (buffer->count == 0 && !buffer->is_done) {
        pthread_cond_wait(&buffer_not_empty, &buffer_mutex);
    }
    if (buffer->count == 0 && buffer->is_done) {
        pthread_mutex_unlock(&buffer_mutex);
        return NULL;
    }
    char* line = buffer->lines[buffer->front];
    buffer->front = (buffer->front + 1) % buffer->capacity;
    buffer->count--;
    pthread_cond_signal(&buffer_not_full);
    pthread_mutex_unlock(&buffer_mutex);
    return line;
}