
#include "buffer.h"

#define MAX_WORKERS 100

Buffer buffer;
pthread_barrier_t barrier;
int num_workers;
char* search_term;
FILE* input_file;

volatile sig_atomic_t stop = 0;
int global_count = 0;
int summary = 0;

pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_not_empty = PTHREAD_COND_INITIALIZER;



void handle_sigint(int sig) {
    (void)sig; // Suppress unused variable warning
    stop = 1;
    pthread_mutex_lock(&buffer_mutex);
    buffer.is_done = 1;
    
    pthread_cond_broadcast(&buffer_not_empty);  // Notify waiting threads
    pthread_mutex_unlock(&buffer_mutex);
}


void* worker(void* arg) {
    int id = *(int*)arg;
    int local_count = 0;
    while (!stop) {
        char* line = buffer_get(&buffer);
        if (!line) break;
        if (strstr(line, search_term)) {
            local_count++;
            global_count++;
        }
        free(line);

         // Check for stop signal periodically
        if (stop) {
            break;
        }
    }
    

    
    printf("Worker %d found %d matches of '%s'\n", id+1 , local_count,search_term);
    pthread_barrier_wait(&barrier);
    
    pthread_mutex_lock(&buffer_mutex);
    if(summary==0){
        if(stop){
            printf("CTRL+C INTERRUPT!!! Partial results are printed...\n");
        }
        printf("\n[Summary]\n");
        printf("Total %d matches found of '%s'\n",global_count,search_term);
        summary = 1;

    }
    pthread_mutex_unlock(&buffer_mutex);


    return NULL;
}

void* manager(void* arg) {
    (void)arg;
    char line[MAX_LINE_LENGTH];
    while (!stop && fgets(line, sizeof(line), input_file)) {
        char* copy = strdup(line);
        if (!copy) continue;
        buffer_add(&buffer, copy);

         // Check for stop signal periodically
        if (stop) {
            break;
        }
    }

    pthread_mutex_lock(&buffer_mutex);
    buffer.is_done = 1;
    pthread_cond_broadcast(&buffer_not_empty);
    pthread_mutex_unlock(&buffer_mutex);

    return NULL;
}

void print_log_analyzer_usage(const char* prog) {
    fprintf(stderr, "print_log_analyzer_usage: %s <buffer_size> <num_workers> <log_file> <search_term>\n", prog);
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        print_log_analyzer_usage(argv[0]);
        return 1;
    }

    int buffer_size = atoi(argv[1]);
    num_workers = atoi(argv[2]);
    char* file_path = argv[3];
    search_term = argv[4];

    if (buffer_size <= 0 || num_workers <= 0 || num_workers > MAX_WORKERS) {
        printf("Either buffer size is <=0 or num_workers <=0 or num_workers > MAX_WORKERS size which is 100.\n");
        print_log_analyzer_usage(argv[0]);
        return 1;
    }

    signal(SIGINT, handle_sigint);
    input_file = fopen(file_path, "r");
    if (!input_file) {
        perror("fopen");
        return 1;
    }

    buffer_init(&buffer, buffer_size);
    pthread_t threads[num_workers];
    pthread_t manager_thread;
    pthread_barrier_init(&barrier, NULL, num_workers);


    int ids[num_workers];
    for (int i = 0; i < num_workers; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    pthread_create(&manager_thread, NULL, manager, NULL);
    pthread_join(manager_thread, NULL);

    for (int i = 0; i < num_workers; ++i) {
        pthread_join(threads[i], NULL);
    }

    buffer_destroy(&buffer);
    fclose(input_file);
    pthread_barrier_destroy(&barrier);

    return 0;
}