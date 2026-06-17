#include "fileSystemOper.h"
#include "makeFileSystem.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>



// Function to list the contents of a directory
void dir(FileSystem &fs, const char *path) {
    // Traverse the directory table to find entries with the specified path
    for (const auto &entry : fs.directoryTable) {
        // Check if the entry's path matches the specified path
        if (strcmp(entry.fileName, path) == 0) {
            // Print directory contents
            for (const auto &dirEntry : fs.directoryTable) {
                // Check if the entry is in the specified directory
                if (strstr(dirEntry.fileName, path) == dirEntry.fileName) {
                    // Print file details
                    printf("File: %s\n", dirEntry.fileName);
                    printf("Size: %u bytes\n", dirEntry.size);
                    printf("Permissions: %c%c\n", (dirEntry.permissions & 0b01) ? 'R' : '-', (dirEntry.permissions & 0b10) ? 'W' : '-');
                    printf("Last Modification: %s", ctime(&dirEntry.lastModification));
                    printf("Creation Time: %s", ctime(&dirEntry.creationTime));
                    printf("Password Protected: %s\n", (dirEntry.password ? "Yes" : "No"));
                }
            }
            return;
        }
    }
    // If directory not found, print error
    printf("Directory not found: %s\n", path);
}

// Function to create a directory
void mkdir_fs(FileSystem &fs, const char *path) {
    // Create a new directory entry
    DirectoryEntry entry;
    entry.fileName = strdup(path);
    entry.size = 0;
    entry.permissions = 0b11; // Default permissions (read and write)
    entry.lastModification = time(NULL);
    entry.creationTime = time(NULL);
    entry.password = nullptr; // No password initially
    entry.firstBlock = 0xFFFF; // Indicates no blocks allocated

    // Add the new directory entry to the file system directory table
    fs.directoryTable.push_back(entry);

    printf("Directory created: %s\n", path);
}

// Function to remove a directory
void rmdir_fs(FileSystem &fs, const char *path) {
    // Traverse the directory table to find entries with the specified path
    for (auto it = fs.directoryTable.begin(); it != fs.directoryTable.end(); ++it) {
        // Check if the entry's path matches the specified path
        if (strcmp(it->fileName, path) == 0) {
            // Erase the directory entry
            fs.directoryTable.erase(it);
            printf("Directory removed: %s\n", path);
            return;
        }
    }
    // If directory not found, print error
    printf("Directory not found: %s\n", path);
}

// Function to dump file system information
void dumpe2fs(const FileSystem &fs) {
    // Print file system information
    printf("Total blocks: %u\n", fs.superBlock.totalSize / fs.superBlock.blockSize);
    printf("Block size: %u bytes\n", fs.superBlock.blockSize);
    printf("Free blocks: %u\n", fs.superBlock.freeBlocksCount);

    // Print occupied blocks and file names
    printf("Occupied blocks and files:\n");
    for (const auto &entry : fs.directoryTable) {
        uint16_t block = entry.firstBlock;
        printf("File: %s\n", entry.fileName);
        while (block != 0xFFFF) {
            printf("Block: %u\n", block);
            block = fs.FAT[block].value;
        }
    }
}

void writeFile(FileSystem &fs, const char *path, const char *linuxFile) {
    // Check if the Linux file exists
    struct stat fileInfo;
    if (stat(linuxFile, &fileInfo) == -1) {
        perror("Could not access Linux file");
        return;
    }

    // Open the Linux file for reading
    int fd = open(linuxFile, O_RDONLY);
    if (fd == -1) {
        perror("Could not open Linux file");
        return;
    }

    // Allocate memory for file data
    char *data = (char *)malloc(fileInfo.st_size);
    // Read data from the Linux file
    read(fd, data, fileInfo.st_size);
    // Close the Linux file
    close(fd);

    // Extract file name from the path
    const char *fileName = strrchr(path, '/') + 1;
    // Create a new directory entry in the file system
    mkdir_fs(fs, fileName);
    // Write data to the file in the file system
    ::writeFile(fs, fileName, data); // Adjusted to match the function signature
    // Free allocated memory
    free(data);

    printf("File written: %s\n", path);
}


// Function to read a file
void readFile(FileSystem &fs, const char *path, const char *linuxFile, const char *password) {
    // Extract file name from the path
    const char *fileName = strrchr(path, '/') + 1;
    // Allocate memory for file data
    char *buffer = (char *)malloc(MAX_FILE_SYSTEM_SIZE); // Assuming MAX_FILE_SIZE is defined elsewhere
    if (buffer == nullptr) {
        perror("Memory allocation failed");
        return;
    }
    // Traverse the directory table to find the specified file
    for (const auto &entry : fs.directoryTable) {
        // Check if the entry's file name matches the specified file name
        if (strcmp(entry.fileName, fileName) == 0) {
            // Check if the file is password protected and password is provided
            if (entry.password && strcmp(entry.password, password) != 0) {
                printf("Incorrect password.\n");
                free(buffer); // Free allocated memory before returning
                return;
            }

            // Read data from the file in the file system
            ::readFile(fs, fileName, buffer,password);

            // Open the Linux file for writing
            int fd = open(linuxFile, O_WRONLY | O_CREAT, entry.permissions);
            if (fd == -1) {
                perror("Could not open Linux file for writing");
                free(buffer); // Free allocated memory before returning
                return;
            }
            // Write data to the Linux file
            if (write(fd, buffer, entry.size) == -1) {
                perror("Error writing to Linux file");
                close(fd);    // Close the file descriptor
                free(buffer); // Free allocated memory before returning
                return;
            }
            // Close the Linux file
            close(fd);
            free(buffer); // Free allocated memory before returning
            return;
        }
    }
    // If file not found, print error
    printf("File not found: %s\n", fileName);
    free(buffer); // Free allocated memory before returning
}







