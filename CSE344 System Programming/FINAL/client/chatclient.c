#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h> // For errno

// === Constants ===
#define MAX_USERNAME 17   // Max length for username (16 chars + null terminator)
#define BUF_SIZE 1024     // Max size of message buffer

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

// === Global Variables ===
int sockfd = -1;                             // Socket file descriptor, initialized to an invalid state
char username[MAX_USERNAME];                 // Username of the connected user
volatile sig_atomic_t connection_active = 1; // Flag to indicate if the connection is active
volatile sig_atomic_t sigint_received = 0;   // Flag to indicate if SIGINT (Ctrl+C) was received

// === Signal Handler for SIGINT (Ctrl+C) ===
void handle_sigint(int sig) {
    // Functions called from signal handlers must be async-signal-safe.
    // printf, send, close are not strictly async-signal-safe.
    // Setting flags is the safest approach.
    char msg[] = "\nDisconnecting...\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1); // write() is async-signal-safe

    sigint_received = 1;
    connection_active = 0; // Signal other threads/loops to terminate
}

// === Thread Function: Receives and prints messages from the server ===
void *receive_handler(void *arg) {
    char buffer[BUF_SIZE];
    while (connection_active) { // Loop only while the connection is supposed to be active
        if (sockfd < 0) { // Check if socket is valid
            connection_active = 0; // Ensure flag is set if socket is bad
            break;
        }

        // Read from socket
        int n = recv(sockfd, buffer, BUF_SIZE - 1, 0);

        if (n == 0) { // Server closed the connection gracefully
            if (connection_active) { // Check flag to avoid multiple messages
                printf(RED "\n[INFO] Server has closed the connection." RESET "\n");
                printf("Please press Enter to exit.\n"); // Prompt to unblock fgets in main
            }
            connection_active = 0; // Set flag to indicate connection is no longer active
            break;                 // Exit the receive loop
        } else if (n < 0) { // An error occurred during recv
            if (connection_active) { // Avoid printing error if already disconnecting
                // perror("recv error"); // Use perror for system call errors
                fprintf(stderr, RED "\n[INFO] Connection lost due to a receive error (errno %d: %s)." RESET "\n", errno, strerror(errno));
                printf("Please press Enter to exit.\n");
            }
            connection_active = 0; // Set flag
            break;                 // Exit the receive loop
        }
        buffer[n] = '\0'; // Null-terminate the received data

        // If connection became inactive while processing, don't print
        if (!connection_active) break;

        // Print with colored formatting depending on message type
        if (strstr(buffer, "[ERROR]"))
            printf(RED "%s" RESET "\n", buffer);
        else if (strstr(buffer, "[INFO]"))
            printf(GREEN "%s" RESET "\n", buffer);
        else
            printf("%s\n", buffer);
        fflush(stdout); // Ensure the message is printed immediately
    }
    // printf("[DEBUG] Receive handler thread exiting.\n"); // For debugging
    return NULL;
}

// === Helper Function: Validates the username input ===
int is_valid_username(const char *name) {
    if (strlen(name) == 0 || strlen(name) >= MAX_USERNAME) return 0;
    for (int i = 0; name[i]; i++) {
        if (!isalnum(name[i])) return 0; // Only alphanumeric usernames allowed
    }
    return 1;
}

