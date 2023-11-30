#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sys/stat.h>
#include <cstring>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>

std::atomic<int> j(1);
std::mutex errorsMutex; 

struct ErrorInfo {
    std::string fileName;
    std::string errorMessage;
};

bool createDirectory(const std::string& dirName) {
    struct stat info;
    if (stat(dirName.c_str(), &info) != 0) {
        if (mkdir(dirName.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
            std::cerr << "Error creating directory " << dirName << ": " << strerror(errno) << std::endl;
            return false;
        }
    }
    return true;
}

void createFile(const std::string& dirName, int fileIndex, std::vector<ErrorInfo>& errors) {
    std::ofstream file(dirName + "/f" + std::to_string(fileIndex) + ".txt", std::ios::binary);
    if (!file) {
        ErrorInfo error;
        error.fileName = dirName + "/f" + std::to_string(fileIndex) + ".txt";
        error.errorMessage = strerror(errno);
        std::lock_guard<std::mutex> lock(errorsMutex); // Lock the mutex
        errors.push_back(error);
    } else {
        const int fileSize = 10;
        char buffer[fileSize] = {};
        file.write(buffer, fileSize);

        std::cout << "File created successfully: " << dirName << "/f" << fileIndex << ".txt" << std::endl;
    }
}

void createDirRecursive(const std::string& dirName, std::atomic<int>& fileIndex, std::vector<ErrorInfo>& errors) {
    createDirectory(dirName);

    const int filesPerFolder = 100;

    for (int i = 0; i < filesPerFolder; ++i) {
        if (fileIndex.load() > 10000) {
            break;
        }

        createFile(dirName, fileIndex.load(), errors);
        ++fileIndex;
    }

    if (fileIndex.load() <= 10000) {
        std::string subDir = dirName + "/subdir";
        createDirRecursive(subDir, fileIndex, errors);
    }
}

void createDirs(const std::string& rootDir, int totalFiles) {
    std::vector<ErrorInfo> errors1;
    std::vector<ErrorInfo> errors2;
    std::atomic<int> j1(1);
    std::thread thread1(createDirRecursive, rootDir + "1", std::ref(j1), std::ref(errors1));
    std::atomic<int> j2(1);
    createDirRecursive(rootDir + "2", j2, errors2);
    thread1.join();
}

int main() {
    const std::string rootDir = "dir";
    const int totalFiles = 10000;

    createDirs(rootDir, totalFiles);

    return 0;
}