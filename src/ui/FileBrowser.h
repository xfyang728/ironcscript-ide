#pragma once

#include <string>
#include <vector>

class FileBrowser {
public:
    FileBrowser();
    ~FileBrowser();

    void render(bool* isOpen);

private:
    std::string currentPath;
    std::vector<std::string> files;
    std::vector<std::string> directories;

    void loadDirectory(const std::string& path);
};