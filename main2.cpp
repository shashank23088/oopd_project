

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sys/stat.h>
#include <thread>
#include <vector>
#include <cstring>
#include <mutex>

// Structure to store error information
struct ErrorInfo {
    std::string fileName;
    std::string errorMessage;
};

// Function to create a directory if it doesn't exist
bool createDirectory(const std::string& dirName) {
    struct stat info;
    if (stat(dirName.c_str(), &info) != 0) {
        // Directory doesn't exist, create it
        if (mkdir(dirName.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
            // Failed to create directory
            std::cerr << "Error creating directory " << dirName << ": " << strerror(errno) << std::endl;
            return false;
        }
    }
    return true;
}

// Function to create a file with a given index in the specified directory
void createFile(const std::string& dirName, int fileIndex, std::vector<ErrorInfo>& errors, std::mutex& errorsMutex) {
    std::ofstream file(dirName + "/file" + std::to_string(fileIndex) + ".txt", std::ios::binary);
    if (!file) {
        // Store error information
        ErrorInfo error;
        error.fileName = dirName + "/file" + std::to_string(fileIndex) + ".txt";
        error.errorMessage = strerror(errno); // Include the error message

        // Protect access to the shared vector using a mutex
        std::lock_guard<std::mutex> lock(errorsMutex);
        errors.push_back(error);
    } else {
        const int fileSize = 10;
        char buffer[fileSize] = {};
        file.write(buffer, fileSize);

        std::cout << "File created successfully: " << dirName << "/file" << fileIndex << ".txt" << std::endl;
    }
}

// Function to create files in a specified directory using multiple threads
void createDirMultiThreaded(const std::string& dirName, int numFiles, int numThreads, std::vector<ErrorInfo>& errors, std::mutex& errorsMutex) {
    std::vector<std::thread> threads;

    // Calculate the number of files each thread will create
    int filesPerThread = numFiles / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startFileIndex = i * filesPerThread + 1;
        int endFileIndex = (i == numThreads - 1) ? numFiles : (i + 1) * filesPerThread;

        // Launch a thread to create files in the specified range
        threads.emplace_back([dirName, startFileIndex, endFileIndex, &errors, &errorsMutex]() {
            for (int fileIndex = startFileIndex; fileIndex <= endFileIndex; ++fileIndex) {
                createFile(dirName, fileIndex, errors, errorsMutex);
            }
        });
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }
}

int main() {
    const std::string dir1 = "dir1";
    const std::string dir2 = "dir2";
    const int numFiles = 10000;

    // Create directories if they don't exist
    if (!createDirectory(dir1) || !createDirectory(dir2)) {
        return 1;  // Exit with an error code
    }

    // Get the number of available cores
    unsigned int numCores = std::thread::hardware_concurrency();

    // Vector to store error information
    std::vector<ErrorInfo> errors;
    // Mutex to protect access to the errors vector
    std::mutex errorsMutex;

    // Create directories using multiple threads
    auto start = std::chrono::high_resolution_clock::now();
    createDirMultiThreaded(dir1, numFiles, numCores, errors, errorsMutex);
    createDirMultiThreaded(dir2, numFiles, numCores, errors, errorsMutex);
    auto end = std::chrono::high_resolution_clock::now();

    // Print errors
    if (!errors.empty()) {
        std::cerr << "Errors occurred during file creation:" << std::endl;
        for (const auto& error : errors) {
            std::cerr << "Error creating file " << error.fileName << ": " << error.errorMessage << std::endl;
        }
    }

    return 0;
}
