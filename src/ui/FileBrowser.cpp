#include "FileBrowser.h"
#include "imgui.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

FileBrowser::FileBrowser() {
    // 初始化当前路径为当前工作目录
    currentPath = fs::current_path().string();
    loadDirectory(currentPath);
}

FileBrowser::~FileBrowser() {
}

void FileBrowser::render(bool* isOpen) {
    if (ImGui::Begin("File Browser", isOpen, ImGuiWindowFlags_Modal)) {
        // 路径导航栏
        ImGui::Text("Current Path: %s", currentPath.c_str());
        ImGui::Separator();
        
        // 目录列表
        if (ImGui::BeginListBox("##directories", ImVec2(-1, 200))) {
            // 上一级目录
            if (ImGui::Selectable("..")) {
                fs::path path = fs::path(currentPath).parent_path();
                if (!path.empty()) {
                    currentPath = path.string();
                    loadDirectory(currentPath);
                }
            }
            
            // 显示子目录
            for (const auto& dir : directories) {
                if (ImGui::Selectable(dir.c_str())) {
                    currentPath = (fs::path(currentPath) / dir).string();
                    loadDirectory(currentPath);
                }
            }
            ImGui::EndListBox();
        }
        
        ImGui::Separator();
        
        // 文件列表
        if (ImGui::BeginListBox("##files", ImVec2(-1, 200))) {
            for (const auto& file : files) {
                if (ImGui::Selectable(file.c_str())) {
                    // 这里可以处理文件选择
                    std::string filePath = (fs::path(currentPath) / file).string();
                    std::cout << "Selected file: " << filePath << std::endl;
                }
            }
            ImGui::EndListBox();
        }
        
        ImGui::End();
    }
}

void FileBrowser::loadDirectory(const std::string& path) {
    files.clear();
    directories.clear();
    
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_directory()) {
                directories.push_back(entry.path().filename().string());
            } else if (entry.is_regular_file()) {
                files.push_back(entry.path().filename().string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error loading directory: " << e.what() << std::endl;
    }
}