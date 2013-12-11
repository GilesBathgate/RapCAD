@echo off

call c:\Qt\Qt5.1.1\5.1.1\mingw48_32\bin\qtenv2.bat
set PATH=%PATH%;%ProgramFiles%\Git\bin
set PATH=%PATH%;%ProgramFiles%\NSIS

cd c:\rapcad
bash release-win32.sh
