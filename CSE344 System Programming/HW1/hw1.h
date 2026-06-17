#ifndef HW1
#define HW1
#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <time.h>
#include <sys/file.h>


// Function Prototypes
void createDir(const char *folderName);
void createFile(const char *fileName);
void listDir(const char *folderName);
void listFilesByExtension(const char *folderName, const char *extension);
void readFile(const char *fileName);
void appendToFile(const char *fileName, const char *content);
void deleteFile(const char *fileName);
void deleteDir(const char *folderName);
void showLogs();
void log_operation(const char *message,int b_time);
void print_usage();

#endif

