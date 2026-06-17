#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#define MAX_QUEUE_SIZE 100
#define NUM_ENGINEERS 3
#define MAX_NUM_SATELLITES 10
#define MAX_WAIT_TIME 5

// Struct to represent a satellite request
typedef struct {
    int priority;
    int satelliteId;
    time_t requestTime;
    sem_t requestHandled;
} SatelliteRequest;

// Priority queue (max-heap) for SatelliteRequest
static SatelliteRequest requestQueue[MAX_NUM_SATELLITES ]; 
static int queueSize = 0;

static pthread_mutex_t engineerMutex = PTHREAD_MUTEX_INITIALIZER;
static sem_t newRequest;
//static sem_t requestHandled;
static int availableEngineers = NUM_ENGINEERS;
static int shutdownFlag = 0; // Flag to signal engineers to shut down


// Swap utility
static void swapRequests(SatelliteRequest *a, SatelliteRequest *b) {
    SatelliteRequest tmp = *a;
    *a = *b;
    *b = tmp;
}

// Push into min-heap
void pushRequest(SatelliteRequest req) {
    if (queueSize >= MAX_QUEUE_SIZE) return;
    int i = queueSize;
    requestQueue[i] = req;
    queueSize++;
    // Sift up
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (requestQueue[parent].priority > requestQueue[i].priority) {
            swapRequests(&requestQueue[parent], &requestQueue[i]);
            i = parent;
        } else break;
    }
}
// Pop from min-heap (lowest priority first)
SatelliteRequest popRequest() {
    SatelliteRequest top = requestQueue[0];
    queueSize--;
    requestQueue[0] = requestQueue[queueSize];
    int i = 0;

    // Sift down for min-heap
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < queueSize && requestQueue[left].priority < requestQueue[smallest].priority)
            smallest = left;
        if (right < queueSize && requestQueue[right].priority < requestQueue[smallest].priority)
            smallest = right;

        if (smallest != i) {
            swapRequests(&requestQueue[i], &requestQueue[smallest]);
            i = smallest;
        } else break;
    }

    return top;
}

// Remove request by satelliteId from the min-heap
int removeRequestById(int satelliteId) {
    for (int i = 0; i < queueSize; i++) {
        if (requestQueue[i].satelliteId == satelliteId) {
            // Replace with the last element
            requestQueue[i] = requestQueue[queueSize - 1];
            queueSize--;

            // Re-heapify from index i
            int parent = (i - 1) / 2;
            if (i > 0 && requestQueue[i].priority < requestQueue[parent].priority) {
                // Sift up
                while (i > 0 && requestQueue[i].priority < requestQueue[parent].priority) {
                    swapRequests(&requestQueue[i], &requestQueue[parent]);
                    i = parent;
                    parent = (i - 1) / 2;
                }
            } else {
                // Sift down
                while (1) {
                    int left = 2 * i + 1;
                    int right = 2 * i + 2;
                    int smallest = i;

                    if (left < queueSize && requestQueue[left].priority < requestQueue[smallest].priority)
                        smallest = left;
                    if (right < queueSize && requestQueue[right].priority < requestQueue[smallest].priority)
                        smallest = right;

                    if (smallest != i) {
                        swapRequests(&requestQueue[i], &requestQueue[smallest]);
                        i = smallest;
                    } else break;
                }
            }

            return 1; // removed successfully
        }
    }
    return 0; // not found
}


int findRequest(int id){
    for (int i = 0; i < queueSize; i++)
    {
        if(requestQueue[i].satelliteId == id){
            return 1;
        }
    }
    return 0;
}

