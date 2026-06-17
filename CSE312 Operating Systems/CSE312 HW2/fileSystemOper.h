#ifndef FILESYSTEMOPER_H
#define FILESYSTEMOPER_H

#include <string>

class FileSystemOper {
public:
    FileSystemOper(const std::string& fileName);
    ~FileSystemOper();

    void mkdir_fs(const std::string& path);  // Function to create a director
    void rmdir_fs(const std::string& path);  // Function to remove a director
    void dumpe2fs(); // Function to display file system information
    void writeFile(const std::string& path, const std::string& fileName); // Function to write data to a file -write
    void readFile(const std::string& path, const std::string& fileName); // Function to read data from a file -read-
    void deleteFile(const std::string& path, const std::string& fileName); // Function to delete a file
    void chmod_fs(const std::string& path, const std::string& fileName, const std::string& permissions);  // Function to change file permissions
    void addpw(const std::string& path, const std::string& fileName, const std::string& password); // Function to add a password to a file
    void dir(const std::string& path); // Function to list directory contents 

private:
    std::string fileName;

    // Helper functions
    void parsePath(const std::string& path, std::string& directory, std::string& file);
    void copyFilePermissions(const std::string& sourceFile, const std::string& destFile);
};

#endif // FILESYSTEMOPER_H
