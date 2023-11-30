#include "headers.h"

class Shell {
public:
    void run();
private:
    bool containsHelpFlag(const vector<string>& args);
    string getFileName(const string& path);
    vector<string> parseCommand(const string& command);
    void showHelp(const string& command);
    void changeDirectory(const vector<string>& args);
    void listDirectoryContents(const vector<string>& args) ;
    void listDirectoryContentsRecursive(const string& path, bool showHidden, bool longFormat, bool recursive);
    bool isDirectory(const string& path);
    bool fileExists(const std::string& filename) ;
    void moveFile(const vector<string>& args) ;
    void removeFile(const vector<string>& args) ;
    void removeDirectoryRecursive(const string& dir_path);
    void copyFile(const vector<string>& args);
    void copyFileContents(ifstream& sourceFile, const string& destination, const vector<string>& args) ;
    void recursiveCopyDirectory(const string& source, const string& destination);
    void copyFileContents(const string& source, const string& destination) ;
    void listDirectoryContentsWildcard(const string& path, bool showHidden, bool longFormat, bool recursive, const string& wildcardPattern);
    
    };
