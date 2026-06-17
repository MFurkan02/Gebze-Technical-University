#include "hw1.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    if (strcmp(argv[1], "createDir") == 0 && argc == 3) {
        createDir(argv[2]);
    } 
   
    else if (strcmp(argv[1], "createFile") == 0 && argc == 3) {
        createFile(argv[2]);
    }
   
    else if (strcmp(argv[1], "listDir") == 0 && argc == 3) {
        listDir(argv[2]);
    }  
   
    else if (strcmp(argv[1], "listFilesByExtension") == 0 && argc == 4) {
        listFilesByExtension(argv[2], argv[3]);
    } 
   
    else if (strcmp(argv[1], "readFile") == 0 && argc == 3) {
        readFile(argv[2]);
    } 
   
    else if (strcmp(argv[1], "appendToFile") == 0 && argc == 4) {
        appendToFile(argv[2], argv[3]);
    } 
    
    else if (strcmp(argv[1], "deleteFile") == 0 && argc == 3) {
        deleteFile(argv[2]);
    } 
    
    else if (strcmp(argv[1], "deleteDir") == 0 && argc == 3) {
        deleteDir(argv[2]);
    } 
    
    else if (strcmp(argv[1], "showLogs") == 0 && argc == 2) {
        showLogs();
    } 
    
    else {
        print_usage();
    }

    return 0;
}

void createDir(const char *folderName){
    if(mkdir(folderName,0755)==-1){
        write(STDERR_FILENO, "Error: Directory '", 19);
        write(STDERR_FILENO, folderName, strlen(folderName));
        write(STDERR_FILENO, "' already exists.\n", 19);
    
        return;
    }


    write(STDOUT_FILENO, "Directory '", 12);
    write(STDOUT_FILENO, folderName, strlen(folderName));
    write(STDOUT_FILENO, "' created successfully.\n", 25);
    
    log_operation("Directory '",1);
    log_operation(folderName,0);
    log_operation("' created successfully.\n",0);

}


void createFile(const char *fileName){

    int fd = open(fileName, O_CREAT | O_RDWR | O_EXCL , 0644 ) ;
    if(fd==-1){
        write(STDERR_FILENO, "Error: File '", 14);
        write(STDERR_FILENO, fileName, strlen(fileName));
        write(STDERR_FILENO, "' already exists.\n", 19);
        
        return;
    }

    time_t tm = time(NULL)  ;

    write(fd, "Created on: ", 13);
    write(fd, ctime(&tm), strlen(ctime(&tm)));
    close(fd);

    write(STDERR_FILENO, "File '", 7);
    write(STDERR_FILENO, fileName, strlen(fileName));
    write(STDERR_FILENO, "' created successfully.\n", 25);
    
    log_operation("File '",1);
    log_operation(fileName,0);
    log_operation("' created successfully.\n",0);


}

void listDir(const char *folderName){ 
    pid_t pid = fork();

    if(pid < 0 ){
        write(STDERR_FILENO,"Fork Error!",12);
        exit(1);
    }
    else if(pid == 0){ // child process
        DIR *directory = opendir(folderName);

        if (directory == NULL){
            write(STDERR_FILENO, "Error: Directory '", 19);
            write(STDERR_FILENO, folderName, strlen(folderName));
            write(STDERR_FILENO, "' not found.\n", 14);

            exit(1);
        }

        struct dirent* entry;
        while ((entry = readdir(directory)) != NULL) {

            write(STDOUT_FILENO,entry->d_name,strlen(entry->d_name));
            write(STDOUT_FILENO,"\n",2);

        }

        closedir(directory);
        exit(0);
    }
        else {  // Parent process waits for child
            wait(NULL);
        }
    
}

void listFilesByExtension(const char *folderName, const char *extension){ // fork()

    pid_t pid = fork();

    if ( pid < 0 ) {
        write(STDERR_FILENO,"Fork Error!",12);
        exit(1);
    }
    else if ( pid == 0 ) { // child process
        
        DIR *directory = opendir(folderName);

        if (directory == NULL){

            write(STDERR_FILENO, "Error: Directory '", 19);
            write(STDERR_FILENO, folderName, strlen(folderName));
            write(STDERR_FILENO, "' not found.\n", 14);

            exit(1);
        }

        struct dirent* entry ;
        int bool = 0 ;

        while ( (entry = readdir(directory)) != NULL )
        {
            if(entry->d_type == DT_REG ) {
                const char* dot = strrchr(entry->d_name, '.'); // finding the extension type after the dot (.'txt')
                if(dot && strcmp(dot,extension)==0){
                    write(STDOUT_FILENO,entry->d_name,strlen(entry->d_name));
                    write(STDOUT_FILENO,"\n",2);
                    bool = 1 ;
                }
            } 
        }

        if (bool == 0) {
            write(STDERR_FILENO, "No files with extension '", 26);
            write(STDERR_FILENO, extension, strlen(extension));
            write(STDERR_FILENO, "' found in '", 13);
            write(STDERR_FILENO, folderName, strlen(folderName));
            write(STDERR_FILENO, "'.\n", 4);

        }
        
        
        closedir(directory);
        exit(0);

    }
    else {  // Parent process waits for child
        wait(NULL);
    }

}

