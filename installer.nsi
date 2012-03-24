Name "RapCAD"
OutFile "rapcad_setup.exe"
InstallDir $PROGRAMFILES\RapCAD
DirText "This will install RapCAD on your computer. Choose a directory"
Section "install"
SetOutPath $INSTDIR
File release\rapcad.exe
File release\libboost_thread-mgw46-mt-1_49.dll
File release\libgcc_s_dw2-1.dll
File release\libstdc++-6.dll
File release\mingwm10.dll
File release\QtCore4.dll
File release\QtGui4.dll
File release\QtOpenGL4.dll
File release\libCGAL.dll
File release\libCGAL_Core.dll
File release\libmpfr-4.dll
File release\libgmp-10.dll
CreateShortCut $SMPROGRAMS\RapCAD.lnk $INSTDIR\rapcad.exe
WriteUninstaller $INSTDIR\Uninstall.exe
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\RapCAD" "DisplayName" "RapCAD (remove only)"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\RapCAD" "UninstallString" "$INSTDIR\Uninstall.exe"
SectionEnd
Section "Uninstall"
Delete $INSTDIR\Uninstall.exe
Delete $INSTDIR\rapcad.exe
Delete $INSTDIR\libboost_thread-mgw46-mt-1_49.dll
Delete $INSTDIR\libgcc_s_dw2-1.dll
Delete $INSTDIR\libstdc++-6.dll
Delete $INSTDIR\mingwm10.dll
Delete $INSTDIR\QtCore4.dll
Delete $INSTDIR\QtGui4.dll
Delete $INSTDIR\QtOpenGL4.dll
Delete $INSTDIR\libCGAL.dll
Delete $INSTDIR\libCGAL_Core.dll
Delete $INSTDIR\libmpfr-4.dll
Delete $INSTDIR\libgmp-10.dll
Delete $SMPROGRAMS\RapCAD.lnk
DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\RapCAD"
RMDir $INSTDIR
SectionEnd 
