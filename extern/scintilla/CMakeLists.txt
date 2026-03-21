cmake_minimum_required(VERSION 3.8)
project(scintilla)

set(SCINTILLA_SOURCES
    src/AutoComplete.cxx
    src/CallTip.cxx
    src/CaseConvert.cxx
    src/CaseFolder.cxx
    src/CellBuffer.cxx
    src/ChangeHistory.cxx
    src/CharClassify.cxx
    src/CharacterCategoryMap.cxx
    src/CharacterType.cxx
    src/ContractionState.cxx
    src/DBCS.cxx
    src/Decoration.cxx
    src/Document.cxx
    src/EditModel.cxx
    src/EditView.cxx
    src/Editor.cxx
    src/Geometry.cxx
    src/Indicator.cxx
    src/KeyMap.cxx
    src/LineMarker.cxx
    src/MarginView.cxx
    src/PerLine.cxx
    src/PositionCache.cxx
    src/RESearch.cxx
    src/RunStyles.cxx
    src/ScintillaBase.cxx
    src/Selection.cxx
    src/Style.cxx
    src/UndoHistory.cxx
    src/UniConversion.cxx
    src/UniqueString.cxx
    src/ViewStyle.cxx
    src/XPM.cxx
)

# 根据平台添加相应的源文件
if(WIN32)
    list(APPEND SCINTILLA_SOURCES
        win32/PlatWin.cxx
        win32/ScintillaWin.cxx
        win32/ScintillaDLL.cxx
        win32/HanjaDic.cxx
        win32/ListBox.cxx
        win32/SurfaceGDI.cxx
        win32/SurfaceD2D.cxx
    )
elseif(APPLE)
    list(APPEND SCINTILLA_SOURCES
        cocoa/PlatCocoa.mm
        cocoa/ScintillaCocoa.mm
        cocoa/ScintillaView.mm
        cocoa/InfoBar.mm
    )
elseif(UNIX AND NOT APPLE)
    list(APPEND SCINTILLA_SOURCES
        gtk/PlatGTK.cxx
        gtk/ScintillaGTK.cxx
    )
endif()

# 包含头文件目录
include_directories(
    include
    src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 创建静态库
add_library(scintilla STATIC ${SCINTILLA_SOURCES})

# 平台特定设置
if(WIN32)
    target_link_libraries(scintilla user32 gdi32 comctl32)
elseif(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(CORE_FOUNDATION_LIBRARY CoreFoundation)
    target_link_libraries(scintilla ${COCOA_LIBRARY} ${CORE_FOUNDATION_LIBRARY})
elseif(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    include_directories(${GTK3_INCLUDE_DIRS})
    target_link_libraries(scintilla ${GTK3_LIBRARIES})
endif()
