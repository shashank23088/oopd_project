#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fstream>

using namespace std;

class Shell {
public:
    void run() {
        while (true) {
            cout << "MyShell> ";
            string command;
            getline(cin, command);

            vector<string> args = parseCommand(command);

            if (args.empty()) {
                continue;
            }

            if (args[0] == "cd") {
                changeDirectory(args);
            } else if (args[0] == "ls") {
                if (containsHelpFlag(args)) {
                    showHelp("ls");
                } else {
                    listDirectoryContents(args);
                }
            } else if (args[0] == "mv") {
                if (containsHelpFlag(args)) {
                    showHelp("mv");
                } else {
                    moveFile(args);
                }
            } else if (args[0] == "rm") {
                if (containsHelpFlag(args)) {
                    showHelp("rm");
                } else {
                    removeFile(args);
                }
            } else if (args[0] == "cp") {
                if (containsHelpFlag(args)) {
                    showHelp("cp");
                } else {
                    copyFile(args);
                }
            } else {
                cout << "Command not recognized. Use '-h' or 'help' for command usage.\n";
            }
        }
    }

private:
    bool containsHelpFlag(const vector<string>& args) {
        return find(args.begin(), args.end(), "-h") != args.end() ||
               find(args.begin(), args.end(), "--help") != args.end();
    }

    string getFileName(const string& path) {
        size_t pos = path.find_last_of("/\\");
        if (pos != string::npos) {
            return path.substr(pos + 1);
        }
        return path;
    }

    vector<string> parseCommand(const string& command) {
        vector<string> args;
        istringstream tokenStream(command);
        string token;

        // Use space as a delimiter and treat multiple spaces as one
        while (tokenStream >> token) {
            args.push_back(token);
        }

        return args;
    }

    void showHelp(const string& command) {
        if (command == "cd") {
            cout << "cd - Change current working directory.\n";
            cout << "Usage: cd <directory>\n";
        } else if (command == "ls") {
            cout << "ls - List directory contents.\n";
            cout << "Usage: ls [-a] [-l] [-r]\n";
            cout << "Options:\n";
            cout << "  -a, --all                  do not ignore entries starting with .\n";
            cout << "  -l, --long                 show additional information\n";
            cout << "  -r, --recursive            list subdirectories recursively\n";
        } else if (command == "mv") {
            cout << "mv - Move or rename files or directories.\n";
            cout << "Usage: mv <source> <destination>\n";
        } else if (command == "rm") {
            cout << "rm - Remove files or directories.\n";
            cout << "Usage: rm [-r] [-i] [-f] <file(s)>\n";
            cout << "Options:\n";
            cout << "  -r, --recursive            remove directories and their contents recursively\n";
            cout << "  -i                         prompt before every removal\n";
            cout << "  -f                         force removal without confirmation\n";
        } else if (command == "cp") {
            cout << "cp - Copy files or directories.\n";
            cout << "Usage: cp [-i] <source> <destination>\n";
            cout << "Options:\n";
            cout << "  -i                         prompt before overwriting files\n";
        } else {
            cout << "No help available for command '" << command << "'.\n";
        }
    }

    void changeDirectory(const vector<string>& args) {
        if (args.size() > 1) {
            if (args[1] == "-h" || args[1] == "--help") {
                showHelp("cd");
            } else {
                if (chdir(args[1].c_str()) != 0) {
                    perror("cd");
                }
            }
        } else {
            showHelp("cd");
        }
    }

    void listDirectoryContents(const vector<string>& args) {
        const char* path = ".";
        bool showHidden = false;
        bool longFormat = false;
        bool recursive = false;

        // Handle options
        for (size_t i = 1; i < args.size(); ++i) {
            if (args[i] == "-a") {
                showHidden = true;
            } else if (args[i] == "-l") {
                longFormat = true;
            } else if (args[i] == "-r") {
                recursive = true;
            } else if (args[i] == "-h" || args[i] == "--help") {
                showHelp("ls");
                return;
            } else {
                cerr << "Unrecognized option: " << args[i] << endl;
                return;
            }
        }

        listDirectoryContentsRecursive(path, showHidden, longFormat, recursive);
    }

