#include "headers.h"

class Shell {
private:
    mutex mtx;
public:
    void run();
    bool containsHelpFlag(const vector<string>& args);
    string getFileName(const string& path);
    vector<string> parseCommand(const string& command);
    void showHelp(const string& command);
    int mkdirRecursive(const string& path);
    void changeDirectory(const vector<string>& args);
    void listDirectoryContents(const vector<string>& args);
    void listDirectoryContentsRecursive(const string& path, bool showHidden, bool longFormat, bool recursive);
    bool isDirectory(const string& path);
    bool fileExists(const std::string& filename);
    void moveFile(const vector<string>& args);
    void moveDirectoryContentsRecursive(const string& source, const string& destination);
    void removeFile(const vector<string>& args);
    void removeDirectoryRecursive(const string& dir_path);
    void copyFile(const vector<string>& args);
    void copyFileContents(ifstream& sourceFile, const string& destination, const vector<string>& args);
    void copyFileContents(const string& source, const string& destination);
    void processRecursiveCopyDirectoryEntry(const string& source, const string& destination);
    void recursiveCopyDirectoryParallel(const string& source, const string& destination);
    void recursiveCopyDirectory(const string& source, const string& destination);
    void copyFileInternal(const string& source, const string& destination, bool recursive, bool interactive);
    void moveFileInternal(const string& source, const string& destination, bool recursive);
    void processListDirectoryEntry(const string& path, dirent* entry, bool showHidden, bool longFormat, bool recursive);
    void listDirectoryContentsRecursiveParallel(const string& path, bool showHidden, bool longFormat, bool recursive);
    void listDirectoryContentsWildcard(const string& path, bool showHidden, bool longFormat, bool recursive, const string& wildcardPattern);
};
