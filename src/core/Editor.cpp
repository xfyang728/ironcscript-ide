#include "Editor.h"
#include "imgui.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>

Editor::Editor() : 
    isModified(false), 
    cursorPos(0), 
    selectionStart(0), 
    selectionEnd(0), 
    currentLine(0), 
    currentColumn(0), 
    lineNumbers(true), 
    syntaxHighlighting(true), 
    autoIndent(true), 
    codeFolding(false), 
    showSearchReplace(false), 
    tabSize(4), 
    theme("dark"), 
    showCompletions(false), 
    completionIndex(0), 
    caseSensitiveSearch(false), 
    wholeWordSearch(false), 
    undoIndex(0) {
    // 初始化默认代码
    code = "int main() {\n    int a = 10;\n    int b = 20;\n    int c = a + b;\n    printf(\"Sum: %d\\n\", c);\n    return 0;\n}";
    
    // 初始化主题
    initThemes();
    currentTheme = themes[theme];
    
    // 初始化代码行
    updateLines();
    
    // 检查语法错误
    checkSyntaxErrors();
}

Editor::~Editor() {
}

void Editor::initThemes() {
    // 深色主题
    Theme darkTheme;
    darkTheme.background = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    darkTheme.text = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    darkTheme.keyword = ImVec4(0.4f, 0.7f, 1.0f, 1.0f);
    darkTheme.string = ImVec4(0.7f, 0.5f, 0.3f, 1.0f);
    darkTheme.comment = ImVec4(0.3f, 0.6f, 0.3f, 1.0f);
    darkTheme.number = ImVec4(0.7f, 0.7f, 0.3f, 1.0f);
    darkTheme.op = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    darkTheme.error = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
    themes["dark"] = darkTheme;
    
    // 浅色主题
    Theme lightTheme;
    lightTheme.background = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    lightTheme.text = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    lightTheme.keyword = ImVec4(0.0f, 0.0f, 0.7f, 1.0f);
    lightTheme.string = ImVec4(0.7f, 0.3f, 0.0f, 1.0f);
    lightTheme.comment = ImVec4(0.0f, 0.6f, 0.0f, 1.0f);
    lightTheme.number = ImVec4(0.7f, 0.3f, 0.7f, 1.0f);
    lightTheme.op = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    lightTheme.error = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    themes["light"] = lightTheme;
}

void Editor::updateLines() {
    lines.clear();
    size_t pos = 0;
    while (pos < code.size()) {
        size_t end = code.find('\n', pos);
        if (end == std::string::npos) {
            lines.push_back(code.substr(pos));
            break;
        }
        lines.push_back(code.substr(pos, end - pos));
        pos = end + 1;
    }
    
    // 更新行高亮
    if (syntaxHighlighting) {
        highlightSyntax();
    }
}

