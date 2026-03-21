#include "Terminal.h"
#include "imgui.h"
#include <sstream>

Terminal::Terminal() {
    // 初始化输出行
    outputLines.push_back("CScriptEngine Terminal");
    outputLines.push_back("Type 'help' for available commands");
}

Terminal::~Terminal() {
}

void Terminal::render() {
    ImGui::Text("Terminal");
    ImGui::Separator();
    
    // 显示输出
    ImGui::BeginChild("##terminalOutput", ImVec2(-1, ImGui::GetWindowHeight() - 50), true);
    for (const auto& line : outputLines) {
        ImGui::TextUnformatted(line.c_str());
    }
    // 自动滚动到底部
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }
    ImGui::EndChild();
    
    // 输入框
    ImGui::SetNextItemWidth(-1);
    if (ImGui::InputText("##terminalInput", &inputBuffer[0], inputBuffer.capacity(), 
        ImGuiInputTextFlags_EnterReturnsTrue)) {
        processInput();
    }
}

void Terminal::appendOutput(const std::string& output) {
    // 将输出分割成多行
    std::stringstream ss(output);
    std::string line;
    while (std::getline(ss, line)) {
        outputLines.push_back(line);
    }
    
    // 限制输出行数，防止内存占用过大
    if (outputLines.size() > 1000) {
        outputLines.erase(outputLines.begin(), outputLines.begin() + (outputLines.size() - 1000));
    }
}

void Terminal::processInput() {
    if (!inputBuffer.empty()) {
        // 添加输入到输出
        outputLines.push_back("> " + inputBuffer);
        
        // 处理命令
        if (inputBuffer == "help") {
            outputLines.push_back("Available commands:");
            outputLines.push_back("- help: Show this help");
            outputLines.push_back("- clear: Clear terminal");
        } else if (inputBuffer == "clear") {
            outputLines.clear();
            outputLines.push_back("CScriptEngine Terminal");
            outputLines.push_back("Type 'help' for available commands");
        } else {
            outputLines.push_back("Command not found: " + inputBuffer);
        }
        
        // 清空输入缓冲区
        inputBuffer = "";
    }
}