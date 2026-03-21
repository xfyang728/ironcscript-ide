#include "Editor.h"
#include "imgui.h"
#include <fstream>
#include <iostream>

#ifdef _WIN32
// 声明 Scintilla 窗口类
int Scintilla_RegisterClasses(void *hInstance);

// 加载 Scintilla 库
void LoadScintillaLibrary() {
    static bool loaded = false;
    if (!loaded) {
        // 加载 Scintilla 库
        LoadLibrary(TEXT("SciLexer.dll"));
        loaded = true;
    }
}

// Scintilla 回调函数
LRESULT CALLBACK ScintillaCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // 处理 Scintilla 事件
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
#endif

Editor::Editor() : isModified(false)
#ifdef _WIN32
    , hScintilla(nullptr), isScintillaInitialized(false)
#endif
{
    // 初始化默认代码
    code = "int main() {\n    int a = 10;\n    int b = 20;\n    int c = a + b;\n    printf(\"Sum: %d\\n\", c);\n    return 0;\n}";
}

Editor::~Editor() {
#ifdef _WIN32
    if (hScintilla) {
        DestroyWindow(hScintilla);
    }
#endif
}

#ifdef _WIN32
void Editor::initializeScintilla() {
    if (!isScintillaInitialized) {
        // 加载 Scintilla 库
        LoadScintillaLibrary();
        
        // 注册 Scintilla 窗口类
        HINSTANCE hInstance = GetModuleHandle(NULL);
        Scintilla_RegisterClasses(hInstance);
        
        // 获取 ImGui 的父窗口
        HWND hParent = GetActiveWindow();
        if (hParent) {
            // 创建 Scintilla 窗口
            hScintilla = CreateWindow(
                "Scintilla",
                "",
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                0, 0, 100, 100,
                hParent,
                NULL,
                hInstance,
                NULL
            );
            
            if (hScintilla) {
                // 配置 Scintilla
                configureScintilla();
                // 更新内容
                updateScintillaContent();
                isScintillaInitialized = true;
            }
        }
    }
}

