Name "RapCAD"
Icon "rapcad.ico"
OutFile "rapcad_setup.exe"
InstallDir $PROGRAMFILES\RapCAD
DirText "This will install RapCAD on your computer. Choose a directory"
Section "install"
SetOutPath $INSTDIR
File release\rapcad.exe
File release\Qt5Core.dll
File release\Qt5Gui.dll
File release\Qt5OpenGL.dll
File release\Qt5Widgets.dll
File release\libCGAL.dll
File release\libCGAL_Core.dll
File release\libgcc_s_dw2-1.dll
File release\libgmp-10.dll
File release\libmpfr-4.dll
File release\libstdc++-6.dll
File release\libwinpthread-1.dll
File release\user_guide.html
File /r release\platforms
CreateShortCut $SMPROGRAMS\RapCAD.lnk $INSTDIR\rapcad.exe
WriteUninstaller $INSTDIR\Uninstall.exe
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\RapCAD" "DisplayName" "RapCAD (remove only)"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\RapCAD" "UninstallString" "$INSTDIR\Uninstall.exe"
SectionEnd
Section "Uninstall"
Delete $INSTDIR\Uninstall.exe
Delete $INSTDIR\rapcad.exe
Delete $INSTDIR\Qt5Core.dll
Delete $INSTDIR\Qt5Gui.dll
Delete $INSTDIR\Qt5OpenGL.dll
Delete $INSTDIR\Qt5Widgets.dll
Delete $INSTDIR\libCGAL.dll
Delete $INSTDIR\libCGAL_Core.dll
Delete $INSTDIR\libgcc_s_dw2-1.dll
Delete $INSTDIR\libgmp-10.dll
Delete $INSTDIR\libmpfr-4.dll
Delete $INSTDIR\libstdc++-6.dll
Delete $INSTDIR\libwinpthread-1.dll
Delete $INSTDIR\user_guide.html
Delete $INSTDIR\platforms\qwindows.dll
RMDir  $INSTDIR\platforms
Delete $SMPROGRAMS\RapCAD.lnk
DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\RapCAD"
RMDir $INSTDIR
SectionEnd 
