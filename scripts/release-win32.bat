@echo off

call c:\Qt\Qt5.2.0\5.2.0\mingw48_32\bin\qtenv2.bat
set PATH=%PATH%;%ProgramFiles%\Git\bin
set PATH=%PATH%;%ProgramFiles%\NSIS
set PATH=%PATH%;%ProgramFiles%\7-Zip

cd c:\rapcad\scripts
bash release-win32.sh
