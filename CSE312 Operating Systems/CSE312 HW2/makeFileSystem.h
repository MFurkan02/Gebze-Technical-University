#ifndef MAKEFILESYSTEM_H
#define MAKEFILESYSTEM_H

#include "makeFileSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>

// Constants
#define MAX_FILE_SYSTEM_SIZE (4 * 1024 * 1024) // 4 MB
#define SUPER_BLOCK_SIZE 1024 // 1 KB -> size of the super block
#define DIRECTORY_ENTRY_SIZE 256 // Size of each directory entry ( 256 bytes )

struct SuperBlock {
    uint32_t totalSize; // Total size of the file system
    uint32_t blockSize; // Size of each block in the file system
    uint32_t freeBlocksCount; // Number of free blocks in the file system
    uint32_t rootDirOffset; // Offset of the root directory in the file system
    uint32_t fatOffset; // Offset of the File Allocation Table (FAT) in the file system
    uint32_t dataOffset; // Offset of the data area in the file system
};

// Struct of directory entry in the file system
struct DirectoryEntry {
    char *fileName; // Name of the file or directory
    uint32_t size; // Size of the file in bytes
    uint16_t permissions; // Permissions of the file (e.g., read/write)
    time_t lastModification; // Last modification time of the file
    time_t creationTime; // Creation time of the file
    char *password; // Password for accessing the file (if protected)
    uint16_t firstBlock; // Index of the first data block allocated to the file
};

// Structure of an entry in the File Allocation Table (FAT) of the file system
struct fat_entry {
    uint16_t value; // Value indicating the next block in the chain (0xFFFF indicates end of chain)
};

// Structure of the file system
struct FileSystem {
    SuperBlock superBlock; // Super block of the file system
    std::vector<DirectoryEntry> directoryTable; // Vector storing directory entries
    std::vector<fat_entry> FAT; // Vector storing entries of the File Allocation Table (FAT)
    std::vector<bool> freeBlockBitmap; // Vector representing the free blocks in the file system
    std::vector<uint8_t *> dataBlocks; // Vector storing pointers to data blocks in the file system
};

// Functions
void createFileSystem(int blockSizeKB, const char *fileName); // Function to create the file system
void formatFileSystem(FileSystem &fs); // Function to format the file system
void loadFileSystem(FileSystem &fs, const char *filename); // to load the file system.
void saveFileSystem(const FileSystem &fs, const char *filename); // to save the file system.

#endif // MAKEFILESYSTEM_H