// === Main Entry Point ===
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serv_addr;
    pthread_t tid;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Register SIGINT handler
    signal(SIGINT, handle_sigint);

    // Set server connection info
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2])); // Convert port from string to network byte order
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) { // Convert IP string to binary
        perror("inet_pton failed (invalid IP address)");
        close(sockfd); // Close socket before exiting
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect to server failed");
        close(sockfd); // Close socket before exiting
        exit(EXIT_FAILURE);
    }

    printf(GREEN "[INFO] Connected to server." RESET "\n");

    // Prompt user until a valid username is entered
    do {
        printf("Enter username (max 16 chars, alphanumeric): ");
        if (fgets(username, MAX_USERNAME, stdin) == NULL) {
            // Handle EOF or error on stdin for username input
            if (ferror(stdin)) perror("fgets for username");
            fprintf(stderr, RED "[ERROR] Could not read username. Exiting." RESET "\n");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        username[strcspn(username, "\n")] = 0; // Remove newline character
        if (!connection_active) { // e.g. Ctrl+C during username input
            printf(RED "[INFO] Disconnecting due to signal." RESET "\n");
            close(sockfd);
            exit(EXIT_SUCCESS);
        }
    } while (!is_valid_username(username));

    // Send username to server
    if (send(sockfd, username, strlen(username), 0) < 0) {
        perror("send username failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Create a thread to handle receiving messages from the server
    if (pthread_create(&tid, NULL, receive_handler, NULL) != 0) {
        perror("pthread_create for receive_handler failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    pthread_detach(tid); // Detach the thread as we won't be joining it

    // === Main loop: Read input from user and send to server ===
    char message[BUF_SIZE];

    while (connection_active) {
        //printf("> ");
        if (fgets(message, BUF_SIZE, stdin) == NULL) {
            // This can happen if stdin is closed (e.g., Ctrl+D for EOF) or an error occurs
            if (ferror(stdin)) { // Check if it was an actual error
                // perror("fgets for message input"); // Can be noisy
            }
            // Assume EOF or unrecoverable error on stdin, so initiate disconnect
            // printf(RED "[INFO] Input stream closed or error. Disconnecting." RESET "\n");
            connection_active = 0; // Signal to stop
            if (sigint_received) { // If Ctrl+D followed Ctrl+C
                // handle_sigint already printed a message
            } else {
                 // If only Ctrl+D, try to send /exit
                 if (sockfd != -1) send(sockfd, "/exit", 5, 0); // Best effort
            }
            break;
        }

        // If connection_active was set to 0 by receive_handler or SIGINT while fgets was blocking
        if (!connection_active) {
            // The receive_handler or sigint_handler might have printed "Press Enter..." or "Ctrl+C..."
            // User pressed Enter (or input arrived), fgets returned, now we break.
            break;
        }

        

        message[strcspn(message, "\n")] = 0; // Strip newline

        if (!connection_active) break; // Double check before sending

        // Check for exit command
        if (strcmp(message, "/exit") == 0 )  {
            if (connection_active && sockfd != -1) { // Only send if connection was thought to be active
                if (send(sockfd, message, strlen(message), 0) < 0) {
                    // perror("send /exit command failed"); // Optional: can be noisy if server already gone
                }
            }
            connection_active = 0; // Signal intent to exit
            break; // Exit the main loop
        }

        // Send message to server
        if (connection_active && sockfd != -1) {
            ssize_t sent_bytes = send(sockfd, message, strlen(message), 0);
            if (sent_bytes < 0) {
                if (connection_active) { // Only show error if we weren't already disconnecting
                    if (errno == EPIPE || errno == ECONNRESET) {
                        printf(RED "\n[INFO] Failed to send message: Server disconnected." RESET "\n");
                        printf("Please press Enter to exit.\n");
                    } else {
                        perror("send message failed");
                    }
                    connection_active = 0; // Mark connection as lost
                }
                break; // Exit main loop on send error
            }
        } else if (!connection_active) {
            break; // Break if connection became inactive
        }
    }

    // === Cleanup ===
    connection_active = 0; // Ensure flag is set for receive_handler if it's still looping

    if (sigint_received) {
        // The handle_sigint already printed a message.
        // No further "Disconnected" message needed from here if it was Ctrl+C
    } else {
        printf("Disconnected from server.\n");
    }

    if (sockfd != -1) {
        // Shutdown can help unblock the other side or our own receive thread.
        // SHUT_RD: No more reads from this socket.
        // SHUT_WR: No more writes to this socket.
        // SHUT_RDWR: No more reads or writes.
        shutdown(sockfd, SHUT_RDWR); // Gracefully signal no more send/recv
        close(sockfd);
        sockfd = -1; // Mark as closed
    }

    // printf("[DEBUG] Main thread exiting.\n"); // For debugging
    return 0;
}