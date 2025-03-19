### 定义安装包名称和输出路径
Outfile "mineNotePad_Installer.exe"

### 定义安装目录（默认 C:\\Program Files\\mineNotePad）
InstallDir "$PROGRAMFILES\mineNotePad"

### 获取默认安装目录（如果用户已安装过）
InstallDirRegKey HKLM "Software\mineNotePad" "Install_Dir"

### 设置安装页面（许可协议、组件选择等）
Page directory
Page instfiles

### 定义安装段
Section "Install"

    # 创建安装目录
    SetOutPath "$INSTDIR"

    # 复制可执行文件
    File "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\mineNotePad.exe"
    
    # 复制 Qt 依赖库
    File "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\Qt6Core.dll"
    File "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\Qt6Gui.dll"
    File "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\Qt6Widgets.dll"
    File "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\Qt6Network.dll"
    File "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\Qt6PrintSupport.dll"
    File "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\Qt6Svg.dll"
	File "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\libgcc_s_seh-1.dll"
	File "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\libstdc++-6.dll"
	File "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\libwinpthread-1.dll"
	File "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\opengl32sw.dll"
	File "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\dxcompiler.dll"
	File "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\D3Dcompiler_47.dll"
	
	
    # 复制 Qt 插件（如 imageformats, platforms, styles 等）
    SetOutPath "$INSTDIR\platforms"
    File "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\platforms\qwindows.dll"
    
    SetOutPath "$INSTDIR\imageformats"
    File "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\imageformats\qjpeg.dll"
    File "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\imageformats\qsvg.dll"
    
    SetOutPath "$INSTDIR\styles"
    File "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\styles\qmodernwindowsstyle.dll"

    #复制 Icon 文件夹（包含所有图标）
    SetOutPath "$INSTDIR\Icon"
    ##File /r "D:\Documents\STUDY_4\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release\Icon\*.*"
	
    # 注册安装路径到注册表（方便卸载程序使用）
    WriteRegStr HKLM "Software\mineNotePad" "Install_Dir" "$INSTDIR"
    
    # 创建桌面快捷方式
    CreateShortCut "$DESKTOP\mineNotePad.lnk" "$INSTDIR\mineNotePad.exe"
    
    # 创建开始菜单快捷方式
    CreateDirectory "$SMPROGRAMS\mineNotePad"
    CreateShortCut "$SMPROGRAMS\mineNotePad\mineNotePad.lnk" "$INSTDIR\mineNotePad.exe"
    
    # 生成卸载程序
    WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

### 定义卸载段
Section "Uninstall"

    # 1. 关闭运行中的 mineNotePad.exe（如果正在运行）
    ExecWait 'taskkill /F /IM mineNotePad.exe /T'  # 强制关闭进程及子进程
    Sleep 1000  # 等待 1 秒，确保进程被彻底关闭

    # 2. 再次检查进程是否还在运行（有时 taskkill 可能失败）
    System::Call 'kernel32::OpenProcess(i 0x400, i 0, i $INSTDIR\mineNotePad.exe) i .r0'
    StrCmp $0 0 done  # 如果进程不存在，继续删除

    # 3. 如果进程仍在运行，则提示用户手动关闭
    MessageBox MB_OK "mineNotePad 仍在运行，请先关闭程序后再继续卸载。" 
    Abort  # 终止卸载

done:
    # 4. 删除 exe 文件
    Delete "$INSTDIR\mineNotePad.exe"

    # 5. 删除其他文件
    Delete "$INSTDIR\Qt6Core.dll"
    Delete "$INSTDIR\Qt6Gui.dll"
    Delete "$INSTDIR\Qt6Widgets.dll"
    Delete "$INSTDIR\Qt6Network.dll"
    Delete "$INSTDIR\Qt6PrintSupport.dll"
    Delete "$INSTDIR\Qt6Svg.dll"
    Delete "$INSTDIR\libgcc_s_seh-1.dll"
    Delete "$INSTDIR\libstdc++-6.dll"
    Delete "$INSTDIR\libwinpthread-1.dll"
    Delete "$INSTDIR\opengl32sw.dll"
    Delete "$INSTDIR\dxcompiler.dll"
    Delete "$INSTDIR\D3Dcompiler_47.dll"

    # 6. 删除 Qt 插件目录
    RMDir /r "$INSTDIR\platforms"
    RMDir /r "$INSTDIR\imageformats"
    RMDir /r "$INSTDIR\styles"
	### 删除 Icon 目录（包含所有图标文件）
    RMDir /r "$INSTDIR\Icon"

    # 7. 删除卸载程序
    Delete "$INSTDIR\uninstall.exe"
	
	RMDir /r "$INSTDIR\Icon"

    # 8. 删除主目录（确保为空）
    RMDir "$INSTDIR"

    # 9. 删除快捷方式
    Delete "$DESKTOP\mineNotePad.lnk"
    Delete "$SMPROGRAMS\mineNotePad\mineNotePad.lnk"
	

    RMDir "$SMPROGRAMS\mineNotePad"
	

    # 10. 删除注册表信息
    DeleteRegKey HKCU "Software\mineNotePad"

SectionEnd

