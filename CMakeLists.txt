cmake_minimum_required(VERSION 3.8)
project(IronCScriptIDE)

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

# 集成 Scintilla（已移除，使用 ImGui 内置编辑器）

# 集成 IronCScript (从 IronCScript-core)
set(IronCScript_CORE_DIR "${CMAKE_SOURCE_DIR}/../IronCScript-core")
add_subdirectory(${IronCScript_CORE_DIR} IronCScript)

# 包含头文件目录
include_directories(
    src
    extern/imgui
    extern/imgui/backends
    extern/glfw/include
    ${IronCScript_CORE_DIR}/src
)

# 创建可执行文件
add_executable(IronCScriptIDE ${SOURCES} ${HEADERS})

# 链接库
target_link_libraries(IronCScriptIDE
    imgui
    glfw
    IronCScriptCoreLib
)

# 平台特定设置
if(WIN32)
    target_link_libraries(IronCScriptIDE opengl32 user32 gdi32 comctl32 ole32 oleaut32 uuid imm32)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(IronCScriptIDE GL)
elseif(APPLE)
    target_link_libraries(IronCScriptIDE "-framework OpenGL")
endif()