void Editor::render() {
    ImVec2 size = ImGui::GetContentRegionAvail();
    
    // 渲染工具栏
    renderToolbar();
    
    // 渲染搜索替换面板
    if (showSearchReplace) {
        renderSearchReplace();
    }
    
    // 计算行号区域宽度
    float lineNumberWidth = 40.0f;
    if (lineNumbers) {
        int maxLineNumber = lines.size();
        lineNumberWidth = ImGui::CalcTextSize(std::to_string(maxLineNumber).c_str()).x + 10.0f;
    }
    
    // 渲染代码编辑区域
    ImGui::BeginChild("##editor", ImVec2(-1, -30), true, ImGuiWindowFlags_HorizontalScrollbar);
    
    // 渲染行号和代码
    if (lineNumbers) {
        // 创建行号区域
        ImGui::PushStyleColor(ImGuiCol_Button, currentTheme.background);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
        
        // 行号区域
        ImGui::BeginChild("##lineNumbers", ImVec2(lineNumberWidth, -1), false);
        for (size_t i = 0; i < lines.size(); i++) {
            // 检查是否有错误
            bool hasError = false;
            for (const auto& error : syntaxErrors) {
                if (error.first == static_cast<int>(i + 1)) {
                    hasError = true;
                    break;
                }
            }
            
            if (hasError) {
                ImGui::PushStyleColor(ImGuiCol_Text, currentTheme.error);
            }
            
            ImGui::Text("%d", static_cast<int>(i + 1));
            
            if (hasError) {
                ImGui::PopStyleColor();
            }
        }
        ImGui::EndChild();
        
        ImGui::PopStyleColor(2);
        
        ImGui::SameLine();
    }
    
    // 渲染代码编辑区域
    
    // 使用 ImGui 的 InputTextMultiline 作为基础，添加自定义功能
    static char buffer[65536] = {0}; // 更大的缓冲区
    // 复制代码到缓冲区
    strncpy(buffer, code.c_str(), sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    if (ImGui::InputTextMultiline("##codeEditor", buffer, sizeof(buffer), 
        ImVec2(-1, -1), 
        ImGuiInputTextFlags_AllowTabInput | 
        ImGuiInputTextFlags_NoHorizontalScroll | 
        ImGuiInputTextFlags_CallbackCharFilter | 
        ImGuiInputTextFlags_CallbackEdit, 
        &Editor::textEditCallback, this)) {
        // 更新代码
        std::string newCode(buffer);
        if (newCode != code) {
            // 保存到撤销栈
            undoStack.push_back(code);
            if (undoStack.size() > 50) {
                undoStack.erase(undoStack.begin());
            }
            undoIndex = undoStack.size();
            redoStack.clear();
            
            code = newCode;
            isModified = true;
            updateLines();
            
            // 检查语法错误
            checkSyntaxErrors();
        }
    }
    ImGui::PopStyleVar();
    
    // 渲染代码补全
    if (showCompletions && !completions.empty()) {
        renderCodeCompletion();
    }
    
    ImGui::EndChild();
    
    // 渲染状态栏
    renderStatusBar();
}

void Editor::renderToolbar() {
    ImGui::BeginChild("##toolbar", ImVec2(-1, 30), false, 0);
    
    if (ImGui::Button("New")) {
        newFile();
    }
    ImGui::SameLine();
    if (ImGui::Button("Open")) {
        // 这里可以实现文件打开对话框
    }
    ImGui::SameLine();
    if (ImGui::Button("Save")) {
        saveFile();
    }
    ImGui::SameLine();
    if (ImGui::Button("Save As")) {
        saveFileAs();
    }
    
    ImGui::SameLine();
    ImGui::Text("|");
    
    ImGui::SameLine();
    if (ImGui::Button("Undo")) {
        undo();
    }
    ImGui::SameLine();
    if (ImGui::Button("Redo")) {
        redo();
    }
    
    ImGui::SameLine();
    ImGui::Text("|");
    
    ImGui::SameLine();
    if (ImGui::Button("Cut")) {
        cut();
    }
    ImGui::SameLine();
    if (ImGui::Button("Copy")) {
        copy();
    }
    ImGui::SameLine();
    if (ImGui::Button("Paste")) {
        paste();
    }
    
    ImGui::SameLine();
    ImGui::Text("|");
    
    ImGui::SameLine();
    if (ImGui::Button("Search")) {
        showSearchReplace = !showSearchReplace;
    }
    ImGui::SameLine();
    if (ImGui::Button("Format")) {
        formatCode();
    }
    
    ImGui::SameLine();
    ImGui::Text("|");
    
    ImGui::SameLine();
    ImGui::Checkbox("Line Numbers", &lineNumbers);
    ImGui::SameLine();
    ImGui::Checkbox("Syntax", &syntaxHighlighting);
    ImGui::SameLine();
    ImGui::Checkbox("Auto Indent", &autoIndent);
    ImGui::SameLine();
    ImGui::Checkbox("Folding", &codeFolding);
    
    // 主题选择
    ImGui::SameLine();
    if (ImGui::BeginCombo("Theme", theme.c_str())) {
        for (const auto& pair : themes) {
            if (ImGui::Selectable(pair.first.c_str())) {
                theme = pair.first;
                currentTheme = pair.second;
            }
        }
        ImGui::EndCombo();
    }
    
    ImGui::EndChild();
}

void Editor::renderStatusBar() {
    ImGui::BeginChild("##statusBar", ImVec2(-1, 20), false, 0);
    
    // 显示当前行和列
    ImGui::Text("Line %d, Column %d", currentLine + 1, currentColumn + 1);
    
    // 显示文件状态
    ImGui::SameLine(150);
    if (isModified) {
        ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.0f, 1.0f), "Modified");
    } else {
        ImGui::Text("Saved");
    }
    
    // 显示错误数量
    ImGui::SameLine(250);
    if (!syntaxErrors.empty()) {
        ImGui::TextColored(currentTheme.error, "Errors: %d", syntaxErrors.size());
    } else {
        ImGui::Text("No errors");
    }
    
    // 显示文件路径
    ImGui::SameLine(350);
    if (!currentFilePath.empty()) {
        ImGui::Text(currentFilePath.c_str());
    } else {
        ImGui::Text("Untitled");
    }
    
    ImGui::EndChild();
}