    void listDirectoryContentsRecursive(const string& path, bool showHidden, bool longFormat, bool recursive) {
        DIR* dir = opendir(path.c_str());
        if (dir == nullptr) {
            perror("opendir");
            return;
        }

        dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (!showHidden && entry->d_name[0] == '.') {
                continue; // Skip hidden files
            }

            cout << entry->d_name;

            // Additional information for the '-l' option
            if (longFormat) {
                struct stat fileInfo;
                if (stat(entry->d_name, &fileInfo) == 0) {
                    cout << " Size: " << fileInfo.st_size << " bytes";
                    // Add more information as needed
                } else {
                    cerr << " Error getting file information for " << entry->d_name << endl;
                }
            }

            cout << endl;

            // Recursive listing for directories
            if (recursive && isDirectory(path + "/" + entry->d_name)) {
                cout << "  Subdirectory: " << entry->d_name << "\n";
                listDirectoryContentsRecursive(path + "/" + entry->d_name, showHidden, longFormat, recursive);
            }
        }

        closedir(dir);
    }

    bool isDirectory(const string& path) {
        struct stat fileInfo;
        if (stat(path.c_str(), &fileInfo) != 0) {
            perror("stat");
            cerr << "Error: Unable to access " << path << endl;
            return false;
        }

        return S_ISDIR(fileInfo.st_mode);
    }

    bool fileExists(const std::string& filename) {
        struct stat buffer;
        return (stat(filename.c_str(), &buffer) == 0);
    }

    void moveFile(const vector<string>& args) {
        if (args.size() < 3 || args[1] == "-h" || args[1] == "--help") {
            showHelp("mv");
            return;
        }

        const string& source = args[1];
        const string& destination = args[2];

        // Check if the source file exists
        struct stat sourceInfo;
        if (stat(source.c_str(), &sourceInfo) != 0) {
            perror("stat");
            cerr << "Error: Unable to access source file " << source << endl;
            return;
        }

        // Check if the destination is a directory
        struct stat destInfo;
        if (stat(destination.c_str(), &destInfo) == 0 && S_ISDIR(destInfo.st_mode)) {
            // If the destination is a directory, append the source file's name to it
            const string destPath = destination + "/" + getFileName(source);
            if (rename(source.c_str(), destPath.c_str()) != 0) {
                perror("rename");
                cerr << "Error: Unable to move file to " << destPath << endl;
            } else {
                cout << "File moved successfully\n";
            }
        } else {
            // If the destination is not a directory, move the file to the destination
            if (rename(source.c_str(), destination.c_str()) != 0) {
                perror("rename");
                cerr << "Error: Unable to move file to " << destination << endl;
            } else {
                cout << "File moved successfully\n";
            }
        }
    }

    void removeFile(const vector<string>& args) {
        bool recursive = false;
        bool interactive = false;
        bool force = false;

        // Check for help option
        if (args.size() == 2 && (args[1] == "-h" || args[1] == "--help")) {
            showHelp("rm");
            return;
        }

        // Skip flags when processing the command
        vector<string> filesToDelete;
        for (size_t i = 1; i < args.size(); ++i) {
            if (args[i] == "-r" || args[i] == "--recursive") {
                recursive = true;
            } else if (args[i] == "-i") {
                interactive = true;
            } else if (args[i] == "-f") {
                force = true;
            } else {
                // Add more flags as needed
                filesToDelete.push_back(args[i]);
            }
        }

        for (const auto& file : filesToDelete) {
            if (!fileExists(file)) {
                cerr << "rm: '" << file << "' does not exist." << endl;
                continue;
            }

            if (interactive) {
                cout << "rm: remove '" << file << "'? (y/n): ";
                char response;
                cin >> response;

                if (response != 'y') {
                    cout << "rm: operation canceled for '" << file << "'." << endl;
                    continue;
                }
            }

            if (isDirectory(file) && !recursive) {
                cerr << "rm: cannot remove '" << file << "': Is a directory. Use -r to remove directories." << endl;
                continue;
            }

            if (force || !isDirectory(file)) {
                if (remove(file.c_str()) != 0) {
                    cerr << "rm: Error deleting file '" << file << "'." << endl;
                }
            } else {
                removeDirectoryRecursive(file);
            }
        }
    }

    void removeDirectoryRecursive(const string& dir_path) {
        DIR* dir;
        struct dirent* ent;
        if ((dir = opendir(dir_path.c_str())) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                if (string(ent->d_name) != "." && string(ent->d_name) != "..") {
                    string full_path = dir_path + "/" + string(ent->d_name);
                    if (isDirectory(full_path)) {
                        removeDirectoryRecursive(full_path);
                    } else {
                        remove(full_path.c_str());
                    }
                }
            }
            closedir(dir);
            rmdir(dir_path.c_str());
        } else {
            cerr << "Error opening directory" << endl;
        }
    }

    void copyFile(const vector<string>& args) {
        // Check for help option
        if (args.size() == 2 && (args[1] == "-h" || args[1] == "--help")) {
            showHelp("cp");
            return;
        }

        if (args.size() < 3) {
            cerr << "Usage: cp [-r] <source> <destination>\n";
            return;
        }

        bool recursive = false;
        size_t sourceIndex = 1;

        // Check for recursive option
        if (args.size() > 3 && args[1] == "-r") {
            recursive = true;
            sourceIndex = 2;
        }

        const string& source = args[sourceIndex];
        const string& destination = args[sourceIndex + 1];

        if (recursive) {
            recursiveCopyDirectory(source, destination);
        } else {
            // Check if the source file exists
            ifstream sourceFile(source, ios::binary);
            if (!sourceFile.is_open()) {
                cerr << "Error: Unable to open source file " << source << endl;
                return;
            }

            // Check if the destination is a directory
            struct stat destInfo;
            if (stat(destination.c_str(), &destInfo) == 0 && S_ISDIR(destInfo.st_mode)) {
                // If the destination is a directory, append the source file's name to it
                const string destPath = destination + "/" + getFileName(source);
                copyFileContents(sourceFile, destPath, args);
            } else {
                // If the destination is not a directory, copy the file to the destination
                copyFileContents(sourceFile, destination, args);
            }

            sourceFile.close();
        }
    }

    void copyFileContents(ifstream& sourceFile, const string& destination, const vector<string>& args) {
        // Check if the destination file exists
        ofstream destFile(destination, ios::binary);
        bool overwrite = false;

        if (destFile.is_open()) {
            destFile.close();

            // Check if the '-i' option is provided for interactive prompting
            for (size_t i = 3; i < args.size(); ++i) {
                if (args[i] == "-i") {
                    cout << "File " << destination << " already exists. Overwrite? (y/n): ";
                    char response;
                    cin >> response;
                    overwrite = (response == 'y' || response == 'Y');
                } else {
                    cerr << "Unrecognized option: " << args[i] << endl;
                    return;
                }
            }
        }

        if (overwrite || !destFile.is_open()) {
            // Reset ifstream position before copying
            sourceFile.clear();
            sourceFile.seekg(0, ios::beg);

            // Copy file contents
            destFile << sourceFile.rdbuf();
            destFile.close();

            cout << "File copied successfully\n";
        } else {
            cout << "File not overwritten.\n";
        }
    }

    void recursiveCopyDirectory(const string& source, const string& destination) {
        DIR* dir;
        struct dirent* ent;

        if ((dir = opendir(source.c_str())) != NULL) {
            if (mkdir(destination.c_str(), 0777) != 0 && errno != EEXIST) {
                cerr << "Error creating directory " << destination << endl;
                closedir(dir);
                return;
            }

            while ((ent = readdir(dir)) != NULL) {
                if (string(ent->d_name) != "." && string(ent->d_name) != "..") {
                    const string fullSourcePath = source + "/" + string(ent->d_name);
                    const string fullDestPath = destination + "/" + string(ent->d_name);

                    if (isDirectory(fullSourcePath)) {
                        recursiveCopyDirectory(fullSourcePath, fullDestPath);
                    } else {
                        copyFileContents(fullSourcePath, fullDestPath);
                    }
                }
            }

            closedir(dir);
        } else {
            cerr << "Error opening directory " << source << endl;
        }
    }

    void copyFileContents(const string& source, const string& destination) {
        ifstream sourceFile(source, ios::binary);
        ofstream destFile(destination, ios::binary);

        if (!sourceFile.is_open()) {
            cerr << "Error: Unable to open source file " << source << endl;
            return;
        }

        if (!destFile.is_open()) {
            cerr << "Error: Unable to open destination file " << destination << endl;
            sourceFile.close();
            return;
        }

        // Reset ifstream position before copying
        sourceFile.clear();
        sourceFile.seekg(0, ios::beg);

        // Copy file contents
        destFile << sourceFile.rdbuf();
        destFile.close();
        sourceFile.close();

        cout << "File copied successfully\n";
    }
};

int main() {
    Shell myShell;
    myShell.run();

    return 0;
}
