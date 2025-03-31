Outfile "mineNotePad_Installer.exe"

InstallDir "$PROGRAMFILES\mineNotePad"

InstallDirRegKey HKLM "Software\mineNotePad" "Install_Dir"

Page directory

Page instfiles

Section "Install"

SetOutPath "$INSTDIR"

File "D:\Documents\mineNotePad\mineNotePad.exe"

File "D:\Documents\mineNotePad\D3Dcompiler_47.dll"

File "D:\Documents\mineNotePad\dxcompiler.dll"

File "D:\Documents\mineNotePad\libgcc_s_seh-1.dll"

File "D:\Documents\mineNotePad\libstdc++-6.dll"

File "D:\Documents\mineNotePad\libwinpthread-1.dll"

File "D:\Documents\mineNotePad\opengl32sw.dll"

File "D:\Documents\mineNotePad\Qt6Core.dll"

File "D:\Documents\mineNotePad\Qt6Gui.dll"

File "D:\Documents\mineNotePad\Qt6Network.dll"

File "D:\Documents\mineNotePad\Qt6PrintSupport.dll"

File "D:\Documents\mineNotePad\Qt6Svg.dll"

File "D:\Documents\mineNotePad\Qt6Widgets.dll"

SetOutPath "$INSTDIR\generic"

File "D:\Documents\mineNotePad\generic\qtuiotouchplugin.dll"

SetOutPath "$INSTDIR\iconengines"

File "D:\Documents\mineNotePad\iconengines\qsvgicon.dll"

SetOutPath "$INSTDIR\imageformats"

File "D:\Documents\mineNotePad\imageformats\qgif.dll"

File "D:\Documents\mineNotePad\imageformats\qico.dll"

File "D:\Documents\mineNotePad\imageformats\qjpeg.dll"

File "D:\Documents\mineNotePad\imageformats\qsvg.dll"

SetOutPath "$INSTDIR\networkinformation"

File "D:\Documents\mineNotePad\networkinformation\qnetworklistmanager.dll"

SetOutPath "$INSTDIR\platforms"

File "D:\Documents\mineNotePad\platforms\qwindows.dll"

SetOutPath "$INSTDIR\styles"

File "D:\Documents\mineNotePad\styles\qmodernwindowsstyle.dll"

SetOutPath "$INSTDIR\tls"

File "D:\Documents\mineNotePad\tls\qcertonlybackend.dll"

File "D:\Documents\mineNotePad\tls\qschannelbackend.dll"

SetOutPath "$INSTDIR\translations"

WriteRegStr HKLM "Software\mineNotePad" "Install_Dir" "$INSTDIR"

CreateShortCut "$DESKTOP\mineNotePad.lnk" "$INSTDIR\mineNotePad.exe"

CreateDirectory "$SMPROGRAMS\mineNotePad"

CreateShortCut "$SMPROGRAMS\mineNotePad\mineNotePad.lnk" "$INSTDIR\mineNotePad.exe"

WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

Section "Uninstall"

ExecWait 'taskkill /F /IM mineNotePad.exe /T'

Sleep 1000

System::Call 'kernel32::OpenProcess(i 0x400, i 0, i $INSTDIR\mineNotePad.exe) i .r0'

StrCmp $0 0 done

MessageBox MB_OK "mineNotePad仍在运行，请先关闭程序后再继续卸载。"

Abort

done:

Delete "$INSTDIR\mineNotePad.exe"

Delete "$INSTDIR\D3Dcompiler_47.dll"

Delete "$INSTDIR\dxcompiler.dll"

Delete "$INSTDIR\libgcc_s_seh-1.dll"

Delete "$INSTDIR\libstdc++-6.dll"

Delete "$INSTDIR\libwinpthread-1.dll"

Delete "$INSTDIR\opengl32sw.dll"

Delete "$INSTDIR\Qt6Core.dll"

Delete "$INSTDIR\Qt6Gui.dll"

Delete "$INSTDIR\Qt6Network.dll"

Delete "$INSTDIR\Qt6PrintSupport.dll"

Delete "$INSTDIR\Qt6Svg.dll"

Delete "$INSTDIR\Qt6Widgets.dll"

RMDir /r "$INSTDIR\generic"

RMDir /r "$INSTDIR\iconengines"

RMDir /r "$INSTDIR\imageformats"

RMDir /r "$INSTDIR\networkinformation"

RMDir /r "$INSTDIR\platforms"

RMDir /r "$INSTDIR\styles"

RMDir /r "$INSTDIR\tls"

RMDir /r "$INSTDIR\translations"

Delete "$INSTDIR\uninstall.exe"

RMDir "$INSTDIR"

Delete "$DESKTOP\mineNotePad.lnk"

Delete "$SMPROGRAMS\mineNotePad\mineNotePad.lnk"

RMDir "$SMPROGRAMS\mineNotePad"

DeleteRegKey HKCU "Software\mineNotePad"

SectionEnd

