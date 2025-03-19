# 📝 mineNotePad 使用指南

**mineNotePad** 是一个个人学习实践项目，旨在实现一个简洁但功能丰富的记事本应用程序。

---

## 🌟 功能特性

✅ **自定义标题栏**：实现窗口最大化、最小化等功能，提升用户体验。<br>
✅ **多标签页管理**：支持新建标签页，选项卡点击切换，选项卡水平展示，方便用户在多个文档间快速切换。<br>
✅ **文件操作**：提供保存功能，确保用户的编辑内容不会丢失。<br>
✅ **标签页关闭提示**：允许关闭标签页，关闭时提示保存相应文件，防止数据丢失。<br>
✅ **状态栏实时信息展示**：在状态栏实时展示光标位置、文本缩放比例等信息。<br>
✅ **应用程序图标设置**：支持自定义应用程序图标，增强应用辨识度。<br>
✅ **主题设置**：提供多种主题设置，用户可以根据个人喜好定制编辑器的外观。<br>

---

## 🔧 项目环境配置

本项目基于 **Qt 6** 框架，采用 **C++17** 进行开发。

📂 **核心模块**：
- `core`（核心模块）
- `gui`（图形用户界面模块）
- `printsupport`（打印支持模块）
- `widgets`（小部件模块）

📂 **关键文件**：
- **源文件**：`main.cpp`, `mainwindow.cpp`
- **头文件**：`mainwindow.h`
- **UI 文件**：`mainwindow.ui`
- **资源文件**：`applicationIcon.qrc`

---

## 🚀 安装与运行

### 🔹 1. 使用 WinQt 进行依赖收集

1️⃣ **打开 Qt 终端** (Qt Creator 提供的 `Qt 6.8.2 (MinGW 64-bit)` 命令行)。

2️⃣ **进入可执行文件所在目录**：
```sh
cd D:\Documents\mineNotePad\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release
```

3️⃣ **运行 windeployqt 收集依赖**：
```sh
windeployqt mineNotePad.exe --release 
```

📌 **windeployqt** 会自动检测并复制所有 Qt 依赖项，如：
- `Qt6Core.dll`
- `Qt6Gui.dll`
- `Qt6Widgets.dll`
- `Qt6Network.dll`
- `Qt6PrintSupport.dll`
- `Qt6Svg.dll`
- `platforms/qwindows.dll`
- `imageformats/qjpeg.dll`, `qsvg.dll`
- `styles/qmodernwindowsstyle.dll`

🔹 **确保 `Icon` 文件夹包含所有图标文件！**

### 🔹 2. 运行安装程序

✅ **双击运行** `mineNotePad_Installer.exe`，按照向导安装。<br>
✅ **默认安装路径**：`C:\Program Files\mineNotePad` 可选择。<br>
✅ **安装后，系统会自动创建快捷方式**<br>

📌 **如遇权限问题，请以管理员身份运行安装程序！**

---

## 🗑️ 卸载指南

### 🔹 1. 关闭 `mineNotePad`

- 卸载前，程序会自动尝试关闭 `mineNotePad.exe`。
- 若无法关闭，请手动结束 `mineNotePad.exe` 进程。

### 🔹 2. 删除相关文件

- 删除 `mineNotePad.exe` 及 **所有 Qt 依赖库**。
- 移除 `platforms`、`imageformats` 和 `styles` 目录。
- **删除 Icon 目录**（包含所有图标文件）。

### 🔹 3. 移除快捷方式

- 删除 **桌面** 和 **开始菜单** 快捷方式。

### 🔹 4. 清理注册表

- 从 Windows **注册表** 中移除 `mineNotePad` 相关信息，避免残留数据。

📌 **卸载完成后，可手动检查 `C:\Program Files\mineNotePad` 是否仍存在文件！**

---

## 🔧 常见问题 & 解决方案

### ❓ 安装后运行时没有显示应用图标？

✅ **可能原因**：Windows 图标缓存未更新。

🔹 **解决方案**：
1. **尝试重启电脑** 以刷新系统缓存。
2. **手动清理图标缓存**（运行 `ie4uinit.exe -show` 或删除 `%localappdata%\IconCache.db` 后重启）。

### ❓ 卸载后仍然有文件残留？

✅ **可能原因**：`mineNotePad` 仍在后台运行。

🔹 **解决方案**：
1. 打开任务管理器，查找并结束 `mineNotePad.exe` 进程。
2. 手动删除 `C:\Program Files\mineNotePad` 目录。

---

🎉 **恭喜！您已成功安装 mineNotePad！**

如果您在使用过程中遇到任何问题，请随时反馈，我们将尽快提供帮助！ 🚀

