Name "RapCAD"
Icon "rapcad.ico"
UninstallIcon "rapcad.ico"
OutFile "rapcad_setup.exe"
InstallDir $PROGRAMFILES64\RapCAD
DirText "This will install RapCAD on your computer. Choose a directory"
Section "install"
SetOutPath $INSTDIR
File ${BUILDDIR}\rapcad.exe
File ${BUILDDIR}\Qt6Core.dll
File ${BUILDDIR}\Qt6Gui.dll
File ${BUILDDIR}\Qt6OpenGL.dll
File ${BUILDDIR}\Qt6Widgets.dll
File ${BUILDDIR}\Qt6Concurrent.dll
File ${BUILDDIR}\Qt6OpenGLWidgets.dll
File ${BUILDDIR}\libgcc_s_seh-1.dll
File ${BUILDDIR}\libgmp-10.dll
File ${BUILDDIR}\libmpfr-4.dll
File ${BUILDDIR}\libstdc++-6.dll
File ${BUILDDIR}\libwinpthread-1.dll
File ${BUILDDIR}\opengl32sw.dll
File ${BUILDDIR}\libgit2.dll
File ${BUILDDIR}\user_guide.html
File /r ${BUILDDIR}\platforms
File /r ${BUILDDIR}\styles
CreateShortCut $SMPROGRAMS\RapCAD.lnk $INSTDIR\rapcad.exe
WriteUninstaller $INSTDIR\Uninstall.exe
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\RapCAD" "DisplayName" "RapCAD (remove only)"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\RapCAD" "UninstallString" "$INSTDIR\Uninstall.exe"
SectionEnd
Section "Uninstall"
Delete $INSTDIR\Uninstall.exe
Delete $INSTDIR\rapcad.exe
Delete $INSTDIR\Qt6Core.dll
Delete $INSTDIR\Qt6Gui.dll
Delete $INSTDIR\Qt6OpenGL.dll
Delete $INSTDIR\Qt6Widgets.dll
Delete $INSTDIR\Qt6Concurrent.dll
Delete $INSTDIR\Qt6OpenGLWidgets.dll
Delete $INSTDIR\libgcc_s_seh-1.dll
Delete $INSTDIR\libgmp-10.dll
Delete $INSTDIR\libmpfr-4.dll
Delete $INSTDIR\libstdc++-6.dll
Delete $INSTDIR\libwinpthread-1.dll
Delete $INSTDIR\opengl32sw.dll
Delete $INSTDIR\libgit2.dll
Delete $INSTDIR\user_guide.html
Delete $INSTDIR\platforms\qwindows.dll
RMDir  $INSTDIR\platforms
Delete $INSTDIR\styles\qwindowsvistastyle.dll
RMDir  $INSTDIR\styles
Delete $SMPROGRAMS\RapCAD.lnk
DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\RapCAD"
RMDir $INSTDIR
SectionEnd 
