#pragma once

#include <string>

#ifdef _WIN32
#include <windows.h>
#include "Scintilla.h"
#include "SciLexer.h"
#endif

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
    std::string code;
    std::string currentFilePath;
    bool isModified;

#ifdef _WIN32
    HWND hScintilla;
    bool isScintillaInitialized;

    void initializeScintilla();
    void updateScintillaContent();
    void configureScintilla();
#endif
};