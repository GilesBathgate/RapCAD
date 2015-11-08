@echo off

call c:\Qt\Qt5.5.1\5.5\mingw492_32\bin\qtenv2.bat
set PATH=%PATH%;c:\git\bin
set PATH=%PATH%;c:\source-highlight\bin
set PATH=%PATH%;%ProgramFiles%\NSIS
set PATH=%PATH%;c:\7zip
set PATH=%PATH%;c:\Python27

cd c:\rapcad\scripts
bash release-win32.sh
