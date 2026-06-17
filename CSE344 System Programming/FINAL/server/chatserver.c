#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>

// === Constants ===
#define MAX_CLIENTS 50
#define MAX_USERNAME 17
#define MAX_ROOMNAME 33
#define MAX_FILENAME 50
#define MAX_MSG 1024
#define MAX_FILE_SIZE (3 * 1024 * 1024)
#define FILE_RECV_TIMEOUT_SEC 5
#define MAX_UPLOAD_QUEUE 5
#define MAX_FILES 15
#define MAX_ROOMS 15
#define LOGFILE "chat.log"

// Terminal color codes for styled output
#define GREEN "\033[0;32m"
#define RED   "\033[0;31m"
#define RESET "\033[0m"

// === Data Structures ===

// Holds information about each connected client
typedef struct {
    int socket;
    char username[MAX_USERNAME];
    char previous_room[MAX_ROOMNAME][MAX_ROOMS];
    char room[MAX_ROOMNAME];
    char files[MAX_FILENAME][MAX_FILES];
    struct sockaddr_in addr;
    int active;
} client_t;

// Represents a file upload request between two clients
typedef struct {
    char filename[256];
    char sender[MAX_USERNAME];
    char receiver[MAX_USERNAME];
    int client_socket;
    size_t size;
} file_request_t;

// === Global Variables ===
client_t clients[MAX_CLIENTS]; // Array of connected clients
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER; // Lock for client list
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;     // Lock for logging
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;   // Lock for file queue
sem_t queue_sem; // Semaphore for upload queue control

volatile sig_atomic_t stop = 0;

file_request_t upload_queue[MAX_UPLOAD_QUEUE]; // Bounded file upload queue
int queue_front = 0, queue_rear = 0, queue_size = 0; // Queue pointers
FILE *log_file; // Log file pointer

// === Utility Functions ===

// Thread-safe logger for server events
void log_event(const char *format, ...) {
    pthread_mutex_lock(&log_mutex);
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    fprintf(log_file, "%s - ", timestamp);

    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);

    fprintf(log_file, "\n");
    fflush(log_file);
    pthread_mutex_unlock(&log_mutex);
}

// Sends a message to all clients in a specified chat room
void broadcast_message(const char *room, const char *sender, const char *msg) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].active && strcmp(clients[i].room, room) == 0) {
            char buffer[MAX_MSG + MAX_USERNAME + 32];
            snprintf(buffer, sizeof(buffer), "[ROOM:%s] %s: %s\n", room, sender, msg);
            send(clients[i].socket, buffer, strlen(buffer), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Finds a client in the active list by username
client_t* find_client_by_name(const char* name) {
    //printf("searched name is %s\n",name);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && strcmp(clients[i].username, name) == 0)
            return &clients[i];
    }
    return NULL;
}


int get_all_active_clients(){
    int active_clients = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active){
            //printf("USER : %s , ROOM : %s n",clients[i].username , clients[i].room);     
            active_clients++;
        }
    }
    return active_clients;
}


