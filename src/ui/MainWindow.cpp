#include "MainWindow.h"

MainWindow::MainWindow() : showFileBrowser(false), showProjectManager(false) {
}

MainWindow::~MainWindow() {
}

void MainWindow::render() {
    // 渲染菜单栏
    renderMenuBar();

    // 主内容区域
    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    float menuBarHeight = 20; // 菜单栏高度
    ImGui::SetNextWindowSize(ImVec2(displaySize.x, displaySize.y - menuBarHeight));
    ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight));
    ImGui::Begin("##MainContent", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    // 分割窗口
    static float splitterPos = 0.7f;
    
    // 左侧编辑器
    if (ImGui::BeginChild("#editor", ImVec2(ImGui::GetWindowWidth() * splitterPos, ImGui::GetWindowHeight() - 20), true)) {
        editor.render();
    }
    ImGui::EndChild();

    // 分割器
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    if (ImGui::Button("##splitter", ImVec2(4, ImGui::GetWindowHeight() - 20))) {
        // 分割器被点击
    }
    ImGui::PopStyleColor();
    if (ImGui::IsItemActive()) {
        splitterPos += ImGui::GetIO().MouseDelta.x / ImGui::GetWindowWidth();
        if (splitterPos < 0.1f) splitterPos = 0.1f;
        if (splitterPos > 0.9f) splitterPos = 0.9f;
    }

    // 右侧终端
    ImGui::SameLine();
    if (ImGui::BeginChild("#terminal", ImVec2(ImGui::GetWindowWidth() * (1 - splitterPos) - 4, ImGui::GetWindowHeight() - 20), true)) {
        terminal.render();
    }
    ImGui::EndChild();

    // 渲染状态栏
    renderStatusBar();

    ImGui::End();

    // 渲染文件浏览器
    if (showFileBrowser) {
        fileBrowser.render(&showFileBrowser);
    }

    // 渲染项目管理器
    if (showProjectManager) {
        projectManager.render(&showProjectManager);
    }
}

void MainWindow::renderMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New")) {
                editor.newFile();
            }
            if (ImGui::MenuItem("Open")) {
                showFileBrowser = true;
            }
            if (ImGui::MenuItem("Save")) {
                editor.saveFile();
            }
            if (ImGui::MenuItem("Save As")) {
                editor.saveFileAs();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                // 退出应用
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo")) {
                editor.undo();
            }
            if (ImGui::MenuItem("Redo")) {
                editor.redo();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Cut")) {
                editor.cut();
            }
            if (ImGui::MenuItem("Copy")) {
                editor.copy();
            }
            if (ImGui::MenuItem("Paste")) {
                editor.paste();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Run")) {
            if (ImGui::MenuItem("Run Script")) {
                std::string code = editor.getCode();
                std::string result = engine.executeScript(code);
                terminal.appendOutput(result);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("File Browser")) {
                showFileBrowser = true;
            }
            if (ImGui::MenuItem("Project Manager")) {
                showProjectManager = true;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void MainWindow::renderStatusBar() {
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 20);
    ImGui::BeginChild("#statusBar", ImVec2(ImGui::GetWindowWidth(), 20), true);
    ImGui::Text("Ready");
    ImGui::EndChild();
}