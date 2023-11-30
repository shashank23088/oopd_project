#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sys/stat.h>

const long long int GIGABYTE = 1024 * 1024 * 1024;

void createDir(const std::string& dirName) {
    mkdir(dirName.c_str(), 0777);

    for (int i = 1; i <= 100; ++i) {
        std::ofstream file(dirName + "/file" + std::to_string(i) + ".txt", std::ios::binary);
        if (!file) {
            std::cerr << "Error creating file in " << dirName << "/file" << i << ".txt" << std::endl;
            exit(1);
        } else {
            const int fileSize = 1 * GIGABYTE;
            char buffer[fileSize] = {}; 
            file.write(buffer, fileSize);

            std::cout << "File created successfully: " << dirName << "/file" << i << ".txt" << std::endl;
        }
    }
}

int main() {
    createDir("dir1");
    createDir("dir2");

    return 0;
}
