cmake_minimum_required(VERSION 3.8)
project(CScriptEngineIDE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# 添加源文件
set(SOURCES
    src/main.cpp
    src/core/Editor.cpp
    src/core/Engine.cpp
    src/core/Terminal.cpp
    src/ui/MainWindow.cpp
    src/ui/FileBrowser.cpp
    src/ui/ProjectManager.cpp
    src/platform/Platform.cpp
    src/platform/K210Support.cpp
)

# 添加头文件
set(HEADERS
    src/core/Editor.h
    src/core/Engine.h
    src/core/Terminal.h
    src/ui/MainWindow.h
    src/ui/FileBrowser.h
    src/ui/ProjectManager.h
    src/platform/Platform.h
    src/platform/K210Support.h
)

# 集成 ImGui
add_subdirectory(extern/imgui)

# 集成 GLFW
add_subdirectory(extern/glfw)

# 集成 Scintilla
add_subdirectory(extern/scintilla)

# 集成 CScriptEngine
# add_subdirectory(extern/CScriptEngine)

# 包含头文件目录
include_directories(
    src
    extern/imgui
    extern/imgui/backends
    extern/glfw/include
    extern/scintilla/include
    extern/scintilla/src
    extern/scintilla/win32
    extern/lexilla/include
    extern/CScriptEngine/src
)

# 创建可执行文件
add_executable(CScriptEngineIDE ${SOURCES} ${HEADERS})

# 链接库
target_link_libraries(CScriptEngineIDE
    imgui
    glfw
    scintilla
    # CScriptEngineLib
)

# 平台特定设置
if(WIN32)
    target_link_libraries(CScriptEngineIDE opengl32 user32 gdi32 comctl32 ole32 oleaut32 uuid imm32)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(CScriptEngineIDE GL)
elseif(APPLE)
    target_link_libraries(CScriptEngineIDE "-framework OpenGL")
endif()