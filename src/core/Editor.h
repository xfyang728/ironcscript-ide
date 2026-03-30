#pragma once

#include <string>
#include <vector>
#include <map>
#include <imgui.h>

class Editor {
public:
    Editor();
    ~Editor();

    void render();
    void newFile();
    void openFile(const std::string& path);
    void saveFile();
    void saveFileAs();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    std::string getCode() const;

private:
    // 辅助方法
    void initThemes();
    void updateLines();
    void renderToolbar();
    void renderStatusBar();
    static int textEditCallback(ImGuiInputTextCallbackData* data);
    void renderSyntaxHighlighting();
    void renderCodeCompletion();
    void renderSearchReplace();
    void formatCode();
    
    // 语法高亮
    void highlightSyntax();
    std::string highlightLine(const std::string& line);
    
    // 代码补全
    void generateCompletions();
    
    // 错误检查
    void checkSyntaxErrors();
    
    // 编辑器状态
    std::string code;
    std::string currentFilePath;
    bool isModified;
    int cursorPos;
    int selectionStart;
    int selectionEnd;
    int currentLine;
    int currentColumn;
    
    // 编辑器配置
    bool lineNumbers;
    bool syntaxHighlighting;
    bool autoIndent;
    bool codeFolding;
    bool showSearchReplace;
    int tabSize;
    std::string theme;
    
    // 代码行
    std::vector<std::string> lines;
    
    // 语法高亮数据
    std::vector<std::vector<std::pair<int, int>>> lineHighlights; // 每行的高亮范围和类型
    
    // 代码补全数据
    std::vector<std::string> completions;
    bool showCompletions;
    int completionIndex;
    
    // 搜索替换数据
    std::string searchText;
    std::string replaceText;
    bool caseSensitiveSearch;
    bool wholeWordSearch;
    
    // 错误检查数据
    std::vector<std::pair<int, std::string>> syntaxErrors; // 行号和错误信息
    
    // 历史记录
    std::vector<std::string> undoStack;
    std::vector<std::string> redoStack;
    int undoIndex;
    
    // 主题数据
    struct Theme {
        ImVec4 background;
        ImVec4 text;
        ImVec4 keyword;
        ImVec4 string;
        ImVec4 comment;
        ImVec4 number;
        ImVec4 op;
        ImVec4 error;
    };
    std::map<std::string, Theme> themes;
    Theme currentTheme;
    
    // 多光标支持
    std::vector<std::pair<int, int>> multiCursors; // 多个光标位置
};