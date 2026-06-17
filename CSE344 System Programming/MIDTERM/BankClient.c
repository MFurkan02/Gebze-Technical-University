#include "Bank.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s ClientFile ServerFIFO_Name\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *client_file_name = argv[1];
    const char *server_fifo_name = argv[2];

    printf("> BankClient <%s>  #%s\n", client_file_name, server_fifo_name);
    
    // Open client file
    FILE *client_file = fopen(client_file_name, "r");
    if (client_file == NULL) {
        perror("fopen client file failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Reading %s\n", client_file_name);
    
    char line[BUFFER_SIZE];
    int client_count = 0;
    
    // Count lines in client file
    while (fgets(line, sizeof(line), client_file)) {
        if (strlen(line) > 1) {  // Skip empty lines
            client_count++;
        }
    }
    rewind(client_file);
    
    printf("%d clients to connect.. creating clients..\n", client_count);

    // Check if server FIFO exists
    if (access(server_fifo_name, F_OK) == -1) {
        printf("Cannot connect to %s...\n", server_fifo_name);
        printf("exiting..\n");
        exit(EXIT_FAILURE);
    }

    printf("Connected to Adabank..\n");

    // Open server FIFO once and keep it open
    int server_fd = open(server_fifo_name, O_WRONLY);
    if (server_fd == -1) {
        perror("open server fifo failed");
        exit(EXIT_FAILURE);
    }

    // Send client count first
    if (write(server_fd, &client_count, sizeof(int)) != sizeof(int)) {
        perror("write client count failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    int client_no = 0;
    
    // Process each client request
    while (fgets(line, sizeof(line), client_file)) {
        // Skip empty lines
        if (strlen(line) <= 1) continue;

        char account_id[20] = "N";
        char operation[10];
        int amount;
        
        if (sscanf(line, "%s %s %d", account_id, operation, &amount) < 2) {
            fprintf(stderr, "Skipping invalid line: %s", line);
            continue;
        }
        
        client_no++;

        // Create unique client FIFO for response
        char client_fifo[50];
        snprintf(client_fifo, sizeof(client_fifo), "%s%d_%d", CLIENT_FIFO_PREFIX, getpid(), client_no);
        
        // Remove old FIFO if exists
        unlink(client_fifo);
        if (mkfifo(client_fifo, 0666)) {
            perror("mkfifo failed");
            continue;
        }

        // Prepare message for server
        BankMessage msg;
        msg.client_pid = getpid();
        strncpy(msg.account_id, account_id, sizeof(msg.account_id)-1);
        strncpy(msg.operation, operation, sizeof(msg.operation)-1);
        msg.amount = amount;
        strncpy(msg.response_fifo, client_fifo, sizeof(msg.response_fifo)-1);
        
        // Send request to server
        if (write(server_fd, &msg, sizeof(BankMessage)) != sizeof(BankMessage)) {
            perror("write to server failed");
            unlink(client_fifo);
            continue;
        }

        // Immediately print client connection
        if (strcmp(operation, "withdraw") == 0) {
            printf("Client with ID : '%s' connected..withdrawing %d credits\n", msg.account_id, amount);
        } else {
            printf("Client with ID : '%s' connected..depositing %d credits\n", msg.account_id, amount);
        }
        
        // Open client FIFO for reading response (blocks until server opens for writing)
        int client_fd = open(client_fifo, O_RDONLY);
        if (client_fd == -1) {
            perror("open client fifo failed");
            unlink(client_fifo);
            continue;
        }
        
        BankMessage response;
        if (read(client_fd, &response, sizeof(BankMessage)) != sizeof(BankMessage)) {
            perror("read response failed");
            close(client_fd);
            unlink(client_fifo);
            continue;
        }
        close(client_fd);
        unlink(client_fifo);

        // Process response
        if (strcmp(response.operation, "withdraw") == 0) {
            if (response.amount == -1) {
                printf("Client with ID : %s - something went WRONG\n", response.account_id);
            } else if (response.amount == -2) {
                printf("Client with ID : %s - operation not permitted.\n", response.account_id);
            } else if (response.amount == 0) {
                printf("Client with ID : %s served.. account closed\n", response.account_id);
            } else {
                printf("Client with ID : %s served.. %s\n", response.account_id, response.account_id); 
            }
        } else if (strcmp(response.operation, "deposit") == 0) {
            if (response.amount == -1) {
                printf("Client with ID : %s - something went WRONG\n", response.account_id);
            } else {
                printf("Client with ID : %s served.. %s\n", response.account_id, response.account_id);
            }
        }
    }

    // Cleanup
    close(server_fd);
    fclose(client_file);
    printf("All clients processed. Exiting..\n");
    
    return 0;
}