#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sys/stat.h>
#include <thread>
#include <vector>

// Function to create a file with a given index in the specified directory
void createFile(const std::string& dirName, int fileIndex) {
    std::ofstream file(dirName + "/file" + std::to_string(fileIndex) + ".txt", std::ios::binary);
    if (!file) {
        std::cerr << "Error creating file in " << dirName << "/file" << fileIndex << ".txt" << std::endl;
        exit(1);
    } else {
        const int fileSize = 1024;
        char buffer[fileSize] = {};
        file.write(buffer, fileSize);

        std::cout << "File created successfully: " << dirName << "/file" << fileIndex << ".txt" << std::endl;
    }
}

// Function to create files in a specified directory using multiple threads
void createDirMultiThreaded(const std::string& dirName, int numFiles, int numThreads) {
    std::vector<std::thread> threads;

    // Create the directory if it doesn't exist
    if (mkdir(dirName.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0 && errno != EEXIST) {
        std::cerr << "Error creating directory: " << dirName << std::endl;
        exit(1);
    }

    // Calculate the number of files each thread will create
    int filesPerThread = numFiles / numThreads;
    int extraFiles = numFiles % numThreads; // Handle the remainder

    for (int i = 0; i < numThreads; ++i) {
        int startFileIndex = i * filesPerThread + 1;
        int endFileIndex = (i == numThreads - 1) ? (i + 1) * filesPerThread + extraFiles : (i + 1) * filesPerThread;

        // Launch a thread to create files in the specified range
        threads.emplace_back([dirName, startFileIndex, endFileIndex]() {
            for (int fileIndex = startFileIndex; fileIndex <= endFileIndex; ++fileIndex) {
                createFile(dirName, fileIndex);
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
    const int numFiles = 100;

    // Get the number of available cores
    unsigned int numCores = std::thread::hardware_concurrency();

    // Create directories using multiple threads
    createDirMultiThreaded(dir1, numFiles, numCores);
    createDirMultiThreaded(dir2, numFiles, numCores);

    return 0;
}