// Function to delete a file
void deleteFile(FileSystem &fs, const char *path) {
    // Extract file name from the path
    const char *fileName = strrchr(path, '/') + 1;
    // Traverse the directory table to find the specified file
    for (auto it = fs.directoryTable.begin(); it != fs.directoryTable.end(); ++it) {
        // Check if the entry's file name matches the specified file name
        if (strcmp(it->fileName, fileName) == 0) {
            // Erase the file entry
            fs.directoryTable.erase(it);
            printf("File deleted: %s\n", path);
            return;
        }
    }
    // If file not found, print error
    printf("File not found: %s\n", fileName);
}

// Function to change file permissions
void chmod_fs(FileSystem &fs, const char *path, uint16_t permissions) {
    // Extract file name from the path
    const char *fileName = strrchr(path, '/') + 1;
    // Traverse the directory table to find the specified file
    for (auto &entry : fs.directoryTable) {
        // Check if the entry's file name matches the specified file name
        if (strcmp(entry.fileName, fileName) == 0) {
            // Update file permissions
            entry.permissions = permissions;
            printf("Permissions changed for %s\n", path);
            return;
        }
    }
    // If file not found, print error
    printf("File not found: %s\n", fileName);
}

// Function to add a password to a file
void addpw(FileSystem &fs, const char *path, const char *password) {
    // Extract file name from the path
    const char *fileName = strrchr(path, '/') + 1;
    // Traverse the directory table to find the specified file
    for (auto &entry : fs.directoryTable) {
        // Check if the entry's file name matches the specified file name
        if (strcmp(entry.fileName, fileName) == 0) {
            // Allocate memory for the password
            entry.password = strdup(password);
            printf("Password added to %s\n", path);
            return;
        }
    }
    // If file not found, print error
    printf("File not found: %s\n", fileName);
}


int main(int argc, char *argv[]) {

    const char *command = argv[1];

    if (strcmp(command, "makeFileSystem") == 0) {
        if (argc != 4) {
            std::cerr << "Usage: " << argv[0] << " makeFileSystem <blockSizeKB> <fileName>\n";
            return 1;
        }
        int blockSize = std::atoi(argv[2]);
        if (blockSize <= 0) {
            std::cerr << "Block Size must be a positive integer.\n";
            return 1;
        }
        const char *fileName = argv[3];
        FileSystem fs;
        createFileSystem(blockSize, fileName);
        formatFileSystem(fs);
        saveFileSystem(fs, fileName);
        std::cout << "FileSystem is created!\n";
        return 0; // Exit after creating the file system
    }

   const char *command2 = argv[2];

    const char *fileSystemFile = argv[1];
    FileSystem fs;
    loadFileSystem(fs, fileSystemFile);

    if (strcmp(command2, "mkdir") == 0) {
        if (argc != 4) {
            std::cerr << "Usage: " << argv[0] << " mkdir <fileSystemFile> <path>\n";
            return 1;
        }
        mkdir_fs(fs, argv[3]);
        saveFileSystem(fs, fileSystemFile);
    } else if (strcmp(command2, "rmdir") == 0) {
        if (argc != 4) {
            std::cerr << "Usage: " << argv[0] << " rmdir <fileSystemFile> <path>\n";
            return 1;
        }
        rmdir_fs(fs, argv[3]);
        saveFileSystem(fs, fileSystemFile);
    } else if (strcmp(command2, "write") == 0) {
        if (argc != 5) {
            std::cerr << "Usage: " << argv[0] << " write <fileSystemFile> <path_in_fs> <linux_file>\n";
            return 1;
        }
        writeFile(fs, argv[3], argv[4]);
        saveFileSystem(fs, fileSystemFile);
    } else if (strcmp(command2, "dir") == 0) {
        if (argc != 4) {
            std::cerr << "Usage: " << argv[0] << " dir <fileSystemFile> <path>\n";
            return 1;
        }
        dir(fs, argv[3]);
    } else if (strcmp(command2, "del") == 0) {
        if (argc != 4) {
            std::cerr << "Usage: " << argv[0] << " del <fileSystemFile> <path>\n";
            return 1;
        }
        deleteFile(fs, argv[3]);
        saveFileSystem(fs, fileSystemFile);
    } else if (strcmp(command2, "dumpe2fs") == 0) {
        dumpe2fs(fs);
    } else if (strcmp(command2, "read") == 0) {
        if (argc != 6) {
            std::cerr << "Usage: " << argv[0] << " read <fileSystemFile> <path_in_fs> <linux_file> <password>\n";
            return 1;
        }
        readFile(fs, argv[3], argv[4], argv[5]);
    } else if (strcmp(command2, "chmod") == 0) {
        if (argc != 5) {
            std::cerr << "Usage: " << argv[0] << " chmod <fileSystemFile> <path_in_fs> <permissions>\n";
            return 1;
        }
        chmod_fs(fs, argv[3], static_cast<uint16_t>(std::stoi(argv[4])));
        saveFileSystem(fs, fileSystemFile);
    } else if (strcmp(command2, "addpw") == 0) {
        if (argc != 5) {
            std::cerr << "Usage: " << argv[0] << " addpw <fileSystemFile> <path_in_fs> <password>\n";
            return 1;
        }
        addpw(fs, argv[3], argv[4]);
        saveFileSystem(fs, fileSystemFile);
    } else {
        std::cerr << "Invalid command: " << command2 << "\n";
        return 1;
    }

    return 0;
}