// Engineer thread function
void* engineer(void* arg) {
    int engineerId = *(int*)arg;
    free(arg); // Free the allocated memory for engineerId
    while (1) {
        // Wait for a new request
        sem_wait(&newRequest);
        pthread_mutex_lock(&engineerMutex);
        if(shutdownFlag == 1 && queueSize == 0){ 
            pthread_mutex_unlock(&engineerMutex);
            printf("[ENGINEER %d] -> Engineer %d is exiting...\n", engineerId, engineerId); 
            return NULL;
        }
        if (queueSize > 0 && availableEngineers > 0) {
            availableEngineers--;
            
            //sleep(1);
            SatelliteRequest req = popRequest();
            
            pthread_mutex_unlock(&engineerMutex);
            
            printf("[ENGINEER %d] -> Engineer %d is handling Satellite %d with priority %d.\n",engineerId,engineerId,req.satelliteId, req.priority);
            

            // Simulate work
            sleep(7);

            
            printf("[ENGINEER %d] -> Engineer %d finished handling Satellite %d with priority %d.\n", engineerId, engineerId, req.satelliteId,req.priority);

            sem_post(&req.requestHandled);

            pthread_mutex_lock(&engineerMutex);
            availableEngineers++;
        }
        pthread_mutex_unlock(&engineerMutex);
    }
}
    

void* satellite(void* arg) {
    int satellite_id = *(int*)arg;
    free(arg);

    int priority = rand() % 4 + 1;
    time_t requestTime = time(NULL);

    SatelliteRequest req;
    req.priority = priority;
    req.satelliteId = satellite_id;
    req.requestTime = requestTime;
    sem_init(&req.requestHandled, 0, 0); 
    
    pthread_mutex_lock(&engineerMutex);
    pushRequest(req);
    printf("[SATELLITE] -> Satellite %d with priority %d requesting...\n", satellite_id, priority);
    pthread_mutex_unlock(&engineerMutex);

    sem_post(&newRequest); // Signal engineer

    // Wait with timeout
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += MAX_WAIT_TIME;

    int result = sem_timedwait(&req.requestHandled, &ts);

    pthread_mutex_lock(&engineerMutex);
    
    pthread_mutex_unlock(&engineerMutex);

    if (result == -1 && errno == ETIMEDOUT ) { // if req is still in the which means not popped yet 
        pthread_mutex_lock(&engineerMutex);
        int stillQueued = findRequest(satellite_id);
        if(stillQueued){
            removeRequestById(satellite_id);
            printf("[TIMEOUT] Satellite %d timeout after %d seconds.\n", satellite_id, MAX_WAIT_TIME);
            //sleep(1);
        }
        pthread_mutex_unlock(&engineerMutex);
    }

    sem_destroy(&req.requestHandled);
    pthread_exit(NULL);
}





int main() {
    srand((unsigned)time(NULL));

    sem_init(&newRequest, 0, 0);


    pthread_mutex_init(&engineerMutex, NULL);

    int satellite_number = (rand() % 5) + 5 ; // Random number of satellites between 5 and 10
    
    printf("Creating number of %d satellites...\n", satellite_number);

    pthread_t engineers[NUM_ENGINEERS];
    pthread_t satellites[satellite_number];

    // Start engineer threads
    for (int i = 0; i < NUM_ENGINEERS; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&engineers[i], NULL, engineer, id);
    }


    // Start satellite threads
    for (int i = 0; i < satellite_number; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&satellites[i], NULL, satellite, id);
        sleep(1);
    }

    // Wait for satellites to finish
    for (int i = 0; i < satellite_number; i++) {
        pthread_join(satellites[i], NULL);
    }

    
    // Wait for all requests to be handled (you can keep handledRequests logic if you want)
    sleep(1); // Let engineers finish last tasks (optional)

    // Signal engineers to shut down
    pthread_mutex_lock(&engineerMutex);
    shutdownFlag = 1;
    pthread_mutex_unlock(&engineerMutex);

    // Post enough to wake all engineers for them to shutdown(exit).
    for (int i = 0; i < NUM_ENGINEERS; i++) {
        sem_post(&newRequest);
    }

    // Wait for engineers to finish
    for (int i = 0; i < NUM_ENGINEERS; i++) {
        pthread_join(engineers[i], NULL);
    }

    // Note: engineer threads run indefinitely; in a real program you'd signal them to exit.

    // Clean up
    sem_destroy(&newRequest);
    pthread_mutex_destroy(&engineerMutex);

    printf("Program finished.\n");
    return 0;
}
