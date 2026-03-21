#include "ProjectManager.h"
#include "imgui.h"
#include <iostream>

ProjectManager::ProjectManager() {
    projectName = "Untitled Project";
    loadRecentProjects();
    
    // 添加一些示例文件
    projectFiles.push_back("main.c");
    projectFiles.push_back("utils.c");
    projectFiles.push_back("header.h");
}

ProjectManager::~ProjectManager() {
    saveRecentProjects();
}

void ProjectManager::render(bool* isOpen) {
    if (ImGui::Begin("Project Manager", isOpen, ImGuiWindowFlags_Modal)) {
        // 项目名称
        ImGui::Text("Project: %s", projectName.c_str());
        ImGui::Separator();
        
        // 项目文件列表
        if (ImGui::BeginListBox("##projectFiles", ImVec2(-1, 200))) {
            for (const auto& file : projectFiles) {
                if (ImGui::Selectable(file.c_str())) {
                    // 这里可以处理文件选择
                    std::cout << "Selected file: " << file << std::endl;
                }
            }
            ImGui::EndListBox();
        }
        
        ImGui::Separator();
        
        // 最近项目
        ImGui::Text("Recent Projects");
        if (ImGui::BeginListBox("##recentProjects", ImVec2(-1, 150))) {
            for (const auto& project : recentProjects) {
                if (ImGui::Selectable(project.c_str())) {
                    // 这里可以处理项目选择
                    std::cout << "Selected project: " << project << std::endl;
                }
            }
            ImGui::EndListBox();
        }
        
        // 按钮
        if (ImGui::Button("New Project")) {
            // 这里可以实现新建项目功能
            std::cout << "New project" << std::endl;
        }
        ImGui::SameLine();
        if (ImGui::Button("Open Project")) {
            // 这里可以实现打开项目功能
            std::cout << "Open project" << std::endl;
        }
        ImGui::SameLine();
        if (ImGui::Button("Save Project")) {
            // 这里可以实现保存项目功能
            std::cout << "Save project" << std::endl;
        }
        
        ImGui::End();
    }
}

void ProjectManager::loadRecentProjects() {
    // 这里可以从配置文件加载最近项目
    // 暂时添加一些示例项目
    recentProjects.push_back("Project 1");
    recentProjects.push_back("Project 2");
    recentProjects.push_back("Project 3");
}

void ProjectManager::saveRecentProjects() {
    // 这里可以将最近项目保存到配置文件
}