void Editor::renderSearchReplace() {
    ImGui::BeginChild("##searchReplace", ImVec2(-1, 80), false, 0);
    
    ImGui::Text("Search:");
    ImGui::SameLine();
    static char searchBuf[256] = {0};
    if (ImGui::InputText("##search", searchBuf, sizeof(searchBuf))) {
        searchText = searchBuf;
    }
    
    ImGui::Text("Replace:");
    ImGui::SameLine();
    static char replaceBuf[256] = {0};
    if (ImGui::InputText("##replace", replaceBuf, sizeof(replaceBuf))) {
        replaceText = replaceBuf;
    }
    
    ImGui::Checkbox("Case Sensitive", &caseSensitiveSearch);
    ImGui::SameLine();
    ImGui::Checkbox("Whole Word", &wholeWordSearch);
    
    if (ImGui::Button("Find")) {
        // 实现查找功能
    }
    ImGui::SameLine();
    if (ImGui::Button("Replace")) {
        // 实现替换功能
    }
    ImGui::SameLine();
    if (ImGui::Button("Replace All")) {
        // 实现全部替换功能
    }
    
    ImGui::EndChild();
}

void Editor::renderCodeCompletion() {
    ImVec2 pos = ImGui::GetCursorScreenPos();
    pos.y += 20;
    
    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(ImVec2(300, 200));
    
    if (ImGui::Begin("##completion", nullptr, 
        ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_Popup)) {
        
        for (size_t i = 0; i < completions.size(); i++) {
            bool isSelected = (i == completionIndex);
            if (ImGui::Selectable(completions[i].c_str(), isSelected)) {
                // 插入选中的补全项
                showCompletions = false;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        
        ImGui::End();
    }
}

int Editor::textEditCallback(ImGuiInputTextCallbackData* data) {
    Editor* editor = static_cast<Editor*>(data->UserData);
    
    // 更新光标位置
    editor->cursorPos = data->CursorPos;
    editor->selectionStart = data->SelectionStart;
    editor->selectionEnd = data->SelectionEnd;
    
    // 计算当前行和列
    int line = 0;
    int column = 0;
    for (int i = 0; i < data->CursorPos; i++) {
        if (data->Buf[i] == '\n') {
            line++;
            column = 0;
        } else {
            column++;
        }
    }
    editor->currentLine = line;
    editor->currentColumn = column;
    
    // 处理自动缩进
    if (editor->autoIndent && data->EventFlag == ImGuiInputTextFlags_CallbackEdit) {
        // 检查是否按下了回车键
        if (data->EventKey == ImGuiKey_Enter) {
            // 找到当前行的缩进
            int pos = data->CursorPos - 1;
            while (pos >= 0 && data->Buf[pos] != '\n') {
                pos--;
            }
            
            // 计算缩进
            int indent = 0;
            pos++;
            while (pos < data->CursorPos && (data->Buf[pos] == ' ' || data->Buf[pos] == '\t')) {
                indent++;
                pos++;
            }
            
            // 检查当前行是否以 { 结尾
            pos = data->CursorPos - 2;
            while (pos >= 0 && (data->Buf[pos] == ' ' || data->Buf[pos] == '\t')) {
                pos--;
            }
            if (pos >= 0 && data->Buf[pos] == '{') {
                indent += editor->tabSize;
            }
            
            // 插入缩进
            std::string indentStr(indent, ' ');
            data->InsertChars(data->CursorPos, indentStr.c_str());
            data->CursorPos += indent;
            data->SelectionStart = data->CursorPos;
            data->SelectionEnd = data->CursorPos;
        }
    }
    
    // 处理代码补全
    if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion) {
        editor->generateCompletions();
        editor->showCompletions = true;
        editor->completionIndex = 0;
    }
    
    return 0;
}

void Editor::highlightSyntax() {
    lineHighlights.clear();
    
    // 关键字列表
    std::vector<std::string> keywords = {
        "int", "float", "double", "char", "void", "bool",
        "if", "else", "for", "while", "do", "switch", "case", "default",
        "return", "break", "continue", "struct", "class", "namespace",
        "using", "typedef", "const", "static", "extern", "inline"
    };
    
    for (const auto& line : lines) {
        std::vector<std::pair<int, int>> highlights;
        size_t endStringPos = std::string::npos;
        
        // 检查注释
        size_t commentPos = line.find("//");
        if (commentPos != std::string::npos) {
            highlights.push_back({static_cast<int>(commentPos), 0}); // 0 = comment
        }
        
        // 检查字符串
        size_t stringPos = line.find('"');
        if (stringPos != std::string::npos && (commentPos == std::string::npos || stringPos < commentPos)) {
            endStringPos = line.find('"', stringPos + 1);
            if (endStringPos != std::string::npos) {
                highlights.push_back({static_cast<int>(stringPos), 1}); // 1 = string
            }
        }
        
        // 检查数字
        std::regex numberRegex("\\b\\d+\\.?\\d*\\b");
        std::smatch match;
        std::string lineCopy = line;
        while (std::regex_search(lineCopy, match, numberRegex)) {
            int pos = static_cast<int>(match.position());
            if ((commentPos == std::string::npos || pos < commentPos) &&
                (stringPos == std::string::npos || pos > endStringPos)) {
                highlights.push_back({pos, 2}); // 2 = number
            }
            lineCopy = match.suffix().str();
        }
        
        // 检查关键字
        for (const auto& keyword : keywords) {
            size_t pos = 0;
            while ((pos = line.find(keyword, pos)) != std::string::npos) {
                // 确保是完整单词
                if ((pos == 0 || !isalnum(line[pos - 1])) &&
                    (pos + keyword.size() >= line.size() || !isalnum(line[pos + keyword.size()])) &&
                    (commentPos == std::string::npos || pos < commentPos) &&
                    (stringPos == std::string::npos || (pos > endStringPos || pos < stringPos))) {
                    highlights.push_back({static_cast<int>(pos), 3}); // 3 = keyword
                }
                pos += keyword.size();
            }
        }
        
        lineHighlights.push_back(highlights);
    }
}

void Editor::generateCompletions() {
    completions.clear();
    
    // 基础补全项
    std::vector<std::string> basicCompletions = {
        "int", "float", "double", "char", "void", "bool",
        "if", "else", "for", "while", "do", "switch", "case", "default",
        "return", "break", "continue", "struct", "class", "namespace",
        "using", "typedef", "const", "static", "extern", "inline",
        "printf", "scanf", "malloc", "free", "memcpy", "memset"
    };
    
    // 根据当前输入过滤补全项
    std::string currentWord;
    int pos = cursorPos - 1;
    while (pos >= 0 && (isalnum(code[pos]) || code[pos] == '_')) {
        currentWord = code[pos] + currentWord;
        pos--;
    }
    
    for (const auto& comp : basicCompletions) {
        if (comp.substr(0, currentWord.size()) == currentWord) {
            completions.push_back(comp);
        }
    }
}

void Editor::checkSyntaxErrors() {
    syntaxErrors.clear();
    
    // 简单的语法检查
    int braceCount = 0;
    for (size_t i = 0; i < lines.size(); i++) {
        const std::string& line = lines[i];
        
        // 检查大括号匹配
        for (char c : line) {
            if (c == '{') {
                braceCount++;
            } else if (c == '}') {
                braceCount--;
            }
        }
        
        // 检查分号
        if (line.find(';') == std::string::npos && 
            line.find('{') == std::string::npos && 
            line.find('}') == std::string::npos && 
            !line.empty() && 
            line.find("//") != 0) {
            // 可能缺少分号
            syntaxErrors.push_back({static_cast<int>(i + 1), "Possible missing semicolon"});
        }
    }
    
    // 检查大括号是否匹配
    if (braceCount != 0) {
        syntaxErrors.push_back({static_cast<int>(lines.size()), "Unmatched braces"});
    }
}

void Editor::formatCode() {
    // 简单的代码格式化
    std::string formattedCode;
    int indentLevel = 0;
    
    for (const auto& line : lines) {
        // 去除行首空白
        std::string trimmedLine = line;
        trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t"));
        
        // 检查是否需要减少缩进
        if (!trimmedLine.empty() && trimmedLine[0] == '}') {
            indentLevel = std::max(0, indentLevel - 1);
        }
        
        // 添加缩进
        std::string indent(indentLevel * tabSize, ' ');
        formattedCode += indent + trimmedLine + "\n";
        
        // 检查是否需要增加缩进
        if (!trimmedLine.empty() && trimmedLine.back() == '{') {
            indentLevel++;
        }
    }
    
    code = formattedCode;
    updateLines();
    isModified = true;
}

void Editor::newFile() {
    // 保存到撤销栈
    if (!code.empty()) {
        undoStack.push_back(code);
        if (undoStack.size() > 50) {
            undoStack.erase(undoStack.begin());
        }
        undoIndex = undoStack.size();
    }
    
    code = "";
    currentFilePath = "";
    isModified = false;
    updateLines();
    syntaxErrors.clear();
}

void Editor::openFile(const std::string& path) {
    std::ifstream file(path);
    if (file.is_open()) {
        std::string line;
        code = "";
        while (std::getline(file, line)) {
            code += line + "\n";
        }
        file.close();
        currentFilePath = path;
        isModified = false;
        updateLines();
        checkSyntaxErrors();
    } else {
        std::cerr << "Failed to open file: " << path << std::endl;
    }
}

void Editor::saveFile() {
    if (!currentFilePath.empty()) {
        std::ofstream file(currentFilePath);
        if (file.is_open()) {
            file << code;
            file.close();
            isModified = false;
        } else {
            std::cerr << "Failed to save file: " << currentFilePath << std::endl;
        }
    } else {
        saveFileAs();
    }
}

void Editor::saveFileAs() {
    // 这里可以实现文件保存对话框
    // 暂时使用硬编码的路径
    currentFilePath = "script.c";
    saveFile();
}

void Editor::undo() {
    if (undoIndex > 0) {
        undoIndex--;
        std::string prevCode = undoStack[undoIndex];
        redoStack.push_back(code);
        code = prevCode;
        updateLines();
        checkSyntaxErrors();
        isModified = true;
    }
}

void Editor::redo() {
    if (!redoStack.empty()) {
        std::string nextCode = redoStack.back();
        undoStack.push_back(code);
        if (undoStack.size() > 50) {
            undoStack.erase(undoStack.begin());
        }
        undoIndex = undoStack.size();
        code = nextCode;
        redoStack.pop_back();
        updateLines();
        checkSyntaxErrors();
        isModified = true;
    }
}

void Editor::cut() {
    // 实现剪切功能
}

void Editor::copy() {
    // 实现复制功能
}

void Editor::paste() {
    // 实现粘贴功能
}

std::string Editor::getCode() const {
    return code;
}