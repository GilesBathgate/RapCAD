@echo off

call c:\Qt\Qt5.5.1\5.5\mingw492_32\bin\qtenv2.bat
set PATH=%PATH%;%ProgramFiles%\Git\bin
set PATH=%PATH%;%ProgramFiles%\NSIS
set PATH=%PATH%;%ProgramFiles%\7-Zip
set PATH=%PATH%;c:\Python27

cd c:\rapcad\scripts
bash release-win32.sh