void Editor::configureScintilla() {
    if (hScintilla) {
        // 设置字体
        SendMessage(hScintilla, SCI_STYLESETFONT, STYLE_DEFAULT, (LPARAM)"Consolas");
        SendMessage(hScintilla, SCI_SETZOOM, 0, 10);
        
        // 启用语法高亮
        SendMessage(hScintilla, SCI_SETILEXER, SCLEX_CPP, 0);
        
        // 设置代码折叠
        SendMessage(hScintilla, SCI_SETMARGINTYPEN, 2, SC_MARGIN_SYMBOL);
        SendMessage(hScintilla, SCI_SETMARGINWIDTHN, 2, 20);
        SendMessage(hScintilla, SCI_SETFOLDFLAGS, SC_FOLDLEVELHEADERFLAG, 0);
        
        // 设置行号
        SendMessage(hScintilla, SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
        SendMessage(hScintilla, SCI_SETMARGINWIDTHN, 0, 40);
        
        // 设置缩进
        SendMessage(hScintilla, SCI_SETTABWIDTH, 4, 0);
        SendMessage(hScintilla, SCI_SETINDENT, 4, 0);
        SendMessage(hScintilla, SCI_SETUSETABS, 0, 0);
        
        // 设置自动缩进
        SendMessage(hScintilla, SCI_SETTABINDENTS, 1, 0);
        SendMessage(hScintilla, SCI_SETBACKSPACEUNINDENTS, 1, 0);
        
        // 设置括号匹配
        SendMessage(hScintilla, SCI_BRACEHIGHLIGHT, 1, 0);
        
        // 设置语法高亮颜色
        // 注释
        SendMessage(hScintilla, SCI_STYLESETFORE, SCE_C_COMMENT, 0x008000);
        // 字符串
        SendMessage(hScintilla, SCI_STYLESETFORE, SCE_C_STRING, 0x800080);
        // 关键字
        SendMessage(hScintilla, SCI_STYLESETFORE, SCE_C_WORD, 0x0000FF);
        // 数字
        SendMessage(hScintilla, SCI_STYLESETFORE, SCE_C_NUMBER, 0xFF8000);
        // 操作符
        SendMessage(hScintilla, SCI_STYLESETFORE, SCE_C_OPERATOR, 0x000000);
    }
}

void Editor::updateScintillaContent() {
    if (hScintilla) {
        // 清空内容
        SendMessage(hScintilla, SCI_CLEARALL, 0, 0);
        // 设置内容
        SendMessage(hScintilla, SCI_SETTEXT, 0, (LPARAM)code.c_str());
    }
}
#endif

void Editor::render() {
#ifdef _WIN32
    // 初始化 Scintilla
    initializeScintilla();
    
    // 获取 ImGui 窗口的位置和大小
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 size = ImGui::GetContentRegionAvail();
    
    if (hScintilla && size.x > 0 && size.y > 0) {
        // 设置 Scintilla 窗口的位置和大小
        MoveWindow(hScintilla, (int)pos.x, (int)pos.y, (int)size.x, (int)size.y, TRUE);
        
        // 显示 Scintilla 窗口
        ShowWindow(hScintilla, SW_SHOW);
        
        // 确保 Scintilla 窗口在 ImGui 绘制内容下方
        SetWindowPos(hScintilla, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        
        // 处理 Scintilla 事件
        MSG msg;
        while (PeekMessage(&msg, hScintilla, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        // 从 Scintilla 获取内容
        int length = SendMessage(hScintilla, SCI_GETLENGTH, 0, 0);
        if (length > 0) {
            char* buffer = new char[length + 1];
            SendMessage(hScintilla, SCI_GETTEXT, length + 1, (LPARAM)buffer);
            std::string newCode(buffer);
            delete[] buffer;
            
            if (newCode != code) {
                code = newCode;
                isModified = true;
            }
        }
        
        // 确保 Scintilla 窗口在 ImGui 绘制内容下方
        SetWindowPos(hScintilla, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    } else {
        // 后备方案：使用 ImGui 的文本编辑功能
        static char buffer[16384] = {0}; // 固定大小的缓冲区
        // 复制代码到缓冲区
        strncpy(buffer, code.c_str(), sizeof(buffer) - 1);
        // 确保缓冲区零终止
        buffer[sizeof(buffer) - 1] = '\0';
        
        if (ImGui::InputTextMultiline("##codeEditor", buffer, sizeof(buffer), 
            ImVec2(-1, -1), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_NoHorizontalScroll)) {
            // 更新代码
            code = buffer;
            isModified = true;
        }
    }
#else
    // 非 Windows 平台使用 ImGui 的文本编辑功能
    static char buffer[16384] = {0}; // 固定大小的缓冲区
    // 复制代码到缓冲区
    strncpy(buffer, code.c_str(), sizeof(buffer) - 1);
    // 确保缓冲区零终止
    buffer[sizeof(buffer) - 1] = '\0';
    
    if (ImGui::InputTextMultiline("##codeEditor", buffer, sizeof(buffer), 
        ImVec2(-1, -1), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_NoHorizontalScroll)) {
        // 更新代码
        code = buffer;
        isModified = true;
    }
#endif
    
    // 显示当前文件路径
    if (!currentFilePath.empty()) {
        ImGui::Text("Current file: %s", currentFilePath.c_str());
    } else {
        ImGui::Text("Untitled");
    }
}

void Editor::newFile() {
    code = "";
    currentFilePath = "";
    isModified = false;
#ifdef _WIN32
    updateScintillaContent();
#endif
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
#ifdef _WIN32
        updateScintillaContent();
#endif
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
#ifdef _WIN32
    if (hScintilla) {
        SendMessage(hScintilla, SCI_UNDO, 0, 0);
    }
#endif
}

void Editor::redo() {
#ifdef _WIN32
    if (hScintilla) {
        SendMessage(hScintilla, SCI_REDO, 0, 0);
    }
#endif
}

void Editor::cut() {
#ifdef _WIN32
    if (hScintilla) {
        SendMessage(hScintilla, SCI_CUT, 0, 0);
    }
#endif
}

void Editor::copy() {
#ifdef _WIN32
    if (hScintilla) {
        SendMessage(hScintilla, SCI_COPY, 0, 0);
    }
#endif
}

void Editor::paste() {
#ifdef _WIN32
    if (hScintilla) {
        SendMessage(hScintilla, SCI_PASTE, 0, 0);
    }
#endif
}

std::string Editor::getCode() const {
    return code;
}
