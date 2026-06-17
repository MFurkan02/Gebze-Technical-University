#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <signal.h>


#define PIPE1 "/tmp/pipe1"
#define PIPE2 "/tmp/pipe2"
#define LOGFILE "/tmp/daemon_log.txt"

int child_counter = 0;
int total_children = 2;
pid_t child2_pid = 0;  // PID of child2

// ============================== Logging
void log_event(const char *msg) {
    FILE *log = fopen(LOGFILE, "a");
    if (log) {
        time_t now = time(NULL);
        fprintf(log, "%s: %s\n", ctime(&now), msg);
        fclose(log);
    }
}

// ============================== SIGTERM Handler for Child2
void handle_sigterm(int sig) {
    (void) sig;
    log_event("Child2 received SIGTERM and is exiting.");
    exit(1);
}

// ============================== SIGCHLD Handler (zombie cleanup + logging)
void sigchld_handler(int sig) {
    (void)sig;
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        char buffer[256];
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%c", tm_info);

        if (WIFEXITED(status)) {
            snprintf(buffer, sizeof(buffer),
                     "Child with PID %d exited with status %d at %s",
                     pid, WEXITSTATUS(status), time_str);
        } else if (WIFSIGNALED(status)) {
            snprintf(buffer, sizeof(buffer),
                     "Child with PID %d was killed by signal %d at %s",
                     pid, WTERMSIG(status), time_str);
        } else {
            snprintf(buffer, sizeof(buffer),
                     "Child with PID %d terminated abnormally at %s",
                     pid, time_str);
        }

        log_event(buffer);
        child_counter++;
    }
}

// ============================== Daemonization
void daemonize() {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    if (setsid() < 0) exit(EXIT_FAILURE);
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    umask(0);
    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    open(LOGFILE, O_RDWR | O_CREAT | O_APPEND, 0600);


    dup(1);
    dup(2);
}

// ============================== Daemon Signal Handler
void signal_handler(int sig) {
    if (sig == SIGTERM) {
        log_event("Daemon received SIGTERM, exiting.");
        exit(0);
    } else if (sig == SIGHUP) {
        log_event("Daemon received SIGHUP.");
    } else if (sig == SIGUSR1) {
        log_event("Daemon received SIGUSR1.");
    }
}

// ============================== Child 1: Compare Numbers
void child_process1() {
    sleep(10);
    int fd1 = open(PIPE1, O_RDONLY);
    if (fd1 < 0) exit(EXIT_FAILURE);

    int num1, num2;
    read(fd1, &num1, sizeof(int));
    read(fd1, &num2, sizeof(int));
    log_event("Child1 has read the both numbers\n");
    close(fd1);

    int fd2 = open(PIPE2, O_WRONLY);
    if (num1 > num2) {
        write(fd2, &num1, sizeof(int));
    } else {
        write(fd2, &num2, sizeof(int));
    }
    log_event("Child1 has written the larger number to the second pipe.\n");
    close(fd2);
    exit(EXIT_SUCCESS);
}

// ============================== Child 2: Read and Print
void child_process2() {
    signal(SIGTERM, handle_sigterm);  // Handle timeout termination
    sleep(10);

    int fd2 = open(PIPE2, O_RDONLY);
    if (fd2 < 0) exit(EXIT_FAILURE);

    int result = 0;
    read(fd2, &result, sizeof(int));
    log_event("Child2 has read the larger number\n");
    printf("The larger number is %d\n", result);
    close(fd2);

    exit(EXIT_SUCCESS);
}

// ============================== Log Child Creation
void log_child_creation(pid_t pid) {
    time_t now = time(NULL);
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Child with PID %d created at %s", pid, ctime(&now));
    log_event(buffer);
}

// ============================== Terminate Child2 from Daemon
void terminate_child2(pid_t pid) {
    int kill(pid_t pid, int sig);

    if (kill(pid, SIGTERM) == 0) {
        log_event("Timeout reached. Sending SIGTERM to child2.");
        log_event("Daemon terminating after timeout.");
    } else {
        log_event("Failed to send SIGTERM to child2.");
    }
}
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <int1> <int2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int number1 = atoi(argv[1]);
    int number2 = atoi(argv[2]);

    printf("Number1 is %d , Number2 is %d\n", number1, number2);

    // Reset log file
    FILE *log = fopen(LOGFILE, "w");
    if (log) fclose(log);

    // Create FIFOs
    if (mkfifo(PIPE1, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo PIPE1");
        exit(EXIT_FAILURE);
    }
    if (mkfifo(PIPE2, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo PIPE2");
        exit(EXIT_FAILURE);
    }

    signal(SIGCHLD, sigchld_handler);  // Handle child exits

    // Slight delay before forking to prevent immediate finish
    sleep(2);

    // Child 1
    pid_t child1 = fork();
    if (child1 == 0) {
        log_child_creation(getpid());
        child_process1();
    }

    // Child 2
    child2_pid = fork();
    if (child2_pid == 0) {
        log_child_creation(getpid());
        child_process2();
    }

    // Write numbers to PIPE1
    sleep(1);  // Ensure child1 is ready
    int fd1 = open(PIPE1, O_WRONLY);
    if (fd1 < 0) {
        perror("open PIPE1 for writing");
        exit(EXIT_FAILURE);
    }
    write(fd1, &number1, sizeof(int));
    write(fd1, &number2, sizeof(int));
    close(fd1);

    


    // Fork Daemon
    pid_t daemon_pid = fork();
    if (daemon_pid == 0) {
        daemonize();
        signal(SIGUSR1, signal_handler);
        signal(SIGHUP, signal_handler);
        signal(SIGTERM, signal_handler);
        sleep(30);
        exit(0);
    }

    
    // Main process loop
    int timeout = 0;
    int proceed = 0;
    while (timeout < 30) {
        fprintf(stderr, "Proceeding...\n");
        sleep(2);
        timeout += 2;
        proceed++;

        if (child_counter >= total_children && proceed > 5 ) {
            break; // Exit the loop if all children are done
        }
    }

    if (child_counter >= total_children) {
        fprintf(stderr, "All children finished successfully.\n");
        log_event("All children finished successfully.\n");
        log_event("Program has terminated successfully!\n");
    } else {
        fprintf(stderr, "Timeout reached in parent. Proceeding to cleanup.\n");
        terminate_child2(child2_pid);
    }

    unlink(PIPE1);
    unlink(PIPE2);

    
    return 0;
}