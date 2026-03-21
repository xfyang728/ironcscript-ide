cmake_minimum_required(VERSION 3.8)
project(imgui)

set(IMGUI_SOURCES
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.cpp
    ${GLFW_SOURCES}
)

set(IMGUI_HEADERS
    imgui.h
    imgui_internal.h
    imconfig.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
    backends/imgui_impl_glfw.h
    backends/imgui_impl_opengl3.h
    backends/imgui_impl_opengl3_loader.h
)

add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)

# 包含 GLFW 目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../glfw/include)

# 添加 GLFW 源文件
set(GLFW_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/context.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/input.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/monitor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/vulkan.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/window.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/win32_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/win32_monitor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/win32_window.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/wgl_context.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/egl_context.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/osmesa_context.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/posix_thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/posix_time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/posix_module.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/posix_poll.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/platform.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/src/internal.h
)

# 查找 OpenGL 库
find_package(OpenGL REQUIRED)

# 链接依赖
target_link_libraries(imgui PRIVATE OpenGL::GL)
