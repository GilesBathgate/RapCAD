@echo off

call C:\Qt\4.8.0\bin\qtvars.bat
set PATH=%PATH%;%ProgramFiles%\Git\bin
set PATH=%PATH%;%ProgramFiles%\NSIS

pushd c:\rapcad && ^
git pull && ^
git reset --hard master && ^
git clean -df && ^
qmake CONFIG+=official && ^
make -f Makefile.Release && ^
make clean && ^
copy installer.nsi release && ^
copy ..\rapcad-dlls\* release && ^
pushd release && ^
makensis.exe installer.nsi && ^
popd
popd
