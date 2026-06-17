#include "makeFileSystem.h"


FileSystem fs; //  global instance of the FileSystem(fs).

// Function to create the file system with the given block size and file name
void createFileSystem(int blockSizeKB, const char *fileName) {
    // Initializing the super block structure with appropriate values
    fs.superBlock.totalSize = MAX_FILE_SYSTEM_SIZE;
    fs.superBlock.blockSize = blockSizeKB * 1024;
    fs.superBlock.freeBlocksCount = (MAX_FILE_SYSTEM_SIZE / fs.superBlock.blockSize) - 1;
    fs.superBlock.rootDirOffset = sizeof(SuperBlock);
    fs.superBlock.fatOffset = fs.superBlock.rootDirOffset + sizeof(DirectoryEntry) * 128; // Assuming max 128 directory entries
    fs.superBlock.dataOffset = fs.superBlock.fatOffset + (fs.superBlock.totalSize / fs.superBlock.blockSize) * sizeof(fat_entry);

    // Creating a new file with the specified file name in binary mode
    std::ofstream fsFile(fileName, std::ios::binary);
    // Setting the file pointer to the end of the file
    fsFile.seekp(MAX_FILE_SYSTEM_SIZE - 1);
    // Writing a single byte to extend the file size to the maximum file system size
    fsFile.write("", 1);
    // Closing the file stream
    fsFile.close();
}

// Function to format the file system
void formatFileSystem(FileSystem &fs) {
    // Clearing the directory table
    fs.directoryTable.clear();
    // Initializing the File Allocation Table (FAT) with default values (0xFFFF indicates free block)
    fs.FAT.assign(fs.superBlock.totalSize / fs.superBlock.blockSize, {0xFFFF});
    // Initializing the free block bitmap (all blocks are initially free)
    fs.freeBlockBitmap.assign(fs.superBlock.totalSize / fs.superBlock.blockSize, false);
    // Updating the count of free blocks
    fs.superBlock.freeBlocksCount = fs.superBlock.totalSize / fs.superBlock.blockSize;

    // Initialize the data blocks vector
    fs.dataBlocks.resize(fs.superBlock.totalSize / fs.superBlock.blockSize);
    // Allocating memory for each data block
    for (auto &block : fs.dataBlocks) {
        // I used unsigned char for binary data
        block = new unsigned char[fs.superBlock.blockSize];
    }
}

// Function to load the file system from a file
void loadFileSystem(FileSystem &fs, const char *filename) {
    std::ifstream in(filename, std::ios::binary);
    if (in.is_open()) {
        in.read(reinterpret_cast<char*>(&fs), sizeof(FileSystem));
        in.close();
    } else {
        std::cerr << "Could not load file system from " << filename << std::endl;
    }
}

// Function to save the file system to a file
void saveFileSystem(const FileSystem &fs, const char *filename) {
    std::ofstream out(filename, std::ios::binary);
    if (out.is_open()) {
        out.write(reinterpret_cast<const char*>(&fs), sizeof(FileSystem));
        out.close();
    } else {
        std::cerr << "Could not save file system to " << filename << std::endl;
    }
}