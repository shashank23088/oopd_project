#include "class_structures.h"

void Shell::run() {
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

bool Shell::containsHelpFlag(const vector<string>& args) {
    return find(args.begin(), args.end(), "-h") != args.end() ||
           find(args.begin(), args.end(), "--help") != args.end();
}

string Shell::getFileName(const string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos != string::npos) {
        return path.substr(pos + 1);
    }
    return path;
}

vector<string> Shell::parseCommand(const string& command) {
    vector<string> args;
    istringstream tokenStream(command);
    string token;

    while (tokenStream >> token) {
        args.push_back(token);
    }

    return args;
}

void Shell::showHelp(const string& command) {
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
        cout << "Usage: cp [-i] [-r] <source> <destination>\n";
        cout << "Options:\n";
        cout << "  -i                         prompt before overwriting files\n";
        cout << "  -r, --recursive            copy directories and their contents recursively\n";
    } else {
        cout << "No help available for command '" << command << "'.\n";
    }
}

void Shell::changeDirectory(const vector<string>& args) {
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

void Shell::listDirectoryContents(const vector<string>& args) {
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

void Shell::listDirectoryContentsRecursive(const string& path, bool showHidden, bool longFormat, bool recursive) {
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
            if (stat((path + "/" + entry->d_name).c_str(), &fileInfo) == 0) {
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

void Shell::listDirectoryContentsRecursiveParallel(const string& path, bool showHidden, bool longFormat, bool recursive) {
    DIR* dir = opendir(path.c_str());
    if (dir == nullptr) {
        perror("opendir");
        return;
    }

    vector<thread> threads;
    dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (!showHidden && entry->d_name[0] == '.') {
            continue; // Skip hidden files
        }

        threads.emplace_back(&Shell::processListDirectoryEntry, this, path, entry, showHidden, longFormat, recursive);
    }

    closedir(dir);

    for (auto& thread : threads) {
        thread.join();  
    }
}

void Shell::processListDirectoryEntry(const string& path, dirent* entry, bool showHidden, bool longFormat, bool recursive) {
    lock_guard<mutex> lock(mtx);

    cout << entry->d_name;

    // Additional information for the '-l' option
    if (longFormat) {
        struct stat fileInfo;
        if (stat((path + "/" + entry->d_name).c_str(), &fileInfo) == 0) {
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
        listDirectoryContentsRecursiveParallel(path + "/" + entry->d_name, showHidden, longFormat, recursive);
    }
}

bool Shell::isDirectory(const string& path) {
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0) {
        perror("stat");
        cerr << "Error: Unable to access " << path << endl;
        return false;
    }

    return S_ISDIR(fileInfo.st_mode);
}

bool Shell::fileExists(const std::string& filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

void Shell::moveFileInternal(const string& source, const string& destination, bool recursive) {
    if (fileExists(destination)) {
        cerr << "Error: Destination file or directory already exists: " << destination << endl;
        return;
    }

    if (rename(source.c_str(), destination.c_str()) != 0) {
        perror("rename");
        cerr << "Error: Unable to move file or directory to " << destination << endl;
    } else {
        cout << "Moved successfully\n";
    }

    if (recursive && isDirectory(source)) {
        moveDirectoryContentsRecursive(source, destination);
    }
}

void Shell::moveFile(const vector<string>& args) {
    if (args.size() < 3 || args[1] == "-h" || args[1] == "--help") {
        showHelp("mv");
        return;
    }

    const string& source = args[1];
    const string& destination = args[2];

    // Check if the source file or directory exists
    struct stat sourceInfo;
    if (stat(source.c_str(), &sourceInfo) != 0) {
        perror("stat");
        cerr << "Error: Unable to access source " << source << endl;
        return;
    }

    // Check if the destination is a directory
    struct stat destInfo;
    if (stat(destination.c_str(), &destInfo) == 0 && S_ISDIR(destInfo.st_mode)) {
        // If the destination is a directory, append the source file or directory's name to it
        const string destPath = destination + "/" + getFileName(source);
        if (rename(source.c_str(), destPath.c_str()) != 0) {
            perror("rename");
            cerr << "Error: Unable to move " << source << " to " << destPath << endl;
        } else {
            cout << "Moved successfully\n";
        }
    } else {
        // If the destination is not a directory, move the file or directory to the destination
        if (rename(source.c_str(), destination.c_str()) != 0) {
            perror("rename");
            cerr << "Error: Unable to move " << source << " to " << destination << endl;
        } else {
            cout << "Moved successfully\n";
        }
    }

    // If the recursive option is specified, and the source is a directory, move its contents recursively
    if (args.size() >= 4 && (args[3] == "-r" || args[3] == "--recursive") && isDirectory(source)) {
        moveDirectoryContentsRecursive(source, destination);
    }
}

void Shell::moveDirectoryContentsRecursive(const string& source, const string& destination) {
    // Create the destination directory if it doesn't exist
    if (mkdirRecursive(destination) != 0) {
        cerr << "Error creating directory " << destination << endl;
        return;
    }

    DIR* dir;
    struct dirent* ent;

    if ((dir = opendir(source.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (string(ent->d_name) != "." && string(ent->d_name) != "..") {
                const string fullSourcePath = source + "/" + string(ent->d_name);
                const string fullDestPath = destination + "/" + string(ent->d_name);

                if (isDirectory(fullSourcePath)) {
                    moveDirectoryContentsRecursive(fullSourcePath, fullDestPath);
                } else {
                    // Move files (non-recursively) within the source directory to the destination
                    moveFileInternal(fullSourcePath, fullDestPath, false);
                }
            }
        }
        closedir(dir);

        // Remove the source directory after its contents are moved
        rmdir(source.c_str());
    } else {
        cerr << "Error opening directory " << source << endl;
    }
}

void Shell::removeFile(const vector<string>& args) {
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

void Shell::removeDirectoryRecursive(const string& dir_path) {
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

void Shell::copyFile(const vector<string>& args) {
    // Check for help option
    if (args.size() == 2 && (args[1] == "-h" || args[1] == "--help")) {
        showHelp("cp");
        return;
    }

    bool recursive = false;
    bool interactive = false;

    // Skip flags when processing the command
    size_t sourceIndex = 1;
    size_t destIndex = 2;
    if (args.size() > 3) {
        if (args[1] == "-r" || args[1] == "--recursive") {
            recursive = true;
            sourceIndex = 2;
            destIndex = 3;
        } else if (args[1] == "-i") {
            interactive = true;
            sourceIndex = 2;
            destIndex = 3;
        }
    }

    if (args.size() <= sourceIndex || args.size() <= destIndex) {
        cerr << "cp: missing file operand\n";
        showHelp("cp");
        return;
    }

    const string& source = args[sourceIndex];
    const string& destination = args[destIndex];

    if (isDirectory(destination)) {
        // Append the source directory name to the destination path
        const string destPath = destination + "/" + getFileName(source);
        recursiveCopyDirectoryParallel(source, destPath);
    } else {
        copyFileInternal(source, destination, recursive, interactive);
    }
}

void Shell::copyFileInternal(const string& source, const string& destination, bool recursive, bool interactive) {
    if (fileExists(destination)) {
        if (interactive) {
            cout << "cp: overwrite '" << destination << "'? (y/n): ";
            char response;
            cin >> response;

            if (response != 'y') {
                cout << "cp: operation canceled for '" << destination << "'." << endl;
                return;
            }
        }

        remove(destination.c_str());
    }

    ifstream sourceFile(source, ios::binary);
    ofstream destFile(destination, ios::binary);

    destFile << sourceFile.rdbuf();

    sourceFile.close();
    destFile.close();

    cout << "File copied successfully\n";

    if (recursive && isDirectory(source)) {
        recursiveCopyDirectoryParallel(source, destination);
    }
}

void Shell::recursiveCopyDirectory(const string& source, const string& destination) {
    // Create the destination directory and intermediate directories if they don't exist
    if (mkdirRecursive(destination) != 0) {
        cerr << "Error creating directory " << destination << endl;
        return;
    }

    DIR* dir;
    struct dirent* ent;

    if ((dir = opendir(source.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (string(ent->d_name) != "." && string(ent->d_name) != "..") {
                const string fullSourcePath = source + "/" + string(ent->d_name);
                const string fullDestPath = destination + "/" + string(ent->d_name);

                if (isDirectory(fullSourcePath)) {
                    recursiveCopyDirectory(fullSourcePath, destination); // Fix here: Use destination instead of fullDestPath
                } else {
                    copyFileInternal(fullSourcePath, fullDestPath, true, false);
                }
            }
        }
        closedir(dir);
    } else {
        cerr << "Error opening directory " << source << endl;
    }
}

void Shell::recursiveCopyDirectoryParallel(const string& source, const string& destination) {
    // Create the destination directory and intermediate directories if they don't exist
    if (mkdirRecursive(destination) != 0) {
        cerr << "Error creating directory " << destination << endl;
        return;
    }

    DIR* dir;
    struct dirent* ent;

    if ((dir = opendir(source.c_str())) != NULL) {
        vector<thread> threads;

        while ((ent = readdir(dir)) != NULL) {
            if (string(ent->d_name) != "." && string(ent->d_name) != "..") {
                const string fullSourcePath = source + "/" + string(ent->d_name);
                const string fullDestPath = destination + "/" + string(ent->d_name);

                if (isDirectory(fullSourcePath)) {
                    threads.emplace_back(&Shell::processRecursiveCopyDirectoryEntry,
                                        this, fullSourcePath, destination); // New function to handle subdirectories
                } else {
                    copyFileInternal(fullSourcePath, fullDestPath, true, false);
                }
            }
        }

        closedir(dir);

        for (auto& thread : threads) {
            thread.join();
        }
    } else {
        cerr << "Error opening directory " << source << endl;
    }
}

void Shell::processRecursiveCopyDirectoryEntry(const string& source, const string& destination) {
    lock_guard<mutex> lock(mtx);

    const string fullDestPath = destination + "/" + getFileName(source);
    recursiveCopyDirectory(source, fullDestPath);
}

int Shell::mkdirRecursive(const string& path) {
    size_t pos = 0;
    while ((pos = path.find_first_of("/", pos)) != string::npos) {
        string subpath = path.substr(0, pos);
        if (mkdir(subpath.c_str(), 0777) != 0 && errno != EEXIST && !isDirectory(subpath)) {
            perror("mkdir");
            return -1;
        }
        pos++;
    }

    if (mkdir(path.c_str(), 0777) != 0 && errno != EEXIST && !isDirectory(path)) {
        perror("mkdir");
        return -1;
    }
    return 0;
}