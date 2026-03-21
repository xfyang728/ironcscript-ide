#pragma once

#include <string>
#include <vector>

class ProjectManager {
public:
    ProjectManager();
    ~ProjectManager();

    void render(bool* isOpen);

private:
    std::string projectName;
    std::vector<std::string> projectFiles;
    std::vector<std::string> recentProjects;

    void loadRecentProjects();
    void saveRecentProjects();
};