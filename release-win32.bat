@echo off

set PATH=%PATH%;%ProgramFiles%\NSIS

qmake CONFIG+=official && ^
make -f Makefile.Release && ^
make clean && ^
copy installer.nsi release && ^
copy ..\rapcad-dlls\* release && ^
pushd release && ^
makensis.exe installer.nsi && ^
popd
