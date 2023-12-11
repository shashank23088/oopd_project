#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sys/stat.h>

const long long int BYTE = 1;

int j = 1;

void createDir(const std::string& dirName, int totalFiles) {
    if (totalFiles == 0) {
        return;
    }

    mkdir(dirName.c_str(), 0777);
    for (int i = 1; i <= 100; ++i) {
        std::ofstream file(dirName + "/file" + std::to_string(i) + ".txt", std::ios::binary);
        if (!file) {
            std::cerr << "Error creating file in " << dirName << "/file" << i << ".txt" << std::endl;
            exit(1);
        } else {
            const int fileSize = 10 * BYTE;
            char buffer[fileSize] = {}; 
            file.write(buffer, fileSize);

            std::cout << "File created successfully: " << dirName << "/file" << i << ".txt" << std::endl;
        }
        --totalFiles;
    }

    createDir(dirName + "/subdir" + std::to_string(j++), totalFiles);
}

int main() {

    createDir("dir1", 10000);
    createDir("dir2", 10000);

    return 0;
}
