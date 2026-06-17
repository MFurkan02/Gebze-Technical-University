#ifndef BANK_H
#define BANK_H
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define MAX_CLIENTS 20
#define MAX_ACCOUNTS 100
#define SERVER_FIFO "bank_server_fifo"
#define CLIENT_FIFO_PREFIX "bank_client_fifo_"
#define LOG_FILE "AdaBank.bankLog"
#define BUFFER_SIZE 256
#define BANK_DATABASE "BankDatabase.db"

//static int client_number = 1;

// Account structure
typedef struct {
    char id[20];
    int balance;
    int deposit ;
    int withdraw ;
    int active;
    int client_id ;
} Account;

// Message structure for client-server communication
typedef struct {
    pid_t client_pid;
    char account_id[20];
    char operation[10]; // for deposit and  withdraw
    int amount;
    char response_fifo[50];
    int client_id;
} BankMessage;

// Shared memory structure
typedef struct {
    Account accounts[MAX_ACCOUNTS];
    int account_count;
} BankData;

// Function prototypes
void handle_signal(int sig);
void log_transaction();
void update_bank_database();
void read_bank_database();
pid_t Teller(void *(*func)(void *), void *arg_func);
int waitTeller(pid_t pid, int *status);
void *deposit(void *arg);
void *withdraw(void *arg);

#endif // BANK_H
