# IronCScript IDE

IronCScript IDE 是一个专为 IronCScript 脚本引擎设计的轻量级集成开发环境，提供代码编辑、执行、调试等功能，帮助用户更高效地开发和测试 IronCScript 脚本。

## 技术特点

- **轻量级**：基于 ImGui 和 C++，启动快速，内存占用低
- **高性能**：即时模式 GUI，响应流畅
- **跨平台**：支持 Windows、macOS、Linux
- **功能完整**：代码编辑、脚本执行、项目管理、K210 支持

## 项目结构

```
IronCScript-IDE/
├── src/                  # 源代码
│   ├── core/             # 核心功能
│   │   ├── Editor.cpp    # 代码编辑器
│   │   ├── Engine.cpp    # IronCScript 集成
│   │   └── Terminal.cpp  # 终端
│   ├── ui/               # 界面组件
│   │   ├── MainWindow.cpp      # 主窗口
│   │   ├── FileBrowser.cpp     # 文件浏览器
│   │   └── ProjectManager.cpp  # 项目管理器
│   ├── platform/         # 平台抽象
│   │   ├── Platform.cpp      # 平台接口
│   │   └── K210Support.cpp    # K210 支持
│   └── main.cpp          # 主入口
├── extern/               # 第三方依赖
│   ├── imgui/            # ImGui 库
│   ├── glfw/             # GLFW 库
│   └── scintilla/        # Scintilla 编辑器
├── CMakeLists.txt        # CMake 配置
└── README.md             # 项目说明
```

## 构建方法

### 前置条件

- CMake 3.8 或更高版本
- C++17 兼容的编译器
- OpenGL 3.3 或更高版本

### 构建步骤

1. **克隆仓库**

   ```bash
   git clone <repository-url>
   cd IronCScript-IDE
   ```

2. **初始化子模块**

   ```bash
   git submodule update --init --recursive
   ```

3. **配置 CMake**

   ```bash
   mkdir build
   cd build
   cmake ..
   ```

4. **编译项目**

   ```bash
   cmake --build .
   ```

5. **运行 IDE**

   ```bash
   ./IronCScriptIDE
   ```

## 使用方法

1. **代码编辑**：在左侧编辑器中编写 C 脚本代码
2. **执行脚本**：点击 "Run" -> "Run Script" 执行脚本
3. **查看结果**：在右侧终端中查看执行结果
4. **文件管理**：使用 "File" 菜单打开和保存文件
5. **项目管理**：使用 "View" -> "Project Manager" 管理项目
6. **K210 支持**：使用平台工具编译和传输到 K210 设备

## 功能特性

- **代码编辑器**：支持 C 语言语法高亮、基本代码补全
- **脚本执行**：执行 IronCScript 脚本并显示结果
- **终端**：实时显示执行结果，支持基本命令
- **文件浏览器**：浏览和选择文件
- **项目管理**：管理项目文件和配置
- **K210 支持**：编译和传输脚本到 K210 设备

## 许可证

本项目采用 MIT 许可证，详见 LICENSE 文件。