void readFile(const char *fileName){
    int fd=open(fileName,O_RDONLY);
    if(fd==-1){
        write(STDERR_FILENO, "Error: File '", 14);
        write(STDERR_FILENO, fileName, strlen(fileName));
        write(STDERR_FILENO, "' not found.\n", 14);

        return;
    }

    char buffer[1024];
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytesRead);
    }
    close(fd);

}

void appendToFile(const char *fileName, const char *content){
    int fd = open(fileName,O_WRONLY | O_APPEND);
    if(fd==-1){
        write(STDERR_FILENO, "Error: Can not write to '", 26);
        write(STDERR_FILENO, fileName, strlen(fileName));
        write(STDERR_FILENO, "'.File is locked or read-only.\n",32);

        return;
    }

    flock(fd, LOCK_EX);
    write(fd, content, strlen(content));
    write(fd,"\n",2);
    flock(fd, LOCK_UN);

    close(fd);

    log_operation("Content '",1);
    log_operation(content,0);
    log_operation("' is appended to file '",0);
    log_operation(fileName,0);
    log_operation("' .\n",0);

}

void deleteFile(const char *fileName){
    pid_t pid = fork();

    if(pid < 0 ){
        write(STDERR_FILENO, "Fork Error\n", 12);
        exit(1);
    }
    if(pid==0){
        if(unlink(fileName)==0){
            write(STDOUT_FILENO, "File '", 7);
            write(STDOUT_FILENO, fileName, strlen(fileName));
            write(STDOUT_FILENO, "' deleted successfully.\n", 25);

            log_operation("File '",1);
            log_operation(fileName,0);
            log_operation("' deleted successfully.\n",0);

        } else {
            write(STDERR_FILENO, "Error: File '", 14);
            write(STDERR_FILENO, fileName, strlen(fileName));
            write(STDERR_FILENO, "' not found.\n", 14);

        }
        exit(0);
    }
    else {
        wait(NULL);
    }
}


void deleteDir(const char *folderName){
    pid_t pid = fork();

    if(pid < 0 ){
        write(STDERR_FILENO, "Fork Error\n", 12);
        exit(1);
    }
    if(pid==0){
        if(rmdir(folderName)==0){
            write(STDOUT_FILENO, "Directory '", 12);
            write(STDOUT_FILENO, folderName, strlen(folderName));
            write(STDOUT_FILENO, "' deleted successfully.\n", 25);
         
            log_operation("Directory '",1);
            log_operation(folderName,0);
            log_operation("' deleted successfully.\n",0);
        }
        else {
            write(STDERR_FILENO, "Error: Directory '", 19);
            write(STDERR_FILENO, folderName, strlen(folderName));
            write(STDERR_FILENO, "' is not empty or does not exist.\n", 35);
     
        }
        exit(0);
    }
    else {
        wait(NULL);
    }
}


void showLogs(){

    int fd=open("log.txt",  O_RDONLY);
    if(fd==-1){
        write(STDERR_FILENO, "Error : File 'log.txt' does not exist.\n", 40);
        return;
    }

    char buffer[1024];
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytesRead);
    }
    close(fd);


}

void log_operation(const char *message,int b_time){

    int fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if(fd == -1){
        write(STDERR_FILENO, "Error: Could not open 'log.txt'.\n", 34);
        return;
    }

    if(b_time==1){ // since snprintf is forbidden, only for the first write() function time will be written to the log.txt file from the called log_operation functions.
        time_t t = time(NULL);
        char *time_str = ctime(&t);
        // Remove the newline character at the end of time_str
        time_str[strlen(time_str) - 1] = '\0';

        write(fd, "[",1);
        write(fd,time_str,strlen(time_str));
        write(fd, "] - ",2);
    }

    write(fd,message, strlen(message));
    close(fd);

}

void print_usage() {
    write(STDOUT_FILENO,"Example output:\n"
           "Usage: ./fileManager <command> [arguments]\n\n"
           "Commands:\n"
           "  createDir \"folderName\"         - Create a new directory\n"
           "  createFile \"fileName\"          - Create a new file\n"
           "  listDir \"folderName\"           - List all files in a directory\n"
           "  listFilesByExtension \"folderName\" \".txt\" - List files with specific extension\n"
           "  readFile \"fileName\"            - Read a file's content\n"
           "  appendToFile \"fileName\" \"new content\"  - Append content to a file\n"
           "  deleteFile \"fileName\"          - Delete a file\n"
           "  deleteDir \"folderName\"         - Delete an empty directory\n"
           "  showLogs                        - Display operation logs\n",621);
}
