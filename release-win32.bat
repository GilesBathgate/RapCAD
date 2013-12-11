@echo off

call c:\Qt\4.8.0\bin\qtvars.bat
set PATH=%PATH%;c:\MinGW\msys\1.0\bin
set PATH=%PATH%;%ProgramFiles%\Git\bin
set PATH=%PATH%;%ProgramFiles%\NSIS

cd c:\rapcad
bash release-win32.sh
