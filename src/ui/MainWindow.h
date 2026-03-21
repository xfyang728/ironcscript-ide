#pragma once

#include "imgui.h"
#include "core/Editor.h"
#include "core/Engine.h"
#include "core/Terminal.h"
#include "ui/FileBrowser.h"
#include "ui/ProjectManager.h"

class MainWindow {
public:
    MainWindow();
    ~MainWindow();

    void render();

private:
    Editor editor;
    Engine engine;
    Terminal terminal;
    FileBrowser fileBrowser;
    ProjectManager projectManager;

    bool showFileBrowser;
    bool showProjectManager;

    void renderMenuBar();
    void renderStatusBar();
};