// Helper function to check if a file exists for rename logic
int file_exists(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

// Generate a new filename if it already exists, e.g., project1.pdf -> project1_1.pdf
void get_unique_filename(const char *orig_name, char *new_name, size_t max_len) {
    if (!file_exists(orig_name)) {
        strncpy(new_name, orig_name, max_len);
        new_name[max_len-1] = '\0';
        return;
    }
    char name[256], ext[64];
    // Split filename and extension
    char *dot = strrchr(orig_name, '.');
    if (dot) {
        size_t base_len = dot - orig_name;
        strncpy(name, orig_name, base_len);
        name[base_len] = '\0';
        strncpy(ext, dot, sizeof(ext));
        ext[sizeof(ext)-1] = '\0';
    } else {
        strncpy(name, orig_name, sizeof(name));
        name[sizeof(name)-1] = '\0';
        ext[0] = '\0';
    }
    int counter = 1;
    do {
        snprintf(new_name, max_len, "%s_%d%s", name, counter, ext);
        counter++;
    } while (file_exists(new_name));
}



// Adds a file upload request to the bounded upload queue
void enqueue_file(file_request_t request) {


    sem_wait(&queue_sem);
    pthread_mutex_lock(&queue_mutex);
    upload_queue[queue_rear] = request;
    queue_rear = (queue_rear + 1) % MAX_UPLOAD_QUEUE;
    queue_size++;
    pthread_mutex_unlock(&queue_mutex);
    log_event("[FILE-QUEUE] Upload '%s' from %s added to queue. Queue size: %d", request.filename, request.sender, queue_size);
}


int is_valid_extension(const char *filename) {
    int is_extension = 0;
    const char *ext = strrchr(filename, '.');
    if (!ext) return 0;  // No extension
    
    ext++;  // Move past the dot
    
    if(strcmp(ext,"txt")==0){
        is_extension = 1 ;
    }
    else if(strcmp(ext,"pdf")==0){
        is_extension = 1 ;
    }
    else if(strcmp(ext,"png")==0){
        is_extension = 1 ;
    }
    else if(strcmp(ext,"jpg")==0){
        is_extension = 1 ;
    }

    return is_extension;
}

int is_file_less_than_3mb(const char *filename) {
    struct stat file_stat;
    
    if (stat(filename, &file_stat) != 0) {
        perror("Error getting file status");
        return 0;
    }
    
    return file_stat.st_size < MAX_FILE_SIZE ;
}

// Returns a formatted string with size in MB (caller must free)
char* get_file_size_mb(const char *filename) {
    struct stat file_stat;
    if (stat(filename, &file_stat) != 0) {
        return NULL;
    }

    // Allocate buffer for result
    char* result = malloc(32); // Enough for "XXX.XXX MB"
    if (!result) return NULL;

    // Calculate size in MB with 3 decimal places
    double size_mb = (double)file_stat.st_size / (1024 * 1024);
    snprintf(result, 32, "%.3f MB", size_mb);

    return result;
}


int file_exists_client(client_t* receiver,const char* filename){
    for (int i = 0; i < MAX_FILES; i++)
    {
        if(strcmp(receiver->files[i],filename)==0){
            return 1;
        }
    }
    return 0;
}

// Returns 1 if successful, 0 if array is full
int add_filename(client_t* recv , const char *new_filename) {
    // Find first empty slot (empty string or null terminator in first position)
    for (int i = 0; i < MAX_FILES; i++) {
        if (recv->files[i][0] == '\0') {  // Check if slot is empty
            strcpy(recv->files[i], new_filename);
            //recv->files[i][MAX_FILENAME-1] = '\0';  // Ensure null-termination
            return 1;  // Success
        }
    }
    
    // Array is full - handle error (you could also overwrite last element)
    return 0;  // Failed to add
}


// Returns 1 if already exists, 0 if array is full or added to array
int add_previous_room(client_t* cli , const char *new) {
    // Find first empty slot (empty string or null terminator in first position)
    for (int i = 0; i < MAX_ROOMS; i++) {
        if(strcmp(cli->previous_room[i],new)==0){
            return 1;
        }
        else if (cli->previous_room[i][0] == '\0') {  // Check if slot is empty
            strcpy(cli->previous_room[i], new);
            return 0;  // Success
        }
    }
    
    // Array is full - handle error (you could also overwrite last element)
    return 0;  // Failed to add
}


// Dedicated thread for processing file upload requests
void* file_handler(void *arg) {
    while (1) {
        pthread_mutex_lock(&queue_mutex);
        if (queue_size == 0) {
            pthread_mutex_unlock(&queue_mutex);
            sleep(1); // Idle wait when no requests
            continue;
        }

        // Dequeue file request
        file_request_t req = upload_queue[queue_front];
        queue_front = (queue_front + 1) % MAX_UPLOAD_QUEUE;
        queue_size--;
        pthread_mutex_unlock(&queue_mutex);
        
        sleep(2); // Simulate upload delay

        // Try delivering file to intended receiver
        client_t *receiver = find_client_by_name(req.receiver);
        if (receiver) {
            char notify[512];
            snprintf(notify, sizeof(notify), "[FILE] '%s' received from %s\n", req.filename, req.sender);
            send(receiver->socket, notify, strlen(notify), 0);
            log_event("[SEND FILE] '%s' sent from %s to %s (success)", req.filename, req.sender, req.receiver);
        } else {
            log_event("[SEND FILE] '%s' from %s failed - receiver not found", req.filename, req.sender);
        }

        sem_post(&queue_sem);
    }
    return NULL;
}

// Handles all interactions with a single connected client
void *client_handler(void *arg) {
    int idx = *(int*)arg;
    free(arg);
    char buffer[MAX_MSG];
    client_t *cli = &clients[idx];

    while (!stop) {
        int bytes = recv(cli->socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) break; // Disconnected
        buffer[bytes] = '\0';

        if (strncmp(buffer, "/join ", 6) == 0) {
            char new_room[MAX_ROOMNAME] = {0};


            if (sscanf(buffer + 6, "%s", new_room) != 1) {
                send(cli->socket, "[ERROR] Usage: /join <room_name>\n", 31, 0);
            }
            else {
                // room2 room1 -> room2
                if (add_previous_room(cli,new_room)) {
                    // Rejoin same room
                    char msg[128];
                    snprintf(msg, sizeof(msg), "[SERVER] You rejoined the room '%s'\n", new_room);
                    send(cli->socket, msg, strlen(msg), 0);
                    log_event("[JOIN] user '%s' rejoined room '%s'", cli->username, new_room);
                    printf("[COMMAND] %s rejoined room '%s'\n", cli->username, new_room);
                    strcpy(cli->room,new_room);
                } else if (strlen(cli->room) > 0) {
                    // Switch from previous room
                    char msg[128];
                    snprintf(msg, sizeof(msg), "[ROOM] User %s left room '%s' and joined '%s'\n",
                            cli->username, cli->room, new_room);
                    send(cli->socket, msg, strlen(msg), 0);
                    log_event("[JOIN] user '%s' joined room '%s' from room '%s'",
                            cli->username, new_room, cli->room);
                    printf("[COMMAND] %s joined room '%s' from room '%s'\n", cli->username, new_room, cli->room);

                    // Update previous room
                    add_previous_room(cli,cli->room);
                    strcpy(cli->room, new_room);
                } else {
                    // First time joining
                    strcpy(cli->room, new_room);
                    char msg[128];
                    snprintf(msg, sizeof(msg), "[SERVER] You joined the room '%s'\n", new_room);
                    send(cli->socket, msg, strlen(msg), 0);
                    log_event("[JOIN] user '%s' joined room '%s'", cli->username, new_room);
                    printf("[COMMAND] %s joined room '%s'\n", cli->username, new_room);
                    add_previous_room(cli,new_room);
                }
            } 
        }
        else if (strncmp(buffer, "/leave", 6) == 0) {
            if (strlen(buffer) > 6 && buffer[6] != '\n' && buffer[6] != '\0') {
                send(cli->socket, "[ERROR] Usage: /leave\n", 21, 0);
            } else {
                log_event("[LEAVE] user '%s' left room '%s'\n", cli->username,cli->room);
                printf("[COMMAND] user '%s' left room '%s'\n", cli->username,cli->room);
                add_previous_room(cli,cli->room);
                strcpy(cli->room, "");
                send(cli->socket, "[SERVER] Left room\n", 20, 0);
            }
        } else if (strncmp(buffer, "/broadcast ", 11) == 0) {
            if (strlen(buffer + 11) == 0 || buffer[11] == '\n' || buffer[11] == '\0') {
                send(cli->socket, "[ERROR] Usage: /broadcast <message>\n", 35, 0);
            } else {
                broadcast_message(cli->room, cli->username, buffer + 11);
                log_event("[BROADCAST] user '%s' broadcasted %s", cli->username, buffer + 11);
                printf("[COMMAND] user '%s' broadcasted to %s\n", cli->username, cli->room);
            }
        } else if (strncmp(buffer, "/whisper ", 9) == 0) {

            char target[MAX_USERNAME];
            char *msg = strchr(buffer + 9, ' ');
            if (!msg || strlen(msg + 1) == 0) {
                send(cli->socket, "[ERROR] Usage: /whisper <username> <message>\n", 44, 0);
            } else {
                
                    *msg = '\0';
                    strcpy(target, buffer + 9);
                    msg++;
                    client_t *dest = find_client_by_name(target);
                    if (dest) {
                        char whisper[MAX_MSG];
                        snprintf(whisper, sizeof(whisper), "[WHISPER] %s: %s\n", cli->username, msg);
                        send(dest->socket, whisper, strlen(whisper), 0);
                        send(cli->socket,"Whisper is sent\n",18,0);
                        log_event("[WHISPER] %s whispered -> %s: %s", cli->username, target, msg);
                        printf("[COMMAND] %s whispered -> %s: %s\n", cli->username, target, msg);
                    } else {
                        send(cli->socket, "[ERROR] User not found\n", 25, 0);
                        log_event("[ERROR] User '%s' not found\n",target);
                        printf("[ERROR] User '%s' not found\n",target);
                    }
                
            }   
        } 
        else if (strncmp(buffer, "/sendfile ", 10) == 0) {
            char filename[256], receiver[MAX_USERNAME];

            if (sscanf(buffer + 10, "%255s %s", filename, receiver) != 2) {
                send(cli->socket, "[ERROR] Usage: /sendfile <filename> <receiver>\n", 46, 0);
                continue;
            }
            client_t* recv = find_client_by_name(receiver);
            char msg[MAX_MSG];
            if(recv==NULL){
                snprintf(msg, sizeof(msg), "[ERROR] User '%s' does not exist\n", receiver);
                send(cli->socket, msg, strlen(msg), 0);
                printf("%s", msg);
                log_event("%s", msg);
            }
            else if(file_exists(filename)==0){
                snprintf(msg, sizeof(msg), "[ERROR] File '%s' does not exist\n", filename);
                send(cli->socket, msg, strlen(msg), 0);
                printf("%s", msg);
                log_event("%s", msg);
            }

            else if(!is_file_less_than_3mb(filename)){
                char msg[MAX_MSG];
                char *file_size = get_file_size_mb(filename);
                snprintf(msg, sizeof(msg), "[ERROR] File '%s' (%s) from user '%s' exceeds size limit.\n",filename, file_size ,cli->username);
                log_event("[ERROR] File '%s' (%s) from user '%s' exceeds size limit.\n",filename, file_size ,cli->username);
                printf("[ERROR] File '%s' (%s) from user '%s' exceeds size limit.\n",filename, file_size ,cli->username);
                send(cli->socket,msg,strlen(msg),0);
                free(file_size);
            }
            else if (!is_valid_extension(filename)){
                char msg[MAX_MSG];
                snprintf(msg, sizeof(msg), "[ERROR] File '%s' from user '%s' does not match the extensions(.txt,.pdf,.png,.jpg)\n", filename,cli->username);
                log_event( "[ERROR] File '%s' from user '%s' does not match the extensions(.txt,.pdf,.png,.jpg)\n", filename,cli->username);
                printf( "[ERROR] File '%s' from user '%s' does not match the extensions(.txt,.pdf,.png,.jpg)\n", filename,cli->username);
                send(cli->socket,msg,strlen(msg),0);
            }
            else {
                
                if(file_exists_client(recv,filename)){
                    // Prepare filename to save, rename if already exists
                    //char saved_filename[MAX_FILENAME+10];
                    //get_unique_filename(filename, saved_filename, sizeof(saved_filename));
                    snprintf(msg, sizeof(msg), "[FILE] Conflict: file '%s' already exists for user '%s'. Please check the filename and try sending it again!\n", filename,recv->username);
                    send(cli->socket,msg,strlen(msg),0);
                    send(recv->socket,msg,strlen(msg),0);
                    printf("%s\n",msg);
                    log_event("%s\n",msg);
                }
                else {
                    char *file_size = get_file_size_mb(filename);
                    char msg[MAX_MSG];
                    snprintf(msg, sizeof(msg), "[FILE] File '%s' (%s) from user '%s' sent to user '%s'.\n", filename,file_size,cli->username,recv->username);
                    send(cli->socket,msg,strlen(msg),0);
                    send(recv->socket,msg,strlen(msg),0);
                    log_event(msg);
                    add_filename(recv,filename);
                                 
                    free(file_size);
                }
            }
        } 
            else if (strncmp(buffer, "/exit", 5) == 0) {
                    break;
            } 
            else {
                send(cli->socket, "[ERROR] Unknown command\n", 25, 0);
            }
        }

  
            // Cleanup after client disconnects
            log_event("[DISCONNECT] user '%s' disconnected", cli->username);
            printf("[DISCONNECT] user '%s' disconnected\n", cli->username);
            close(cli->socket);
            cli->active = 0;
            pthread_exit(NULL); 
}

// Handles server shutdown and cleanup on SIGINT (Ctrl+C)
void handle_sigint(int sig) {
    log_event("[SHUTDOWN] SIGINT received. Disconnecting clients.");
    int active_clients = get_all_active_clients();
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) {
            send(clients[i].socket, "[INFO] Server shutting down\n", 28, 0);
            //shutdown(clients[i].socket, SHUT_RDWR);
            close(clients[i].socket);
            clients[i].active = 0;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    printf(RED "[SHUTDOWN] Signal received! Server is shut down! Disconnecting all %d clients...\n" RESET,active_clients);
    if(log_file) fclose(log_file);

     // Destroy semaphore
    sem_destroy(&queue_sem);

    exit(0);
}

// === Main Server Entry Point ===
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }

    signal(SIGPIPE, SIG_IGN);

    int port = atoi(argv[1]);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);


    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, MAX_CLIENTS);
    

    signal(SIGINT, handle_sigint);
    sem_init(&queue_sem, 0, MAX_UPLOAD_QUEUE);
    log_file = fopen(LOGFILE, "a");
    printf("[START] Server listening on port %d...\n", port);
    log_event("[START] Server listening on port %d...", port);
    

    // Start file upload handler thread
    pthread_t file_thread;
    pthread_create(&file_thread, NULL, file_handler, NULL);
    pthread_detach(file_thread);  

    while (1) {
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &len);

    // Receive client username
    char username[MAX_USERNAME] = {0};
    ssize_t n = recv(client_socket, username, MAX_USERNAME - 1, 0);
    if (n <= 0) {
        close(client_socket);
        continue;
    }
    username[n] = '\0';

    pthread_mutex_lock(&clients_mutex);

    if (find_client_by_name(username) != NULL) {
        pthread_mutex_unlock(&clients_mutex);
        const char *msg = "[ERROR] Username already taken.Choose another.\n";
        send(client_socket, msg, strlen(msg), 0);
        log_event("[REJECTED] Duplicate username attempted: '%s'\n", username);
        close(client_socket);
        //pthread_mutex_unlock(&clients_mutex);
        continue;
    }

    int i;
    for (i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active == 0) {
            clients[i].socket = client_socket;
            strncpy(clients[i].username, username, MAX_USERNAME - 1);
            clients[i].username[MAX_USERNAME - 1] = '\0';  // Ensure null-termination
            clients[i].addr = client_addr;
            clients[i].active = 1;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    // Reject connection if server is full
        if (i == MAX_CLIENTS) {
            const char *msg = "[ERROR] Server full\n";
            send(client_socket, msg, strlen(msg), 0);
            close(client_socket);
        } else {
            int *arg = malloc(sizeof(int));
            if (arg == NULL) {
                perror("malloc");
                close(client_socket);
                continue;
            }
            *arg = i;

            pthread_t tid;
            if (pthread_create(&tid, NULL, client_handler, arg) != 0) {
                perror("pthread_create");
                free(arg);
                close(client_socket);
                clients[i].active = 0;
                continue;
            }
            pthread_detach(tid);

            printf("[CONNECT] New Client connected: '%s' connected from %s\n", username, inet_ntoa(client_addr.sin_addr));
            log_event("[CONNECT] New Client connected: '%s' connected from %s", username, inet_ntoa(client_addr.sin_addr));
        }
    }
}
