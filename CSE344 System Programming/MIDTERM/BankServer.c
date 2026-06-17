#include "Bank.h"

BankData *shared_data;
int shm_id, sem_id;
const char *server_fifo;


// Signal handler
void handle_signal(int sig) {
    (void)sig;
    printf("\nSignal received closing active Tellers\n");

    // Cleanup shared resources if they exist
    if (shared_data != (void *)-1) {
        shmdt(shared_data);
    }
    if (shm_id != -1) {
        shmctl(shm_id, IPC_RMID, NULL);
    }
    if (sem_id != -1) {
        semctl(sem_id, 0, IPC_RMID);
    }

    // Remove server FIFO
    unlink(server_fifo);
    printf("Removing ServerFIFO '%s'... Updating log file...\n", server_fifo);
    printf("Adabank says \"Bye\"...\n");

    exit(EXIT_SUCCESS);
}

// Initialize shared memory and semaphore
void init_shared_resources() {
    // Initialize IDs to invalid values
    shm_id = -1;
    sem_id = -1;
    shared_data = (void *)-1;

    // Create shared memory
    shm_id = shmget(IPC_PRIVATE, sizeof(BankData), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    shared_data = (BankData *)shmat(shm_id, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    // Initialize bank data
    memset(shared_data, 0, sizeof(BankData));
    
    // Create semaphore
    sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget failed");
        exit(EXIT_FAILURE);
    }
    
    // Initialize semaphore to 1 (binary semaphore for mutual exclusion)
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;
    
    arg.val = 1;
    if (semctl(sem_id, 0, SETVAL, arg) == -1) {
        perror("semctl failed");
        exit(EXIT_FAILURE);
    }
}

// Semaphore operations
void sem_lock() {
    struct sembuf sb = {0, -1, 0};
    if (semop(sem_id, &sb, 1) == -1) {
        perror("semop lock failed");
        // Don't exit here as it might be during cleanup
        return;
    }
}

void sem_unlock() {
    struct sembuf sb = {0, 1, 0};
    if (semop(sem_id, &sb, 1) == -1) {
        perror("semop unlock failed");
        // Don't exit here as it might be during cleanup
        return;
    }
}

// Create a new account
char* create_account(BankMessage *msg) {
    static char account_id[20];
    snprintf(account_id, sizeof(account_id), "BankID_%02d", shared_data->account_count + 1);
    
    sem_lock();

    strcpy(shared_data->accounts[shared_data->account_count].id, account_id);
    shared_data->accounts[shared_data->account_count].balance = 0;
    shared_data->accounts[shared_data->account_count].active = 1;
    shared_data->accounts[shared_data->account_count].deposit = 0;
    shared_data->accounts[shared_data->account_count].withdraw = 0;
    shared_data->accounts[shared_data->account_count].client_id = msg->client_id ;

    shared_data->account_count++;

    sem_unlock();
    
    return account_id;
}

// Find account by ID
Account* find_account(const char *account_id) {
    sem_lock();
    for (int i = 0; i < shared_data->account_count; i++) {
        if (strcmp(shared_data->accounts[i].id, account_id) == 0 && shared_data->accounts[i].active==1 ) {
            sem_unlock();
            return &shared_data->accounts[i];
        }
    }
    sem_unlock();
    return NULL;
}

// Process deposit request
void *deposit(void *arg) {
    //printf("inside deposit!\n");
    BankMessage *msg = (BankMessage *)arg;
    Account *account = find_account(msg->account_id);
    
    if (account == NULL && strcmp(msg->account_id,"N")==0 ) {
        // New account
        //printf("creating new account...\n");
        strcpy(msg->account_id, create_account(msg));
        account = find_account(msg->account_id);
    }
    else if(account==NULL && msg->account_id[0]!='N'){ 
        printf("Client with ID : %s deposited %d credits...operation not permitted...account_id is not found in database and not a 'N'(new) account!!!\n",msg->account_id,msg->amount);
        msg->amount = -1;
        int client_fd = open(msg->response_fifo, O_WRONLY);
        if (client_fd == -1) {
            perror("open client fifo failed");
            exit(EXIT_FAILURE);
        }
        
        write(client_fd, msg, sizeof(BankMessage));
        close(client_fd);
        return NULL;
    }



    sem_lock();
    account->balance += msg->amount;
    account->deposit += msg->amount;
    printf("Client with ID : %s deposited %d credits...updating log...\n",msg->account_id,msg->amount);
    //log_transaction(account->id, account->deposit ,  account->withdraw, account->balance);
    sem_unlock();
    
    // Send response to client
    int client_fd = open(msg->response_fifo, O_WRONLY);
    if (client_fd == -1) {
        perror("open client fifo failed");
        exit(EXIT_FAILURE);
    }
    
    write(client_fd, msg, sizeof(BankMessage));
    close(client_fd);
    


    return NULL;
}

// Process withdraw request
void *withdraw(void *arg) {
    BankMessage *msg = (BankMessage *)arg;
    Account *account = find_account(msg->account_id);
    
    if (account == NULL) {
        // Account not found
        msg->amount = -1; // Error code
        if(strcmp(msg->account_id,"N")!=0){
            printf("Account(%s) not found!!!\n",msg->account_id);
        }
        else {
            printf("New account can not withdraw money!!!\n");
            strcpy(msg->account_id, create_account(msg));
            account = find_account(msg->account_id);
        }
    } else {
        sem_lock();
        if (account->balance >= msg->amount) {
            account->balance -= msg->amount;
            account->withdraw += msg->amount;
            if(account->balance !=0){
                printf("Client with ID : %s withdrawn %d credits...updating log...\n",msg->account_id,msg->amount);
                //log_transaction(account->id, account->deposit ,  account->withdraw, account->balance);
            }
            
            // Close account if balance is zero
            if (account->balance == 0) {
                printf("Client with ID : %s withdrawn %d credits...updating log...Bye %s...\n",msg->account_id,msg->amount,msg->account_id);
                //log_transaction(account->id, account->deposit ,  account->withdraw, account->balance);
                account->active = 0;
            }
        } else {
            printf("Client with ID : %s withdrawn %d credits...operation not permitted... insufficient funds...\n",msg->account_id,msg->amount);
            msg->amount = -2; // Insufficient funds
        }
        sem_unlock();
    }
    
    // Send response to client
    int client_fd = open(msg->response_fifo, O_WRONLY);
    if (client_fd == -1) {
        perror("open client fifo failed");
        exit(EXIT_FAILURE);
    }
    
    write(client_fd, msg, sizeof(BankMessage));
    close(client_fd);


    return NULL;
}


void update_bank_database(){
    // Lock shared memory before accessing
    sem_lock();
    
    FILE* database = fopen(BANK_DATABASE, "w");
    if (database == NULL) {
        perror("Failed to open database for writing");
        sem_unlock();
        return;
    }

    for(int i=0 ; i < shared_data->account_count ; i++){
        if(shared_data->accounts[i].active)
            fprintf(database,"%s D %d W %d B %d\n", shared_data->accounts[i].id ,shared_data->accounts[i].deposit,shared_data->accounts[i].withdraw,shared_data->accounts[i].balance);
        else {
            fprintf(database,"# %s D %d W %d B %d\n", shared_data->accounts[i].id ,shared_data->accounts[i].deposit,shared_data->accounts[i].withdraw,shared_data->accounts[i].balance);

        }
    }

    fclose(database);
    sem_unlock();

}




void read_bank_database(){
    
    sem_lock();
    FILE* database = fopen(BANK_DATABASE, "r");
    if (database == NULL) {
        // Database doesn't exist yet - this is normal for first run
        sem_unlock();
        return;
    }

    char line[BUFFER_SIZE];
    shared_data->account_count = 0 ;

    while (fgets(line, sizeof(line), database)) {
        char account_id[20] ;
        int deposit ;
        int withdraw;
        int balance;
        
        if (sscanf(line, "%19s D %d W %d B %d", account_id, &deposit, &withdraw , &balance) == 4 ) {
            if (shared_data->account_count >= MAX_ACCOUNTS) {
                fprintf(stderr, "Warning: Maximum account limit(%d) reached\n",MAX_ACCOUNTS);
                break;
            }

            strcpy(shared_data->accounts[shared_data->account_count].id,account_id);
            shared_data->accounts[shared_data->account_count].deposit = deposit ;
            shared_data->accounts[shared_data->account_count].withdraw = withdraw ;
            shared_data->accounts[shared_data->account_count].balance = balance ;
            shared_data->accounts[shared_data->account_count].active = 1;
            //shared_data->accounts[shared_data->account_count].client_id = client_number++;

            shared_data->account_count++;
        }
    }
    fclose(database);
    sem_unlock();
}


void get_database(){
    printf("Bank Database is loading...\n");
    for(int i=0 ; i < shared_data->account_count ; i++){
        printf("BankID :%s , Deposit : %d , Withdraw : %d, Balance : %d\n", shared_data->accounts[i].id ,shared_data->accounts[i].deposit,shared_data->accounts[i].withdraw,shared_data->accounts[i].balance);
    }
}



// Teller process creation
pid_t Teller(void *(*func)(void *), void *arg_func) {
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process (Teller)
        func(arg_func);
        exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        // Parent process (Bank Server)
        return pid;
    } else {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
}

// Signal handler for SIGCHLD
void sigchld_handler(int signum) {
    (void)signum;
    int saved_errno = errno; // Save errno because waitpid might change it
    while (waitpid(-1, NULL, WNOHANG) > 0); // Reap all finished children
    errno = saved_errno; // Restore errno
}


// Wait for Teller process
int waitTeller(pid_t pid, int *status) {
    return waitpid(pid, status, 0);
}

// Log transaction to file
void log_transaction() {
    FILE *log_file = fopen(LOG_FILE, "w");
    if (log_file == NULL) {
        perror("fopen log file failed");
        exit(EXIT_FAILURE);
    }

    time_t now;
    time(&now);
    fprintf(log_file, "# Adabank Log file updated @%s", ctime(&now));
    
   for(int i=0 ; i<shared_data->account_count ; i++){
        if(shared_data->accounts[i].active == 0){
            fprintf(log_file, "# %s D %d W %d B %d\n", shared_data->accounts[i].id , shared_data->accounts[i].deposit , shared_data->accounts[i].withdraw , shared_data->accounts[i].balance);
        }
        else if(shared_data->accounts[i].deposit > 0 && shared_data->accounts[i].withdraw > 0){
            //printf("logging both\n");
            fprintf(log_file, "%s D %d W %d B %d\n", shared_data->accounts[i].id , shared_data->accounts[i].deposit , shared_data->accounts[i].withdraw , shared_data->accounts[i].balance);
        }
        else if(shared_data->accounts[i].deposit > 0){
            //printf("logging deposit\n");
            fprintf(log_file, "%s D %d B %d\n",shared_data->accounts[i].id , shared_data->accounts[i].deposit , shared_data->accounts[i].balance);
        }
   }
    
    fprintf(log_file,"\n## end of log\n");
    
    fclose(log_file);
}

int extract_number_from_bank_id(const char *bank_id) {
    int number = 0;
    // Use sscanf to extract the number part after "BankID_"
    if (sscanf(bank_id, "BankID_%d", &number) == 1) {
        return number;
    }
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s AdaBank ServerFIFO_Name\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *bank_name = argv[1];
    server_fifo = argv[2];

    // Initialize shared resources FIRST
    init_shared_resources();
    
    printf("> BankServer %s @%s\n", bank_name, server_fifo);
    printf("%s is active...\n", bank_name);
    
    // Check for existing log file
    FILE *log_file = fopen(LOG_FILE, "r");
    if (log_file == NULL) {
        printf("No previous logs... Creating the bank database\n");
    } else {
        printf("reading database...\n");
        read_bank_database();
        //get_database();
        fclose(log_file);
    }

   
    
    // Set up signal handlers
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);


    // Set up SIGCHLD handler to reap finished Tellers
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }
    
    if (access(server_fifo, F_OK) != -1) {
        printf("FIFO already exists, removing it...\n");
        unlink(server_fifo);
    }

    int result = mkfifo(server_fifo, 0666);

    if (result == 0) {
        printf("FIFO '%s' created successfully in the current directory.\n", server_fifo);
    } else if (errno == EEXIST) {
        printf("FIFO '%s' already exists in the current directory.\n", server_fifo);
    } else {
        perror("Error creating FIFO");
        exit(EXIT_FAILURE);
    }
    
    

    
    while(1){

        printf("Waiting for clients @%s...\n", server_fifo);
        // Open server FIFO for reading
        int server_fd = open(server_fifo, O_RDWR);
        if (server_fd == -1) {
            perror("open server fifo failed");
            unlink(server_fifo);
            exit(EXIT_FAILURE);
        }
        int client_count ;
        read(server_fd,&client_count,sizeof(int));
        printf("Client Number : %d\n",client_count);

        //int client_no = shared_data->account_count;

        for(int i=0;i<client_count ; i++){
            // Read client message
            BankMessage msg;
            read(server_fd, &msg, sizeof(BankMessage));
            //printf("Message is %s-%s-%d\n",msg.account_id,msg.operation,msg.amount);
            
            if(i==0)
                printf("-- Received %d clients from PIDClient%d..\n", client_count , msg.client_pid);
            
            // Create Teller process
            pid_t teller_pid;
            if (strcmp(msg.operation, "deposit") == 0) {
                teller_pid = Teller(deposit, &msg);
                    printf("-- Teller PID%02d is active serving Client with ID : '%s' ...\n", teller_pid, msg.account_id);
            } else if (strcmp(msg.operation, "withdraw") == 0) {
                teller_pid = Teller(withdraw, &msg);
                    printf("-- Teller PID%02d is active serving Client with ID : '%s' ...\n", teller_pid, msg.account_id);
            }

            
            

            int status;
            waitTeller(teller_pid, &status);

            
            
        }

        log_transaction();
        update_bank_database();
        close(server_fd);
    }
        
    
    //get_database();
    
    return 0;
}